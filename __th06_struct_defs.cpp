// Static: RNG 0x69D8F8
struct zRng {
    uint16_t rng_value;  // 0x0
	unsigned char __the_only_field_i_100_percent_know_is_padding[0x2];  // 0x2
    uint32_t rng_index;  // 0x4
};

// Static: INPUT 0x69D904
struct zInput {
    uint16_t __word_0;
	unsigned char __probably_padding[0x2];
    uint16_t __word_4;
	unsigned char __probably_padding[0x2];
    uint16_t __word_8;
	unsigned char __probably_padding[0x2];
    uint16_t __word_C;
	unsigned char __probably_padding[0x2];
};

struct zConfig {
    unsigned char __unknown[0x14];  // 0x0
    uint32_t version;  // 0x14
    uint8_t starting_lives;  // 0x18
    uint8_t starting_bombs;  // 0x19
    uint8_t graphic_format;  // 0x1a
    uint8_t bgm_format;  // 0x1b
    uint8_t __byte_1c;  // 0x1c
    uint8_t __byte_1d;  // 0x1d
    uint8_t fullscreen;  // 0x1e
    uint8_t __frameskip_setting;  // 0x1f
    uint16_t deadzone_x;  // 0x20
    uint16_t deadzone_y;  // 0x22
    unsigned char __unknown[0x10];  // 0x24
    uint32_t flags;  // 0x34
};  // 0x38

// Size unknown
struct zMidiManager {
    unsigned char __unknown[0x2C8];
    int __dword_2C8;
    unsigned char __unknown[0x14];
    int __dword_2DC;
    int __dword_2E0;
    int __dword_2E4;
    int __dword_2E8;
};

struct zSupervisor {
    HINSTANCE current_instance;  // 0x0
    IDirect3D8* d3d;  // 0x4
    IDirect3DDevice8* d3d_device;  // 0x8
    unsigned char __unknown[0x8];  // 0xc
    void* __dinput_related_ptr_1;  // 0x14
    unsigned char __unknown[0x2C];  // 0x18
    HWND main_window;  // 0x44
    D3DMATRIX view_matrix;  // 0x48
    D3DMATRIX projection_matrix;  // 0x88
    D3DVIEWPORT8 viewport;  // 0xc8
    D3DPRESENT_PARAMETERS present_params;  // 0xe0
    zConfig configA;  // 0x114 (One of these two is the default and the other is the active config)
    zConfig configB;  // 0x14c
    int __dword_184;  // 0x184
    int __dword_188;  // 0x188
    int __dword_18C;  // 0x18c
    int __dword_190;  // 0x190
    unsigned char __unknown[0x4];  // 0x194
    uint32_t __frames_to_disable_something__see_0x419f66;  // 0x198
    int __dword_19C;  // 0x19c
    int __dword_1A0;  // 0x1a0
    int __dword_1A4;  // 0x1a4
    float game_speed;  // 0x1a8
    float slowdown;  // 0x1ac
    zMidiManager* midi_manager_ptr;  // 0x1b0
    float __float_1B4;  // 0x1b4
    float __float_1B8;  // 0x1b8
    unsigned char __unknown[0x244];  // 0x1bc
    int __dword_400;  // 0x400
    D3DCAPS8 device_caps;  // 0x404
};

// Static: UnknownSoundData_Table[0x20] 0x478528
struct zUnknownSoundData {
    int32_t __dword_0;
    int16_t __short_4;
    int16_t __short_6;
};

struct zSoundManager {
    IDirectSound* dsound_api;  // 0x0
    unsigned char __unknown[0x4];  // 0x4
    IUnknown* __ptrs_8[0x80];  // 0x8
    IUnknown* __ptrs_208[0x80];  // 0x208
    int32_t __array_408[0x80];  // 0x408
    void* __ptr_608;  // 0x608
    HWND window;  // 0x60c
    IDirectSound** dsound;  // 0x610
    int __dword_614;  // 0x614
    int __dword_618;  // 0x618
    unsigned char __unknown[0x4];  // 0x614
    int32_t __sound_index_array[0x3];  // 0x620
    CSound* c_sound_ptr;  // 0x62c
    int __dword_630;  // 0x630
    unsigned char __unknown[0x4];  // 0x634
};

struct zAnmLoadedSprite {
    int32_t source_file_index;  // 0x0
    zFloat2 start_pixel_inclusive;  // 0x4
    zFloat2 end_pixel_inclusive;  // 0xc
	Float2Inv texture_size;  // 0x14
    zFloat2 uv_start;  // 0x1c
    zFloat2 uv_end;  // 0x24
	Float2Inv sprite_size;  // 0x2c
    int32_t sprite_id;  // 0x34
};  // 0x38

