.set @feat.00, 1

.intel_syntax noprefix

.global b_render_layer_turbo

b_render_layer_turbo:
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
	MOV EAX, EDI
	ADD EDI, -4
	MOV DWORD PTR [ESP+4], EBX
	MOV DWORD PTR [ESP], EAX
	VMOVD XMM0, EBX
	VPBROADCASTB YMM0, XMM0
.align 16
main_loop:
	VPCMPEQB YMM1, YMM0, YMMWORD PTR [ESI]
	ADD ESI, 32
	VPMOVMSKB EBX, YMM1
	SUB EDI, -128
	TEST EBX, EBX
	JNZ render_vms
	CMP ESI, EAX
	JNE main_loop
	VZEROUPPER
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
	VZEROUPPER
	TEST BL, 0x01
	JZ skip_vm0
	PUSH DWORD PTR [EDI-0x7C]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm0:
	TEST BL, 0x02
	JZ skip_vm1
	PUSH DWORD PTR [EDI-0x78]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm1:
	TEST BL, 0x04
	JZ skip_vm2
	PUSH DWORD PTR [EDI-0x74]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm2:
	TEST BL, 0x08
	JZ skip_vm3
	PUSH DWORD PTR [EDI-0x70]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm3:
	TEST BL, 0x10
	JZ skip_vm4
	PUSH DWORD PTR [EDI-0x6C]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm4:
	TEST BL, 0x20
	JZ skip_vm5
	PUSH DWORD PTR [EDI-0x68]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm5:
	TEST BL, 0x40
	JZ skip_vm6
	PUSH DWORD PTR [EDI-0x64]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm6:
	TEST BL, BL
	JNS skip_vm7
	PUSH DWORD PTR [EDI-0x60]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm7:
	TEST BH, 0x01
	JZ skip_vm8
	PUSH DWORD PTR [EDI-0x5C]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm8:
	TEST BH, 0x02
	JZ skip_vm9
	PUSH DWORD PTR [EDI-0x58]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm9:
	TEST BH, 0x04
	JZ skip_vmA
	PUSH DWORD PTR [EDI-0x54]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmA:
	TEST BH, 0x08
	JZ skip_vmB
	PUSH DWORD PTR [EDI-0x50]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmB:
	TEST BH, 0x10
	JZ skip_vmC
	PUSH DWORD PTR [EDI-0x4C]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmC:
	TEST BH, 0x20
	JZ skip_vmD
	PUSH DWORD PTR [EDI-0x48]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmD:
	TEST BH, 0x40
	JZ skip_vmE
	PUSH DWORD PTR [EDI-0x44]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmE:
	TEST BH, BH
	JNS skip_vmF
	PUSH DWORD PTR [EDI-0x40]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vmF:
	TEST EBX, 0x00010000
	JZ skip_vm10
	PUSH DWORD PTR [EDI-0x3C]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm10:
	TEST EBX, 0x00020000
	JZ skip_vm11
	PUSH DWORD PTR [EDI-0x38]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm11:
	TEST EBX, 0x00040000
	JZ skip_vm12
	PUSH DWORD PTR [EDI-0x34]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm12:
	TEST EBX, 0x00080000
	JZ skip_vm13
	PUSH DWORD PTR [EDI-0x30]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm13:
	TEST EBX, 0x00100000
	JZ skip_vm14
	PUSH DWORD PTR [EDI-0x2C]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm14:
	TEST EBX, 0x00200000
	JZ skip_vm15
	PUSH DWORD PTR [EDI-0x28]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm15:
	TEST EBX, 0x00400000
	JZ skip_vm16
	PUSH DWORD PTR [EDI-0x24]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm16:
	TEST EBX, 0x00800000
	JZ skip_vm17
	PUSH DWORD PTR [EDI-0x20]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm17:
	TEST EBX, 0x01000000
	JZ skip_vm18
	PUSH DWORD PTR [EDI-0x1C]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm18:
	TEST EBX, 0x02000000
	JZ skip_vm19
	PUSH DWORD PTR [EDI-0x18]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm19:
	TEST EBX, 0x04000000
	JZ skip_vm1A
	PUSH DWORD PTR [EDI-0x14]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm1A:
	TEST EBX, 0x08000000
	JZ skip_vm1B
	PUSH DWORD PTR [EDI-0x10]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm1B:
	TEST EBX, 0x10000000
	JZ skip_vm1C
	PUSH DWORD PTR [EDI-0xC]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm1C:
	TEST EBX, 0x20000000
	JZ skip_vm1D
	PUSH DWORD PTR [EDI-0x8]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm1D:
	TEST EBX, 0x40000000
	JZ skip_vm1E
	PUSH DWORD PTR [EDI-0x4]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm1E:
	TEST EBX, EBX
	JNS skip_vm1F
	PUSH DWORD PTR [EDI]
	MOV ECX, EBP
	CALL draw_vm_dummy
skip_vm1F:
	MOV EAX, DWORD PTR [ESP]
	CMP ESI, EAX
	JE end
	VPBROADCASTB YMM0, BYTE PTR [ESP+4]
	JMP main_loop
	INT3
render_layer_prep_dummy:
draw_vm_dummy:
	INT3
