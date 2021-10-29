//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  sdRemoveImpossibles.cpp
//
//  Code generation for function 'sdRemoveImpossibles'
//


// Include files
#include "sdRemoveImpossibles.h"
#include "ITaskAssignment.h"
#include "assign2DRelaxed.h"
#include "assignAuctionWithPrice.h"
#include "assignkbestsd.h"
#include "bsxfun.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include "sortIdx.h"

// Function Definitions
void sdRemoveImpossibles(coder::array<double, 3U> &costMatrix)
{
  signed char dimSize_idx_0;
  signed char dimSize_idx_1;
  signed char dimSize_idx_2;
  coder::array<double, 2U> y;
  int loop_ub;
  int end;
  int costMatrix_size[1];
  double costMatrix_data[51];
  coder::array<double, 3U> temporaryMatrix;
  int temporaryMatrix_size[2];
  coder::array<double, 3U> b_temporaryMatrix;
  int b_temporaryMatrix_size[3];
  int i;
  coder::array<int, 1U> r;
  dimSize_idx_0 = static_cast<signed char>(costMatrix.size(0));
  dimSize_idx_1 = static_cast<signed char>(costMatrix.size(1));
  dimSize_idx_2 = static_cast<signed char>(costMatrix.size(2));
  if (dimSize_idx_0 < 1) {
    y.set_size(1, 0);
  } else {
    y.set_size(1, (static_cast<int>(dimSize_idx_0)));
    loop_ub = dimSize_idx_0 - 1;
    for (end = 0; end <= loop_ub; end++) {
      y[end] = static_cast<double>(end) + 1.0;
    }
  }

  costMatrix_size[0] = y.size(1);
  loop_ub = y.size(1);
  for (end = 0; end < loop_ub; end++) {
    costMatrix_data[end] = costMatrix[static_cast<int>(y[end]) - 1];
  }

  bsxfun(costMatrix, costMatrix_data, costMatrix_size, temporaryMatrix);
  if (dimSize_idx_1 < 1) {
    y.set_size(1, 0);
  } else {
    loop_ub = dimSize_idx_1 - 1;
    y.set_size(1, (loop_ub + 1));
    for (end = 0; end <= loop_ub; end++) {
      y[end] = static_cast<double>(end) + 1.0;
    }
  }

  temporaryMatrix_size[0] = 1;
  temporaryMatrix_size[1] = y.size(1);
  loop_ub = y.size(1);
  for (end = 0; end < loop_ub; end++) {
    costMatrix_data[end] = temporaryMatrix[temporaryMatrix.size(0) * (
      static_cast<int>(y[end]) - 1)];
  }

  b_temporaryMatrix.set_size(temporaryMatrix.size(0), temporaryMatrix.size(1),
    temporaryMatrix.size(2));
  loop_ub = temporaryMatrix.size(0) * temporaryMatrix.size(1) *
    temporaryMatrix.size(2) - 1;
  for (end = 0; end <= loop_ub; end++) {
    b_temporaryMatrix[end] = temporaryMatrix[end];
  }

  b_bsxfun(b_temporaryMatrix, costMatrix_data, temporaryMatrix_size,
           temporaryMatrix);
  if (dimSize_idx_2 < 1) {
    y.set_size(1, 0);
  } else {
    loop_ub = dimSize_idx_2 - 1;
    y.set_size(1, (loop_ub + 1));
    for (end = 0; end <= loop_ub; end++) {
      y[end] = static_cast<double>(end) + 1.0;
    }
  }

  b_temporaryMatrix_size[0] = 1;
  b_temporaryMatrix_size[1] = 1;
  b_temporaryMatrix_size[2] = y.size(1);
  loop_ub = y.size(1);
  for (end = 0; end < loop_ub; end++) {
    costMatrix_data[end] = temporaryMatrix[temporaryMatrix.size(0) *
      temporaryMatrix.size(1) * (static_cast<int>(y[end]) - 1)];
  }

  b_temporaryMatrix.set_size(temporaryMatrix.size(0), temporaryMatrix.size(1),
    temporaryMatrix.size(2));
  loop_ub = temporaryMatrix.size(0) * temporaryMatrix.size(1) *
    temporaryMatrix.size(2) - 1;
  for (end = 0; end <= loop_ub; end++) {
    b_temporaryMatrix[end] = temporaryMatrix[end];
  }

  c_bsxfun(b_temporaryMatrix, costMatrix_data, b_temporaryMatrix_size,
           temporaryMatrix);
  end = temporaryMatrix.size(0) * (temporaryMatrix.size(1) *
    temporaryMatrix.size(2)) - 1;
  loop_ub = 0;
  for (i = 0; i <= end; i++) {
    if (temporaryMatrix[i] > 0.0) {
      loop_ub++;
    }
  }

  r.set_size(loop_ub);
  loop_ub = 0;
  for (i = 0; i <= end; i++) {
    if (temporaryMatrix[i] > 0.0) {
      r[loop_ub] = i + 1;
      loop_ub++;
    }
  }

  loop_ub = r.size(0);
  for (end = 0; end < loop_ub; end++) {
    costMatrix[r[end] - 1] = rtInf;
  }
}

// End of code generation (sdRemoveImpossibles.cpp)
