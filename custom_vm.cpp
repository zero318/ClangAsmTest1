struct VMStack {
    uint32_t pointer;
    uint32_t base;
    //uint32_t stack_limit;
    
    StackData* raw;
    
    VMStack(uint32_t size) : base(0), pointer(0) {
        this->raw = new StackData[size];
    }
    
    template<typename T>
    inline T read_offset(uint32_t offset) const {
        return *(T*)&this->raw[offset];
    }
    template<typename T>
    inline void write_offset(uint32_t offset, const T& value) {
        *(T*)&this->raw[offset] = value;
    }
    template<typename T>
    inline T& ref_offset(uint32_t offset) const {
        return *(T*)&this->raw[offset];
    }
    
    template<typename T>
    inline T read_local(int32_t offset, uint32_t base_index) const {
        return this->read_offset<T>(base_index + offset);
    }
    template<typename T>
    inline T read_local(int32_t offset) const {
        return this->read_local<T>(offset, this->base);
    }
    
    template<typename T>
    inline T read_temp(uint32_t offset, uint32_t top_index) const {
        return this->read_offset<T>(top_index - offset);
    }
    template<typename T>
    inline T read_temp(uint32_t offset) const {
        return this->read_temp<T>(offset, this->pointer);
    }
    
    template<typename T>
    inline void write_local(int32_t offset, const T& value, uint32_t base_index) {
        this->write_offset<T>(base_index + offset, value);
    }
    template<typename T>
    inline void write_local(int32_t offset, const T& value) {
        this->write_offset<T>(offset, value, this->base);
    }
    
    template<typename T>
    inline void write_temp(uint32_t offset, const T& value, uint32_t top_index) {
        this->write_offset<T>(top_index - offset, value);
    }
    template<typename T>
    inline void write_temp(uint32_t offset, const T& value) {
        this->write_temp<T>(offset, value, this->pointer);
    }
    
    template<typename T>
    inline T& ref_local(int32_t offset, uint32_t base_index) const {
        return this->ref_offset<T>(base_index + offset);
    }
    template<typename T>
    inline T& ref_local(int32_t offset) const {
        return this->ref_local<T>(offset, this->base);
    }
    
    template<typename T>
    inline T& ref_temp(uint32_t offset, uint32_t top_index) const {
        return this->ref_offset<T>(top_index - offset);
    }
    template<typename T>
    inline T& ref_temp(uint32_t offset) const {
        return this->ref_temp<T>(offset, this->pointer);
    }
    
    template<typename T>
    inline void push(uint32_t& starting_offset, const T& value) {
        this->write_offset<T>(starting_offset++, value);
    }
    template<typename T>
    inline void push(const T& value) {
        this->push(this->pointer, value);
    }
    
    template<typename T>
    inline T pop(uint32_t& starting_offset) {
        return this->read_offset<T>(--starting_offset);
    }
    template<typename T>
    inline T pop() {
        return this->pop<T>(this->pointer);
    }
    
    template<typename L> requires(FunctionTraitsType<L>::arg_count == 1)
    void unary_op(const L& func) {
        using T = typename FunctionTraitsType<L>::template nth_arg_type<0>;
        using P = typename FunctionTraitsType<L>::ret_type;
        
        T& stack_val = this->ref_temp<T>(0);
        *(P*)&stack_val = func(stack_val);
    }
    
    template<typename L> requires(FunctionTraitsType<L>::arg_count == 2 && std::is_same_v<typename FunctionTraitsType<L>::template nth_arg_type<0>, typename FunctionTraitsType<L>::template nth_arg_type<1>>)
    void binary_op(const L& func) {
        using T = typename FunctionTraitsType<L>::template nth_arg_type<0>;
        using P = typename FunctionTraitsType<L>::ret_type;
        T right = this->pop<T>();
        T& left = this->ref_temp<T>(0);
        *(P*)&left = func(left, right);
    }
};

union InsWord {
    struct {
        uint8_t opcode;
        union {
            int8_t arg_s8;
            uint8_t arg_u8;
        };
    };
    int16_t arg_s16;
    uint16_t arg_u16;
};

union StackData : uint32_t {
    int32_t s32;
    uint32_t u32;
    float f32;
};

enum PrimaryOpcodes : uint8_t {
    NOARGS  = 0x00,
    ONEWORD = 0x01,
    TWOWORD = 0x02,
    
