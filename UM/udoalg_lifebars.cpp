
/*
	gnu_used gnu_noinline AnmID& thiscall instantiate_vm_19_impl(AnmID& out, int, int32_t script_index, Float3* position, float z_rotation, int32_t layer, int flags, AnmVM** raw_out) asm("instantiate_vm_19_impl") {
		use_var(script_index);
		use_var(position);
		use_var(z_rotation);
		use_var(layer);
		use_var(flags);
		use_var(raw_out);
		return this->instantiate_vm(out, script_index, position, z_rotation, layer, bitcast<AnmVMCreationFlags>(flags), raw_out);
	}

	gnu_used forceinline AnmID& thiscall instantiate_vm_19(AnmID& out, int32_t script_index, Float3* position, float z_rotation, int32_t layer, int flags, AnmVM** raw_out) {
		return this->instantiate_vm_19_impl(out, UNUSED_DWORD, script_index, position, z_rotation, layer, flags, raw_out);
	}
*/

//namespace UDoALGCrap {
// size: 0x3C
struct GameSide {
	void* bullet_manager_ptr; // 0x0
	Player* player_ptr; // 0x4
	void* enemy_manager_ptr; // 0x8
	void* item_manager_ptr; // 0xC
	void* spellcard_ptr; // 0x10
	void* laser_manager_ptr; // 0x14
	void* gauge_manager_ptr; // 0x18
	void* bomb_manager_ptr; // 0x1C
	void* ability_manager_ptr; // 0x20
	void* popup_manager_ptr; // 0x24
	void* effect_manager_ptr; // 0x28
	void* globals_side_ptr; // 0x2C
	void* shot_manager_ptr; // 0x30
	void* __cpu_hit_ptr; // 0x34
	void* ex_attack_manager_ptr; // 0x38
	// 0x3C
};
ValidateStructSize(0x3C, GameSide);

dllexport GameSide GAME_SIDES[2];

struct GuiSide {
	unknown_fields(0x64);
	uint32_t side_index;
	unknown_fields(0x4);

	dllexport gnu_noinline void gui_mess_for_udoalg() const;

	dllexport gnu_noinline void gui_mess_cleanup() const;

	dllexport gnu_noinline void on_tick() const {
		rand();
	}

	dllexport gnu_noinline void cleanup() const {
		rand();
	}
};
struct Gui19 {
	unknown_fields(0x10); // 0x0
	GuiSide sides[2]; // 0x10
	unknown_fields(0x310 - 0xE8); // 0xE8
	MsgVM* msg_vm; // 0x310
	void* msg_files[2];
	Lifebar lifebars[3];
	AnmID __anm_id_418;
	AnmLoaded* front_anm;
};

