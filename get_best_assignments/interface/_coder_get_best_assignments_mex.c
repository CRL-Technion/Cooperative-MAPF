/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_get_best_assignments_mex.c
 *
 * Code generation for function '_coder_get_best_assignments_mex'
 *
 */

/* Include files */
#include "_coder_get_best_assignments_mex.h"
#include "_coder_get_best_assignments_api.h"

/* Function Declarations */
MEXFUNCTION_LINKAGE void c_get_best_assignments_mexFunct(int32_T nlhs, mxArray
  *plhs[3], int32_T nrhs, const mxArray *prhs[2]);

/* Function Definitions */
void c_get_best_assignments_mexFunct(int32_T nlhs, mxArray *plhs[3], int32_T
  nrhs, const mxArray *prhs[2])
{
  const mxArray *outputs[3];
  int32_T b_nlhs;
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  st.tls = emlrtRootTLSGlobal;

  /* Check for proper number of arguments. */
  if (nrhs != 2) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 2, 4,
                        20, "get_best_assignments");
  }

  if (nlhs > 3) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 20,
                        "get_best_assignments");
  }

  /* Call the function. */
  get_best_assignments_api(prhs, nlhs, outputs);

  /* Copy over outputs to the caller. */
  if (nlhs < 1) {
    b_nlhs = 1;
  } else {
    b_nlhs = nlhs;
  }

  emlrtReturnArrays(b_nlhs, plhs, outputs);
}

void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs, const mxArray
                 *prhs[])
{
  mexAtExit(&get_best_assignments_atexit);

  /* Module initialization. */
  get_best_assignments_initialize();

  /* Dispatch the entry-point. */
  c_get_best_assignments_mexFunct(nlhs, plhs, nrhs, prhs);

  /* Module termination. */
  get_best_assignments_terminate();
}

emlrtCTX mexFunctionCreateRootTLS(void)
{
  emlrtCreateRootTLS(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1);
  return emlrtRootTLSGlobal;
}

/* End of code generation (_coder_get_best_assignments_mex.c) */
