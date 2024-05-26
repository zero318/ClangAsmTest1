Float2 Float2::rotate(float angle) {
    float sin_val = sin(angle);
    float cos_val = cos(angle);
    return {
        this->x * cos_val - this->y * sin_val,
        this->x * sin_val + this->y * cos_val
    };
}

Float3 Float3::rotate(float angle) {
    float sin_val = sin(angle);
    float cos_val = cos(angle);
    return {
        this->x * cos_val - this->y * sin_val,
        this->x * sin_val + this->y * cos_val
        // undefined Z value :D
    };
}

struct PlayerDamageArea;

using PlayerDamageArea_UknownFuncA = int32_t(*__fastcall)(PlayerDamageArea* damage_area, Float3* position);

struct PlayerDamageArea {
    unknown_fields(0x10); // 0x0 (-0x74)
    union {
        uint32_t flags; // 0x10 (-0x64)
        struct {
            uint32_t active : 1;
            uint32_t is_circle : 1;
            uint32_t __unknown_flag_B : 1;
        };
    };
    float radius; // 0x14 (-0x60)
    unknown_fields(0x4); // 0x18 (-0x5C)
    float angle; // 0x1C (-0x58)
    unknown_fields(0x4); // 0x20 (-0x54)
    Float2 hitbox_size; // 0x24 (-0x50)
    MotionData motion; // 0x2C (-0x48)
    Timer __timer_70; // 0x70 (-0x4)
    int32_t damage; // 0x84 (0x10)
    int32_t __int_88; // 0x88 (0x14)
    int32_t __int_8C; // 0x8C (0x18)
    int32_t damage_frequency; // 0x90 (0x1C)
    PlayerBullet* parent_bullet; // 0x94 (0x20)
    PlayerDamageArea_UknownFuncA __func_98; // 0x98 (0x24)
    // 0x9C
};

struct Player {
    unknown_fields(0x664); // 0x0
    int __int_664; // 0x664
    int __int_668; // 0x668
};

int32_t __stdcall enm_compute_damage_sources(Float3* position, Float2* hurtbox, BOOL* out, Float3* hit_pos, int idk) {
    Player* player_ptr = PLAYER_PTR;
    
    if (player_ptr->__int_668 == player_ptr->__int_664) {
        return 0;
    }
    
    Float2 top_left = position->as2() - *hurtbox * 0.5; // LOCAL 9, 8
    Float2 bottom_right = position->as2() + *hurtbox * 0.5; // LOCAL 12, 11
    
    if (out) {
        *out = FALSE:
    }
    
    int32_t total_damage; // LOCAL 18
    if (!BOMB_PTR->active) {
        total_damage = 0;
    } else {
        total_damage = /* some bomb function */;
        if (total_damage > 0 && out) {
            *out = TRUE;
        }
    }
    
    for (PlayerDamageArea& damage_area : player_ptr->damage_areas) {
        if (
            damage_area.active &&
            damage_area.__timer_70.current != damage_area.__timer_70.previous &&
            damage_area.__timer_70.is_multiple_of(damage_area.damage_frequency)
        ) {
            if (!damage_area.is_circle) {
                if (damage_area.angle == 0) {
                    // Non-rotated rectangle
                    if (
                        bottom_right.x < damage_area.motion.position.x - damage_area.hitbox_size.x * 0.5 ||
                        top_left.x > damage_area.motion.position.x + damage_area.hitbox_size.x * 0.5 ||
                        bottom_right.y < damage_area.motion.position.y damage_area.hitbox_size.y * 0.5 ||
                        top_left.y > damage_area.motion.position.y + damage_area.hitbox_size.y * 0.5
                    ) {
                        continue;
                    }
                } else {
                    // Rotated rectangle
                    Float3 A = *position - damage_area.motion.position;
                    float angle = -damage_area.angle;
                    Float3 B = A.rotate(angle);
                    Float2 C = fabsf(hurtbox->rotate(angle));
                    
                    if (
                        -damage_area.motion.position.x * 0.5 < C.x * 0.5 + B.x ||
                        damage_area.motion.position.x * 0.5 < C.x * 0.5 - B.x ||
                        -damage_area.motion.position.y * 0.5 < C.y * 0.5 + B.y ||
                        damage_area.motion.position.y * 0.5 < C.y * 0.5 - B.y
                    ) {
                        continue;
                    }
                }
            } else {
                // Circle
                if (damage_area.motion.position.as2().distance_squared(position->as2()) > damage_area.radius * damage_area.radius) {
                    continue;
                }
            }
            
            // Hit is confirmed
            
            if (out && !damage_area.__unknown_flag_B) {
                *out = TRUE;
            }
            
            int32_t damage = damage_area.damage;
            if (idk == 0) {
                if (auto func = damage_area.__func_98) {
                    // This func seems to delete
                    // the corresponding bullet
                    damage = func(damage_area, position);
                }
                damage_area.__int_88 += damage_area.damage;
            }
            total_damage += damage;
            
            if (hit_pos) {
                *hit_pos = damage_area.motion.position;
            }
            
            if (
                damage_area.__int_8C < 9999999 &&
                damage_area.__int_8C <= damage_area.__int_88
            ) {
                damage_area.active = false;
                damage_area.damage = 0;
            }
        }
    }
    
    if (total_damage >= 80) {
        total_damage = 80;
    }
    
    if (idk == 0 && total_damage) {
        // Add to score
    }
    
    return total_damage;
}