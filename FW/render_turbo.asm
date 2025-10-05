.set @feat.00, 1

.intel_syntax noprefix

.global a_render_layer_turbo

a_render_layer_turbo:
	PUSH EBP
	PUSH EBX
	PUSH EDI
	PUSH ESI
	SUB ESP, 8
	MOV ESI, DWORD PTR [ECX+0x708]
	MOV EDI, DWORD PTR [ECX+0x70C]
	CMP ESI, EDI
	JZ end
	MOV EBX, EDX
	MOV EBP, ECX
	PUSH 0
	PUSH EDX
	CALL render_layer_prep_dummy
	IMUL EDX, EBX, 0x01010101
	MOV EAX, EDI
	ADD EDI, -4
	MOV DWORD PTR [ESP+4], EDX
	MOV DWORD PTR [ESP], EAX
	MOVD XMM0, EDX
.nops ((. + 0xF) & -0x10) - . - (main_loop - splat_layer)
splat_layer:
	PSHUFD XMM0, XMM0, 0
main_loop:
	MOVDQA XMM1, XMMWORD PTR [ESI]
	PCMPEQB XMM1, XMM0
	ADD ESI, 16
	PMOVMSKB EBX, XMM1
	ADD EDI, 64
	TEST EBX, EBX
	JNZ render_vms
	CMP ESI, EAX
	JNE main_loop
end:
	MOV EAX, 1
	ADD ESP, 8
	POP ESI
	POP EDI
	POP EBX
	POP EBP
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
	MOV EAX, DWORD PTR [ESP]
	CMP ESI, EAX
	JE end
	MOVD XMM0, DWORD PTR [ESP+4]
	JMP splat_layer
	INT3
render_layer_prep_dummy:
draw_vm_dummy:
	INT3