struct zAnmRawInstr {
    int16_t time;  // 0x0
    uint8_t opcode;  // 0x2
    uint8_t argsize;  // 0x3
    unsigned char args[];  // 0x4
};  // 0x2c

struct zAnmRawEntry {
    int32_t num_sprites;  // 0x0
    int32_t num_scripts;  // 0x4
    int32_t rt_texture_slot;  // 0x8
    uint32_t width;  // 0xc
    uint32_t height;  // 0x10
    uint32_t format;  // 0x14
    D3DCOLOR colorkey;  // 0x18
    char* path_1;  // 0x1c
    uint32_t __unknown__used_at_runtime;  // 0x20
    char* path_2;  // 0x24
    uint32_t version;  // 0x28
    uint32_t memory_priority;  // 0x2c
    zAnmRawTexture* texture;  // 0x30
    uint16_t __has_data__unused_in_EoSD;  // 0x34
    uint16_t __unused_2;  // 0x36
    uint32_t offset_to_next;  // 0x38
    uint32_t __unused_3;  // 0x3c
    zAnmRawSprite* sprites[0xA];  // 0x40
    zAnmRawScript scripts[0xA];  // 0x68
};  // 0xb8

// This is literally just because I got annoyed at the inverted x/y floats
struct zFloat2Inv {
	float y;
	float x;
};

struct zAnmVm {
    zFloat3 rotation;  // 0x0
    zFloat3 angle_vel;  // 0xc
	zFloat2Inv scale;  // 0x18
	zFloat2Inv scale_interp_final;  // 0x20
    zFloat2 uv_scroll_pos;  // 0x28
    zTimer script_time;  // 0x30
	D3DMATRIX __matrix;  // 0x3c
    D3DCOLOR color;  // 0x7c
    union {
        uint16_t flags;  // 0x80
        struct {
            uint16_t visible : 1; // 0x0001
            uint16_t __unknown_flag_B : 1; // 0x0002
            uint16_t additive_blending : 1; // 0x0004
            uint16_t __unknown_flag_D : 1; // 0x0008
            uint16_t __unknown_flag_E : 1; // 0x0010
            uint16_t position_mode : 1; // 0x0020
            uint16_t flip_x : 1; // 0x0040
            uint16_t flip_y : 1; // 0x0080
            uint16_t anchor_mode : 2; // 0x0100-0x0200
            uint16_t move_mode : 2; // 0x0400-0x0800
            uint16_t z_write_disable : 1; // 0x1000
            uint16_t stop_enable : 1; // 0x2000
        };
    };
	unsigned char __probably_padding[0x2];
    int16_t alpha_interp_end_time;  // 0x84
    int16_t scale_interp_end_time;  // 0x86
    uint16_t __auto_rotate;  // 0x88
    int16_t pending_interrupt_label;  // 0x8a
    int16_t position_interp_end_time;  // 0x8c
    unsigned char __unknown[0x2];  // 0x8e
    zFloat3 position;  // 0x90
	zFloat2Inv scale_interp_initial;  // 0x9c
    zTimer scale_interp_timer;  // 0xa4
    int16_t sprite_number;  // 0xb0
    unsigned char __unknown[0x2];  // 0xb2
    int16_t anm_file_index;  // 0xb4
    unsigned char __unknown[0x2];  // 0xb6
    zAnmRawInstr* beginning_of_script;  // 0xb8
    zAnmRawInstr* current_instr;  // 0xbc
    zAnmLoadedSprite* sprite;  // 0xc0
    D3DCOLOR alpha_interp_initial;  // 0xc4
    D3DCOLOR alpha_interp_final;  // 0xc8
    zFloat3 position_interp_initial;  // 0xcc
    zFloat3 position_interp_final;  // 0xd8
    zFloat3 __position_2;  // 0xe4
    zTimer position_interp_timer;  // 0xf0
    int32_t time_of_last_sprite_set;  // 0xfc
    zTimer alpha_interp_timer;  // 0x100
    uint8_t font_width;  // 0x10c
    uint8_t font_height;  // 0x10d
    unsigned char __probably_padding[0x2];  // 0x10e
};

