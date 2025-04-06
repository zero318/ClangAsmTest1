#pragma clang diagnostic ignored "-Wc++11-narrowing"

#include "../ClangAsmTest1.h"

#include <stdio.h>

#include "../zero/util.h"
#include "../util.h"

#include <math.h>

#include <stdint.h>

#include "../x87_math_helpers.h"
#include "../RNG_helpers.h"

//#include "../zero/LinkerCheeseValues.h"
#include "../zero/FloatConstants.h"

#include "../zero/BoundingBox.h"

#include "../zero/custom_intrin.h"

template <typename T>
using ZUNList = ZUNListBase<T, true>;

#define SetInstr(value) \
current_instruction = (decltype(current_instruction))(value)

#define IndexInstr(offset) \
SetInstr((intptr_t)current_instruction + (offset))

#define ByteArg(number) \
(((uint8_t*)current_instruction->args)[(number)])

#define OneBitArg(number) \
(ByteArg(number) & 0b1)
#define ThreeBitArg(number) \
(ByteArg(number) & 0b111)

#define ShortArg(number) \
(((int16_t*)current_instruction->args)[(number)])

#define RawArg(number) \
(((EclArg*)current_instruction->args)[(number)])

#define IntArg(number) \
(((int32_t*)current_instruction->args)[(number)])

#define UIntArg(number) \
(((uint32_t*)current_instruction->args)[(number)])

#define FloatArg(number) \
(((float*)current_instruction->args)[(number)])

#define Float2Arg(number) \
(((Float2*)current_instruction->args)[(number)])

#define Float3Arg(number) \
(((Float3*)current_instruction->args)[(number)])

#define StringArg(offset) \
((const char*)(current_instruction->args + (offset)))

#define TypeArg(type, number) \
(((type*)current_instruction->args)[(number)])


union StackArg {
	int32_t integer;
	float real;
	volatile int32_t vinteger;
	volatile float vreal;
};

struct EclInstruction {
	uint32_t time; // 0x0
	uint16_t opcode; // 0x4
	uint16_t offset_to_next; // 0x6
	uint16_t param_mask; // 0x8
	uint8_t difficulty_mask; // 0xA
	uint8_t param_count; // 0xB
	uint8_t stack_refs; // 0xC
	probably_padding_bytes(0x3); // 0xD
	unsigned char args[]; // 0x10
};

#define EclStackCount 0x400
#define EclStackSize sizeof(EclStackItem[EclStackCount])

union EclStackItem {
	int32_t integer;
	float real;
	char character;
};

struct EclTypedStackItem {
	char type;
	union {
		int32_t integer;
		float real;
	};
};

struct EclSubHeader {
	int32_t magic;
	uint32_t data_offset;
	unknown_fields(0x8);
	unsigned char instructions[]; // Actually EclInstr[]
};
struct EclSubroutinePtr {
	char* name;
	EclSubHeader* sub_header;
};
struct EclFileManager {
	void* vtable;
	int32_t file_count;
	int32_t subroutine_count;
	void* file_data[0x20];
	EclSubroutinePtr(*subroutines)[];
};

static volatile int32_t yeetus;
struct EnemyFull {
	unsigned char dummy[0x1214];
	EclFileManager* file_manager;

	virtual void funcA() {};
	virtual int32_t get_int_var(int32_t index) {
		return yeetus + index;
	};
	virtual int32_t* get_int_ptr(int32_t index) {
		return (int32_t*)&yeetus + index;
	};
	virtual void get_float_var(int32_t index) {
		//return yeetus + index;
	};
	virtual float get_float_var2(int32_t index) {
		return yeetus + index;
	};
	virtual float* get_float_ptr(int32_t index) {
		return (float*)&yeetus + index;
	};
	virtual void funcB() {};
};

typedef struct EclContext EclContext;
struct EclContext {
	float time; // 0x0
	int32_t current_sub_index; // 0x4
	int32_t current_instr_offset; // 0x8
	union {
		EclStackItem stack[EclStackCount];
		unsigned char stack_raw[EclStackSize];
	};
	int32_t stack_top_offset; // Like ESP
	int32_t stack_locals_offset; // Like EBP
	int32_t async_id;
	union { // WTF even is this field, I can't tell.
		EnemyFull* enemy_full;
		ZUNList<EclContext>* parent;
	};
	unknown_fields(0x4);
	uint8_t current_difficulty_mask;
	probably_padding_bytes(0x3);
	//InterpFloat float_interps[8];
	//EclLocation float_interp_locations[8];
	unknown_fields(0x4);
	unknown_fields(0x20);

	template <typename T>
	forceinline T read_local(int32_t offset) {
		return *(T*)(this->stack_raw + this->stack_locals_offset + offset);
	}

	forceinline EclTypedStackItem& read_temp(int32_t index) {
		return ((EclTypedStackItem*)(this->stack_raw + this->stack_top_offset))[index];
	}

	template <typename T>
	forceinline void stack_push(T value) {
		*(T*)(this->stack_raw + this->stack_top_offset) = value;
		this->stack_top_offset += 4;
	}

	template <typename T>
	forceinline void stack_push_type(T value) {
		if constexpr (std::is_same_v<T, float>) {
			this->stack_push<char>('f');
		} else {
			this->stack_push<char>('i');
		}
		this->stack_push<T>(value);
	}

	template <typename T>
	forceinline T stack_pop() {
		this->stack_top_offset -= 4;
		return *(T*)(this->stack_raw + this->stack_top_offset);
	}

