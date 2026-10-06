# Core patch: vanilla classes use the Bronzebeard copies (stock id + 1100000) of the class spells and talents,
# but the class scripts check stock ids (HasAura, GetAuraOfRankedSpell, GetAuraEffect, HasSpell, HasTalent,
# RemoveAurasDueToSpell...). Each of these lookups retries with the copy when the stock id finds nothing.
# The helper lives in Entities/Unit/AscensionSpellCopy.h (written by hand, see the branch).
import sys, pathlib

core = pathlib.Path(sys.argv[1] if len(sys.argv) > 1 else r'C:\CoA-Build\core')
unit = core / 'src/server/game/Entities/Unit/Unit.cpp'
player = core / 'src/server/game/Entities/Player/Player.cpp'


def patch(path, old, new, key):
    text = path.read_text(encoding='utf-8')
    if key in text:
        return
    assert text.count(old) == 1, (path.name, old[:80])
    path.write_text(text.replace(old, new), encoding='utf-8', newline='')


def retry(path, signature, old_tail, new_tail, key):
    """Replace the tail of the function that starts at `signature` (first match after it)."""
    text = path.read_text(encoding='utf-8')
    if key in text:
        return
    start = text.index(signature)
    pos = text.index(old_tail, start)
    text = text[:pos] + new_tail + text[pos + len(old_tail):]
    path.write_text(text, encoding='utf-8', newline='')


patch(unit, '#include "Unit.h"\n', '#include "Unit.h"\n#include "AscensionSpellCopy.h"\n', key='#include "AscensionSpellCopy.h"')

patch(unit, '''void Unit::RemoveAurasDueToSpell(uint32 spellId, ObjectGuid casterGUID, uint8 reqEffMask, AuraRemoveMode removeMode)
{
''', '''void Unit::RemoveAurasDueToSpell(uint32 spellId, ObjectGuid casterGUID, uint8 reqEffMask, AuraRemoveMode removeMode)
{
    if (uint32 copy = GetAscensionSpellCopy(spellId)) // Bronzebeard copy (vanilla classes)
        if (m_appliedAuras.find(spellId) == m_appliedAuras.end())
            spellId = copy;
''', key='if (m_appliedAuras.find(spellId) == m_appliedAuras.end())')

retry(unit, 'AuraEffect* Unit::GetAuraEffect(uint32 spellId, uint8 effIndex, ObjectGuid caster) const\n{',
      '''            return itr->second->GetBase()->GetEffect(effIndex);
        }
    }
    return nullptr;
}''', '''            return itr->second->GetBase()->GetEffect(effIndex);
        }
    }
    if (uint32 copy = GetAscensionSpellCopy(spellId)) // Bronzebeard copy (vanilla classes)
        return GetAuraEffect(copy, effIndex, caster);
    return nullptr;
}''', key='return GetAuraEffect(copy, effIndex, caster);')

retry(unit, 'AuraEffect* Unit::GetAuraEffectDummy(uint32 spellid) const\n{',
      '''            return *itr;
    }

    return nullptr;
}''', '''            return *itr;
    }

    if (uint32 copy = GetAscensionSpellCopy(spellid)) // Bronzebeard copy (vanilla classes)
        return GetAuraEffectDummy(copy);
    return nullptr;
}''', key='return GetAuraEffectDummy(copy);')

# Not in GetAuraApplication / GetOwnedAura themselves: the aura engine calls those with an aura's own id
# (stacking, refresh, client slots) and must never match a different aura.
patch(unit, '''Aura* Unit::GetAura(uint32 spellId, ObjectGuid casterGUID, ObjectGuid itemCasterGUID, uint8 reqEffMask) const
{
    AuraApplication* aurApp = GetAuraApplication(spellId, casterGUID, itemCasterGUID, reqEffMask);
''', '''Aura* Unit::GetAura(uint32 spellId, ObjectGuid casterGUID, ObjectGuid itemCasterGUID, uint8 reqEffMask) const
{
    AuraApplication* aurApp = GetAuraApplication(spellId, casterGUID, itemCasterGUID, reqEffMask);
    if (!aurApp)
        if (uint32 copy = GetAscensionSpellCopy(spellId)) // Bronzebeard copy (vanilla classes)
            aurApp = GetAuraApplication(copy, casterGUID, itemCasterGUID, reqEffMask);
''', key='aurApp = GetAuraApplication(copy, casterGUID, itemCasterGUID, reqEffMask);')

