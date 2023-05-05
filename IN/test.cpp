#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "../zero/util.h"

#include "../zero/BoundingBox.h"
#include <Windows.h>

#include "../zero/custom_intrin.h"

#include <vector>
#include <bit>

#define SPELL_COUNT (222)

enum DifficultyIDs : int32_t {
	Easy = 0,
	Normal = 1,
	Hard = 2,
	Lunatic = 3,
	Extra = 4,
	Phantasm = 5
};
#define DIFFICULTY_COUNT (6)

enum StageIDs : int32_t {
	Stage1 = 0,
	Stage2 = 1,
	Stage3 = 2,
	Stage4A = 3,
	Stage4B = 4,
	Stage5 = 5,
	Stage6A = 6,
	Stage6B = 7,
	StageEx = 8
};
#define STAGE_COUNT (9)

enum ShottypeIDs : uint8_t {
	ReimuYukari = 0,
	MarisaAlice = 1,
	SakuyaRemilia = 2,
	YoumuYuyuko = 3,
	ReimuSolo = 4,
	YukariSolo = 5,
	MarisaSolo = 6,
	AliceSolo = 7,
	SakuyaSolo = 8,
	RemiliaSolo = 9,
	YoumuSolo = 10,
	YuyukoSolo = 11
};
#define SHOTTYPE_COUNT (12)

// size: 0x3C
struct Config {
	unknown_fields(0x14); // 0x0
	uint32_t version; // 0x14
	uint16_t deadzone_x; // 0x18
	uint16_t deadzone_y; // 0x1A
	uint8_t starting_lives; // 0x1C
	uint8_t starting_bombs; // 0x1D
	uint8_t graphic_format; // 0x1E
	uint8_t bgm_format; // 0x1F
	uint8_t __byte_20; // 0x20
	uint8_t selected_difficulty; // 0x21
	uint8_t fullscreen; // 0x22
	uint8_t frameskip; // 0x23
	uint8_t effect_quality; // 0x24
	uint8_t slow_mode; // 0x25
	uint8_t shot_slow; // 0x26
	uint8_t music_volume; // 0x27
	uint8_t sfx_volume; // 0x28
	unknown_fields(0xF); // 0x29
	uint32_t flags; // 0x38
};
ValidateStructSize(0x3C, Config);

// size: 0xE4
struct Globals {
	int32_t displayed_score; // 0x0
	int32_t graze_in_stage; // 0x4
	int32_t true_score; // 0x8
	int32_t graze; // 0xC
	unknown_fields(0x4); // 0x10
	int32_t displayed_high_score; // 0x14
	uint8_t continues_used_in_high_score; // 0x18
	padding_bytes(0x3); // 0x19
	int __dword_1C; // 0x1C
	int16_t youkai_gauge_copy; // 0x20
	int16_t youkai_gauge; // 0x22
	int32_t point_item_value; // 0x24
	uint8_t clock_time; // 0x28
	uint8_t continues_used; // 0x29
	padding_bytes(0x2); // 0x2A
	int32_t point_items_collected_in_stage; // 0x2C
	int32_t point_items_collected_in_game; // 0x30
	int32_t point_item_extends_so_far; // 0x34
	int32_t next_extend_at; // 0x38
	int32_t current_time_orbs; // 0x3C
	int32_t time_requirement_1; // 0x40
	int32_t __total_time_orbs; // 0x44
	uint32_t RNG_IB[7]; // 0x48
	float death_count_in_game; // 0x64
	float death_count_in_stage; // 0x68
	float RNG_FD[2]; // 0x6C
	float lives_remaining; // 74
	float RNG_FE[2]; // 0x78
	float bombs_remaining; // 0x80
	float bombs_used_in_game; // 0x84
	float bombs_used_in_stage; // 0x88
	float RNG_FF[3]; // 0x8C
	float current_power; // 0x98
	float RNG_FG[2]; // 0x9C
	uint32_t RNG_IA; // 0xA4
	uint32_t RNG_IC[8]; // 0xA8
	uint32_t RNG_value; // 0xC8
	uint32_t RNG_total; // 0xCC
	uint32_t RNG_IH[5]; // 0xD0
};
ValidateStructSize(0xE4, Globals);