	template <typename T>
	forceinline T stack_pop_cast() {
		int32_t temp = this->stack_pop<int32_t>();
		switch (this->stack_pop<char>()) {
			case 'f':
				//return reinterpret_cast<float>(temp);
				return *(float*)&temp;
			case 'i':
				return temp;
			default:
				//return reinterpret_cast<T>(temp);
				return *(T*)&temp;
		}
	}

	forceinline int32_t stack_pop_int() {
		//int32_t temp = this->stack_pop<int32_t>();
		//if (this->stack_pop<char>() == 'f') {
		//	temp = reinterpret_cast<float>(temp);
		//	//temp = reinterpret_cast<float>(temp);
		//}
		EclStackItem stack_val = this->stack_pop<EclStackItem>();
		if (this->stack_pop<char>() == 'f') {
			stack_val.integer = stack_val.real;
		}
		return stack_val.integer;
	}

	forceinline int32_t stackLeave(void) {
		int32_t old_locals_offset = this->stack_locals_offset;
		this->stack_locals_offset = this->stack_pop<int>();
		this->stack_top_offset = old_locals_offset;
		return 0;
	}

	EclInstruction* get_sub_instrs();

	noinline int32_t LowECL_Run(float current_gamespeed);

	dllexport int32_t thiscall get_int_arg(int32_t arg_index) {
		EclInstruction* current_instruction = this->get_sub_instrs();
		if (current_instruction->param_mask & (1 << arg_index)) {
			int32_t value = IntArg(arg_index);
			if (value >= 0) {
				return this->read_local<int32_t>(value);
			} else if (value >= -100) {
				arg_index = this->read_temp(value).integer;
				if (this->read_temp(value).type == 'f') {
					arg_index = *(float*)&arg_index;
				}
				return arg_index;
			} else {
				return this->enemy_full->get_int_var(value);
			}
		} else {
			return IntArg(arg_index);
		}
	}

	/*noinline int32_t get_int_arg2(int32_t arg_index) {
		EclInstruction *restrict current_instruction = this->get_sub_instrs();
		int32_t value = IntArg(arg_index);
		if (current_instruction->param_mask & (1 << arg_index)) {
			if (value >= 0) {
				return this->read_local<int32_t>(value);
			}
			else if (value >= -100) {
				arg_index = this->read_temp(value).integer;
				if (this->read_temp(value).type == 'f') {
					arg_index = *(float*)&arg_index;
				}
				return arg_index;
			}
			else {
				value = this->enemy_full->get_int_var(value);
			}
		}
		return value;
	}*/

	noinline int32_t* get_int_arg_ptr(int32_t sub_index) {
		//assume_all_registers_volatile(this);
		return (int32_t*)(intptr_t)sub_index + 1;
	}

	dllexport float vectorcall get_float_arg(int32_t pingas, int32_t arg_index) {
		EclInstruction* current_instruction = this->get_sub_instrs();
		float value = FloatArg(arg_index);
		if (current_instruction->param_mask & (1 << arg_index)) {
			if (value >= 0.0f) {
				return this->read_local<float>(value);
			} else if (-1.0f >= value && value >= -100.0f) {
				EclTypedStackItem* stack_ptr = &this->read_temp(value);
				float temp = stack_ptr->real;
				return stack_ptr->type == 'i'
						? *(int32_t*)&temp
						: temp;
			} else {
				return this->enemy_full->get_float_var2(value);
			}
		} else {
			return value;
		}
	}

	dllexport float vectorcall get_float_arg2(int32_t pingas, int32_t arg_index) {
		EclInstruction* current_instruction = this->get_sub_instrs();
		float value = FloatArg(arg_index);
		if (current_instruction->param_mask & (1 << arg_index)) {
			int32_t temp2 = value;
			int32_t temp;
			__asm__(
				"VTESTPS %[value], %[value]"
				: asm_flags(z, temp)
				: asm_arg("Yz", value)
			);
			if (temp) {
				return this->read_local<float>(temp2);
			} else if (value >= -100.0f) {
				EclTypedStackItem* stack_ptr = &this->read_temp(temp2);
				vec<float, 4> temp3 = { stack_ptr->real, 0.0f, 0.0f, 0.0f };
				if (stack_ptr->type == 'i') {
					temp3 = convertvec(bitcast(vec<int32_t, 4>, temp3), vec<float, 4>);
				}
				return temp3[0];
			} else {
				__asm__(
					"FSTPS %[arg_index] \n"
					: asm_arg("=m", arg_index)
					: "X"(this->enemy_full->get_float_var(value), NULL)
				);
				return bit_cast(float, arg_index);
			}
		} else {
			return value;
		}
	}

	noinline float* get_float_arg_ptr(int32_t arg_index) {
		//assume_all_registers_volatile(this);
		return (float*)(intptr_t)arg_index + 1;
	}

	noinline int32_t ecl_call(int32_t sub_index, int, EclContext* parent) {
		//assume_all_registers_volatile(this);
		return sub_index;
	}
};

EclInstruction* EclContext::get_sub_instrs() {
	int32_t instr_offset = this->current_instr_offset;
	if (instr_offset != -1 && this->current_sub_index != -1) {
		return (EclInstruction*)&(*(this->enemy_full->file_manager->subroutines))[this->current_sub_index].sub_header->instructions[instr_offset];
	} else {
		return NULL;
	}
}

//static 

dllexport int32_t context_test() {
	EclContext context;
	context.current_sub_index = -1;
	context.current_instr_offset = -1;
	context.stack_locals_offset = 0;
	context.stack_top_offset = 0;
	context.get_int_arg(0);
	context.get_float_arg(garbage_arg<int32_t>(), 2);
	return context.get_int_arg(1);
}