retry(unit, 'AuraApplication* Unit::GetAuraApplicationOfRankedSpell(uint32 spellId, ObjectGuid casterGUID, ObjectGuid itemCasterGUID, uint8 reqEffMask, AuraApplication* except) const\n{',
      '''        rankSpell = sSpellMgr->GetNextSpellInChain(rankSpell);
    }
    return nullptr;
}''', '''        rankSpell = sSpellMgr->GetNextSpellInChain(rankSpell);
    }
    if (uint32 copy = GetAscensionSpellCopy(spellId)) // Bronzebeard copy (vanilla classes)
        return GetAuraApplicationOfRankedSpell(copy, casterGUID, itemCasterGUID, reqEffMask, except);
    return nullptr;
}''', key='return GetAuraApplicationOfRankedSpell(copy,')

patch(unit, '''    if (GetAuraApplication(spellId, casterGUID, itemCasterGUID, reqEffMask))
        return true;
    return false;
}''', '''    if (GetAuraApplication(spellId, casterGUID, itemCasterGUID, reqEffMask))
        return true;
    if (uint32 copy = GetAscensionSpellCopy(spellId)) // Bronzebeard copy (vanilla classes)
        return GetAuraApplication(copy, casterGUID, itemCasterGUID, reqEffMask) != nullptr;
    return false;
}''', key='return GetAuraApplication(copy, casterGUID, itemCasterGUID, reqEffMask) != nullptr;')

retry(unit, 'bool Unit::HasAuraEffect(uint32 spellId, uint8 effIndex, ObjectGuid caster) const\n{',
      '''            return true;
        }
    }
    return false;
}''', '''            return true;
        }
    }
    if (uint32 copy = GetAscensionSpellCopy(spellId)) // Bronzebeard copy (vanilla classes)
        return HasAuraEffect(copy, effIndex, caster);
    return false;
}''', key='return HasAuraEffect(copy,')

retry(unit, 'uint32 Unit::GetAuraCount(uint32 spellId) const\n{',
      '''    return count;
}''', '''    if (!count)
        if (uint32 copy = GetAscensionSpellCopy(spellId)) // Bronzebeard copy (vanilla classes)
            return GetAuraCount(copy);
    return count;
}''', key='return GetAuraCount(copy);')

patch(player, '#include "Player.h"\n', '#include "Player.h"\n#include "AscensionSpellCopy.h"\n', key='#include "AscensionSpellCopy.h"')
for name, extra in (('HasSpell', ''), ('HasActiveSpell', ' && itr->second->Active')):
    patch(player, '''bool Player::%s(uint32 spell) const
{
    PlayerSpellMap::const_iterator itr = m_spells.find(spell);
    return (itr != m_spells.end() && itr->second->State != PLAYERSPELL_REMOVED%s && itr->second->IsInSpec(m_activeSpec));
}''' % (name, extra), '''bool Player::%s(uint32 spell) const
{
    PlayerSpellMap::const_iterator itr = m_spells.find(spell);
    if (itr != m_spells.end() && itr->second->State != PLAYERSPELL_REMOVED%s && itr->second->IsInSpec(m_activeSpec))
        return true;
    uint32 copy = GetAscensionSpellCopy(spell); // Bronzebeard copy (vanilla classes)
    return copy && %s(copy);
}''' % (name, extra, name), key='return copy && %s(copy);' % name)
patch(player, '''bool Player::HasTalent(uint32 spell, uint8 spec) const
{
    PlayerTalentMap::const_iterator itr = m_talents.find(spell);
    return (itr != m_talents.end() && itr->second->State != PLAYERSPELL_REMOVED && itr->second->IsInSpec(spec));
}''', '''bool Player::HasTalent(uint32 spell, uint8 spec) const
{
    PlayerTalentMap::const_iterator itr = m_talents.find(spell);
    if (itr != m_talents.end() && itr->second->State != PLAYERSPELL_REMOVED && itr->second->IsInSpec(spec))
        return true;
    uint32 copy = GetAscensionSpellCopy(spell); // Bronzebeard copy (vanilla classes)
    return copy && HasTalent(copy, spec);
}''', key='return copy && HasTalent(copy, spec);')
print('spell copy patch applied')