struct zAnmManager {
    zAnmLoadedSprite sprites[0x800];  // 0x0
    zAnmVm __mystery_vm;  // 0x1c000
    IDirect3DTexture8* textures[0x108];  // 0x1c110
    void* image_data_array[0x100];  // 0x1c530
    int32_t __maybe_loaded_sprite_count;  // 0x1c930
    zAnmRawInstr* scripts[0x800];  // 0x1c934
    int32_t sprite_indices[0x800];  // 0x1e934
    zAnmRawEntry* anm_files[0x80];  // 0x20934
    uint32_t anm_file_sprite_index_offsets[0x80];  // 0x20b34
    IDirect3DSurface8* surfaces_a[0x20];  // 0x20d34
    IDirect3DSurface8* surfaces_b[0x20];  // 0x20db4
    D3DXIMAGE_INFO surface_source_info[0x20];  // 0x20e34
    D3DCOLOR current_texture_factor;  // 0x210b4
    IDirect3DTexture8* current_texture;  // 0x210b8
    uint8_t current_blend_mode;  // 0x210bc
    uint8_t current_colorop;  // 0x210bd
    uint8_t current___uhhhhh;  // 0x210be
    uint8_t current_zwrite_disable;  // 0x210bf
    zAnmLoadedSprite* current_sprite;  // 0x210c0
    IDirect3DVertexBuffer8* stream_source;  // 0x210c4
    zRenderVertex142 __some_vertex_buffer[0x4];  // 0x210c8
    int32_t __wierd_texture_height;  // 0x21128
};

struct zAsciiManagerPopup {
    char digits[0x8];  // 0x0
    zFloat3 position;  // 0x8
    D3DCOLOR color;  // 0x14
    zTimer timer;  // 0x18
    uint8_t in_use;  // 0x24
    uint8_t character_count;  // 0x25
    unsigned char __unknown[0x2];  // 0x26
};  // 0x28

struct zAsciiManager {
    zAnmVm __vm_0;  // 0x0
    zAnmVm __vm_110;  // 0x110
    zAsciiManagerString strings[0x100];  // 0x220
    int32_t num_strings;  // 0x6220
    D3DCOLOR color;  // 0x6224
    zFloat2 scale;  // 0x6228
    unsigned char __unknown[0x8];  // 0x6230
    int32_t next_popupA_index;  // 0x6238
    int32_t next_popupB_index;  // 0x623c
    unsigned char __unknown[0x4];  // 0x6240
    AsciiManagerBigChild __child_6244;  // 0x6244
    AsciiManagerBigChild __child_69BC;  // 0x69bc
    // Technically the original game was written with an array
    // of 515 popups and ZUN just looped the first 512 and the
    // last 3 separately. This is evident from the +512 found at
    // address 0x401A8B.
    // Screw that though, I'm doing it the sane way.
    zAsciiManagerPopup popupsA_7134[0x200];  // 0x7134
    zAsciiManagerPopup popupsB_C134[0x3];  // 0xc134
};

// Static: EffectData_Table[0x14] 0x476298
struct zEffectData {
    int32_t anm_script_index;
    int __dword_4;
};

struct zEffect {
    zAnmVm vm;  // 0x0
    zFloat3 __float3_110;  // 0x110
    unsigned char __unknown[0x18]; // 0x11C
    zFloat3 __float3_134;  // 0x134
    zFloat3 position;  // 0x140
    unsigned char __unknown[0x10];  // 0x14C
    float __float_15C;  // 0x15c
    float __float_160;  // 0x160
    zTimer timer;  // 0x164
    unsigned char __unknown[0x4];  // 0x170
    int __dword_174;  // 0x174
    int8_t in_use_flag;  // 0x178
    int8_t effect_id;  // 0x179
    char __byte_17A;  // 0x17a
    char __byte_17B;  // 0x17b
};  // 0x17c

struct zEffectManager {
    int32_t next_index;  // 0x0
    unsigned char __unknown[0x4];  // 0x4
    zEffect effects[0x200];  // 0x8
    zEffect dummy_effect_for_failed_spawns;
};

// This one might be better named zFloatRect or something
struct zPlayerRect {
    zFloat2 position; // 0x0
    zFloat2 size; // 0x8
};

struct zShottypeData {
    float __float_0;
    float __float_4;
    float __float_8;
    float __float_C;
    void* __shot_func_A;
    void* __shot_func_B;
};

// Used to be zPlayerArrayItem
struct zPlayerBullet {
    zAnmVm primary_vm;  // 0x0
    zFloat3 position;  // 0x110
    zFloat3 size;  // 0x11c
    float __float_128;  // 0x128
    float __float_12C;  // 0x12c
    float __float_130;  // 0x130
    float __float_134;  // 0x134
    float __float_138;  // 0x138
    float __float_13C;  // 0x13c
    zTimer __timer_140;  // 0x140
    int16_t __short_14C;  // 0x14c
    int16_t state;  // 0x14e
    int16_t __short_150;  // 0x150
    short __word_152;  // 0x152
    short __word_154;  // 0x154
    unsigned char __probably_padding[0x2];
};

struct zPlayerInner {
    int __dword_0;
    int __dword_4;
    zTimer __timer_8;
    void* __bomb_func_14;
    void* __bomb_func_18;
    unsigned char __unknown[0x44];
    int __dword_array_5C[0x8];
    unsigned char __unknown[0x40];
    int __dword_array_BC[0x8];
    unsigned char __unknown[0x40];
    zAnmVm __vms_11C[0x20];
};

