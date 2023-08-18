enum MotionMode : int32_t {
    AxisVelocityMovement = 0,
    NoMovement = 1,
    OrbitMovement = 2,
    EllipseMovement = 3,
    UnknownMovement = 4
};

enum EclOpcodes {
    MovePositionAbs = 400,
    MovePositionAbsInterp = 401,
    MovePositionRel = 402,
    MovePositionRelInterp = 403,
    MoveVelocityAbs = 404,
    MoveVelocityAbsInterp = 405,
    MoveVelocityRel = 406,
    MoveVelocityRelInterp = 407,
    MoveOrbitAbs = 408,
    MoveOrbitAbsInterp = 409,
    MoveOrbitRel = 410,
    MoveOrbitRelInterp = 411,
    
    MoveToBoss0Abs = 414,
    MoveToBoss0Rel = 415,
    MovePositionAddAbs = 416,
    MovePositionAddRel = 417,
    MoveOriginAbs = 418,
    MoveOriginRel = 419,
    MoveEllipseAbs = 420,
    MoveEllipseAbsInterp = 421,
    MoveEllipseRel = 421,
    MoveEllipseRelInterp = 423,
    
    MoveBezierAbs = 425,
    MoveBezierRel = 426,
    MoveStop = 427,
    MoveVelocityNoMirrorAbs = 428,
    MoveVelocityNoMirrorAbsInterp = 429,
    MoveVelocityNoMirrorRel = 430,
    MoveVelocityNoMirrorRelInterp = 431,
    MoveToEnemyIdAbs = 432,
    MoveToEnemyIdRel = 433,
    MoveCurveAbs = 434,
    MoveCurveRel = 435,
    
    MoveCurveAddAbs = 438,
    MoveCurveAddRel = 439,
    MoveAngleAbs = 440,
    MoveAngleAbsInterp = 441,
    MoveAngleRel = 442,
    MoveAngleRelInterp = 443,
    MoveSpeedAbs = 444,
    MoveSpeedAbsInterp = 445,
    MoveSpeedRel = 446,
    MoveSpeedRelInterp = 447
};

// Notes:
// -The UNKNOWN_FLOAT2_A global is so unused that the compiler assumes it's 0
// -UNKNOWN_FLOAT3_B also seems to be 0, but the compiler doesn't assume that
// -The ".get_position()" function returns a non-const reference. Why is ZUN like this
// -Any inconsistent float/double usage is accurate
// -Abs/Rel for Angle/Speed interp are intertwined, so I can't use my Abs/Rel struct
// -Motion data has axis_velocity and orbit_origin as a union

