#if 1 || INCLUDE_PATCH_INSTRUCTIONS
namespace thcrap_print_impl {

static inline constexpr char format_strings[][8] = {
	['S'] = "%i",
	['U'] = "%u",
	['i'] = "%i",
	['s'] = "%hi",
	['u'] = "%hu",
	['b'] = "%hhu",
	['c'] = "%hhi",
	['C'] = "%X",
	//['X'] = "%X",
	['e'] = "%e",
	['f'] = "%f",
	['g'] = "%g"
};

dllexport gnu_noinline void cdecl log_print(const char* text) {
	printf("%s\n", text);
	//use_var(text);
}
dllexport gnu_noinline void cdecl log_nprint(const char* text, size_t n) {
	printf("%*s\n", (int)strnlen(text, n), text);
	//use_var(text);
	//use_var(n);
}

gnu_noinline uint64_t ecl_format(/*uint32_t va_index, const EclInstruction* current_instruction, EclContext* ecl_context*/) asm("_ecl_format");
gnu_noinline uint64_t ecl_format(/*uint32_t va_index, const EclInstruction* current_instruction, EclContext* ecl_context*/) {

	uint32_t va_index;
	const EclInstruction* current_instruction;
	EclContext* ecl_context;
	__asm__ volatile (
		""
		: "=a"(va_index), "=d"(current_instruction), "=c"(ecl_context)
	);
	assume(va_index > 0);

	EclVM* vm_ptr = ecl_context->vm;
	EclVM *volatile vm = vm_ptr;
	uintptr_t* vtable = *(uintptr_t**)vm_ptr;
	auto get_int_var = *(int32_t(EclVM::**)(int32_t))&vtable[1];
	auto get_float_var = *(float(EclVM::**)(int32_t))&vtable[3];

	const EclValue *volatile stack_pointer = (EclValue*)&ecl_context->stack.raw[ecl_context->stack.pointer];
	const uint8_t *volatile stack_base = (uint8_t*)&ecl_context->stack.raw[ecl_context->stack.base];

	volatile uint32_t param_mask = current_instruction->param_mask;
	
	const unsigned char* format = (unsigned char*)StringArg((va_index - 1) * 4 + 0x4);

	const unsigned char *volatile format_backup = format;

	const EclRawValue* print_args_ptr = (const EclRawValue*)(format + IntArg(va_index - 1));

	EclRawValue values[200];

	size_t buffer_length = 0;
	register size_t arg_index asm("ebp") = va_index;

	uint32_t printed_length;
	//uint8_t c;
	//uint32_t c_wide;
	uint32_t c;

	double xmm_dbl;
	float fcvt;
	vec<float, 4> xmm_val;

#define FORCE_PRINT_REGS() \
	__asm__ volatile ( \
		"" \
		: "+r"(arg_index), "+d"(c), "+D"(print_args_ptr), "+S"(format), "+b"(buffer_length), "+m"(vm), "+m"(get_int_var), "+m"(get_float_var), "+m"(stack_pointer), "+m"(stack_base), "+m"(param_mask), "+m"(format_backup) \
	)

	FORCE_PRINT_REGS();

	__asm__ volatile (
		"JMP %[arg_loop_start] \n"
		".align 16, 0xCC \n"
		:
		: [arg_loop_start]"i"(&& arg_loop_start)
	);

arg_loop_read:
	FORCE_PRINT_REGS();
	c = *format;
	FORCE_PRINT_REGS();
	//++format;
	switch (c) {
		default:
			++buffer_length;
		case '%':
			++buffer_length;
			goto arg_loop_next;
		case '\0':
			goto start_print_loop;
		case 'e': case 'f': case 'g':
		{
			uint8_t type = print_args_ptr[0].type;
			xmm_val = (vec<float, 4>){ print_args_ptr[1].real };
			__asm__ volatile ("":"+x"(xmm_val));
			if (expect_chance((bool)(param_mask & (1 << arg_index)), true, 0.9)) {
				int32_t value;
				if (expect_chance(type == 'f', true, 0.6)) {
					value = xmm_val[0];
					if (value < -100) {
						__asm__(
							"FSTPS %[fcvt]"
							: asm_arg("=m", fcvt)
							: "t"((vm->*get_float_var)(value))
							: clobber_list("st")
						);
						xmm_val = (vec<float, 4>){ fcvt };
						c = *format;
					}
					else {
					float_var_read:
						if (value >= 0) {
							//val = ecl_context->stack.read_local(value);
							//xmm_val = (vec<float, 4>){ ecl_context->stack.read_offset<float>(ecl_context->stack.base + value) };
						//float_local_val:
							xmm_val = (vec<float, 4>){ *(float*)&stack_base[value] };
						} else /*if (value <= -1 && value >= -100)*/ {
							//val = ecl_context->stack.read_temp(value);
						//float_stack_val:
							{
								const EclValue* ecl_value = &stack_pointer[value];
								xmm_val = (vec<float, 4>){ ecl_value->value.real };
								type = ecl_value->type;
							}
						cast_float_stack:
							
							if (type != 'f') {
								xmm_val = convertvec((vec<int32_t, 4>)xmm_val, vec<float, 4>);
							}
							
						}
						/*
						else {
							__asm__(
								"FSTPS %[fcvt]"
								: asm_arg("=m", fcvt)
								: "t"((vm->*get_float_var)(value))
								: clobber_list("st")
							);
							xmm_val = (vec<float, 4>){ fcvt };
							c = *format;
						}
						*/
					}
				} else {
					value = bitcast<int32_t>(xmm_val[0]);
					if (value >= -100) {
						goto float_var_read;
					}
					xmm_val = convertvec((vec<int32_t, 4>) { (vm->*get_int_var)(value) }, vec<float, 4>);
					c = *format;
					//else {
						/*
						if (value >= 0) {
							goto float_local_val;
							//val = ecl_context->stack.read_local(value);
							//xmm_val = (vec<float, 4>){ ecl_context->stack.read_offset<int32_t>(ecl_context->stack.base + value) };
							//xmm_val = (vec<float, 4>){ *(float*)&stack_base[value] };
						} else if (value <= -1 && value >= -100) {
							goto float_stack_val;
							//xmm_val = (vec<float, 4>){ ecl_context->stack.read_temp(value) };
							//const EclValue* ecl_value = &stack_pointer[value];
							//xmm_val = (vec<float, 4>){ ecl_value->value.real };
							//if (ecl_value->type != 'f') {
								//xmm_val = convertvec((vec<int32_t, 4>)xmm_val, vec<float, 4>);
							//}
						} else {
							//xmm_val = (vec<float, 4>){ ecl_context->vm->get_int_var(value) };
							xmm_val = convertvec((vec<int32_t, 4>){ (vm->*get_int_var)(value) }, vec<float, 4>);
						}
						*/
					//}
				}
			} else {
				goto cast_float_stack;
				/*
		cast_float_stack:
				if (expect_chance(type == 'f', true, 0.6)) {
					//val = print_args_ptr[1].real;
				} else {
					//val = print_args_ptr[1].integer;
					//val = bitcast<int32_t>(val);
					xmm_val = convertvec((vec<int32_t, 4>)xmm_val, vec<float, 4>);
				}
				*/
			}
			values[arg_index - 1].real = xmm_val[0];
			//full_val = xmm_val[0];
			(*(vec<double, 2>*)&xmm_val)[0] = xmm_val[0];
			//printed_length = snprintf(NULL, 0, format_strings[c], val);
			//goto arg_loop_end_arg;
			FORCE_PRINT_REGS();
			//c = *format;
			//__asm__ volatile ("":"+r"(c));
			break;
		}
		case 'c': case 'b':
		case 's': case 'u':
		case 'S': case 'U':
		case 'C': case 'B':
		{
			/*
			int32_t val;
			switch (expect(print_args_ptr[0].type, 'S')) {
				default:
					val = ecl_context->parse_int_as_arg(arg_index, print_args_ptr[1].integer, current_instruction);
					break;
				case 'f':
					val = ecl_context->parse_float_as_arg(arg_index, print_args_ptr[1].real, current_instruction);
					break;
			}
			*/
			int32_t val;
			uint8_t type = print_args_ptr[0].type;
			if (expect_chance((bool)(param_mask & (1 << arg_index)), true, 0.9)) {
				int32_t value;
				if (expect_chance(type != 'f', true, 0.6)) {
					value = print_args_ptr[1].integer;
					if (value < -100) {
						//val = ecl_context->vm->get_int_var(value);
						val = (vm->*get_int_var)(value);
						c = *format;
					}
					else {
					int_var_read:
						if (value >= 0) {
							//val = ecl_context->stack.read_local(value);
						//int_local_val:
							val = *(int32_t*)&stack_base[value];
						} else /*if (value <= -1 && value >= -100)*/ {
							//val = ecl_context->stack.read_temp(value);
						//int_stack_val:
							const EclValue* ecl_value = &stack_pointer[value];
							__asm__ volatile ("":"+r"(ecl_value));
							__asm__(
								"CVTTSS2SI %[stack], %[val] \n"
								"CMPB $'f', %[type] \n"
								"CMOVNE %[stack], %[val] \n"
								: asm_arg("=&r", val)
								: [stack] "m"(ecl_value->value.integer), [type]"m"(ecl_value->type)
							);
						}
						/*
						else {
							//val = ecl_context->vm->get_int_var(value);
							val = (vm->*get_int_var)(value);
						c = *format;
						}
						*/
					}
				} else {
					value = print_args_ptr[1].real;
					if (value >= -100) {
						goto int_var_read;
					}
					__asm__(
						"FSTPS %[fcvt]"
						: asm_arg("=m", fcvt)
						: "t"((vm->*get_float_var)(value))
						: clobber_list("st")
					);
					val = fcvt;
					c = *format;
					/*
					if (value >= 0) {
						goto int_local_val;
						//val = ecl_context->stack.read_local(value);
						//val = ecl_context->stack.read_offset<float>(ecl_context->stack.base + value);
						//val = *(int32_t*)&stack_base[value];
					} else if (value <= -1 && value >= -100) {
						goto int_stack_val;
						//val = ecl_context->stack.read_temp(value);
						//const EclValue* ecl_value = &stack_pointer[value];
						//__asm__(
							//"CVTTSS2SI %[stack], %[val] \n"
							//"CMPB $'f', %[type] \n"
							//"CMOVNE %[stack], %[val] \n"
							//: asm_arg("=&r", val)
							//: [stack]"m"(ecl_value->value.integer), [type]"m"(ecl_value->type)
						//);
					} else {
						//val = ecl_context->vm->get_float_var((int32_t)value);
						__asm__(
							"FSTPS %[fcvt]"
							: asm_arg("=m", fcvt)
							: "t"((vm->*get_float_var)(value))
							: clobber_list("st")
						);
						val = fcvt;
					}
					*/
				}
			} else {
				__asm__(
					"CVTTSS2SI %[stack], %[val] \n"
					"CMPB $'f', %[type] \n"
					"CMOVNE %[stack], %[val] \n"
					: asm_arg("=&r", val)
					: [stack]"m"(print_args_ptr[1].integer), [type]"r"(type)
				);
				/*
				if (expect_chance(print_args_ptr[0].type != 'f', true, 0.6)) {
					val = print_args_ptr[1].integer;
				} else {
					val = print_args_ptr[1].real;
				}
				*/
			}

			values[arg_index - 1].integer = val;
			printed_length = 5;
			__asm__ volatile ("":"+a"(printed_length));
			FORCE_PRINT_REGS();
			//c_wide = *format;
			//__asm__ volatile ("":"+r"(c_wide));
			if ((uint8_t)c == 'B') {
				goto arg_loop_end_bool;
			}
			__asm__ volatile ("":"+r"(val));
			//full_val = bitcast<double>((uint64_t)(uint32_t)val);
			//xmm_val = (vec<float, 4>){ __builtin_bit_cast(float, val) };
			xmm_val = (vec<float, 4>)((vec<uint32_t, 4>){ val });
			//printed_length = snprintf(NULL, 0, format_strings[c], val);
			//goto arg_loop_end_arg;
			break;
		}
	}

	FORCE_PRINT_REGS();
	//__asm__ volatile ("":"+r"(c_wide));
	printed_length = snprintf(NULL, 0, format_strings[c], ((vec<double, 2>)xmm_val)[0]);
	__asm__ volatile ("":"+a"(printed_length));
arg_loop_end_bool:
	FORCE_PRINT_REGS();
	buffer_length += printed_length;
	++arg_index;
	print_args_ptr += 2;
arg_loop_next:
	++format;
	FORCE_PRINT_REGS();
	__asm__ volatile (".align 16":);
arg_loop_start:
	__asm__ (
	"1: \n"
		"MOVZBL (%[format]), %k[c] \n"
		"INC %[format] \n"
		"CMP $'%%', %[c] \n"
		"JE %[arg_loop_read] \n"
		"INC %[buffer_length] \n"
		"TEST %[c], %[c] \n"
		"JNZ 1b \n"
		: asm_arg("=r", c), asm_arg("+b", buffer_length), asm_arg("+S", format), "+r"(arg_index), "+D"(print_args_ptr), "+m"(vm), "+m"(get_int_var), "+m"(get_float_var), "+m"(stack_pointer), "+m"(stack_base), "+m"(param_mask), "+m"(format_backup)
		: [arg_loop_read]"i"(&& arg_loop_read)
	);

start_print_loop:
	register char* text asm("ebp") = (char*)malloc(buffer_length);
	char* text_write = text;

	format = (const unsigned char*)format_backup;
	print_args_ptr = values;

#define FORCE_PRINT_REGS2() \
	__asm__ volatile ( \
		"" \
		: "+r"(text), "+D"(print_args_ptr), "+S"(format), "+b"(text_write) \
	)

	FORCE_PRINT_REGS2();

	__asm__ (
		"JMP %[print_loop_start] \n"
		".align 16, 0xCC \n"
		:
		: [print_loop_start]"i"(&& print_loop_start)
		:
	);

print_loop_read:
	FORCE_PRINT_REGS2();
	c = *format;
	//++format;
	switch (c) {
		default:
			*text_write++ = '%';
		case '%':
			*text_write++ = c;
			goto print_loop_next;
		case '\0':
			*text_write = c;
			goto final_print;
		case 'B':
			if (print_args_ptr->integer) {
				*(uint32_t*)text_write = PackUInt32('t', 'r', 'u', 'e');
				text_write += 4;
			} else {
				*(uint32_t*)text_write = PackUInt32('f', 'a', 'l', 's');
				text_write[4] = 'e';
				text_write += 5;
				FORCE_PRINT_REGS2();
				__asm__ goto (
					"JMP %l[print_loop_inc_arg]"
					:
					:
					:
					: print_loop_inc_arg
				);
			}
			FORCE_PRINT_REGS2();
			goto print_loop_inc_arg;
			//__asm__ volatile ("":"+a"(printed_length));
			//goto print_loop_inc_arg;
		case 'e': case 'f': case 'g':
			xmm_dbl = print_args_ptr->real;
			//xmm_val = (vec<float, 4>)((vec<double,2>){ print_args_ptr->real });
			//xmm_val = (vec<float, 4>){ print_args_ptr->real };
			//(*(vec<double, 2>*) & xmm_val)[0] = xmm_val[0];
			//xmm_val = {};
			//(*(vec<double, 2>*)&xmm_val)[0] = print_args_ptr->real;
			break;
		case 'c': case 'b':
		case 's': case 'u':
		case 'S': case 'U':
		case 'C':
			xmm_dbl = bitcast<double>((uint64_t)(uint32_t)print_args_ptr->integer);
			//xmm_val = (vec<float, 4>)((vec<uint32_t, 4>){ print_args_ptr->integer });
			break;
	}

	FORCE_PRINT_REGS2();
	//printed_length = sprintf(text_write, format_strings[c], ((vec<double, 2>)xmm_val)[0]);
	printed_length = sprintf(text_write, format_strings[c], xmm_dbl);
	__asm__ volatile ("":"+a"(printed_length));
	FORCE_PRINT_REGS2();
	text_write += printed_length;
print_loop_inc_arg:
	++print_args_ptr;
print_loop_next:
	++format;
	FORCE_PRINT_REGS2();
	__asm__ volatile (".align 16":);
print_loop_start:
	__asm__ (
	"1: \n"
		"MOVZBL (%[format]), %k[c] \n"
		"INC %[format] \n"
		"CMP $'%%', %[c] \n"
		"JE %[print_loop_read] \n"
		"MOV %[c], (%[text_write]) \n"
		"INC %[text_write] \n"
		"TEST %[c], %[c] \n"
		"JNZ 1b \n"
		: asm_arg("=r", c), asm_arg("+b", text_write), asm_arg("+S", format), asm_arg("+D", print_args_ptr)
		: [print_loop_read]"i"(&&print_loop_read)
		: "memory"
	);
	--text_write;
final_print:
	FORCE_PRINT_REGS2();
	//log_print(text);
	//free(text);
	return pack_dreg(text, text_write - text);

	// unreachable


	/*
	for (;;) {
		++buffer_length;
		uint8_t c = *format_read++;

		switch (c) {
			[[likely]] default:
				[[likely]] continue;
			case '%': {
				c = *format_read++;
				switch (c) {
					[[unlikely]] default:
						[[unlikely]] ++buffer_length;
					case '%':
						continue;
					case 'c': case 'b':
					case 's': case 'u':
					case 'S': case 'U':
					case 'C': case 'B':
					{
						int32_t val;
						switch (expect(print_args_ptr[0].type, 'S')) {
							default:
								val = ecl_context->parse_int_as_arg(arg_index, print_args_ptr[1].integer, current_instruction);
								break;
							case 'f':
								val = ecl_context->parse_float_as_arg(arg_index, print_args_ptr[1].real, current_instruction);
								break;
						}
						values[arg_index - 1].integer = val;
						if (c != 'B') {
							buffer_length += snprintf(NULL, 0, format_strings[c], val) - 1;
						} else {
							buffer_length += 4;
						}
						++arg_index;
						print_args_ptr += 2;
						continue;
					}
					case 'e': case 'f': case 'g':
					{
						float val;
						switch (expect(print_args_ptr[0].type, 'f')) {
							default:
								val = ecl_context->parse_int_as_arg(arg_index, print_args_ptr[1].integer, current_instruction);
								break;
							case 'f':
								val = ecl_context->parse_float_as_arg(arg_index, print_args_ptr[1].real, current_instruction);
								break;
						}
						values[arg_index - 1].real = val;
						buffer_length += snprintf(NULL, 0, format_strings[c], val) - 1;
						++arg_index;
						print_args_ptr += 2;
						continue;
					}
					case '\0':;
				}
			}
			case '\0': {
				char text[buffer_length];
				char* text_write = text;

				print_args_ptr = values;

				for (;;) {
					c = *format++;
					switch (c) {
						[[likely]] default:
							[[likely]] *text_write++ = c;
							continue;
						case '%': {
							c = *format++;
							switch (c) {
								default:
									*text_write++ = '%';
								case '%':
									*text_write++ = c;
									continue;
								case 'B':
									if (print_args_ptr++->integer) {
										*text_write++ = 't';
										*text_write++ = 'r';
										*text_write++ = 'u';
										*text_write++ = 'e';
									} else {
										*text_write++ = 'f';
										*text_write++ = 'a';
										*text_write++ = 'l';
										*text_write++ = 's';
										*text_write++ = 'e';
									}
									continue;
								case 'c': case 'b':
								case 's': case 'u':
								case 'S': case 'U':
								case 'C':
								{
									text_write += sprintf(text_write, format_strings[c], print_args_ptr->integer);
									++print_args_ptr;
									continue;
								}
								case 'e': case 'f': case 'g':
								{
									text_write += sprintf(text_write, format_strings[c], print_args_ptr->real);
									++print_args_ptr;
									continue;
								}
								case '\0':;
							}
						}
						case '\0':
							*text_write = '\0';
							printf("%s", text);
							return;
					}
				}
			}
		}
	}
	*/
}

dllexport naked uint64_t ecl_format_asm() {
#define VM_VAR "0x14"
#define PARAM_MASK "0x18"
#define STACK_BASE "0x1C"
#define STACK_POINTER "0x20"
#define GET_FLOAT_VAR "0x24"
#define GET_INT_VAR "0x28"
#define FORMAT_BACKUP "0x2C"
#define FCVT_VAR "0x30"
#define VALUES_ARRAY "0x34"

	__asm__(
		INTEL_SYNTAX_DIRECTIVE
		"PUSH EBP \n"
		"PUSH EBX \n"
		"PUSH EDI \n"
		"PUSH ESI \n"
		"SUB ESP, 0x354 \n"
		"MOV ESI, [ECX + 0x1018] \n"
		"MOV [ESP + " VM_VAR "], ESI \n"
		"MOV ESI, [ESI] \n"
		"MOV EDI, [ESI + 4] \n"
		"MOV [ESP + " GET_INT_VAR "], EDI \n"
		"MOV ESI, [ESI + 0xC] \n"
		"MOV [ESP + " GET_FLOAT_VAR "], ESI \n"
		"LEA ESI, [ECX + 0x8C] \n"
		"MOV EDI, [ECX + 0x100C] \n"
		"ADD EDI, ESI \n"
		"MOV [ESP + " STACK_POINTER "], EDI \n"
		"ADD ESI, [ECX + 0x1010] \n"
		"MOV [ESP + " STACK_BASE "], ESI \n"
		"MOVZX ECX, WORD PTR [EDX + 8] \n"
		"MOV [ESP + " PARAM_MASK "], ECX \n"
		"LEA ESI, [EAX*4 + EDX + 0x10] \n"
		"MOV [ESP + " FORMAT_BACKUP "], ESI \n"
		"MOV EDI, [EAX*4 + EDX + 0xC] \n"
		"ADD EDI, ESI \n"
		"MOV EBP, EAX \n"
		"XOR EBX, EBX \n"
		"JMP eclf_arg_loop \n"

	".align 16, 0xCC \n"

	"eclf_arg_loop_read: \n"
		//"MOVZX EAX, BYTE PTR [ESI] \n"
		//"CMP EAX, 0x75 \n"
		//"JA eclf_arg_invalid \n"
		//"JMP DWORD PTR [EAX*4 + eclf_arg_jump_table] \n"
		"MOVZX EDX, BYTE PTR [ESI] \n"
		"MOVZX EAX, BYTE PTR [EDX + eclf_case_table] \n"
		"JMP DWORD PTR [EAX*4 + eclf_arg_jump_table] \n"

	"eclf_arg_loop_int: \n"
		"MOVZX EAX, BYTE PTR [EDI] \n"
		"MOV ECX, [ESP + " PARAM_MASK "] \n"
		"BT ECX, EBP \n"
		"JNC eclf_int_arg_not_var \n"
		"CMP AL, 'f' \n"
		"JE eclf_float_as_int \n"
		"MOV EAX, [EDI + 4] \n"
		"CMP EAX, -100 \n"
		"JGE eclf_int_as_int_not_global \n"
		"MOV ECX, [ESP + " VM_VAR "] \n"
		"MOV [ESP], EAX \n"
		"CALL DWORD PTR [ESP + " GET_INT_VAR "] \n"
		"SUB ESP, 4 \n"
		"MOV ECX, EAX \n"
		"JMP eclf_int_reload_c \n"
		//"MOVZX EDX, BYTE PTR [ESI] \n"
		//"JMP eclf_int_arg_done \n"

	"eclf_arg_loop_float: \n"
		"MOVZX EAX, BYTE PTR [EDI] \n"
		"MOVSS XMM0, [EDI + 4] \n"
		"MOV ECX, [ESP + " PARAM_MASK "] \n"
		"BT ECX, EBP \n"
		"JNC eclf_float_arg_not_var \n"
		"CMP AL, 'f' \n"
		"JNE eclf_int_as_float \n"
		"CVTTSS2SI EAX, XMM0 \n"
		"CMP EAX, -100 \n"
		"JGE eclf_float_as_float_not_global \n"
		"MOV ECX, [ESP + " VM_VAR "] \n"
		"MOV [ESP], EAX \n"
		"CALL DWORD PTR [ESP + " GET_FLOAT_VAR "] \n"
		//"FSTP DWORD PTR [ESP + " FCVT_VAR " - 4] \n"
		//"MOVSS XMM0, [ESP + " FCVT_VAR " - 4] \n"
		"SUB ESP, 4 \n"
		"FSTP DWORD PTR [ESP + " FCVT_VAR "] \n"
		"MOVSS XMM0, [ESP + " FCVT_VAR "] \n"
		"JMP eclf_float_reload_c \n"
		//"MOVZX EDX, BYTE PTR [ESI] \n"
		//"JMP eclf_float_arg_done \n"

	"eclf_float_as_int: \n"
		"CVTTSS2SI EAX, [EDI + 4] \n"
		"CMP EAX, -100 \n"
		"JL eclf_float_as_int_global \n"
	"eclf_int_as_int_not_global: \n"
		"TEST EAX, EAX \n"
		"JS eclf_int_var_stack \n"
		"MOV ECX, [ESP + " STACK_BASE "] \n"
		"MOV ECX, [EAX*1 + ECX] \n"
		"JMP eclf_int_arg_done \n"

	//"eclf_float_arg_not_var: \n"
		//"CMP AL, 'f' \n"
		//"JE eclf_float_arg_done \n"
		//"JMP eclf_float_cast_from_int \n"

	"eclf_int_var_stack: \n"
		"MOV ECX, [ESP + " STACK_POINTER "] \n"
		"LEA EAX, [EAX*8 + ECX] \n"
		"CVTTSS2SI ECX, [EAX + 4] \n"
		"CMP BYTE PTR [EAX], 'f' \n"
		"CMOVNE ECX, [EAX + 4] \n"
		"JMP eclf_int_arg_done \n"

	"eclf_int_as_float: \n"
		"MOVD EAX, XMM0 \n"
		"CMP EAX, -100 \n"
		"JL eclf_int_as_float_global \n"
	"eclf_float_as_float_not_global: \n"
		"TEST EAX, EAX \n"
		"JS eclf_float_var_stack \n"
		"MOV ECX, [ESP + " STACK_BASE "] \n"
		"MOVSS XMM0, [EAX*1 + ECX] \n"
		"JMP eclf_float_arg_done \n"

	"eclf_int_arg_not_var: \n"
		"CVTTSS2SI ECX, [EDI + 4] \n"
		"CMP AL, 'f' \n"
		"CMOVNE ECX, [EDI + 4] \n"
		"JMP eclf_int_arg_done \n"

	"eclf_float_as_int_global:"
		"MOV ECX, [ESP + " VM_VAR "] \n"
		"MOV [ESP], EAX \n"
		"CALL DWORD PTR [ESP + " GET_FLOAT_VAR "] \n"
		//"FSTP DWORD PTR [ESP + " FCVT_VAR "- 4] \n"
		//"CVTTSS2SI ECX, [ESP + " FCVT_VAR "- 4] \n"
		"SUB ESP, 4 \n"
		"FSTP DWORD PTR [ESP + " FCVT_VAR "] \n"
		"CVTTSS2SI ECX, [ESP + " FCVT_VAR "] \n"
	"eclf_int_reload_c: \n"
		//"SUB ESP, 4 \n"
		"MOVZX EDX, BYTE PTR [ESI] \n"
		//"JMP eclf_int_arg_done \n"
	"eclf_int_arg_done: \n"
		"MOV [EBP*4 + ESP + " VALUES_ARRAY " - 4], ECX \n"
		"MOV EAX, 5 \n"
		//"MOVZX EDX, BYTE PTR [ESI] \n"
		"CMP DL, 'B' \n"
		"JE eclf_int_arg_is_bool \n"
		"MOVD XMM0, ECX \n"
		"JMP eclf_int_arg_length \n"

	"eclf_float_var_stack: \n"
		"MOV ECX, [ESP + " STACK_POINTER "] \n"
		"MOVSS XMM0, [EAX*8 + ECX + 4] \n"
		//"CMP BYTE PTR [EAX*8 + ECX], 'f' \n"
		"MOVZX EAX, BYTE PTR [EAX*8 + ECX] \n"
	"eclf_float_arg_not_var: \n"
		"CMP AL, 'f' \n"
		"JE eclf_float_arg_done \n"
		//"JMP eclf_float_cast_from_int \n"
		"CVTDQ2PS XMM0, XMM0 \n"
		"JMP eclf_float_arg_done \n"

	"eclf_arg_loop_invalid: \n"
		"INC EBX \n"
	"eclf_arg_literal_percent: \n"
		"INC EBX \n"
		"JMP eclf_arg_loop_next \n"

	"eclf_int_as_float_global: \n"
		"MOV ECX, [ESP + " VM_VAR "] \n"
		"MOV [ESP], EAX \n"
		"CALL DWORD PTR [ESP + " GET_INT_VAR "] \n"
		"SUB ESP, 4 \n"
		"XORPS XMM0, XMM0 \n"
		"CVTSI2SS XMM0, EAX \n"
	"eclf_float_reload_c: \n"
		//"SUB ESP, 4 \n"
		"MOVZX EDX, BYTE PTR [ESI] \n"
		//"JMP eclf_float_cast_from_int \n"

	//"eclf_float_cast_from_int: \n"
		//"CVTDQ2PS XMM0, XMM0 \n"
	"eclf_float_arg_done: \n"
		"MOVSS [EBP*4 + ESP + " VALUES_ARRAY " - 4], XMM0 \n"
		"CVTSS2SD XMM0, XMM0 \n"
		//"MOVZX EDX, BYTE PTR [ESI] \n"
	"eclf_int_arg_length: \n"
		"LEA EAX, [EDX*8 + eclf_format_strings] \n"
		"MOVLPS [ESP + 0xC], XMM0 \n"
		"MOV [ESP + 8], EAX \n"
		"MOVHPS [ESP], XMM0 \n"
		//"XORPS XMM1, XMM1 \n"
		//"MOVLPS [ESP], XMM1 \n"
		//"MOV DWORD PTR [ESP + 4], 0 \n"
		//"MOV DWORD PTR [ESP], 0 \n"
		"CALL _snprintf \n"
	"eclf_int_arg_is_bool: \n"
		"ADD EBX, EAX \n"
		"INC EBP \n"
		"ADD EDI, 8 \n"
	"eclf_arg_loop_next: \n"
		"INC ESI \n"

	".align 16 \n"
		//"NOP \n"

	"eclf_arg_loop: \n"
		"MOVZX EAX, BYTE PTR [ESI] \n"
		"INC ESI \n"
		"CMP AL, '%' \n"
		"JE eclf_arg_loop_read \n"
		"INC EBX \n"
		"TEST AL, AL \n"
		//"JNZ eclf_arg_loop \n"
		".byte 0x75, 0xEF \n"
	"eclf_arg_loop_break: \n"
		"MOV [ESP], EBX \n"
		"CALL _malloc \n"
		"MOV ESI, [ESP + " FORMAT_BACKUP "] \n"
		"LEA EDI, [ESP + " VALUES_ARRAY "] \n"
		"MOV EBP, EAX \n"
		"MOV EBX, EAX \n"
		//"JMP eclf_print_loop \n"
		".byte 0xEB, 0x49 \n"

	".align 8, 0xCC \n"

	"eclf_print_loop_end: \n"
		"MOV BYTE PTR [EBX], 0 \n"
		//"JMP eclf_final_print \n"
		".byte 0xEB, 0x53 \n"

	".align 16, 0xCC \n"

	"eclf_print_loop_read: \n"
		//"MOVZX EAX, BYTE PTR [ESI] \n"
		//"CMP EAX, 0x75 \n"
		//"JA eclf_print_loop_invalid \n"
		//"JMP DWORD PTR [EAX*4 + eclf_print_jump_table] \n"
		"MOVZX EDX, BYTE PTR [ESI] \n"
		"MOVZX EAX, BYTE PTR [EDX + eclf_case_table] \n"
		"JMP DWORD PTR [EAX*4 + eclf_print_jump_table] \n"


	"eclf_print_loop_int: \n"
		"MOVSS XMM0, [EDI] \n"
		"JMP eclf_print_val \n"

	"eclf_print_loop_float: \n"
		"XORPS XMM0, XMM0 \n"
		"CVTSS2SD XMM0, [EDI] \n"
		//"MOVSS XMM0, [EDI] \n"
		//"CVTSS2SD XMM0, XMM0 \n"
	"eclf_print_val: \n"
		"LEA EAX, [EDX*8 + eclf_format_strings] \n"
		"MOVLPS [ESP + 8], XMM0 \n"
		"MOV [ESP + 4], EAX \n"
		"MOV [ESP], EBX \n"
		"CALL _sprintf \n"
		"ADD EBX, EAX \n"
	"eclf_print_loop_arg_done: \n"
		"ADD EDI, 4 \n"
	"eclf_print_loop_next: \n"
		"INC ESI \n"

	".align 16 \n"

	"eclf_print_loop: \n"
		"MOVZX EAX, BYTE PTR [ESI] \n"
		"INC ESI \n"
		"CMP AL, '%' \n"
		"JE eclf_print_loop_read \n"
		"MOV [EBX], AL \n"
		"INC EBX \n"
		"TEST AL, AL \n"
		"JNZ eclf_print_loop \n"
		"DEC EBX \n"
	"eclf_final_print: \n"
		"MOV EAX, EBP \n"
		"SUB EBX, EBP \n"
		"MOV EDX, EBX \n"
		"ADD ESP, 0x354 \n"
		"POP ESI \n"
		"POP EDI \n"
		"POP EBX \n"
		"POP EBP \n"
		"RET \n"
	".align 4, 0xCC \n"

	"eclf_print_loop_invalid: \n"
		"MOV BYTE PTR [EBX], '%' \n"
		"INC EBX \n"
	"eclf_print_loop_literal_percent: \n"
		"MOV BYTE PTR [EBX], AL \n"
		"INC EBX \n"
		".byte 0xEB, 0xCE \n"
		//"JMP eclf_print_loop_next \n"

	".align 16, 0xCC \n"

	"eclf_print_loop_bool: \n"
		"CMP DWORD PTR [EDI], 0 \n"
		"JE eclf_print_loop_false \n"
		"MOV DWORD PTR [EBX], 0x65757274 \n"
		"ADD EBX, 4 \n"
		"JMP eclf_print_loop_arg_done \n"

	"eclf_print_loop_false: \n"
		"MOV DWORD PTR [EBX], 0x736C6166 \n"
		"MOV BYTE PTR [EBX + 4], 0x65 \n"
		"ADD EBX, 5 \n"
		"JMP eclf_print_loop_arg_done \n"

	

	".align 16, 0xCC \n"
	"eclf_case_table: \n"
		".byte 0x00 \n" // '0'
	".rept 0x25 - 0x00 - 1 \n .byte 0x05 \n .endr \n"
		".byte 0x01 \n" // '%'
	".rept ('A' - '%') - 1 \n .byte 0x05 \n .endr \n"
		".byte 0x04 \n" // 'A'
		".byte 0x02 \n" // 'B'
		".byte 0x03 \n" // 'C'
		".byte 0x03 \n" // 'D'
		".byte 0x04 \n" // 'E'
		".byte 0x04 \n" // 'F'
		".byte 0x04 \n" // 'G'
	".byte 0x05 \n"
		".byte 0x03 \n" // 'I'
	".byte 0x05 \n"
	".byte 0x05 \n"
	".byte 0x05 \n"
	".byte 0x05 \n"
		".byte 0x03 \n" // 'N'
	".byte 0x05 \n"
		".byte 0x03 \n" // 'P'
	".byte 0x05 \n"
	".byte 0x05 \n"
		".byte 0x03 \n" // 'S'
	".byte 0x05 \n"
		".byte 0x03 \n" // 'U'
	".byte 0x05 \n"
	".byte 0x05 \n"
		".byte 0x03 \n" // 'X'
	".rept ('a' - 'X') - 1 \n .byte 0x05 \n .endr \n"
		".byte 0x04 \n" // 'a'
		".byte 0x03 \n" // 'b'
		".byte 0x03 \n" // 'c'
		".byte 0x03 \n" // 'd'
		".byte 0x04 \n" // 'e'
		".byte 0x04 \n" // 'f'
		".byte 0x04 \n" // 'g'
	".byte 0x05 \n"
		".byte 0x03 \n" // 'i'
	".byte 0x05 \n"
	".byte 0x05 \n"
	".byte 0x05 \n"
	".byte 0x05 \n"
		".byte 0x03 \n" // 'n'
	".byte 0x05 \n"
		".byte 0x03 \n" // 'p'
	".byte 0x05 \n"
	".byte 0x05 \n"
		".byte 0x03 \n" // 's'
	".byte 0x05 \n"
		".byte 0x03 \n" // 'u'
	".byte 0x05 \n"
	".byte 0x05 \n"
		".byte 0x03 \n" // 'x'
	".rept (256 - 'x') - 1 \n .byte 0x05 \n .endr \n"


			
	".align 16, 0xCC \n"
	"eclf_arg_jump_table: \n"
		".int eclf_arg_loop_break \n"
		".int eclf_arg_literal_percent \n"
		".int eclf_arg_loop_int \n"
		".int eclf_arg_loop_int \n"
		".int eclf_arg_loop_float \n"
		".int eclf_arg_loop_invalid \n"

	".align 16, 0xCC \n"
	"eclf_print_jump_table: \n"
		".int eclf_print_loop_end \n"
		".int eclf_print_loop_literal_percent \n"
		".int eclf_print_loop_bool \n"
		".int eclf_print_loop_int \n"
		".int eclf_print_loop_float \n"
		".int eclf_print_loop_invalid \n"
		

	".align 16, 0xCC \n"
	"eclf_format_strings: \n"
	//".rept ('A' - 32) + 1 \n .fill 8 \n .endr \n"
	".rept 'A' \n .fill 8 \n .endr \n"
		".string \"%A\" \n .align 8, 0 \n" // 'A'
	".fill 8 \n"
		".string \"#%08X\" \n .align 8, 0 \n" // 'C'
		".string \"%i\" \n .align 8, 0 \n" // 'D'
		".string \"%E\" \n .align 8, 0 \n" // 'E'
		".string \"%F\" \n .align 8, 0 \n" // 'F'
		".string \"%G\" \n .align 8, 0 \n" // 'G'
	".fill 8 \n"
		".string \"%i\" \n .align 8, 0 \n" // 'I'
	".fill 8 \n"
	".fill 8 \n"
	".fill 8 \n"
	".fill 8 \n"
		".string \"script%u\" \n .align 8, 0 \n" // 'N'
	//".fill 8 \n"
		".string \"%08X\" \n .align 8, 0 \n" // 'P'
	".fill 8 \n"
	".fill 8 \n"
		".string \"%i\" \n .align 8, 0 \n" // 'S'
	".fill 8 \n"
		".string \"%u\" \n .align 8, 0 \n" // 'U'
	".fill 8 \n"
	".fill 8 \n"
		".string \"%X\" \n .align 8, 0 \n" // 'X'
	".rept ('a' - 'X') - 1 \n .fill 8 \n .endr \n"
		".string \"%a\" \n .align 8, 0 \n" // 'a'
		".string \"%hhu\" \n .align 8, 0 \n" // 'b'
		".string \"%hhi\" \n .align 8, 0 \n" // 'c'
		".string \"%i\" \n .align 8, 0 \n" // 'd'
		".string \"%e\" \n .align 8, 0 \n" // 'e'
		".string \"%f\" \n .align 8, 0 \n" // 'f'
		".string \"%g\" \n .align 8, 0 \n" // 'g'
	".fill 8 \n"
		".string \"%i\" \n .align 8, 0 \n" // 'i'
	".fill 8 \n"
	".fill 8 \n"
	".fill 8 \n"
	".fill 8 \n"
		".string \"sprite%u\" \n .align 8, 0 \n" // 'n'
	//".fill 8 \n"
		".string \"%08x\" \n .align 8, 0 \n" // 'p'
	".fill 8 \n"
	".fill 8 \n"
		".string \"%hi\" \n .align 8, 0 \n" // 's'
	".fill 8 \n"
		".string \"%hu\" \n .align 8, 0 \n" // 'u'
	".fill 8 \n"
	".fill 8 \n"
		".string \"%x\"\n .align 8, 0 \n" // 'x'
	".rept (256 - 'x') - 1 \n .fill 8 \n .endr \n"
	);
#undef VM_VAR
#undef GET_INT_VAR
#undef GET_FLOAT_VAR
#undef STACK_POINTER
#undef STACK_BASE
#undef PARAM_MASK
#undef FORMAT_BACKUP
#undef FCVT_VAR
#undef VALUES_ARRAY
}

inline void print(EclContext* ecl_context, const EclInstruction* current_instruction) {
	uint64_t raw;
	__asm__ volatile (
		"CALL _ecl_format"
		: "=A"(raw)
		: "a"(0), "d"(current_instruction), "c"(ecl_context)
	);
	
	char* str;
	size_t length;
	unpack_dreg(raw, str, length);
	log_print(str);
	free(str);
}

}
#endif