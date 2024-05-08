#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wc++20-extensions"
#pragma clang diagnostic ignored "-Wshift-op-parentheses"

// Something got undefined in my cmake file or some
// crap and I'd rather just define it here than fix it
#define POINTER_64 __ptr64

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../zero/util.h"

enum BytecodeOp : uint8_t {
    NOP             = 0x00,
    ACONST_NULL     = 0x01,
    ICONST_M1       = 0x02,
    ICONST_0        = 0x03,
    ICONST_1        = 0x04,
    ICONST_2        = 0x05,
    ICONST_3        = 0x06,
    ICONST_4        = 0x07,
    ICONST_5        = 0x08,
    LCONST_0        = 0x09,
    LCONST_1        = 0x0A,
    FCONST_0        = 0x0B,
    FCONST_1        = 0x0C,
    FCONST_2        = 0x0D,
    DCONST_0        = 0x0E,
    DCONST_1        = 0x0F,
    BIPUSH          = 0x10, // s8
    SIPUSH          = 0x11, // s16
    LDC             = 0x12, // u8
    LDC_W           = 0x13, // u16
    LDC2_W          = 0x14, // u16
    ILOAD           = 0x15, // u8(w)
    LLOAD           = 0x16, // u8(w)
    FLOAD           = 0x17, // u8(w)
    DLOAD           = 0x18, // u8(w)
    ALOAD           = 0x19, // u8(w)
    ILOAD_0         = 0x1A,
    ILOAD_1         = 0x1B,
    ILOAD_2         = 0x1C,
    ILOAD_3         = 0x1D,
    LLOAD_0         = 0x1E,
    LLOAD_1         = 0x1F,
    LLOAD_2         = 0x20,
    LLOAD_3         = 0x21,
    FLOAD_0         = 0x22,
    FLOAD_1         = 0x23,
    FLOAD_2         = 0x24,
    FLOAD_3         = 0x25,
    DLOAD_0         = 0x26,
    DLOAD_1         = 0x27,
    DLOAD_2         = 0x28,
    DLOAD_3         = 0x29,
    ALOAD_0         = 0x2A,
    ALOAD_1         = 0x2B,
    ALOAD_2         = 0x2C,
    ALOAD_3         = 0x2D,
    IALOAD          = 0x2E,
    LALOAD          = 0x2F,
    FALOAD          = 0x30,
    DALOAD          = 0x31,
    AALOAD          = 0x32,
    BALOAD          = 0x33,
    CALOAD          = 0x34,
    SALOAD          = 0x35,
    ISTORE          = 0x36, // u8(w)
    LSTORE          = 0x37, // u8(w)
    FSTORE          = 0x38, // u8(w)
    DSTORE          = 0x39, // u8(w)
    ASTORE          = 0x3A, // u8(w)
    ISTORE_0        = 0x3B,
    ISTORE_1        = 0x3C,
    ISTORE_2        = 0x3D,
    ISTORE_3        = 0x3E,
    LSTORE_0        = 0x3F,
    LSTORE_1        = 0x40,
    LSTORE_2        = 0x41,
    LSTORE_3        = 0x42,
    FSTORE_0        = 0x43,
    FSTORE_1        = 0x44,
    FSTORE_2        = 0x45,
    FSTORE_3        = 0x46,
    DSTORE_0        = 0x47,
    DSTORE_1        = 0x48,
    DSTORE_2        = 0x49,
    DSTORE_3        = 0x4A,
    ASTORE_0        = 0x4B,
    ASTORE_1        = 0x4C,
    ASTORE_2        = 0x4D,
    ASTORE_3        = 0x4E,
    IASTORE         = 0x4F,
    LASTORE         = 0x50,
    FASTORE         = 0x51,
    DASTORE         = 0x52,
    AASTORE         = 0x53,
    BASTORE         = 0x54,
    CASTORE         = 0x55,
    SASTORE         = 0x56,
    POP             = 0x57,
    POP2            = 0x58,
    DUP             = 0x59,
    DUP_X1          = 0x5A,
    DUP_X2          = 0x5B,
    DUP2            = 0x5C,
    DUP2_X1         = 0x5D,
    DUP2_X2         = 0x5E,
    SWAP            = 0x5F,
    IADD            = 0x60,
    LADD            = 0x61,
    FADD            = 0x62,
    DADD            = 0x63,
    ISUB            = 0x64,
    LSUB            = 0x65,
    FSUB            = 0x66,
    DSUB            = 0x67,
    IMUL            = 0x68,
    LMUL            = 0x69,
    FMUL            = 0x6A,
    DMUL            = 0x6B,
    IDIV            = 0x6C,
    LDIV            = 0x6D,
    FDIV            = 0x6E,
    DDIV            = 0x6F,
    IREM            = 0x70,
    LREM            = 0x71,
    FREM            = 0x72,
    DREM            = 0x73,
    INEG            = 0x74,
    LNEG            = 0x75,
    FNEG            = 0x76,
    DNEG            = 0x77,
    ISHL            = 0x78,
    LSHL            = 0x79,
    ISHR            = 0x7A,
    LSHR            = 0x7B,
    IUSHR           = 0x7C,
    LUSHR           = 0x7D,
    IAND            = 0x7E,
    LAND            = 0x7F,
    IOR             = 0x80,
    LOR             = 0x81,
    IXOR            = 0x82,
    LXOR            = 0x83,
    IINC            = 0x84, // u8(w), s8(w)
    I2L             = 0x85,
    I2F             = 0x86,
    I2D             = 0x87,
    L2I             = 0x88,
    L2F             = 0x89,
    L2D             = 0x8A,
    F2I             = 0x8B,
    F2L             = 0x8C,
    F2D             = 0x8D,
    D2I             = 0x8E,
    D2L             = 0x8F,
    D2F             = 0x90,
    I2B             = 0x91,
    I2C             = 0x92,
    I2S             = 0x93,
    LCMP            = 0x94,
    FCMPL           = 0x95,
    FCMPG           = 0x96,
    DCMPL           = 0x97,
    DCMPG           = 0x98,
    IFEQ            = 0x99, // s16
    IFNE            = 0x9A, // s16
    IFLT            = 0x9B, // s16
    IFGE            = 0x9C, // s16
    IFGT            = 0x9D, // s16
    IFLE            = 0x9E, // s16
    IF_ICMPEQ       = 0x9F, // s16
    IF_ICMPNE       = 0xA0, // s16
    IF_ICMPLT       = 0xA1, // s16
    IF_ICMPGE       = 0xA2, // s16
    IF_ICMPGT       = 0xA3, // s16
    IF_ICMPLE       = 0xA4, // s16
    IF_ACMPEQ       = 0xA5, // s16
    IF_ACMPNE       = 0xA6, // s16
    GOTO            = 0xA7, // s16
    JSR             = 0xA8, // s16
    RET             = 0xA9, // u8(w)
    TABLESWITCH     = 0xAA, // align4, s32, s32, s32, v(rep="s32")
    LOOKUPSWITCH    = 0xAB, // bunch of BS
    IRETURN         = 0xAC,
    LRETURN         = 0xAD,
    FRETURN         = 0xAE,
    DRETURN         = 0xAF,
    ARETURN         = 0xB0,
    RETURN          = 0xB1,
    GETSTATIC       = 0xB2, // u16
    PUTSTATIC       = 0xB3, // u16
    GETFIELD        = 0xB4, // u16
    PUTFIELD        = 0xB5, // u16
    INVOKEVIRTUAL   = 0xB6, // u16
    INVOKESPECIAL   = 0xB7, // u16
    INVOKESTATIC    = 0xB8, // u16
    INVOKEINTERFACE = 0xB9, // u16, u8, 0
    INVOKEDYNAMIC   = 0xBA, // u16, 0, 0
    NEW             = 0xBB, // u16
    NEWARRAY        = 0xBC, // u8
    ANEWARRAY       = 0xBD, // u16
    ARRAYLENGTH     = 0xBE,
    ATHROW          = 0xBF,
    CHECKCAST       = 0xC0, // u16
    INSTANCEOF      = 0xC1, // u16
    MONITORENTER    = 0xC2,
    MONITOREXIT     = 0xC3,
    WIDE            = 0xC4, // prefix
    MULTIANEWARRAY  = 0xC5, // u16, u8
    IFNULL          = 0xC6, // s16
    IFNONNULL       = 0xC7, // s16
    GOTO_W          = 0xC8, // s32
    JSR_W           = 0xC9, // s32
	BREAKPOINT		= 0xCA,
    