// size: 0x20
struct ScorefileFLSP {
	ZUNMagic magic_FLSP; // 0x0
	uint16_t size_1;  // 0x4
	uint16_t size_2;  // 0x6
	uint8_t __byte_8;  // 0x8
	probably_padding_bytes(0x3);  // 0x9
	unknown_fields(0x14); // 0xC
};
ValidateStructSize(0x20, ScorefileFLSP);

// size: 0x9C
struct ScorefileCATKHistory {
	int32_t max_bonus_per_shot[SHOTTYPE_COUNT]; // 0x0
	int32_t max_bonus_best; // 0x30
	int32_t attempts_per_shot[SHOTTYPE_COUNT]; // 0x34
	int32_t attempts_total; // 0x64
	int32_t captures_per_shot[SHOTTYPE_COUNT]; // 0x68
	int32_t captures_total; // 0x98
};
ValidateStructSize(0x9C, ScorefileCATKHistory);

// size: 0x22C
struct ScorefileCATK {
	ZUNMagic magic_CATK; // 0x0
	uint16_t size_1;  // 0x4
	uint16_t size_2;  // 0x6
	uint8_t __byte_8;  // 0x8
	probably_padding_bytes(0x3);  // 0x9
	unknown_bytes(0x3); // 0xC
	uint8_t __byte_F; // 0xF
	char spell_name[48]; // 0x10
	char spell_owner_name[48]; // 0x40
	char spell_comment_line_1[64]; // 0x70
	char spell_comment_line_2[64]; // 0xB0
	ScorefileCATKHistory ingame_history; // 0xF0
	ScorefileCATKHistory spell_practice_history; // 0x18C
	unknown_bytes(0x4); // 0x228
};
ValidateStructSize(0x22C, ScorefileCATK);

// size: 0x24
struct ScorefileCLRD {
	ZUNMagic magic_CLRD; // 0x0
	uint16_t size_1;  // 0x4
	uint16_t size_2;  // 0x6
	uint8_t __byte_8;  // 0x8
	probably_padding_bytes(0x3);  // 0x9
	unknown_fields(0x18); // 0xC
};
ValidateStructSize(0x24, ScorefileCLRD);

// size: 0x178
struct ScorefilePSCR {
	ZUNMagic magic_PSCR; // 0x0
	uint16_t size_1;  // 0x4
	uint16_t size_2;  // 0x6
	uint8_t __byte_8;  // 0x8
	probably_padding_bytes(0x3);  // 0x9
	unknown_fields(0x16C); // 0xC
};
ValidateStructSize(0x178, ScorefilePSCR);

// size: 0x44
struct ScorefilePLSTPlayCounts {
	int32_t total_attempts; // 0x0
	int32_t attempts_per_shot[SHOTTYPE_COUNT]; // 0x4
	int32_t __i_forgor; // 0x34
	int32_t clears; // 0x38
	int32_t continues; // 0x3C
	int32_t practices; // 0x40
};
ValidateStructSize(0x44, ScorefilePLSTPlayCounts);

// size: 0x220
struct ScorefilePLST {
	ZUNMagic magic_PLST; // 0x0
	uint16_t size_1;  // 0x4
	uint16_t size_2;  // 0x6
	uint8_t __byte_8;  // 0x8
	probably_padding_bytes(0x3);  // 0x9
	uint32_t total_hours; // 0xC
	uint32_t total_minutes; // 0x10
	uint32_t total_seconds; // 0x14
	uint32_t total_milliseconds; // 0x18
	uint32_t game_hours; // 0x1C
	uint32_t game_minutes; // 0x20
	uint32_t game_seconds; // 0x24
	uint32_t game_milliseconds; // 0x28
	ScorefilePLSTPlayCounts play_data_per_difficulty[DIFFICULTY_COUNT]; // 0x2C
	ScorefilePLSTPlayCounts play_data_totals; // 0x1C4
	int8_t bgm_unlocked[21]; // 0x208
	padding_bytes(0x3); // 0x21D
};
ValidateStructSize(0x220, ScorefilePLST);