struct zPlayer {
    zAnmVm vm_0;  // 0x0
    zAnmVm vms_1[0x3];  // 0x110
    zFloat3 position;  // 0x440
    unsigned char __unknown[0xC];  // 0x44c
    zFloat3 __float3_458;  // 0x458
    zFloat3 __float3_464;  // 0x464
    zFloat3 __float3_470;  // 0x470
    zFloat3 __float3_47C;  // 0x47c
    zFloat3 __float3_488;  // 0x488
    zFloat3 __float3_494;  // 0x494
    zFloat3 __float3_array_4A0[0x2];  // 0x4A0
    zFloat3 __position_array_4B8[0x20];  // 0x4B8
    zFloat3 __size_array_638[0x20];  // 0x638
    int __dword_array_7B8[0x20];  // 0x7b8
    int __dword_array_838[0x20];  // 0x838
    zPlayerRect __player_rect_array_8B8[0x10];  // 0x8b8
    zTimer __timer_array_9B8[0x2];  // 0x9b8
    float __float_9D0;  // 0x9d0
    float __float_9D4;  // 0x9d4
    int __dword_9D8;  // 0x9d8
    int __dword_9DC;  // 0x9dc
    char __byte_9E0;  // 0x9e0
    char __byte_9E1;  // 0x9e1
    char __byte_9E2;  // 0x9e2
    char __byte_9E3;  // 0x9e3
    uint8_t __byte_9E4;  // 0x9e4
    unsigned char __unknown[0x3];  // 0x9e5
    zTimer __timer_9E8;  // 0x9e8
    zShottypeData __shottype_data;  // 0x9f4
    int __dword_A0C;  // 0xa0c
    float __float_A10;  // 0xa10
    int __dword_A14;  // 0xa14
    short __word_A18;  // 0xa18
    unsigned char __unknown[0x2];  // 0xa1a
    zFloat3 position_of_last_enemy_hit;  // 0xa1c
    zPlayerBullet bullets[0x50];  // 0xa28
    zTimer __timer_75A8;  // 0x75a8
    zTimer __timer_75B4;  // 0x75b4
    void* __shot_func_75C0;  // 0x75c0
    void* __shot_func_75C4;  // 0x75c4
    zPlayerInnerD __player_inner;
    zUpdateFunc* on_tick;  // 0x98e4
    zUpdateFunc* on_draw_1;  // 0x98e8
    zUpdateFunc* on_draw_2;  // 0x98ec
};

enum ItemID {
    PowerItem = 0,
    PointItem = 1,
    BigPowerItem = 2,
    BombItem = 3,
    FItem = 4,
    LifeItem = 5,
    StarItem = 6
};

struct zEclRawInstr {
	uint32_t time;
	int16_t opcode;
	int16_t offset_to_next;
	uint8_t unused;
	uint8_t difficulty_mask;
	uint16_t param_mask;
	unsigned char args[];
};

struct zEnemyEclContext {
	zEclRawInstr* current_instr;  // 0x0
	zTimer time;  // 0x4
    ins_call func;  // 0x10
	int32_t int_vars[0x4];  // 0x14
	float float_vars[0x4];  // 0x24
	int32_t more_int_vars[0x4];  // 0x34
	int32_t compare_register;  // 0x44
	uint16_t sub_id;  // 0x48
    unsigned char __probably_padding[0x2];  // 0x4A
};  // 0x4c

struct zScorefileCatk {
    char magic__CATK[0x4];  // 0x0
    uint16_t size_1;  // 0x4
    uint16_t size_2;  // 0x6
    uint8_t __byte_8;  // 0x8
    unsigned char __probably_padding[0x3];  // 0x9
    int32_t highscore;  // 0xc
    short __word_10;  // 0x10
    uint8_t spell_name_hash;  // 0x12
    uint8_t most_recent_capture_shottype;  // 0x13
    unsigned char __unknown[0x4];  // 0x14
    char spell_name[0x24];  // 0x18
    short attempts;  // 0x3c
    short captures;  // 0x3e
};  // 0x40