    IMPDEP1         = 0xFE,
    IMPDEP2         = 0xFF
};

enum JavaMajorVersions : uint16_t {
    // IDGAF about older java versions
    JAVA_6 = 50,
    JAVA_7 = 51,
    JAVA_8 = 52,
    JAVA_9 = 53,
    JAVA_10 = 54,
    JAVA_11 = 55,
    JAVA_12 = 56,
    JAVA_13 = 57,
    JAVA_14 = 58,
    JAVA_15 = 59,
    JAVA_16 = 60,
    JAVA_17 = 61,
    JAVA_18 = 62,
    JAVA_19 = 63,
    JAVA_20 = 64,
    JAVA_21 = 65,
    JAVA_22 = 66
};

enum TypeSigil : char {
    TYPE_INT8 = 'B',
    TYPE_CHAR16 = 'C',
    TYPE_DOUBLE = 'D',
    TYPE_FLOAT = 'F',
    TYPE_INT = 'I',
    TYPE_LONG = 'J',
    TYPE_CLASS = 'L',
    TYPE_SHORT = 'S',
    TYPE_BOOL = 'Z',
    TYPE_ARRAY = '[',
    TYPE_VOID = 'V',
    TYPE_STRING = 's',
    TYPE_ENUM = 'e',
    TYPE_CLASS_INFO = 'c',
    TYPE_ANNOTATION = '@'
};