    JMP     = 0x10,
    JZ      = 0x11,
    JNZ     = 0x12,
    
};

enum NoArgsOpcodes : uint8_t {
    ADD     = 0x00,
    SUB     = 0x01,
    MUL     = 0x02,
    IDIV    = 0x03,
    UDIV    = 0x04,
    IMOD    = 0x05,
    UMOD    = 0x06,
    SHL     = 0x07,
    SHR     = 0x08,
    SAR     = 0x09,
    ROL     = 0x0A,
    ROR     = 0x0B,
    OR      = 0x0C,
    AND     = 0x0D,
    XOR     = 0x0E,
    EQU     = 0x0F,
    NEQ     = 0x10,
    LES     = 0x11,
    LEQ     = 0x12,
    GTR     = 0x13,
    GEQ     = 0x14,
    BLW     = 0x15,
    BEQ     = 0x16,
    ABV     = 0x17,
    AEQ     = 0x18,
    FADD    = 0x19,
    FSUB    = 0x1A,
    FMUL    = 0x1B,
    FDIV    = 0x1C,
    FMOD    = 0x1D,
    FREM    = 0x1E,
    FDIM    = 0x1F,
    FPOW    = 0x20,
    FATAN2  = 0x21,
    FEQU    = 0x22,
    FNEQ    = 0x23,
    FLES    = 0x24,
    FLEQ    = 0x25,
    FGTR    = 0x26,
    FGEQ    = 0x27,
    B2S     = 0x28,
    W2S     = 0x29,
    B2U     = 0x2A,
    W2U     = 0x2B,
    S2F     = 0x2C,
    U2F     = 0x2D,
    F2S     = 0x2E,
    F2U     = 0x2F,
    NEG     = 0x30,
    INV     = 0x31,
    NOT     = 0x32,
    EQUZ    = 0x33,
    NEQZ    = 0x34,
    LESZ    = 0x35,
    LEQZ    = 0x36,
    GTRZ    = 0x37,
    GEQZ    = 0x38,
    FNEG    = 0x39,
    FNOT    = 0x3A,
    FSIN    = 0x3B,
    FCOS    = 0x3C,
    FTAN    = 0x3D,
    FASIN   = 0x3E,
    FACOS   = 0x3F,
    FATAN   = 0x40,
    FSQRT   = 0x41,
    FFLOOR  = 0x42,
    FCEIL   = 0x43,
    FROUND  = 0x44,
    FTRUNC  = 0x45,
    
    ALLOC   = 0x80,
    FREE    = 0x81,
    
};

struct VMContext {
    uint32_t instruction_pointer;
    InsWord* address_space;
    VMStack stack;
    
