//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  repmat.cpp
//
//  Code generation for function 'repmat'
//


// Include files
#include "repmat.h"
#include "ITaskAssignment.h"
#include "assignkbestsd.h"
#include "rt_nonfinite.h"

// Function Definitions
void b_repmat(const cell_wrap_23 a[1], const double varargin_1[2], coder::array<
              cell_wrap_0, 2U> &b)
{
  int i;
  i = static_cast<int>(varargin_1[0]);
  b.set_size(i, 1);
  if (i != 0) {
    for (int itilerow = 0; itilerow < i; itilerow++) {
      b[itilerow].f1.set_size(0, a[0].f1.size[1]);
    }
  }
}

void repmat(const cell_3 a_data[], const double varargin_1[2], coder::array<
            cell_3, 2U> &b)
{
  int i;
  i = static_cast<int>(varargin_1[0]);
  b.set_size(i, 1);
  if (i != 0) {
    for (int itilerow = 0; itilerow < i; itilerow++) {
      b[itilerow] = a_data[0];
    }
  }
}

// End of code generation (repmat.cpp)
