CLI_DESCRIPTION = """Check the CoA talent budget table and the spellbook-derived talent state without a server.

Compiles the module's talent catalog loader and talent state code against the client DBC set the server loads
(--dbc-dir), then checks the essence budgets, rank derivation, point accounting, the known-entries wire form and
the specialization a known-entries upload selects.
No database, server build or game client is needed.
"""

import argparse
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
sys.path.insert(0, str(HERE.parent))
from coa_talent_catalog import STUBS  # noqa: E402
from client_data import dbc_dir  # noqa: E402

MAIN = r"""
#include "AscensionCoATalentData.h"
#include "AscensionCoATalentState.h"
#include "DBCStores.h"
#include <algorithm>
#include <cstdio>
#include <set>

using namespace AscensionCompatData;
using namespace AscensionCoATalentState;

namespace
{
int failures = 0;
void Check(bool value, char const* name)
{
    failures += !value;
    std::printf("%s: %s\n", value ? "PASS" : "FAIL", name);
}

CoATalentEntry const* Find(std::uint32_t entryId)
{
    for (CoATalentEntry const& entry : CoATalentEntries)
        if (entry.EntryId == entryId)
            return &entry;
    return nullptr;
}

// The first paid entry of a class on a tree, with at least `ranks` ranks.
CoATalentEntry const* FirstPaid(std::uint8_t classId, bool classTree, std::uint32_t ranks = 1)
{
    for (CoATalentEntry const& entry : CoATalentEntries)
        if (entry.ClassId == classId && (entry.AECost || entry.TECost) && (entry.SpecId == 0) == classTree &&
            entry.SpellCount >= ranks)
            return &entry;
    return nullptr;
}
}

int main(int, char** argv)
{
    DbcDirectory = std::string(argv[1]) + "/";
    Check(LoadCoATalentData(), "catalog loads with the essence table");

    // Budgets: the client's local rule is one class point at 10, one specialization point at 11, alternating.
    std::uint32_t ae = 0, te = 0;
    bool everyClass = true;
    for (std::uint8_t classId = 12; classId <= 32; ++classId)
    {
        everyClass = everyClass && GetCoATalentBudget(classId, 10, ae, te) && ae == 1 && te == 0;
        everyClass = everyClass && GetCoATalentBudget(classId, 11, ae, te) && ae == 1 && te == 1;
        everyClass = everyClass && GetCoATalentBudget(classId, 60, ae, te) && ae == 26 && te == 25;
        everyClass = everyClass && GetCoATalentBudget(classId, 80, ae, te) && ae == 36 && te == 35;
    }
    Check(everyClass, "every custom class has the 1/0, 1/1, 26/25 and 36/35 budgets at levels 10, 11, 60, 80");
    Check(GetCoATalentBudget(30, 9, ae, te) && ae == 0 && te == 0, "level 9 holds no points");
    Check(GetCoATalentBudget(30, 255, ae, te) && ae == 36 && te == 35, "a level past the table keeps the last row");
    Check(!GetCoATalentBudget(1, 60, ae, te), "a native class has no budget row");

    // Ranks and points from a spellbook.
    CoATalentEntry const* three = nullptr;
    for (CoATalentEntry const& entry : CoATalentEntries)
        if ((entry.AECost || entry.TECost) && entry.SpellCount == 3 && entry.SpecId == 0)
        {
            three = &entry;
            break;
        }
    Check(three != nullptr, "a paid three-rank class talent exists");
    CoATalentEntry const* spec = three ? FirstPaid(three->ClassId, false) : nullptr;
    Check(spec != nullptr, "the same class has a paid specialization talent");
    if (three && spec)
    {
        std::set<std::uint32_t> spellbook = { three->SpellIds[1], spec->SpellIds[0] };
        HasSpell hasSpell = [&spellbook](std::uint32_t id) { return spellbook.count(id) != 0; };
        Check(KnownRank(*three, hasSpell) == 2, "rank is the highest owned rank spell");
        Check(KnownRank(*spec, hasSpell) == 1, "a single owned rank spell is rank 1");
        std::vector<KnownEntry> known = KnownEntries(three->ClassId, hasSpell);
        Check(known.size() == 2, "known entries list exactly the owned entries");
        SpentPoints spent = Spent(known);
        Check(spent.AE == 2 * three->AECost && spent.TE == spec->TECost,
              "spent points charge every rank on the tree it belongs to");
        Check(Spent({}).AE == 0 && Spent({}).TE == 0, "nothing owned spends nothing");
        Check(KnownEntries(three->ClassId, [](std::uint32_t) { return false; }).empty(),
              "an empty spellbook knows no entry");

        // Shared rank spell: charged once, to the lower entry id (7131 and 12264 share 503748 in the catalog).
        CoATalentEntry const* first = Find(7131);
        CoATalentEntry const* second = Find(12264);
        if (first && second && first->SpellIds[0] == second->SpellIds[0])
        {
            std::vector<KnownEntry> shared = { { 7131, 1 }, { 12264, 1 } };
            SpentPoints once = Spent(shared);
            Check(once.AE + once.TE == 1, "a rank spell two entries share is charged once");
            Check(once.TE == 1 && once.AE == 0, "the shared spell is charged to the lower entry id's tree");
        }
        else
            Check(false, "catalog still shares spell 503748 between entries 7131 and 12264");

        // Wire form.
        std::vector<std::uint8_t> body = KnownEntriesPayload(known);
        Check(body.size() == 4 + 21 * known.size(), "known-entries body is u32 count plus 21 bytes per record");
        Check(body[0] == 2 && body[1] == 0 && body[2] == 0 && body[3] == 0, "count is little-endian");
        std::vector<KnownEntry> parsed;
        Check(ParseKnownEntriesUpload(body.data(), body.size(), parsed) && parsed.size() == 2 &&
                  parsed[0].EntryId == known[0].EntryId && parsed[0].Rank == known[0].Rank &&
                  parsed[1].EntryId == known[1].EntryId && parsed[1].Rank == known[1].Rank,
              "the upload parser reads back what the payload wrote");
        Check(!ParseKnownEntriesUpload(body.data(), body.size() - 1, parsed), "a short body is refused");
        body.push_back(0);
        Check(!ParseKnownEntriesUpload(body.data(), body.size(), parsed), "a long body is refused");
        std::uint8_t empty[4] = { 0, 0, 0, 0 };
        Check(ParseKnownEntriesUpload(empty, 4, parsed) && parsed.empty(), "count zero is an empty set");
        Check(!ParseKnownEntriesUpload(empty, 3, parsed), "less than a count is refused");
        Check(KnownEntriesPayload({}).size() == 4, "an empty set is a bare zero count");
    }

    CoATalentEntry const* spikes = Find(7237);
    CoATalentEntry const* lair = Find(7242);
    CoATalentEntry const* captivation = Find(5215);
    CoATalentEntry const* mamba = Find(7244);
    CoATalentEntry const* celerity = Find(30465);
    Check(spikes && spikes->Row == 4 && spikes->RequiredTreePoints == 9, "class tree row 4 needs 9 points");
    Check(lair && lair->Row == 9 && lair->RequiredTreePoints == 24, "class tree row 9 needs 24 points");
    Check(captivation && captivation->SpecId && captivation->Row == 4 && captivation->RequiredTreePoints == 8,
          "specialization row 4 needs 8 points");
    Check(mamba && celerity && mamba->ChoiceGroup && mamba->ChoiceGroup == celerity->ChoiceGroup,
          "the two options of a paid choice node share a group");
    std::vector<KnownEntry> gated = { { 7229, 1 }, { 7237, 1 } };
    std::vector<std::uint32_t> violations = LayoutViolations(gated);
    Check(violations.size() == 1 && violations[0] == 7237, "a row 4 node with one point above it breaks its gate");
    std::vector<KnownEntry> met = { { 7229, 1 }, { 29607, 1 }, { 7722, 1 }, { 30482, 1 }, { 29588, 1 },
                                    { 6117, 1 }, { 7241, 1 }, { 6116, 1 }, { 7252, 1 }, { 7237, 1 } };
    Check(LayoutViolations(met).empty(), "nine points above row 4 meet its gate");
    met.push_back({ 7244, 1 });
    met.push_back({ 30465, 1 });
    violations = LayoutViolations(met);
    Check(violations.size() == 1 && violations[0] == 30465, "both options of a choice node flag the later one");

    std::set<std::uint8_t> specializedClasses;
    for (CoATalentEntry const& entry : CoATalentEntries)
        if (entry.SpecId)
            specializedClasses.insert(entry.ClassId);
    std::set<std::uint8_t> identifiedClasses;
    bool identitiesBelong = !CoASpecializations.empty();
    bool switchesDetected = true;
    for (CoASpecialization const& specialization : CoASpecializations)
    {
        identifiedClasses.insert(specialization.ClassId);
        CoATalentEntry const* identity = Find(specialization.IdentityEntryId);
        identitiesBelong = identitiesBelong && identity && identity->ClassId == specialization.ClassId &&
            identity->SpecId == specialization.SpecId;
        UploadedSpecialization const uploaded = SpecializationOf(
            SpecializationSwitch(specialization.ClassId, [](std::uint32_t) { return false; }, specialization.SpecId));
        switchesDetected = switchesDetected && uploaded.SpecId == specialization.SpecId && !uploaded.Mixed &&
            !uploaded.ChoosesTalents;
    }
    Check(identitiesBelong,
          "every specialization identity entry is a catalog entry of its own class and specialization");
    Check(identifiedClasses == specializedClasses, "every class with specialization talents has identity entries");
    Check(switchesDetected,
          "a switch upload names exactly the specialization it enters and chooses none of its talents");

    CoATalentEntry const* classTalent = spec ? FirstPaid(spec->ClassId, true) : nullptr;
    std::vector<CoASpecialization> specializations;
    for (CoASpecialization const& specialization : CoASpecializations)
        if (classTalent && specialization.ClassId == classTalent->ClassId)
            specializations.push_back(specialization);
    Check(classTalent && specializations.size() >= 2, "a class has a paid class talent and two specializations");
    if (classTalent && specializations.size() >= 2)
    {
        std::set<std::uint32_t> spellbook = { classTalent->SpellIds[0] };
        HasSpell hasSpell = [&spellbook](std::uint32_t id) { return spellbook.count(id) != 0; };
        std::vector<KnownEntry> const entering =
            SpecializationSwitch(classTalent->ClassId, hasSpell, specializations[0].SpecId);
        Check(std::any_of(entering.begin(), entering.end(),
                  [classTalent](KnownEntry const& item) { return item.EntryId == classTalent->EntryId; }),
              "a switch upload keeps the class tree");
        Check(SpecializationOf({ { classTalent->EntryId, 1 } }).SpecId == 0,
              "a class-tree upload names no specialization");
        Check(SpecializationOf({ { specializations[0].IdentityEntryId, 0 } }).SpecId == 0,
              "an entry at rank 0 names no specialization");
        Check(SpecializationOf({ { specializations[0].IdentityEntryId, 1 },
                  { specializations[1].IdentityEntryId, 1 } }).Mixed,
              "entries of two specializations are a mixed upload");

        CoATalentEntry const* chosen = nullptr;
        CoATalentEntry const* automatic = nullptr;
        for (CoATalentEntry const& entry : CoATalentEntries)
        {
            if (entry.SpecId != specializations[0].SpecId)
                continue;
            if (!chosen && (entry.AECost || entry.TECost) && entry.EntryId != specializations[0].SignatureEntryId)
                chosen = &entry;
            if (!automatic && !entry.AECost && !entry.TECost && entry.EntryId != specializations[0].IdentityEntryId)
                automatic = &entry;
        }
        Check(chosen && automatic, "the specialization has a paid talent and an automatic entry");
        if (chosen && automatic)
        {
            UploadedSpecialization const picked = SpecializationOf({ { chosen->EntryId, 1 } });
            Check(picked.SpecId == specializations[0].SpecId && picked.ChoosesTalents,
                  "a paid specialization talent names its specialization and chooses a talent");
            Check(SpecializationOf({ { automatic->EntryId, 1 } }).SpecId == 0,
                  "an automatic entry other than the identity names no specialization");
            Check(!SpecializationOf({ { automatic->EntryId, 1 }, { specializations[1].IdentityEntryId, 1 } }).Mixed,
                  "an automatic entry of another specialization does not mix the upload");
        }
    }

    return failures ? 1 : 0;
}
"""


