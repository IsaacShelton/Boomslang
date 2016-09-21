
#ifndef BOOMSLANG_MATH_H_INCLUDED
#define BOOMSLANG_MATH_H_INCLUDED

#include <math.h>

#define boomslang_PI         boomslang_Double(3.14159265)
#define boomslang_INFINITY   boomslang_Double(INFINITY)
#define boomslang_NAN        boomslang_Double(NAN)

#define boomslang_cos(a)     boomslang_Double(cos( a.data ))
#define boomslang_sin(a)     boomslang_Double(sin( a.data ))
#define boomslang_tan(a)     boomslang_Double(tan( a.data ))
#define boomslang_acos(a)    boomslang_Double(acos( a.data ))
#define boomslang_asin(a)    boomslang_Double(asin( a.data ))
#define boomslang_atan(a)    boomslang_Double(atan( a.data ))
#define boomslang_atan2(a,b) boomslang_Double(atan( a.data, b.data ))
#define boomslang_cosh(a)    boomslang_Double(cosh( a.data ))
#define boomslang_sinh(a)    boomslang_Double(sinh( a.data ))
#define boomslang_tanh(a)    boomslang_Double(tanh( a.data ))
#define boomslang_acosh(a)   boomslang_Double(acosh( a.data ))
#define boomslang_asinh(a)   boomslang_Double(asinh( a.data ))
#define boomslang_atanh(a)   boomslang_Double(atanh( a.data ))
#define boomslang_exp(a)     boomslang_Double(exp( a.data ))
// <Some Functions Not Supported>
#define boomslang_pow(a)     boomslang_Double(pow( a.data ))
#define boomslang_sqrt(a)    boomslang_Double(sqrt( a.data ))
#define boomslang_cbrt(a)    boomslang_Double(cbrt( a.data ))
#define boomslang_hypot(a,b) boomslang_Double(hypot( a.data, b.data ))
// <Some Functions Not Supported>
#define boomslang_ceil(a)    boomslang_Double(ceil( a.data ))
#define boomslang_floor(a)   boomslang_Double(floor( a.data ))
#define boomslang_fmod(a,b)  boomslang_Double(fmod( a.data, b.data ))
#define boomslang_mod(a,b)   boomslang_Double(fmod( a.data, b.data ))
#define boomslang_trunc(a)   boomslang_Double(trunc( a.data ))
#define boomslang_round(a)   boomslang_Double(round( a.data ))
#define boomslang_fdim(a,b)  boomslang_Double(fdim( a.data, b.data ))
#define boomslang_fmax(a,b)  boomslang_Double(fmax( a.data, b.data ))
#define boomslang_max(a,b)   boomslang_Double(fmax( a.data, b.data ))
#define boomslang_fmin(a,b)  boomslang_Double(fmin( a.data, b.data ))
#define boomslang_min(a,b)   boomslang_Double(fmin( a.data, b.data ))
#define boomslang_fabs(a)    boomslang_Double(fabs( a.data ))
#define boomslang_abs(a)     boomslang_Double(abs( a.data ))
// <Some Functions Not Supported>

#endif // BOOMSLANG_MATH_H_INCLUDED