struct zGameManager  {
    int __dword_0;  // 0x0
    int32_t score;  // 0x4
    int __dword_8;  // 0x8
    int32_t high_score;  // 0xc
    int32_t difficulty;  // 0x10
    int32_t graze_in_stage;  // 0x14
    int __dword_18;  // 0x18
    int32_t __spellcard_related_1;  // 0x1c
    int __dword_20;  // 0x20
    int __dword_24;  // 0x24
    int __dword_28;  // 0x28
    char __byte_2C;  // 0x2c
    unsigned char __probably_padding[0x3];  // 0x2d
    zScorefileCatk catk_data[0x40];  // 0x30
    zScorefileClrd clrd_data[0x4];  // 0x1030
    zScorefilePscr pscr_data[0x60];  // 0x1090
    int16_t current_power;  // 0x1810
    unsigned char __probably_padding[0x2];  // 0x1812
    uint16_t point_items_collected_in_stage;  // 0x1814
    short __word_1816;  // 0x1816
    char __byte_1818;  // 0x1818
    int8_t power_item_count_for_score;  // 0x1819
    int8_t lives_remaining;  // 0x181a
    int8_t bombs_remaining;  // 0x181b
    char __byte_181C;  // 0x181c
    int8_t character;  // 0x181d
    int8_t shottype;  // 0x181e
    char __byte_181F;  // 0x181f
    char __byte_1820;  // 0x1820
    char __byte_1821;  // 0x1821
    char __byte_1822;  // 0x1822
    uint8_t __byte_1823;  // 0x1823
    char demoplay_mode;  // 0x1824
    unsigned char __unknown[0x7];  // 0x1825
    char replay_filename[0x100];  // 0x182c
    char __probably_a_filename[0x100];  // 0x192c
    short __word_1A2C;  // 0x1a2c
    unsigned char __unknown[0x6];  // 0x1a2e
    int32_t current_stage;  // 0x1a34
    int __dword_1A38;  // 0x1a38
    zFloat2 arcade_region_top_left_pos;  // 0x1a3c
    zFloat2 arcade_region_size;  // 0x1a44
    zFloat2 __float2_1A4C;  // 0x1a4c
    zFloat2 __float2_1A54;  // 0x1a54
    float __float_1A5C;  // 0x1a5c
    zFloat3 stage_camera_facing_dir;  // 0x1a64
    int32_t __int_1A6C;  // 0x1a6c
	int32_t rank;  // 0x1a70
	int32_t max_rank;  // 0x1a74
	int32_t min_rank;  // 0x1a78
	int32_t subrank;  // 0x1a7c
};

struct zMsgRawInstr {
    uint16_t time;  // 0x0
    uint8_t opcode;  // 0x2
    uint8_t args_size;  // 0x3
    unsigned char args[];
};  // 0x4

struct zGuiMsgVM {
    zMsgRawHeader* msg_file;  // 0x0, 0x2534
    zMsgRawInstr* current_instr;  // 0x4, 0x2538
    int32_t current_msg_index;  // 0x8, 0x253C
    zTimer script_time;  // 0xc, 0x2B40
    int32_t frames_elapsed_during_pause;  // 0x18, 0x254C
    zAnmVm portraits[0x2];  // 0x1c, 0x2550
    zAnmVm dialogue_lines[0x2];  // 0x23c, 0x2770
    zAnmVm intro_lines[0x2];  // 0x45c, 0x2990
    D3DCOLOR text_colors_A[0x2];  // 0x67c, 0x2BB0 (Probably actually 4 length array, but only 2 used)
    unsigned char __unknown[0x8];  // 0x684
    D3DCOLOR text_colors_B[0x2];  // 0x68c, 0x2BC0 (Probably actually 4 length array, but only 2 used)
    unsigned char __unknown[0x8];  // 0x694
    uint32_t font_size;  // 0x69c, 0x2BD0
    uint32_t __counter_incremented_by_enemyAppear;  // 0x6a0, 0x2BD4
    uint8_t dialog_skippable;  // 0x6a4, 0x2BD8
    unsigned char __unknown[0x3];  // 0x6a5
};  // 0x67c

struct zGuiImpl {
    zAnmVm __vms_0[0x1A];  // 0x0
    uint8_t __byte_1BA0;  // 0x1ba0
    unsigned char __unknown[0x3];  // 0x1ba1
    zAnmVm __vm_1BA4;  // 0x1ba4
    zAnmVm __vm_1CB4;  // 0x1cb4
    zAnmVm __vm_1DC4;  // 0x1dc4
    zAnmVm __vm_1ED4;  // 0x1ed4
    zAnmVm __vm_1FE4;  // 0x1fe4
    zAnmVm __vm_20F4;  // 0x20f4
    zAnmVm __vm_2204;  // 0x2204
    zAnmVm __vm_2314;  // 0x2314
    zAnmVm __vm_2424;  // 0x2424
    zGuiMsgVM msg;  // 0x2534
    int __dword_2BDC;  // 0x2bdc
    int32_t stage_score;  // 0x2be0
    GuiImplChildB __child_b_2BE4;  // 0x2be4
    GuiImplChildB __child_b_2C04;  // 0x2c04
    GuiImplChildB __child_b_2C24;  // 0x2c24
};

