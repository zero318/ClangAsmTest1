// ECL opcodes

case 136: // debug_breakpoint
/* case 137: thcrap_expr, just ignore this */
    __debugbreak();
    break;
case 138: // casti2f
case 157: // castf2i
{
    EclValueType src_type;
    i32* dst = EnemyEclInstr::GetVar(enemy, &args->alu.res, &src_type);
    EclValueType dst_type = src_type;
    i32* src = EnemyEclInstr::GetVar(enemy, &args->alu.arg1.id, &src_type);
    if ((u32)(src_type | dst_type) <= 1) {
        switch (src_type - dst_type) {
            case 1:
                *dst = *(f32*)src;
                break;
            case 0:
                *dst = *src;
                break;
            default:
                *(f32*)dst = *src;
                break;
        }
    }
    break;
}
case 139: // math_bit_and
{
    EclValueType type;
    i32* dst = EnemyEclInstr::GetVar(enemy, &args->alu.res, &type);
    if (type == ECL_VALUE_TYPE_INT) {
        i32* lhs = EnemyEclInstr::GetVar(enemy, &args->alu.arg1.id, NULL);
        i32* rhs = EnemyEclInstr::GetVar(enemy, &args->alu.arg2.id, NULL);
        *dst = *lhs & *rhs;
    }
    break;
}
case 140: // math_bit_or
{
    EclValueType type;
    i32* dst = EnemyEclInstr::GetVar(enemy, &args->alu.res, &type);
    if (type == ECL_VALUE_TYPE_INT) {
        i32* lhs = EnemyEclInstr::GetVar(enemy, &args->alu.arg1.id, NULL);
        i32* rhs = EnemyEclInstr::GetVar(enemy, &args->alu.arg2.id, NULL);
        *dst = *lhs | *rhs;
    }
    break;
}
case 141: // math_bit_xor
{
    EclValueType type;
    i32* dst = EnemyEclInstr::GetVar(enemy, &args->alu.res, &type);
    if (type == ECL_VALUE_TYPE_INT) {
        i32* lhs = EnemyEclInstr::GetVar(enemy, &args->alu.arg1.id, NULL);
        i32* rhs = EnemyEclInstr::GetVar(enemy, &args->alu.arg2.id, NULL);
        *dst = *lhs ^ *rhs;
    }
    break;
}
case 142: // math_bit_neg
{
    EclValueType type;
    i32* dst = EnemyEclInstr::GetVar(enemy, &args->alu.res, &type);
    if (type == ECL_VALUE_TYPE_INT) {
        i32* src = EnemyEclInstr::GetVar(enemy, &args->alu.arg1.id, NULL);
        *dst = ~*src;
    }
    break;
}
case 143: // math_shl
{
    EclValueType type;
    i32* dst = EnemyEclInstr::GetVar(enemy, &args->alu.res, &type);
    if (type == ECL_VALUE_TYPE_INT) {
        i32* lhs = EnemyEclInstr::GetVar(enemy, &args->alu.arg1.id, NULL);
        i32* rhs = EnemyEclInstr::GetVar(enemy, &args->alu.arg2.id, NULL);
        *dst = *lhs << *(u8*)rhs;
    }
    break;
}
case 144: // math_shr
{
    EclValueType type;
    i32* dst = EnemyEclInstr::GetVar(enemy, &args->alu.res, &type);
    if (type == ECL_VALUE_TYPE_INT) {
        i32* lhs = EnemyEclInstr::GetVar(enemy, &args->alu.arg1.id, NULL);
        i32* rhs = EnemyEclInstr::GetVar(enemy, &args->alu.arg2.id, NULL);
        *dst = (u32)*lhs >> *(u8*)rhs;
    }
    break;
}
case 145: // math_sar
{
    EclValueType type;
    i32* dst = EnemyEclInstr::GetVar(enemy, &args->alu.res, &type);
    if (type == ECL_VALUE_TYPE_INT) {
        i32* lhs = EnemyEclInstr::GetVar(enemy, &args->alu.arg1.id, NULL);
        i32* rhs = EnemyEclInstr::GetVar(enemy, &args->alu.arg2.id, NULL);
        *dst = *lhs >> *(u8*)rhs;
    }
    break;
}
case 146: // math_rol
{
    EclValueType type;
    i32* dst = EnemyEclInstr::GetVar(enemy, &args->alu.res, &type);
    if (type == ECL_VALUE_TYPE_INT) {
        i32* lhs = EnemyEclInstr::GetVar(enemy, &args->alu.arg1.id, NULL);
        i32* rhs = EnemyEclInstr::GetVar(enemy, &args->alu.arg2.id, NULL);
        u32 temp = *lhs;
        u8 count = *(u8*)rhs;
        *dst = temp << count | temp >> 32 - count;
    }
    break;
}
case 147: // math_ror
{
    EclValueType type;
    i32* dst = EnemyEclInstr::GetVar(enemy, &args->alu.res, &type);
    if (type == ECL_VALUE_TYPE_INT) {
        i32* lhs = EnemyEclInstr::GetVar(enemy, &args->alu.arg1.id, NULL);
        i32* rhs = EnemyEclInstr::GetVar(enemy, &args->alu.arg2.id, NULL);
        u32 temp = *lhs;
        u8 count = *(u8*)rhs;
        *dst = temp >> count | temp << 32 - count;
    }
    break;
}
case 148: // math_udiv
{
    EclValueType type;
    i32* dst = EnemyEclInstr::GetVar(enemy, &args->alu.res, &type);
    if (type == ECL_VALUE_TYPE_INT) {
        i32* lhs = EnemyEclInstr::GetVar(enemy, &args->alu.arg1.id, NULL);
        i32* rhs = EnemyEclInstr::GetVar(enemy, &args->alu.arg2.id, NULL);
        *dst = *(u32*)lhs / *(u32*)rhs;
    }
    break;
}
case 149: // math_umod
{
    EclValueType type;
    i32* dst = EnemyEclInstr::GetVar(enemy, &args->alu.res, &type);
    if (type == ECL_VALUE_TYPE_INT) {
        i32* lhs = EnemyEclInstr::GetVar(enemy, &args->alu.arg1.id, NULL);
        i32* rhs = EnemyEclInstr::GetVar(enemy, &args->alu.arg2.id, NULL);
        *dst = *(u32*)lhs % *(u32*)rhs;
    }
    break;
}
case 150: // math_int_neg
case 158: // math_float_neg
{
    EclValueType type;
    i32* dst = EnemyEclInstr::GetVar(enemy, &args->alu.res, &type);
    switch (type) {
        case ECL_VALUE_TYPE_INT:
            *dst = -*EnemyEclInstr::GetVar(enemy, &args->alu.arg1.id, NULL);
            break;
        case ECL_VALUE_TYPE_FLOAT:
            *(f32*)dst = -*EnemyEclInstr::GetVarFloat(enemy, (f32*)&args->alu.arg1.id, NULL);
            break;
    }
    break;
}
case 151: // set_int_indirect
{
    EclValueType type;
    i32* dst = EnemyEclInstr::GetVar(enemy, &args->alu.res, &type);
    if ((u32)type <= 1) {
        i32* src = EnemyEclInstr::GetVar(enemy, &args->alu.arg1.id, NULL);
        *dst = *EnemyEclInstr::GetVar(enemy, (EclVarId*)src, NULL);
    }
    break;
}
case 152: // set_int_abs
case 153: // set_float_abs
{
    EclValueType type;
    i32* dst = EnemyEclInstr::GetVar(enemy, &args->alu.res, &type);
    if ((u32)type <= 1) {
        *dst = args->alu.arg1.i32Param;
    }
    break;
}
case 154: // math_sin
{
    EclValueType type;
    i32* dst = EnemyEclInstr::GetVar(enemy, &args->alu.res, &type);
    if (type == ECL_VALUE_TYPE_FLOAT) {
        f32* src = EnemyEclInstr::GetVarFloat(enemy, (f32*)&args->alu.arg1.id, NULL);
        *dst = sinf(*src);
    }
    break;
}
case 155: // math_cos
{
    EclValueType type;
    i32* dst = EnemyEclInstr::GetVar(enemy, &args->alu.res, &type);
    if (type == ECL_VALUE_TYPE_FLOAT) {
        f32* src = EnemyEclInstr::GetVarFloat(enemy, (f32*)&args->alu.arg1.id, NULL);
        *dst = cosf(*src);
    }
    break;
}
case 156: // inline_switch
{
    u32 val = *EnemyEclInstr::GetVar(enemy, &args->alu.res, NULL);
    i32 i = val <= (u32)args->alu.arg1.i32Param ? val : -1;
    i32* jump = (i32*)&((u64*)&args->alu.arg4)[i];
    enemy->currentContext.time.current = jump[0];
    instruction = (EclRawInstr*)(((u8*)instruction) + jump[1]);
    goto YOLO;
}

// ECL variables

case -10026: // COMPARE
    if (valueType != NULL)
            *valueType = ECL_VALUE_TYPE_INT;
    return &enemy->currentContext.compareRegister;

// Timeline opcodes

case 13: // jump
{
    EclTimelineInstr* instruction = this->timelineInstr;
    this->timelineTime.current = instruction->arg0;
    this->timelineInstr = (EclTimelineInstr*)((u8*)instruction + instruction->args.uintVar1);
    continue;
}