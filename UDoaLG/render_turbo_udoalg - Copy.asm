.set @feat.00, 1

.intel_syntax noprefix

.global th19a_render_layer_turbo
.global th19a_render_layer_turbo_avx2
.global th19a_render_layer_turbo_inner
.global th19a_render_layer_turbo_inner_avx2
.global th19a_process_lists

//; 1.00a = 0x724 (0x734)
//; 1.10c = 0x714 (0x724)

th19a_process_lists:
	PUSH EBP
	PUSH EBX
	PUSH EDI
	PUSH ESI
	MOV EBP, DWORD PTR [ESP+0x14]
	MOV EDI, DWORD PTR [EBP+0x734]
	AND EDI, -16
	ADD EDI, 16
	MOV ESI, DWORD PTR [EBP+0x754]
	AND ESI, -16
	ADD ESI, 16
	LEA EDX, [EDI+ESI]
	LEA EBX, [EDX*4+EDX]
	MOV EAX, DWORD PTR [EBP+0x73C]
	CMP DWORD PTR [EBP+0x738], EBX
	JAE no_resize
	MOV DWORD PTR [EBP+0x738], EBX
	PUSH 16
	PUSH EBX
	PUSH EAX
	CALL __aligned_realloc
	ADD ESP, 0xC
	MOV DWORD PTR [EBP+0x73C], EAX
no_resize:
	PUSH EBX
	PUSH -1
	PUSH EAX
	CALL _memset
	ADD ESP, 0xC
	LEA EBX, [EDI*4+EDI]
	ADD EDI, EAX
	ADD EBX, EAX
	MOV ECX, DWORD PTR [EBP+0x724]
	JMP start_world_preprocess
	INT3
	INT3
world_layer_table_set:
	MOVZX EDX, BYTE PTR [EBP+world_layer_lookup]
//;.align 16
world_layer_set:
	MOV BYTE PTR [EAX], DL
	INC EAX
	LEA EDX, [ECX-0x50C]
	MOV DWORD PTR [EDI], EDX
	ADD EDI, 4
next_world_preprocess:
	MOV ECX, DWORD PTR [ECX+4]
start_world_preprocess:
	TEST ECX, ECX
	JZ end_world_preprocess
	CMP DWORD PTR [ECX+0x6C], 0
	JNE next_world_preprocess
	MOV EDX, DWORD PTR [ECX-0x4F4]
	LEA EBP, [EDX-0x2B]
	CMP EBP, 8
	JBE world_layer_table_set
	CMP EDX, 0xFE
	JBE world_layer_set
	JMP next_world_preprocess
	INT3
	
end_world_preprocess:
	MOV EBP, DWORD PTR [ESP+0x14]
	MOV ECX, DWORD PTR [EBP+0x72C]
	JMP start_ui_preprocess

.align 16, 0xCC
ui_layer_table_set:
	MOVZX EDX, BYTE PTR [EDX+ui_layer_lookup]
ui_layer_set:
	MOV BYTE PTR [EAX], DL
	INC EAX
	LEA EDX, [ECX-0x50C]
	MOV DWORD PTR [EDI], EDX
	ADD EDI, 4
next_ui_preprocess:
	MOV ECX, DWORD PTR [ECX+4]
start_ui_preprocess:
	TEST ECX, ECX
	JZ end_ui_preprocess
	CMP DWORD PTR [ECX+0x6C], 0
	JNE next_ui_preprocess
	MOV EDX, DWORD PTR [ECX-0x4F4]
	ADD EDX, -0x1A
	CMP EDX, 0xA
	JBE ui_layer_table_set
	MOV EDX, 0x2D
	JMP ui_layer_set
	INT3
end_ui_preprocess:
	ADD ESI, EBX
	MOV DWORD PTR [EBP+0x75C], EBX
	MOV EAX, DWORD PTR [EBP+0x744]
	JMP start_world2_preprocess
	INT3
world2_layer_table_set:
	MOVZX ECX, BYTE PTR [EDX+world_layer_lookup]
//;.align 16
world2_layer_set:
	MOV BYTE PTR [EBX], CL
	INC EBX
	LEA ECX, [EAX-0x50C]
	MOV DWORD PTR [ESI], ECX
	ADD ESI, 4
next_world2_preprocess:
	MOV EAX, DWORD PTR [EAX+4]