def main():
    parser = argparse.ArgumentParser(description=CLI_DESCRIPTION)
    parser.add_argument("--dbc-dir", type=Path)
    args = parser.parse_args()
    args.dbc_dir = args.dbc_dir or dbc_dir()

    compiler = shutil.which(os.environ.get("CXX", "cl.exe" if os.name == "nt" else "c++"))
    assert compiler, "Enable a C++20 compiler (VS Developer PowerShell on Windows)."
    with tempfile.TemporaryDirectory(prefix="coa-talent-state-") as directory:
        out = Path(directory)
        for name, text in STUBS.items():
            (out / name).write_text(text, encoding="utf-8")
        (out / "main.cpp").write_text(MAIN, encoding="utf-8")
        includes = [out, ROOT / "src/server/coa", ROOT / "src/server/shared/DataStores",
                    ROOT / "src/common"]
        sources = [out / "main.cpp", ROOT / "src/server/coa/AscensionCoATalentData.cpp",
                   ROOT / "src/server/coa/AscensionCoATalentState.cpp",
                   ROOT / "src/server/shared/DataStores/ClientDBC.cpp"]
        executable = out / ("state.exe" if os.name == "nt" else "state")
        if Path(compiler).stem.lower() == "cl":
            flags = ["/nologo", "/std:c++20", "/EHsc", "/utf-8", "/D_CRT_SECURE_NO_WARNINGS",
                     *["/I" + str(p) for p in includes], *map(str, sources), "/Fe" + str(executable)]
        else:
            flags = ["-std=c++20", "-Wall", "-Wextra", *["-I" + str(p) for p in includes], *map(str, sources),
                     "-o", str(executable)]
        build = subprocess.run([compiler, *flags], cwd=out, capture_output=True, text=True, errors="replace")
        if build.returncode:
            raise SystemExit("Talent state harness did not compile:\n" + build.stdout + build.stderr)
        result = subprocess.run([str(executable), str(args.dbc_dir.resolve())], text=True)
        raise SystemExit(result.returncode)


if __name__ == "__main__":
    main()
