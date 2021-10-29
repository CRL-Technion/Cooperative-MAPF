/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_get_best_assignments_api.h
 *
 * Code generation for function '_coder_get_best_assignments_api'
 *
 */

#ifndef _CODER_GET_BEST_ASSIGNMENTS_API_H
#define _CODER_GET_BEST_ASSIGNMENTS_API_H

/* Include files */
#include <stddef.h>
#include <stdlib.h>
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"

/* Type Definitions */
#ifndef struct_emxArray_uint32_T
#define struct_emxArray_uint32_T

struct emxArray_uint32_T
{
  uint32_T *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_uint32_T*/

#ifndef typedef_emxArray_uint32_T
#define typedef_emxArray_uint32_T

typedef struct emxArray_uint32_T emxArray_uint32_T;

#endif                                 /*typedef_emxArray_uint32_T*/

#ifndef typedef_cell_wrap_0
#define typedef_cell_wrap_0

typedef struct {
  emxArray_uint32_T *f1;
} cell_wrap_0;

#endif                                 /*typedef_cell_wrap_0*/

#ifndef typedef_emxArray_cell_wrap_0
#define typedef_emxArray_cell_wrap_0

typedef struct {
  cell_wrap_0 *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
} emxArray_cell_wrap_0;

#endif                                 /*typedef_emxArray_cell_wrap_0*/

#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T

struct emxArray_real_T
{
  real_T *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_real_T*/

#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T

typedef struct emxArray_real_T emxArray_real_T;

#endif                                 /*typedef_emxArray_real_T*/

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void get_best_assignments(emxArray_real_T *costMatrix, real_T k,
  emxArray_cell_wrap_0 *assignments, emxArray_real_T *cost, emxArray_real_T
  *solutionGap);
extern void get_best_assignments_api(const mxArray * const prhs[2], int32_T nlhs,
  const mxArray *plhs[3]);
extern void get_best_assignments_atexit(void);
extern void get_best_assignments_initialize(void);
extern void get_best_assignments_terminate(void);
extern void get_best_assignments_xil_shutdown(void);
extern void get_best_assignments_xil_terminate(void);

#endif

/* End of code generation (_coder_get_best_assignments_api.h) */
