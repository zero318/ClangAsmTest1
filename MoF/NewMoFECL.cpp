#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <type_traits>

#include <bit>

#include "../zero/util.h"

template <typename T>
using ZUNList = ZUNListBase<T, true>;

#include <math.h>

#include <stdint.h>

#include "../zero/custom_intrin.h"

typedef uint32_t DWORD;

typedef uint32_t D3DCOLOR;

gnu_noinline float stdcall reduce_angle(float angle) {
	return angle;
}

struct Float2 {
	float x;
	float y;

	gnu_noinline void stdcall make_from_vector(float angle, float magnitude) {
		this->x = angle;
		this->y = magnitude;
	}
};

struct Float3 {
	float x;
	float y;
	float z;
};

typedef int32_t AnmID;

// size 
struct HintData {
	Float2 position; // 0x0
	
	char text[0]; // 0x1C
	
	int32_t count; // 0x60
	int32_t alignment_index; // 0x64
	int32_t type_index; // 0x68
	int32_t time; // 0x6C
	int32_t __remain; // 0x70
	
	float scale; // 0x7C
	
	D3DCOLOR color; // 0x84
};

// size 0x6A
struct Input {
	short __word_0; // 0x0
	short __word_2; // 0x2
	uint16_t __uint_4; // 0x4
	short __word_6; // 0x6
	short __word_8; // 0x8
	uint16_t __uints_A[16]; // 0xA
	uint16_t __uint_2A; // 0x2A
	uint16_t __uint_2C; // 0x2C
	short __word_2E; // 0x2E
	short __word_30; // 0x30
	short __word_32; // 0x32
	short __word_34; // 0x34
	
	uint16_t __uints_38[16]; // 0x38
	int16_t __int_58; // 0x58
	int16_t __int_5A; // 0x5A
	int16_t __int_5C; // 0x5C
	int16_t __int_5E; // 0x5E
	int __dword_60; // 0x60
	int __dword_64; // 0x64
	short __word_68; // 0x68
};

extern "C" {
    // 0x474E30
    extern Input INPUT;
}

#define SetInstr(value) \
current_instruction = (std::remove_reference_t<decltype(current_instruction)>)(value)

#define IndexInstr(offset) \
SetInstr((intptr_t)current_instruction + (offset))

#define ByteArg(number) \
(((uint8_t*)current_instruction->args)[(number)])

#define OneBitArg(number) \
(ByteArg(number) & 0b1)
#define ThreeBitArg(number) \
(ByteArg(number) & 0b111)

#define CharArg(offset) \
(((char*)current_instruction->args)[(offset)])

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

#define FormatArg(number) \
((char)IntArg(number))

#define TypeArg(type, number) \
(((type*)current_instruction->args)[(number)])

typedef struct EnemyFull EnemyFull;

struct EclSubHeader {
	int32_t magic;
	uint32_t data_offset;
	//unknown_fields(0x8);
	unsigned char dummy[8];
	unsigned char instructions[]; // Actually EclInstr[]
};
struct EclSubroutinePtr {
	char* name;
	EclSubHeader* sub_header;
};
typedef struct EclInstruction EclInstruction;

// size: 0x24
struct EclFileHeader {
	ZUNMagic magic; // 0x0
	uint16_t version; // 0x4
	uint16_t include_length; // 0x6 /* include_offset + ANIM+ECLI length */
	uint32_t include_offset; // 0x8
	int __dword_C; // 0xC
	uint16_t sub_count; // 0x10
	
	int __dword_14; // 0x14
	int __dword_18; // 0x18
	int __dword_1C; // 0x1C
	int __dword_20; // 0x20
};

struct ECLIncludeData {
	ZUNMagic magic; // 0x0
	uint32_t count; // 0x4
	char names[]; // 0x8
};

struct EclFile {
	EclFileHeader header;
};

typedef uint32_t EclFileOffset;

// size: 0x1098
struct EnemyController {
	//void* vtable; // 0x0
	int32_t file_count; // 0x4
	int32_t subroutine_count; // 0x8
	EclFile* ecl_files[0x20]; // 0xC
	EclSubroutinePtr(*subroutines)[]; // 0x8C
	
	int __dword_1090; // 0x1090
	int __dword_1094; // 0x1094
	
	virtual int32_t method_0(EclFile* file_buffer) {
		this->ecl_files[this->file_count] = file_buffer;
		EclFile* file_data = this->ecl_files[this->file_count];
		EclFile** file_buffer_ptr = &this->ecl_files[this->file_count];
		if (file_data->header.magic.as_int != PackInt('S','C','P','T') || file_data->header.version != 1) {
			*file_buffer_ptr = NULL;
			return -1;
		}
		file_data = *file_buffer_ptr;
		uint32_t include_length = file_data->header.include_length;
		uint32_t total_sub_count = this->subroutine_count;
		EclSubroutinePtr* prev_sub_array = *this->subroutines;
		EclFileOffset* current_sub_header_offset = (EclFileOffset*)((uintptr_t)file_data + include_length + sizeof(EclFileHeader));
		uint32_t new_sub_count = file_data->header.sub_count;
		total_sub_count += new_sub_count;
		char* current_sub_name = (char*)(current_sub_header_offset + new_sub_count);
		this->subroutine_count = new_sub_count;
		EclSubroutinePtr(*new_sub_array)[] = (EclSubroutinePtr(*)[])malloc(sizeof(EclSubroutinePtr[new_sub_count]));
		this->subroutines = new_sub_array;
		if (prev_sub_array) {
			for (
				int32_t i = 0;
				i < this->subroutine_count;
				++i, ++current_sub_header_offset, current_sub_name += strlen(current_sub_name) + 1
			) {
				(*this->subroutines)[i].sub_header = (EclSubHeader*)((uintptr_t)this->ecl_files[this->file_count] + *current_sub_header_offset);
				(*this->subroutines)[i].name = current_sub_name;
			}
		} else {
			int32_t new_sub_count = this->subroutine_count - this->ecl_files[this->file_count]->header.sub_count;
			memcpy(new_sub_array, prev_sub_array, sizeof(EclSubroutinePtr[new_sub_count]));
			if (prev_sub_array) {
				free(prev_sub_array);
			}
			for (
				int32_t i = 0;
				i < this->ecl_files[this->file_count]->header.sub_count;
				++i, ++new_sub_count, ++current_sub_header_offset, current_sub_name += strlen(current_sub_name) + 1
			) {
				int32_t j = 0;
				if (new_sub_count > 0) {
					for (
						EclSubroutinePtr* sub_array = *this->subroutines;
						j < new_sub_count;
						++j, ++sub_array
					) {
						if (strcmp(current_sub_name, sub_array->name) <= 0) {
							break;
						};
					}
				}
				for (
					int32_t k = this->subroutine_count - 1;
					k > j;
					--k
				) {
					EclSubroutinePtr(*sub_array)[] = this->subroutines;
					EclSubroutinePtr* prev_func = &(*sub_array)[k - 1];
					EclSubroutinePtr* cur_func = &(*sub_array)[k];
					cur_func->name = prev_func->name;
					cur_func->sub_header = prev_func->sub_header;
				}
				(*this->subroutines)[j].sub_header = (EclSubHeader*)((uintptr_t)this->ecl_files[this->file_count] + *current_sub_header_offset);
				(*this->subroutines)[j].name = current_sub_name;
			}
		}
		int32_t file_index = this->file_count++;
		EclFile* cur_file = this->ecl_files[file_index];
		if (cur_file->header.include_length) {
			this->method_4((ECLIncludeData*)((uintptr_t)cur_file + sizeof(EclFileHeader)));
		}
		return file_index;
	}
	
	virtual int32_t method_4(ECLIncludeData* include_data) {
		if (include_data->magic.as_int != PackInt('A','N','I','M')) {
			return 0;
		}
		char* current_include_name = include_data->names;
		for (
			uint32_t i = 0;
			i < include_data->count;
			++i, current_include_name += strlen(current_include_name) + 1
		) {
			//AnmLoaded* anm = ANM_MANAGER_PTR->preload_anm(i + 9, current_include_name);
			//ENEMY_MANAGER_PTR->included_anms[i] = anm;
			//if (!anm) goto CorruptedData;
		}
		// Lol IDK
		
		
	CorruptedData:
		//DEBUG_LOG.normal_log("The data is corrupted\r\n");
		return -1;
	}
};

