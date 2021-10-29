//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  sort.cpp
//
//  Code generation for function 'sort'
//


// Include files
#include "sort.h"
#include "ITaskAssignment.h"
#include "assignkbestsd.h"
#include "rt_nonfinite.h"
#include "sortIdx.h"

// Function Definitions
void sort(coder::array<double, 1U> &x, coder::array<int, 1U> &idx)
{
  int dim;
  int i;
  int vlen;
  coder::array<double, 1U> vwork;
  int vstride;
  int k;
  coder::array<int, 1U> iidx;
  dim = 0;
  if (x.size(0) != 1) {
    dim = -1;
  }

  if (dim + 2 <= 1) {
    i = x.size(0);
  } else {
    i = 1;
  }

  vlen = i - 1;
  vwork.set_size(i);
  idx.set_size(x.size(0));
  vstride = 1;
  for (k = 0; k <= dim; k++) {
    vstride *= x.size(0);
  }

  for (dim = 0; dim < vstride; dim++) {
    for (k = 0; k <= vlen; k++) {
      vwork[k] = x[dim + k * vstride];
    }

    sortIdx(vwork, iidx);
    for (k = 0; k <= vlen; k++) {
      i = dim + k * vstride;
      x[i] = vwork[k];
      idx[i] = iidx[k];
    }
  }
}

// End of code generation (sort.cpp)
