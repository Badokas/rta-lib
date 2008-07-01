/**
 * @file   rta_mex_yin.c
 * @author Jean-Philippe.Lambert@ircam.fr
 * @date   Thu May 15 21:12:57 2008
 * 
 * @brief  rta_yin mex function for simple floating-point precision
 * 
 * Copyright (C) 2008 by IRCAM-Centre Georges Pompidou, Paris, France.
 *
 */

#include "mex.h"
#include "rta_yin.h"
#include "rta_math.h" /* rta_sqrt */

static const unsigned int yin_max_mins = 128;


/** 
 * Entry point to C MEX-file
 * 
 * @param nlhs number of expected outputs
 * @param plhs array of pointers to the expected output 
 * @param nrhs number of inputs
 * @param prhs array of pointers to the inputs
 */
void mexFunction(int nlhs, mxArray *plhs[],int nrhs, const mxArray *prhs[])
{
  /* matlab inputs */
  double * input;
  int input_size;
  rta_real_t threshold;
  double sample_rate;
  double min_freq;
  rta_ptr_t setup_address;

  /* rta inputs */
  rta_real_t * real_input; 
  rta_yin_setup_t * yin_setup;
  unsigned int ac_size;

  /* rta outputs */
  rta_real_t lag; 
  rta_real_t abs_min;

  /* matlab outputs */
  rta_real_t * f0;
  rta_real_t * energy;
  rta_real_t * periodicity;
  rta_real_t * ac1_over_ac0;
  rta_real_t * autocorrelation;

  /* other */
  int i;

  /* input arguments */
  setup_address = mxGetScalar(prhs[0]);
  yin_setup = (rta_yin_setup_t *) setup_address;
  
  input = mxGetData(prhs[1]); 
  input_size = mxGetNumberOfElements(prhs[1]);    

  threshold = (rta_real_t) mxGetScalar(prhs[2]);

  min_freq = mxGetScalar(prhs[3]);

  sample_rate =  mxGetScalar(prhs[4]);
 
  ac_size = (unsigned int)ceil(sample_rate / min_freq) + 2;

  if(ac_size < input_size)
  {
    /* output results */
    plhs[0] = mxCreateNumericMatrix(1, 1, RTA_MEX_REAL_TYPE, mxREAL);
    f0 = mxGetData(plhs[0]);
  
    plhs[1] = mxCreateNumericMatrix(1, 1, RTA_MEX_REAL_TYPE, mxREAL);
    energy = mxGetData(plhs[1]);

    plhs[2] = mxCreateNumericMatrix(1, 1, RTA_MEX_REAL_TYPE, mxREAL);
    periodicity = mxGetData(plhs[2]);

    plhs[3] = mxCreateNumericMatrix(1, 1, RTA_MEX_REAL_TYPE, mxREAL);
    ac1_over_ac0 = mxGetData(plhs[3]);

    plhs[4] = mxCreateNumericMatrix(1, ac_size, RTA_MEX_REAL_TYPE, mxREAL);
    autocorrelation = mxGetData(plhs[4]);

#if (RTA_REAL_TYPE != RTA_DOUBLE_TYPE)
    /* input float precision conversion */
    real_input = mxMalloc( input_size * sizeof(rta_real_t)); 
    for (i=0; i<input_size ;i++)
    {
      real_input[i] = (rta_real_t) input[i]; 
    }
#else
    real_input = input;
#endif

    /* yin computation */
    lag = rta_yin(&abs_min, autocorrelation, ac_size, real_input, input_size,
                  yin_setup, threshold);

    /* conform results */
    *f0 = sample_rate / lag;
    *energy = rta_sqrt(autocorrelation[0] / (input_size - ac_size));

    *periodicity = 1.0 - sqrt(abs_min);

    if(autocorrelation[0] != 0.0)
    {
      *ac1_over_ac0 = autocorrelation[1]/autocorrelation[0];
    }
    else
    {
      *ac1_over_ac0 = 0.0;
    }

#if (RTA_REAL_TYPE != RTA_DOUBLE_TYPE)
    /* free mem of tmp vec for float precision conversion */
    mxFree(real_input);
#endif

  }
  else
  {
    printf("rta_yin input too short: at least %d points expected (%d recommended) for a minimum frequency of %g Hz at a sample rate of %g Hz.\n", 
           ac_size + 1, 2 * ac_size, min_freq, sample_rate);
  }
  return;
}
