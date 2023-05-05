#ifndef __ZERO_FLOAT_H
#define __ZERO_FLOAT_H

#ifdef __LDBL_MANT_DIG__
#undef __LDBL_MANT_DIG__
#endif
#define __LDBL_MANT_DIG__ 64

#ifdef __LDBL_DIG__
#undef __LDBL_DIG__
#endif
#define __LDBL_DIG__ 18

#ifdef __LDBL_MIN_EXP__
#undef __LDBL_MIN_EXP__
#endif
#define __LDBL_MIN_EXP__ -16381

#ifdef __LDBL_MIN_10_EXP__
#undef __LDBL_MIN_10_EXP__
#endif
#define __LDBL_MIN_10_EXP__ -4931

#ifdef __LDBL_MAX_EXP__
#undef __LDBL_MAX_EXP__
#endif
#define __LDBL_MAX_EXP__ 16384

#ifdef __LDBL_MAX_10_EXP__
#undef __LDBL_MAX_10_EXP__
#endif
#define __LDBL_MAX_10_EXP__ 4932

#ifdef __LDBL_MAX__
#undef __LDBL_MAX__
#endif
#define __LDBL_MAX__ 1.18973149535723176502e+4932L

#ifdef __LDBL_EPSILON__
#undef __LDBL_EPSILON__
#endif
#define __LDBL_EPSILON__ 1.08420217248550443401e-19L

#ifdef __LDBL_MIN__
#undef __LDBL_MIN__
#endif
#define __LDBL_MIN__ 3.36210314311209350626e-4932L

#ifdef __LDBL_DENORM_MIN__
#undef __LDBL_DENORM_MIN__
#endif
#define __LDBL_DENORM_MIN__ 3.64519953188247460253e-4951L

#ifdef __LDBL_DECIMAL_DIG__
#undef __LDBL_DECIMAL_DIG__
#endif
#define __LDBL_DECIMAL_DIG__ 21

#include_next <float.h>

#endif