

struct AuraBase {
	void* vftable;
	Timer timer;
	MotionData motion;
	int dummy;
	AnmID __anm_id_60;
	uint32_t side_index;
	void* game_side_ptr;
};

struct AuraReimu : AuraBase {
	ZUNInterp<float> scale_interp;
	float scale;

	dllexport gnu_noinline BOOL thiscall check_collision_circle(Float2* position, float radius) {
		return this->motion.position.distance_squared(position) <= this->scale * this->scale + radius * radius;
	}
};