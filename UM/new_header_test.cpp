//#include "..\zero\util.h"

#include <intrin.h>

#include "..\zero\th_common.h"

#define NoUnroll _Pragma("clang loop unroll(disable)")
#define nounroll NoUnroll

#define expect_chance __builtin_expect_with_probability

#define countof(array_type) \
(sizeof(array_type) / sizeof(array_type[0]))

#if __has_attribute(ext_vector_type)
template <typename T, size_t count, bool is_aligned>
struct _evec_impl {
    using type gnu_attr(ext_vector_type(count), __aligned__(alignof(T))) = T;
};
template <typename T, size_t count>
struct _evec_impl<T, count, true> {
    using type gnu_attr(ext_vector_type(count)) = T;
};
template <typename T, size_t count, bool is_aligned = false>
using evec = typename _evec_impl<T, count, is_aligned>::type;
#else
template <typename T, size_t count, bool is_aligned = false>
using evec = vec<T, count, is_aligned>;
#endif


__declspec(dllexport) void pingas(ZUNListRoot<ZUNInterp<Float3>>& list_head) {
	list_head.for_each([](auto* interp) {
		interp->step();
	});
}

// size: 0x1C
// D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 (0x144)
struct SpriteVertex {
    Float3 position; // 0x0
    float w;
    union {
        uint32_t diffuse; // 0x10
        struct {
            uint8_t diffuse_b;
            uint8_t diffuse_g;
            union {
                uint16_t diffuse_high;
                struct {
                    uint8_t diffuse_r;
                    uint8_t diffuse_a;
                };
            };
        };
    };

    Float2 texture_uv; // 0x14
    // 0x1C
    //static constexpr uint32_t FVF_TYPE = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
};

gnu_attr(used) void regcall vertex_test(SpriteVertex* vertices, const Float2& player_pos, const Float2& camera_pos) {
    Float2 position = player_pos + camera_pos + 12.0f;

    vec<float, 4> position_wide = { position.x, position.y, 0.0f, 0.0f };
    position_wide[0] += 192.0f;

    position_wide = shufflevec(position_wide, position_wide, 0, 1, 0, 1);

    vec<float, 4> vert_positionsA = { vertices[0].position.x, vertices[0].position.y, vertices[1].position.x, vertices[1].position.y };
    vec<float, 4> vert_positionsB = { vertices[2].position.x, vertices[2].position.y, vertices[3].position.x, vertices[3].position.y };

    vec<float, 4> tempA = vert_positionsA - position_wide;
    vec<float, 4> tempB = vert_positionsB - position_wide;

    tempA *= tempA;
    tempB *= tempB;

    vec<float, 4> distance = { tempA[0] + tempA[1], tempA[2] + tempA[3], tempB[0] + tempB[1], tempB[2] + tempB[3] };

    distance *= 1.0f / 8192.0f;

    vec<uint16_t, 8> colors_in = {
        vertices[0].diffuse_high, 0,
        vertices[1].diffuse_high, 0,
        vertices[2].diffuse_high, 0,
        vertices[3].diffuse_high, 0
    };

    vec<uint32_t, 4> colors = std::bit_cast<vec<uint32_t, 4>>(colors_in);

    vec<int32_t, 4> alphas = colors >>= 8;

    alphas = convertvec(convertvec(alphas, vec<float, 4>) * __builtin_elementwise_min(distance, (vec<float, 4>){ 1.0f, 1.0f, 1.0f, 1.0f }), vec<int32_t, 4>);
    
    uint16_t temp;
    temp = alphas[0];
    __asm__(""::"r"(temp));
    vertices[0].diffuse_a = temp;
    temp = alphas[1];
    __asm__(""::"r"(temp));
    vertices[1].diffuse_a = temp;
    temp = alphas[2];
    __asm__(""::"r"(temp));
    vertices[2].diffuse_a = temp;
    temp = alphas[3];
    __asm__(""::"r"(temp));
    vertices[3].diffuse_a = temp;
}

struct Player {
    //dummy_fields(0x20E4);
    unsigned char dummy[0x20E4];
    int32_t unfocused_linear_speed;
    int32_t focused_linear_speed;
    int32_t unfocused_diagonal_speed;
    int32_t focused_diagonal_speed;
};

struct GameSide {
    void* bullet_manager_ptr;
    Player* player_ptr;
};

struct CPUCollider {
    Float3 position;
    Float3 __float3_C;
    Float2 __hitbox_size;
    float __hitbox_radius;
    float __angle_24;
    float __angle_28;
    float __float_2C;
    int32_t __life;
    void* __entity_ptr;
    union {
        uint32_t flags;
        struct {
            uint32_t __flag_A : 1;
        };
    };
};

