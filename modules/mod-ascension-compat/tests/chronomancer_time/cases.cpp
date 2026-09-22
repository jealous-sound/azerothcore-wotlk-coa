int main()
{
    InitData();
    for (auto &pair : manager.infos)
        ApplyTimeContracts(&pair.second);
    Player player;
    Unit target, friendOne, friendTwo, enemy;
    target.guid = 2;
    friendOne.guid = 3;
    friendTwo.guid = 4;
    enemy.guid = 5;
    target.x = 10;
    friendOne.x = 12;
    friendTwo.x = 14;
    enemy.x = 11;
    enemy.friendly = false;
    nearby = {&player, &target, &friendOne, &friendTwo, &enemy};
    chronomancer_time_casts hooks;
    Spell epoch(&player, manager.GetSpellInfo(Epoch), 0);
    epoch.m_targets.SetUnitTarget(&target);
    for (int i = 1; i <= 5; ++i)
    {
        hooks.OnSpellCast(&epoch, &player, epoch.GetSpellInfo(), false);
        assert(player.GetAura(Sands)->GetStackAmount() == i);
    }
    hooks.OnSpellCast(&epoch, &player, epoch.GetSpellInfo(), false);
    assert(!player.HasAura(Sands));
    epoch.triggered = true;
    hooks.OnSpellCast(&epoch, &player, epoch.GetSpellInfo(), false);
    assert(!player.HasAura(Sands));
    epoch.triggered = false;
    player.Add(EndlessSandsTalent, &player);
    hooks.OnSpellCast(&epoch, &player, epoch.GetSpellInfo(), false);
    assert(player.HasAura(EndlessSands));
    for (uint32 aeon : {RenewalAeon, ResilienceAeon, ProtectionAeon, OblivionAeon})
    {
        for (uint32 other : {RenewalAeon, ResilienceAeon, ProtectionAeon, OblivionAeon})
            player.Add(other, &player);
        Spell activation(&player, manager.GetSpellInfo(aeon), 0);
        assert(activation.IsTriggered());
        hooks.OnSpellCast(&activation, &player, activation.GetSpellInfo(), false);
        int count = 0;
        for (uint32 other : {RenewalAeon, ResilienceAeon, ProtectionAeon, OblivionAeon})
            count += player.HasAura(other);
        assert(count == 1 && player.HasAura(aeon));
        player.RemoveAurasDueToSpell(aeon);
        for (uint32 other : {RenewalAeon, ResilienceAeon, ProtectionAeon, OblivionAeon})
            assert(!player.HasAura(other));
    }
    for (uint32 beacon : {0u, 574310u, 574362u})
    {
        player.RemoveAurasDueToSpell(574310);
        player.RemoveAurasDueToSpell(574362);
        if (beacon)
            player.Add(beacon, &player);
        int duration = beacon == 0 ? 3000 : beacon == 574310 ? 7000 : 11000;
        player.Add(RenewalAeon, &player);
        ApplyEpochAeon(&player, &target, 13200);
        assert(player.casts.back().id == Renewal && player.casts.back().amount == 9900 / (duration / 1000));
        assert(target.GetAura(Renewal)->GetDuration() == duration);
        player.RemoveAurasDueToSpell(RenewalAeon);
        player.Add(ProtectionAeon, &player);
        ApplyEpochAeon(&player, &target, 13200);
        assert(player.casts.back().id == Protection && player.casts.back().amount == 6600);
        assert(target.GetAura(Protection)->GetDuration() == duration + 5000);
        player.RemoveAurasDueToSpell(ProtectionAeon);
    }
    player.Add(OblivionAeon, &player);
    ApplyEpochAeon(&player, &target, 600);
    assert(player.casts.back().id == Oblivion && player.casts.back().target == &enemy &&
           player.casts.back().amount == 600);
    auto count = player.casts.size();
    enemy.los = false;
    ApplyEpochAeon(&player, &target, 600);
    assert(player.casts.size() == count);
    enemy.los = true;
    player.RemoveAurasDueToSpell(OblivionAeon);
    player.Add(CadenceTalent, &player);
    player.Add(OrderlyTalent, &player);
    hooks.OnSpellHitResult(&epoch, &target, SPELL_MISS_IMMUNE, 0, 600, true);
    assert(!player.HasAura(Cadence));
    for (int i = 0; i < 12; ++i)
        hooks.OnSpellHitResult(&epoch, &target, SPELL_MISS_NONE, 0, 600, true);
    assert(player.GetAura(Cadence)->GetStackAmount() == 5 && player.GetAura(Orderly)->GetStackAmount() == 10);

    Aura *recovery = player.Add(501778, &target);
    recovery->SetDuration(4000);
    for (int i = 0; i < 6; ++i)
        ExtendRecovery(&player, &target);
    assert(recovery->GetDuration() == 19000 && recovery->GetScriptValue(Fortify) == 15000);
    Spell refresh(&player, manager.GetSpellInfo(501778), 0);
    hooks.OnSpellHitResult(&refresh, &target, 0, 0, 0, false);
    assert(!recovery->GetScriptValue(Fortify));
    recovery->SetDuration(4000);
    player.Add(Chronicler, &player);
    for (int i = 0; i < 6; ++i)
        ExtendRecovery(&player, &target);
    assert(recovery->GetDuration() == 34000 && recovery->GetScriptValue(Fortify) == 30000);
    recovery->caster = 99;
    ExtendRecovery(&player, &target);
    assert(recovery->GetDuration() == 34000);
    recovery->caster = player.guid;
    nearby = {&target, &friendOne, &friendTwo};
    friendOne.accept = false;
    SpreadRecovery(&player, &target, KeepAcceleratingSpread);
    assert(!friendOne.HasAura(501778) && friendTwo.HasAura(501778));
    friendOne.accept = true;
    player.Add(501778, &friendOne);
    player.Add(EpicRecovery, &player);
    player.known = {{Epoch, 1}, {501784, 1}, {Fortify, 1}, {807458, 1}};
    CastEpicRecovery(&player, &target);
    assert(echoes.size() == 2 && echoes[0].id == 501784 && echoes[1].id == 501784 && echoes[0].percent == 50);
    Spell echo(&player, manager.GetSpellInfo(501784), 1);
    echo.SetScriptValue(EpicRecovery, 50);
    TargetInfo result{-600, -500};
    hooks.OnSpellCalculatedTarget(&echo, &target, result);
    assert(result.damage == -300 && result.damageBeforeTakenMods == -250);
    count = player.GetAura(Sands)->GetStackAmount();
    hooks.OnSpellCast(&echo, &player, echo.GetSpellInfo(), false);
    assert(player.GetAura(Sands)->GetStackAmount() == count);

    aura_ascension_timeline_tether tether;
    tether.fixtureTarget = &player;
    HealInfo heal{100};
    DamageInfo damage{100};
    ProcEventInfo event{&player, PROC_FLAG_DONE_PERIODIC, &heal, nullptr};
    assert(tether.CheckProc(event));
    player.cooldowns = {{Fortify, 20000}, {807458, 15000}, {1, 9000}};
    tether.ReduceCooldown(nullptr, event);
    assert(tether.prevented);
    assert(player.cooldowns[Fortify] == 19000 && player.cooldowns[807458] == 14000 && player.cooldowns[1] == 9000);
    event.mask = 0;
    assert(!tether.CheckProc(event));
    event.mask = PROC_FLAG_DONE_PERIODIC;
    event.actor = &target;
    assert(!tether.CheckProc(event));
    event.actor = &player;
    heal.amount = 0;
    assert(!tether.CheckProc(event));
    event.heal = nullptr;
    event.damage = &damage;
    assert(tether.CheckProc(event));
    for (uint32 id : {TimeOut, TimeOutRankTwo, TimeOutRankThree})
    {
        auto info = manager.GetSpellInfo(id);
        assert(info->Effects[1].ApplyAuraName == 21 && info->Effects[1].Amplitude == 500 &&
               info->Effects[2].Effect == 137);
        assert(NativeManaTick(info->Effects[1].CalcValue(), 10000) == 500);
        assert(info->Effects[2].CalcValue() == 15);
    }
    assert(NativeManaTick(-5, 10000) == 0);
    player.Add(572635, &target);
    Spell slow(&player, manager.GetSpellInfo(Decelerate), 0);
    hooks.OnSpellHitResult(&slow, &target, 0, 0, 0, false);
    assert(!target.HasAura(572635));
    player.Add(Decelerate, &target);
    Spell fast(&player, manager.GetSpellInfo(572635), 0);
    hooks.OnSpellHitResult(&fast, &target, 0, 0, 0, false);
    assert(!target.HasAura(Decelerate));
    for (auto const &pair : manager.roots)
        if (pair.second == Fortify)
            assert(!manager.GetSpellInfo(pair.first)->Effects[1].Effect);
}
