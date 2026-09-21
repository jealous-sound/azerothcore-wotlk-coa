/*
 * Spellbook: the Books of Ascension as a real class trainer.
 *
 * Right-clicking a book opens the ordinary trainer window - the one with known, available
 * and unavailable rows and a Train button - and the list is built for whoever opened it, so
 * a book someone else put down trains your class, not theirs.
 *
 * Two things here are deliberate:
 *
 *  * The list is sent by hand instead of through Trainer::SendSpells. The core's version
 *    filters every row through Player::IsSpellFitByClassAndRace, which asks
 *    SkillRaceClassInfo.dbc about the spell's skill line, and that table has no rows for
 *    Ascension's classes (ids 12 and up). Every custom spell is therefore filtered out and
 *    the window arrives empty, which is indistinguishable client side from a book that does
 *    nothing.
 *
 *  * Purchases are answered here too. The core resolves trainers per creature entry
 *    (sObjectMgr->GetTrainer(npc->GetEntry())), which cannot serve a list that depends on who
 *    is looking, and Trainer::TeachSpell runs the same class/race filter that would refuse
 *    every one of these spells.
 *
 * The list itself is what automatic progression would have granted: class progression spells,
 * rank upgrades gated on their first rank, and the automatic talent entries. No ability is
 * invented here, so the book can never offer something the realm would not have given.
 */

#include "Chat.h"
#include "Configuration/Config.h"
#include "Creature.h"
#include "GossipDef.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "SpellMgr.h"
#include "WorldSession.h"

#include <algorithm>
#include <string>
#include <vector>

// The progression tables this module serves live with the compat layer that generated them.
// They are plain generated data headers, so they are read rather than copied.
#include "../../mod-ascension-compat/src/AscensionCoATalentData.h"
#include "../../mod-ascension-compat/src/AscensionCustomClassData.h"
#include "../../mod-ascension-compat/src/AscensionSpellProgressionData.h"
#include "SpellbookCostData.h"
#include "SpellbookOfferData.h"
#include "SpellbookRankData.h"
#include "SpellbookTrainerData.h"
#include "SpellbookTreeSpellData.h"
#include "spellbook_api.h"
#include "spellbook_notify.h"

namespace
{
    /// The script the book templates carry. Kept in one place so the SQL that installs a book
    /// and the code that serves it cannot drift apart silently.
    constexpr char const *BOOK_SCRIPT = "npc_spellbook_trainer";
    constexpr char const *ENABLE_KEY = "Spellbook.Enable";
    constexpr char const *NOTIFY_ON_READY_KEY = "Spellbook.Notify.OnClientReady";
    constexpr char const *COST_ENABLE_KEY = "Spellbook.Cost.Enable";
    constexpr char const *COST_MULTIPLIER_KEY = "Spellbook.Cost.Multiplier";

    /// The client telling the server its extension module has registered its handlers,
    /// which is the first moment the announcement push can be answered at all. The compat
    /// layer answers the same message for the advancement handshake.
    constexpr uint16 CMSG_EXTENSION_INITIALIZED = 0x0561;
    constexpr char const *GREETING = "Hello! Ready for some training?";

    constexpr uint32 BEGINNERS_BOOK = 75118;
    constexpr uint32 BEGINNERS_TEXT = 22;
    constexpr uint32 BOOK_TEXT = 19108;

    /// The client reads the active row state from this byte. A known spell is never sent -
    /// the window drops it - so only the first two are ever written.
    enum class RowState : uint8
    {
        Available = 0,
        Unavailable = 1,
        Known = 2
    };

    enum class BuyResult : uint32
    {
        Unavailable = 0,
        NotEnoughMoney = 1,
        NotEnoughSkill = 2
    };

    struct Row
    {
        uint32 SpellId = 0;
        uint8 RequiredLevel = 0;
        uint32 RequiredAbility = 0;
        uint32 SkillLine = 0;
        uint32 Cost = 0;   // copper, before the multiplier and the reputation discount
        RowState State = RowState::Unavailable;
    };

    bool Enabled()
    {
        return sConfigMgr->GetOption<bool>(ENABLE_KEY, true);
    }

    bool Charging()
    {
        return sConfigMgr->GetOption<bool>(COST_ENABLE_KEY, true);
    }