struct EclInstruction {
	int32_t time; // 0x0
	int16_t opcode; // 0x4
	uint16_t offset_to_next; // 0x6
	uint16_t param_mask; // 0x8
	uint8_t difficulty_mask; // 0xA
	uint8_t param_count; // 0xB
	uint32_t dummy; // 0xC
	unsigned char args[]; // 0x10
};

#define EclStackCount 0x400
#define EclStackSize sizeof(EclStackItem[EclStackCount])

union EclArg {
	int32_t integer;
	float real;
	char character;
};

using EclStackItem = EclArg;

struct EclStackValue {
	char type;
	EclStackItem value;
};

template <typename T>
inline int32_t& get_stack_top_offset(T* thing) {
	if constexpr (std::is_same_v<T, EclContext>) {
		return thing->stack.top_offset;
	} else if constexpr (std::is_same_v<T, EclStack>) {
		return thing->top_offset;
	}
}

template <typename T>
struct EclStackType {
	static inline constexpr char type_char = '\0';
};
#define EclStackTypeChar(type, character) \
template <> \
struct EclStackType<type> { \
	static inline constexpr char type_char = 'c'; \
}

EclStackTypeChar(int8_t, 'c');
EclStackTypeChar(int16_t, 's');
EclStackTypeChar(int32_t, 'i');
EclStackTypeChar(long, 'i');
EclStackTypeChar(int64_t, 'l');
EclStackTypeChar(uint8_t, 'b');
EclStackTypeChar(uint16_t, 'w');
EclStackTypeChar(uint32_t, 'u');
EclStackTypeChar(unsigned long, 'u');
EclStackTypeChar(uint64_t, 'q');
EclStackTypeChar(float, 'f');
EclStackTypeChar(double, 'd');
EclStackTypeChar(long double, 'L');



forceinline int32_t fast_f32i32(float input) {
//#ifndef __SSE2__
	int32_t ret;
	__asm__(
		"FISTTPL %[ret]"
		: asm_arg("=m", ret)
		: asm_arg("t", input)
		: clobber_list("st")
	);
	return ret;
//#else
//	int32_t ret;
//	__asm__(
//		"CVTTSS2SI %[input], %[ret]"
//		: asm_arg("=r", ret)
//		: asm_arg("mx", input)
//	);
//	return ret;
//#endif
}
forceinline int32_t fast_f32i32_new(long double input) {
	return (int32_t)input;
}

struct EclStack {

	union { // 0x0
		EclStackItem stack[EclStackCount];
		unsigned char stack_raw[EclStackSize];
	};
	int32_t top_offset; // 0x1000
	int32_t locals_offset; // 0x1004

	template <typename T>
	forceinline T read_offset(int32_t offset) {
		return *(T*)(this->stack_raw + offset);
	}
	template <typename T>
	forceinline T read_local(int32_t offset, int32_t locals_offset) {
		return this->read_offset<T>(locals_offset + offset);
	}
	template <typename T>
	forceinline T read_local(int32_t offset) {
		return this->read_local<T>(offset, this->locals_offset);
	}
	template <typename T>
	forceinline T read_temp(int32_t offset, int32_t top_offset) {
		return this->read_offset<T>(top_offset - offset);
	}
	template <typename T>
	forceinline T read_temp(int32_t offset) {
		return this->read_temp<T>(offset, this->top_offset);
	}

	template <typename T>
	forceinline void write_offset(int32_t offset, T value) {
		*(T*)(this->stack_raw + offset) = value;
	}
	template <typename T>
	forceinline void write_local(int32_t offset, T value) {
		this->write_offset<T>(this->locals_offset + offset, value);
	}
	template <typename T>
	forceinline void write_temp(int32_t offset, T value) {
		this->write_offset<T>(this->top_offset - offset, value);
	}

	template <typename T>
	forceinline T& ref_offset(int32_t offset) {
		return *(T*)(this->stack_raw + offset);
	}
	template <typename T>
	forceinline T& ref_local(int32_t offset) {
		return this->ref_offset<T>(this->locals_offset + offset);
	}
	template <typename T>
	forceinline T& ref_temp(int32_t offset) {
		return this->ref_offset<T>(this->top_offset - offset);
	}

	template <typename T>
	forceinline T pop(int32_t& starting_offset) {
		starting_offset -= 4;
		return *(T*)(this->stack_raw + starting_offset);
	}
	template <typename T>
	forceinline T pop() {
		return this->pop<T>(this->top_offset);
	}

	template <typename T>
	forceinline T safe_pop_cast(int32_t& starting_offset) {
		T ret;
		int32_t new_top_offset = starting_offset - 4;
		if (expect_chance(new_top_offset >= 0, true, 0.99)) {
			union {
				int32_t integer;
				float real;
			} value = { this->read_offset<int32_t>(new_top_offset) };
			starting_offset = new_top_offset -= 4;
			if constexpr (std::is_same_v<T, int32_t>) {
				if (this->read_offset<char>(new_top_offset) == 'f') {
					value.integer = (long double)value.real;
				}
				ret = value.integer;
			}
			else if constexpr (std::is_same_v<T, float>) {
				if (this->read_offset<char>(new_top_offset) == 'i') {
					value.real = value.integer;
				}
				ret = value.real;
			}
		} else {
			unreachable;
		}
		return ret;
	}
	template <typename T>
	forceinline T safe_pop_cast() {
		return this->safe_pop_cast<T>(this->top_offset);
	}

