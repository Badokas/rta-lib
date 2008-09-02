/**
 * @file   rta_biquad.c
 * @author Jean-Philippe.Lambert@ircam.fr
 * @date   Fri Aug 29 12:38:46 2008
 * 
 * @brief  Biquad filter and coefficients calculation
 * 
 * Based on the "Cookbook formulae for audio EQ biquad filter
 * coefficients" by Robert Bristow-Johnson 
 *
 * Copyright (C) 2007 by IRCAM-Centre Georges Pompidou, Paris, France.
 * 
 */

#include "rta_biquad.h"
#include "rta_filter.h" /* filter types */
#include "rta_math.h" /* rta_sin, rta_cos */

/* y(n) = b0 x(n) + b1 x(n-1) + b2 x(n-2)  */
/*                - a1 x(n-1) - a2 x(n-2)  */

/* note that MaxMSP biquad~ swaps the names for a and b */

/* f0 is normalised by the nyquist frequency */
/* q must be > 0. */
/* gain must be > 0. and is linear */

/* when there is no gain parameter, one can simply multiply the b
 * coefficients by a (linear) gain */

/* a0 is always 1. as each coefficient is normalised by a0, including a0 */

/* a1 is a[0] and a2 is a[1] */


/* LPF: H(s) = 1 / (s^2 + s/Q + 1) */
void rta_biquad_lowpass_coefs(rta_real_t * b, rta_real_t * a,
                              const rta_real_t f0, const rta_real_t q)
{
  const rta_real_t w0 = M_PI * f0;
  const rta_real_t alpha = rta_sin(w0) / (2. * q);
  const rta_real_t c = rta_cos(w0);

  const rta_real_t  a0_inv = 1. / (1. + alpha);

  a[0] = (-2. * c) * a0_inv;
  a[1] = (1. - alpha) * a0_inv;

  b[0] = ((1. - c) * 0.5) * a0_inv;
  b[1] = (1. - c) * a0_inv;
  b[2] = b[0];

  return;
}

/* HPF: H(s) = s^2 / (s^2 + s/Q + 1) */
void rta_biquad_highpass_coefs(rta_real_t * b, rta_real_t * a,
                               const rta_real_t f0, const rta_real_t q)
{
  const rta_real_t w0 = M_PI * f0;
  const rta_real_t alpha = rta_sin(w0) / (2. * q);
  const rta_real_t c = rta_cos(w0);

  const rta_real_t a0_inv = 1. / (1. + alpha);

  a[0] = (-2. * c) * a0_inv;
  a[1] = (1. - alpha) * a0_inv;

  b[0] = ((1. + c) * 0.5) * a0_inv;
  b[1] = (-1. - c) * a0_inv;
  b[2] = b[0];

  return;
}

/* BPF: H(s) = s / (s^2 + s/Q + 1)  (constant skirt gain, peak gain = Q) */
void rta_biquad_bandpass_constant_skirt_coefs(rta_real_t * b, rta_real_t * a, 
                                              const rta_real_t f0, 
                                              const rta_real_t q)
{
  const rta_real_t w0 = M_PI * f0;
  const rta_real_t s = rta_sin(w0);
  const rta_real_t alpha = s / (2. * q);
  const rta_real_t c = rta_cos(w0);

  const rta_real_t a0_inv = 1. / (1. + alpha);

  a[0] = (-2. * c) * a0_inv;
  a[1] = (1. - alpha) * a0_inv;

  b[0] = (s * 0.5) * a0_inv;
  b[1] = 0.;
  b[2] = -b[0];

  return;
}

/* BPF: H(s) = (s/Q) / (s^2 + s/Q + 1)      (constant 0 dB peak gain) */
void rta_biquad_bandpass_constant_peak_coefs(rta_real_t * b, rta_real_t * a, 
                                             const rta_real_t f0, 
                                             const rta_real_t q)
{
  const rta_real_t w0 = M_PI * f0;
  const rta_real_t alpha = rta_sin(w0) / (2. * q);
  const rta_real_t c = rta_cos(w0);

  const rta_real_t a0_inv = 1. / (1. + alpha);

  a[0] = (-2. * c) * a0_inv;
  a[1] = (1. - alpha) * a0_inv;

  b[0] = alpha * a0_inv;
  b[1] = 0.;
  b[2] = -b[0];

  return;
}

/* notch: H(s) = (s^2 + 1) / (s^2 + s/Q + 1) */
void rta_biquad_notch_coefs(rta_real_t * b, rta_real_t * a,
                            const rta_real_t f0, const rta_real_t q)
{
  const rta_real_t w0 = M_PI * f0;
  const rta_real_t alpha = rta_sin(w0) / (2. * q);
  const rta_real_t c = rta_cos(w0);

  const rta_real_t a0_inv = 1. / (1. + alpha);

  a[0] = (-2. * c) * a0_inv;
  a[1] = (1. - alpha) * a0_inv;

  b[0] = a0_inv;
  b[1] = b[1];
  b[2] = b[0];

  return;
}

/* APF: H(s) = (s^2 - s/Q + 1) / (s^2 + s/Q + 1) */
void rta_biquad_allpass_coefs(rta_real_t * b, rta_real_t * a,
                              const rta_real_t f0, const rta_real_t q)
{
  const rta_real_t w0 = M_PI * f0;
  const rta_real_t alpha = rta_sin(w0) / (2. * q);
  const rta_real_t c = rta_cos(w0);

  const rta_real_t a0_inv = 1. / (1. + alpha);

  a[0] = (-2. * c) * a0_inv;
  a[1] = (1. - alpha) * a0_inv;

  b[0] = a[1];
  b[1] = a[0];
  b[2] = (1. + alpha) * a0_inv;

  return;
}