struct zGui {
    uint32_t __flags_0;  // 0x0
    zGuiImpl* impl;  // 0x4
    float __float_8;  // 0x8
    float blue_spellcard_bar_length;  // 0xc
    uint32_t __uint_10;  // 0x10
    int32_t __ecl_set_lives;  // 0x14
    int32_t __read_by_spellcard_ecl;  // 0x18
    unsigned char __unknown[0x4];  // 0x1c
    bool boss_present;  // 0x20
    unsigned char __unknown[0x3];  // 0x21
    float __boss_health_bar_24;  // 0x24
    float __boss_health_bar_28;  // 0x28
};

struct zStdRawQuadBasic {
    int16_t type;  // 0x0
    int16_t offset_to_next;  // 0x2
    uint16_t anm_script;  // 0x4
    int16_t vm_index;  // 0x6
    zFloat3 position;  // 0x8
    zFloat2 size;  // 0x14
};  // 0x1c

struct zStdRawObject {
    int16_t id;  // 0x0
    uint8_t __unknown_always_zero;  // 0x2
    union {
        int8_t flags;  // 0x3
        struct {
            int8_t active : 1;
        };
    };
    zFloat3 posisiton;  // 0x4
    zFloat3 __size;  // 0x10
    zStdRawQuadBasic first_quad;  // 0x1c
    unsigned char __unknown[0x4];  // 0x38
};  // 0x3c

struct zStdRawInstr {
    int32_t time;  // 0x0
    int16_t opcode;  // 0x4
    uint16_t args_size;  // 0x6
    int32_t args[3];  // 0x8
};

struct zStdRawHeader {
    uint16_t num_objects;  // 0x0
    uint16_t num_quads;  // 0x2
    uint32_t offset_to_instances;  // 0x4
    uint32_t offset_to_script;  // 0x8
    uint32_t __dword_C;  // 0xc
    char stage_name[0x80];  // 0x10
    char bgm_names[0x4][0x80];  // 0x90
    char bgm_paths[0x4][0x80];  // 0x290
    zStdRawObject* objects[0x10];  // 0x490
};  // 0x4d0

// Why TF does D3D SetRenderState pass floats as a DWORD?
// I'm not going
union D3DFLOATDWORD {
	float good;
	DWORD bad;
};
struct zStageCameraSky {
	D3DFLOATDWORD near_plane;  // 0x0
    D3DFLOATDWORD far_plane;  // 0x4
    D3DCOLOR color;  // 0x8
};  // 0xc

struct zStage {
    zAnmVm* quad_vms;  // 0x0
    zStdRawHeader* std_file;  // 0x4
    int32_t num_quads;  // 0x8
    int32_t num_objects;  // 0xc
    zStdRawObject** objects;  // 0x10
    zStdRawInstance* object_instances;  // 0x14
    zStdRawInstr* beginning_of_script;  // 0x18
    zTimer script_time;  // 0x1c
    int32_t instr_index;  // 0x28
    zTimer __timer_2C;  // 0x2c
    unsigned char __unknown[0x4];  // 0x38
    zFloat3 position;  // 0x3c
    zStageCameraSky sky_fog;  // 0x48
    zStageCameraSky sky_fog_interp_initial;  // 0x54
    zStageCameraSky sky_fog_interp_final;  // 0x60
    int32_t sky_fog_interp_duration;  // 0x6c
    zTimer sky_fog_interp_timer;  // 0x70
    unsigned char __unknown[0x4];  // 0x7c
    int32_t __set_by_spellcard_ecl_80;  // 0x80
    int32_t __set_by_spellcard_ecl_84;  // 0x84
    zAnmVm __vm_88;  // 0x88
    zAnmVm __vm_198;  // 0x198
    uint8_t unpause_flag;  // 0x2a8
    unsigned char __unknown[0x3];  // 0x2a9
    zFloat3 facing_dir_interp_initial;  // 0x2ac
    zFloat3 facing_dir_interp_final;  // 0x2b8
    int32_t facing_dir_interp_duration;  // 0x2c4
    zTimer facing_dir_interp_timer;  // 0x2c8
    zFloat3 position_interp_final;  // 0x2d4
    int32_t position_interp_end_time;  // 0x2e0
    zFloat3 position_interp_initial;  // 0x2e4
    int32_t position_interp_start_time;  // 0x2f0
};

struct zEnemyBulletShooter {
    uint16_t sprite;  // 0x0
    uint16_t color;  // 0x2
    zFloat3 position;  // 0x4
    float angle1;  // 0x10
	float angle2;  // 0x14
	float speed1;  // 0x18
	float speed2;  // 0x1c
    float float_data[0x4];  // 0x20
    int32_t int_data[0x4];  // 0x30
    unsigned char __unknown[0x4];  // 0x40
    int16_t count_1;  // 0x44
    int16_t count_2;  // 0x46
    uint16_t aim_mode;  // 0x48
    short __word_4A;  // 0x4a
    union {
        uint32_t flags;  // 0x4c
        struct {
            uint32_t : 1;
            uint32_t play_sound : 1;
            uint32_t : 21;
            uint32_t play_sound_on_spawn : 1;
        };
    };
    int32_t sfx;  // 0x50
};  // 0x54

