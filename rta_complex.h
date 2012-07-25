/**
 * @file   rta_complex.h
 * @author Jean-Philippe Lambert
 * @date   Mon Sep 10 11:05:09 2007
 * 
 * @brief  rta_complex_t type warper for float, double or long double
 * complex.
 *
 * Default is the same as RTA_REAL_TYPE. Define your RTA_COMPLEX_TYPE to
 * override these.
 * \see rta_configuration.h
 * \see rta_real.h
 * 
 * Copyright (C) 2007 by IRCAM-Centre Georges Pompidou, Paris, France.
 * 
 */
#ifndef _RTA_COMPLEX_H_
#define _RTA_COMPLEX_H_ 1

#include "rta.h"

/** default complex precision is the same as real precision */
#ifndef RTA_COMPLEX_TYPE
#define RTA_COMPLEX_TYPE RTA_REAL_TYPE
#endif

#ifdef WIN32

#if (RTA_COMPLEX_TYPE == RTA_FLOAT_TYPE)
#undef rta_complex_t

typedef struct complex_
{
  float real;
  float imag;
} complex;

#define rta_complex_t complex

inline rta_complex_t rta_make_complex(float real, float imag)
{
	rta_complex_t z = {real, imag};
	return z;
}
#endif

#if (RTA_COMPLEX_TYPE == RTA_DOUBLE_TYPE)
#undef rta_complex_t 

typedef struct complex_
{
  double real;
  double imag;
} complex;

#define rta_complex_t complex
inline rta_complex_t rta_make_complex(double real, double imag)
{
	rta_complex_t z = {real, imag};
	return z;
}
#endif

#if (RTA_COMPLEX_TYPE == RTA_LONG_DOUBLE_TYPE)
#undef rta_complex_t

typedef struct complex_
{
  long double real;
  long double imag;
} complex;

#define rta_complex_t complex
inline rta_complex_t rta_make_complex(long double real, long double imag)
{
	rta_complex_t z = {real, imag};
	return z;
}
#endif

#define creal(z) ((z).real)
#define cimag(z) ((z).imag)

inline rta_complex_t rta_add_complex(rta_complex_t a, rta_complex_t b)
{
	rta_complex_t z = {a.real + b.real, a.imag + b.imag};
	return z;
}

inline rta_complex_t rta_sub_complex(rta_complex_t a, rta_complex_t b)
{
	rta_complex_t z = {a.real - b.real, a.imag - b.imag};
	return z;
}

inline rta_complex_t rta_mul_complex(rta_complex_t a, rta_complex_t b)
{
	rta_complex_t z = {a.real * b.real - a.imag * b.imag, a.imag * b.real + a.real * b.imag};
	return z;
}

inline rta_complex_t rta_mul_complex_real(rta_complex_t a, float b)
{
	rta_complex_t z = {a.real * b, a.imag * b};
	return z;
}

inline rta_complex_t rta_div_complex(rta_complex_t a, rta_complex_t b)
{
	rta_complex_t z = {(a.real * b.real + a.imag * b.imag)/(b.real * b.real + b.imag * b.imag), (b.real * a.imag - a.real * b.imag) / (b.real * b.real + b.imag * b.imag)};
	return z;
}

inline rta_complex_t rta_conj(rta_complex_t a)
{
	rta_complex_t z = {a.real, -a.imag};
	return z;
}

inline void rta_set_complex_real(rta_complex_t a, float b)
{
	a.real = b;
	a.imag = 0.0;
}

#define rta_cabs cabs
#define rta_cacos cacos
#define rta_cacosh cacosh
#define rta_carg carg
#define rta_casin casin
#define rta_casinh casinh
#define rta_catan catan
#define rta_catanh catanh
#define rta_ccos ccos
#define rta_ccosh ccosh
#define rta_cexp cexp
#define rta_cimag cimag
#define rta_clog clog
#define rta_cpow cpow
#define rta_cproj cproj
#define rta_creal creal
#define rta_csin csin
#define rta_csinh csinh
#define rta_csqrt csqrt
#define rta_ctan ctan
#define rta_ctanh ctanh

