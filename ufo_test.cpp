// size: 0x34
struct ShtEntry {
    int8_t __short_timer_modulo; // 0x0
    int8_t __short_timer_value; // 0x1
    int16_t damage; // 0x2
    // 0x4
    int8_t __option_index; // 0x1C
    // 0x1D
    union {
        BulletInitFunc* __init_func; // 0x24
        uint32_t __init_func_index; // 0x24
    };
    union {
        BulletFuncB* __unknown_func_B; // 0x28
        uint32_t __unknown_func_B_index; // 0x28
    };
    union {
        void* __unknown_func_C; // 0x2C
        uint32_t __unknown_func_C_index; // 0x2C
    };
    union {
        BulletDamageFunc* __damage_func; // 0x30
        uint32_t __damage_func_index; // 0x30
    };
    // 0x34
};

// size: 
struct ShtFile {
    unknown_fields(0x2); // 0x0
    uint16_t __unknownA_count; // 0x2
    float __float_4; // 0x4
    float __float_8; // 0x8
    float __float_C; // 0xC
    float movement_speeds[4]; // 0x10
    int32_t max_level; // 0x20
    int32_t power_per_level; // 0x24
    int32_t damage_cap; // 0x28
    // 0x2C
    
    ShtEntry* __entry_array_268[]; // 0x268
};

// size: 0x74
struct PlayerDamageSource {
    float radius; // 0x0
    float radius_delta; // 0x4
    float angle; // 0x8
    float angular_velocity; // 0xC
    Float2 size; // 0x10
    MotionData motion; // 0x18
    Timer duration; // 0x4C
    int32_t damage; // 0x60
    int32_t damage_dealt; // 0x64
    int32_t damage_cap; // 0x68
    int32_t __hit_frequency; // 0x6C
    union {
        uint32_t flags; // 0x70
        struct {
            uint32_t active : 1; // 1
            uint32_t hitbox_type : 1; // 2
        };
    };
    // 0x74
};

// size: 0xE4
struct PlayerOption {
    
};

// size: 0x78
struct PlayerBullet {
    Timer __timer_0; // 0x0
    MotionData motion; // 0x14
    int state; // 0x48
    AnmID __vm_id_4C; // 0x4C
    AnmID __vm_id_50; // 0x50
    // 0x54
    int32_t damage; // 0x60
    // 0x64
    Float2 size; // 0x68
    // 0x70
    ShtEntry* __entry_ptr; // 0x74
    // 0x78
};

static inline constexpr size_t PLAYER_OPTION_COUNT = 8;
static inline constexpr size_t PLAYER_BULLET_COUNT = 256;
static inline constexpr size_t PLAYER_DAMAGE_SOURCE_COUNT = 128;

// size: 0xC59C
struct Player : ZUNTask {
    // ZUNTask base; // 0x0
    AnmLoaded* player_anm; // 0x10
    AnmVM __vm_14; // 0x14
    AnmVM __vm_4C8; // 0x4C8
    Float3 position; // 0x97C
    Int2 internal_position; // 0x988
    int32_t __unfocused_linear_speed; // 0x990
    int32_t __focused_linear_speed; // 0x994
    int32_t __unfocused_diagonal_speed; // 0x998
    int32_t __focused_diagonal_speed; // 0x99C
    Float3 velocity; // 0x9A0
    Float3 __last_movement_velocity; // 0x9AC
    Int2 __internal_velocity; // 0x9B8
    // 0x9C0
    
    Float3 __float3_A08; // 0xA08
    Int2 __base_movement; // 0xA14
    MovementDirection movement_direction; // 0xA1C
    int32_t __int_A20; // 0xA20
    // 0xA24
    int32_t state; // 0xA28
    ShtFile* sht_file; // 0xA2C
    Timer __death_timer; // 0xA30
    Timer __timer_A44; // 0xA44 (UM = __timer_28)
    PlayerBullet bullets[PLAYER_BULLET_COUNT]; // 0xA58
    AnmID __vm_id_8258; // 0x8258 (UM = __vm_id_47090)
    float __speed_modifier; // 0x825C
    PlayerOption options[PLAYER_OPTION_COUNT]; // 0x8260
    int __dword_8980; // 0x8980 (UM = __dword_47944)
    char __byte_8984; // 0x8984 (UM = __byte_47948)
    probably_padding_bytes(0x3); // 0x8985
    PlayerDamageSource damage_sources[PLAYER_DAMAGE_SOURCE_COUNT + 1]; // 0x8988
    int __int_C3FC; // 0xC3FC (UM = __int_471C4)
    Timer __timer_C400; // 0xC400 (UM = __timer_47154)
    union {
        uint32_t flags; // 0xC414
        struct {
            uint32_t : 3;
            uint32_t __unknown_flag_C : 1;
        };
    };
    int32_t __int_C418; // 0xC418 (UM = __int_471C8)
    // 0xC41C
    
    Timer shoot_key_short_timer; // 0xC420
    int32_t __int_array_C434[]; // 0xC434 size unknown
    
    BOOL focused; // 0xC598
    // 0xC59C
};

// 0x4390F0
PlayerDamageSource* Player::create_damage_source_circle(Float3* position, float radius, float radius_delta, int32_t duration, int32_t damage);

// 0x4B4230
static BulletInitFunc* PLAYER_BULLET_INIT_FUNCS[8];

// 0x4AEBD4
static BulletFuncB* PLAYER_FUNC_TABLE_B[7];

// 0x4AEE10
static BulletDamageFunc* PLAYER_BULLET_DAMAGE_FUNCS[2];

// 0x4CE8AC
static void* PLAYER_FUNC_TABLE_C[1];