struct zEnemyLaserShooter {
    uint16_t sprite;  // 0x0
    uint16_t color; // 0x2
    zFloat3 position;  // 0x4
    float angle;  // 0x10
    unsigned char __unknown[0x4];  // 0x14
    float speed;  // 0x18
    unsigned char __unknown[0x4];  // 0x1c
    float start_offset;  // 0x20
    float end_offset;  // 0x24
    float start_length;  // 0x28
    float width;  // 0x2c
    int32_t start_time;  // 0x30
    int32_t duration;  // 0x34
    int32_t stop_time;  // 0x38
    int32_t graze_delay;  // 0x3c
    int32_t graze_distance;  // 0x40
    unsigned char __unknown[0x4];  // 0x44
    uint16_t type;  // 0x48
    unsigned char __unknown[0x2];  // 0x4a
    int __dword_4C;  // 0x4c
    unsigned char __unknown[0x4];  // 0x50
};  // 0x54

struct zLaser {
    zAnmVm vm_0;  // 0x0
    zAnmVm vm_1;  // 0x110
    zFloat3 position;  // 0x220
    float angle;  // 0x22C
    float start_offset;  // 0x230
    float end_offset;  // 0x234
    float start_length;  // 0x238
    float width;  // 0x23c
    float speed;  // 0x240
    int32_t start_time;  // 0x244
    int32_t graze_delay;  // 0x248
    int32_t duration;  // 0x24c
    int32_t end_time;  // 0x250
    int32_t graze_interval;  // 0x254
    int32_t in_use;  // 0x258
    zTimer timer;  // 0x25c
    unsigned char __unknown[0x2]; // 0x268
    uint16_t color;  // 0x26a
    uint8_t state;  // 0x26c
    unsigned char __unknown[0x3];  // 0x26d
};  // 0x270

enum EnemyMoveMode {
    PerAxisSpeed = 0,
    Velocity = 1,
    Interpolate = 2
};

enum EnemyInterpMode {
    Constant = 0,
    Decelerate = 1,
    DecelerateFast = 2,
    Accelerate = 3,
    AccelerateFast = 4
};

struct zEnemy {
    zAnmVm primary_vm;  // 0x0
    zAnmVm vms[0x8];  // 0x110
    zEnemyEclContext current_context;  // 0x990
    zEnemyEclContext saved_context_stack[0x8];  // 0x9dc
    int32_t stack_depth;  // 0xc3c
    unsigned char __unknown[0x4];  // 0xc40
    int32_t death_callback_sub;  // 0xc44
    int32_t interrupts[0x8];  // 0xc48
    int32_t run_interrupt;  // 0xc68
    zFloat3 position;  // 0xc6c
    zFloat3 hitbox_dimensions;  // 0xc78
    zFloat3 axis_speed;  // 0xc84
    float direction;  // 0xc90
    float rotation;  // 0xc94
    float speed;  // 0xc98
    float acceleration;  // 0xc9c
    zFloat3 shoot_offset;  // 0xca0
    zFloat3 move_interp;  // 0xcac
    zFloat3 move_interp_start_pos;  // 0xcb8
    zTimer move_interp_timer;  // 0xcc4
    int32_t move_interp_start_time;  // 0xcd0
	float bullet_rank_speed_low;  // 0xcd4
	float bullet_rank_speed_high;  // 0xcd8
	uint16_t bullet_rank_amount1_low;  // 0xcdc
	uint16_t bullet_rank_amount1_high;  // 0xcde
	uint16_t bullet_rank_amount2_low;  // 0xce0
	uint16_t bullet_rank_amount2_high;  // 0xce2
    int32_t life;  // 0xce4
    int32_t max_life;  // 0xce8
    int32_t score;  // 0xcec
    zTimer boss_timer;  // 0xcf0
    D3DCOLOR __color_CFC;  // 0xcfc
    zEnemyBulletShooter bullet_properties;  // 0xd00
    int32_t shoot_interval;  // 0xd54
    zTimer shoot_interval_timer;  // 0xd58
    zEnemyLaserShooter laser_properties;  // 0xd64
    zLaser* lasers[0x20];  // 0xdb8
    int32_t laser_store;  // 0xe38
	uint8_t death_anm_one;  // 0xe3c
	uint8_t death_anm_two;  // 0xe3d
	uint8_t death_anm_three;  // 0xe3e
    uint8_t item_drop;  // 0xe3f
    int8_t boss_id;  // 0xe40
    uint8_t __byte_E41;  // 0xe41
    unsigned char __unknown[0x2];  // 0xe42
    zTimer __timer_e44;  // 0xe44
    union {
        uint8_t flags_E50;  // 0xe50
        struct {
            uint8_t move_mode : 2;  // 0, 0x03, 0xFC
            uint8_t interp_mode : 3;  // 2, 0x1C, 0xE3
            uint8_t cannot_shoot : 1;  // 5, 0x20, 0xDF
            uint8_t mirrored : 1;  // 6, 0x40, 0xBF
            uint8_t active : 1;  // 7, 0x80, 0x7F
        };
    };
    union {
        uint8_t flags_E51;  // 0xe51
        struct {
            uint8_t can_interact : 1;  // 0, 0x01, 0xFE
            uint8_t contact_damage : 1;  // 1, 0x02, 0xFD
            uint8_t __offscreen_related : 1;  // 2, 0x04, 0xFB
            uint8_t is_boss : 1;  // 3, 0x08, 0xF7
            uint8_t can_take_damage : 1;  // 4, 0x10, 0xEF
            uint8_t death_flags : 3;  // 5, 0xE0, 0x1F
        };
    };
    union {
        uint8_t flags_E52;  // 0xe52
        struct {
            uint8_t move_limit : 1;  // 0, 0x01, 0xFE
            uint8_t anm_rotate : 1;  // 1, 0x02, 0xFD
            uint8_t disable_call_stack : 1;  // 2, 0x04, 0xFB
            uint8_t invisible : 1;  // 3, 0x08, 0xF7
            uint8_t timeout_spell : 1;  // 4, 0x10, 0xEF
        };
    };
    int8_t anm_ex_flags;  // 0xe53
	int16_t anm_ex_default;  // 0xe54
	int16_t anm_ex_far_left;  // 0xe56
	int16_t anm_ex_far_right;  // 0xe58
	int16_t anm_ex_left;  // 0xe5a
	int16_t anm_ex_right;  // 0xe5c
    unsigned char __probably_padding[0x2);  // 0xe5e (The AnmEx data was probably declared as int16_t[5]]
	zFloat2 lower_move_limit;  // 0xe60
	zFloat2 upper_move_limit;  // 0xe68
    zEffect* __effect_array[0xC];  // 0xe70
    int32_t __effect_index;  // 0xea0
    float __effect_distance;  // 0xea4
    int32_t life_callback_threshold;  // 0xea8
    int32_t life_callback_sub;  // 0xeac
    int32_t timer_callback_threshold;  // 0xeb0
    int32_t timer_callback_sub;  // 0xeb4
    float __float_eb8;  // 0xeb8
    zTimer __timer_ebc;  // 0xebc
};

