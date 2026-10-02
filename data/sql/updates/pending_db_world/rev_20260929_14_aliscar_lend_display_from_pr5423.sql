-- Aliscar Lend (162806) was on known-safe stock fallback display 1593 (see
-- coa_codebase_gotchas). Matched to PR #5423's display for him (15767, his
-- only model there -- no alternates).
UPDATE `creature_template_model` SET `CreatureDisplayID`=15767 WHERE `CreatureID`=162806;
