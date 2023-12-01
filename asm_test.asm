.set @feat.00, 1

.intel_syntax
.global _GAME_MANAGER, _SUPERVISOR, _ANM_MANAGER_PTR, _EFFECT_MANAGER, _SOUND_MANAGER, _PLAYER, _ITEM_MANAGER, _GUI, _STAGE, _BULLET_MANAGER, _ENEMY_MANAGER, _ENEMY_CONTROLLER, _RNG, _INPUT, _ASCII_MANAGER

_GAME_MANAGER = 0x69BCA0
_SUPERVISOR = 0x6C6D18
_ANM_MANAGER_PTR = 0x6D4588
_EFFECT_MANAGER = 0x487FE0
_SOUND_MANAGER = 0x6D3F50
_PLAYER = 0x6CA628
_ITEM_MANAGER = 0x69E268
_GUI = 0x69BC30
_STAGE = 0x487B10
_BULLET_MANAGER = 0x5A5FF8
_ENEMY_MANAGER = 0x4B79C8
_ENEMY_CONTROLLER = 0x487E50
_RNG = 0x69D8F8
_INPUT = 0x69D904
_ASCII_MANAGER = 0x47B900

.global _BULLET_TYPE_ANM_DATA
_BULLET_TYPE_ANM_DATA = 0x46A6E8

.global _EffectColorTablePtr, _UnknownSoundData_Table, _RandomItemIDs_LookupTable, _SpellcardPointsTable, _FuncCall_PtrTable, _EffectData_Table, _RenderDataFormatLookupDataTable
_EffectColorTablePtr = 0x476438
_UnknownSoundData_Table = 0x478528
_RandomItemIDs_LookupTable = 0x476338
_SpellcardPointsTable = 0x476120
_FuncCall_PtrTable = 0x476220
_EffectData_Table = 0x476298
_RenderDataFormatLookupDataTable = 0x4765B8

.global _SingleSpaceString, _MusicFormatString
_SingleSpaceString = 0x46AC10
_MusicFormatString = 0x46A98C

.global _SomeGlobalStaticBufferThatMakesMeSad
_SomeGlobalStaticBufferThatMakesMeSad = 0x4D2D5C

.global _LinkerCheeseA
_LinkerCheeseA = 0x1CB4

.global _UNKNOWN_SURFACE_A, _SPRITE_VERTEX_BUFFER_A, _SPRITE_VERTEX_BUFFER_B, _SPRITE_VERTEX_BUFFER_C
_UNKNOWN_SURFACE_A = 0x69E230
_SPRITE_VERTEX_BUFFER_A = 0x6D4590
_SPRITE_VERTEX_BUFFER_B = 0x6D4600
_SPRITE_VERTEX_BUFFER_C = 0x6D4660

.global _LinkerZero, _LinkerOne, _LinkerTwo, _LinkerThree, _LinkerFour, _LinkerFive, _LinkerSix, _LinkerSeven
_LinkerZero = 0
_LinkerOne = 1
_LinkerTwo = 2
_LinkerThree = 3
_LinkerFour = 4
_LinkerFive = 5
_LinkerSix = 6
_LinkerSeven = 7

/*
.global _decrypt

_decrypt:
.intel_syntax noprefix
	PUSH EBP
	PUSH EBX
	PUSH EDI
	PUSH ESI
	MOV ESI, DWORD PTR [ESP + 0x14]
	MOV EDI, DWORD PTR [ESP + 0x1C]
	TEST EDI, EDI
	JNZ no_local_alloc
	PUSH DWORD PTR [ESP + 0x20]
	PUSH 0
	CALL DWORD PTR [0x453128]
	MOV EDI, EAX
no_local_alloc:
	MOV EAX, 1
	JMP LoopB
.align 16
LoopA:
	XOR ECX, ECX
LoopB:
	INC CL
	MOV EBX, DWORD PTR [ESI]
	SHR EBX, CL
	JNC BreakLoopB
	AND CL, 7
	SETZ CH
	ADD CL, CH
	MOVZX EBP, CH
	LEA ESI, [ESI+EBP+1]
	MOV BYTE PTR [EDI], BL
	INC EDI
	MOV BYTE PTR [EAX + 0x4D2D5C], BL
	INC EAX
	AND EAX, 0x7FFF
	JMP LoopB
.align 16
BreakLoopB:
	MOV ECX, EBX
	AND EBX, 0x7FFF
	JZ BreakLoopA
	SHR ECX, 0xF
	AND ECX, 0xF
	ADD ECX, 2
	JS LoopD
	XOR EBP, EBP
.align 16
LoopC:
	LEA EDX, [EBP + EBX]
	INC EBP
	AND EDX, 0x7FFF
	MOVZX EDX, BYTE PTR [EDX + 0x4D2D5C]
	MOV BYTE PTR [EDI], DL
	INC EDI
	MOV BYTE PTR [EAX + 0x4D2D5C], DL
	INC EAX
	AND EAX, 0x7FFF
	CMP EBP, ECX
	JBE LoopC
LoopD:
	MOV EBX, DWORD PTR [ESI + 2]
	SHR EBX, 5
	JNC BreakLoopD
	ADD ESI, 3
	MOV BYTE PTR [EDI], BL
	INC EDI
	MOV BYTE PTR [EAX + 0x4D2D5C], BL
	INC EAX
	AND EAX, 0x7FFF
	JMP LoopB
.align 16 
BreakLoopD:
	MOV ECX, EBX
	AND EBX, 0x7FFF
	JZ BreakLoopA
	SHR ECX, 15
	AND ECX, 0xF
	ADD ECX, 2
	JS LoopB
	XOR EBP, EBP
.align 16
LoopE:
	LEA EDX, [EBP + EBX]
	INC EBP
	AND EDX, 0x7FFF
	MOVZX EDX, BYTE PTR [EDX + 0x4D2D5C]
	MOV BYTE PTR [EDI], DL
	INC EDI
	MOV BYTE PTR [EAX + 0x4D2D5C], DL
	INC EAX
	AND EAX, 0x7FFF
	CMP EBP, ECX
	JBE LoopE
	JMP LoopB
.align 16
BreakLoopA:
	MOV EAX, EDI
	POP ESI
	POP EDI
	POP EBX
	POP EBP
	RET

//.global @utf8_to_utf32_asm@4

.align 16
@utf8_to_utf32_asm@4:
	PUSH ESI
	PUSH EDI
	PUSH EBP
	MOV EBP, ESP
	MOV ESI, ECX
	MOV EDI, 0xFF
.align 16
FirstByte:
	MOVZX EAX, BYTE PTR [ESI]
	INC ESI
	TEST AL, AL
	JS MultiByte
EndMultiByte:
	PUSH EAX
	JNZ FirstByte
	MOV ESI, EBP
	JMP FoundNull
	INT3
MultiByte:
	ANDN EDX, EAX, EDI
	LZCNT ECX, EDX
	SHL EAX, CL
	SHR EAX, CL
	SUB ECX, 0x19
ContinuingBytes:
	MOVZX EDX, BYTE PTR [ESI]
	INC ESI
	AND EDX, 0x3F
	SHL EAX, 6
	OR EAX, EDX
	LOOP ContinuingBytes
	JMP EndMultiByte
FoundNull:
	SUB ESI, ESP
	PUSH ESI
	CALL _malloc
	ADD ESP, 4
	MOV ECX, ESI
	SHR ECX, 2
WriteUTF32:
	POP EDX
	MOV DWORD PTR [ECX*4+EAX-4], EDX
	LOOP WriteUTF32
	POP EBP
	POP EDI
	POP ESI
	RET
.att_syntax
*/