struct zTimelineRawInstr {
    int16_t time;
    int16_t first_arg;
    int16_t opcode;
    int16_t offset_to_next;
    unsigned char args[];
};

struct zEnemyManager {
    char* stg_enm_anm_filename;  // 0x0
    char* stg_enm2_anm_filename;  // 0x4
    zEnemy template_enemy;  // 0x8
    zEnemy enemies[0x100];  // 0xed0
    zEnemy dummy_enemy_for_failed_spawns;
    zEnemy* bosses[0x8];  // 0xee598
    uint16_t random_item_spawn_index;  // 0xee5b8
    uint16_t random_item_table_index;  // 0xee5ba
    int32_t enemy_count_real;  // 0xee5bc
    unsigned char __unknown[0x4];  // 0xee5c0
    int __dword_EE5C4;  // 0xee5c4
    int32_t __spellcard_capture;  // 0xee5c8
    int32_t __spellcard_bonus;  // 0xee5cc
    int32_t __spellcard_number;  // 0xee5d0
    int __dword_EE5D4;  // 0xee5d4
    unsigned char __unknown[0x4];  // 0xee5d8
    zTimelineRawInstr* timeline_instr;  // 0xee5dc
    zTimer timeline_time;  // 0xee5e0
};

struct zEnemyController {
    void* ecl_file_buffer; // zEclRawHeader?
    zEclRawInstr** sub_table;
    zTimelineRawInstr* current_timeline_instruction;
};

struct zUpdateFunc {
    int16_t priority;  // 0x0
    union {
        uint16_t flags;  // 0x2
        struct {
            uint16_t __flag_A : 1;
        };
    };
    int32_t (cdecl *function_ptr)(void* update_data);  // 0x4
    int32_t (cdecl *on_registration)(void* update_data);  // 0x8
    void (cdecl *on_cleanup)(void* update_data);  // 0xc
    zUpdateFunc* prev;  // 0x10
    zUpdateFunc* next;  // 0x14
    zUpdateFunc* self;  // 0x18
    void* data_ptr;  // 0x1c
};

struct zUpdateFuncRegistry {
    zUpdateFunc on_tick_list_head;
    zUpdateFunc on_draw_list_head;
    uint32_t midi_outputs_count;
    uint32_t __dword_44;
};