    /// Copper this row costs this viewer: the band price for the level the window gates the row
    /// at, times the configured multiplier, times the viewer's reputation discount with the
    /// book. Trainer::SendSpells and Trainer::TeachSpell apply the same two steps, so the price
    /// the window shows and the price the Train button charges cannot drift apart.
    int32 RowPrice(Player *player, Creature *book, Row const &row)
    {
        if (!Charging() || !row.Cost)
            return 0;

        float price = float(row.Cost) * sConfigMgr->GetOption<float>(COST_MULTIPLIER_KEY, 1.0f);
        if (book)
            price *= player->GetReputationPriceDiscount(book);

        return int32(price);
    }

    bool IsBook(Creature const *creature)
    {
        return creature && creature->GetScriptName() == BOOK_SCRIPT;
    }

    /// The skill line a spell is filed under, which is what the client groups the window by.
    /// It is a label only: nothing here gates on skill values, because these spells are not
    /// in the DBC tables the core's skill checks consult.
    uint32 SkillLineForSpell(uint32 spellId)
    {
        auto const bounds = sSpellMgr->GetSkillLineAbilityMapBounds(spellId);
        for (auto itr = bounds.first; itr != bounds.second; ++itr)
            if (itr->second->SkillLine)
                return itr->second->SkillLine;

        return 0;
    }

    /// The specialisation the character is currently in, read straight from the setting the
    /// Character Advancement storage writes, so this module needs nothing from the compat
    /// layer.
    uint32 ActiveSpec(Player *player)
    {
        if (!player)
            return 0;

        return player->GetPlayerSetting("core.ascension_active_spec", 0).value;
    }

    /// Whether the character has this ability or a higher rank of the same chain. A rank below
    /// the highest the character holds is not an upgrade and cannot be granted: Player::addSpell
    /// marks it inactive and returns without announcing anything, so the window must show it as
    /// known rather than offer a purchase that does nothing.
    bool HasRankOrBetter(Player *player, uint32 spellId)
    {
        if (player->HasSpell(spellId))
            return true;

        SpellInfo const *info = sSpellMgr->GetSpellInfo(spellId);
        if (!info || !info->IsRanked())
            return false;

        for (SpellInfo const *rank = sSpellMgr->GetSpellInfo(sSpellMgr->GetFirstSpellInChain(spellId));
             rank; rank = rank->GetNextRankSpell())
            if (rank->GetRank() >= info->GetRank() && player->HasSpell(rank->Id))
                return true;

        return false;
    }

    /// Whether the client's talent trees grant this spell. A node's spell enters the
    /// character through the tree: its one rank by spending that node, or by reaching the
    /// level of an automatic node. So the window must not sell it - and since the node data
    /// lists only that entry rank, the upgrades above it stay in the window. See
    /// SpellbookTreeSpellData.h, which the generator derives from the client's own tree data.
    bool IsTreeSpell(uint32 classId, uint32 spellId)
    {
        auto const &table = SpellbookTreeSpellData::TreeSpells;
        std::pair<uint32, uint32> const key(classId, spellId);
        auto const itr = std::lower_bound(table.begin(), table.end(), key,
            [](SpellbookTreeSpellData::TreeSpell const &row, std::pair<uint32, uint32> const &wanted)
            {
                return row.ClassId < wanted.first ||
                       (row.ClassId == wanted.first && row.SpellId < wanted.second);
            });

        return itr != table.end() && itr->ClassId == classId && itr->SpellId == spellId;
    }