dllexport gnu_noinline vec<float, 2> vectorcall libm_sse2_sincosf(float angle) {
	use_var(angle);
	__asm__ volatile ("":::clobber_list("eax", "edx", "ecx", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7"));
	return (vec<float, 2>) { rand(), rand() };
}

dllexport gnu_noinline naked void gui_mess_for_udoalg_asm() {
	__asm__ volatile (
		INTEL_SYNTAX_DIRECTIVE
		"PUSH EBP \n"
		"PUSH EBX \n"
		"PUSH EDI \n"
		"PUSH ESI \n"
		"SUB ESP, 0x28 \n"
		"MOV ESI, ECX \n"
		"CALL _rand \n"
		"MOV EBP, [ESI + 0x64] \n"
		"IMUL ESI, EBP, 0x3C \n"
		"MOV ECX, [ESI + 0xDEADBEEF] \n"
		"TEST ECX, ECX \n"
		"JZ return \n"
		"TEST BYTE PTR [ECX + 0x94], 1 \n"
		"JNZ return \n"
		"PUSH DWORD PTR [ECX + 0x54] \n"
		"CALL _rand \n"
		"MOV ECX, [0xDEADBEEF] \n" // GUI_PTR
		"IMUL EDX, EBP, 0x54 \n"
		"LEA EDI, [EDX*1 + ECX + 0x31C] \n"
		"TEST EAX, EAX \n"
		"JNZ boss_present \n" // JUMP DOWN
		"MOV [EDI], EAX \n"
		"XORPS XMM0, XMM0 \n"
		"MOVUPS [EDI + 0x10], XMM0 \n"
		"MOVUPS [EDI + 0x20], XMM0 \n"
	"health_bars_hidden: \n"
		"CMP DWORD PTR [EDI + 0x4C], 0 \n"
		"JNE cleanup_health_bars \n" // JUMP DOWN
	"return: \n"
		"ADD ESP, 0x28 \n"
		"POP ESI \n"
		"POP EDI \n"
		"POP EBX \n"
		"POP EBP \n"
		"RET \n"

	".align 16, 0xCC \n"

	"boss_present: \n"
		"TEST BYTE PTR [EAX + 0x639C], 0x71 \n"
		"JNZ health_bars_hidden \n"
		"CMP DWORD PTR [EAX + 0x635C], 0 \n"
		"JG health_bars_hidden \n"
		"CMP DWORD PTR [EAX + 0x6238], 100000 \n"
		"JGE health_bars_hidden \n"
		"CMP DWORD PTR [ECX + 0x310], 0 \n"
		"JNE health_bars_hidden \n"
		"MOV EBX, [ESI + 0xDEADBEEF] \n"
		"CMP DWORD PTR [EDI + 0x4C], 0 \n"
		"JE initialize_health_bars \n" // JUMP DOWN
		"MOV EBP, EAX \n"
		"MOV ESI, [0xDEADBEEF] \n"
		"MOV ECX, ESI \n"
		"PUSH DWORD PTR [EDI + 0x48] \n"
		"CALL _rand \n"
		"MOV [ESP + 0x18], EAX \n"
		"MOV ECX, ESI \n"
		"PUSH DWORD PTR [EDI + 0x44] \n"
		"CALL _rand \n"
		"MOV [ESP + 0x14], EAX \n"
		"MOV ECX, ESI \n"
		"PUSH DWORD PTR [EDI + 0x40] \n"
		"CALL _rand \n"
		"MOV [ESP + 0x10], EAX \n"
		"MOV ECX, ESI \n"
		"PUSH DWORD PTR [EDI + 0x3C] \n"
		"CALL _rand \n"
		"MOV [ESP + 0xC], EAX \n"
		"MOV ECX, ESI \n"
		"PUSH DWORD PTR [EDI + 0x38] \n"
		"CALL _rand \n"
		"MOV [ESP + 0x8], EAX \n"
		"MOV ECX, ESI \n"
		"PUSH DWORD PTR [EDI + 0x34] \n"
		"CALL _rand \n"
		"MOV [ESP + 0x4], EAX \n"
		"MOV ECX, ESI \n"
		"PUSH DWORD PTR [EDI + 0x30] \n"
		"CALL _rand \n"
		"MOV [ESP], EAX \n"
	"health_bars_initialized: \n"
		"MOV ECX, [EBP + 0x6238] \n"
		"MOV [EDI + 8], ECX \n"
		"XORPS XMM0, XMM0 \n"
		"CVTSI2SS XMM0, ECX \n"
		"XORPS XMM1, XMM1 \n"
		"CVTSI2SS XMM1, DWORD PTR [EBP + 0x623C] \n"
		"DIVSS XMM0, XMM1 \n"
		"MOVSS [EDI + 4], XMM0 \n"
		"MOVSS XMM1, [EDI] \n"
		"MOVAPS XMM2, XMM1 \n"
		"CMPLTSS XMM2, XMM0 \n"
		"MOVSS XMM3, [0xDEADBEEF] \n"
		"ANDPS XMM3, XMM2 \n"
		"ADDSS XMM3, XMM1 \n"
		"MINSS XMM3, XMM0 \n"
		"MOVSS [EDI], XMM3 \n"
		"MULSS XMM3, [0xDEADBEEF] \n"
		"MOV BYTE PTR [EAX + 0x4B8], 1 \n"
		"MOVSS [EAX + 0x3C], XMM3 \n"
		"MOVSD XMM0, [EBP + 0x1278] \n"
		"MOVSD XMM1, [EBX + 0x68C] \n"
		"MOVAPS XMM2, XMM0 \n"
		"SUBPS XMM2, XMM1 \n"
		"MULPS XMM2, XMM2 \n"
		"MOVAPS XMM1, XMM2 \n"
		"SHUFPS XMM1, XMM2, 0x55 \n"
		"ADDSS XMM1, XMM2 \n"
		"MOVSS [ESP + 0x1C], XMM1 \n"
		"ADDPS XMM0, XMM0 \n"
		//"CMP EBX, [0xDEADBEEF] \n"
		//"JE is_side_zero \n"
		//"ADDSS XMM0, [0xDEADBEEF] \n"
	//"is_side_zero: \n"
		"MOVLPS [EAX + 0x614], XMM0 \n"
		"MOV EAX, [ESP + 0x4] \n"
		"MOVLPS [EAX + 0x614], XMM0 \n"
		"MOV EAX, [ESP + 0x8] \n"
		"MOVLPS [EAX + 0x614], XMM0 \n"
		"MOVLPS [ESP + 0x20], XMM0 \n"
		"MOV EBX, 4 \n"

	".align 16 \n"

	"marker_loop: \n"
		"MOV ESI, [EBX*4 + ESP + 0x8] \n"
		"MOVSS XMM0, [EBX*8 + EDI + 0x8] \n"
		"XORPS XMM1, XMM1 \n"
		"UCOMISS XMM1, XMM0 \n"
		"JNE marker_is_set \n"
	"marker_is_hidden: \n"
		"MOV ECX, ESI \n"
		"CALL _rand \n"
		"DEC EBX \n"
		"JNZ marker_loop \n"
	"marker_loop_break: \n"
		"MOVSS XMM0, [ESP + 0x1C] \n"
		"CMP DWORD PTR [EDI + 0x50], EBX \n"
		JNE_SHORT(player_is_near)
	"player_is_near_branch: \n"
		"UCOMISS XMM0, [0xDEADBEEF] \n"
		"JAE return \n"
		"MOV DWORD PTR [EDI + 0x50], 1 \n"
		"MOV ESI, 7 \n"
		"MOV EDI, 3 \n"

	".align 16 \n"

	"interrupt_loop: \n"
		"MOV EAX, [ESI*4 + ESP - 0x4] \n"
		"LEA EBX, [EAX + 0x53C] \n"

	".align 16 \n"

	"interrupt_child_loop: \n"
		"MOV [EAX + 0x454], EDI \n"
		"MOV ECX, [EAX + 0x604] \n"
		"TEST ECX, ECX \n"
		"JNZ has_on_interrupt \n"
	"done_with_on_interrupt: \n"
		"MOV EBX, [EBX + 4] \n"
		"LEA EAX, [EBX - 0x52C] \n"
		"TEST EBX, EBX \n"
		JNZ_SHORT(interrupt_child_loop)
		"DEC ESI \n"
		JNZ_SHORT(interrupt_loop)
		"JMP return \n"

	".align 16, 0xCC \n"

	"player_is_near: \n"
		"UCOMISS XMM0, [0xDEADBEEF] \n"
		"JB return \n"
		"MOV DWORD PTR [EDI + 0x50], EBX \n"
		"MOV ESI, 7 \n"
		"MOV EDI, 2 \n"
		JMP_SHORT(interrupt_loop)

	".align 16, 0xCC \n"

	"has_on_interrupt: \n"
		"PUSH EDI \n"
		"PUSH EAX \n"
		"CALL DWORD PTR [ECX*4 + 0xDEADBEEF] \n"
		"ADD ESP, 8 \n"
		JMP_SHORT(done_with_on_interrupt)

	".align 16, 0xCC \n"

	"marker_is_set: \n"
		"UCOMISS XMM0, [EDI] \n"
		"JAE marker_is_hidden \n"
		"MOVSS XMM1, [0xDEADBEEF] \n" // -TWO_PI_f
		"MULSS XMM0, XMM1 \n"
		"MOVSS XMM2, [0xDEADBEEF] \n" // -PI_f
		"ADDSS XMM0, XMM2 \n"
		"MOVSS XMM3, [0xDEADBEEF] \n" // PI_f
		"XOR EAX, EAX \n"
		"UCOMISS XMM0, XMM3 \n"
		"JBE first_reduce_done \n"
	"angle_above_pi: \n"
		"ADDSS XMM0, XMM1 \n"
		"CMP EAX, 32 \n"
		"JA first_reduce_done \n"
		"INC EAX \n"
		"UCOMISS XMM0, XMM3 \n"
		"JA angle_above_pi \n"
	"first_reduce_done: \n"
		"UCOMISS XMM0, XMM2 \n"
		"JAE angle_reduce_done \n"
	"angle_below_neg_pi: \n"
		"SUBSS XMM0, XMM1 \n"
		"CMP EAX, 32 \n"
		"JA angle_reduce_done \n"
		"INC EAX \n"
		"UCOMISS XMM0, XMM2 \n"
		"JB angle_below_neg_pi \n"
	"angle_reduce_done: \n"
		"MOV BYTE PTR [ESI + 0x4B8], 1 \n"
		"MOVSS [ESI + 0x44], XMM0 \n"
		"CALL _rand \n"
		"MULPS XMM0, [0xDEADBEEF] \n"
		"MOVSD XMM1, [ESP + 0x20] \n"
		"ADDPS XMM1, XMM0 \n"
		"MOVLPS [ESI + 0x614], XMM1 \n"
		"MOV ECX, ESI \n"
		"CALL _rand \n"
		"DEC EBX \n"
		"JNZ marker_loop \n"
		"JMP marker_loop_break \n"

	".align 16, 0xCC \n"

	"cleanup_health_bars: \n"
		"MOV EBX, 7 \n"
		"MOV ESI, [0xDEADBEEF] \n" // ANM_MANAGER_PTR

	".align 16 \n"

	"cleanup_health_bars_loop: \n"
		"MOV ECX, ESI \n"
		"PUSH DWORD PTR [EBX*4 + EDI + 0x2C] \n"
		"CALL _rand \n"
		"MOV ECX, ESI \n"
		"PUSH EAX \n"
		"CALL _rand \n"
		"DEC EBX \n"
		JNZ_SHORT(cleanup_health_bars_loop)
		"XORPS XMM0, XMM0 \n"
		"MOVUPS [EDI + 0x30], XMM0 \n"
		"MOVUPS [EDI + 0x40], XMM0 \n"
		"MOV [EDI + 0x50], EBX \n"
		"JMP return \n"

	".align 16, 0xCC \n"

	"initialize_health_bars: \n"
		"MOV DWORD PTR [EDI + 0x4C], 1 \n"
		"MOV EDX, [0xDEADBEEF] \n"
		"MOV ECX, [EDX + 0x720] \n"
		"MOV [ESP + 0x1C], ECX \n"
		"MOV [EDX + 0x720], EBP \n"
		"MOV EBP, EAX \n"

		"MOV ESI, [ECX + 0x41C] \n"

		"LEA EAX, [EDI + 0x30] \n"
		"MOV ECX, ESP \n"
		"SUB ESP, 0x20 \n"
		"MOV [ESP + 0x1C], ECX \n"
		"MOVAPS XMM0, [0xDEADBEEF] \n"
		"MOVUPS [ESP + 0xC], XMM0 \n"
		"MOV DWORD PTR [ESP + 0x8], 330 \n"
		"MOV DWORD PTR [ESP], EAX \n"
		"MOV ECX, ESI \n"
		"CALL _rand \n"

		"LEA EAX, [EDI + 0x34] \n"
		"LEA ECX, [ESP + 0x4] \n"
		"SUB ESP, 0x20 \n"
		"MOV [ESP + 0x1C], ECX \n"
		"MOVAPS XMM0, [0xDEADBEEF] \n"
		"MOVUPS [ESP + 0xC], XMM0 \n"
		"MOV DWORD PTR [ESP + 0x8], 331 \n"
		"MOV DWORD PTR [ESP], EAX \n"
		"MOV ECX, ESI \n"
		"CALL _rand \n"

		"LEA EAX, [EDI + 0x38] \n"
		"LEA ECX, [ESP + 0x8] \n"
		"SUB ESP, 0x20 \n"
		"MOV [ESP + 0x1C], ECX \n"
		"MOVAPS XMM0, [0xDEADBEEF] \n"
		"MOVUPS [ESP + 0xC], XMM0 \n"
		"MOV DWORD PTR [ESP + 0x8], 332 \n"
		"MOV DWORD PTR [ESP], EAX \n"
		"MOV ECX, ESI \n"
		"CALL _rand \n"

		"LEA EAX, [EDI + 0x3C] \n"
		"LEA ECX, [ESP + 0xC] \n"
		"SUB ESP, 0x20 \n"
		"MOV [ESP + 0x1C], ECX \n"
		"MOVAPS XMM0, [0xDEADBEEF] \n"
		"MOVUPS [ESP + 0xC], XMM0 \n"
		"MOV DWORD PTR [ESP + 0x8], 333 \n"
		"MOV DWORD PTR [ESP], EAX \n"
		"MOV ECX, ESI \n"
		"CALL _rand \n"

		"LEA EAX, [EDI + 0x40] \n"
		"LEA ECX, [ESP + 0x10] \n"
		"SUB ESP, 0x20 \n"
		"MOV [ESP + 0x1C], ECX \n"
		"MOVAPS XMM0, [0xDEADBEEF] \n"
		"MOVUPS [ESP + 0xC], XMM0 \n"
		"MOV DWORD PTR [ESP + 0x8], 333 \n"
		"MOV DWORD PTR [ESP], EAX \n"
		"MOV ECX, ESI \n"
		"CALL _rand \n"

		"LEA EAX, [EDI + 0x44] \n"
		"LEA ECX, [ESP + 0x14] \n"
		"SUB ESP, 0x20 \n"
		"MOV [ESP + 0x1C], ECX \n"
		"MOVAPS XMM0, [0xDEADBEEF] \n"
		"MOVUPS [ESP + 0xC], XMM0 \n"
		"MOV DWORD PTR [ESP + 0x8], 333 \n"
		"MOV DWORD PTR [ESP], EAX \n"
		"MOV ECX, ESI \n"
		"CALL _rand \n"

		"LEA EAX, [EDI + 0x48] \n"
		"LEA ECX, [ESP + 0x18] \n"
		"SUB ESP, 0x20 \n"
		"MOV [ESP + 0x1C], ECX \n"
		"MOVAPS XMM0, [0xDEADBEEF] \n"
		"MOVUPS [ESP + 0xC], XMM0 \n"
		"MOV DWORD PTR [ESP + 0x8], 333 \n"
		"MOV DWORD PTR [ESP], EAX \n"
		"MOV ECX, ESI \n"
		"CALL _rand \n"

		"MOV EAX, [ESP] \n"
		"MOV ESI, [0xDEADBEEF] \n"
		"MOV ECX, [ESP + 0x1C] \n"
		"MOV [ESI + 0x720], ECX \n"
		"JMP health_bars_initialized \n"
	);
}

dllexport gnu_noinline void GuiSide::gui_mess_for_udoalg() const {
	
	union {
		struct {
			AnmVM* vms[3 + MAX_LIFEBAR_MARKERS] packed_field;
			volatile float distance_squared_store packed_field;
			volatile vec<float, 2> position_store packed_field;
		};
	};

	this->on_tick();

	uint32_t side_index = this->side_index;
	const GameSide& game_side = GAME_SIDES[side_index];
	EnemyManager* enemy_manager = (EnemyManager*)game_side.enemy_manager_ptr;
	if (
		expect(enemy_manager && !enemy_manager->__hide_boss_hud, true)
	) {
		Enemy* boss_out = enemy_manager->get_enemy_by_id(enemy_manager->boss_ids[0]);
		Gui19* gui = (Gui19*)GUI_PTR;
		Lifebar* lifebar = &gui->lifebars[side_index];
		__asm__ volatile ("":"+r"(lifebar));

		//uintptr_t lifebar_offset = side_index * sizeof(Lifebar);

//#define LIFEBAR (*(Lifebar*)((uintptr_t)gui->lifebars + lifebar_offset))
#define LIFEBAR (*lifebar)

		if (expect(boss_out != NULL, false)) {
			if (
				//expect(boss->data.__is_midboss, false) &&
				//!boss->data.hide_lifebar() &&
				!((boss_out->data.__is_midboss | boss_out->data.disable_hitbox | boss_out->data.invincible | boss_out->data.intangible) || boss_out->data.invulnerable_timer > 0) &&
				boss_out->data.life.current < 100000 &&
				!gui->msg_vm
			) {
				register Enemy* boss asm("ebp") = boss_out;
				__asm__("":"+r"(boss));
				Player* player = game_side.player_ptr;
				AnmVM* main_vm;

				if (expect(!LIFEBAR.vms_initialized, false)) {
					__asm__ volatile (".align 16");
					//AnmVM* main_vm_out;
					LIFEBAR.vms_initialized = TRUE;
					// instantiate_vm(AnmID& out, int32_t script_index, Float3* position, float z_rotation, int32_t layer, AnmVMCreationFlags flags, AnmVM** raw_out)

					/*
					AnmLoaded* front_anm = gui->front_anm;
					front_anm->instantiate_vm_19(LIFEBAR.main_vm, 330, nullptr, 0.0f, -1, 0, &vms[0]);
					front_anm->instantiate_vm_19(LIFEBAR.glowA_vm, 331, nullptr, 0.0f, -1, 0, &vms[1]);
					front_anm->instantiate_vm_19(LIFEBAR.glowB_vm, 332, nullptr, 0.0f, -1, 0, &vms[2]);
					size_t i = 0;
					do {
						front_anm->instantiate_vm_19(LIFEBAR.marker_vms[i], 333, nullptr, 0.0f, -1, 0, &vms[3 + i]);
					} while (++i != MAX_LIFEBAR_MARKERS);
					*/

					AnmLoaded* front_anm = gui->front_anm;
					/*
					size_t i = MAX_LIFEBAR_MARKERS;
					while (i--) {
						front_anm->instantiate_vm_19(LIFEBAR.marker_vms[i], 333, nullptr, 0.0f, -1, 0, &vms[3 + i]);
					}
					front_anm->instantiate_vm_19(LIFEBAR.glowB_vm, 332, nullptr, 0.0f, -1, 0, &vms[2]);
					front_anm->instantiate_vm_19(LIFEBAR.glowA_vm, 331, nullptr, 0.0f, -1, 0, &vms[1]);
					front_anm->instantiate_vm_19(LIFEBAR.main_vm, 330, nullptr, 0.0f, -1, 0, &vms[0]);
					*/

					constexpr static vec<int32_t, 4> mem_const = { 0, 0, -1, 0 };
					int eax, edx, ecx;

#define INSTANTIATE_VM(id, script_num, vm) \
	__asm__ volatile ( \
		"SUB $0x20, %%ESP \n" \
		"MOV %[vm_out], 0x1C(%%ESP) \n" \
		"MOVAPS %[mem_const], %%XMM0 \n" \
		"MOVUPS %%XMM0, 0xC(%%ESP) \n" \
		"MOVL %[script], 0x8(%%ESP) \n" \
		"MOV %[id_out], (%%ESP) \n" \
		"MOV %[front_anm], %%ECX \n" \
		"CALL instantiate_vm_19_impl \n" \
		: "=m"(*id), "=m"(*vm), "=a"(eax), "=d"(edx), "=c"(ecx), "+r"(lifebar) \
		: [script]"i"(script_num), asm_arg("r", front_anm), [id_out]"r"(id), [vm_out]"r"(vm), asm_arg("m", mem_const) \
		: clobber_list("xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7") \
	)

					INSTANTIATE_VM(&LIFEBAR.marker_vms[3], 333, &vms[6]);
					INSTANTIATE_VM(&LIFEBAR.marker_vms[2], 333, &vms[5]);
					INSTANTIATE_VM(&LIFEBAR.marker_vms[1], 333, &vms[4]);
					INSTANTIATE_VM(&LIFEBAR.marker_vms[0], 333, &vms[3]);
					INSTANTIATE_VM(&LIFEBAR.glowB_vm, 332, &vms[2]);
					INSTANTIATE_VM(&LIFEBAR.glowA_vm, 331, &vms[1]);
					INSTANTIATE_VM(&LIFEBAR.main_vm, 330, &vms[0]);
					
					main_vm = vms[0];
				} else {
					AnmManager* anm_manager = ANM_MANAGER_PTR;
					size_t i = 3 + MAX_LIFEBAR_MARKERS - 1;
					do {
						vms[i] = anm_manager->get_vm_with_id((&LIFEBAR.main_vm)[i]);
					} while (--i);
					main_vm = anm_manager->get_vm_with_id(LIFEBAR.main_vm);
					vms[0] = main_vm;
				}

				int32_t life = boss->data.life.current;
				LIFEBAR.current_life = life;
				float percentage = (float)life / (float)boss->data.life.maximum;
				LIFEBAR.life_percentage = percentage;

				float current_value = LIFEBAR.bar_value;
				float add_value = 0.0f;
				if (current_value < percentage) {
					add_value += 0.025f; // 1/40
				}
				current_value += add_value;
				if (current_value > percentage) {
					current_value = percentage;
				}
				LIFEBAR.bar_value = current_value;

				main_vm->set_x_rotation(-current_value * TWO_PI_f);

				vec<float, 2> position = *(vec<float, 2>*)&boss->data.current_motion.position;

				vec<float, 2> position_diff = position - *(vec<float, 2>*)&player->data.position;
				position_diff *= position_diff;
				*(float*)&distance_squared_store = position_diff[0] + position_diff[1];

				position *= 2.0f;

				//if ((void*)player != (void*)GAME_SIDES[0].player_ptr) {
					//position[0] += 320.0f;
				//}

				*(vec<float, 2>*)&main_vm->controller.position = position;

				*(vec<float, 2>*)&vms[1]->controller.position = position;
				*(vec<float, 2>*)&vms[2]->controller.position = position;

				position_store = position;

				size_t i = MAX_LIFEBAR_MARKERS;
				nounroll do {
					AnmVM* marker_vm = vms[3 + i - 1];

					float marker_position = LIFEBAR.markers[i - 1].bar_position;
					if (
						expect_chance(marker_position != 0.0f, true, 0.0) &&
						marker_position < LIFEBAR.bar_value
						//expect_chance(!(
							//marker_position != 0.0f &&
							//marker_position < LIFEBAR.bar_value
						//), true, 1.0)
					) {
						float angle = NPI_f - marker_position * TWO_PI_f;

						int32_t counter = 0;
						if (angle > PI_f) {
							do {
								angle -= TWO_PI_f;
								if (++counter > 33) break;
							} while (angle > PI_f);
						} //else
							if (angle < -PI_f) {
							do {
								angle += TWO_PI_f;
								if (++counter > 33) break;
							} while (angle < -PI_f);
						}

						marker_vm->set_z_rotation(angle);

						vec<float, 2> offset = libm_sse2_sincosf(angle);

						constexpr vec<float, 2> offset_mul = { -112.0f, 112.0f };

						offset *= offset_mul;
						
						position = position_store;
						*(vec<float, 2>*)&marker_vm->controller.position = position + offset;

						marker_vm->__show_tree();
					} else {
						marker_vm->__hide_tree();
					}
					__asm__ volatile ("":"+r"(i));
				} while (--i);

				float distance_squared = distance_squared_store;

				int32_t interrupt_index;
				size_t j;
				if (expect(LIFEBAR.player_is_close == FALSE, true)) {
					if (expect(distance_squared < 6400.0f, true)) { // 80 squared
						LIFEBAR.player_is_close = true;
						__asm__ volatile ("":"+m"(LIFEBAR.player_is_close));
						interrupt_index = 3;
						j = 3 + MAX_LIFEBAR_MARKERS;
						__asm__ volatile (".align 16 ": "+r"(j), "+r"(interrupt_index));
					run_interrupts:
						__asm__ volatile ("": "+r"(j), "+r"(interrupt_index));
						nounroll do {
							AnmVM* vm = vms[j - 1];
							ZUNList<AnmVM>* node = &vm->controller.child_list;
							do {
								vm->data.run_interrupt = interrupt_index;
								uint32_t index = vm->controller.on_interrupt_index;
								if (expect(index != 0, false)) {
									((AnmOnFuncArg19)ANM_ON_INTERRUPT_FUNCS[index])(vm, interrupt_index);
								}

								node = node->next;
								vm = based_pointer<AnmVM>(node, -offsetof(AnmVM, controller.child_list_node));
							} while (node);
							/*
							//vm->interrupt(interrupt_number);
							//vm->run_on_interrupt(interrupt_index);
							if (uint32_t index = vm->controller.on_interrupt_index) {
								((AnmOnFuncArg19)ANM_ON_INTERRUPT_FUNCS[index])(vm, interrupt_index);
							}
							vm->data.run_interrupt = interrupt_index;
							vm->controller.child_list.for_each([=](AnmVM* vm) {
								//vm->interrupt(interrupt_number);
								//vm->run_on_interrupt(interrupt_index);
								if (uint32_t index = vm->controller.on_interrupt_index) {
									((AnmOnFuncArg19)ANM_ON_INTERRUPT_FUNCS[index])(vm, interrupt_index);
								}
								vm->data.run_interrupt = interrupt_index;
							});
							*/
							__asm__ volatile ("":"+r"(j));
						} while (--j);
					}
				} else {
					if (expect(distance_squared >= 9216.0f, true)) { // 96 squared
						LIFEBAR.player_is_close = false;
						__asm__ volatile ("":"+m"(LIFEBAR.player_is_close));
						interrupt_index = 2;
						j = 3 + MAX_LIFEBAR_MARKERS;
						__asm__ volatile ("": "+r"(j), "+r"(interrupt_index));
						goto run_interrupts;
					}
				}
			}
			else {
				goto delete_lifebar_vms;
			}
		}
		else {
			__asm__ volatile ("":"=a"(boss_out));
			LIFEBAR.bar_value = bitcast<float>(boss_out);
			memset(&LIFEBAR.markers, 0, sizeof(LIFEBAR.markers));
		delete_lifebar_vms:
			if (expect(LIFEBAR.vms_initialized != FALSE, false)) {
				AnmManager* anm_manager = ANM_MANAGER_PTR;
				size_t i = 3 + MAX_LIFEBAR_MARKERS;
				nounroll do {
					anm_manager->mark_tree_for_delete(anm_manager->get_vm_with_id((&LIFEBAR.main_vm)[i - 1]));
					__asm__ volatile ("":"+r"(i));
				} while (--i);
				__asm__ volatile ("":"+r"(lifebar));
				memset(&LIFEBAR.main_vm, 0, sizeof(AnmID[3 + MAX_LIFEBAR_MARKERS]));
				LIFEBAR.vms_initialized = FALSE;
				LIFEBAR.player_is_close = false;
			}
		}
	}
}

dllexport gnu_noinline void GuiSide::gui_mess_cleanup() const {
	this->cleanup();

	uint32_t side_index = this->side_index;
	Gui19* gui = (Gui19*)GUI_PTR;
	Lifebar* lifebar = &gui->lifebars[side_index];
	if (expect(LIFEBAR.vms_initialized != FALSE, false)) {
		AnmManager* anm_manager = ANM_MANAGER_PTR;
		size_t i = 3 + MAX_LIFEBAR_MARKERS;
		nounroll do {
			anm_manager->mark_tree_for_delete(anm_manager->get_vm_with_id((&LIFEBAR.main_vm)[i - 1]));
			__asm__ volatile ("":"+r"(i));
		} while (--i);
		__asm__ volatile ("":"+r"(lifebar));
		memset(&LIFEBAR.main_vm, 0, sizeof(AnmID[3 + MAX_LIFEBAR_MARKERS]));
		LIFEBAR.vms_initialized = FALSE;
		LIFEBAR.player_is_close = false;
	}
}

//}