switch (opcode) {
    case MoveStop:
    {
        MotionData& motion_rel = this->motion.relative;
        MotionData& motion_abs = this->motion.absolute;
        motion_abs.get_position() += motion_rel.get_position();
        motion_rel.position = UNKNOWN_FLOAT3_B;
        motion_rel.set_speed(0.0f);
        motion_abs.set_speed(0.0f);
        motion_rel.set_orbit_origin(UNKNOWN_FLOAT3_B);
        motion_abs.set_orbit_origin(UNKNOWN_FLOAT3_B);
        motion_rel.set_axis_velocity_mode();
        motion_abs.set_axis_velocity_mode();
        this->position_interp.absolute.reset_end_time();
        this->position_interp.relative.reset_end_time();
        this->angle_interp_absolute.reset_end_time();
        this->angle_interp_relative.reset_end_time();
        this->speed_interp_absolute.reset_end_time();
        this->speed_interp_relative.reset_end_time();
        this->orbit_radius_interp.absolute.reset_end_time();
        this->orbit_radius_interp.relative.reset_end_time();
        this->ellipse_interp.absolute.reset_end_time();
        this->ellipse_interp.relative.reset_end_time();
        break;
    }
    case MovePositionAbs: case MovePositionRel:
    {
        // Screwy inline asm BS here that wasn't in UM...?
        MotionData& motion = (opcode != MovePositionAbs) ? this->motion.relative : this->motion.absolute;
        float X = this->get_float_arg(0);
        float Y = this->get_float_arg(1);
        if (X > -999999.0) {
            motion.set_position_x(X);
        }
        if (Y > -999999.0) {
            motion.set_position_y(Y);
        }
        motion.set_axis_velocity_mode();
        this->current_motion.get_position() = this->motion.relative.get_position() + this->motion.absolute.get_position();
        this->update_motion();
        break;
    }
    case MovePositionAddAbs: case MovePositionAddRel:
    {
        MotionData& motion = (opcode == MovePositionAbs) ? this->motion.absolute : this->motion.relative;
        float X = this->get_float_arg(0);
        motion.get_position().x = X;
        float Y = this->get_float_arg(1);
        motion.get_position().y = Y;
        float Z = this->get_float_arg(2);
        motion.get_position().z = Z;
        this->update_motion();
        break;
    }
    case MovePositionAbsInterp: case MovePositionRelInterp:
    case 436: case 437:
    {
        MotionData& motion = (opcode != MovePositionAbsInterp && opcode != 436) ? this->motion.relative : this->motion.absolute;
        ZUNInterpEx<Float3>& position_interp = (opcode != MovePositionAbsInterp && opcode != 436) ? this->position_interp.relative : this->position_interp.absolute;
        float X = this->get_float_arg(2);
        float Y = this->get_float_arg(3);
        if (this->get_int_arg(0) <= 0) {
            position_interp.reset_end_time();
            break;
        }
        if (opcode == 436 || opcode == 437) {
            if (!this->get_mirror_flag()) {
                X = motion.get_position_x() + X;
            } else {
                X = motion.get_position_x() - X;
            }
            Y = motion.get_position_y() + Y;
        }
        position_interp.set_end_time(this->get_int_arg(0));
        position_interp.set_bezier2(UNKNOWN_FLOAT3_B);
        position_interp.set_bezier1(UNKNOWN_FLOAT3_B);
        position_interp.set_mode_3(this->get_int_arg(1));
        position_interp.set_final_value(motion->get_position());
        if (!(Y > -999999.0f)) {
            Y = motion.get_position_y();
        }
        if (!(X > -999999.0f)) {
            X = motion.get_position_x();
        }
        position_interp.set_bezier1(Float3(X, Y, 0.0f));
        position_interp.reset_timer();
        position_interp.set_mode_to_0();
        break;
    }
    case MoveCurveAbs: case MoveCurveRel:
    case MoveCurveAddAbs: case MoveCurveAddRel:
    {
        MotionData& motion = (opcode != MoveCurveAbs && opcode != MoveCurveAddAbs) ? this->motion.relative : this->motion.absolute;
        ZUNInterpEx<Float3>& position_interp = (opcode != MoveCurveAbs && opcode != MoveCurveAddAbs) ? this->position_interp.relative : this->position_interp.absolute;
        float X = this->get_float_arg(3);
        float Y = this->get_float_arg(4);
        if (this->get_int_arg(0) <= 0) {
            position_interp.reset_end_time();
            break;
        }
        if (opcode == MoveCurveAddAbs || opcode == MoveCurveAddRel) {
            if (!this->get_mirror_flag()) {
                X = motion.get_position_x() + X;
            } else {
                X = motion.get_position_x() - X;
            }
            Y = motion.get_position_y() + Y;
        }
        position_interp.set_end_time(this->get_int_arg(0));
        position_interp.set_bezier2(UNKNOWN_FLOAT3_B);
        position_interp.set_bezier1(UNKNOWN_FLOAT3_B);
        position_interp.set_indexed_mode(0, this->get_int_arg(1));
        position_interp.set_indexed_mode(1, this->get_int_arg(2));
        position_interp.set_final_value(motion->get_position());
        if (!(Y > -999999.0f)) {
            Y = motion.get_position_y();
        }
        if (!(X > -999999.0f)) {
            X = motion.get_position_x();
        }
        position_interp.set_bezier1(Float3(X, Y, 0.0f));
        position_interp.reset_timer();
        motion.set_axis_velocity_mode();
        break;
    }
    case MoveBezierAbs: case MoveBezierRel:
    {
        MotionData& motion = (opcode == MoveBezierAbs) ? this->motion.absolute : this->motion.relative;
        ZUNInterpEx<Float3>& position_interp = (opcode == MoveBezierAbs) ? this->position_interp.absolute : this->position_interp.relative;
        float X = this->get_float_arg(3);
        float Y = this->get_float_arg(4);
        float Z = 0.0f;
        Float3 bezier2 = {};
        Float3 bezier1 = {};
        bezier2.x = this->get_float_arg(1);
        bezier2.y = this->get_float_arg(2);
        bezier2.z = 0.0f;
        current.x = this->get_float_arg(5);
        current.y = this->get_float_arg(6);
        current.z = 0.0f;
        position_interp.set_end_time(this->get_int_arg(0));
        position_interp.set_bezier2(bezier2);
        position_interp.set_current(current);
        position_interp.set_mode_3(Bezier);
        position_interp.set_final_value(motion.get_position());
        if (!(Y > -999999.0)) {
            Y = motion.get_position_y();
        }
        if (!(X > -999999.0)) {
            X = motion.get_position_x();
        }
        position_interp.set_bezier1(Float3(X, Y, Z));
        position_interp.reset_timer();
        motion.set_axis_velocity_mode();
        break;
    }
    case MoveVelocityAbs: case MoveVelocityRel:
    case MoveVelocityNoMirrorAbs: case MoveVelocityNoMirrorRel:
    {
        MotionData& motion = (opcode != MoveVelocityAbs && opcode != MoveVelocityNoMirrorAbs) ? this->motion.relative : this->motion.absolute;
        float angle = this->get_float_arg(0);
        float speed = this->get_float_arg(1);
        if (angle > -999999.0) {
            if (this->get_mirror_flag() && (opcode == MoveVelocityAbs || opcode == MoveVelocityRel)) {
                angle = reduce_angle(HALF_PI_f - reduce_angle(angle - HALF_PI_f));
            }
            motion.set_angle(angle);
        }
        if (speed > -999999.0) {
            motion.set_speed(speed);
        }
        motion.set_axis_velocity_mode();
        break;
    }
    case MoveVelocityAbsInterp: case MoveVelocityRelInterp:
    case MoveVelocityNoMirrorAbsInterp: case MoveVelocityNoMirrorRelInterp:
    {
        MotionData& motion = (opcode != MoveVelocityAbsInterp && opcode != MoveVelocityNoMirrorAbsInterp) ? this->motion.relative : this->motion.absolute;
        ZUNInterp<float>& angle_interp = (opcode != MoveVelocityAbsInterp && opcode != MoveVelocityNoMirrorAbsInterp) ? this->angle_interp_relative : this->angle_interp_absolute;
        ZUNInterp<float>& speed_interp = (opcode != MoveVelocityAbsInterp && opcode != MoveVelocityNoMirrorAbsInterp) ? this->speed_interp_relative : this->speed_interp_absolute;
        float angle = this->get_float_arg(2);
        float speed = this->get_float_arg(3);
        Float2 final_val = {};
        if (this->get_int_arg(0) <= 0) {
            angle_interp.reset_end_time();
            speed_interp.reset_end_time();
            break;
        }
        int32_t interp_mode = this->get_int_arg(1);
        if (interp_mode != ConstantVelocity) {
            if (angle > -999999.0) {
                if (this->get_mirror_flag() && (opcode == MoveVelocityAbsInterp || opcode == MoveVelocityRelInterp)) {
                    final_val.x = reduce_angle(HALF_PI_f - reduce_angle(angle - HALF_PI_f))
                } else {
                    final_val.x = angle;
                }
            } else {
                final_val.x = motion.get_angle();
            }
            if (speed > -999999.0) {
                final_val.y = speed;
            } else {
                final_val.y = motion.get_speed();
            }
        } else {
            if (angle > -999999.0) {
                if (this->get_mirror_flag() && (opcode == MoveVelocityAbsInterp || opcode == MoveVelocityRelInterp)) {
                    final_val.x = -angle;
                } else {
                    final_val.x = angle;
                }
            } else {
                final_val.x = 0.0f;
            }
            if (speed > -999999.0) {
                final_val.y = speed;
            } else {
                final_val.y = 0.0f;
            }
        }
        Float2 initial_val = Float2(motion.get_angle(), motion.get_speed());
        if (fabsf(initial_val.x - final_val.x) >= PI_f) {
            if (A.x > initial_val.x) {
                initial_val.x += TWO_PI_f;
            } else {
                final_val.x += TWO_PI_f;
            }
        }
        int32_t end_time = this->get_int_arg(0);
        angle_interp.initialize(end_time, interp_mode, initial_val.x, final_val.x);
        speed_interp.initialize(end_time, interp_mode, initial_val.y, final_val.y);
        motion.set_axis_velocity_mode();
        break;
    }
    case MoveAngleAbs: case MoveAngleRel:
    {
        MotionData& motion = (opcode == MoveAngleAbs) ? this->motion.absolute : this->motion.relative;
        float angle = this->get_float_arg(0);
        if (this->get_mirror_flag()) {
            motion.set_angle(reduce_angle(HALF_PI_f - reduce_angle(angle - HALF_PI_f)));
            motion.set_axis_velocity_mode();
        } else {
            motion.set_angle(angle);
            motion.set_axis_velocity_mode();
        }
        break;
    }
    case MoveAngleAbsInterp: case MoveAngleRelInterp:
    {
        MotionData& motion = (opcode == MoveAngleAbs) ? this->motion.absolute : this->motion.relative;
        ZUNInterp<float>& angle_interp = (opcode == MoveAngleAbs) ? this->angle_interp_absolute : this->angle_interp_relative;
        float angle = this->get_float_arg(2);
        if (this->get_int_arg(0) <= 0) {
            angle_interp.reset_end_time();
            break;
        }
        int32_t interp_mode = this->get_int_arg(1);
        if (interp_mode != ConstantVelocity) {
            if (angle > -999999.0) {
                if (this->get_mirror_flag() && (opcode == MoveAngleAbsInterp || opcode == MoveAngleRelInterp)) {
                    angle = reduce_angle(HALF_PI_f - reduce_angle(angle - HALF_PI_f))
                }
            } else {
                angle = motion.get_angle();
            }
        } else {
            if (angle > -999999.0) {
                if (this->get_mirror_flag() && (opcode == MoveAngleAbsInterp || opcode == MoveAngleRelInterp)) {
                    angle = -angle;
                }
            } else {
                angle = 0.0f;
            }
        }
        float current_angle = motion.get_angle();
        angle = reduced_angle_diff(current_angle, angle) + current_angle;
        angle_interp.initialize(this->get_int_arg(0), interp_mode, current_angle, angle);
        motion.set_axis_velocity_mode();
        break;
    }
    case MoveSpeedAbs: case MoveSpeedRel:
    {
        MotionData& motion = (opcode == MoveSpeedAbs) ? this->motion.absolute : this->motion.relative;
        float speed = this->get_float_arg(0);
        if (speed > -999999.0) {
            motion.set_speed(speed);
        }
        motion.set_axis_velocity_mode();
        break;
    }
    case MoveSpeedAbsInterp: case MoveSpeedRelInterp:
    {
        MotionData& motion = (opcode == MoveSpeedAbs) ? this->motion.absolute : this->motion.relative;
        ZUNInterp<float>& speed_interp = (opcode == MoveSpeedAbs) ? this->speed_interp_absolute : this->speed_interp_relative;
        float speed = this->get_float_arg(2);
        float current_speed = motion.get_speed();
        if (this->get_int_arg(0) <= 0) {
            speed_interp.reset_end_time();
            break;
        }
        int32_t interp_mode = this->get_int_arg(1);
        if (interp_mode != ConstantVelocity) {
            if (!(speed > -999999.0)) {
                speed = motion.get_speed();
            }
        } else {
            if (!(speed > -999999.0)) {
                speed = 0.0f;
            }
        }
        speed_interp.initialize(this->get_int_arg(0), interp_mode, current_speed, speed);
        motion.set_axis_velocity_mode();
        break;
    }
    case MoveOrbitAbs: case MoveOrbitRel:
    {
        MotionData& motion = (opcode == MoveOrbitAbs) ? this->motion.absolute : this->motion.relative;
        float angle = this->get_float_arg(0);
        float speed = this->get_float_arg(1);
        float orbit_radius = this->get_float_arg(2);
        float orbit_velocity = this->get_float_arg(3);
        if (motion.mode_is_orbit()) {
            motion.set_orbit_origin(motion.get_position());
        }
        if (angle > -999999.0) {
            motion.set_angle(angle);
        }
        if (speed > -999999.0) {
            motion.set_speed(speed);
        }
        if (orbit_radius > -999999.0) {
            motion.set_orbit_radius(orbit_radius);
        }
        if (orbit_velocity > -999999.0) {
            motion.set_orbit_velocity(orbit_velocity);
        }
        motion.set_orbit_mode();
        motion.update2();
        this->update_motion();
        break;
    }
    case MoveOrbitAbsInterp: case MoveOrbitRelInterp:
    {
        MotionData& motion = (opcode == MoveOrbitAbsInterp) ? this->motion.absolute : this->motion.relative;
        ZUNInterp<float>& speed_interp = (opcode == MoveOrbitAbsInterp) ? this->speed_interp_absolute : this->speed_interp_relative;
        ZUNInterp<Float2>& orbit_radius_interp = (opcode == MoveOrbitAbsInterp) ? this->orbit_radius_interp.absolute : this->orbit_radius_interp.relative;
        float speed = this->get_float_arg(2);
        float orbit_radius = this->get_float_arg(3);
        float orbit_velocity = this->get_float_arg(4);
        if (!(speed > -999999.0)) {
            speed = motion.get_speed();
        }
        Float2 final_speed = Float2(0.0f, speed);
        Float2 initial_speed = Float2(0.0f, motion.get_speed());
        if (!(orbit_velocity > -999999.0)) {
            orbit_velocity = motion.get_orbit_velocity();
        }
        if (!(orbit_radius > -999999.0)) {
            orbit_radius = motion.get_orbit_radius();
        }
        Float2 final_orbit = Float2(orbit_radius, orbit_velocity);
        Float2 initial_orbit = Float2(motion.get_orbit_radius(), motion.get_orbit_velocity());
        int32_t end_time = this->get_int_arg(0);
        int32_t interp_mode = this->get_int_arg(1);
        if (end_time <= 0) {
            speed_interp.reset_end_time();
            orbit_radius_interp.reset_end_time();
            break;
        }
        speed_interp.set_end_time(end_time);
        speed_interp.set_bezier1(0.0f);
        speed_interp.set_bezier2(0.0f);
        speed_interp.set_mode(interp_mode);
        speed_interp.set_initial_value(initial_speed.y);
        speed_interp.set_final_value(final_speed.y);
        speed_interp.reset_timer();
        orbit_radius_interp.set_end_time(end_time);
        orbit_radius_interp.set_bezier1(UNKNOWN_FLOAT2_A);
        orbit_radius_interp.set_bezier2(UNKNOWN_FLOAT2_A);
        orbit_radius_interp.set_mode(interp_mode);
        orbit_radius_interp.set_initial_value(initial_orbit);
        orbit_radius_interp.set_final_value(final_orbit);
        orbit_radius_interp.reset_timer();
        motion.set_orbit_mode();
        motion.update2();
        this->update_motion();
        break;
    }
    case 418: case 419:
    {
        MotionData& motion = (opcode == 418) ? this->motion.absolute : this->motion.relative;
        float X = this->get_float_arg(0);
        float Y = this->get_float_arg(1);
        if (X > -999999.0) {
            motion.set_orbit_origin_x(X);
        }
        if (Y > -999999.0) {
            motion.set_orbit_origin_y(Y);
        }
        motion.update2();
        this->update_motion();
        break;
    }
    case MoveEllipseAbs: case MoveEllipseRel:
    {
        MotionData& motion = (opcode == MoveEllipseAbs) ? this->motion.absolute : this->motion.relative;
        float angle = this->get_float_arg(0);
        float speed = this->get_float_arg(1);
        float orbit_radius = this->get_float_arg(2);
        float orbit_velocity = this->get_float_arg(3);
        float ellipse_angle = this->get_float_arg(4);
        float ellipse_ratio = this->get_float_arg(5);
        if (motion.mode_is_orbit()) {
            motion.set_orbit_origin(motion.get_position());
        }
        if (angle > -999999.0) {
            motion.set_angle(angle);
        }
        if (speed > -999999.0) {
            motion.set_speed(speed);
        }
        if (orbit_radius > -999999.0) {
            motion.set_orbit_radius(orbit_radius);
        }
        if (orbit_velocity > -999999.0) {
            motion.set_orbit_velocity(orbit_velocity);
        }
        if (ellipse_angle > -999999.0) {
            motion.__set_anlge_28(ellipse_angle);
        }
        if (ellipse_ratio > -999999.0) {
            motion.__set_float_2C(ellipse_ratio);
        }
        motion.set_orbit_mode();
        motion.set_ellipse_mode();
        motion.update2();
        this->update_motion();
        break;
    }
    case MoveEllipseAbsInterp: case MoveEllipseRelInterp:
    {
        MotionData& motion = (opcode == MoveEllipseAbsInterp) ? this->motion.absolute : this->motion.relative;
        ZUNInterp<float>& speed_interp = (opcode == MoveEllipseAbsInterp) ? this->speed_interp_absolute : this->speed_interp_relative;
        ZUNInterp<Float2>& orbit_radius_interp = (opcode == MoveEllipseAbsInterp) ? this->orbit_radius_interp.absolute : this->orbit_radius_interp.relative;
        ZUNInterp<Float2>& ellipse_interp = (opcode == MoveEllipseAbsInterp) ? this->ellipse_interp.absolute : this->ellipse_interp.relative;
        float speed = this->get_float_arg(2);
        float orbit_radius = this->get_float_arg(3);
        float orbit_velocity = this->get_float_arg(4);
        float ellipse_angle = this->get_float_arg(5);
        float ellipse_ratio = this->get_float_arg(6);
        if (!(speed > -999999.0)) {
            speed = motion.get_speed();
        }
        Float2 final_speed = Float2(0.0f, speed);
        Float2 initial_speed = Float2(0.0f, motion.get_speed());
        if (!(orbit_velocity > -999999.0)) {
            orbit_velocity = motion.get_orbit_velocity();
        }
        if (!(orbit_radius > -999999.0)) {
            orbit_radius = motion.get_orbit_radius();
        }
        Float2 final_orbit = Float2(orbit_radius, orbit_velocity);
        Float2 initial_orbit = Float2(motion.get_orbit_radius(), motion.get_orbit_velocity());
        if (!(ellipse_ratio > -999999.0)) {
            ellipse_ratio = motion.get_ellipse_ratio();
        }
        if (!(ellipse_angle > -999999.0)) {
            ellipse_angle = motion.get_ellipse_angle();
        }
        Float2 final_ellipse = Float2(ellipse_angle, ellipse_ratio);
        Float2 initial_ellipse = Float2(motion.get_ellipse_angle(), motion.get_ellipse_ratio());
        int32_t end_time = this->get_int_arg(0);
        int32_t interp_mode = this->get_int_arg(1);
        if (end_time <= 0) {
            speed_interp.reset_end_time();
            orbit_radius_interp.reset_end_time();
            ellipse_interp.reset_end_time();
            break;
        }
        speed_interp.set_end_time(end_time);
        speed_interp.set_bezier1(0.0f);
        speed_interp.set_bezier2(0.0f);
        speed_interp.set_mode(interp_mode);
        speed_interp.set_initial_value(initial_speed);
        speed_interp.set_final_value(final_speed);
        speed_interp.reset_timer();
        orbit_radius_interp.set_end_time(end_time);
        orbit_radius_interp.set_bezier1(UNKNOWN_FLOAT2_A);
        orbit_radius_interp.set_bezier2(UNKNOWN_FLOAT2_A);
        orbit_radius_interp.set_mode(interp_mode);
        orbit_radius_interp.set_initial_value(initial_orbit);
        orbit_radius_interp.set_final_value(final_orbit);
        orbit_radius_interp.reset_timer();
        ellipse_interp.set_end_time(end_time);
        ellipse_interp.set_bezier1(UNKNOWN_FLOAT2_A);
        ellipse_interp.set_bezier2(UNKNOWN_FLOAT2_A);
        ellipse_interp.set_mode(interp_mode);
        ellipse_interp.set_initial_value(initial_ellipse);
        ellipse_interp.set_final_value(final_ellipse);
        ellipse_interp.reset_timer();
        motion.set_orbit_origin(motion.get_position());
        motion.set_orbit_mode();
        motion.set_ellipse_mode();
        motion.update2();
        this->update_motion();
        break;
    }
    case MoveToBoss0Abs:
    {
        this->motion.absolute.get_position() = this->game_side->enemy_manager->get_boss_by_index(0).get_current_motion();
        break;
    }
    case MoveToBoss0Rel:
    {
        this->motion.relative.get_position() = this->game_side->enemy_manager->get_boss_by_index(0).get_current_motion();
        break;
    }
    case MoveToEnemyIdAbs:
    {
        this->motion.absolute.get_position() = this->game_side->enemy_manager->get_enemy_by_id(this->get_int_arg(0));
        break;
    }
    case MoveToEnemyIdRel:
    {
        this->motion.relative.get_position() = this->game_side->enemy_manager->get_enemy_by_id(this->get_int_arg(1));
        break;
    }
    
}

    