	template <typename T, bool use_normal_cast = false, typename L>
	void unary_op(const L& func) {
		union TypePun {
			int32_t integer;
			uint32_t uinteger;
			float real;
		};

		const int32_t top_offset = this->top_offset;

		TypePun stack_value_1 = { this->read_offset<int32_t>(top_offset - 4) };
		if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, long>) {
			if (this->read_offset<char>(top_offset - 8) == 'f') {
				//if constexpr (use_normal_cast) {
					stack_value_1.integer = (long double)stack_value_1.real;
				//} else {
					//stack_value_1.integer = fast_f32i32(stack_value_1.real);
				//}
			}
		} else if constexpr (std::is_same_v<T, uint32_t> || std::is_same_v<T, unsigned long>) {
			if (this->read_offset<char>(top_offset - 8) == 'f') {
				stack_value_1.uinteger = (long double)stack_value_1.real;
			}
		} else if constexpr (std::is_same_v<T, float>) {
			if (this->read_offset<char>(top_offset - 8) == 'i') {
				stack_value_1.real = stack_value_1.integer;
			}
		}
		if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, long>) {
			this->ref_offset<char>(top_offset - 8) = 'i';
			this->ref_offset<int32_t>(top_offset - 4) = func(stack_value_1.integer);
		} else if constexpr (std::is_same_v<T, uint32_t> || std::is_same_v<T, unsigned long>) {
			this->ref_offset<char>(top_offset - 8) = 'i';
			this->ref_offset<int32_t>(top_offset - 4) = func(stack_value_1.integer);
		} else if constexpr (std::is_same_v<T, float>) {
			this->ref_offset<char>(top_offset - 8) = 'f';
			this->ref_offset<float>(top_offset - 4) = func(stack_value_1.real);
		}
	}

	template <typename T, bool use_normal_cast = false, typename L>
	void binary_op(const L& func) {
		union TypePun {
			int32_t integer;
			uint32_t uinteger;
			float real;
		};

		const int32_t top_offset = this->top_offset;

		TypePun stack_value_2 = { this->read_offset<int32_t>(top_offset - 4) };
		if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, long>) {
			if (this->read_offset<char>(top_offset - 8) == 'f') {
				//if constexpr (use_normal_cast) {
					stack_value_2.integer = (long double)stack_value_2.real;
				//} else {
					//stack_value_2.integer = fast_f32i32(stack_value_2.real);
				//}
			}
		} else if constexpr (std::is_same_v<T, uint32_t> || std::is_same_v<T, unsigned long>) {
			if (this->read_offset<char>(top_offset - 8) == 'f') {
				stack_value_2.uinteger = (long double)stack_value_2.real;
			}
		} else if constexpr (std::is_same_v<T, float>) {
			if (this->read_offset<char>(top_offset - 8) == 'i') {
				stack_value_2.real = stack_value_2.integer;
			}
		}
		TypePun stack_value_1 = { this->read_offset<int32_t>(top_offset - 12) };
		if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, long>) {
			if (this->read_offset<char>(top_offset - 16) == 'f') {
				//if constexpr (use_normal_cast) {
					stack_value_1.integer = (long double)stack_value_1.real;
				//} else {
					//stack_value_1.integer = fast_f32i32(stack_value_1.real);
				//}
			}
		} else if constexpr (std::is_same_v<T, uint32_t> || std::is_same_v<T, unsigned long>) {
			if (this->read_offset<char>(top_offset - 16) == 'f') {
				stack_value_1.uinteger = (long double)stack_value_1.real;
			}
		} else if constexpr (std::is_same_v<T, float>) {
			if (this->read_offset<char>(top_offset - 16) == 'i') {
				stack_value_1.real = stack_value_1.integer;
			}
		}

		if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, long>) {
			this->ref_offset<char>(top_offset - 16) = 'i';
			this->ref_offset<int32_t>(top_offset - 12) = func(stack_value_1.integer, stack_value_2.integer);
		} else if constexpr (std::is_same_v<T, uint32_t> || std::is_same_v<T, unsigned long>) {
			this->ref_offset<char>(top_offset - 16) = 'i';
			this->ref_offset<int32_t>(top_offset - 12) = func(stack_value_1.integer, stack_value_2.integer);
		} else if constexpr (std::is_same_v<T, float>) {
			this->ref_offset<char>(top_offset - 16) = 'f';
			this->ref_offset<float>(top_offset - 12) = func(stack_value_1.real, stack_value_2.real);
		}
		this->top_offset -= 8;
	}

	template <typename T, typename L>
	void binary_op_vec(const L& func) {

		using TypePun = std::conditional_t<std::is_same_v<T, float>, vec<float, 4>, vec<int32_t, 4>>;

		const int32_t top_offset = this->top_offset;

		TypePun stack_value_2;
		if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, long>) {
			stack_value_2 = { this->read_offset<int32_t>(top_offset - 4), 0, 0, 0 };
			if (this->read_offset<char>(top_offset - 8) == 'f') {
				stack_value_2 = convertvec(*(vec<float, 4>*)&stack_value_2, vec<int32_t, 4>);
			}
		} else if constexpr (std::is_same_v<T, float>) {
			stack_value_2 = { this->read_offset<float>(top_offset - 4), 0.0f, 0.0f, 0.0f };
			if (this->read_offset<char>(top_offset - 8) == 'i') {
				stack_value_2 = convertvec(*(vec<int32_t, 4>*)&stack_value_2, vec<float, 4>);
			}
		}
		TypePun stack_value_1;
		if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, long>) {
			stack_value_1 = { this->read_offset<int32_t>(top_offset - 12), 0, 0, 0 };
			if (this->read_offset<char>(top_offset - 16) == 'f') {
				stack_value_1 = convertvec(*(vec<float, 4>*)&stack_value_1, vec<int32_t, 4>);
			}
		} else if constexpr (std::is_same_v<T, float>) {
			stack_value_1 = { this->read_offset<float>(top_offset - 12), 0.0f, 0.0f, 0.0f };
			if (this->read_offset<char>(top_offset - 16) == 'i') {
				stack_value_1 = convertvec(*(vec<int32_t, 4>*)&stack_value_1, vec<float, 4>);
			}
		}

		if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, long>) {
			this->ref_offset<char>(top_offset - 16) = 'i';
			auto temp = func(stack_value_1, stack_value_2);
			if constexpr (std::is_arithmetic_v<decltype(temp)>) {
				this->ref_offset<int32_t>(top_offset - 12) = temp;
			} else {
				this->ref_offset<int32_t>(top_offset - 12) = temp[0];
			}
		} else if constexpr (std::is_same_v<T, float>) {
			this->ref_offset<char>(top_offset - 16) = 'f';
			auto temp = func(stack_value_1, stack_value_2);
			if constexpr (std::is_arithmetic_v<decltype(temp)>) {
				this->ref_offset<float>(top_offset - 12) = temp;
			} else {
				this->ref_offset<float>(top_offset - 12) = temp[0];
			}
		}
		this->top_offset -= 8;
	}

//#define StackPopSafe(stack, ...) \
	//switch (int32_t new_top_offset = (stack)->top_offset - (MACRO_DEFAULT_ARG(4, __VA_ARGS__)); new_top_offset >= 0 ? ((stack)->top_offset = new_top_offset), true : false) case true:

#pragma clang diagnostic ignored "-Wsometimes-uninitialized"

#define StackPopSafe(stack, ...) \
    if (int32_t new_top_offset = (stack)->top_offset - (MACRO_DEFAULT_ARG(4, __VA_ARGS__)); new_top_offset >= 0 ? ((stack)->top_offset = new_top_offset), true : false) 

#define ReadPoppedValue(stack, type, ...) \
	((stack)->read_offset<type>(MACRO_DEFAULT_ARG(new_top_offset, __VA_ARGS__)))
	//(*(type*)((stack)->stack_raw + (MACRO_DEFAULT_ARG(new_top_offset, __VA_ARGS__))))

	template <typename T, int32_t size = sizeof(T)>
	forceinline void push(T value, int32_t& starting_offset) {
		if (expect_chance(starting_offset + size < EclStackSize, true, 0.99)) {
			this->write_offset<T>(starting_offset, value);
			starting_offset += size;
		}
	}
	template <typename T, int32_t size = sizeof(T)>
	forceinline void push(T value) {
		this->push<T, size>(value, this->top_offset);
	}

	dllexport int fastcall push_value(char type, int32_t size, void *restrict value_ptr, int32_t& starting_offset) {
		if (expect_chance(size + starting_offset < (int32_t)EclStackSize, true, 0.99)) {
			if (expect_chance(type != 0, true, 0.99)) {
				this->push<char, sizeof(int32_t)>(type, starting_offset);
			}
			memcpy(this->stack_raw + starting_offset, value_ptr, size);
			starting_offset += size;
			return 0;
		} else {
			return -1;
		}
	}
	dllexport int fastcall push_value(char type, int32_t size, void *restrict value_ptr) {
		return this->push_value(type, size, value_ptr, this->top_offset);
	}
//#define push_value(type, size, value_ptr, ...) push_value((type), (size), (value_ptr), (MACRO_DEFAULT_ARG(get_stack_top_offset(this), __VA_ARGS__)))

	void pop(int32_t size, void* out_ptr);

	int32_t enter_frame(int32_t size) {
		int32_t prev_top = this->top_offset;
		size += prev_top;
		if (size >= EclStackSize) {
			return -1;
		} else {
			this->top_offset = size;
			this->push<int32_t>(this->locals_offset);
			this->locals_offset = prev_top;
			return 0;
		}
	}

	int32_t leave_frame(void) {
		int32_t old_locals = this->locals_offset;
		StackPopSafe(this) {
			this->locals_offset = ReadPoppedValue(this, int32_t);
		}
		this->top_offset = old_locals;
		return 0;
	}

};

#ifdef _M_IX86
extern void _CIfmod() asm("__CIfmod");
#endif

inline float CIfmod(float arg1, float arg2) {
#ifdef _M_IX86
	__asm__(
		"CALL %P[fmodf] \n"
		: "+t" (arg1)
		: "u" (arg2), [fmodf]"i"(_CIfmod)
		: clobber_list("st(1)", "eax", "ecx", "edx")
	);
#endif
	return arg1;
}

dllexport gnu_noinline void stdcall log_vprintf(const char *format, void* va) {
	use_var(format);
	use_var(va);
}

