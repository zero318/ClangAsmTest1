// size: 0x4
struct RngImpl {
    uint32_t rng_value = 1; // 0x0
    // 0x4
    
    // trial:   Rx22D90
    // release: Rx22C30
    RngImpl() {
        this->rng_value = validate_seed(1);
    }
    
    // trial:   Rx22D60
    // release: Rx22C00
    static uint32_t validate_seed(uint32_t value) {
        value %= INT32_MAX;
        if (!value) {
            value = 1;
        }
        return value;
    }
    
    // trial:   Rx22E90
    // release: Rx22CB0
    static uint32_t generate_random(uint32_t seed) {
        uint64_t temp = 0xBC8Full * seed;
#if BEFORE_TH20_RELEASE
        return temp % INT32_MAX;
#else
        temp = (temp >> 31) + (temp & INT32_MAX);
        uint64_t value;
        if (temp < INT32_MAX) {
            value = temp;
        } else {
            value = temp - INT32_MAX;
        }
        temp = value;
        return temp;
#endif
    }
    
    // trial:   RxE03F0
    // release: RxDDA10
    void initialize(uint32_t seed) {
        this->rng_value = validate_seed(seed);
    }
    
    // trial:   Rx23990
    // release: Rx22CB0
    uint32_t rand_uint() {
        this->rng_value = generate_random(this->rng_value);
        return this->rng_value;
    }
};

// size: 0x18
struct Rng {
    uint32_t index = 0; // 0x0
    RngImpl impl; // 0x4
    uint32_t minimum = 0; // 0x8
    uint32_t maximum = 0x00FFFF00; // 0xC
    uint32_t range = 0; // 0x10
    uint32_t prev_value = 0; // 0x14
    uint32_t __generator_index; // 0x18
    // 0x1C
    
    // trial:   Rx22DB0
    // release: Rx22C50
    Rng(uint32_t index) {
        this->__generator_index = index;
    }
    
    // trial:   RxDC450
    // release: RxD9940
    void initialize(uint32_t seed) {
        std::scoped_lock<std::mutex> lock(CRITICAL_SECTION_MANAGER.get_mutex(RNG_MUTEX));
        this->minimum = 0;
        this->maximum = std::numeric_limits<uint32_t>::max() / 2;
        this->range = this->maximum - this->minimum;
        RngImpl* impl = &this->impl;
        impl->initialize(seed);
    }
    
    // trial:   Rx49970
    // release: Rx49D20
    uint8_t rand_byte() {
        std::scoped_lock<std::mutex> lock(CRITICAL_SECTION_MANAGER.get_mutex(RNG_MUTEX));
        this->prev_value = this->impl.rand_uint();
        return this->prev_value;
    }
    
    // trial:   Rx9A7B0
    // release: Rx97470
    uint16_t rand_ushort() {
        std::scoped_lock<std::mutex> lock(CRITICAL_SECTION_MANAGER.get_mutex(RNG_MUTEX));
        this->prev_value = this->impl.rand_uint();
        return this->prev_value;
    }
    
    // trial:   Rx24280
    // release: Rx23EE0
    uint32_t rand_uint() {
        std::scoped_lock<std::mutex> lock(CRITICAL_SECTION_MANAGER.get_mutex(RNG_MUTEX));
        this->prev_value = this->impl.rand_uint();
        return this->prev_value % this->range;
    }
    
    // trial:   Rx24240
    // release: Rx23EA0
    uint32_t rand_uint_range(uint32_t range) {
        if (range) {
            return this->rand_uint() % range;
        } else {
            return 0;
        }
    }
    
    // trial:   Rx29110
    // release: Rx29830
    float rand_float() {
        float value = this->rand_uint();
        float range = (float)this->range - 1.0f;
        return value / range;
    }
    
    // trial:   Rx290E0
    // release: Rx29800
    float rand_float_range(float range) {
        return this->rand_float() * range;
    }
    
    // trial:   Rx291C0
    // release: Rx298E0
    float rand_float_signed() {
        float value = this->rand_uint();
        float range = (float)this->range / 2.0f - 1.0f;
        return (value / range) - 1.0f;
    }
    
    // trial:   Rx29190
    // release: Rx298B0
    float rand_float_signed_range(float range) {
        return this->rand_float_signed() * range;
    }
    
    // trial:   Rx290B0
    // release: Rx298D0
    float rand_angle() {
        return this->rand_float_signed() * PI_f;
    }
    
    // trial:   Rx1079C0
    // release: Rx104A60
    float rand_angle_2() {
        float value = this->rand_uint();
        float range = (float)this->range / (PI_f * 2.0f) - PI_f;
        return (value / range) - PI_f;
    }
};
