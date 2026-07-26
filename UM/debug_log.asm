vm = 0x14
param_mask = 0x18
stack_base = 0x1C
stack_pointer = 0x20
get_float_var = 0x24
get_int_var = 0x28
format_backup = 0x2C
fcvt = 0x30
values = 0x34

    PUSH EBP
    PUSH EBX
    PUSH EDI
    PUSH ESI
    SUB ESP, 0x354
    MOV ESI, [ECX + 0x1018]
    MOV [ESP + vm], ESI
    MOV ESI, [ESI]
    MOV EDI, [ESI + 4]
    MOV [ESP + get_int_var], EDI
    MOV ESI, [ESI + 0xC]
    MOV [ESP + get_float_var], ESI
    LEA ESI, [ECX + 0xC]
    MOV EDI, [ECX + 0x100C]
    ADD EDI, ESI
    MOV [ESP + stack_pointer], EDI
    ADD ESI, [ECX + 0x1010]
    MOV [ESP + stack_base], ESI
    MOVZX ECX, WORD PTR [EDX + 8]
    MOV [ESP + param_mask], ECX
    LEA ESI, [EAX*4 + EDX + 0x14]
    MOV [ESP + format_backup], ESI
    MOV EDI, [EAX*4 + EDX + 0x10]
    ADD EDI, ESI
    MOV EBP, EAX
    XOR EBX, EBX
    JMP arg_loop

.align 16, 0xCC

arg_loop_read:
    MOVZX EDX, BYTE PTR [ESI]
    MOVZX EAX, BYTE PTR [EDX + case_table]
    JMP DWORD PTR [EAX*4 + arg_jump_table]

arg_loop_int:
    MOVZX EAX, BYTE PTR [EDI]
    MOV ECX, [ESP + param_mask]
    BT ECX, EBP
    JNC int_arg_not_var
    CMP AL, 'f'
    JE float_as_int
    MOV EAX, [EDI + 4]
    CMP EAX, -100
    JGE int_as_int_not_global
    MOV ECX, [ESP + vm]
    MOV [ESP], EAX
    CALL DWORD PTR [ESP + get_int_var]
    SUB ESP, 4
    MOV ECX, EAX
    JMP int_reload_c

arg_loop_float:
    MOVZX EAX, BYTE PTR [EDI]
    MOVSS XMM0, [EDI + 4]
    MOV ECX, [ESP + param_mask]
    BT ECX, EBP
    JNC float_arg_not_var
    CMP AL, 'f'
    JNE int_as_float
    CVTTSS2SI EAX, XMM0
    CMP EAX, -100
    JGE float_as_float_not_global
    MOV ECX, [ESP + vm]
    MOV [ESP], EAX
    CALL DWORD PTR [ESP + get_float_var]
    SUB ESP, 4
    FSTP DWORD PTR [ESP + fcvt]
    MOVSS XMM0, [ESP + fcvt]
    JMP float_reload_c

float_as_int:
    CVTTSS2SI EAX, [EDI + 4]
    CMP EAX, -100
    JL float_as_int_global
int_as_int_not_global:
    TEST EAX, EAX
    JS int_var_stack
    MOV ECX, [ESP + stack_base]
    MOV ECX, [EAX*1 + ECX]
    JMP int_arg_done

int_var_stack:
    MOV ECX, [ESP + stack_pointer]
    LEA EAX, [EAX*8 + ECX]
    CVTTSS2SI ECX, [EAX + 4]
    CMP BYTE PTR [EAX], 'f'
    CMOVNE ECX, [EAX + 4]
    JMP int_arg_done

int_as_float:
    MOVD EAX, XMM0
    CMP EAX, -100
    JL int_as_float_global
float_as_float_not_global:
    TEST EAX, EAX
    JS float_var_stack
    MOV ECX, [ESP + stack_base]
    MOVSS XMM0, [EAX*1 + ECX]
    JMP float_arg_done

int_arg_not_var:
    CVTTSS2SI ECX, [EDI + 4]
    CMP AL, 'f'
    CMOVNE ECX, [EDI + 4]
    JMP int_arg_done

float_as_int_global:
    MOV ECX, [vm]
    MOV [ESP], EAX
    CALL DWORD PTR [ESP + get_float_var]
    SUB ESP, 4
    FSTP DWORD PTR [ESP + fcvt]
    CVTTSS2SI ECX, [ESP + fcvt]
