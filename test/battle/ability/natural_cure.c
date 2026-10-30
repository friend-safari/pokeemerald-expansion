#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Natural Cure cures the user that switched out")
{
    GIVEN {
        PLAYER(SPECIES_CELEBI) { Ability(ABILITY_NATURAL_CURE); Status1(STATUS1_POISON); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { SWITCH(player, 1); MOVE(opponent, MOVE_CELEBRATE); }
    } THEN {
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_STATUS), STATUS1_NONE);
    }
}

SINGLE_BATTLE_TEST("Natural Cure cures the user that passed out")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_BATON_PASS) == EFFECT_BATON_PASS);
        PLAYER(SPECIES_CELEBI) { Ability(ABILITY_NATURAL_CURE); Status1(STATUS1_POISON); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_BATON_PASS); MOVE(opponent, MOVE_CELEBRATE); SEND_OUT(player, 1); }
    } THEN {
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_STATUS), STATUS1_NONE);
    }
}

SINGLE_BATTLE_TEST("Natural Cure cures the user that pivoted out")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_U_TURN) == EFFECT_HIT_ESCAPE);
        PLAYER(SPECIES_CELEBI) { Ability(ABILITY_NATURAL_CURE); Status1(STATUS1_POISON); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_U_TURN); MOVE(opponent, MOVE_CELEBRATE); SEND_OUT(player, 1); }
    } THEN {
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_STATUS), STATUS1_NONE);
    }
}