/* peakingEQ: H(s) = (s^2 + s*(A/Q) + 1) / (s^2 + s/(A*Q) + 1) */
/* A = sqrt( 10^(dBgain/20) ) = 10^(dBgain/40) */
/* gain is linear here */
void rta_biquad_peaking_coefs(rta_real_t * b, rta_real_t * a,
                              const rta_real_t f0, const rta_real_t q,
                              const rta_real_t gain)
{
  const rta_real_t g = rta_sqrt(gain);
  const rta_real_t g_inv = 1. / g;

  const rta_real_t w0 = M_PI * f0;
  const rta_real_t alpha = rta_sin(w0) / (2. * q);
  const rta_real_t c = rta_cos(w0);

  const rta_real_t a0_inv = 1. / (1. + alpha * g_inv);

  a[0] = (-2. * c) * a0_inv;
  a[1] = (1. - alpha * g_inv) * a0_inv;

  b[0] = (1. + alpha * g) * a0_inv;
  b[1] = a[0];
  b[2] = (1. - alpha * g) * a0_inv;

  return;
}

/* lowShelf: H(s) = A * (s^2 + (sqrt(A)/Q)*s + A)/(A*s^2 + (sqrt(A)/Q)*s + 1) */
/* A = sqrt( 10^(dBgain/20) ) = 10^(dBgain/40) */
/* gain is linear here */
void rta_biquad_lowshelf_coefs(rta_real_t * b, rta_real_t * a,
                               const rta_real_t f0, const rta_real_t q, 
                               const rta_real_t gain)
{
  const rta_real_t g = rta_sqrt(gain);

  const rta_real_t w0 = M_PI * f0;
  const rta_real_t alpha_2_sqrtg = rta_sin(w0) * rta_sqrt(g) / q ;
  const rta_real_t c = rta_cos(w0);

  const rta_real_t a0_inv = 1. / 
                    ( (g+1.) + (g-1.) * c + alpha_2_sqrtg);

  a[0] = (-2. *     ( (g-1.) + (g+1.) * c                ) ) * a0_inv;
  a[1] = (            (g+1.) + (g-1.) * c - alpha_2_sqrtg  ) * a0_inv; 

  b[0] = (      g * ( (g+1.) - (g-1.) * c + alpha_2_sqrtg) ) * a0_inv;
  b[1] = ( 2. * g * ( (g-1.) - (g+1.) * c                ) ) * a0_inv;
  b[2] = (      g * ( (g+1.) - (g-1.) * c - alpha_2_sqrtg) ) * a0_inv;

  return;
}

/* highShelf: H(s) = A * (A*s^2 + (sqrt(A)/Q)*s + 1)/(s^2 + (sqrt(A)/Q)*s + A) */
/* A = sqrt( 10^(dBgain/20) ) = 10^(dBgain/40) */
/* gain is linear here */
void rta_biquad_highshelf_coefs(rta_real_t * b, rta_real_t * a,
                                const rta_real_t f0, const rta_real_t q, 
                                const rta_real_t gain)
{
  const rta_real_t g = rta_sqrt(gain);

  const rta_real_t w0 = M_PI * f0;
  const rta_real_t alpha_2_sqrtg = rta_sin(w0) * rta_sqrt(g) / q ;
  const rta_real_t c = rta_cos(w0);

  const rta_real_t a0_inv = 1. / 
                    ( (g+1.) - (g-1.) * c + alpha_2_sqrtg);

  a[0] = ( 2. *     ( (g-1.) - (g+1.) * c                ) ) * a0_inv;
  a[1] = (            (g+1.) - (g-1.) * c - alpha_2_sqrtg  ) * a0_inv; 

  b[0] = (      g * ( (g+1.) + (g-1.) * c + alpha_2_sqrtg) ) * a0_inv;
  b[1] = (-2. * g * ( (g-1.) + (g+1.) * c                ) ) * a0_inv;
  b[2] = (      g * ( (g+1.) + (g-1.) * c - alpha_2_sqrtg) ) * a0_inv;

  return;
}

/* helper */
void rta_biquad_coefs(rta_real_t * b, rta_real_t * a, 
                      const rta_filter_t type, 
                      const rta_real_t f0, const rta_real_t q, 
                      const rta_real_t gain)
{
  
  switch(type)
  {
    case rta_lowpass:
      rta_biquad_lowpass_coefs(b, a, f0, q);
      break;

    case rta_highpass:
      rta_biquad_highpass_coefs(b, a, f0, q);
      break;

    case rta_bandpass_constant_skirt:
      rta_biquad_bandpass_constant_skirt_coefs(b, a, f0, q);
      break;

    case rta_bandpass_constant_peak:
      rta_biquad_bandpass_constant_peak_coefs(b, a, f0, q);
      break;

    case rta_notch:
      rta_biquad_notch_coefs(b, a, f0, q);
      break;

    case rta_allpass:
      rta_biquad_allpass_coefs(b, a, f0, q);
      break;

    case rta_peaking:
      rta_biquad_peaking_coefs(b, a, f0, q, gain);
      break;

    case rta_lowshelf:
      rta_biquad_lowshelf_coefs(b, a, f0, q, gain);
      break;

    case rta_highshelf:
      rta_biquad_highshelf_coefs(b, a, f0, q, gain);
      break;
  }

  switch(type)
  {
    case rta_lowpass:
    case rta_highpass:
    case rta_bandpass_constant_skirt:
    case rta_bandpass_constant_peak:
    case rta_notch:
    case rta_allpass:

     if(gain != 1.)
      {
        b[0] *= gain;
        b[1] *= gain;
        b[2] *= gain;
      }
      break;
  }

  return;
}