int_reload_c:
    MOVZX EDX, BYTE PTR [ESI]
int_arg_done:
    MOV [EBP*4 + ESP + values - 4], ECX
    MOV EAX, 5
    CMP DL, 'B'
    JE int_arg_is_bool
    MOVD XMM0, ECX
    JMP int_arg_length

float_var_stack:
    MOV ECX, [ESP + stack_pointer]
    MOVSS XMM0, [EAX*8 + ECX + 4]
    MOVZX EAX, BYTE PTR [EAX*8 + ECX]
float_arg_not_var:
    CMP AL, 'f'
    JE float_arg_done
    CVTDQ2PS XMM0, XMM0
    JMP float_arg_done
    
arg_invalid:
    INC EBX
arg_literal_percent:
    INC EBX
    JMP arg_loop_next

int_as_float_global:
    MOV ECX, [ESP + vm]
    MOV [ESP], EAX
    CALL DWORD PTR [ESP + get_int_var]
    SUB ESP, 4
    XORPS XMM0, XMM0
    CVTSI2SS XMM0, EAX
float_reload_c:
    MOVZX EDX, BYTE PTR [ESI]
float_arg_done:
    MOVSS [EBP*4 + ESP + values - 4], XMM0
    CVTSS2SD XMM0, XMM0
int_arg_length:
    LEA EAX, [EDX*8 + format_strings]
    MOVLPS [ESP + 0xC], XMM0
    MOV [ESP + 8], EAX
    MOVHPS [ESP], XMM0
    CALL _snprintf
int_arg_is_bool:
    ADD EBX, EAX
    INC EBP
    ADD EDI, 8
arg_loop_next:
    INC ESI

.align 16

arg_loop:
    MOVZX EAX, BYTE PTR [ESI]
    INC ESI
    CMP AL, '%'
    JE arg_loop_read
    INC EBX
    TEST AL, AL
    JNZ arg_loop
    MOV [ESP], EBX
    CALL _malloc
    MOV ESI, [ESP + format_backup]
    LEA EDI, [ESP + values]
    MOV EBP, EAX
    MOV EBX, EAX
    JMP print_loop

.align 8, 0xCC

print_loop_end:
    MOV BYTE PTR [EBX], 0
    JMP final_print

.align 16, 0xCC

print_loop_read:
    MOVZX EDX, BYTE PTR [ESI]
    MOVZX EAX, BYTE PTR [EDX + case_table]
    JMP DWORD PTR [EAX*4 + print_jump_table]

print_loop_int:
    MOVSS XMM0, [EDI]
    JMP print_val

print_loop_float:
    XORPS XMM0, XMM0
    CVTSS2SD XMM0, [EDI]
print_val:
    LEA EAX, [EDX*8 + format_strings]
    MOVLPS [ESP + 8], XMM0
    MOV [ESP + 4], EAX
    MOV [ESP], EBX
    CALL _sprintf
    ADD EBX, EAX
print_loop_arg_done:
    ADD EDI, 4
print_loop_next:
    INC ESI

.align 16

print_loop:
    MOVZX EAX, BYTE PTR [ESI]
    INC ESI
    CMP AL, '%'
    JE print_loop_read
    MOV [EBX], AL
    INC EBX
    TEST AL, AL
    JNZ print_loop
    DEC EBX
final_print:
    MOV EAX, EBP
    SUB EBX, EBP
    MOV EDX, EBX
    ADD ESP, 0x354
    POP ESI
    POP EDI
    POP EBX
    POP EBP
    RET

.align 4, 0xCC

print_loop_invalid:
    MOV BYTE PTR [EBX], '%'
    INC EBX
print_loop_literal_percent:
    MOV BYTE PTR [EBX], AL
    INC EBX
    JMP print_loop_next

.align 16, 0xCC

print_loop_bool:
    CMP DWORD PTR [EDI], 0
    JE print_loop_false
    MOV DWORD PTR [EBX], 0x65757274
    ADD EBX, 4
    JMP print_loop_arg_done

print_loop_false:
    MOV DWORD PTR [EBX], 0x736C6166
    MOV BYTE PTR [EBX + 4], 0x65
    ADD EBX, 5
    JMP print_loop_arg_done