    /// The row set behind both the window and the announcement push.
    ///
    /// The window view is what the player is shown, and it is the only view that gets
    /// narrowed: a spell the talent trees grant is left out, and so is anything the character
    /// already holds, so every row on screen is a row the Train button can grant. The
    /// announcement push asks for the union instead - every spell this class can ever be
    /// handed, tree content and already-held ranks included - because its job is to tell the
    /// client which abilities deserve a "New Spell Learned" alert wherever they came from.
    std::vector<Row> BuildRows(Player *player, bool windowView = true)
    {
        std::vector<Row> rows;
        if (!player)
            return rows;

        uint8 const classId = uint8(player->getClass());
        uint32 const spec = ActiveSpec(player);

        auto add = [&rows, classId, windowView](uint32 spellId, uint8 requiredLevel,
                                                uint32 requiredAbility)
        {
            if (!spellId || !sSpellMgr->GetSpellInfo(spellId))
                return;

            // A spell the talent trees grant is the tree's to hand out, whatever source
            // below would otherwise reach it through.
            if (windowView && IsTreeSpell(classId, spellId))
                return;

            for (Row const &known : rows)
                if (known.SpellId == spellId)
                    return;

            Row row;
            row.SpellId = spellId;
            row.RequiredLevel = requiredLevel;
            row.RequiredAbility = requiredAbility;
            row.SkillLine = SkillLineForSpell(spellId);

            // Priced at the level the window gates the row at. A source that carries no level
            // for the spell falls back to Spell.dbc's BaseLevel, the same fallback the rank
            // chains above use, so a row is never free because one source left its level blank.
            uint8 const pricedLevel = requiredLevel ? requiredLevel
                : uint8(std::min<uint32>(sSpellMgr->AssertSpellInfo(spellId)->BaseLevel, 255));
            row.Cost = SpellbookCostData::Price(pricedLevel, spellId);

            rows.push_back(row);
        };

        for (auto const &entry : AscensionCompatData::ClassSpells)
            if (entry.ClassId == classId)
                add(entry.SpellId, entry.RequiredLevel, 0);

        // Ranks keep their first rank as the requirement, which is what lets the window show
        // a rank the character has not started as unavailable instead of missing.
        //
        // The first rank is offered as well: it is the class's base ability, and for 199 of
        // them the generated class data carries no level at all, so without this the window
        // hides whole abilities. Spell.dbc's BaseLevel is where those levels come from - it
        // matches the level the generated data records for 236 of the 242 first ranks it does
        // have, and both first ranks the captured original trainer window was seen selling
        // (Chronomancer's Nozdormu's Wisdom, Pyromancer's Seal of Alysrazor) are level 1 in the
        // capture and BaseLevel 1 in the client data.
        for (auto const &entry : AscensionProgression::Ranks)
        {
            if (entry.ClassId != classId)
                continue;

            if (SpellInfo const *first = sSpellMgr->GetSpellInfo(entry.FirstSpellId))
                if (first->BaseLevel >= 1 && first->BaseLevel <= MAX_LEVEL)
                    add(entry.FirstSpellId, uint8(first->BaseLevel), 0);

            add(entry.SpellId, entry.RequiredLevel, entry.FirstSpellId);
        }

        for (auto const &entry : AscensionCompatData::CoATalentEntries)
            if (entry.ClassId == classId && !entry.AECost && !entry.TECost &&
                (!entry.SpecId || !spec || entry.SpecId == spec))
                for (uint32 spellId : entry.SpellIds)
                    add(spellId, entry.RequiredLevel, 0);

        // Services the original trainer window sold but this realm's generated class data
        // never resolved to a level. Without them the window is missing whole rank chains for
        // some classes: Stormbringer's Torrential Wrath and Brine, the Guardian Ballads,
        // Pyromancer's Echo of Nozdormu, the Ranger's Bushcraft kit. See SpellbookOfferData.h.
        for (SpellbookOfferData::Offer const &offer : SpellbookOfferData::Offers)
            if (offer.ClassId == classId)
                add(offer.SpellId, offer.RequiredLevel, offer.FirstSpellId);

        // Class trainer rows the realm's own NPCTrainer.dbc teaches under this class's tab
        // name, that no other source sells: the top ranks of ladders the captured window only
        // showed part of (Devotion of Grace rank 8, Chromie's Wisdom 5, the Ranger's Bushcraft
        // tiers, the Venomancer's fourth form). See SpellbookTrainerData.h.
        for (SpellbookTrainerData::Offer const &offer : SpellbookTrainerData::Offers)
            if (offer.ClassId == classId)
                add(offer.SpellId, offer.RequiredLevel, offer.FirstSpellId);

        // Upgrades of abilities this window already sells that the realm's generated
        // progression data never resolved - the client's own rank ladders carry them, and
        // without them the last ranks of those abilities were unreachable from any book.
        // See SpellbookRankData.h. The requirement is the highest rank this window offers
        // below them, so an upgrade still needs the rank under it.
        for (SpellbookRankData::Rank const &rank : SpellbookRankData::Ranks)
            if (rank.ClassId == classId)
                add(rank.SpellId, rank.RequiredLevel, rank.RequiredSpellId);

        std::vector<Row> view;
        view.reserve(rows.size());
        for (Row &row : rows)
        {
            // Already held, in this rank or a higher one: the book has nothing to teach here,
            // and a row that cannot be bought is worse than no row at all.
            if (windowView && HasRankOrBetter(player, row.SpellId))
                continue;

            if (player->GetLevel() < row.RequiredLevel)
                row.State = RowState::Unavailable;
            else if (row.RequiredAbility && !player->HasSpell(row.RequiredAbility))
                row.State = RowState::Unavailable;
            else
                row.State = RowState::Available;

            view.push_back(row);
        }

        return view;
    }