struct CPUHitInf {
    void* vftable; // 0x0
    CPUCollider __collider_array_4[30000];
    CPUCollider __collider_array_1B7740[30000];
    int32_t __collider_count;
    Float3 __float3_36EE88;
    int __int_36EE94;
    int32_t __int_36EE98;
    int32_t __int_36EE9C;
    int32_t __int_36EEA0;
    int32_t __int_36EEA4;
    int __int_36EEA8;
    int __dword_36EEAC;
    int __int_36EEB0;
    int __int_36EEB4;
    int __int_36EEB8;
    int __int_36EEBC;
    int __int_36EEC0;
    uint32_t side_index;
    GameSide* game_side_ptr;
};

__declspec(dllexport) uint32_t* vectorcall check_colliders(const CPUHitInf* self, int, uint32_t* hit_flags_ptr, float, float, float radius) {
    
    uint32_t hit_flags = 0;
    //__asm__("":"+r"(hit_flags));
    size_t collider_count = self->__collider_count;
    if (!collider_count) goto end;

    const Player* player_ptr = self->game_side_ptr->player_ptr;
    vec<int32_t, 4> speed_vec_int = {
        player_ptr->unfocused_linear_speed,
        player_ptr->focused_linear_speed,
        player_ptr->unfocused_diagonal_speed,
        player_ptr->focused_diagonal_speed
    };

    vec<float, 4> speed_vec = convertvec(speed_vec_int, vec<float, 4>) * 0.0078125f;

    vec<float, 2> self_pos = self->__float3_36EE88.as_vec2();
    vec<float, 4> self_pos_wide = { self_pos[0], self_pos[1], 0.0f, 0.0f };

    auto CLAMP = [](vec<float, 4> val) {
        constexpr vec<float, 4> max_clamp = { 140.0f, 432.0f, 140.0f, 432.0f };
        val = val < max_clamp ? val : max_clamp;
        constexpr vec<float, 4> min_clamp = { -140.0f, 32.0f, -140.0f, 32.0f };
        return val > min_clamp ? val : min_clamp;
    };

    vec<float, 4> temp_vec = CLAMP(self_pos_wide);

    vec<float, 2> check_pos_list[18];
    //double check_pos_list[18];

//#define WRITE_POS_LIST(i, val) = 

    vec<float, 2> temp_vec_narrow = *(vec<float, 2>*)&temp_vec;
    check_pos_list[0] = temp_vec_narrow;
    check_pos_list[9] = temp_vec_narrow;
    
    self_pos_wide = shufflevec(self_pos_wide, self_pos_wide, 0, 1, 0, 1);

    //constexpr vec<uint32_t, 4> neg_mask = { 0u, 0x80000000u, 0u, 0x80000000u };
    constexpr vec<float, 4> neg_mask = { 0.0f, -0.0f, 0.0f, -0.0f };

    vec<float, 4> diagonal_speed_vec = shufflevec(speed_vec, speed_vec, 2, 2, 3, 3);

//#define NEG13(val) val = ((vec<float, 4>)((vec<uint32_t, 4>)(val) ^ neg_mask))
//#define NEG13(val) val = ((val) * neg_mask)
//#define NEG13(val) val = _mm_xor_ps(val, neg_mask)
#define NEG13(val) __asm__("xorps %[neg_mask], %[valr]":[valr]"+x"(val):[neg_mask]"x"(neg_mask))
    
    
    NEG13(diagonal_speed_vec);
    
    temp_vec = shufflevec(diagonal_speed_vec, diagonal_speed_vec, 1, 1, 0, 1);

    *(vec<float, 4>*)&check_pos_list[5] = CLAMP(self_pos_wide + temp_vec);
    NEG13(temp_vec);

    *(vec<float, 4>*)&check_pos_list[7] = CLAMP(self_pos_wide + temp_vec);

    temp_vec = shufflevec(diagonal_speed_vec, diagonal_speed_vec, 3, 3, 2, 3);

    *(vec<float, 4>*)&check_pos_list[14] = CLAMP(self_pos_wide + temp_vec);
    NEG13(temp_vec);

    *(vec<float, 4>*)&check_pos_list[16] = CLAMP(self_pos_wide + temp_vec);

    vec<float, 4> linear_speed_vec = shufflevec(speed_vec, speed_vec, 0, 0, 1, 1);
    NEG13(linear_speed_vec);

    vec<float, 4> zero_vec = {};

    temp_vec = shufflevec(linear_speed_vec, zero_vec, 4, 1, 4, 0);
    *(vec<float, 4>*)&check_pos_list[1] = CLAMP(self_pos_wide + temp_vec);
    temp_vec = shufflevec(temp_vec, temp_vec, 1, 2, 3, 0);
    *(vec<float, 4>*)&check_pos_list[3] = CLAMP(self_pos_wide + temp_vec);

    temp_vec = shufflevec(linear_speed_vec, zero_vec, 4, 3, 4, 2);
    *(vec<float, 4>*)&check_pos_list[10] = CLAMP(self_pos_wide + temp_vec);
    temp_vec = shufflevec(temp_vec, temp_vec, 1, 2, 3, 0);
    *(vec<float, 4>*)&check_pos_list[12] = CLAMP(self_pos_wide + temp_vec);

    
    vec<float, 4> radius_vec = { radius, 0.0f, radius, 0.0f };
    float radius_squared = radius_vec[0] * radius_vec[0];
    vec<float, 4> radius_squared_vec = { radius_squared, radius_squared, radius_squared, radius_squared };

    const CPUCollider* collider_ptr = self->__collider_array_4;

    for (
        ;
        collider_count && hit_flags != 0x3FFFF;
        --collider_count, ++collider_ptr
    ) {

        vec<float, 2> collider_pos = collider_ptr->position.as_vec2();

        //volatile vec<float, 2>* check_pos_list_ptr = check_pos_list;
        vec<float, 2>* check_pos_list_ptr = check_pos_list;
        //double* check_pos_list_ptr = (double*)check_pos_list;

        if (expect_chance(collider_ptr->__flag_A, true, 0.6)) {
            float collider_radius = collider_ptr->__hitbox_radius;
            __asm__("":"+x"(radius_squared_vec));
            collider_radius = collider_radius * collider_radius + radius_squared_vec[0];
            //for (size_t i = 0; i < countof(check_pos_list); ++i) {
            /*
            nounroll for (int32_t i = countof(check_pos_list) - 1; (int32_t)i >= 0; --i) {
                vec<float, 2> check_pos = check_pos_list_ptr[i];
                vec<float, 2> pos_diff = check_pos - collider_pos;
                vec<float, 2> pos_diff_squared = pos_diff * pos_diff;
                if (
                    collider_radius > pos_diff_squared[0] + pos_diff_squared[1]
                ) {
                    hit_flags |= 1 << i;
                }
            }
            */

            uint32_t cur_flags = 1 << countof(check_pos_list) - 1;
            check_pos_list_ptr += countof(check_pos_list) - 1;
            nounroll do {
                vec<float, 2> check_pos = *check_pos_list_ptr--;
                vec<float, 2> pos_diff = check_pos - collider_pos;
                vec<float, 2> pos_diff_squared = pos_diff * pos_diff;
                if (
                    collider_radius > pos_diff_squared[0] + pos_diff_squared[1]
                ) {
                    hit_flags |= cur_flags;
                }
                cur_flags >>= 1;
            } while (cur_flags);
        }
        else {
            
            vec<float, 2> size_vec = collider_ptr->__hitbox_size.as_vec2() * 0.5f;

            vec<float, 2> coarse_size_vec = size_vec * size_vec;

            __asm__("":"+x"(radius_squared_vec));
            float coarse_radius = coarse_size_vec[0] + coarse_size_vec[1] + radius_squared_vec[0];

            //uint8_t sincos_init = false;
            uint32_t cmp_result = 0;
            vec<float, 4> sincos_vec;

            for (int32_t i = countof(check_pos_list) - 1; (int32_t)i >= 0; --i) {
                //vec<float, 2> pos_diff = check_pos_list_ptr[i] - collider_pos;
                //auto temp = _mm_load_sd(check_pos_list_ptr + i);
                //vec<float, 2> pos_diff = *(vec<float, 2>*)&temp - collider_pos;

                //vec<float, 2> check_pos = *(vec<float, 2>*)&check_pos_list_ptr[i];
                //__asm__ volatile ("":"+m"(check_pos_list));
                vec<float, 2> check_pos = check_pos_list_ptr[i];
                vec<float, 2> pos_diff = check_pos - collider_pos;
                vec<float, 2> pos_diff_squared = pos_diff * pos_diff;

                if (
                    coarse_radius > pos_diff_squared[0] + pos_diff_squared[1]
                ) {

                    //if (!sincos_init) {
                    if (!(cmp_result & 0xFF00)) {
                        //sincos_init = true;

                        long double cos, sin;
                        __asm__(
                            "flds %[angle] \n"
                            "fchs \n"
                            "fsincos"
                            : "=t"(cos), "=u"(sin)
                            : [angle]"m"(collider_ptr->__angle_28)
                        );
                        float mem_sincos[2] = { cos, sin };
                        __asm__("":"+m"(mem_sincos));
                        //vec<float, 4> sincos_vec_temp = { mem_sincos[0], mem_sincos[1], 0.0f, 0.0f };
                        vec<float, 2> sincos_vec_tempA = *(vec<float, 2>*)&mem_sincos;
                        vec<float, 4> sincos_vec_tempB = { sincos_vec_tempA[0], sincos_vec_tempA[1], 0.0f, 0.0f };

                        sincos_vec = shufflevec(sincos_vec_tempB, sincos_vec_tempB, 0, 1, 0, 1);
                    }

                    vec<float, 4> pos_diff_wide = { pos_diff[0], pos_diff[0], pos_diff[1], pos_diff[1] };

                    // xcos, xsin, ycos, ysin
                    pos_diff_wide *= sincos_vec;

                    float xcmp = pos_diff_wide[0] - pos_diff_wide[3];
                    float ycmp = pos_diff_wide[1] + pos_diff_wide[2];

                    //float abs_xcmp = __builtin_fabsf(xcmp);
                    //float abs_ycmp = __builtin_fabsf(ycmp);

                    vec<float, 4> cmp_vec = { xcmp, ycmp, ycmp, xcmp };

                    vec<float, 4> abs_vec = __builtin_elementwise_abs(cmp_vec);

                    vec<float, 4> size_vec_wide = { size_vec[0], size_vec[1], 0.0f, 0.0f };
                    size_vec_wide = shufflevec(size_vec_wide, size_vec_wide, 0, 1, 1, 0);
                    vec<float, 4> size_cmp_vec = size_vec_wide + radius_vec;

                    //uint32_t cmp_result = ~(uint32_t)_mm_movemask_ps(abs_vec <= size_cmp_vec);
                    cmp_result = ~(uint32_t)_mm_movemask_ps(abs_vec <= size_cmp_vec);
                    if (
                        //!((cmp_result & 3) == 3 || ((cmp_result & 0xC) == 0xC))
                        !(!(cmp_result & 3) || !(cmp_result & 0xC))
                    ) {
                        NEG13(size_vec_wide);
                        cmp_vec += size_vec_wide;
                        cmp_vec *= cmp_vec;


                        // PX, NY, PY, NX
                        vec<float, 4> cmp_vecB = shufflevec(cmp_vec, cmp_vec, 1, 3, 0, 1);
                        cmp_vec += cmp_vecB;

                        uint32_t cmp_resultB = (uint32_t)_mm_movemask_ps(cmp_vec < radius_squared_vec);
                        if (!cmp_resultB) {
                            continue;
                        }
                    }

                    

                    hit_flags |= 1 << i;
                }
            }

            /*
            long double cos, sin;
            __asm__(
                "flds %[angle] \n"
                "fchs \n"
                "fsincos"
                : asm_arg("=t", cos), asm_arg("=u", sin)
                : [angle]"m"(collider_ptr->__angle_28)
            );
            float mem_sincos[2];
            mem_sincos[0] = cos;
            mem_sincos[1] = sin;
            vec<float, 4> sincos_vec = { mem_sincos[0], mem_sincos[1], 0.0f, 0.0f };

            sincos_vec = shufflevec(sincos_vec, sincos_vec, 0, 1, 0, 1);

            
            vec<float, 4> radius_squared_vec = { radius_squared, radius_squared, radius_squared, radius_squared };
            
            for (size_t i = 0; i < countof(check_pos_list); ++i) {
                vec<float, 2> pos_diff = check_pos_list[i] - collider_pos;
                vec<float, 4> pos_diff_wide = { pos_diff[0], pos_diff[0], pos_diff[1], pos_diff[1] };

                // xcos, xsin, ycos, ysin
                pos_diff_wide *= sincos_vec;

                {

                    float xcmp = pos_diff_wide[0] - pos_diff_wide[3];

                    if ()


                        float ycmp = pos_diff_wide[1] + pos_diff_wide[2];

                    vec<float, 4> xcmp_vec = { xcmp, xcmp, xcmp, xcmp };
                    vec<float, 4> ycmp_vec = { ycmp, ycmp, ycmp, ycmp };

                }
            }
            */
        }
    }

end:
    *hit_flags_ptr = hit_flags;
    return hit_flags_ptr;
}