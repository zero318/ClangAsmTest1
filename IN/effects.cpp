// size: 0x360
struct Effect {
    AnmVM vm; // 0x0
    Float3 position; // 0x2A4
    Float3 rotation; // 0x2B0
    Float3 velocity; // 0x2BC
    Float3 acceleration; // 0x2C8
    Float3 __float3_2D4; // 0x2D4
    Float3 __float3_2E0; // 0x2E0
    Float3 __float3_2EC; // 0x2EC
    D3DXQUATERNION quaternion; // 0x304
    float __float_314; // 0x314
    float __float_318; // 0x318
    float __float_31C; // 0x31C
    float __float_320; // 0x320
    int __int_324; // 0x324
    int __child_effect_id; // 0x328
    float __float_32C; // 0x32C
    float __angle_330; // 0x330
    float __float_334; // 0x334
    Timer timer; // 0x338
    unknown_fields(0x4); // 0x344 this is probably a slot for on_init_func but it never gets saved
    EffectOnTick* on_tick_func; // 0x348
    EffectOnDraw* on_draw_func; // 0x34C
    int8_t alive; // 0x350
    int8_t id; // 0x351
    int8_t __sbyte_352; // 0x352
    int8_t __sbyte_354; // 0x353
    int8_t __sbyte_354; // 0x354
    int8_t __sbyte_355; // 0x355
    int8_t __update_special_data_during_draw; // 0x356
    int8_t __sbyte_357; // 0x357
    void* special_data; // 0x358
    Effect* next; // 0x35C
    // 0x360
  