    /// Tells the client every spell this player's book can offer is worth announcing, so an
    /// ability the realm grants outside the window - automatic progression on a level up -
    /// is announced too. Every push rebuilds the client's spell index once per row, which is
    /// why this is opt-in: a window is a few hundred rows.
    void PushWindowTags(Player *player)
    {
        if (!player || !SpellbookNotify::Enabled() ||
            !sConfigMgr->GetOption<bool>(NOTIFY_ON_READY_KEY, false))
            return;

        std::vector<uint32> spellIds;
        for (Row const &row : BuildRows(player, false))
            spellIds.push_back(row.SpellId);

        // The window no longer sells what a talent tree grants, so a pick in the tree is the
        // only way those spells arrive and it is the pick that has to raise the alert. They are
        // not rows, so they are read straight from the tree table - the classes' own spells,
        // sorted, which is a contiguous run.
        auto const &tree = SpellbookTreeSpellData::TreeSpells;
        auto const first = std::lower_bound(tree.begin(), tree.end(), uint32(player->getClass()),
            [](SpellbookTreeSpellData::TreeSpell const &entry, uint32 classId)
            {
                return entry.ClassId < classId;
            });
        for (auto itr = first; itr != tree.end() && itr->ClassId == player->getClass(); ++itr)
            spellIds.push_back(itr->SpellId);

        SpellbookNotify::Push(player, spellIds);
        LOG_INFO("module.spellbook", "Announced {} spell(s) to {} (class {}) on client ready",
                 spellIds.size(), player->GetName(), uint32(player->getClass()));
    }

    /// The trainer window, written to match WorldPackets::NPC::TrainerList::Write exactly:
    /// guid, type, count, then one block per row, then the greeting.
    void SendTrainerWindow(Player *player, Creature *book, std::vector<Row> const &rows)
    {
        WorldPacket data(SMSG_TRAINER_LIST);
        data << book->GetGUID();
        data << int32(0);  // Trainer::Type::Class
        data << int32(rows.size());

        for (Row const &row : rows)
        {
            data << int32(row.SpellId);
            data << uint8(row.State);
            data << int32(RowPrice(player, book, row));
            data << uint32(0) << uint32(0);  // talent point costs
            data << uint8(row.RequiredLevel);
            data << int32(row.SkillLine);
            data << int32(0);                // no skill value requirement, see above
            data << uint32(row.RequiredAbility) << uint32(0) << uint32(0);
        }

        data << std::string(GREETING);
        player->SendDirectMessage(&data);
    }

    uint32 BookGossipText(Creature const *book)
    {
        return (book && book->GetEntry() == BEGINNERS_BOOK) ? BEGINNERS_TEXT : BOOK_TEXT;
    }

    /// Opens the window for \p player on \p book. False when there is nothing to show, so the
    /// caller can still offer the bulk alternative rather than leaving the book inert.
    bool OpenTrainer(Player *player, Creature *book)
    {
        if (!player || !book)
            return false;

        std::vector<Row> const rows = BuildRows(player);
        if (rows.empty())
            return false;

        SendTrainerWindow(player, book, rows);
        LOG_INFO("module.spellbook", "Sent {} trainer row(s) for {} (class {}) on book {}",
                 rows.size(), player->GetName(), uint32(player->getClass()), book->GetEntry());
        return true;
    }

    /// Learns everything the window currently offers as available. This is the book's
    /// "catch me up" action, so a character who fell behind does not have to click through
    /// eighty rows after a fix or a level cap change.
    uint32 LearnEverythingAvailable(Player *player)
    {
        uint32 learned = 0;
        for (Row const &row : BuildRows(player))
        {
            if (row.State != RowState::Available)
                continue;

            // Per ability, in front of its own learn, so every one of them arrives announced
            // exactly as a single purchase does.
            SpellbookNotify::Push(player, row.SpellId);

            player->learnSpell(row.SpellId, false);
            ++learned;
        }

        return learned;
    }

    /// Whether Player::addSpell will announce this ability as superseding an active lower rank
    /// rather than as newly learned. Mirrors the condition in Player::addSpell: the superseded
    /// path is only taken for a ranked ability that is not stackable with its ranks, and only
    /// when a lower rank is currently active.
    bool SupersedesActiveRank(Player *player, uint32 spellId)
    {
        SpellInfo const *info = sSpellMgr->GetSpellInfo(spellId);
        if (!info || info->IsStackableWithRanks() || !info->IsRanked())
            return false;

        for (SpellInfo const *rank = sSpellMgr->GetSpellInfo(sSpellMgr->GetFirstSpellInChain(spellId));
             rank; rank = rank->GetNextRankSpell())
            if (rank->GetRank() < info->GetRank() && player->HasActiveSpell(rank->Id))
                return true;

        return false;
    }

