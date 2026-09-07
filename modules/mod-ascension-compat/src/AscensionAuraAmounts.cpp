/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionAuraAmounts.h"
#include "Player.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "WorldPacket.h"

namespace
{
// Extensions.dll registers these separately from the stock aura packets.
// Stock 12340's aura decoder does NOT support AFLAG_ANY_EFFECT_AMOUNT_SENT.
constexpr uint16 SMSG_ASCENSION_AURA_AMOUNT = 0x0673;
constexpr uint32 SPELL_BARBARIAN_DRUNKEN_FRENZY = 804771;
constexpr uint32 SPELL_BARBARIAN_TANKARD = 805813;

void AppendAuraAmounts(WorldPacket& data, AuraApplication const& application, bool remove)
{
    Aura const* aura = application.GetBase();
    // Native key: target GUID (packet header), caster GUID and spell ID. A -1
    // operation deletes that key; all other operations carry exactly 3 int32s.
    data << int32(remove ? -1 : application.GetSlot());
    data << aura->GetCasterGUID() << uint32(aura->GetId());
    if (remove)
        return;

    for (uint8 index = 0; index < MAX_SPELL_EFFECTS; ++index)
    {
        // The authored Drunken Frenzy hover reads w2, but its real damage aura
        // occupies effect 1. Preserve w1 too; this is presentation, not a stat.
        uint8 sourceIndex = aura->GetId() == SPELL_BARBARIAN_DRUNKEN_FRENZY && index == 1 ? 0 : index;
        AuraEffect const* effect = aura->GetEffect(sourceIndex);
        int32 amount = effect && application.HasEffect(sourceIndex) && application.IsActive(sourceIndex)
            ? effect->GetAmount() : 0;
        // Tankard's w1 is fullness, not its negative cooldown spell modifier.
        // Compute it from actual live stacks and the verified native capacity.
        if (aura->GetId() == SPELL_BARBARIAN_TANKARD && index == 0 && amount && aura->GetSpellInfo()->StackAmount)
            amount = 100 * aura->GetStackAmount() / aura->GetSpellInfo()->StackAmount;
        data << amount;
    }
}
}

void SendAscensionAuraAmounts(Unit* target, Player* receiver,
    AuraApplication const* application, bool remove)
{
    if (!target || (application && application->GetSlot() >= MAX_AURAS))
        return;

    if (!application)
    {
        // Use separate records for the initial snapshot as well. Native 0x674
        // stops reading an entry before its amounts if the client lacks that
        // spell. SQL-only spells could then desynchronize the following entries.
        // The single-record 0x673 handler safely discards such an unknown record.
        for (auto const& [slot, visible] : *target->GetVisibleAuras())
        {
            (void)slot;
            SendAscensionAuraAmounts(target, receiver, visible, false);
        }
        return;
    }

    if (application && remove)
        for (auto const& [slot, visible] : *target->GetVisibleAuras())
        {
            (void)slot;
            // Different item instances can share the native (caster, spell) key.
            // The removed application has already left the visible map: retain
            // a surviving instance instead of deleting its amount information.
            if (visible != application && visible->GetBase()->GetId() == application->GetBase()->GetId() &&
                visible->GetBase()->GetCasterGUID() == application->GetBase()->GetCasterGUID())
            {
                application = visible;
                remove = false;
                break;
            }
        }

    WorldPacket data(SMSG_ASCENSION_AURA_AMOUNT);
    data << target->GetGUID(); // full uint64, NOT the stock packed GUID
    AppendAuraAmounts(data, *application, remove);

    if (receiver)
        receiver->SendDirectMessage(&data);
    else
        target->SendMessageToSet(&data, true);
}
