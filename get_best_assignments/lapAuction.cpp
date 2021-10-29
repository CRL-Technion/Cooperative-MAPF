//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  lapAuction.cpp
//
//  Code generation for function 'lapAuction'
//


// Include files
#include "lapAuction.h"
#include "ITaskAssignment.h"
#include "assign2DRelaxed.h"
#include "assignAuctionWithPrice.h"
#include "assignkbestsd.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include "sortIdx.h"
#include "unique.h"

// Function Declarations
static void auction(const coder::array<double, 2U> &costMatrix, double epsilon,
                    coder::array<double, 1U> &rowSoln, coder::array<double, 2U>
                    &colSoln, coder::array<double, 2U> &colRedux);
static void findAllMin(const coder::array<double, 2U> &values, double *minValue,
  coder::array<double, 2U> &minIndicies);

// Function Definitions
static void auction(const coder::array<double, 2U> &costMatrix, double epsilon,
                    coder::array<double, 1U> &rowSoln, coder::array<double, 2U>
                    &colSoln, coder::array<double, 2U> &colRedux)
{
  coder::array<boolean_T, 1U> b;
  int loop_ub;
  int i;
  coder::array<boolean_T, 1U> r;
  int nx;
  int idx;
  coder::array<int, 1U> ii;
  boolean_T exitg1;
  int i1;
  int b_loop_ub;
  coder::array<boolean_T, 2U> r1;
  coder::array<double, 2U> minInds;
  double rowPrev;
  coder::array<double, 2U> b_minInds;
  coder::array<int, 2U> r2;
  coder::array<boolean_T, 2U> x;
  b.set_size(rowSoln.size(0));
  loop_ub = rowSoln.size(0);
  for (i = 0; i < loop_ub; i++) {
    b[i] = rtIsNaN(rowSoln[i]);
  }

  r.set_size(rowSoln.size(0));
  loop_ub = rowSoln.size(0);
  for (i = 0; i < loop_ub; i++) {
    r[i] = rtIsNaN(rowSoln[i]);
  }

  nx = b.size(0);
  idx = 0;
  ii.set_size(r.size(0));
  loop_ub = 0;
  exitg1 = false;
  while ((!exitg1) && (loop_ub <= nx - 1)) {
    if (b[loop_ub]) {
      idx++;
      ii[idx - 1] = loop_ub + 1;
      if (idx >= nx) {
        exitg1 = true;
      } else {
        loop_ub++;
      }
    } else {
      loop_ub++;
    }
  }

  if (r.size(0) == 1) {
    if (idx == 0) {
      ii.set_size(0);
    }
  } else {
    if (1 > idx) {
      idx = 0;
    }

    ii.set_size(idx);
  }

  i = ii.size(0);
  if (0 <= ii.size(0) - 1) {
    i1 = costMatrix.size(1);
    b_loop_ub = costMatrix.size(1);
  }

  for (int thisRow = 0; thisRow < i; thisRow++) {
    double bestCol;
    r1.set_size(1, colSoln.size(1));
    loop_ub = colSoln.size(0) * colSoln.size(1);
    for (idx = 0; idx < loop_ub; idx++) {
      r1[idx] = rtIsNaN(colSoln[idx]);
    }

    minInds.set_size(1, i1);
    for (idx = 0; idx < b_loop_ub; idx++) {
      minInds[idx] = costMatrix[(ii[thisRow] + costMatrix.size(0) * idx) - 1] -
        colRedux[idx];
    }

    findAllMin(minInds, &rowPrev, b_minInds);
    bestCol = b_minInds[0];
    if (b_minInds.size(1) > 1) {
      boolean_T y;
      loop_ub = b_minInds.size(1) - 1;
      minInds.set_size(1, (b_minInds.size(1) - 1));
      for (idx = 0; idx < loop_ub; idx++) {
        minInds[idx] = b_minInds[idx + 1];
      }

      x.set_size(1, (b_minInds.size(1) - 1));
      for (idx = 0; idx < loop_ub; idx++) {
        x[idx] = r1[static_cast<int>(b_minInds[idx + 1]) - 1];
      }

      y = false;
      nx = 1;
      exitg1 = false;
      while ((!exitg1) && (nx <= loop_ub)) {
        if (!x[nx - 1]) {
          nx++;
        } else {
          y = true;
          exitg1 = true;
        }
      }

      if (y) {
        nx = b_minInds.size(1) - 2;
        loop_ub = 0;
        idx = 0;
        for (int b_i = 0; b_i <= nx; b_i++) {
          if (r1[static_cast<int>(b_minInds[b_i + 1]) - 1]) {
            loop_ub++;
          }

          if (r1[static_cast<int>(minInds[b_i]) - 1]) {
            minInds[idx] = minInds[b_i];
            idx++;
          }
        }

        minInds.set_size(1, loop_ub);
        bestCol = minInds[0];
      }

      rowPrev = 0.0;
    } else {
      double min2;
      r2.set_size(1, b_minInds.size(1));
      loop_ub = b_minInds.size(0) * b_minInds.size(1);
      for (idx = 0; idx < loop_ub; idx++) {
        r2[idx] = static_cast<int>(b_minInds[idx]);
      }

      loop_ub = r2.size(0) * r2.size(1) - 1;
      for (idx = 0; idx <= loop_ub; idx++) {
        minInds[r2[idx] - 1] = rtNaN;
      }

      nx = minInds.size(1);
      if (minInds.size(1) <= 2) {
        if (minInds.size(1) == 1) {
          min2 = minInds[0];
        } else if ((minInds[0] > minInds[1]) || (rtIsNaN(minInds[0]) &&
                    (!rtIsNaN(minInds[1])))) {
          min2 = minInds[1];
        } else {
          min2 = minInds[0];
        }
      } else {
        if (!rtIsNaN(minInds[0])) {
          idx = 1;
        } else {
          idx = 0;
          loop_ub = 2;
          exitg1 = false;
          while ((!exitg1) && (loop_ub <= minInds.size(1))) {
            if (!rtIsNaN(minInds[loop_ub - 1])) {
              idx = loop_ub;
              exitg1 = true;
            } else {
              loop_ub++;
            }
          }
        }

        if (idx == 0) {
          min2 = minInds[0];
        } else {
          min2 = minInds[idx - 1];
          idx++;
          for (loop_ub = idx; loop_ub <= nx; loop_ub++) {
            double d;
            d = minInds[loop_ub - 1];
            if (min2 > d) {
              min2 = d;
            }
          }
        }
      }

      rowPrev = min2 - rowPrev;
    }

    idx = static_cast<int>(bestCol) - 1;
    colRedux[idx] = colRedux[idx] - (rowPrev + epsilon);
    rowPrev = colSoln[idx];
    colSoln[idx] = ii[thisRow];
    rowSoln[ii[thisRow] - 1] = bestCol;
    if (!rtIsNaN(rowPrev)) {
      rowSoln[static_cast<int>(rowPrev) - 1] = rtNaN;
    }
  }
}