    /// The client's own "New Spell Learned!" announcement. Sent by hand for a purchase the core
    /// answered with SMSG_SUPERCEDED_SPELL, which the client handles silently, so buying from a
    /// book always shows the alert and plays its sound however the ability is powered.
    void SendLearnedAlert(Player *player, uint32 spellId)
    {
        WorldPacket data(SMSG_LEARNED_SPELL, 6);
        data << uint32(spellId);
        data << uint16(0);
        player->SendDirectMessage(&data);
    }

    /// A purchase from the window. Returns true when the packet belonged to a book, whether
    /// or not the ability was granted.
    bool HandlePurchase(Player *player, WorldPacket const &packet)
    {
        if (!player || !IsAscensionClass(player->getClass()))
            return false;

        // A packed guid followed by the spell id; read a copy so the dispatcher's buffer is
        // left alone.
        WorldPacket copy(packet);
        ObjectGuid guid;
        int32 spellId = 0;
        copy >> guid >> spellId;
        if (!guid || spellId <= 0)
            return false;

        Creature *book = player->GetNPCIfCanInteractWith(guid, 0);
        if (!IsBook(book))
            return false;

        uint32 const wanted = uint32(spellId);
        std::vector<Row> const rows = BuildRows(player);
        auto const found = std::find_if(rows.begin(), rows.end(),
            [wanted](Row const &row) { return row.SpellId == wanted; });

        // Only what this player was shown can be bought, so the window is the only way in.
        // A row the window no longer holds is either one the tree grants - it never was the
        // book's - or one the character has learned since the window was drawn. Both are
        // answered so the client is never left waiting on a purchase nothing will handle.
        if (found == rows.end())
        {
            if (IsTreeSpell(uint32(player->getClass()), wanted) ||
                HasRankOrBetter(player, wanted))
            {
                WorldPacket failed(SMSG_TRAINER_BUY_FAILED);
                failed << book->GetGUID() << uint32(wanted)
                       << uint32(AsUnderlyingType(BuyResult::Unavailable));
                player->SendDirectMessage(&failed);
                return true;
            }

            return false;
        }

        if (found->State != RowState::Available)
        {
            WorldPacket failed(SMSG_TRAINER_BUY_FAILED);
            failed << book->GetGUID() << uint32(wanted)
                   << uint32(AsUnderlyingType(BuyResult::NotEnoughSkill));
            player->SendDirectMessage(&failed);
            LOG_INFO("module.spellbook", "{} was refused {} from book {}: state {}", player->GetName(),
                     wanted, book->GetEntry(), AsUnderlyingType(found->State));
            return true;
        }

        // Charged before the learn, like Trainer::TeachSpell, and refused the way a trainer
        // refuses: an empty purse answers with the failure the client shows as "Not enough
        // money" instead of learning the spell for free.
        int32 const price = RowPrice(player, book, *found);
        if (price > 0 && !player->HasEnoughMoney(price))
        {
            WorldPacket failed(SMSG_TRAINER_BUY_FAILED);
            failed << book->GetGUID() << uint32(wanted)
                   << uint32(AsUnderlyingType(BuyResult::NotEnoughMoney));
            player->SendDirectMessage(&failed);
            LOG_INFO("module.spellbook", "{} could not afford {} ({} copper) from book {}",
                     player->GetName(), wanted, price, book->GetEntry());
            return true;
        }

        if (price > 0)
            player->ModifyMoney(-price);

        // Sampled before the learn: the core deactivates the lower rank while learning, so
        // afterwards there is nothing left to observe.
        bool const supersedes = SupersedesActiveRank(player, wanted);

        // In front of the learn, so the client's answer to "is this worth announcing" is
        // already in place when the learned-spell packet arrives behind it.
        SpellbookNotify::Push(player, wanted);

        player->learnSpell(wanted, false);
        if (supersedes && player->HasSpell(wanted))
            SendLearnedAlert(player, wanted);

        WorldPacket succeeded(SMSG_TRAINER_BUY_SUCCEEDED);
        succeeded << book->GetGUID() << uint32(wanted);
        player->SendDirectMessage(&succeeded);

        // The one line that says whether the client was told: the core announces an ability it
        // added, the book announces a rank the core answered with SMSG_SUPERCEDED_SPELL, and a
        // purchase the core refused outright is learned by nobody and must announce nothing.
        LOG_INFO("module.spellbook", "{} trained {} from book {} (class {}) for {} copper: {}",
                 player->GetName(), wanted, book->GetEntry(), uint32(player->getClass()), price,
                 !player->HasSpell(wanted) ? "nothing learned"
                     : (supersedes ? "announced by the book" : "announced by the core"));
        return true;
    }