start_world2_preprocess:
	TEST EAX, EAX
	JZ end_world2_preprocess
	CMP DWORD PTR [EAX+0x6C], 0
	JNE next_world2_preprocess
	MOV ECX, DWORD PTR [EAX-0x4F4]
	LEA EDX, [ECX-0x2B]
	CMP EDX, 8
	JBE world2_layer_table_set
	CMP ECX, 0xFE
	JBE world2_layer_set
	JMP next_world2_preprocess
	INT3

end_world2_preprocess:
	MOV EAX, DWORD PTR [EBP+0x74C]
	JMP start_ui2_preprocess

.align 16, 0xCC
ui2_layer_table_set:
	MOVZX ECX, BYTE PTR [ECX+ui_layer_lookup]
ui2_layer_set:
	MOV BYTE PTR [EBX], CL
	INC EBX
	LEA ECX, [EAX-0x50C]
	MOV DWORD PTR [ESI], ECX
	ADD ESI, 4
next_ui2_preprocess:
	MOV EAX, DWORD PTR [EAX+4]
start_ui2_preprocess:
	TEST EAX, EAX
	JZ end_ui2_preprocess
	CMP DWORD PTR [EAX+0x6C], 0
	JNE next_ui2_preprocess
	MOV ECX, DWORD PTR [EAX-0x4F4]
	ADD ECX, -0x1A
	CMP ECX, 0xA
	JBE ui2_layer_table_set
	MOV ECX, 0x2D
	JMP ui2_layer_set
	INT3
end_ui2_preprocess:

	MOV EAX, 1
	POP ESI
	POP EDI
	POP EBX
	POP EBP
	RET
	
.align 16, 0xCC
th19a_draw_layerA:
	MOV EDX, DWORD PTR [ESP+0x4]
	MOV ECX, DWORD PTR [0xDEADBEEF]
	JMP th19a_render_layer_turbo
.align 16, 0xCC
th19a_draw_layerB:
	MOV EDX, DWORD PTR [ESP+0x4]
	MOV ECX, DWORD PTR [0xDEADBEEF]
	MOV DWORD PTR [ECX+0x71C], -1
	JMP th19a_render_layer_turbo

.align 16, 0xCC
th19a_render_layer_turbo:
	PUSH EBP
	PUSH EBX
	PUSH EDI
	PUSH ESI
	SUB ESP, 8
	MOV EBP, ECX
	MOV EDI, DWORD PTR [ECX+0x734]
	TEST EDI, EDI
	JZ skip_side_0
	MOV EBX, EDX
	PUSH 0
	PUSH EDX
	CALL render_layer_prep_dummy
	AND EDI, -16
	MOV ESI, [EBP+0x73C]
	LEA EAX, [ESI*1+EDI+16]
	MOV DWORD PTR [ESP+4], EAX
	LEA EDI, [EAX-4]
	IMUL EDX, EBX, 0x01010101
	MOV DWORD PTR [ESP], EDX
	MOVD XMM0, EDX
	PSHUFD XMM0, XMM0, 0
	CALL th19a_render_layer_turbo_inner
	MOVZX EDX, BYTE PTR [ESP]
	MOV ECX, EBP
skip_side_0:
	MOV EDI, DWORD PTR [ECX+0x754]
	TEST EDI, EDI
	JZ skip_side_1
	MOV EBX, EDX
	PUSH 1
	PUSH EDX
	CALL render_layer_prep_dummy
	AND EDI, -16
	MOV ESI, [EBP+0x75C]
	LEA EAX, [ESI*1+EDI+16]
	MOV DWORD PTR [ESP+4], EAX
	LEA EDI, [EAX-4]
	IMUL EDX, EBX, 0x01010101
	MOV DWORD PTR [ESP], EDX
	MOVD XMM0, EDX
	PSHUFD XMM0, XMM0, 0
	CALL th19a_render_layer_turbo_inner
skip_side_1:
	MOV EAX, 1
	ADD ESP, 8
	POP ESI
	POP EDI
	POP EBX
	POP EBP
	RET
	

.align 16, 0xCC
th19a_render_layer_turbo_inner:
	MOVDQA XMM1, XMM0
	PCMPEQB XMM1, XMMWORD PTR [ESI]
	ADD ESI, 16
	ADD EDI, 64
	PMOVMSKB EBX, XMM1
	TEST EBX, EBX
	JNZ render_vms
	CMP ESI, EAX
	JNE th19a_render_layer_turbo_inner
end:
	RET