    void run() {
        InsWord *const address_space = this->address_space;
        StackData *const stack_space = this->stack_space;
        
        for ( ; ; ++this->instruction_pointer) {
            InsWord* current_instruction = address_space[this->instruction_pointer];
            InsWord instruction = *current_instruction;
            switch (instruction.opcode) {
                case NOARGS:
                    switch (instruction.arg_u8) {
                        case ADD:
                            this->stack.binary_op([](uint32_t lhs, uint32_t rhs){
                                return lhs + rhs;
                            });
                            break;
                        case SUB:
                            this->stack.binary_op([](uint32_t lhs, uint32_t rhs){
                                return lhs - rhs;
                            });
                            break;
                        case MUL:
                            this->stack.binary_op([](int32_t lhs, int32_t rhs){
                                return lhs * rhs;
                            });
                            break;
                        case IDIV:
                            this->stack.binary_op([](int32_t lhs, int32_t rhs){
                                return lhs / rhs;
                            });
                            break;
                        case UDIV:
                            this->stack.binary_op([](uint32_t lhs, uint32_t rhs){
                                return lhs / rhs;
                            });
                            break;
                        case IMOD:
                            this->stack.binary_op([](int32_t lhs, int32_t rhs){
                                return lhs % rhs;
                            });
                            break;
                        case UMOD:
                            this->stack.binary_op([](uint32_t lhs, uint32_t rhs){
                                return lhs % rhs;
                            });
                            break;
                        case SHL:
                            this->stack.binary_op([](uint32_t lhs, uint32_t rhs){
                                return lhs << rhs;
                            });
                            break;
                        case SHR:
                            this->stack.binary_op([](uint32_t lhs, uint32_t rhs){
                                return lhs >> rhs;
                            });
                            break;
                        case SAR:
                            this->stack.binary_op([](int32_t lhs, int32_t rhs){
                                return lhs >> rhs;
                            });
                            break;
                        case ROL:
                            this->stack.binary_op([](uint32_t lhs, uint32_t rhs){
                                return std::rotl(lhs, rhs);
                            });
                            break;
                        case ROR:
                            this->stack.binary_op([](uint32_t lhs, uint32_t rhs){
                                return std::rotr(lhs, rhs);
                            });
                            break;
                        case OR:
                            this->stack.binary_op([](uint32_t lhs, uint32_t rhs){
                                return lhs | rhs;
                            });
                            break;
                        case AND:
                            this->stack.binary_op([](uint32_t lhs, uint32_t rhs){
                                return lhs & rhs;
                            });
                            break;
                        case XOR:
                            this->stack.binary_op([](uint32_t lhs, uint32_t rhs){
                                return lhs ^ rhs;
                            });
                            break;
                        case EQU:
                            this->stack.binary_op([](uint32_t lhs, uint32_t rhs){
                                return lhs == rhs;
                            });
                            break;
                        case NEQ:
                            this->stack.binary_op([](uint32_t lhs, uint32_t rhs){
                                return lhs != rhs;
                            });
                            break;
                        case LES:
                            this->stack.binary_op([](int32_t lhs, int32_t rhs){
                                return lhs < rhs;
                            });
                            break;
                        case LEQ:
                            this->stack.binary_op([](int32_t lhs, int32_t rhs){
                                return lhs <= rhs;
                            });
                            break;
                        case GTR:
                            this->stack.binary_op([](int32_t lhs, int32_t rhs){
                                return lhs > rhs;
                            });
                            break;
                        case GEQ:
                            this->stack.binary_op([](int32_t lhs, int32_t rhs){
                                return lhs >= rhs;
                            });
                            break;
                        case BLW:
                            this->stack.binary_op([](uint32_t lhs, uint32_t rhs){
                                return lhs < rhs;
                            });
                            break;
                        case BEQ:
                            this->stack.binary_op([](uint32_t lhs, uint32_t rhs){
                                return lhs <= rhs;
                            });
                            break;
                        case ABV:
                            this->stack.binary_op([](uint32_t lhs, uint32_t rhs){
                                return lhs > rhs;
                            });
                            break;
                        case AEQ:
                            this->stack.binary_op([](uint32_t lhs, uint32_t rhs){
                                return lhs >= rhs;
                            });
                            break;
                        case FADD:
                            this->stack.binary_op([](float lhs, float rhs){
                                return lhs + rhs;
                            });
                            break;
                        case FSUB:
                            this->stack.binary_op([](float lhs, float rhs){
                                return lhs - rhs;
                            });
                            break;
                        case FMUL:
                            this->stack.binary_op([](float lhs, float rhs){
                                return lhs * rhs;
                            });
                            break;
                        case FDIV:
                            this->stack.binary_op([](float lhs, float rhs){
                                return lhs / rhs;
                            });
                            break;
                        case FMOD:
                            this->stack.binary_op([](float lhs, float rhs){
                                return fmodf(lhs, rhs);
                            });
                            break;
                        case FREM:
                            this->stack.binary_op([](float lhs, float rhs){
                                return remainderf(lhs, rhs);
                            });
                            break;
                        case FDIM:
                            this->stack.binary_op([](float lhs, float rhs){
                                return fdimf(lhs, rhs);
                            });
                            break;
                        case FPOW:
                            this->stack.binary_op([](float lhs, float rhs){
                                return powf(lhs, rhs);
                            });
                            break;
                        case FATAN2:
                            this->stack.binary_op([](float lhs, float rhs){
                                return atan2f(lhs, rhs);
                            });
                            break;
                        case FEQU:
                            this->stack.binary_op([](float lhs, float rhs){
                                return lhs == rhs;
                            })
                            break;
                        case FNEQ:
                            this->stack.binary_op([](float lhs, float rhs){
                                return lhs != rhs;
                            });
                            break;
                        case FLES:
                            this->stack.binary_op([](float lhs, float rhs){
                                return lhs < rhs;
                            });
                            break;
                        case FLEQ:
                            this->stack.binary_op([](float lhs, float rhs){
                                return lhs <= rhs;
                            });
                            break;
                        case FGTR:
                            this->stack.binary_op([](float lhs, float rhs){
                                return lhs > rhs;
                            });
                            break;
                        case FGEQ:
                            this->stack.binary_op([](float lhs, float rhs){
                                return lhs >= rhs;
                            });
                            break;
                        case B2S:
                            this->stack.unary_op([](int32_t val)->int32_t{
                                return (int8_t)val;
                            });
                            break;
                        case W2S:
                            this->stack.unary_op([](int32_t val)->int32_t{
                                return (int16_t)val;
                            });
                            break;
                        case B2U:
                            this->stack.unary_op([](uint32_t val)->uint32_t{
                                return (uint8_t)val;
                            });
                            break;
                        case W2U:
                            this->stack.unary_op([](uint32_t val)->uint32_t{
                                return (uint16_t)val;
                            });
                            break;
                        case S2F:
                            this->stack.unary_op([](int32_t val)->float{
                                return val;
                            });
                            break;
                        case U2F:
                            this->stack.unary_op([](uint32_t val)->float{
                                return val;
                            });
                            break;
                        case F2S:
                            this->stack.unary_op([](float val)->int32_t{
                                return val;
                            });
                            break;
                        case F2U:
                            this->stack.unary_op([](float val)->uint32_t{
                                return val;
                            });
                            break;
                        case NEG:
                            this->stack.unary_op([](int32_t val){
                                return -val;
                            });
                            break;
                        case INV:
                            this->stack.unary_op([](uint32_t val){
                                return ~val;
                            });
                            break;
                        case NOT:
                            this->stack.unary_op([](uint32_t val){
                                return !val;
                            });
                            break;
                        case EQUZ:
                            this->stack.unary_op([](uint32_t val){
                                return val == 0;
                            });
                            break;
                        case NEQZ:
                            this->stack.unary_op([](uint32_t val){
                                return val != 0;
                            });
                            break;
                        case LESZ:
                            this->stack.unary_op([](int32_t val){
                                return val < 0;
                            });
                            break;
                        case LEQZ:
                            this->stack.unary_op([](int32_t val){
                                return val <= 0;
                            });
                            break;
                        case GTRZ:
                            this->stack.unary_op([](int32_t val){
                                return val > 0;
                            });
                            break;
                        case GEQZ:
                            this->stack.unary_op([](int32_t val){
                                return val >= 0;
                            });
                            break;
                        case FNEG:
                            this->stack.unary_op([](float val){
                                return -val;
                            });
                            break;
                        case FNOT:
                            this->stack.unary_op([](float val)->uint32_t{
                                return !val;
                            });
                            break;
                        case FSIN:
                            this->stack.unary_op([](float val){
                                return sinf(val);
                            });
                            break;
                        case FCOS:
                            this->stack.unary_op([](float val){
                                return cosf(val);
                            });
                            break;
                        case FTAN:
                            this->stack.unary_op([](float val){
                                return tanf(val);
                            });
                            break;
                        case FASIN:
                            this->stack.unary_op([](float val){
                                return asinf(val);
                            });
                            break;
                        case FACOS:
                            this->stack.unary_op([](float val){
                                return acosf(val);
                            });
                            break;
                        case FATAN:
                            this->stack.unary_op([](float val){
                                return atanf(val);
                            });
                            break;
                        case FSQRT:
                            this->stack.unary_op([](float val){
                                return sqrtf(val);
                            });
                            break;
                        case FFLOOR:
                            this->stack.unary_op([](float val){
                                return floorf(val);
                            });
                            break;
                        case FCEIL:
                            this->stack.unary_op([](float val){
                                return ceilf(val);
                            });
                            break;
                        case FTRUNC:
                            this->stack.unary_op([](float val){
                                return truncf(val);
                            });
                            break;
                        case FROUND:
                            this->stack.unary_op([](float val){
                                return roundf(val);
                            });
                            break;
                            
                        case ALLOC:
                            this->stack.unary_op
                    }
                    break;
                case ONEWORD:
                    break;
                case TWOWORD:
                    
            }
        }
    }
};