// size: 0x168
struct Scorefile0SCR {
	ZUNMagic magic_0SCR; // 0x0
	uint16_t size_1;  // 0x4
	uint16_t size_2;  // 0x6
	uint8_t __byte_8;  // 0x8
	uint8_t __byte_9; // 0x9
	probably_padding_bytes(0x2);  // 0xA
	int32_t score; // 0xC
	float __float_10; // 0x10
	uint8_t shottype; // 0x14
	uint8_t difficulty; // 0x15
	uint8_t current_stage; // 0x16
	char __string_17[15]; // 0x17
	uint8_t continues_used; // 0x26
	unknown_fields(0x1); // 0x27
	Config config; // 0x28
	int __dword_64; // 0x64
	int __dword_68; // 0x68
	unknown_fields(0x8); // 0x6C
	int __dword_74; // 0x74
	int __dword_78; // 0x78
	unknown_fields(0x8); // 0x7C
	int32_t __int_84; // 0x84
	uint8_t spell_counters[SPELL_COUNT]; // 0x88
	probably_padding_bytes(0x2); // 0x166
};
ValidateStructSize(0x168, Scorefile0SCR);

// size: 0x3DE3C
struct GameManager {
	unknown_fields(0x4); // 0x0
	Config* config_ptr; // 0x4
	Globals* globals_ptr; // 0x8
	ScorefileFLSP flsp_data; // 0xC
	int8_t __sbyte_2C; // 0x2C
	int8_t __sbyte_2D; // 0x2D
	probably_padding_bytes(0x2); // 0x2E
	int32_t difficulty; // 0x30
	int32_t difficulty_mask; // 0x34
	int __dword_38; // 0x38
	unknown_bytes(0x4); // 0x3C
	ScorefileCATK catk_data[SPELL_COUNT * 2]; // 0x40
	ScorefileCLRD clrd_data[13]; // 0x3C490
	ScorefilePSCR pscr_data[12]; // 0x3C664
	ScorefilePLST plst_data; // 0x3D804
	unknown_fields(0x8); // 0x3DA24
	Scorefile0SCR zscr_data; // 0x3DA2C
	int __dword_3DB94; // 0x3DB94
	int __dword_3DB98; // 0x3DB98
	int __dword_3DB9C; // 0x3DB9C
	int __dword_3DBA0; // 0x3DBA0
	int __dword_3DBA4; // 0x3DBA4
	uint8_t character; // 0x3DBA8
	uint8_t shottype; // 0x3DBA9
	uint8_t full_shottype; // 0x3DBAA
	probably_padding_bytes(0x1); // 0x3DBAB
	union {
		uint32_t __flags_3DBAC; // 0x3DBAC
		struct {
			uint32_t practice_mode : 1; // 1
			uint32_t __check_unlocked_info : 1; // 2
			uint32_t : 1; // 3
			uint32_t __debug_related : 1; // 4
			uint32_t __ending_type : 1; // 5
			uint32_t : 1; // 6
			uint32_t __msg_11_related : 1; // 7
			uint32_t : 2; // 8-9
			uint32_t __set_at_end_of_stage : 1; // 10
			uint32_t : 1; // 11
			uint32_t selection_box_choice : 2; // 12-13
			uint32_t : 1; // 14
			uint32_t __something_important : 1; // 15
			uint32_t __show_extra_unlocked : 1; // 16
			uint32_t __show_spell_practice_unlocked : 1; // 17
			uint32_t __show_solo_characters_unlocked : 1; // 18
			// 19-32
		};
	};
	int16_t __short_3DBB0; // 0x3DBB0
	uint8_t __byte_3DBB2; // 0x3DBB2
	uint8_t show_retry_menu; // 0x3DBB3
	uint8_t __byte_3DBB4; // 0x3DBB4
	probably_padding_bytes(0x3); // 0x3DBB5
	int __dword_3DBB8; // 0x3DBB8
	char replay_filename[MAX_PATH * 2]; // 0x3DBBC
	int32_t current_stage; // 0x3DDC4
	int32_t __current_stage_2; // 0x3DDC8
	unknown_fields(0x4); // 0x3DDCC
	uint16_t __ushort_3DDD0; // 0x3DDD0
	unknown_fields(0x2); // 0x3DDD2
	Float2 arcade_region_position; // 0x3DDD4
	Float2 arcade_region_size; // 0x3DDDC
	Float2 __float2_3DDE4; // 0x3DDE4
	Float2 __float2_3DDEC; // 0x3DDEC
	float rng_antitamper; // 0x3DDF4
	int16_t __short_3DDF8; // 0x3DDF8
	int16_t __short_3DDFA; // 0x3DDFA
	short __word_3DDFC; // 0x3DDFC
	short __word_3DDFE; // 0x3DDFE
	short __word_3DE00; // 0x3DE00
	short __word_3DE02; // 0x3DE02
	int32_t __int_3DE04; // 0x3DE04
	unknown_fields(0x8); // 0x3DE08
	int __dword_3DE10; // 0x3DE10
	int32_t __int_3DE14; // 0x3DE14
	int __dword_3DE18; // 0x3DE18
	int __dword_3DE1C; // 0x3DE1C
	int32_t __int_3DE20; // 0x3DE20
	int32_t __int_3DE24; // 0x3DE24
	unknown_fields(0x4); // 0x3DE28
	struct {
		int32_t current;  // 0x3DE2C
		int32_t max;  // 0x3DE30
		int32_t min;  // 0x3DE34
		int32_t subrank;  // 0x3DE38
	} rank;