static void findAllMin(const coder::array<double, 2U> &values, double *minValue,
  coder::array<double, 2U> &minIndicies)
{
  int n;
  int idx;
  double ex;
  int k;
  boolean_T exitg1;
  coder::array<boolean_T, 2U> x;
  coder::array<int, 2U> ii;
  n = values.size(1);
  if (values.size(1) <= 2) {
    if (values.size(1) == 1) {
      ex = values[0];
    } else if ((values[0] > values[1]) || (rtIsNaN(values[0]) && (!rtIsNaN
                 (values[1])))) {
      ex = values[1];
    } else {
      ex = values[0];
    }
  } else {
    if (!rtIsNaN(values[0])) {
      idx = 1;
    } else {
      idx = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k <= values.size(1))) {
        if (!rtIsNaN(values[k - 1])) {
          idx = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }

    if (idx == 0) {
      ex = values[0];
    } else {
      ex = values[idx - 1];
      idx++;
      for (k = idx; k <= n; k++) {
        double d;
        d = values[k - 1];
        if (ex > d) {
          ex = d;
        }
      }
    }
  }

  x.set_size(1, values.size(1));
  n = values.size(0) * values.size(1);
  for (idx = 0; idx < n; idx++) {
    x[idx] = (values[idx] == ex);
  }

  n = x.size(1);
  idx = 0;
  ii.set_size(1, x.size(1));
  k = 0;
  exitg1 = false;
  while ((!exitg1) && (k <= n - 1)) {
    if (x[k]) {
      idx++;
      ii[idx - 1] = k + 1;
      if (idx >= n) {
        exitg1 = true;
      } else {
        k++;
      }
    } else {
      k++;
    }
  }

  if (x.size(1) == 1) {
    if (idx == 0) {
      ii.set_size(1, 0);
    }
  } else {
    if (1 > idx) {
      idx = 0;
    }

    ii.set_size(ii.size(0), idx);
  }

  minIndicies.set_size(1, ii.size(1));
  n = ii.size(0) * ii.size(1);
  for (idx = 0; idx < n; idx++) {
    minIndicies[idx] = ii[idx];
  }

  *minValue = ex;
}