.align 16, 0xCC
render_vms:
	TEST BL, 0x01
	JZ skip_vm0
	PUSH DWORD PTR [EDI-0x3C]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm0:
	TEST BL, 0x02
	JZ skip_vm1
	PUSH DWORD PTR [EDI-0x38]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm1:
	TEST BL, 0x04
	JZ skip_vm2
	PUSH DWORD PTR [EDI-0x34]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm2:
	TEST BL, 0x08
	JZ skip_vm3
	PUSH DWORD PTR [EDI-0x30]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm3:
	TEST BL, 0x10
	JZ skip_vm4
	PUSH DWORD PTR [EDI-0x2C]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm4:
	TEST BL, 0x20
	JZ skip_vm5
	PUSH DWORD PTR [EDI-0x28]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm5:
	TEST BL, 0x40
	JZ skip_vm6
	PUSH DWORD PTR [EDI-0x24]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm6:
	TEST BL, BL
	JNS skip_vm7
	PUSH DWORD PTR [EDI-0x20]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm7:
	TEST BH, 0x01
	JZ skip_vm8
	PUSH DWORD PTR [EDI-0x1C]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm8:
	TEST BH, 0x02
	JZ skip_vm9
	PUSH DWORD PTR [EDI-0x18]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm9:
	TEST BH, 0x04
	JZ skip_vmA
	PUSH DWORD PTR [EDI-0x14]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmA:
	TEST BH, 0x08
	JZ skip_vmB
	PUSH DWORD PTR [EDI-0x10]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB:
	TEST BH, 0x10
	JZ skip_vmC
	PUSH DWORD PTR [EDI-0xC]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmC:
	TEST BH, 0x20
	JZ skip_vmD
	PUSH DWORD PTR [EDI-0x8]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmD:
	TEST BH, 0x40
	JZ skip_vmE
	PUSH DWORD PTR [EDI-0x4]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmE:
	TEST BH, BH
	JNS skip_vmF
	PUSH DWORD PTR [EDI]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmF:
	MOV EAX, DWORD PTR [ESP+8]
	CMP ESI, EAX
	JE end
	MOVD XMM0, DWORD PTR [ESP+4]
	PSHUFD XMM0, XMM0, 0
	JMP th19a_render_layer_turbo_inner
	INT3
render_layer_prep_dummy:
draw_vm_dummy:
	INT3
	
.align 16, 0xCC
th19a_render_layer_turbo_avx2:
	PUSH EBP
	PUSH EBX
	PUSH EDI
	PUSH ESI
	SUB ESP, 8
	MOV EBP, ECX
	MOV EDI, DWORD PTR [ECX+0x734]
	TEST EDI, EDI
	JZ skip_side_0B
	MOV EBX, EDX
	PUSH 0
	PUSH EDX
	CALL render_layer_prep_dummy
	AND EDI, -32
	MOV ESI, [EBP+0x73C]
	LEA EAX, [ESI*1+EDI+32]
	MOV DWORD PTR [ESP+4], EAX
	LEA EDI, [EAX-4]
	MOV DWORD PTR [ESP], EBX
	VMOVD XMM0, EBX
	VPBROADCASTB YMM0, XMM0
	CALL th19a_render_layer_turbo_inner_avx2
	MOVZX EDX, BYTE PTR [ESP]
	MOV ECX, EBP
skip_side_0B:
	MOV EDI, DWORD PTR [ECX+0x754]
	TEST EDI, EDI
	JZ skip_side_1B
	MOV EBX, EDX
	PUSH 1
	PUSH EDX
	CALL render_layer_prep_dummy
	AND EDI, -32
	MOV ESI, [EBP+0x75C]
	LEA EAX, [ESI*1+EDI+32]
	MOV DWORD PTR [ESP+4], EAX
	LEA EDI, [EAX-4]
	MOV DWORD PTR [ESP], EBX
	VMOVD XMM0, EBX
	VPBROADCASTB YMM0, XMM0
	CALL th19a_render_layer_turbo_inner_avx2
skip_side_1B:
	MOV EAX, 1
	ADD ESP, 8
	POP ESI
	POP EDI
	POP EBX
	POP EBP
	RET
	
.align 16, 0xCC
th19a_render_layer_turbo_inner_avx2:
	VPCMPEQB YMM1, YMM0, YMMWORD PTR [ESI]
	ADD ESI, 32
	SUB EDI, -128
	VPMOVMSKB EBX, YMM1
	TEST EBX, EBX
	JNZ render_vmsB
	CMP ESI, EAX
	JNZ th19a_render_layer_turbo_inner_avx2
endB:
	VZEROUPPER
	RET