	// 0x43C4B3
	dllexport gnu_noinline void thiscall select_next_stage() asm_symbol_rel(0x43C4B3) {
		int32_t stage_number = this->current_stage;
		if (stage_number == Stage1) {
			this->current_stage = Stage2;
		}
		else if (stage_number == Stage2) {
			this->current_stage = Stage3;
		}
		else if (stage_number == Stage3) {
			int32_t shottype_number = this->shottype;
			switch (shottype_number) {
				case ReimuYukari: case ReimuSolo: case YukariSolo:
					this->current_stage = Stage4B;
					break;
				case MarisaAlice: case MarisaSolo: case AliceSolo:
					this->current_stage = Stage4A;
					break;
				case SakuyaRemilia: case SakuyaSolo: case RemiliaSolo:
					this->current_stage = Stage4A;
					break;
				case YoumuYuyuko: case YoumuSolo: case YuyukoSolo:
					this->current_stage = Stage4B;
					break;
			}
		}
		else if (stage_number <= Stage3) {

		}
		else if (stage_number <= Stage4B) {
			this->current_stage = Stage5;
		}
		else if (stage_number == Stage5) {
			this->current_stage = (bool)this->selection_box_choice + Stage6A;
		}
		else if (stage_number == Stage6A) {
			this->current_stage = Stage6B;
		}
	}

	dllexport gnu_noinline void thiscall select_next_stage_9D() {
		static constexpr uint32_t lookup = 0b110000111001;
		switch (int32_t current_stage = this->current_stage) {
			default: unreachable;
			case Stage1: case Stage2: case Stage5: case Stage6A: case Stage6B:
				this->current_stage = current_stage + 1;
				break;
			case Stage3: case Stage4A:
				this->current_stage = current_stage + 1 + (lookup >> this->shottype & 1);
				break;
			case Stage4B:
				switch (shottype) {
					default: unreachable;
					case ReimuYukari: case ReimuSolo: case YukariSolo:
					case YoumuYuyuko: case YoumuSolo: case YuyukoSolo:
						this->current_stage = Stage4A;
						break;
					case MarisaAlice: case MarisaSolo: case AliceSolo:
					case SakuyaRemilia: case SakuyaSolo: case RemiliaSolo:
						this->current_stage = Stage5;
						break;
				}
				this->current_stage = current_stage + 1 - ((lookup >> this->shottype & 1) << 1);
				break;
		}
	}
	dllexport gnu_noinline void thiscall select_next_stage_9C() {

		switch (this->shottype) {
			default: unreachable;
			case ReimuYukari: case ReimuSolo: case YukariSolo:
			case YoumuYuyuko: case YoumuSolo: case YuyukoSolo:
				switch (int32_t current_stage = this->current_stage) {
					default: unreachable;
					case Stage4B:
						this->current_stage = current_stage - 1;
						break;
					case Stage3: case Stage4A:
						this->current_stage = current_stage + 2;
						break;
					case Stage1: case Stage2: case Stage5: case Stage6A: case Stage6B:
						this->current_stage = current_stage + 1;
						break;
				}
				break;
			case MarisaAlice: case MarisaSolo: case AliceSolo:
			case SakuyaRemilia: case SakuyaSolo: case RemiliaSolo:
				++this->current_stage;
				break;
		}
	}

