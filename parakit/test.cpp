#pragma clang diagnostic ignored "-Wc++17-extensions"
#include <stdlib.h>
#include <stdint.h>

#include <Windows.h>

#include "../zero/util.h"

struct PlayerData {
	vec<float, 2> position;
	vec<float, 2> hurtbox;
};
struct BulletData {
	vec<float, 2> position;
	vec<float, 2> velocity;
	float speed;
	float angle;
	union {
		float hitbox_radius;
		struct {
			float hitbox_width;
			float hitbox_height;
		};
		vec<float, 2> size;
	};
	float scale;
	uint16_t sprite;
	uint16_t color;
};
struct EnemyData {
	vec<float, 2> position;
	vec<float, 2> hurtbox;
	vec<float, 2> hitbox;
	float hurtbox_rotation;

};
struct ItemData {

};

/*
This enum is defined in that zero/util.h file
enum GameVersion : uint32_t {
	EoSD,
	PCB,
	IN,
	PoFV,
	StB,
	MoF,
	UB,
	SA,
	UFO,
	DS,
	GFW,
	TD,
	DDC,
	ISC,
	LoLK,
	HSiFS,
	VD,
	WBaWC,
	UM,
	HBM,
	UDoALG
};
*/

struct dllexport PerGameData {
	// I don't remember which games moved individual structs into pointers,
	// so these flags can be simplified once I check my notes properly.
	uint32_t player_address_is_pointer : 1;
	uint32_t bullet_manager_address_is_pointer : 1;
	uint32_t enemy_manager_address_is_pointer : 1;
	uint32_t item_manager_address_is_pointer : 1;
	uint32_t laser_manager_address_is_pointer : 1;
	uint32_t fixed_enemy_count : 1;
	uint32_t fixed_laser_count : 1;
	uint32_t laser_manager_is_separate : 1;

	GameVersion game_id;
	
	mutable uintptr_t player_address;
	mutable uintptr_t bullet_manager_address;
	mutable uintptr_t enemy_manager_address;
	mutable uintptr_t item_manager_address;
	mutable uintptr_t laser_manager_address;

	size_t player_position_offset;
	size_t player_hurtbox_size_offset;

	size_t bullet_array_offset;
	size_t bullet_struct_size;

	size_t bullet_state_offset;
	size_t bullet_position_offset;
	size_t bullet_velocity_offset;
	size_t bullet_speed_offset;
	size_t bullet_angle_offset;
	size_t bullet_size_offset;
	size_t bullet_scale_offset;
	size_t bullet_sprite_offset;
	size_t bullet_color_offset;

	uint32_t bullet_count_max;
	uint32_t laser_count_max;
	uint32_t enemy_count_max;
	uint32_t item_count_max;

	inline const size_t base_mem_size() const {
		return sizeof(BulletData[this->bullet_count_max]);
	}

	void fastcall copy_player_data(PlayerData* out) const {
		void* player_address = (void*)this->player_address;
		out->position = *based_pointer<unaligned vec<float, 2>>(player_address, this->player_position_offset);
		out->hurtbox = *based_pointer<unaligned vec<float, 2>>(player_address, this->player_hurtbox_size_offset);
	}

	void fastcall copy_bullet_data(BulletData* out) const {
		void* bullet_manager_address = (void*)this->bullet_manager_address;
		void* bullet_ptr = based_pointer(bullet_manager_address, this->bullet_array_offset);
		void* bullet_array_end = based_pointer(bullet_ptr, this->bullet_struct_size * this->bullet_count_max);
		size_t bullet_state_offset = this->bullet_state_offset;
		for (
			;
			bullet_ptr != bullet_array_end;
			bullet_ptr = based_pointer(bullet_ptr, this->bullet_struct_size)
		) {
			if (*based_pointer<uint16_t>(bullet_ptr, bullet_state_offset)) {
				out->position = *based_pointer<unaligned vec<float, 2>>(bullet_ptr, this->bullet_position_offset);
				out->velocity = *based_pointer<unaligned vec<float, 2>>(bullet_ptr, this->bullet_velocity_offset);
				out->speed = *based_pointer<float>(bullet_ptr, this->bullet_speed_offset);
				out->angle = *based_pointer<float>(bullet_ptr, this->bullet_angle_offset);
				out->size = *based_pointer<unaligned vec<float, 2>>(bullet_ptr, this->bullet_size_offset);
				out->scale = *based_pointer<float>(bullet_ptr, this->bullet_scale_offset);
				out->sprite = *based_pointer<uint16_t>(bullet_ptr, this->bullet_sprite_offset);
				out->color = *based_pointer<uint16_t>(bullet_ptr, this->bullet_color_offset);
				++out;
			}
		}
	}

