//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  assignAuctionWithPrice.cpp
//
//  Code generation for function 'assignAuctionWithPrice'
//


// Include files
#include "assignAuctionWithPrice.h"
#include "ITaskAssignment.h"
#include "assign2DRelaxed.h"
#include "assignkbestsd.h"
#include "assignsd.h"
#include "get_best_assignments_rtwutil.h"
#include "lapAuction.h"
#include "lapRemoveImpossibles.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include "sortIdx.h"
#include <cmath>

// Function Definitions
void assignAuctionWithPrice(coder::array<double, 2U> &costMatrix, coder::array<
  unsigned int, 2U> &assignments, coder::array<unsigned int, 2U> &unassignedRows,
  coder::array<unsigned int, 2U> &unassignedColumns, coder::array<double, 2U>
  &price)
{
  coder::array<boolean_T, 1U> r;
  int n;
  int i;
  coder::array<boolean_T, 1U> isRowAssigned;
  int end;
  int nx;
  coder::array<int, 1U> r1;
  coder::array<double, 1U> y;
  double maxValue;
  boolean_T exitg1;
  double x;
  coder::array<boolean_T, 2U> isColAssigned;
  coder::array<boolean_T, 2U> r2;
  coder::array<int, 1U> r3;
  coder::array<int, 1U> r4;
  coder::array<double, 2U> colSoln;
  coder::array<unsigned int, 1U> rowIdx;
  coder::array<unsigned int, 2U> colIdx;
  coder::array<unsigned int, 1U> b_unassignedRows;
  coder::array<unsigned int, 2U> b_unassignedColumns;
  boolean_T b_x[2];
  boolean_T b_y;
  coder::array<double, 1U> rowSoln;
  coder::array<double, 2U> b_colSoln;
  coder::array<double, 2U> priceColn;
  coder::array<int, 1U> r5;
  coder::array<unsigned int, 1U> c_unassignedColumns;
  coder::array<unsigned int, 2U> result;
  coder::array<int, 1U> r6;
  coder::array<unsigned int, 1U> c_unassignedRows;
  coder::array<int, 2U> r7;
  r.set_size((costMatrix.size(0) * costMatrix.size(1)));
  n = costMatrix.size(0) * costMatrix.size(1);
  for (i = 0; i < n; i++) {
    r[i] = rtIsInf(costMatrix[i]);
  }

  isRowAssigned.set_size((costMatrix.size(0) * costMatrix.size(1)));
  n = costMatrix.size(0) * costMatrix.size(1);
  for (i = 0; i < n; i++) {
    isRowAssigned[i] = rtIsNaN(costMatrix[i]);
  }

  n = r.size(0);
  for (i = 0; i < n; i++) {
    r[i] = ((!r[i]) && (!isRowAssigned[i]));
  }

  end = r.size(0) - 1;
  nx = 0;
  for (i = 0; i <= end; i++) {
    if (r[i]) {
      nx++;
    }
  }

  r1.set_size(nx);
  nx = 0;
  for (i = 0; i <= end; i++) {
    if (r[i]) {
      r1[nx] = i + 1;
      nx++;
    }
  }

  nx = r1.size(0);
  y.set_size(r1.size(0));
  for (end = 0; end < nx; end++) {
    y[end] = std::abs(costMatrix[r1[end] - 1]);
  }

  n = y.size(0);
  if (y.size(0) <= 2) {
    if (y.size(0) == 1) {
      maxValue = y[0];
    } else if ((y[0] < y[1]) || (rtIsNaN(y[0]) && (!rtIsNaN(y[1])))) {
      maxValue = y[1];
    } else {
      maxValue = y[0];
    }
  } else {
    if (!rtIsNaN(y[0])) {
      nx = 1;
    } else {
      nx = 0;
      end = 2;
      exitg1 = false;
      while ((!exitg1) && (end <= y.size(0))) {
        if (!rtIsNaN(y[end - 1])) {
          nx = end;
          exitg1 = true;
        } else {
          end++;
        }
      }
    }

    if (nx == 0) {
      maxValue = y[0];
    } else {
      maxValue = y[nx - 1];
      i = nx + 1;
      for (end = i; end <= n; end++) {
        x = y[end - 1];
        if (maxValue < x) {
          maxValue = x;
        }
      }
    }
  }

  if ((2.2204460492503131E-16 > maxValue) || rtIsNaN(maxValue)) {
    maxValue = 2.2204460492503131E-16;
  }

  n = costMatrix.size(0) * costMatrix.size(1);
  for (i = 0; i < n; i++) {
    costMatrix[i] = costMatrix[i] / maxValue;
  }

  n = costMatrix.size(0) * costMatrix.size(1);
  for (i = 0; i < n; i++) {
    costMatrix[i] = costMatrix[i] * 1.0E+10;
  }

  nx = costMatrix.size(0) * costMatrix.size(1);
  for (end = 0; end < nx; end++) {
    costMatrix[end] = rt_roundd_snf(costMatrix[end]);
  }

  n = costMatrix.size(0) * costMatrix.size(1);
  for (i = 0; i < n; i++) {
    costMatrix[i] = costMatrix[i] / 1.0E+10;
  }

  isColAssigned.set_size(costMatrix.size(0), costMatrix.size(1));
  n = costMatrix.size(0) * costMatrix.size(1);
  for (i = 0; i < n; i++) {
    isColAssigned[i] = rtIsInf(costMatrix[i]);
  }

  r2.set_size(costMatrix.size(0), costMatrix.size(1));
  n = costMatrix.size(0) * costMatrix.size(1);
  for (i = 0; i < n; i++) {
    r2[i] = rtIsNaN(costMatrix[i]);
  }

  n = isColAssigned.size(0) * isColAssigned.size(1);
  for (i = 0; i < n; i++) {
    isColAssigned[i] = (isColAssigned[i] || r2[i]);
  }

  end = isColAssigned.size(0) * isColAssigned.size(1) - 1;
  nx = 0;
  for (i = 0; i <= end; i++) {
    if (isColAssigned[i]) {
      nx++;
    }
  }

  r3.set_size(nx);
  nx = 0;
  for (i = 0; i <= end; i++) {
    if (isColAssigned[i]) {
      r3[nx] = i + 1;
      nx++;
    }
  }

  end = isColAssigned.size(0) * isColAssigned.size(1) - 1;
  nx = 0;
  for (i = 0; i <= end; i++) {
    if (isColAssigned[i]) {
      nx++;
    }
  }

  r4.set_size(nx);
  nx = 0;
  for (i = 0; i <= end; i++) {
    if (isColAssigned[i]) {
      r4[nx] = i + 1;
      nx++;
    }
  }

  y.set_size(r4.size(0));
  n = r4.size(0);
  for (i = 0; i < n; i++) {
    y[i] = costMatrix[r4[i] - 1];
  }

  nx = r4.size(0);
  for (end = 0; end < nx; end++) {
    x = y[end];
    if (y[end] < 0.0) {
      x = -1.0;
    } else if (y[end] > 0.0) {
      x = 1.0;
    } else {
      if (y[end] == 0.0) {
        x = 0.0;
      }
    }

    y[end] = x;
  }

  n = y.size(0);
  for (i = 0; i < n; i++) {
    costMatrix[r3[i] - 1] = y[i] * 2.1;
  }

  price.set_size(1, costMatrix.size(1));
  n = costMatrix.size(1);
  for (i = 0; i < n; i++) {
    price[i] = 0.0;
  }

  colSoln.set_size(costMatrix.size(0), costMatrix.size(1));
  n = costMatrix.size(0) * costMatrix.size(1);
  for (i = 0; i < n; i++) {
    colSoln[i] = costMatrix[i];
  }

  lapRemoveImpossibles(colSoln, rowIdx, colIdx, b_unassignedRows,
                       b_unassignedColumns);
  b_x[0] = (colSoln.size(0) == costMatrix.size(0));
  b_x[1] = (colSoln.size(1) == costMatrix.size(1));
  b_y = true;
  end = 0;
  exitg1 = false;
  while ((!exitg1) && (end <= 1)) {
    if (!b_x[end]) {
      b_y = false;
      exitg1 = true;
    } else {
      end++;
    }
  }

  if (!b_y) {
    unsigned int u;
    assignments.set_size(0, 2);
    if (costMatrix.size(0) < 1) {
      b_colSoln.set_size(1, 0);
    } else {
      b_colSoln.set_size(1, (static_cast<int>(static_cast<double>
        (costMatrix.size(0)) - 1.0) + 1));
      n = static_cast<int>(static_cast<double>(costMatrix.size(0)) - 1.0);
      for (i = 0; i <= n; i++) {
        b_colSoln[i] = static_cast<double>(i) + 1.0;
      }
    }

    unassignedRows.set_size(1, b_colSoln.size(1));
    n = b_colSoln.size(0) * b_colSoln.size(1);
    for (i = 0; i < n; i++) {
      x = rt_roundd_snf(b_colSoln[i]);
      if (x < 4.294967296E+9) {
        if (x >= 0.0) {
          u = static_cast<unsigned int>(x);
        } else {
          u = 0U;
        }
      } else if (x >= 4.294967296E+9) {
        u = MAX_uint32_T;
      } else {
        u = 0U;
      }

      unassignedRows[i] = u;
    }

    if (costMatrix.size(1) < 1) {
      b_colSoln.set_size(1, 0);
    } else {
      b_colSoln.set_size(1, (static_cast<int>(static_cast<double>
        (costMatrix.size(1)) - 1.0) + 1));
      n = static_cast<int>(static_cast<double>(costMatrix.size(1)) - 1.0);
      for (i = 0; i <= n; i++) {
        b_colSoln[i] = static_cast<double>(i) + 1.0;
      }
    }

    unassignedColumns.set_size(1, b_colSoln.size(1));
    n = b_colSoln.size(0) * b_colSoln.size(1);
    for (i = 0; i < n; i++) {
      x = rt_roundd_snf(b_colSoln[i]);
      if (x < 4.294967296E+9) {
        if (x >= 0.0) {
          u = static_cast<unsigned int>(x);
        } else {
          u = 0U;
        }
      } else if (x >= 4.294967296E+9) {
        u = MAX_uint32_T;
      } else {
        u = 0U;
      }

      unassignedColumns[i] = u;
    }
  } else {
    lapAuction(costMatrix, rowSoln, b_colSoln, priceColn);
    i = priceColn.size(0) * priceColn.size(1);
    priceColn.set_size(1, priceColn.size(1));
    n = i - 1;
    for (i = 0; i <= n; i++) {
      priceColn[i] = priceColn[i] * maxValue;
    }

    if (1 > costMatrix.size(0)) {
      end = 0;
    } else {
      end = costMatrix.size(0);
    }

    rowSoln.set_size(end);
    if (1 > costMatrix.size(1)) {
      n = 0;
    } else {
      n = costMatrix.size(1);
    }

    colSoln.set_size(1, n);
    for (i = 0; i < n; i++) {
      colSoln[i] = b_colSoln[i];
    }

    if (1 > costMatrix.size(1)) {
      nx = 0;
    } else {
      nx = costMatrix.size(1);
    }

    price.set_size(1, nx);
    for (i = 0; i < nx; i++) {
      price[i] = priceColn[i];
    }

    for (i = 0; i < end; i++) {
      if (rowSoln[i] > costMatrix.size(1)) {
        rowSoln[i] = rtNaN;
      }
    }

    end = n - 1;
    for (i = 0; i <= end; i++) {
      if (colSoln[i] > costMatrix.size(0)) {
        colSoln[i] = rtNaN;
      }
    }

    r.set_size(rowSoln.size(0));
    n = rowSoln.size(0);
    for (i = 0; i < n; i++) {
      r[i] = rtIsNaN(rowSoln[i]);
    }

    isRowAssigned.set_size(r.size(0));
    n = r.size(0);
    for (i = 0; i < n; i++) {
      isRowAssigned[i] = !r[i];
    }

    isColAssigned.set_size(1, colSoln.size(1));
    n = colSoln.size(0) * colSoln.size(1);
    for (i = 0; i < n; i++) {
      isColAssigned[i] = rtIsNaN(colSoln[i]);
    }

    i = isColAssigned.size(0) * isColAssigned.size(1);
    isColAssigned.set_size(1, isColAssigned.size(1));
    n = i - 1;
    for (i = 0; i <= n; i++) {
      isColAssigned[i] = !isColAssigned[i];
    }

    end = isRowAssigned.size(0) - 1;
    nx = 0;
    for (i = 0; i <= end; i++) {
      if (isRowAssigned[i]) {
        nx++;
      }
    }

    r5.set_size(nx);
    nx = 0;
    for (i = 0; i <= end; i++) {
      if (isRowAssigned[i]) {
        r5[nx] = i + 1;
        nx++;
      }
    }

    c_unassignedColumns.set_size(r5.size(0));
    n = r5.size(0);
    for (i = 0; i < n; i++) {
      c_unassignedColumns[i] = colIdx[static_cast<int>(rowSoln[r5[i] - 1]) - 1];
    }

    result.set_size(r5.size(0), 2);
    n = r5.size(0);
    for (i = 0; i < n; i++) {
      result[i] = rowIdx[r5[i] - 1];
    }

    n = c_unassignedColumns.size(0);
    for (i = 0; i < n; i++) {
      result[i + result.size(0)] = c_unassignedColumns[i];
    }

    end = isRowAssigned.size(0) - 1;
    nx = 0;
    for (i = 0; i <= end; i++) {
      if (!isRowAssigned[i]) {
        nx++;
      }
    }

    r6.set_size(nx);
    nx = 0;
    for (i = 0; i <= end; i++) {
      if (!isRowAssigned[i]) {
        r6[nx] = i + 1;
        nx++;
      }
    }

    c_unassignedRows.set_size((b_unassignedRows.size(0) + r6.size(0)));
    n = b_unassignedRows.size(0);
    for (i = 0; i < n; i++) {
      c_unassignedRows[i] = b_unassignedRows[i];
    }

    n = r6.size(0);
    for (i = 0; i < n; i++) {
      c_unassignedRows[i + b_unassignedRows.size(0)] = rowIdx[r6[i] - 1];
    }

    end = isColAssigned.size(1) - 1;
    nx = 0;
    for (i = 0; i <= end; i++) {
      if (!isColAssigned[i]) {
        nx++;
      }
    }

    r7.set_size(1, nx);
    nx = 0;
    for (i = 0; i <= end; i++) {
      if (!isColAssigned[i]) {
        r7[nx] = i + 1;
        nx++;
      }
    }

    c_unassignedColumns.set_size((b_unassignedColumns.size(1) + r7.size(1)));
    n = b_unassignedColumns.size(1);
    for (i = 0; i < n; i++) {
      c_unassignedColumns[i] = b_unassignedColumns[i];
    }

    n = r7.size(1);
    for (i = 0; i < n; i++) {
      c_unassignedColumns[i + b_unassignedColumns.size(1)] = colIdx[r7[i] - 1];
    }

    assignments.set_size(((result.size(0) << 1) / 2), 2);
    n = ((result.size(0) << 1) / 2) << 1;
    for (i = 0; i < n; i++) {
      assignments[i] = result[i];
    }

    unassignedRows.set_size(c_unassignedRows.size(0), 1);
    n = c_unassignedRows.size(0);
    for (i = 0; i < n; i++) {
      unassignedRows[i] = c_unassignedRows[i];
    }

    unassignedColumns.set_size(c_unassignedColumns.size(0), 1);
    n = c_unassignedColumns.size(0);
    for (i = 0; i < n; i++) {
      unassignedColumns[i] = c_unassignedColumns[i];
    }
  }
}

// End of code generation (assignAuctionWithPrice.cpp)