    /// The other way a client can ask: some clients request the trainer list for a unit
    /// directly instead of opening its gossip, so the same window answers that too. Without
    /// this, a book flagged as a trainer could answer nothing at all.
    bool HandleTrainerListRequest(Player *player, WorldPacket const &packet)
    {
        if (!player || !IsAscensionClass(player->getClass()))
            return false;

        WorldPacket copy(packet);
        ObjectGuid guid;
        copy >> guid;
        if (!guid)
            return false;

        Creature *book = player->GetNPCIfCanInteractWith(guid, 0);
        if (!IsBook(book))
            return false;

        return OpenTrainer(player, book);
    }

    class SpellbookBookScript : public CreatureScript
    {
    public:
        SpellbookBookScript() : CreatureScript(BOOK_SCRIPT) { }

        bool OnGossipHello(Player *player, Creature *book) override
        {
            ClearGossipMenuFor(player);
            if (!Enabled() || !IsAscensionClass(player->getClass()))
                return true;

            // Right-clicking a book is a request for the trainer window: no gossip page in
            // between, which is how the books behaved on the live realm.
            if (OpenTrainer(player, book))
                return true;

            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Learn everything I am eligible for.",
                             GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
            SendGossipMenuFor(player, BookGossipText(book), book->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player *player, Creature *book, uint32 sender, uint32 action) override
        {
            ClearGossipMenuFor(player);
            CloseGossipMenuFor(player);
            if (sender != GOSSIP_SENDER_MAIN || action != GOSSIP_ACTION_INFO_DEF ||
                !Enabled() || !IsAscensionClass(player->getClass()))
                return true;

            uint32 const learned = LearnEverythingAvailable(player);
            if (learned)
            {
                ChatHandler(player->GetSession())
                    .PSendSysMessage("The book taught you {} ability or rank(s).", learned);
                LOG_INFO("module.spellbook", "{} learned {} ability or rank(s) from book {}",
                         player->GetName(), learned, book->GetEntry());
            }
            else
                ChatHandler(player->GetSession())
                    .SendSysMessage("You already know everything this book can teach you.");

            return true;
        }
    };

    /// Consumes the purchase before the core's handler sees it: sObjectMgr has no trainer for
    /// these entries, so nothing else can serve the request.
    class SpellbookServerScript : public ServerScript
    {
    public:
        SpellbookServerScript()
            : ServerScript("SpellbookServerScript", {SERVERHOOK_CAN_PACKET_RECEIVE})
        {
        }

        [[nodiscard]] bool CanPacketReceive(WorldSession *session,
                                            WorldPacket const &packet) override
        {
            if (!session || !session->GetPlayer() || !Enabled())
                return true;

            uint16 const opcode = packet.GetOpcode();
            if (opcode == CMSG_TRAINER_BUY_SPELL)
                return !HandlePurchase(session->GetPlayer(), packet);

            if (opcode == CMSG_TRAINER_LIST)
                return !HandleTrainerListRequest(session->GetPlayer(), packet);

            if (opcode == CMSG_EXTENSION_INITIALIZED)
                PushWindowTags(session->GetPlayer());

            return true;
        }
    };
}

void AddSpellbookScripts()
{
    new SpellbookBookScript();
    new SpellbookServerScript();
}

// The window this module would build for a player, for callers outside this file. The
// gameplay test driver asserts on it, and it is defined next to the builder so the two
// cannot drift apart.
namespace Spellbook
{
    uint32 RowCount(Player *player)
    {
        return uint32(BuildRows(player).size());
    }

    bool OffersSpell(Player *player, uint32 spellId)
    {
        for (Row const &row : BuildRows(player))
            if (row.SpellId == spellId)
                return true;

        return false;
    }

    bool CoversSpell(Player *player, uint32 spellId)
    {
        if (!player)
            return false;

        // The window drops what the character already holds, so membership alone would report
        // a bought spell as missing. Entitlement is the union of the two.
        return OffersSpell(player, spellId) || HasRankOrBetter(player, spellId);
    }
}
