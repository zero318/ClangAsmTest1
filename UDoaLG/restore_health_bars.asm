GuiSide::on_tick_hook:
	PUSH EBP
	PUSH EBX
	PUSH EDI
	PUSH ESI
	MOV ESI, ECX
	CALL GuiSide::on_tick
	IMUL EAX, [ESI + 0x64], 0x3C
	MOV ECX, [EAX + GAME_SIDES]
	TEST ECX, ECX
	JZ skip_lifebars
	TEST BYTE PTR [ECX + 0x94], 1
	JNZ skip_lifebars
	MOV [game_side], EAX
	PUSH DWORD PTR [ECX + 0x54]
	CALL EnemyManager::get_enemy_by_id
	MOV EBX, [GUI_PTR]
	IMUL EBP, [ESI + 0x64], 0x54
	TEST EAX, EAX
	JNZ boss_active
	MOV [EBP + EBX + 0x31C], EAX
	MOV [EBP + EBX + 0x32C], EAX
	MOV [EBP + EBX + 0x334], EAX
	MOV [EBP + EBX + 0x33C], EAX
	MOV [EBP + EBX + 0x344], EAX
	CMP DWORD PTR [EBP + EBX + 0x368]
	
skip_lifebars: