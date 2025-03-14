#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <utility>

#include "zero/util.h"

static constexpr float RARITY_FACTOR[5] = {
    68.0f / 39.0f,
    73.0f / 39.0f,
    79.0f / 39.0f,
    84.0f / 39.0f,
    91.0f / 39.0f
};

enum WEAPON_SP : int16_t {
    UNIQUE_WEAPON = 400,
    REFINED_ARCANE_WEAPON = 350,
    REFINED_GENERIC_WEAPON = 350,
    ARCANE_WEAPON = 300,
    GENERIC_WEAPON = 300,
    NO_WEAPON = 0,
};
enum ASSIST_SP : int16_t {
    CHROM_UNIQUE_REPOSITION = 500,
    UNIQUE_ASSIST = 400,
    REPOSITION_GAIT = 400,
    DOUBLE_RALLY_PLUS_ASSIST = 400,
    RALLY_UP_PLUS_ASSIST = 400,
    DOUBLE_RALLY_ASSIST = 300,
    RALLY_UP_ASSIST = 300,
    STAFF_ASSIST = 300,
    MOVEMENT_ASSIST = 150,
    DANCE_ASSIST = 150,
    RALLY_ASSIST = 150,
    NO_ASSIST = 0,
};
enum SPECIAL_SP : int16_t {
    UNIQUE_SPECIAL = 500,
    TIER_3_SPECIAL = 500, // Aether, galeforce, and anything rare/expensive to get
    AOE_SPECIAL = 300,
    DOUBLE_BALM_PLUS_SPECIAL = 300,
    DOUBLE_BALM_SPECIAL = 200,
    TIER_2_SPECIAL = 200, // Moonbow, bonfire, astra, aegis, pavise, etc.
    MIRACLE_SPECIAL = 200,
    BALM_SPECIAL = 150,
    HEAVENLY_LIGHT_SPECIAL = 150,
    NO_SPECIAL = 0,
};
enum SKILL_SP : int16_t {
    // A skills
    HECTOR_OSTIAN_COUNTER = 350,
    UNIQUE_A_SKILL = 300,
    TIER_4_DUEL_SKILL = 303,
    TIER_4_A_SKILL = 300,
    TIER_3_DUEL_SKILL = 301,
    DISTANT_COUNTER = 300,
    CLOSE_COUNTER = 300,
    TIER_3_A_SKILL = 240, // Everything else
    TIER_3_A_SKILL_LOW = 200, // Fury, Life and Death, Death blow, Iote's shield, Triangle Adpet, Still water, etc.

    // B skills
    HIGH_DRAGON_WALL = 400,
    UNIQUE_B_SKILL = 300,
    TIER_4_B_SKILL = 300,
    TIER_3_B_SKILL = 240, // Everything else
    TIER_3_B_SKILL_LOW = 200, // Breaker skills, Guard, Vantage, Desperation, Brash Assault, Cancel Affinity, single stat dance buffs

    // C skills
    UNIQUE_C_SKILL = 300,
    TIER_4_C_SKILL = 300,
    SAVE_SKILL = 300,
    TIER_3_C_SKILL = 240, // Everything else
    TIER_3_C_SKILL_LOW = 200, // Hone, Fortify, Goad, Ward, Spur, Threaten, Savage Blow
    XP_SP_C_SKILL = 120, // please don't

    // Seal skills
    BEAST_SEAL = 240,
    SQUAD_ACE_TRIPLE_STAT_SEAL = 240,
    DEFLECT_SEAL = 200,
    HARDY_BEARING = 200,
    PHANTOM_SEAL = 200,
    SQUAD_ACE_DOUBLE_STAT_HP_SEAL = 200,
    SQUAD_ACE_DOUBLE_STAT_SEAL = 160,
    ARMORED_BOOTS = 100,
    QUICKENED_PULSE = 100,

    NO_SKILL = 0,
    NO_SEAL = 0,
};

struct UnitData {
    int8_t rarity;
    int8_t level;
    int8_t merges;
    int16_t bst;
    WEAPON_SP weapon;
    ASSIST_SP assist;
    SPECIAL_SP special;
    SKILL_SP a_skill;
    SKILL_SP b_skill;
    SKILL_SP c_skill;
    SKILL_SP seal;
    bool empty;

    int32_t score() const {
        if (!this->empty) {
            uint32_t bst = this->bst;
            uint32_t total_sp = this->a_skill;
            if (total_sp & 1) {
                if (this->rarity == 5 && this->level == 40) {
                    bst = std::max(bst, (total_sp & 2) ? 175u : 170u);
                }
                total_sp &= -3;
            }
            total_sp += this->weapon;
            total_sp += this->assist;
            total_sp += this->special;
            total_sp += this->b_skill;
            total_sp += this->c_skill;
            total_sp += this->seal;

            int32_t rarity_score = 45 + (this->rarity * 2) + (int32_t)(RARITY_FACTOR[this->rarity - 1] * this->level);
            int32_t merge_score = this->merges * 2;
            int32_t bst_score = (bst / 5);
            int32_t sp_score = (total_sp / 100);
            int32_t total_score = rarity_score + merge_score + bst_score + sp_score;
            return total_score;
        }
        return 0;
    }
};

