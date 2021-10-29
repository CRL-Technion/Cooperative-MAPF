//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  lapRemoveImpossibles.cpp
//
//  Code generation for function 'lapRemoveImpossibles'
//


// Include files
#include "lapRemoveImpossibles.h"
#include "ITaskAssignment.h"
#include "assign2DRelaxed.h"
#include "assignAuctionWithPrice.h"
#include "assignkbestsd.h"
#include "assignsd.h"
#include "get_best_assignments_rtwutil.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include "sortIdx.h"

// Function Definitions
void lapRemoveImpossibles(coder::array<double, 2U> &costMatrix, coder::array<
  unsigned int, 1U> &rowIdx, coder::array<unsigned int, 2U> &colIdx, coder::
  array<unsigned int, 1U> &unassignedRows, coder::array<unsigned int, 2U>
  &unassignedColumns)
{
  int vstride;
  int i1;
  int i;
  coder::array<int, 1U> r;
  int npages;
  coder::array<double, 2U> y;
  double d;
  unsigned int outsize_idx_0;
  coder::array<boolean_T, 2U> isUnassignedRows_tmp;
  coder::array<boolean_T, 1U> isUnassignedRows;
  int i2;
  int iy;
  coder::array<boolean_T, 2U> isUnassignedColumns;
  int ix;
  boolean_T exitg1;
  coder::array<int, 1U> ii;
  coder::array<int, 2U> b_ii;
  vstride = costMatrix.size(0) * costMatrix.size(1) - 1;
  i1 = 0;
  for (i = 0; i <= vstride; i++) {
    if (costMatrix[i] >= 4.2) {
      i1++;
    }
  }

  r.set_size(i1);
  npages = 0;
  for (i = 0; i <= vstride; i++) {
    if (costMatrix[i] >= 4.2) {
      r[npages] = i + 1;
      npages++;
    }
  }

  vstride = r.size(0);
  for (npages = 0; npages < vstride; npages++) {
    costMatrix[r[npages] - 1] = rtInf;
  }

  if (costMatrix.size(0) < 1) {
    y.set_size(1, 0);
  } else {
    vstride = static_cast<int>(static_cast<double>(costMatrix.size(0)) - 1.0);
    y.set_size(1, (vstride + 1));
    for (npages = 0; npages <= vstride; npages++) {
      y[npages] = static_cast<double>(npages) + 1.0;
    }
  }

  rowIdx.set_size(y.size(1));
  vstride = y.size(1);
  for (npages = 0; npages < vstride; npages++) {
    d = rt_roundd_snf(y[npages]);
    if (d < 4.294967296E+9) {
      if (d >= 0.0) {
        outsize_idx_0 = static_cast<unsigned int>(d);
      } else {
        outsize_idx_0 = 0U;
      }
    } else if (d >= 4.294967296E+9) {
      outsize_idx_0 = MAX_uint32_T;
    } else {
      outsize_idx_0 = 0U;
    }

    rowIdx[npages] = outsize_idx_0;
  }

  if (costMatrix.size(1) < 1) {
    y.set_size(1, 0);
  } else {
    vstride = static_cast<int>(static_cast<double>(costMatrix.size(1)) - 1.0);
    y.set_size(1, (vstride + 1));
    for (npages = 0; npages <= vstride; npages++) {
      y[npages] = static_cast<double>(npages) + 1.0;
    }
  }

  colIdx.set_size(1, y.size(1));
  vstride = y.size(0) * y.size(1);
  for (npages = 0; npages < vstride; npages++) {
    d = rt_roundd_snf(y[npages]);
    if (d < 4.294967296E+9) {
      if (d >= 0.0) {
        outsize_idx_0 = static_cast<unsigned int>(d);
      } else {
        outsize_idx_0 = 0U;
      }
    } else if (d >= 4.294967296E+9) {
      outsize_idx_0 = MAX_uint32_T;
    } else {
      outsize_idx_0 = 0U;
    }

    colIdx[npages] = outsize_idx_0;
  }

  isUnassignedRows_tmp.set_size(costMatrix.size(0), costMatrix.size(1));
  vstride = costMatrix.size(0) * costMatrix.size(1);
  for (npages = 0; npages < vstride; npages++) {
    isUnassignedRows_tmp[npages] = rtIsInf(costMatrix[npages]);
  }

  outsize_idx_0 = static_cast<unsigned int>(isUnassignedRows_tmp.size(0));
  isUnassignedRows.set_size((static_cast<int>(outsize_idx_0)));
  vstride = static_cast<int>(outsize_idx_0);
  for (npages = 0; npages < vstride; npages++) {
    isUnassignedRows[npages] = true;
  }

  vstride = isUnassignedRows_tmp.size(0);
  i2 = (isUnassignedRows_tmp.size(1) - 1) * isUnassignedRows_tmp.size(0);
  iy = -1;
  i1 = 0;
  for (npages = 0; npages < vstride; npages++) {
    i1++;
    i2++;
    iy++;
    ix = i1;
    exitg1 = false;
    while ((!exitg1) && ((vstride > 0) && (ix <= i2))) {
      if (!isUnassignedRows_tmp[ix - 1]) {
        isUnassignedRows[iy] = false;
        exitg1 = true;
      } else {
        ix += vstride;
      }
    }
  }

  outsize_idx_0 = static_cast<unsigned int>(isUnassignedRows_tmp.size(1));
  isUnassignedColumns.set_size(1, (static_cast<int>(outsize_idx_0)));
  vstride = static_cast<int>(outsize_idx_0);
  for (npages = 0; npages < vstride; npages++) {
    isUnassignedColumns[npages] = true;
  }

  npages = isUnassignedRows_tmp.size(1);
  i2 = 0;
  iy = -1;
  for (i = 0; i < npages; i++) {
    vstride = i2 + isUnassignedRows_tmp.size(0);
    ix = i2;
    i2 += isUnassignedRows_tmp.size(0);
    iy++;
    exitg1 = false;
    while ((!exitg1) && (ix + 1 <= vstride)) {
      if (!isUnassignedRows_tmp[ix]) {
        isUnassignedColumns[iy] = false;
        exitg1 = true;
      } else {
        ix++;
      }
    }
  }

  vstride = isUnassignedRows.size(0) - 1;
  i1 = 0;
  for (i = 0; i <= vstride; i++) {
    if (!isUnassignedRows[i]) {
      i1++;
    }
  }

  ii.set_size(i1);
  npages = 0;
  for (i = 0; i <= vstride; i++) {
    if (!isUnassignedRows[i]) {
      ii[npages] = i + 1;
      npages++;
    }
  }

  vstride = isUnassignedColumns.size(1) - 1;
  i1 = 0;
  for (i = 0; i <= vstride; i++) {
    if (!isUnassignedColumns[i]) {
      i1++;
    }
  }

  b_ii.set_size(1, i1);
  npages = 0;
  for (i = 0; i <= vstride; i++) {
    if (!isUnassignedColumns[i]) {
      b_ii[npages] = i + 1;
      npages++;
    }
  }

  y.set_size(ii.size(0), b_ii.size(1));
  vstride = b_ii.size(1);
  for (npages = 0; npages < vstride; npages++) {
    i1 = ii.size(0);
    for (i2 = 0; i2 < i1; i2++) {
      y[i2 + y.size(0) * npages] = costMatrix[(ii[i2] + costMatrix.size(0) *
        (b_ii[npages] - 1)) - 1];
    }
  }

  costMatrix.set_size(y.size(0), y.size(1));
  vstride = y.size(1);
  for (npages = 0; npages < vstride; npages++) {
    i1 = y.size(0);
    for (i2 = 0; i2 < i1; i2++) {
      costMatrix[i2 + costMatrix.size(0) * npages] = y[i2 + y.size(0) * npages];
    }
  }

  vstride = isUnassignedRows.size(0) - 1;
  i1 = 0;
  npages = 0;
  for (i = 0; i <= vstride; i++) {
    if (!isUnassignedRows[i]) {
      i1++;
      rowIdx[npages] = rowIdx[i];
      npages++;
    }
  }

  rowIdx.set_size(i1);
  vstride = isUnassignedColumns.size(1) - 1;
  i1 = 0;
  npages = 0;
  for (i = 0; i <= vstride; i++) {
    if (!isUnassignedColumns[i]) {
      i1++;
      colIdx[npages] = colIdx[i];
      npages++;
    }
  }

  colIdx.set_size(1, i1);
  vstride = isUnassignedRows.size(0);
  i1 = 0;
  ii.set_size(isUnassignedRows.size(0));
  npages = 0;
  exitg1 = false;
  while ((!exitg1) && (npages <= vstride - 1)) {
    if (isUnassignedRows[npages]) {
      i1++;
      ii[i1 - 1] = npages + 1;
      if (i1 >= vstride) {
        exitg1 = true;
      } else {
        npages++;
      }
    } else {
      npages++;
    }
  }

  if (isUnassignedRows.size(0) == 1) {
    if (i1 == 0) {
      ii.set_size(0);
    }
  } else {
    if (1 > i1) {
      i1 = 0;
    }

    ii.set_size(i1);
  }

  unassignedRows.set_size(ii.size(0));
  vstride = ii.size(0);
  for (npages = 0; npages < vstride; npages++) {
    unassignedRows[npages] = static_cast<unsigned int>(ii[npages]);
  }

  vstride = isUnassignedColumns.size(1);
  i1 = 0;
  b_ii.set_size(1, isUnassignedColumns.size(1));
  npages = 0;
  exitg1 = false;
  while ((!exitg1) && (npages <= vstride - 1)) {
    if (isUnassignedColumns[npages]) {
      i1++;
      b_ii[i1 - 1] = npages + 1;
      if (i1 >= vstride) {
        exitg1 = true;
      } else {
        npages++;
      }
    } else {
      npages++;
    }
  }

  if (isUnassignedColumns.size(1) == 1) {
    if (i1 == 0) {
      b_ii.set_size(1, 0);
    }
  } else {
    if (1 > i1) {
      i1 = 0;
    }

    b_ii.set_size(b_ii.size(0), i1);
  }

  unassignedColumns.set_size(1, b_ii.size(1));
  vstride = b_ii.size(0) * b_ii.size(1);
  for (npages = 0; npages < vstride; npages++) {
    unassignedColumns[npages] = static_cast<unsigned int>(b_ii[npages]);
  }
}

// End of code generation (lapRemoveImpossibles.cpp)