struct EclContext {
	float time; // 0x0
	EclInstruction* current_instr; // 0x4
	EclStack stack; // 0x8
	// int32_t top_offset; // 0x1008
	// int32_t locals_offset; // 0x100C
	int32_t async_id; // 0x1010
	union { // WTF even is this field, I can't tell.
		EnemyFull* parent_enemy;
		EclContext* parent;
		ZUNList<EclContext>* parent_list;
	};
	int __async_dword_1018; // 0x1018
	union {
		uint32_t current_difficulty_mask_full; // 0x101C
		uint8_t current_difficulty_mask;
	};
	union {
		int __async_flags_1020; // 0x1020
		struct {
			int32_t __async_flag_A : 1;
		};
	};
	EnemyController* controller; // 0x1024

	dllexport gnu_noinline int32_t thiscall LowECL_Run(float current_gamespeed);

	EclInstruction* get_sub_instrs();

	dllexport gnu_noinline int32_t thiscall get_int_arg(int32_t index);

// TD version
	dllexport gnu_noinline int32_t get_int_argTD(int32_t index);

	dllexport gnu_noinline int32_t thiscall parse_int_as_arg(int32_t index, int32_t value);

	dllexport gnu_noinline float fastcall get_float_arg(int32_t index);

	dllexport gnu_noinline float fastcall parse_float_as_arg(int32_t index, float value);

	dllexport gnu_noinline int32_t* fastcall get_int_arg_ptr(int32_t index);

	dllexport gnu_noinline float* fastcall get_float_arg_ptr(int32_t index);

	gnu_noinline int32_t call(EclContext* parent_context, int va_index);

	dllexport gnu_noinline void math_float_mod() {
		this->stack.binary_op<float>([](float value_1, float value_2) {
			return CIfmod(value_1, value_2);
		});
	}
	dllexport gnu_noinline void math_float_neg() {
		this->stack.unary_op<float>([](float value_1) {
			return -value_1;
		});
	}
	dllexport gnu_noinline void gnu_attr(target("no-sse")) math_shl() {
		this->stack.binary_op<int32_t>([](int32_t value_1, int32_t value_2) {
			return value_1 << value_2;
		});
	}
	dllexport gnu_noinline void math_shl2() {
		this->stack.binary_op<int32_t, true>([](int32_t value_1, int32_t value_2) {
			return value_1 << value_2;
		});
	}
	dllexport gnu_noinline void math_sar() {
		this->stack.binary_op<int32_t, true>([](int32_t value_1, int32_t value_2) {
			return value_1 >> value_2;
		});
	}
	dllexport gnu_noinline void math_shr() {
		this->stack.binary_op<int32_t, true>([](int32_t value_1, int32_t value_2) {
			return (uint32_t)value_1 >> value_2;
		});
	}
	dllexport gnu_noinline void math_rol() {
		this->stack.binary_op<int32_t, true>([](int32_t value_1, int32_t value_2) {
			return _rotl(value_1, value_2);
		});
	}
	dllexport gnu_noinline void math_ror() {
		this->stack.binary_op<int32_t, true>([](int32_t value_1, int32_t value_2) {
			return _rotr(value_1, value_2);
		});
	}
	dllexport gnu_noinline void math_udiv() {
		this->stack.binary_op<int32_t, true>([](uint32_t value_1, uint32_t value_2) {
			return value_1 / value_2;
		});
	}
	dllexport gnu_noinline void math_umod() {
		this->stack.binary_op<int32_t, true>([](uint32_t value_1, uint32_t value_2) {
			return value_1 % value_2;
		});
	}

	dllexport gnu_noinline void fastcall debug_print_new(EclInstruction *const restrict current_instruction) {

#define test_end() if (expect_char_chance(c, '\0', 0.05)) goto log_label
		//unsigned char *restrict va_array = (unsigned char*)__builtin_alloca(AlignUpToMultipleOf2(current_instruction->param_count - 1 + 2, 2) * sizeof(double));
		unsigned char *restrict va_array = (unsigned char*)alloc_vla(AlignUpToMultipleOf2(current_instruction->param_count - 1 + 2, 2) * sizeof(double));

		auto write_va_int = [&](int32_t value) gnu_always_inline {
			*(int32_t*)va_array = value;
			va_array += sizeof(int32_t);
		};
		auto write_va_int64 = [&](int64_t value) gnu_always_inline {
			*(int64_t*)va_array = value;
			va_array += sizeof(int64_t);
		};
		auto write_va_float = [&](float value) gnu_always_inline {
			*(double*)va_array = (double)value;
			va_array += sizeof(double);
		};

		const char *restrict format = StringArg(4);
		asm __volatile__(
			"PUSH %[va_array] \n"
			"PUSH %[format] \n"
			:
			: asm_arg("X", format), asm_arg("X", va_array)
		);
		EclArg* va_args = (EclArg*)(current_instruction->args + UIntArg(0));
		int32_t index = 1;
		char c;
		
		auto read_int_arg = [&]() gnu_always_inline {
			const char type_char = va_args++->character;
			if (!(expect_char_chance(type_char, 'f', 0.01) || expect_char_chance(type_char, 'g', 0.00))) {
				return this->parse_int_as_arg(index++, va_args++->integer);
			} else {
				return (int32_t)this->parse_float_as_arg(index++, va_args->real);
			}
		};
		auto read_float_arg = [&]() gnu_always_inline {
			const char type_char = va_args++->character;
			if (expect_char_chance(type_char, 'f', 0.99) || expect_char_chance(type_char, 'g', 0.00)) {
				return this->parse_float_as_arg(index++, va_args++->real);
			} else {
				return (float)this->parse_float_as_arg(index++, va_args->integer);
			}
		};
		
		auto parse_width_or_precision_number = [&]() gnu_always_inline {
			if (expect_char_chance(c, '*', 0.01)) {
				write_va_int(read_int_arg());
				c = *++format;
			} else {
				while (is_valid_decimal(c)) c = *++format;
			}
		};

		for (;; ++format) {
			switch (expect_chance(*format, '%', 0.1)) {
				case '\0': log_label:
					asm_call(log_vprintf);
					return;
				default:
					continue;
				case '%':
					c = *++format;
					// c = %X
					test_end();
					if (expect_char_chance(c, '%', 0.05)) continue;
					do {
						c = *++format;
						test_end();
					} while (expect_char_chance(c, ' ', 0.01) ||
							 expect_char_chance(c, '#', 0.01) ||
							 expect_char_chance(c, '+', 0.01) ||
							 expect_char_chance(c, '-', 0.01) ||
							 expect_char_chance(c, '0', 0.01));
					// c = %[flags]X
					parse_width_or_precision_number();
					test_end();
					// c = %[flags][width]X
					if (expect_char_chance(c, '.', 0.01)) {
						c = *++format;
						test_end();
						parse_width_or_precision_number();
						test_end();
					}
					// c = %[flags][width][precision]X
					bool extra_length = false;
					switch (expect(c, 'd')) {
						case 'L': // Long Double
						case 'j': // intmax_t
							extra_length = true;
						case 'z': // size_t
						case 't': // ptrdiff_t
							c = *++format;
							break;
						case 'l': // double, long, long long
							extra_length = true;
						case 'h': // short, char
							c == format[1]
								? (void)++format // char, long long
								: (void)(extra_length = false); // short, long
							c = *++format;
							break;
					}
					switch (int32_t value; expect(c, 'd')) {
						case '\0':
							goto log_label;
						case 'c': case 's': case 'p':
							extra_length = false;
						case 'd': case 'i': case 'u': case 'x': case 'X': case 'o':
							if (value = read_int_arg(); expect_chance(!extra_length, true, 1.0)) {
							write_int:
								write_va_int(value);
							} else {
								write_va_int64(value);
							}
							break;
						case 'f': case 'F': case 'e': case 'E': case 'g': case 'G':
							write_va_float(read_float_arg());
							break;
						case 'n':
							value = (int32_t)++va_args;
							++va_args;
							goto write_int;
					}
			}
		}
	}
};