	dllexport gnu_noinline void thiscall select_next_stage_9B() {
		switch (int32_t current_stage = this->current_stage) {
			default:
				unreachable;
			case Stage1: case Stage2: case Stage5: case Stage6A: case Stage6B:
			NormalProgression:
				this->current_stage = current_stage + 1;
				break;
			case Stage3: case Stage4A:
				switch (this->shottype) {
					default:
						unreachable;
					case ReimuYukari: case ReimuSolo: case YukariSolo:
					case YoumuYuyuko: case YoumuSolo: case YuyukoSolo:
						this->current_stage = current_stage + 2;
						break;
					case MarisaAlice: case MarisaSolo: case AliceSolo:
					case SakuyaRemilia: case SakuyaSolo: case RemiliaSolo:
						goto NormalProgression;
				}
				break;
			case Stage4B:
				switch (this->shottype) {
					default:
						unreachable;
					case ReimuYukari: case ReimuSolo: case YukariSolo:
					case YoumuYuyuko: case YoumuSolo: case YuyukoSolo:
						this->current_stage = current_stage - 1;
						break;
					case MarisaAlice: case MarisaSolo: case AliceSolo:
					case SakuyaRemilia: case SakuyaSolo: case RemiliaSolo:
						goto NormalProgression;
				}
				break;
		}

	}

	dllexport gnu_noinline void thiscall select_next_stage_9() {
		/*static constexpr uint32_t shottype_lookup = 0b110000111001;
		switch (int32_t current_stage = this->current_stage) {
			default:
				unreachable;
			case Stage1: case Stage2: case Stage5: case Stage6A: case Stage6B:
				this->current_stage = current_stage + 1;
				break;
			case Stage3: case Stage4A:
				this->current_stage = current_stage + 1 + (shottype_lookup >> this->shottype & 1);
				break;
			case Stage4B:
				this->current_stage = current_stage + 1 - ((shottype_lookup >> this->shottype & 1) << 1);
				break;
		}*/


		static constexpr uint32_t shottype_lookup = 0b110000111001;
		int32_t current_stage = this->current_stage;
		switch (current_stage) {
			default: unreachable;
			case Stage1: case Stage2: case Stage5: case Stage6A: case Stage6B:
				this->current_stage = current_stage + 1;
				break;
			case Stage3: case Stage4A: case Stage4B:
				if (uint32_t shottype = this->shottype; current_stage != Stage4B) {
					this->current_stage = current_stage + 1 + (shottype_lookup >> shottype & 1);
				}
				else {
					switch (shottype) {
						default:
							unreachable;
						case ReimuYukari: case ReimuSolo: case YukariSolo:
						case YoumuYuyuko: case YoumuSolo: case YuyukoSolo:
							this->current_stage = Stage4A;
							break;
						case MarisaAlice: case MarisaSolo: case AliceSolo:
						case SakuyaRemilia: case SakuyaSolo: case RemiliaSolo:
							this->current_stage = Stage5;
							break;
					}
				}
				break;
		}
		//this->current_stage = current_stage + 1;
	}
};
ValidateStructSize(0x3DE3C, GameManager);