.align 16, 0xCC
render_vmsB:
	VZEROUPPER
	TEST BL, 0x01
	JZ skip_vmB0
	PUSH DWORD PTR [EDI-0x7C]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB0:
	TEST BL, 0x02
	JZ skip_vmB1
	PUSH DWORD PTR [EDI-0x78]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB1:
	TEST BL, 0x04
	JZ skip_vmB2
	PUSH DWORD PTR [EDI-0x74]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB2:
	TEST BL, 0x08
	JZ skip_vmB3
	PUSH DWORD PTR [EDI-0x70]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB3:
	TEST BL, 0x10
	JZ skip_vmB4
	PUSH DWORD PTR [EDI-0x6C]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB4:
	TEST BL, 0x20
	JZ skip_vmB5
	PUSH DWORD PTR [EDI-0x68]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB5:
	TEST BL, 0x40
	JZ skip_vmB6
	PUSH DWORD PTR [EDI-0x64]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB6:
	TEST BL, BL
	JNS skip_vmB7
	PUSH DWORD PTR [EDI-0x60]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB7:
	TEST BH, 0x01
	JZ skip_vmB8
	PUSH DWORD PTR [EDI-0x5C]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB8:
	TEST BH, 0x02
	JZ skip_vmB9
	PUSH DWORD PTR [EDI-0x58]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB9:
	TEST BH, 0x04
	JZ skip_vmBA
	PUSH DWORD PTR [EDI-0x54]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmBA:
	TEST BH, 0x08
	JZ skip_vmBB
	PUSH DWORD PTR [EDI-0x50]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmBB:
	TEST BH, 0x10
	JZ skip_vmBC
	PUSH DWORD PTR [EDI-0x4C]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmBC:
	TEST BH, 0x20
	JZ skip_vmBD
	PUSH DWORD PTR [EDI-0x48]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmBD:
	TEST BH, 0x40
	JZ skip_vmBE
	PUSH DWORD PTR [EDI-0x44]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmBE:
	TEST BH, BH
	JNS skip_vmBF
	PUSH DWORD PTR [EDI-0x40]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmBF:
	TEST EBX, 0x00010000
	JZ skip_vmB10
	PUSH DWORD PTR [EDI-0x3C]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB10:
	TEST EBX, 0x00020000
	JZ skip_vmB11
	PUSH DWORD PTR [EDI-0x38]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB11:
	TEST EBX, 0x00040000
	JZ skip_vmB12
	PUSH DWORD PTR [EDI-0x34]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB12:
	TEST EBX, 0x00080000
	JZ skip_vmB13
	PUSH DWORD PTR [EDI-0x30]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB13:
	TEST EBX, 0x00100000
	JZ skip_vmB14
	PUSH DWORD PTR [EDI-0x2C]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB14:
	TEST EBX, 0x00200000
	JZ skip_vmB15
	PUSH DWORD PTR [EDI-0x28]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB15:
	TEST EBX, 0x00400000
	JZ skip_vmB16
	PUSH DWORD PTR [EDI-0x24]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB16:
	TEST EBX, 0x00800000
	JZ skip_vmB17
	PUSH DWORD PTR [EDI-0x20]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB17:
	TEST EBX, 0x01000000
	JZ skip_vmB18
	PUSH DWORD PTR [EDI-0x1C]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB18:
	TEST EBX, 0x02000000
	JZ skip_vmB19
	PUSH DWORD PTR [EDI-0x18]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB19:
	TEST EBX, 0x04000000
	JZ skip_vmB1A
	PUSH DWORD PTR [EDI-0x14]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB1A:
	TEST EBX, 0x08000000
	JZ skip_vmB1B
	PUSH DWORD PTR [EDI-0x10]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB1B:
	TEST EBX, 0x10000000
	JZ skip_vmB1C
	PUSH DWORD PTR [EDI-0xC]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB1C:
	TEST EBX, 0x20000000
	JZ skip_vmB1D
	PUSH DWORD PTR [EDI-0x8]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB1D:
	TEST EBX, 0x40000000
	JZ skip_vmB1E
	PUSH DWORD PTR [EDI-0x4]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB1E:
	TEST EBX, EBX
	JNS skip_vmB1F
	PUSH DWORD PTR [EDI]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB1F:
	MOV EAX, DWORD PTR [ESP+8]
	CMP ESI, EAX
	JE endB
	VPBROADCASTB YMM0, BYTE PTR [ESP+4]
	JMP th19a_render_layer_turbo_inner_avx2
	INT3

.align 16, 0xCC
world_layer_lookup:
.byte 26, 29, 30, 31, 32, 33, 34, 35, 36

.align 16, 0xCC
ui_layer_lookup:
.byte 43, 45, 45, 44, 45, 46, 47, 48, 49, 50, 51