//EclInstruction* EclContext::get_sub_instrs() {
	//int32_t instr_offset = this->current_instr_offset;
	//if (instr_offset != -1 && this->current_sub_index != -1) {
		//return (EclInstruction*)&(*(this->enemy_full->file_manager->subroutines))[this->current_sub_index].sub_header->instructions[instr_offset];
	//} else {
		//return NULL;
	//}
//}

// size: 0x103C
struct EnemyBase {
	// void* vtable; // 0x0
	EclContext* current_context_ptr; // 0x4
	EclContext current_context; // 0x8
	ZUNList<EclContext> context_list; // 0x1030
};

// size: 0x2C
struct PosVel {
	Float3 __float3_0; // 0x0

	uint32_t flags; // 0x28
};

// size: 0x14
struct Timer {
	int32_t previous = -999999; // 0x0
	int32_t current = 0; // 0x4
	float current_f = 0.0f; // 0x8
	float* scale_ptr; // 0xC
	union { // 0x10
		uint32_t flags;
		struct {
			uint32_t unknown_flag_A : 1;
		};
	};
	
	// 0x404ED0
	void tick() {
		int32_t temp = this->current;
		this->previous = temp;
		if (*this->scale_ptr > 0.99f && *this->scale_ptr < 1.01f) {
			this->current_f += 1.0f;
			this->current = temp + 1;
		} else {
			this->current = (int32_t)(this->current_f = *this->scale_ptr + this->current_f);
		}
	}
};

// size: 0x3C
struct EnemyDrop {
	int32_t main_id; // 0x0
	int32_t extra_slots[12]; // 0x4
	Float2 area; // 0x34
};

// size: 0xC
struct EnemyLife {
	int32_t current;
	int32_t maximum;
	int32_t remaining_current_attack;
};

// size: 0x10
struct EnemyCallback {
	int32_t hp_threshold; // 0x0
	int32_t timer_threshold; // 0x4
	char* threshold_sub; // 0x8
	char* timeout_sub; // 0xC
};

struct Enemy {
	PosVel prev_final_position; // 0x0
	PosVel final_position; // 0x2C
	PosVel abs_position; // 0x58
	PosVel rel_position; // 0x84
	Float2 hitbox_size; // 0xB0
	Float2 collision_size; // 0xB8
	AnmID anm_vm_ids[10]; // 0xC0

	int32_t selected_anm_index; // 0xE8
	int32_t anm_slot_0_anm_index; // 0xEC
	int32_t anm_slot_0_script; // 0xF0
	int32_t anm_main; // 0xF4
	int32_t cur_subscript_for_anm_main; // 0xF8

	Timer boss_timer; // 0x11C

	Float2 final_sprite_size; // 0x13A4
	Float2 move_limit_center; // 0x13AC
	Float2 move_limit_size; // 0x13B4

	EnemyLife life; // 0x13C0
	EnemyDrop __drop_13CC; // 0x13CC

	Timer invincible_timer; // 0x141C
	Timer no_collision_timer; // 0x1430
	union { // 0x1444
		uint32_t flags_low;
		struct {
			// 0-8
			uint32_t move_limit : 1; // 9
		};
	};

	int32_t boss_id; // 0x1450
	float player_protect_range; // 0x1454

	EnemyCallback callbacks[8]; // 0x1464

	EnemyBase* enemy_base; // 0x14D8
};

static volatile int32_t yeetus;
// size: 0x2518 (+0x14DC)
struct EnemyFull : EnemyBase {

	virtual void funcA() {};
	virtual int32_t get_int_var(int32_t index) {
		return yeetus + index;
	};
	virtual int32_t* get_int_ptr(int32_t index) {
		return (int32_t*)&yeetus + index;
	};
	virtual float get_float_var(int32_t index) {
		return yeetus + index;
	};
	virtual float* get_float_ptr(int32_t index) {
		return (float*)&yeetus + index;
	};
	virtual void funcB() {};

	// void* vtable; // 0x0
	// EclContext* current_context_ptr; // 0x4
	// EclContext current_context; // 0x8
	// ZUNList<EclContext> context_list; // 0x1030

	Enemy enemy_inner;

	//Float3 __float3_1094; // 0x1094

	//int __dword_23F8; // 0x23F8
	//int __dword_23FC; // 0x23FC

	//int __dword_2408; // 0x2408

	//int __dword_2480; // 0x2480

	int32_t init_sub_context(char* sub_name) {
		//this->current_context_ptr->current_instr = this->current_context.controller->get_sub_instrs_by_name(sub_name);
		this->current_context_ptr->time = 0.0f;
		return 0;
	}

	void new_async(int32_t async_id, int index) {
		EclContext* new_context = (EclContext*)malloc(sizeof(EclContext));
		if (new_context != NULL) {
			new_context->stack.top_offset = 0;
			new_context->stack.locals_offset = 0;
		}
		ZUNList<EclContext>* new_context_link = (ZUNList<EclContext>*)malloc(sizeof(ZUNList<EclContext>));
		new_context->async_id = async_id;
		new_context->parent_enemy = this;
		new_context->time = 0.0f;
		new_context->current_instr = NULL;
		new_context->current_difficulty_mask = this->current_context_ptr->current_difficulty_mask;
		ZUNList<EclContext>* context_list_head = &this->context_list;
		new_context_link->data = new_context;
		new_context_link->next = NULL;
		new_context_link->prev = NULL;
		if (context_list_head->next != NULL) {
			new_context_link->next = context_list_head->next;
			context_list_head->next->prev = new_context_link;
		}
		context_list_head->next = new_context_link;
		new_context_link->prev = context_list_head;
		new_context_link->data->call(this->current_context_ptr, index);
	}

	ZUNList<EclContext>* get_async_node_by_id(int32_t ID) {
		ZUNList<EclContext>* current_node = &this->context_list;
		while (current_node) {
			if (current_node->data->async_id == ID) {
				break;
			}
			current_node = current_node->next;
		}
		return current_node;
	}

	void kill_all_asyncs() {
		ZUNList<EclContext>* current_node = this->context_list.next;
		while (current_node) {
			ZUNList<EclContext>* next_node = current_node->next;
			current_node->data->current_instr = NULL;
			current_node = next_node;
		}
	}
};

