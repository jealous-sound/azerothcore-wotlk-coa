/*
 * The classic raids in four difficulties.
 *
 * The difficulty templates, the links between them and the spawn masks are
 * data and live in data/sql. The code is the boss AI that reads its fight out
 * of coa_boss_schedule, the bosses that keep their choreography with Ascension's
 * spells (Onyxia and the Blackwing Lair bosses), the flex health that sizes a boss to the raid, and
 * the corrections to broken spell values.
 *
 * modules/mod-coa-raid-difficulty -> Addmod_coa_raid_difficultyScripts.
 */
void AddCoaBossAIScripts();
void AddCoaFlexHealthScripts();
void AddCoaOnyxiaScripts();
void AddCoaSpellFixScripts();
void AddCoaRazorgoreScripts();
void AddCoaVaelastraszScripts();
void AddCoaBroodlordScripts();
void AddCoaDrakeScripts();
void AddCoaChromaggusScripts();
void AddCoaNefarianScripts();

void Addmod_coa_raid_difficultyScripts()
{
    AddCoaBossAIScripts();
    AddCoaFlexHealthScripts();
    AddCoaOnyxiaScripts();
    AddCoaSpellFixScripts();
    AddCoaRazorgoreScripts();
    AddCoaVaelastraszScripts();
    AddCoaBroodlordScripts();
    AddCoaDrakeScripts();
    AddCoaChromaggusScripts();
    AddCoaNefarianScripts();
}