    // 0x425E60
    static int __fastcall on_tick_3_to_11(Effect* self) {
        self->position += self->velocity;
        self->velocity += self->acceleration;
        return 1;
    }
    // 0x426030
    static int __fastcall on_tick_13_14_15(Effect* self);
    // 0x426BB0
    static int __fastcall on_tick_17_27(Effect* self) {
        self->position = (self->__float3_2EC * (256.0f - (self->timer * 256.0f / 60.0f))) + self->__float3_2E0;
        self->position.z = 0.0f;
        return 1;
    }
    // 0x426C90
    static int __fastcall on_tick_18(Effect* self) {
        self->position = (self->__float3_2EC * (256.0f - (self->timer * 256.0f / 240.0f))) + self->__float3_2E0;
        self->position.z = 0.0f;
        return 1;
    }
    // 0x426D70
    static int __fastcall on_tick_19(Effect* self);
    // 0x4271A0
    static int __fastcall on_tick_21_26(Effect* self) {
        float A = self->timer / 90.0f;
        A = 1.0f - ((1.0f - A) * (1.0f - A));
        self->position = self->__float3_2EC * A * 128.0f + self->__float3_2E0;
        self->position.z = 0.0f;
        return 1;
    }
    // 0x426C40
    static int __fastcall on_tick_22(Effect* self);
    // 0x427250
    static int __fastcall on_tick_23_24(Effect* self) {
        return 1;
    }
    // 0x427990
    static int __fastcall on_tick_35_39_49_52_59(Effect* self) {
        self->__update_special_data_during_draw = true;
        self->__float_320 = self->vm.scale.x;
        self->__float_314 = self->vm.position.x;
        return 1;
    }
    // 0x4114E0
    static int __fastcall on_tick_36_37(Effect* self);
    // 0x4117B0
    static int __fastcall on_tick_37(Effect* self); // init func overwrites table value with this
    // 0x4279D0
    static int __fastcall on_tick_40_55_56_58_65(Effect* self) {
        self->__int_324 = self->vm.I0;
        self->__float_334 = self->vm.I1;
        self->__float_320 = self->vm.scale.x;
        self->__float_314 = self->vm.position.x;
        self->__float_32C = self->vm.position.y;
        self->__float_318 = self->vm.rotation.z;
        self->__angle_330 = self->vm.rotation.y;
        self->__update_special_data_during_draw = true;
        return 1;
    }
    // 0x40EB50
    static int __fastcall on_tick_41(Effect* self) {
        self->__update_special_data_during_draw = true;
        self->__int_324 = 48;
        self->__float_320 = 32.0f;
        self->__float_314 = (self->timer & 1 ? 8 : 0) + 64.0f;
        self->__float_318 = 0.0f;
        return 1;
    }
    // 0x40E040
    static int __fastcall on_tick_42(Effect* self) {
        float A = 1.0f - (self->timer / 40.0f);
        A *= A;
        A = 1.0f - A;
        self->__float_314 = 256.0f * A;
        self->__int_324 = 64;
        self->__update_special_data_during_draw = true;
        self->__float_334 = 5.0f;
        self->__angle_330 = 0.0f;
        if (self->timer < 40) {
            self->__float_320 = 8.0f;
        } else {
            self->__float_32C = 64.0f * A;
            self->__float_320 += 2.0f;
        }
        return 1;
    }
    // 0x40E120
    static int __fastcall on_tick_43(Effect* self) {
        float A = 1.0f - (self->timer / 40.0f);
        A *= A;
        A = 1.0f - A;
        self->__float_314 = 256.0f * A;
        self->__int_324 = 48;
        self->__update_special_data_during_draw = true;
        self->__float_334 = 0.0f;
        self->__float_32C = 128.0f * A ;
        self->__angle_330 = QUARTER_PI_f;
        if (self->timer < 40) {
            self->__float_320 = 8.0f;
        } else {
            self->__float_320 += 1.5f;
        }
        return 1;
    }
    // 0x40E200
    static int __fastcall on_tick_44(Effect* self) {
        float A = 1.0f - (self->timer / 40.0f);
        A *= A;
        A = 1.0f - A;
        self->__float_314 = 256.0f * A;
        self->__int_324 = 48;
        self->__update_special_data_during_draw = true;
        self->__float_334 = 0.0f;
        self->__float_32C = 128.0f * A ;
        self->__angle_330 = -QUARTER_PI_f;
        if (self->timer < 40) {
            self->__float_320 = 8.0f;
        } else {
            self->__float_320 += 1.5f;
        }
        return 1;
    }
    // 0x40E2D0
    static int __fastcall on_tick_45(Effect* self) {
        float A = 1.0f - (self->timer / 40.0f);
        A *= A;
        A = 1.0f - A;
        self->__float_314 = 192.0f * A;
        self->__int_324 = 8;
        self->__update_special_data_during_draw = true;
        self->__float_320 = 8.0f;
        return 1;
    }
    // 0x410BB0
    static int __fastcall on_tick_46(Effect* self) {
        self->__float_314 += 8.0f;
        self->__update_special_data_during_draw = true;
        self->__int_324 = 12;
        self->__float_320 = 32.0f;
        return 1;
    }
    // 0x413070
    static int __fastcall on_tick_48(Effect* self) {
        if (self->timer < 30) {
            self->__float_314 = 192.0f;
            self->__int_324 = 48;
            self->__float_320 = 3.0f;
            self->__float_32C = 0.0001f;
            self->__angle_330 = HALF_PI_f;
        } else {
            float A = (self->timer - 30.0f) / 30.0f;
            A *= A;
            A *= A;
            self->__float_32C = 192.0f * A + 0.0001f;
            self->__float_320 = 80.0f * A + 3.0f;
        }
        self->__update_special_data_during_draw = true;
        return 1;
    }
    // 0x427A60
    static int __fastcall on_tick_50(Effect* self) {
        self->__int_324 = 32;
        self->__float_320 = self->vm.scale.x;
        self->__float_314 = self->vm.position.x;
        self->__float_32C = self->vm.position.y;
        self->__update_special_data_during_draw = true;
        if (self->timer >= 120) {
            return 0;
        } else {
            return 1;
        }
    }
    // 0x4264F0
    static int __fastcall on_tick_51(Effect* self);
    // 0x427AE0
    static int __fastcall on_tick_53_54(Effect* self) {
        self->__update_special_data_during_draw = true;
        self->__float_320 = self->vm.scale.x;
        self->__float_314 = self->vm.position.x;
        self->__float_32C = self->vm.position.y;
        self->__float_318 = self->vm.rotation.z;
        if (!self->vm.alpha) {
            return 0;
        } else {
            return 1;
        }
    }
    // 0x426990
    static int __fastcall on_tick_63(Effect* self);
    // 0x427B50
    static int __fastcall on_tick_64(Effect* self) {
        self->__int_324 = self->vm.I0;
        self->__float_334 = self->vm.I1;
        self->__float_320 = self->vm.scale.x;
        self->__float_314 = self->vm.F1;
        self->__float_318 = self->vm.rotation.z;
        self->__angle_330 = self->vm.rotation.y;
        self->__update_special_data_during_draw = true;
        self->__float3_2E0 = self->vm.position;
        return 1;
    }
    
    
    // 0x425EA0
    static int __fastcall on_init_3(Effect* self);
    // 0x425D70
    static int __fastcall on_init_4_to_11(Effect* self);
    // 0x425FE0
    static int __fastcall on_init_13_14_15(Effect* self) {
        self->__sbyte_354 = 2;
        self->__float3_2EC.x = 0.0f;
        self->__float3_2EC.y = 0.0f;
        self->__float3_2EC.z = 0.0f;
        self->__float_314 = 0.0f;
        return 0;
    }
    // 0x426B20
    static int __fastcall on_init_17_18_27(Effect* self);
    // 0x426E70
    static int __fastcall on_init_19(Effect* self);
    // 0x4270C0
    static int __fastcall on_init_21_26(Effect* self);
    // 0x4272E0
    static int __fastcall on_init_special_data(Effect* self);
    // 0x411720
    static int __fastcall on_init_36(Effect* self);
    // 0x411A80
    static int __fastcall on_init_37(Effect* self);
    // 0x426280
    static int __fastcall on_init_51(Effect* self);
    // 0x427970
    static int __fastcall on_init_56_58_65(Effect* self) {
        on_init_special_data(self);
        self->__byte_355 = 1;
        return 0;
    }
    // 0x427260
    static int __fastcall on_init_60_61(Effect* self);
    // 0x426720
    static int __fastcall on_init_63(Effect* self);
    