	inline void fastcall update_addresses(uintptr_t base_address) const {
		uintptr_t player_address = this->player_address;
		uintptr_t bullet_manager_address = this->bullet_manager_address;
		uintptr_t enemy_manager_address = this->enemy_manager_address;
		uintptr_t item_manager_address = this->item_manager_address;
		uintptr_t laser_manager_address = this->laser_manager_address;
		player_address += base_address;
		bullet_manager_address += base_address;
		enemy_manager_address += base_address;
		item_manager_address += base_address;
		laser_manager_address += base_address;
		if (this->player_address_is_pointer)			player_address = *(uintptr_t*)player_address;
		if (this->bullet_manager_address_is_pointer)	bullet_manager_address = *(uintptr_t*)bullet_manager_address;
		if (this->enemy_manager_address_is_pointer)		enemy_manager_address = *(uintptr_t*)enemy_manager_address;
		if (this->item_manager_address_is_pointer)		item_manager_address = *(uintptr_t*)item_manager_address;
		if (this->laser_manager_address_is_pointer)		laser_manager_address = *(uintptr_t*)laser_manager_address;
		this->player_address = player_address;
		this->bullet_manager_address = bullet_manager_address;
		this->enemy_manager_address = enemy_manager_address;
		this->item_manager_address = item_manager_address;
		this->laser_manager_address = laser_manager_address;
	}
};

static constexpr const PerGameData per_game_data[] = {
	[EoSD] = {
		.player_address_is_pointer = false,
		.bullet_manager_address_is_pointer = false,
		.enemy_manager_address_is_pointer = false,
		.item_manager_address_is_pointer = false,
		.fixed_enemy_count = true,
		.fixed_laser_count = true,
		.laser_manager_is_separate = false,

		.game_id = EoSD,

		.player_address = 0x2CA628,
		.bullet_manager_address = 0x1A5FF8,
		.enemy_manager_address = 0xB79C8,
		.item_manager_address = 0x29E268,

		.player_position_offset = 0x440,
		.player_hurtbox_size_offset = 0x488,

		.bullet_array_offset = 0x5600,
		.bullet_struct_size = 0x5C4,

		.bullet_state_offset = 0x5BE,
		.bullet_position_offset = 0x560,
		.bullet_velocity_offset = 0x56C,
		.bullet_speed_offset = 0x584,
		.bullet_angle_offset = 0x590,
		.bullet_size_offset = 0x550,

		.bullet_count_max = 640,
		.laser_count_max = 64,
		.enemy_count_max = 256,
		.item_count_max = 512
	},
	[DDC] = {
		.player_address_is_pointer = true,
		.bullet_manager_address_is_pointer = true,
		.enemy_manager_address_is_pointer = true,
		.item_manager_address_is_pointer = true,
		.fixed_enemy_count = false,
		.fixed_laser_count = false,
		.laser_manager_is_separate = true,

		.game_id = DDC,

		.player_address = 0xDB67C,
		.bullet_manager_address = 0xDB530,
		.enemy_manager_address = 0xDB544,
		.item_manager_address = 0xDB660,
		.laser_manager_address = 0xDB664,

		.bullet_count_max = 2000,
		.item_count_max = 600
	},
	[UM] = {
		.player_address_is_pointer = true,
		.bullet_manager_address_is_pointer = true,
		.enemy_manager_address_is_pointer = true,
		.item_manager_address_is_pointer = true,
		.fixed_enemy_count = false,
		.fixed_laser_count = false,
		.laser_manager_is_separate = true,

		.game_id = UM,
		.player_address = 0xCF410,
		.bullet_manager_address = 0xCF2BC,
		.enemy_manager_address = 0xCF2D0,
		.item_manager_address = 0xCF2EC,
		.laser_manager_address = 0xCF3F4,

		.bullet_count_max = 2000,
		.item_count_max = 600
	}
};

static PerGameData current_game_data;

static void set_per_game_data(GameVersion game, uintptr_t base_address) {
	current_game_data = per_game_data[game];
	current_game_data.update_addresses(base_address);
}

extern "C" {
dllexport BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	return TRUE;
}
}