void lapAuction(const coder::array<double, 2U> &costMatrix, coder::array<double,
                1U> &rowSoln, coder::array<double, 2U> &colSoln, coder::array<
                double, 2U> &colRedux)
{
  int idx;
  int dimSize;
  coder::array<boolean_T, 1U> r;
  coder::array<boolean_T, 1U> x;
  coder::array<boolean_T, 1U> r1;
  coder::array<int, 1U> r2;
  coder::array<int, 1U> r3;
  coder::array<int, 1U> r4;
  coder::array<double, 1U> b_y1;
  coder::array<double, 1U> costs;
  double work_data_idx_0;
  double tmp1;
  rowSoln.set_size(costMatrix.size(0));
  idx = costMatrix.size(0);
  for (dimSize = 0; dimSize < idx; dimSize++) {
    rowSoln[dimSize] = rtNaN;
  }

  colSoln.set_size(1, costMatrix.size(1));
  idx = costMatrix.size(1);
  for (dimSize = 0; dimSize < idx; dimSize++) {
    colSoln[dimSize] = rtNaN;
  }

  colRedux.set_size(1, costMatrix.size(1));
  idx = costMatrix.size(1);
  for (dimSize = 0; dimSize < idx; dimSize++) {
    colRedux[dimSize] = 0.0;
  }

  r.set_size(costMatrix.size(0));
  idx = costMatrix.size(0);
  for (dimSize = 0; dimSize < idx; dimSize++) {
    r[dimSize] = true;
  }

  x.set_size(r.size(0));
  idx = r.size(0);
  for (dimSize = 0; dimSize < idx; dimSize++) {
    x[dimSize] = r[dimSize];
  }

  if (1 <= x.size(0)) {
    int n;
    double ex;
    int k;
    boolean_T exitg1;
    double b_ex;
    boolean_T lastPhase;
    r.set_size((costMatrix.size(0) * costMatrix.size(1)));
    idx = costMatrix.size(0) * costMatrix.size(1);
    for (dimSize = 0; dimSize < idx; dimSize++) {
      r[dimSize] = rtIsInf(costMatrix[dimSize]);
    }

    x.set_size((costMatrix.size(0) * costMatrix.size(1)));
    idx = costMatrix.size(0) * costMatrix.size(1);
    for (dimSize = 0; dimSize < idx; dimSize++) {
      x[dimSize] = rtIsNaN(costMatrix[dimSize]);
    }

    idx = r.size(0);
    for (dimSize = 0; dimSize < idx; dimSize++) {
      r[dimSize] = ((!r[dimSize]) && (!x[dimSize]));
    }

    x.set_size((costMatrix.size(0) * costMatrix.size(1)));
    idx = costMatrix.size(0) * costMatrix.size(1);
    for (dimSize = 0; dimSize < idx; dimSize++) {
      x[dimSize] = rtIsInf(costMatrix[dimSize]);
    }

    r1.set_size((costMatrix.size(0) * costMatrix.size(1)));
    idx = costMatrix.size(0) * costMatrix.size(1);
    for (dimSize = 0; dimSize < idx; dimSize++) {
      r1[dimSize] = rtIsNaN(costMatrix[dimSize]);
    }

    idx = x.size(0);
    for (dimSize = 0; dimSize < idx; dimSize++) {
      x[dimSize] = ((!x[dimSize]) && (!r1[dimSize]));
    }

    n = r.size(0) - 1;
    idx = 0;
    for (dimSize = 0; dimSize <= n; dimSize++) {
      if (r[dimSize]) {
        idx++;
      }
    }

    r2.set_size(idx);
    idx = 0;
    for (dimSize = 0; dimSize <= n; dimSize++) {
      if (r[dimSize]) {
        r2[idx] = dimSize + 1;
        idx++;
      }
    }

    n = x.size(0) - 1;
    idx = 0;
    for (dimSize = 0; dimSize <= n; dimSize++) {
      if (x[dimSize]) {
        idx++;
      }
    }

    r3.set_size(idx);
    idx = 0;
    for (dimSize = 0; dimSize <= n; dimSize++) {
      if (x[dimSize]) {
        r3[idx] = dimSize + 1;
        idx++;
      }
    }

    n = r2.size(0);
    if (r2.size(0) <= 2) {
      if (r2.size(0) == 1) {
        ex = costMatrix[r2[0] - 1];
      } else if ((costMatrix[r2[0] - 1] < costMatrix[r2[1] - 1]) || (rtIsNaN
                  (costMatrix[r2[0] - 1]) && (!rtIsNaN(costMatrix[r2[1] - 1]))))
      {
        ex = costMatrix[r2[1] - 1];
      } else {
        ex = costMatrix[r2[0] - 1];
      }
    } else {
      if (!rtIsNaN(costMatrix[r2[0] - 1])) {
        idx = 1;
      } else {
        idx = 0;
        k = 2;
        exitg1 = false;
        while ((!exitg1) && (k <= r2.size(0))) {
          if (!rtIsNaN(costMatrix[r2[k - 1] - 1])) {
            idx = k;
            exitg1 = true;
          } else {
            k++;
          }
        }
      }

      if (idx == 0) {
        ex = costMatrix[r2[0] - 1];
      } else {
        ex = costMatrix[r2[idx - 1] - 1];
        dimSize = idx + 1;
        for (k = dimSize; k <= n; k++) {
          idx = r2[k - 1] - 1;
          if (ex < costMatrix[idx]) {
            ex = costMatrix[idx];
          }
        }
      }
    }

    n = r3.size(0);
    if (r3.size(0) <= 2) {
      if (r3.size(0) == 1) {
        b_ex = costMatrix[r3[0] - 1];
      } else if ((costMatrix[r3[0] - 1] > costMatrix[r3[1] - 1]) || (rtIsNaN
                  (costMatrix[r3[0] - 1]) && (!rtIsNaN(costMatrix[r3[1] - 1]))))
      {
        b_ex = costMatrix[r3[1] - 1];
      } else {
        b_ex = costMatrix[r3[0] - 1];
      }
    } else {
      if (!rtIsNaN(costMatrix[r3[0] - 1])) {
        idx = 1;
      } else {
        idx = 0;
        k = 2;
        exitg1 = false;
        while ((!exitg1) && (k <= r3.size(0))) {
          if (!rtIsNaN(costMatrix[r3[k - 1] - 1])) {
            idx = k;
            exitg1 = true;
          } else {
            k++;
          }
        }
      }

      if (idx == 0) {
        b_ex = costMatrix[r3[0] - 1];
      } else {
        b_ex = costMatrix[r3[idx - 1] - 1];
        dimSize = idx + 1;
        for (k = dimSize; k <= n; k++) {
          idx = r3[k - 1] - 1;
          if (b_ex > costMatrix[idx]) {
            b_ex = costMatrix[idx];
          }
        }
      }
    }

    r.set_size((costMatrix.size(0) * costMatrix.size(1)));
    idx = costMatrix.size(0) * costMatrix.size(1);
    for (dimSize = 0; dimSize < idx; dimSize++) {
      r[dimSize] = rtIsInf(costMatrix[dimSize]);
    }

    x.set_size((costMatrix.size(0) * costMatrix.size(1)));
    idx = costMatrix.size(0) * costMatrix.size(1);
    for (dimSize = 0; dimSize < idx; dimSize++) {
      x[dimSize] = rtIsNaN(costMatrix[dimSize]);
    }

    idx = r.size(0);
    for (dimSize = 0; dimSize < idx; dimSize++) {
      r[dimSize] = ((!r[dimSize]) && (!x[dimSize]));
    }

    n = r.size(0) - 1;
    idx = 0;
    for (dimSize = 0; dimSize <= n; dimSize++) {
      if (r[dimSize]) {
        idx++;
      }
    }

    r4.set_size(idx);
    idx = 0;
    for (dimSize = 0; dimSize <= n; dimSize++) {
      if (r[dimSize]) {
        r4[idx] = dimSize + 1;
        idx++;
      }
    }

    b_y1.set_size(r4.size(0));
    idx = r4.size(0);
    for (dimSize = 0; dimSize < idx; dimSize++) {
      b_y1[dimSize] = costMatrix[r4[dimSize] - 1];
    }

    unique_vector(b_y1, costs);
    dimSize = costs.size(0);
    if (costs.size(0) == 0) {
      b_y1.set_size(0);
    } else {
      idx = costs.size(0) - 1;
      if (idx >= 1) {
        idx = 1;
      }

      if (idx < 1) {
        b_y1.set_size(0);
      } else {
        b_y1.set_size((costs.size(0) - 1));
        if (costs.size(0) - 1 != 0) {
          idx = 1;
          n = 0;
          work_data_idx_0 = costs[0];
          for (k = 2; k <= dimSize; k++) {
            tmp1 = costs[idx] - work_data_idx_0;
            work_data_idx_0 = costs[idx];
            idx++;
            b_y1[n] = tmp1;
            n++;
          }
        }
      }
    }

    n = b_y1.size(0);
    if (b_y1.size(0) <= 2) {
      if (b_y1.size(0) == 1) {
        work_data_idx_0 = b_y1[0];
      } else if ((b_y1[0] > b_y1[1]) || (rtIsNaN(b_y1[0]) && (!rtIsNaN(b_y1[1]))))
      {
        work_data_idx_0 = b_y1[1];
      } else {
        work_data_idx_0 = b_y1[0];
      }
    } else {
      if (!rtIsNaN(b_y1[0])) {
        idx = 1;
      } else {
        idx = 0;
        k = 2;
        exitg1 = false;
        while ((!exitg1) && (k <= b_y1.size(0))) {
          if (!rtIsNaN(b_y1[k - 1])) {
            idx = k;
            exitg1 = true;
          } else {
            k++;
          }
        }
      }

      if (idx == 0) {
        work_data_idx_0 = b_y1[0];
      } else {
        work_data_idx_0 = b_y1[idx - 1];
        dimSize = idx + 1;
        for (k = dimSize; k <= n; k++) {
          tmp1 = b_y1[k - 1];
          if (work_data_idx_0 > tmp1) {
            work_data_idx_0 = tmp1;
          }
        }
      }
    }

    tmp1 = (ex - b_ex) / 4.0 / (static_cast<double>(costMatrix.size(0)) + 1.0);
    work_data_idx_0 /= static_cast<double>(costMatrix.size(0)) + 1.0;
    lastPhase = false;
    while (!lastPhase) {
      rowSoln.set_size(costMatrix.size(0));
      idx = costMatrix.size(0);
      for (dimSize = 0; dimSize < idx; dimSize++) {
        rowSoln[dimSize] = rtNaN;
      }

      colSoln.set_size(1, costMatrix.size(1));
      idx = costMatrix.size(1);
      for (dimSize = 0; dimSize < idx; dimSize++) {
        colSoln[dimSize] = rtNaN;
      }

      lastPhase = (tmp1 <= work_data_idx_0);
      int exitg2;
      do {
        boolean_T y;
        exitg2 = 0;
        r.set_size(rowSoln.size(0));
        idx = rowSoln.size(0);
        for (dimSize = 0; dimSize < idx; dimSize++) {
          r[dimSize] = rtIsNaN(rowSoln[dimSize]);
        }

        x.set_size(r.size(0));
        idx = r.size(0);
        for (dimSize = 0; dimSize < idx; dimSize++) {
          x[dimSize] = r[dimSize];
        }

        y = false;
        idx = 1;
        exitg1 = false;
        while ((!exitg1) && (idx <= x.size(0))) {
          if (!x[idx - 1]) {
            idx++;
          } else {
            y = true;
            exitg1 = true;
          }
        }

        if (y) {
          auction(costMatrix, tmp1, rowSoln, colSoln, colRedux);
        } else {
          exitg2 = 1;
        }
      } while (exitg2 == 0);

      tmp1 /= 7.0;
      if ((!(tmp1 > work_data_idx_0)) && (!rtIsNaN(work_data_idx_0))) {
        tmp1 = work_data_idx_0;
      }
    }
  }
}

// End of code generation (lapAuction.cpp)