dllexport gnu_noinline int32_t thiscall EclContext::LowECL_Run(float current_gamespeed) {
	EclInstruction*& current_instruction = this->current_instr;
	if (current_instruction == NULL) {
		return -1;
	}
	int32_t offset_to_next;
	while (expect_chance(current_instruction->time <= this->time, true, 0.75)) {
		switch (current_instruction->opcode) {
			case 10: // ret
				this->stack.leave_frame();
				if (!this->stack.top_offset) goto DeletThis;
				StackPopSafe(&this->stack) {
					this->current_instr = ReadPoppedValue(&this->stack, EclInstruction*);
				}
				StackPopSafe(&this->stack) {
					this->time = ReadPoppedValue(&this->stack, float);
				}
				if (this->current_instr == NULL) {
					goto DeletThis;
				}
				break;
			case 1: // delete
				this->current_instr = NULL;
			DeletThis:
				return -1;
			case 15: // call_async
				this->parent_enemy->new_async(-1, false);
				break;
			case 21: // kill_all_async
				this->parent_enemy->kill_all_asyncs();
				break;
			case 16: { // call_async_id
				int32_t string_len = IntArg(0);
				int32_t dword_len = (string_len + sizeof(int32_t)) / sizeof(int32_t);
				this->parent_enemy->new_async(this->parse_int_as_arg(IntArg(dword_len), 1), true);
				break;
			}
			case 17: // kill_async
				if (ZUNList<EclContext>* async_node = this->parent_enemy->get_async_node_by_id(this->get_int_arg(0))) {
					async_node->data->current_instr = NULL;
				}
				break;
			case 18: // __async_unknown_flag_set
				if (ZUNList<EclContext>* async_node = this->parent_enemy->get_async_node_by_id(this->get_int_arg(0))) {
					async_node->data->__async_flag_A = true;
				}
				break;
			case 19: // __async_unknown_flag_clear
				if (ZUNList<EclContext>* async_node = this->parent_enemy->get_async_node_by_id(this->get_int_arg(0))) {
					async_node->data->__async_flag_A = false;
				}
				break;
			case 20: // __async_unknown_value
				if (ZUNList<EclContext>* async_node = this->parent_enemy->get_async_node_by_id(this->get_int_arg(0))) {
					async_node->data->__async_dword_1018 = this->get_int_arg(1);
				}
				break;
			case 11: // call
				if (this->call(this, 0) != 0) {
					goto DeletThis;
				}
				continue;
			case 14: { // jump_neq
				int32_t stack_value;
				StackPopSafe(&this->stack) {
					stack_value = ReadPoppedValue(&this->stack, int32_t);
					if (this->stack.pop<char>() == 'f') {
						stack_value = *(float*)&stack_value;
					}
				}
				if (stack_value) goto Jump;
				break;
			}
			case 13: { // jump_equ
				int32_t stack_value;
				StackPopSafe(&this->stack) {
					stack_value = ReadPoppedValue(&this->stack, int32_t);
					if (this->stack.pop<char>() == 'f') {
						stack_value = *(float*)&stack_value;
					}
				}
				if (stack_value) break;
			}
		Jump: case 12: // jump
				this->time = IntArg(1);
				offset_to_next = IntArg(0);
				goto AddOffset;
			case 83: // wait
				this->time -= this->get_int_arg(0);
				break;
			case 40: // frame_enter
				this->stack.enter_frame(this->get_int_arg(0));
				break;
			case 41: // frame_leave
				this->stack.leave_frame();
				break;
			case 42: { // push_int
				int32_t value = this->get_int_arg(0);
				this->stack.push_value('i', sizeof(int32_t), &value);
				break;
			}
			case 43: { // pop_int
				int32_t* value_ptr = this->get_int_arg_ptr(0);
				StackPopSafe(&this->stack) {
					*value_ptr = ReadPoppedValue(&this->stack, int32_t);
					if (this->stack.pop<char>() == 'f') {
						*value_ptr = *(float*)value_ptr;
					}
				}
				break;
			}
			case 44: { // push_float
				float value = this->get_float_arg(0);
				this->stack.push_value('f', sizeof(float), &value);
				break;
			}
			case 45: { // pop_float
				float* value_ptr = this->get_float_arg_ptr(0);
				StackPopSafe(&this->stack) {
					*value_ptr = ReadPoppedValue(&this->stack, float);
					switch (this->stack.pop<char>()) {
						case 'i':
							*value_ptr = *(int32_t*)value_ptr;
						case 'f': default:
							break;
					}
				}
				break;
			}

#define IntStackBoilerplate(var_name) \
int32_t var_name; \
StackPopSafe(&this->stack) { \
	var_name = ReadPoppedValue(&this->stack, int32_t); \
	if (this->stack.pop<char>() == 'f') { \
		var_name = (long double)*(float*)&var_name; \
	} \
}
#define FloatStackBoilerplate(var_name) \
union { \
	float real; \
	int32_t integer; \
} var_name; \
var_name.real = GARBAGE_ARG(float); \
StackPopSafe(&this->stack) { \
	var_name.integer = ReadPoppedValue(&this->stack, int32_t); \
	switch (this->stack.pop<char>()) { \
		case 'i': \
			var_name.real = var_name.integer;\
		case 'f': default: \
			break; \
	} \
}
			case 50: { // math_int_add
				IntStackBoilerplate(stack_value_2);
				IntStackBoilerplate(stack_value_1);
				stack_value_1 += stack_value_2;
				this->stack.push_value('i', sizeof(int32_t), &stack_value_1);
				break;
			}
			case 52: { // math_int_sub
				IntStackBoilerplate(stack_value_2);
				IntStackBoilerplate(stack_value_1);
				stack_value_1 -= stack_value_2;
				this->stack.push_value('i', sizeof(int32_t), &stack_value_1);
				break;
			}
			case 54: { // math_int_mul
				IntStackBoilerplate(stack_value_2);
				IntStackBoilerplate(stack_value_1);
				stack_value_1 *= stack_value_2;
				this->stack.push_value('i', sizeof(int32_t), &stack_value_1);
				break;
			}
			case 56: { // math_int_div
				IntStackBoilerplate(stack_value_2);
				IntStackBoilerplate(stack_value_1);
				stack_value_1 /= stack_value_2;
				this->stack.push_value('i', sizeof(int32_t), &stack_value_1);
				break;
			}
			case 58: { // math_int_mod
				IntStackBoilerplate(stack_value_2);
				IntStackBoilerplate(stack_value_1);
				stack_value_1 %= stack_value_2;
				this->stack.push_value('i', sizeof(int32_t), &stack_value_1);
				break;
			}
			case 51: { // math_float_add
				FloatStackBoilerplate(stack_value_2);
				FloatStackBoilerplate(stack_value_1);
				stack_value_1.real += stack_value_2.real;
				this->stack.push_value('f', sizeof(float), &stack_value_1);
				break;
			}
			case 53: { // math_float_sub
				FloatStackBoilerplate(stack_value_2);
				FloatStackBoilerplate(stack_value_1);
				stack_value_1.real -= stack_value_2.real;
				this->stack.push_value('f', sizeof(float), &stack_value_1);
				break;
			}
			case 55: { // math_float_mul
				FloatStackBoilerplate(stack_value_2);
				FloatStackBoilerplate(stack_value_1);
				stack_value_1.real *= stack_value_2.real;
				this->stack.push_value('f', sizeof(float), &stack_value_1);
				break;
			}
			case 57: { // math_float_div
				FloatStackBoilerplate(stack_value_2);
				FloatStackBoilerplate(stack_value_1);
				stack_value_1.real /= stack_value_2.real;
				this->stack.push_value('f', sizeof(float), &stack_value_1);
				break;
			}
			case 59: { // cmp_int_equ
				IntStackBoilerplate(stack_value_2);
				IntStackBoilerplate(stack_value_1);
				int32_t compare = stack_value_1 == stack_value_2;
				this->stack.push_value('i', sizeof(int32_t), &compare);
				break;
			}
			case 61: { // cmp_int_neq
				IntStackBoilerplate(stack_value_2);
				IntStackBoilerplate(stack_value_1);
				int32_t compare = stack_value_1 != stack_value_2;
				this->stack.push_value('i', sizeof(int32_t), &compare);
				break;
			}
			case 63: { // cmp_int_les
				IntStackBoilerplate(stack_value_2);
				IntStackBoilerplate(stack_value_1);
				int32_t compare = stack_value_1 < stack_value_2;
				this->stack.push_value('i', sizeof(int32_t), &compare);
				break;
			}
			case 65: { // cmp_int_leq
				IntStackBoilerplate(stack_value_2);
				IntStackBoilerplate(stack_value_1);
				int32_t compare = stack_value_1 <= stack_value_2;
				this->stack.push_value('i', sizeof(int32_t), &compare);
				break;
			}
			case 67: { // cmp_int_gtr
				IntStackBoilerplate(stack_value_2);
				IntStackBoilerplate(stack_value_1);
				int32_t compare = stack_value_1 > stack_value_2;
				this->stack.push_value('i', sizeof(int32_t), &compare);
				break;
			}
			case 69: { // cmp_int_equ
				IntStackBoilerplate(stack_value_2);
				IntStackBoilerplate(stack_value_1);
				int32_t compare = stack_value_1 >= stack_value_2;
				this->stack.push_value('i', sizeof(int32_t), &compare);
				break;
			}
			case 71: { // cmp_int_not
				IntStackBoilerplate(stack_value_1);
				int32_t compare = stack_value_1 == 0;
				this->stack.push_value('i', sizeof(int32_t), &compare);
				break;
			}
			case 60: { // cmp_float_equ
				FloatStackBoilerplate(stack_value_2);
				FloatStackBoilerplate(stack_value_1);
				int32_t compare = stack_value_1.real == stack_value_2.real;
				this->stack.push_value('i', sizeof(int32_t), &compare);
				break;
			}
			case 62: { // cmp_float_neq
				FloatStackBoilerplate(stack_value_2);
				FloatStackBoilerplate(stack_value_1);
				int32_t compare = stack_value_1.real == stack_value_2.real;
				this->stack.push_value('i', sizeof(int32_t), &compare);
				break;
			}
			case 64: { // cmp_float_les
				FloatStackBoilerplate(stack_value_2);
				FloatStackBoilerplate(stack_value_1);
				int32_t compare = stack_value_1.real == stack_value_2.real;
				this->stack.push_value('i', sizeof(int32_t), &compare);
				break;
			}
			case 66: { // cmp_float_leq
				FloatStackBoilerplate(stack_value_2);
				FloatStackBoilerplate(stack_value_1);
				int32_t compare = stack_value_1.real == stack_value_2.real;
				this->stack.push_value('i', sizeof(int32_t), &compare);
				break;
			}
			case 68: { // cmp_float_gtr
				FloatStackBoilerplate(stack_value_2);
				FloatStackBoilerplate(stack_value_1);
				int32_t compare = stack_value_1.real == stack_value_2.real;
				this->stack.push_value('i', sizeof(int32_t), &compare);
				break;
			}
			case 70: { // cmp_float_geq
				FloatStackBoilerplate(stack_value_2);
				FloatStackBoilerplate(stack_value_1);
				int32_t compare = stack_value_1.real == stack_value_2.real;
				this->stack.push_value('i', sizeof(int32_t), &compare);
				break;
			}
			case 72: { // cmp_float_not
				FloatStackBoilerplate(stack_value_1);
				int32_t compare = stack_value_1.real == 0.0f;
				this->stack.push_value('i', sizeof(int32_t), &compare);
				break;
			}
			case 73: { // cmp_or
				IntStackBoilerplate(stack_value_2);
				IntStackBoilerplate(stack_value_1);
				int32_t compare = stack_value_1 || stack_value_2;
				this->stack.push_value('i', sizeof(int32_t), &compare);
				break;
			}
			case 74: { // cmp_and
				IntStackBoilerplate(stack_value_2);
				IntStackBoilerplate(stack_value_1);
				int32_t compare = stack_value_1 && stack_value_2;
				this->stack.push_value('i', sizeof(int32_t), &compare);
				break;
			}
			case 75: { // math_bit_xor
				IntStackBoilerplate(stack_value_2);
				IntStackBoilerplate(stack_value_1);
				stack_value_1 ^= stack_value_2;
				this->stack.push_value('i', sizeof(int32_t), &stack_value_1);
				break;
			}
			case 76: { // math_bit_or
				IntStackBoilerplate(stack_value_2);
				IntStackBoilerplate(stack_value_1);
				stack_value_1 ^= stack_value_2;
				this->stack.push_value('i', sizeof(int32_t), &stack_value_1);
				break;
			}
			case 77: { // math_bit_and
				IntStackBoilerplate(stack_value_2);
				IntStackBoilerplate(stack_value_1);
				stack_value_1 ^= stack_value_2;
				this->stack.push_value('i', sizeof(int32_t), &stack_value_1);
				break;
			}
			case 84: { // math_int_neg
				IntStackBoilerplate(stack_value_1);
				stack_value_1 = -stack_value_1;
				this->stack.push_value('i', sizeof(int32_t), &stack_value_1);
				break;
			}
			case 85: { // math_float_neg
				FloatStackBoilerplate(stack_value_1);
				// Apparently most of these float operations were type punned, ZUN missed one
				stack_value_1.integer = -stack_value_1.integer;
				this->stack.push_value('f', sizeof(float), &stack_value_1);
				break;
			}
			case 78: { // math_post_dec
				int32_t value = this->get_int_arg(0);
				int32_t* value_ptr = this->get_int_arg_ptr(0);
				*value_ptr = value - 1;
				this->stack.push_value('i', sizeof(int32_t), &value);
				break;
			}
			case 79: { // math_sin
				FloatStackBoilerplate(stack_value_1);
				stack_value_1.real = sinf(stack_value_1.real);
				this->stack.push_value('f', sizeof(float), &stack_value_1);
				break;
			}
			case 80: { // math_cos
				FloatStackBoilerplate(stack_value_1);
				stack_value_1.real = cosf(stack_value_1.real);
				this->stack.push_value('f', sizeof(float), &stack_value_1);
				break;
			}
			case 81: { // math_circle_pos
				float radius = this->get_float_arg(3);
				float angle = reduce_angle(this->get_float_arg(2));
				Float2 coords;
				coords.make_from_vector(angle, radius);
				*this->get_float_arg_ptr(0) = coords.x;
				*this->get_float_arg_ptr(1) = coords.y;
				break;
			}
			case 86: { // math_hypot_squared
				float value_1 = this->get_float_arg(1);
				float value_2 = this->get_float_arg(2);
				float* out = this->get_float_arg_ptr(0);
				*out = value_1 * value_1 + value_2 * value_2;
				break;
			}
			case 82: { // math_reduce_angle
				float* value_ptr = this->get_float_arg_ptr(0);
				//*value_ptr = reduce_angle(this->get_float_arg(0));
				break;
			}
			case 87: { // math_line_angle
				float X2 = this->get_float_arg(2);
				float X = this->get_float_arg(0) - X2;
				float Y2 = this->get_float_arg(3);
				float Y = this->get_float_arg(1) - Y2;
				*this->get_float_arg_ptr(0) = atan2f(Y, X);
				break;
			}
			case 30: { // debug_idk
				char* out_buffer = (char*)malloc(1024);
				out_buffer[0] = '\0';
				if (const char* ins_buffer = StringArg(4)) {
					char* out_write = out_buffer;
					int32_t va_index = 1;
					int32_t va_offset = 0;
					int32_t previous_dwords = 6;
					do {
						const char* format = strchr(ins_buffer, '%');
						if (!format) break;
						strcpy(out_write, ins_buffer);
						out_write[format - ins_buffer] = '\0';
						switch (*(format+1)) {
							case 'd': case 'f': {
								int32_t string_len = IntArg(0);
								switch (CharArg((sizeof(int32_t) + string_len) + va_offset)) {
									default: {
										int value = this->parse_int_as_arg(va_index, IntArg(string_len / 4 + previous_dwords));
#ifndef NDEBUG
										out_write += sprintf(&out_write[format - ins_buffer], "%d", value);
#endif
										break;
									}
									case 'f': case 'g': {
										float value = this->parse_float_as_arg(va_index, FloatArg(string_len / 4 + previous_dwords));
#ifndef NDEBUG
										out_write += sprintf(&out_write[format - ins_buffer], "%f", value);
#endif
										break;
									}
								}
								va_offset += 8;
								previous_dwords += 2;
								++va_index;
								break;
							}
							case '%':
								break;
						}
					} while (ins_buffer += 2);
				}
	#ifndef NDEBUG
				log_print("%s", out_buffer);
	#endif
				free(out_buffer);
				break;
			}
		}
		offset_to_next = current_instruction->offset_to_next;
	AddOffset:
		IndexInstr(offset_to_next);
	}
	this->time += current_gamespeed;
	return 0;
}