void print_scores(const UnitData(&units)[4], bool bonus_unit) {
    int32_t unit_scores[4];
    for (size_t i = 0; i < countof(units); ++i) {
        unit_scores[i] = units[i].score();
    }
    int32_t total_score = unit_scores[0] + unit_scores[1] + unit_scores[2] + unit_scores[3];
    float team_score = ((float)total_score / 4.0f) + 150.0f;
    if (bonus_unit) {
        team_score *= 2.0f;
    }
    printf(
        "Unit1: %u\n"
        "Unit2: %u\n"
        "Unit3: %u\n"
        "Unit4: %u\n"
        "Team:  %f"
        , (unit_scores[0] ? unit_scores[0] + 150 : 0) << bonus_unit
        , (unit_scores[1] ? unit_scores[1] + 150 : 0) << bonus_unit
        , (unit_scores[2] ? unit_scores[2] + 150 : 0) << bonus_unit
        , (unit_scores[3] ? unit_scores[3] + 150 : 0) << bonus_unit
        , team_score
    );
}

int main(int argc, char* argv[]) {

    const UnitData units[4] = {
        /*
        [0] = {
            .rarity = 5, .level = 40,
            .bst = 207,
            .weapon = UNIQUE_WEAPON,
            .assist = DOUBLE_RALLY_ASSIST,
            .special = TIER_3_SPECIAL,
            .a_skill = TIER_4_A_SKILL,
            .b_skill = TIER_3_B_SKILL,
            .c_skill = TIER_4_C_SKILL,
            .seal = DISTANT_COUNTER
        },
        [1] = {
            .rarity = 5, .level = 40,
            .merges = 4,
            .bst = 187,
            .weapon = GENERIC_WEAPON,
            .assist = DOUBLE_RALLY_ASSIST,
            .special = TIER_3_SPECIAL,
            .a_skill = TIER_3_A_SKILL,
            .b_skill = TIER_3_B_SKILL,
            .c_skill = TIER_3_C_SKILL,
            .seal = DEFLECT_SEAL
        },
        [2] = {
            .rarity = 5, .level = 40,
            .bst = 199,
            .weapon = REFINED_ARCANE_WEAPON,
            .assist = DOUBLE_RALLY_ASSIST,
            .special = TIER_3_SPECIAL,
            .a_skill = UNIQUE_A_SKILL,
            .b_skill = TIER_4_B_SKILL,
            .c_skill = TIER_4_C_SKILL,
            .seal = DISTANT_COUNTER
        },
        [3] = {
            .rarity = 5, .level = 40,
            .bst = 194,
            .weapon = UNIQUE_WEAPON,
            .assist = DOUBLE_RALLY_ASSIST,
            .special = TIER_3_SPECIAL,
            .a_skill = UNIQUE_A_SKILL,
            .b_skill = TIER_4_B_SKILL,
            .c_skill = TIER_4_C_SKILL,
            .seal = SQUAD_ACE_TRIPLE_STAT_SEAL
        }
        */
        [0] = {
            .rarity = 5, .level = 40,
            .bst = 199,
            .weapon = UNIQUE_WEAPON,
            .assist = MOVEMENT_ASSIST,
            .special = TIER_3_SPECIAL,
            .a_skill = TIER_4_A_SKILL,
            .b_skill = UNIQUE_B_SKILL,
            .c_skill = TIER_4_C_SKILL,
            .seal = TIER_3_A_SKILL
        },
        [1] = {
            .rarity = 5, .level = 40,
            .bst = 198,
            .weapon = ARCANE_WEAPON,
            .assist = NO_ASSIST,
            .special = TIER_3_SPECIAL,
            .a_skill = UNIQUE_A_SKILL,
            .b_skill = TIER_4_B_SKILL,
            .c_skill = TIER_4_C_SKILL,
            .seal = DISTANT_COUNTER
        },
        [2] = {
            .rarity = 5, .level = 40,
            .bst = 194,
            .weapon = UNIQUE_WEAPON,
            .assist = MOVEMENT_ASSIST,
            .special = TIER_3_SPECIAL,
            .a_skill = TIER_4_A_SKILL,
            .b_skill = TIER_4_B_SKILL,
            .c_skill = TIER_4_C_SKILL,
            .seal = TIER_3_A_SKILL_LOW
        },
        [3] = {
            .rarity = 5, .level = 40,
            .bst = 179,
            .weapon = UNIQUE_WEAPON,
            .assist = UNIQUE_ASSIST,
            .special = TIER_2_SPECIAL,
            .a_skill = TIER_4_A_SKILL,
            .b_skill = TIER_4_B_SKILL,
            .c_skill = TIER_4_C_SKILL,
            .seal = TIER_3_C_SKILL
        }
    };

    print_scores(units, true);

	return 0;
}