enum ConstantType : uint8_t {
    CONSTANT_UTF8_STRING = 1,
    //
    CONSTANT_INT32 = 3,
    CONSTANT_FLOAT = 4,
    CONSTANT_INT64 = 5,
    CONSTANT_DOUBLE = 6,
    CONSTANT_CLASS = 7,
    CONSTANT_UTF16_STRING = 8,
    CONSTANT_FIELD_REF = 9,
    CONSTANT_METHOD_REF = 10,
    CONSTANT_INTERFACE_METHOD_REF = 11,
    CONSTANT_NAME_AND_TYPE = 12,
    //
    CONSTANT_METHOD_HANDLE = 15,
    CONSTANT_METHOD_TYPE = 16,
    CONSTANT_DYNAMIC = 17,
    CONSTANT_INVOKE_DYNAMIC = 18,
    CONSTANT_MODULE = 19,
    CONSTANT_PACKAGE = 20
};

// Check alignment of this
struct ClassInfo {
    ConstantType tag; // must be CONSTANT_CLASS
    uint16_t name_index;
};

struct JavaFlags {
    union {
        uint16_t flags;
        struct {
            uint16_t declared_public        : 1; // 0  0001 Class/Field/Method
            uint16_t declared_private       : 1; // 1  0002 Field/Method
            uint16_t declared_protected     : 1; // 2  0004 Field/Method
            uint16_t declared_static        : 1; // 3  0008 Field/Method
            uint16_t declared_final         : 1; // 4  0010 Class/Field/Method
            uint16_t declared_synchronized  : 1; // 5  0020 Method
            uint16_t bridge_method          : 1; // 6  0040 Method
            uint16_t declared_volatile      : 1; // 7  0080 Field
            uint16_t declared_transient     : 1; // 8  0100 Field
            uint16_t interface              : 1; // 9  0200 Class
            uint16_t declared_abstract      : 1; // 10 0400 Class/Method
            uint16_t strict_float           : 1; // 11 0800 Method
            uint16_t declared_synthetic     : 1; // 12 1000 Class/Field/Method/Module/ModuleRequires/ModuleExports/ModuleOpens
            uint16_t annotation             : 1; // 13 2000 Class
            uint16_t declared_enum          : 1; // 14 4000 Class/Field
            uint16_t declared_module        : 1; // 15 8000 Class
        };
        struct {
            uint16_t                        : 5; // 0-4
            uint16_t special_super          : 1; // 5  0020 Class
            uint16_t compile_time_requires  : 1; // 6  0040 ModuleRequires
            uint16_t variadic_method        : 1; // 7  0080 Method
            uint16_t native_method          : 1; // 8  0100 Method
            uint16_t                        : 7; // 9-14
            uint16_t implicit_module        : 1; // 15 8000 Module/ModuleRequires/ModuleExports/ModuleOpens
        };
        struct {
            uint16_t                        : 5; // 0-4
            uint16_t open_module            : 1; // 5  0020 Module
            uint16_t                        : 0;
        };
        struct {
            uint16_t                        : 5; // 0-4
            uint16_t transitive_dependency  : 1; // 5  0020 ModuleRequires
            uint16_t                        : 0;
        };
    };
};

struct FieldInfo {
    
};

struct ClassFileHeader {
    uint32_t magic;
    uint16_t minor_version;
    uint16_t major_version;
};

struct ClassFileData {
    ClassFileData* super_class;
    uint32_t constant_pool_size; // uint16_t in file
    void* constant_pool_ptr;
    union {
        uint32_t flags;
        struct {
            uint32_t declared_public : 1; // 0
            uint32_t : 3; // 1-3
            uint32_t declared_final : 1; // 4
            uint32_t special_super : 1; // 5
            uint32_t : 3; // 6-8
            uint32_t interface : 1; // 9
            uint32_t declared_abstract : 1; // 10
            uint32_t : 1; // 11
            uint32_t declared_synthetic : 1; // 12
            uint32_t annotation : 1; // 13
            uint32_t declared_enum : 1; // 14
            uint32_t declared_module : 1; // 15
            uint32_t : 16; // 16-31
        };
    };
    
    
    static ClassFileData* parse_class_from_file_buffer(void* file) {
        
    }
};