// size: 0x24
struct UpdateFunc {
	
};

// size: 0x68
struct EnemyManager {
	int __flags_0; // 0x0
	
	UpdateFunc* on_tick_func; // 0x8
	UpdateFunc* on_draw_func; // 0xC
	int __boss_related_10; // 0x10
	
	union { // 0x30
		//AnmLoaded* anm_files[4]; // 0x30 /* (anm index reads this) */
		struct {
			//AnmLoaded* primary_anm; // 0x30
			//AnmLoaded* included_anms[3]; // 0x34
		};
	};
	Timer __timer_40; // 0x40
	EnemyController* enemy_controller; // 0x54

};

// MoF version
gnu_noinline int32_t EclContext::get_int_arg(int32_t index) {
	EclInstruction* current_instruction = this->current_instr;
	if (current_instruction->param_mask & (1 << index)) {
		int32_t value = IntArg(index);
		if (value >= 0) {
			return this->stack.read_local<int32_t>(value);
		} else if (value == -1) {
			StackPopSafe(&this->stack) {
				value = ReadPoppedValue(&this->stack, int32_t);
				if (this->stack.pop<char>() == 'f') {
					return *(float*)&value;
				} else {
					return value;
				}
			}
			return index;
		} else {
			return this->parent_enemy->get_int_var(value);
		}
	} else {
		return IntArg(index);
	}
}