#else

#if (RTA_COMPLEX_TYPE == RTA_FLOAT_TYPE)
#undef rta_complex_t
#define rta_complex_t float complex
inline rta_complex_t rta_make_complex(float real, float imag)
{
	return real + imag * I;
}
#endif

#if (RTA_COMPLEX_TYPE == RTA_DOUBLE_TYPE)
#undef rta_complex_t 
#define rta_complex_t double complex
inline rta_complex_t rta_make_complex(double real, double imag)
{
	return real + imag * I;
}
#endif

#if (RTA_COMPLEX_TYPE == RTA_LONG_DOUBLE_TYPE)
#undef rta_complex_t
#define rta_complex_t long double complex
inline rta_complex_t rta_make_complex(long double real, long double imag)
{
	return real + imag * I;
}
#endif

#if (RTA_COMPLEX_TYPE == RTA_FLOAT_TYPE)

#define rta_cabs cabsf
#define rta_cacos cacosf
#define rta_cacosh cacoshf
#define rta_carg cargf
#define rta_casin casinf
#define rta_casinh casinhf
#define rta_catan catanf
#define rta_catanh catanhf
#define rta_ccos ccosf
#define rta_ccosh ccoshf
#define rta_cexp cexpf
#define rta_cimag cimagf
#define rta_clog clogf
#define rta_conj conjf
#define rta_cpow cpowf
#define rta_cproj cprojf
#define rta_creal crealf
#define rta_csin csinf
#define rta_csinh csinhf
#define rta_csqrt csqrtf
#define rta_ctan ctanf
#define rta_ctanh ctanhf

#endif

#if (RTA_COMPLEX_TYPE == RTA_DOUBLE_TYPE)

#define rta_cabs cabs
#define rta_cacos cacos
#define rta_cacosh cacosh
#define rta_carg carg
#define rta_casin casin
#define rta_casinh casinh
#define rta_catan catan
#define rta_catanh catanh
#define rta_ccos ccos
#define rta_ccosh ccosh
#define rta_cexp cexp
#define rta_cimag cimag
#define rta_clog clog
#define rta_conj conj
#define rta_cpow cpow
#define rta_cproj cproj
#define rta_creal creal
#define rta_csin csin
#define rta_csinh csinh
#define rta_csqrt csqrt
#define rta_ctan ctan
#define rta_ctanh ctanh

#endif


#if (RTA_COMPLEX_TYPE == RTA_LONG_DOUBLE_TYPE)

#define rta_cabs cabsl
#define rta_cacos cacosl
#define rta_cacosh cacoshl
#define rta_carg cargl
#define rta_casin casinl
#define rta_casinh casinhl
#define rta_catan catanl
#define rta_catanh catanhl
#define rta_ccos ccosl
#define rta_ccosh ccoshl
#define rta_cexp cexpl
#define rta_cimag cimagl
#define rta_clog clogl
#define rta_conj conjl
#define rta_cpow cpowl
#define rta_cproj cprojl
#define rta_creal creall
#define rta_csin csinl
#define rta_csinh csinhl
#define rta_csqrt csqrtl
#define rta_ctan ctanl
#define rta_ctanh ctanhl

#define rta_add_complex(a, b) ((a)+(b))
#define rta_sub_complex(a, b) ((a)-(b))
#define rta_mul_complex(a, b) ((a)*(b))
#define rta_div_complex(a, b) ((a)/(b))
#define rta_mul_complex_real(a, b) ((a)*(b))
#define rta_set_complex_real(a, b) ((a) = (b))

#ifndef __cplusplus
#include <complex.h>
#else
#include "/usr/include/complex.h"
#endif

#endif

#endif /* WIN32 */

#endif /* _RTA_COMPLEX_H_ */
