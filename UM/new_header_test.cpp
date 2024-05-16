#include "..\zero\th_common.h"



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