gnu_noinline int32_t EclContext::get_int_argTD(int32_t index) {
	EclInstruction* current_instruction = this->current_instr;
	int32_t value = IntArg(index);
	if (current_instruction->param_mask & (1 << index)) {
		if (value >= 0) {
			return this->stack.read_local<int32_t>(value);
		} else if (value <= -1 && value >= -100) {
			EclStackValue* stack_value_ptr = &((EclStackValue*)(this->stack.stack_raw + this->stack.top_offset))[value];
			value = stack_value_ptr->value.integer;
			if (stack_value_ptr->type == 'f') {
				return *(float*)&value;
			} else {
				return value;
			}
		} else {
			return this->parent_enemy->get_int_var(value);
		}
	} else {
		return value;
	}
}

gnu_noinline int32_t EclContext::parse_int_as_arg(int32_t index, int32_t value) {
	EclInstruction* current_instruction = this->current_instr;
	if (current_instruction->param_mask & (1 << index)) {
		if (value >= 0) {
			return this->stack.read_local<int32_t>(value);
		} else if (value == -1) {
			StackPopSafe(&this->stack) {
				value = ReadPoppedValue(&this->stack, int32_t);
				switch (this->stack.pop<char>()) {
					case 'f':
						return *(float*)&value;
					default:
						return value;
				}
			}
			return value;
		} else {
			return this->parent_enemy->get_int_var(value);
		}
	} else {
		return value;
	}
}

gnu_noinline float EclContext::get_float_arg(int32_t index) {
	EclInstruction* current_instruction = this->current_instr;
	if (current_instruction->param_mask & (1 << index)) {
		if (FloatArg(index) >= 0.0f) {
			return this->stack.read_local<float>(FloatArg(index));
		} else if (FloatArg(index) == -1.0f) {
			StackPopSafe(&this->stack) {
				float value = ReadPoppedValue(&this->stack, int32_t);
				switch (this->stack.pop<char>()) {
					case 'i':
						value = *(int32_t*)&value;
					case 'f': default:
						return value;
				}
			}
			return index;
		} else {
			return this->parent_enemy->get_float_var((int32_t)FloatArg(index));
		}
	} else {
		return FloatArg(index);
	}
}

gnu_noinline float EclContext::parse_float_as_arg(int32_t index, float value) {
	EclInstruction* current_instruction = this->current_instr;
	if (current_instruction->param_mask & (1 << index)) {
		if (value >= 0.0f) {
			return this->stack.read_local<float>(value);
		} else if (value == -1.0f) {
			StackPopSafe(&this->stack) {
				value = ReadPoppedValue(&this->stack, float);
				switch (this->stack.pop<char>()) {
					case 'i':
						value = *(int32_t*)&value;
					case 'f': default:
						return value;
				}
			}
			return value;
		} else {
			return this->parent_enemy->get_float_var(value);
		}
	} else {
		return value;
	}
}

gnu_noinline int32_t* EclContext::get_int_arg_ptr(int32_t index) {
	EclInstruction* current_instruction = this->current_instr;
	if (current_instruction->param_mask & (1 << index)) {
		int32_t value = IntArg(index);
		if (value >= 0) {
			return (int32_t*)(this->stack.stack_raw + this->stack.locals_offset + value);
		} else {
			return this->parent_enemy->get_int_ptr(value);
		}
	} else {
		return NULL;
	}
}

gnu_noinline float* EclContext::get_float_arg_ptr(int32_t index) {
	EclInstruction* current_instruction = this->current_instr;
	if (current_instruction->param_mask & (1 << index)) {
		float value = FloatArg(index);
		if (value >= 0.0f) {
			return (float*)(this->stack.stack_raw + this->stack.locals_offset + (int32_t)value);
		} else {
			return this->parent_enemy->get_float_ptr(value);
		}
	} else {
		return NULL;
	}
}

gnu_noinline int32_t EclContext::call(EclContext* parent_context, int va_index) {
	EclInstruction*& current_instruction = parent_context->current_instr;
	int32_t va_offset = va_index * sizeof(DWORD) + IntArg(0) + sizeof(int32_t); // 
	int32_t temp_top = this->stack.top_offset;
	int32_t temp_value = 0;
	int32_t temp_offset = this->stack.top_offset + 8;
	if (!this->stack.top_offset) {
		this->stack.push<int32_t>(0);
		temp_offset = this->stack.top_offset + 12;
	}
	if (++va_index < current_instruction->param_count) {
		unsigned char* stack_write_ptr = &this->stack.stack_raw[temp_offset];
		int32_t va_value_offset = va_offset + sizeof(DWORD);
		do {
			switch (CharArg(va_offset)) {
				default: {
					int32_t value = parent_context->parse_int_as_arg(va_index, IntArg(va_value_offset / sizeof(DWORD)));
					if (CharArg(va_offset + 1) == 'f') {
						*(float*)stack_write_ptr = value;
						break;
					} else {
						*(int32_t*)stack_write_ptr = value;
						break;
					}
				}
				case 'f': case 'g': {
					float value = parent_context->parse_float_as_arg(va_index, FloatArg(va_value_offset / sizeof(DWORD)));
					if (CharArg(va_offset + 1) == 'f') {
						*(float*)stack_write_ptr = value;
						break;
					} else {
						*(int32_t*)stack_write_ptr = value;
						break;
					}
				}
			}
			stack_write_ptr += sizeof(DWORD);
			va_value_offset += 8;
			va_offset += 8;
		} while (++va_index < current_instruction->param_count);
	}
	if (!temp_top) {
		StackPopSafe(&this->stack) {
			temp_value = ReadPoppedValue(&this->stack, int32_t);
		}
		this->stack.top_offset = temp_top;
		*(int32_t*)(this->stack.stack_raw + temp_top - sizeof(int32_t)) = temp_value;
		this->stack.push<float>(parent_context->time);
		this->stack.push<float>(current_instruction->time);
	} else {
		this->stack.top_offset = 4;
		this->stack.push<float>(0.0f);
		this->stack.push<float>(0.0f);
	}
	EclContext* prev_context = parent_context->parent_enemy->current_context_ptr;
	parent_context->parent_enemy->current_context_ptr = this;
	//parent_context->parent_enemy->current_context.controller->init_sub_context(StringArg(4));
	if (!parent_context->parent_enemy->current_context_ptr->current_instr) {
		current_instruction = NULL;
		return -1;
	} else {
		parent_context->parent_enemy->current_context_ptr = prev_context;
		return 0;
	}
}

gnu_noinline const char* fastcall eval_expr(const char* expr, char end, size_t* out, void* regs, uintptr_t rel_source, void* hMod) {
	return (const char*)(*out = (intptr_t)expr + end + (intptr_t)regs + rel_source + (intptr_t)hMod + yeetus);
}
#define this context
inline void thcrap_expr(EclContext* context) {
	EclInstruction* current_instruction = this->current_instr;
	int32_t* out_ptr = this->get_int_arg_ptr(0);
	eval_expr(StringArg(4), '\0', (size_t*)out_ptr, NULL, NULL, NULL);
}



inline void math_float_mod(EclContext* context) {
	//EclInstruction* current_instruction = this->current_instr;
	FloatStackBoilerplate(stack_value_2);
	FloatStackBoilerplate(stack_value_1);
	//__asm__(
		//"CALL %P[fmodf] \n"
		//: "+t" (stack_value_1.real)
		//: "u" (stack_value_2.real), [fmodf]"i"(fmodf)
		//: "st(1)", "eax", "ecx", "edx"
	//);
	stack_value_1.real = fmodf(stack_value_1.real, stack_value_2.real);
	this->stack.push_value('f', sizeof(float), &stack_value_1);
}
#undef this