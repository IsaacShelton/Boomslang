
#ifndef BOOMSLANG_MATH_H_INCLUDED
#define BOOMSLANG_MATH_H_INCLUDED

#include <math.h>

#define boomslang_PI         boomslang_Number(3.14159265)
#define boomslang_INFINITY   boomslang_Number(INFINITY)
#define boomslang_NAN        boomslang_Number(NAN)

#define boomslang_cos(a)     boomslang_Number(cos( a.data ))
#define boomslang_sin(a)     boomslang_Number(sin( a.data ))
#define boomslang_tan(a)     boomslang_Number(tan( a.data ))
#define boomslang_acos(a)    boomslang_Number(acos( a.data ))
#define boomslang_asin(a)    boomslang_Number(asin( a.data ))
#define boomslang_atan(a)    boomslang_Number(atan( a.data ))
#define boomslang_atan2(a,b) boomslang_Number(atan( a.data, b.data ))
#define boomslang_cosh(a)    boomslang_Number(cosh( a.data ))
#define boomslang_sinh(a)    boomslang_Number(sinh( a.data ))
#define boomslang_tanh(a)    boomslang_Number(tanh( a.data ))
#define boomslang_acosh(a)   boomslang_Number(acosh( a.data ))
#define boomslang_asinh(a)   boomslang_Number(asinh( a.data ))
#define boomslang_atanh(a)   boomslang_Number(atanh( a.data ))
#define boomslang_exp(a)     boomslang_Number(exp( a.data ))
// <Some Functions Not Supported>
#define boomslang_pow(a)     boomslang_Number(pow( a.data ))
#define boomslang_sqrt(a)    boomslang_Number(sqrt( a.data ))
#define boomslang_cbrt(a)    boomslang_Number(cbrt( a.data ))
#define boomslang_hypot(a,b) boomslang_Number(hypot( a.data, b.data ))
// <Some Functions Not Supported>
#define boomslang_ceil(a)    boomslang_Number(ceil( a.data ))
#define boomslang_floor(a)   boomslang_Number(floor( a.data ))
#define boomslang_fmod(a,b)  boomslang_Number(fmod( a.data, b.data ))
#define boomslang_mod(a,b)   boomslang_Number(fmod( a.data, b.data ))
#define boomslang_trunc(a)   boomslang_Number(trunc( a.data ))
#define boomslang_round(a)   boomslang_Number(round( a.data ))
#define boomslang_fdim(a,b)  boomslang_Number(fdim( a.data, b.data ))
#define boomslang_fmax(a,b)  boomslang_Number(fmax( a.data, b.data ))
#define boomslang_max(a,b)   boomslang_Number(fmax( a.data, b.data ))
#define boomslang_fmin(a,b)  boomslang_Number(fmin( a.data, b.data ))
#define boomslang_min(a,b)   boomslang_Number(fmin( a.data, b.data ))
#define boomslang_fabs(a)    boomslang_Number(fabs( a.data ))
#define boomslang_abs(a)     boomslang_Number(abs( a.data ))
// <Some Functions Not Supported>

#endif // BOOMSLANG_MATH_H_INCLUDED
