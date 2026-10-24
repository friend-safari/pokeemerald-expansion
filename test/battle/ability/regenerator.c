#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Regenerator heals 1/3 of max HP upon switching out")
{
    u32 currHP;
    PARAMETRIZE { currHP = 1; }
    PARAMETRIZE { currHP = 2; }
    PARAMETRIZE { currHP = 3; }
    GIVEN {
        PLAYER(SPECIES_SLOWBRO) { Ability(ABILITY_REGENERATOR); HP(currHP); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { SWITCH(player, 1); }
        TURN { SWITCH(player, 0); }
    } SCENE {
        SWITCH_OUT_MESSAGE("Slowbro");
        SEND_IN_MESSAGE("Wobbuffet");
        SWITCH_OUT_MESSAGE("Wobbuffet");
        SEND_IN_MESSAGE("Slowbro");
    } THEN {
        EXPECT_EQ(player->hp, player->maxHP / 3 + currHP);
    }
}

SINGLE_BATTLE_TEST("Regenerator heals 1/3 of max HP upon switching out but doesn't surpass max HP")
{
    u32 currHP;
    PARAMETRIZE { currHP = 5; }
    PARAMETRIZE { currHP = 4; }
    PARAMETRIZE { currHP = 3; }
    PARAMETRIZE { currHP = 2; }
    PARAMETRIZE { currHP = 1; }
    GIVEN {
        PLAYER(SPECIES_SLOWBRO) { Ability(ABILITY_REGENERATOR); HP(currHP); MaxHP(5); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { SWITCH(player, 1); }
        TURN { SWITCH(player, 0); }
    } SCENE {
        SWITCH_OUT_MESSAGE("Slowbro");
        SEND_IN_MESSAGE("Wobbuffet");
        SWITCH_OUT_MESSAGE("Wobbuffet");
        SEND_IN_MESSAGE("Slowbro");
    } THEN {
        EXPECT_LE(player->hp, player->maxHP);
    }
}

SINGLE_BATTLE_TEST("Regenerator heals a pivoting user with a fainted party member")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_U_TURN) == EFFECT_HIT_ESCAPE);
        PLAYER(SPECIES_SHEDINJA);
        PLAYER(SPECIES_TANGELA) { Ability(ABILITY_REGENERATOR); HP(1); MaxHP(3);}
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_FIRE_FANG); SEND_OUT(player, 1);}
        TURN { MOVE(player, MOVE_U_TURN); MOVE(opponent, MOVE_CELEBRATE); SEND_OUT(player, 2); }
    } THEN {
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP), 0);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_HP), 2);
    }
}
