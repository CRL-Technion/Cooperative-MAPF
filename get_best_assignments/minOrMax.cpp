//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  minOrMax.cpp
//
//  Code generation for function 'minOrMax'
//


// Include files
#include "minOrMax.h"
#include "ITaskAssignment.h"
#include "assignkbestsd.h"
#include "rt_nonfinite.h"

// Function Definitions
void minimum(const coder::array<double, 3U> &x, double ex_data[], int ex_size[2],
             int idx_data[], int idx_size[2])
{
  int k;
  int xOffset;
  int vlen;
  int j;
  ex_size[0] = static_cast<signed char>(x.size(0));
  ex_size[1] = static_cast<signed char>(x.size(1));
  idx_size[0] = static_cast<signed char>(ex_size[0]);
  idx_size[1] = static_cast<signed char>(ex_size[1]);
  k = static_cast<signed char>(ex_size[0]) * static_cast<signed char>(ex_size[1]);
  for (xOffset = 0; xOffset < k; xOffset++) {
    idx_data[xOffset] = 1;
  }

  vlen = x.size(2);
  k = 3;
  if (x.size(2) == 1) {
    k = 2;
  }

  if (3 > k) {
    k = x.size(0) * x.size(1) * x.size(2);
  } else {
    k = x.size(0) * x.size(1);
  }

  for (j = 0; j < k; j++) {
    ex_data[j] = x[j];
  }

  for (int i = 2; i <= vlen; i++) {
    xOffset = (i - 1) * k;
    for (j = 0; j < k; j++) {
      int b_tmp;
      boolean_T p;
      b_tmp = xOffset + j;
      p = ((!rtIsNaN(x[b_tmp])) && (rtIsNaN(ex_data[j]) || (ex_data[j] > x[b_tmp])));
      if (p) {
        ex_data[j] = x[b_tmp];
        idx_data[j] = i;
      }
    }
  }
}

// End of code generation (minOrMax.cpp)