    // 0x427450
    static int __fastcall on_draw_special_data(Effect* self);
};



using EffectOnTick = int __fastcall (Effect*);
using EffectOnDraw = int __fastcall (Effect*);
using EffectOnInit = int __fastcall (Effect*);

// size: 0xC
struct EffectData {
  int32_t anm_script; // 0x0
  EffectOnTick* on_tick_func; // 0x4
  EffectOnInit* on_init_func; // 0x8
  // 0xC
};

// 0x4C6D30
EffectData EFFECT_TABLE[66] = {
    /*[0] =*/ { 28, NULL, NULL },
    /*[1] =*/ { 29, NULL, NULL },
    /*[2] =*/ { 30, NULL, NULL },
    /*[3] =*/ { 31, &Effect::on_tick_3_to_11, &Effect::on_init_3 },
    /*[4] =*/ { 36, &Effect::on_tick_3_to_11, &Effect::on_init_4_to_11 },
    /*[5] =*/ { 37, &Effect::on_tick_3_to_11, &Effect::on_init_4_to_11 },
    /*[6] =*/ { 38, &Effect::on_tick_3_to_11, &Effect::on_init_4_to_11 },
    /*[7] =*/ { 39, &Effect::on_tick_3_to_11, &Effect::on_init_4_to_11 },
    /*[8] =*/ { 40, &Effect::on_tick_3_to_11, &Effect::on_init_4_to_11 },
    /*[9] =*/ { 41, &Effect::on_tick_3_to_11, &Effect::on_init_4_to_11 },
    /*[10] =*/ { 42, &Effect::on_tick_3_to_11, &Effect::on_init_4_to_11 },
    /*[11] =*/ { 43, &Effect::on_tick_3_to_11, &Effect::on_init_4_to_11 },
    /*[12] =*/ { 44, NULL, NULL },
    /*[13] =*/ { 45, &Effect::on_tick_13_14_15, &Effect::on_init_13_14_15 },
    /*[14] =*/ { 45, &Effect::on_tick_13_14_15, &Effect::on_init_13_14_15 },
    /*[15] =*/ { 45, &Effect::on_tick_13_14_15, &Effect::on_init_13_14_15 },
    /*[16] =*/ { 0, NULL, NULL },
    /*[17] =*/ { 32, &Effect::on_tick_17_27, &Effect::on_init_17_18_27 },
    /*[18] =*/ { 33, &Effect::on_tick_18, &Effect::on_init_17_18_27 },
    /*[19] =*/ { 51, &Effect::on_tick_19, &Effect::on_init_19 },
    /*[20] =*/ { 56, NULL, NULL },
    /*[21] =*/ { 52, &Effect::on_tick_21_26, &Effect::on_init_21_26 },
    /*[22] =*/ { 54, &Effect::on_tick_22, NULL },
    /*[23] =*/ { 104, &Effect::on_tick_23_24, NULL },
    /*[24] =*/ { 104, &Effect::on_tick_23_24, NULL },
    /*[25] =*/ { 35, NULL, NULL },
    /*[26] =*/ { 53, &Effect::on_tick_21_26, &Effect::on_init_21_26 },
    /*[27] =*/ { 34, &Effect::on_tick_17_27, &Effect::on_init_17_18_27 },
    /*[28] =*/ { 57, NULL, NULL },
    /*[29] =*/ { 58, NULL, NULL },
    /*[30] =*/ { 59, NULL, NULL },
    /*[31] =*/ { 60, NULL, NULL },
    /*[32] =*/ { 48, NULL, NULL },
    /*[33] =*/ { 49, NULL, NULL },
    /*[34] =*/ { 50, NULL, NULL },
    /*[35] =*/ { 88, &Effect::on_tick_35_39_49_52_59, &Effect::on_init_special_data },
    /*[36] =*/ { 88, &Effect::on_tick_36_37, &Effect::on_init_36 },
    /*[37] =*/ { 92, &Effect::on_tick_36_37, &Effect::on_init_37 },
    /*[38] =*/ { 71, NULL, NULL },
    /*[39] =*/ { 76, &Effect::on_tick_35_39_49_52_59, &Effect::on_init_special_data },
    /*[40] =*/ { 81, &Effect::on_tick_40_55_56_58_65, &Effect::on_init_special_data },
    /*[41] =*/ { 82, &Effect::on_tick_41, &Effect::on_init_special_data },
    /*[42] =*/ { 83, &Effect::on_tick_42, &Effect::on_init_special_data },
    /*[43] =*/ { 83, &Effect::on_tick_43, &Effect::on_init_special_data },
    /*[44] =*/ { 83, &Effect::on_tick_44, &Effect::on_init_special_data },
    /*[45] =*/ { 83, &Effect::on_tick_45, &Effect::on_init_special_data },
    /*[46] =*/ { 84, &Effect::on_tick_46, &Effect::on_init_special_data },
    /*[47] =*/ { 72, NULL, NULL },
    /*[48] =*/ { 85, &Effect::on_tick_48, &Effect::on_init_special_data },
    /*[49] =*/ { 86, &Effect::on_tick_35_39_49_52_59, &Effect::on_init_special_data },
    /*[50] =*/ { 80, &Effect::on_tick_50, &Effect::on_init_special_data },
    /*[51] =*/ { 73, &Effect::on_tick_51, &Effect::on_init_51 },
    /*[52] =*/ { 77, &Effect::on_tick_35_39_49_52_59, &Effect::on_init_special_data },
    /*[53] =*/ { 88, &Effect::on_tick_53_54, &Effect::on_init_special_data },
    /*[54] =*/ { 88, &Effect::on_tick_53_54, &Effect::on_init_special_data },
    /*[55] =*/ { 87, &Effect::on_tick_55_56_58_65, &Effect::on_init_special_data },
    /*[56] =*/ { 96, &Effect::on_tick_55_56_58_65, &Effect::on_init_56_58_65 },
    /*[57] =*/ { 55, NULL, NULL },
    /*[58] =*/ { 100, &Effect::on_tick_55_56_58_65, &Effect::on_init_56_58_65 },
    /*[59] =*/ { 78, &Effect::on_tick_35_39_49_52_59, &Effect::on_init_special_data },
    /*[60] =*/ { 102, NULL, &Effect::on_init_60_61 },
    /*[61] =*/ { 103, NULL, &Effect::on_init_60_61 },
    /*[62] =*/ { 75, NULL, NULL },
    /*[63] =*/ { 74, &Effect::on_tick_63, &Effect::on_init_63 },
    /*[64] =*/ { 77, &Effect::on_tick_64, &Effect::on_init_special_data },
    /*[65] =*/ { 98, &Effect::on_tick_55_56_58_65, &Effect::on_init_56_58_65 }
};