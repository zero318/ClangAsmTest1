// size: 0x4
struct RngImpl {
    uint32_t rng_value = 1; // 0x0
    // 0x4
    
    inline void initialize(uint32_t seed) {
        seed %= INT32_MAX;
        this->rng_value = seed ? seed : 1;
    }
    
    // Rx18C0
    uint32_t rand_uint() {
        uint64_t temp = 0xBC8Full * this->rng_value;
        temp %= INT32_MAX;
        this->rng_value = temp;
        return temp;
    }
};

// size: 0x18
struct Rng {
    uint32_t __dword_0; // 0x0
    RngImpl impl; // 0x4
    uint32_t __dword_8; // 0x8
    uint32_t __uint_C = 0x00FFFF00; // 0xC
    uint32_t range; // 0x10
    uint32_t prev_value; // 0x14
    // 0x18
    
    inline void initialize(uint32_t seed) {
        this->__dword_8 = 0;
        this->__uint_C = 0x00FFFF00;
        this->prev_value = seed;
        this->range = 0x00FFFF00;
        this->impl.initialize(seed);
    }
    
    // Rx1960
    uint16_t rand_ushort() {
        std::scoped_lock<std::mutex> lock(RNG_MUTEX);
        uint32_t value = this->impl.rand_uint();
        this->prev_value = value;
        return value;
    }
    
    // Rx2540
    uint32_t rand_uint() {
        std::scoped_lock<std::mutex> lock(RNG_MUTEX);
        uint32_t value = this->impl.rand_uint();
        this->prev_value = value;
        return value % this->range;
    }
    
    // Rx2630
    uint32_t rand_uint_range(uint32_t range) {
        if (range) {
            return this->rand_uint() % range;
        } else {
            return 0;
        }
    }
    
    // Rx42B0
    float rand_angle() {
        return this->rand_float_signed_range(PI_f);
    }
    
    // Rx4B70
    float rand_float_signed() {
        float value = this->rand_uint();
        float range = (float)this->range / 2.0f - 1.0f;
        return (value / range) - 1.0f;
    }
    
    // Rx4BD0
    float rand_float_signed_range(float range) {
        return this->rand_float_signed() * range;
    }
    
    // Rx4C40
    float rand_float() {
        float value = this->rand_uint();
        float range = (float)this->range - 1.0f;
        return value / range;
    }
    
    // Rx4C90
    float rand_float_range(float range) {
        return this->rand_float() * range;
    }
};

// Rx1C65F0
Rng RNG_B;
// Rx1C6608
Rng RNG_C;
// Rx1C6620
Rng RNG_A;
// Rx1C6638
Rng RNG_D;