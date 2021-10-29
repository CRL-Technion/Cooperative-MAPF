//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  assign2DRelaxed.cpp
//
//  Code generation for function 'assign2DRelaxed'
//


// Include files
#include "assign2DRelaxed.h"
#include "ITaskAssignment.h"
#include "assignAuctionWithPrice.h"
#include "assignkbestsd.h"
#include "assignsd.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include "sortIdx.h"
#include "sortrows.h"

// Function Declarations
static void padCostMatrix(const coder::array<double, 2U> &costMatrix, coder::
  array<double, 2U> &paddedCost);

// Function Definitions
static void padCostMatrix(const coder::array<double, 2U> &costMatrix, coder::
  array<double, 2U> &paddedCost)
{
  int i;
  int i1;
  int i2;
  int i3;
  int result;
  int i4;
  int sizes_idx_0;
  int sizes_idx_1;
  int loop_ub_tmp;
  int b_loop_ub_tmp;
  int siz_idx_0_tmp;
  double dummyCostFirst_data[2601];
  coder::array<double, 2U> dummyCostSecond;
  int loop_ub;
  coder::array<double, 2U> y;
  coder::array<double, 2U> b_y;
  coder::array<int, 2U> r;
  coder::array<signed char, 2U> dummyDummyAssignment;
  boolean_T empty_non_axis_sizes;
  signed char input_sizes_idx_1;
  cell_wrap_11 reshapes[2];
  if (2 > costMatrix.size(0)) {
    i = -1;
    i1 = -1;
  } else {
    i = 0;
    i1 = costMatrix.size(0) - 1;
  }

  if (2 > costMatrix.size(1)) {
    i2 = -1;
    i3 = -1;
    result = -1;
    i4 = -1;
  } else {
    i2 = 0;
    i3 = costMatrix.size(1) - 1;
    result = 0;
    i4 = costMatrix.size(1) - 1;
  }

  if (2 > costMatrix.size(0)) {
    sizes_idx_0 = -1;
    sizes_idx_1 = -1;
  } else {
    sizes_idx_0 = 0;
    sizes_idx_1 = costMatrix.size(0) - 1;
  }

  loop_ub_tmp = i4 - result;
  b_loop_ub_tmp = loop_ub_tmp * loop_ub_tmp;
  for (i4 = 0; i4 < b_loop_ub_tmp; i4++) {
    dummyCostFirst_data[i4] = rtInf;
  }

  siz_idx_0_tmp = sizes_idx_1 - sizes_idx_0;
  dummyCostSecond.set_size(siz_idx_0_tmp, siz_idx_0_tmp);
  b_loop_ub_tmp = siz_idx_0_tmp * siz_idx_0_tmp;
  for (i4 = 0; i4 < b_loop_ub_tmp; i4++) {
    dummyCostSecond[i4] = rtInf;
  }

  if (loop_ub_tmp < 1) {
    y.set_size(1, 0);
    b_y.set_size(1, 0);
  } else {
    loop_ub = loop_ub_tmp - 1;
    y.set_size(1, (loop_ub + 1));
    for (i4 = 0; i4 <= loop_ub; i4++) {
      y[i4] = static_cast<double>(i4) + 1.0;
    }

    loop_ub = loop_ub_tmp - 1;
    b_y.set_size(1, (loop_ub + 1));
    for (i4 = 0; i4 <= loop_ub; i4++) {
      b_y[i4] = static_cast<double>(i4) + 1.0;
    }
  }

  for (i4 = 0; i4 < loop_ub_tmp; i4++) {
    dummyCostFirst_data[(static_cast<int>(y[i4]) + static_cast<signed char>
                         (loop_ub_tmp) * (static_cast<int>(b_y[i4]) - 1)) - 1] =
      costMatrix[costMatrix.size(0) * ((result + i4) + 1)];
  }

  if (siz_idx_0_tmp < 1) {
    y.set_size(1, 0);
    b_y.set_size(1, 0);
  } else {
    loop_ub = siz_idx_0_tmp - 1;
    y.set_size(1, (loop_ub + 1));
    for (result = 0; result <= loop_ub; result++) {
      y[result] = static_cast<double>(result) + 1.0;
    }

    loop_ub = siz_idx_0_tmp - 1;
    b_y.set_size(1, (loop_ub + 1));
    for (result = 0; result <= loop_ub; result++) {
      b_y[result] = static_cast<double>(result) + 1.0;
    }
  }

  r.set_size(1, y.size(1));
  loop_ub = y.size(0) * y.size(1);
  for (result = 0; result < loop_ub; result++) {
    r[result] = static_cast<int>(y[result]) + siz_idx_0_tmp * (static_cast<int>
      (b_y[result]) - 1);
  }

  loop_ub = r.size(1);
  for (result = 0; result < loop_ub; result++) {
    dummyCostSecond[r[result] - 1] = costMatrix[(sizes_idx_0 + result) + 1];
  }

  dummyDummyAssignment.set_size(loop_ub_tmp, siz_idx_0_tmp);
  b_loop_ub_tmp = loop_ub_tmp * siz_idx_0_tmp;
  for (result = 0; result < b_loop_ub_tmp; result++) {
    dummyDummyAssignment[result] = 0;
  }

  loop_ub = i1 - i;
  sizes_idx_0 = i3 - i2;
  if ((loop_ub != 0) && (sizes_idx_0 != 0)) {
    result = loop_ub;
  } else if ((dummyCostSecond.size(0) != 0) && (dummyCostSecond.size(1) != 0)) {
    result = dummyCostSecond.size(0);
  } else {
    if (loop_ub > 0) {
      result = loop_ub;
    } else {
      result = 0;
    }

    if (dummyCostSecond.size(0) > result) {
      result = dummyCostSecond.size(0);
    }
  }

  empty_non_axis_sizes = (result == 0);
  if (empty_non_axis_sizes || ((loop_ub != 0) && (sizes_idx_0 != 0))) {
    input_sizes_idx_1 = static_cast<signed char>(sizes_idx_0);
  } else {
    input_sizes_idx_1 = 0;
  }

  y.set_size(loop_ub, sizes_idx_0);
  for (i1 = 0; i1 < sizes_idx_0; i1++) {
    for (i3 = 0; i3 < loop_ub; i3++) {
      y[i3 + y.size(0) * i1] = costMatrix[((i + i3) + costMatrix.size(0) * ((i2
        + i1) + 1)) + 1];
    }
  }

  reshapes[0].f1.set_size(result, (static_cast<int>(input_sizes_idx_1)));
  loop_ub = result * input_sizes_idx_1;
  for (i = 0; i < loop_ub; i++) {
    reshapes[0].f1[i] = y[i];
  }

  if (empty_non_axis_sizes || ((dummyCostSecond.size(0) != 0) &&
       (dummyCostSecond.size(1) != 0))) {
    sizes_idx_1 = dummyCostSecond.size(1);
  } else {
    sizes_idx_1 = 0;
  }

  b_y.set_size(reshapes[0].f1.size(0), (reshapes[0].f1.size(1) + sizes_idx_1));
  loop_ub = reshapes[0].f1.size(1);
  for (i = 0; i < loop_ub; i++) {
    sizes_idx_0 = reshapes[0].f1.size(0);
    for (i1 = 0; i1 < sizes_idx_0; i1++) {
      b_y[i1 + b_y.size(0) * i] = reshapes[0].f1[i1 + reshapes[0].f1.size(0) * i];
    }
  }

  for (i = 0; i < sizes_idx_1; i++) {
    for (i1 = 0; i1 < result; i1++) {
      b_y[i1 + b_y.size(0) * (i + reshapes[0].f1.size(1))] = dummyCostSecond[i1
        + result * i];
    }
  }

  if (loop_ub_tmp != 0) {
    result = loop_ub_tmp;
  } else if ((dummyDummyAssignment.size(0) != 0) && (dummyDummyAssignment.size(1)
              != 0)) {
    result = dummyDummyAssignment.size(0);
  } else {
    result = 0;
    if (dummyDummyAssignment.size(0) > 0) {
      result = dummyDummyAssignment.size(0);
    }
  }

  empty_non_axis_sizes = (result == 0);
  if (empty_non_axis_sizes || (loop_ub_tmp != 0)) {
    input_sizes_idx_1 = static_cast<signed char>(loop_ub_tmp);
  } else {
    input_sizes_idx_1 = 0;
  }

  if (empty_non_axis_sizes || ((dummyDummyAssignment.size(0) != 0) &&
       (dummyDummyAssignment.size(1) != 0))) {
    sizes_idx_1 = dummyDummyAssignment.size(1);
  } else {
    sizes_idx_1 = 0;
  }

  sizes_idx_0 = input_sizes_idx_1 + sizes_idx_1;
  dummyDummyAssignment.set_size(loop_ub_tmp, siz_idx_0_tmp);
  for (i = 0; i < b_loop_ub_tmp; i++) {
    dummyDummyAssignment[i] = 0;
  }

  y.set_size(result, sizes_idx_0);
  loop_ub = input_sizes_idx_1;
  for (i = 0; i < loop_ub; i++) {
    for (i1 = 0; i1 < result; i1++) {
      y[i1 + y.size(0) * i] = dummyCostFirst_data[i1 + result * i];
    }
  }

  for (i = 0; i < sizes_idx_1; i++) {
    for (i1 = 0; i1 < result; i1++) {
      y[i1 + y.size(0) * (i + input_sizes_idx_1)] = dummyDummyAssignment[i1 +
        result * i];
    }
  }

  if ((b_y.size(0) != 0) && (b_y.size(1) != 0)) {
    result = b_y.size(1);
  } else if ((y.size(0) != 0) && (y.size(1) != 0)) {
    result = sizes_idx_0;
  } else {
    result = b_y.size(1);
    if (result <= 0) {
      result = 0;
    }

    if (sizes_idx_0 > result) {
      result = sizes_idx_0;
    }
  }

  empty_non_axis_sizes = (result == 0);
  if (empty_non_axis_sizes || ((b_y.size(0) != 0) && (b_y.size(1) != 0))) {
    sizes_idx_1 = b_y.size(0);
  } else {
    sizes_idx_1 = 0;
  }

  if (empty_non_axis_sizes || ((y.size(0) != 0) && (y.size(1) != 0))) {
    input_sizes_idx_1 = static_cast<signed char>(y.size(0));
  } else {
    input_sizes_idx_1 = 0;
  }

  sizes_idx_0 = input_sizes_idx_1;
  paddedCost.set_size((sizes_idx_1 + input_sizes_idx_1), result);
  for (i = 0; i < result; i++) {
    for (i1 = 0; i1 < sizes_idx_1; i1++) {
      paddedCost[i1 + paddedCost.size(0) * i] = b_y[i1 + sizes_idx_1 * i];
    }
  }

  for (i = 0; i < result; i++) {
    for (i1 = 0; i1 < sizes_idx_0; i1++) {
      paddedCost[(i1 + sizes_idx_1) + paddedCost.size(0) * i] = y[i1 +
        input_sizes_idx_1 * i];
    }
  }
}

void assign2DRelaxed(coder::array<double, 2U> &costMatrix, coder::array<unsigned
                     int, 2U> &assign, double *cost, double price_data[], int
                     price_size[2])
{
  double temp;
  int nCol;
  int nRow;
  coder::array<double, 2U> r;
  coder::array<unsigned int, 2U> assignments;
  coder::array<unsigned int, 2U> unassignedRows;
  coder::array<unsigned int, 2U> unassignedCols;
  coder::array<double, 2U> priceWithDummy;
  int vlen;
  int nz;
  int unassignedCols_idx_0;
  coder::array<unsigned int, 2U> b_assignments;
  int loop_ub;
  int i;
  coder::array<int, 1U> r1;
  coder::array<unsigned int, 1U> rowSoln;
  coder::array<int, 1U> r2;
  coder::array<unsigned int, 1U> dummyRowSoln;
  coder::array<int, 1U> r3;
  cell_wrap_18 reshapes[2];
  coder::array<boolean_T, 1U> dummyAssignment;
  temp = costMatrix[0];
  costMatrix[0] = 0.0;
  nCol = costMatrix.size(1);
  nRow = costMatrix.size(0);
  padCostMatrix(costMatrix, r);
  assignAuctionWithPrice(r, assignments, unassignedRows, unassignedCols,
    priceWithDummy);
  vlen = unassignedRows.size(0) * unassignedRows.size(1);
  nz = unassignedCols.size(0) * unassignedCols.size(1);
  unassignedCols_idx_0 = unassignedCols.size(0) * unassignedCols.size(1);
  b_assignments.set_size(((assignments.size(0) + vlen) + nz), 2);
  loop_ub = assignments.size(0);
  for (i = 0; i < loop_ub; i++) {
    b_assignments[i] = assignments[i];
  }

  for (i = 0; i < loop_ub; i++) {
    b_assignments[i + b_assignments.size(0)] = assignments[i + assignments.size
      (0)];
  }

  for (i = 0; i < vlen; i++) {
    b_assignments[i + assignments.size(0)] = unassignedRows[i];
  }

  loop_ub = unassignedRows.size(0) * unassignedRows.size(1);
  for (i = 0; i < loop_ub; i++) {
    b_assignments[(i + assignments.size(0)) + b_assignments.size(0)] = 0U;
  }

  for (i = 0; i < nz; i++) {
    b_assignments[(i + assignments.size(0)) + vlen] = 0U;
  }

  for (i = 0; i < unassignedCols_idx_0; i++) {
    b_assignments[((i + assignments.size(0)) + vlen) + b_assignments.size(0)] =
      unassignedCols[i];
  }

  sortrows(b_assignments);
  loop_ub = b_assignments.size(0) * b_assignments.size(1);
  b_assignments.set_size(b_assignments.size(0), 2);
  for (i = 0; i < loop_ub; i++) {
    unsigned int qY;
    qY = b_assignments[i] + 1U;
    if (qY < b_assignments[i]) {
      qY = MAX_uint32_T;
    }

    b_assignments[i] = qY;
  }

  if (1 > costMatrix.size(1) - 1) {
    loop_ub = 1;
  } else {
    loop_ub = costMatrix.size(1);
  }

  price_size[0] = 1;
  price_size[1] = loop_ub;
  price_data[0] = 0.0;
  for (i = 0; i <= loop_ub - 2; i++) {
    price_data[i + 1] = priceWithDummy[i];
  }

  nz = b_assignments.size(0) - 1;
  unassignedCols_idx_0 = 0;
  for (loop_ub = 0; loop_ub <= nz; loop_ub++) {
    if (b_assignments[loop_ub] <= static_cast<unsigned int>(nRow)) {
      unassignedCols_idx_0++;
    }
  }

  r1.set_size(unassignedCols_idx_0);
  vlen = 0;
  for (loop_ub = 0; loop_ub <= nz; loop_ub++) {
    if (b_assignments[loop_ub] <= static_cast<unsigned int>(nRow)) {
      r1[vlen] = loop_ub + 1;
      vlen++;
    }
  }

  rowSoln.set_size(r1.size(0));
  loop_ub = r1.size(0);
  for (i = 0; i < loop_ub; i++) {
    rowSoln[i] = b_assignments[(r1[i] + b_assignments.size(0)) - 1];
  }

  nz = r1.size(0);
  for (loop_ub = 0; loop_ub < nz; loop_ub++) {
    if (rowSoln[loop_ub] > static_cast<unsigned int>(nCol)) {
      rowSoln[loop_ub] = 1U;
    }
  }

  nz = b_assignments.size(0) - 1;
  unassignedCols_idx_0 = 0;
  for (loop_ub = 0; loop_ub <= nz; loop_ub++) {
    if (b_assignments[loop_ub] > static_cast<unsigned int>(nRow)) {
      unassignedCols_idx_0++;
    }
  }

  r2.set_size(unassignedCols_idx_0);
  vlen = 0;
  for (loop_ub = 0; loop_ub <= nz; loop_ub++) {
    if (b_assignments[loop_ub] > static_cast<unsigned int>(nRow)) {
      r2[vlen] = loop_ub + 1;
      vlen++;
    }
  }

  dummyRowSoln.set_size(r2.size(0));
  loop_ub = r2.size(0);
  for (i = 0; i < loop_ub; i++) {
    dummyRowSoln[i] = b_assignments[(r2[i] + b_assignments.size(0)) - 1];
  }

  nz = r2.size(0) - 1;
  unassignedCols_idx_0 = 0;
  vlen = 0;
  for (loop_ub = 0; loop_ub <= nz; loop_ub++) {
    if (b_assignments[(r2[loop_ub] + b_assignments.size(0)) - 1] <= static_cast<
        unsigned int>(nCol)) {
      unassignedCols_idx_0++;
    }

    if (dummyRowSoln[loop_ub] <= static_cast<unsigned int>(nCol)) {
      dummyRowSoln[vlen] = dummyRowSoln[loop_ub];
      vlen++;
    }
  }

  dummyRowSoln.set_size(unassignedCols_idx_0);
  nz = b_assignments.size(0) - 1;
  vlen = 0;
  for (loop_ub = 0; loop_ub <= nz; loop_ub++) {
    if (b_assignments[loop_ub] <= static_cast<unsigned int>(nRow)) {
      vlen++;
    }
  }

  r3.set_size(vlen);
  vlen = 0;
  for (loop_ub = 0; loop_ub <= nz; loop_ub++) {
    if (b_assignments[loop_ub] <= static_cast<unsigned int>(nRow)) {
      r3[vlen] = loop_ub + 1;
      vlen++;
    }
  }

  reshapes[0].f1.set_size(r3.size(0));
  loop_ub = r3.size(0);
  for (i = 0; i < loop_ub; i++) {
    reshapes[0].f1[i] = b_assignments[r3[i] - 1];
  }

  assign.set_size((reshapes[0].f1.size(0) + unassignedCols_idx_0), 2);
  loop_ub = reshapes[0].f1.size(0);
  for (i = 0; i < loop_ub; i++) {
    assign[i] = reshapes[0].f1[i];
  }

  loop_ub = rowSoln.size(0);
  for (i = 0; i < loop_ub; i++) {
    assign[i + assign.size(0)] = rowSoln[i];
  }

  for (i = 0; i < unassignedCols_idx_0; i++) {
    assign[i + reshapes[0].f1.size(0)] = 1U;
  }

  for (i = 0; i < unassignedCols_idx_0; i++) {
    assign[(i + reshapes[0].f1.size(0)) + assign.size(0)] = dummyRowSoln[i];
  }

  sortrows(assign);
  loop_ub = assign.size(0);
  dummyAssignment.set_size(assign.size(0));
  for (i = 0; i < loop_ub; i++) {
    dummyAssignment[i] = ((assign[i] == 1U) && (assign[i + assign.size(0)] == 1U));
  }

  vlen = dummyAssignment.size(0);
  if (dummyAssignment.size(0) == 0) {
    nz = 0;
  } else {
    nz = dummyAssignment[0];
    for (unassignedCols_idx_0 = 2; unassignedCols_idx_0 <= vlen;
         unassignedCols_idx_0++) {
      nz += dummyAssignment[unassignedCols_idx_0 - 1];
    }
  }

  if (nz >= 1) {
    if (static_cast<double>(nz) + 1.0 > assign.size(0)) {
      nz = 0;
      i = 0;
    } else {
      i = assign.size(0);
    }

    loop_ub = i - nz;
    b_assignments.set_size(loop_ub, 2);
    for (i = 0; i < loop_ub; i++) {
      b_assignments[i] = assign[nz + i];
    }

    for (i = 0; i < loop_ub; i++) {
      b_assignments[i + b_assignments.size(0)] = assign[(nz + i) + assign.size(0)];
    }

    assign.set_size(b_assignments.size(0), 2);
    loop_ub = b_assignments.size(0) * b_assignments.size(1);
    for (i = 0; i < loop_ub; i++) {
      assign[i] = b_assignments[i];
    }
  }

  b_assignments.set_size((assign.size(0) + 1), 2);
  loop_ub = assign.size(0);
  b_assignments[0] = 1U;
  for (i = 0; i < loop_ub; i++) {
    b_assignments[i + 1] = assign[i];
  }

  b_assignments[b_assignments.size(0)] = 1U;
  for (i = 0; i < loop_ub; i++) {
    b_assignments[(i + b_assignments.size(0)) + 1] = assign[i + assign.size(0)];
  }

  assign.set_size(b_assignments.size(0), 2);
  loop_ub = b_assignments.size(0) * b_assignments.size(1);
  for (i = 0; i < loop_ub; i++) {
    assign[i] = b_assignments[i];
  }

  costMatrix[0] = temp;
  i = assign.size(0);
  *cost = costMatrix[(static_cast<int>(assign[0]) + costMatrix.size(0) * (
    static_cast<int>(assign[assign.size(0)]) - 1)) - 1];
  for (unassignedCols_idx_0 = 2; unassignedCols_idx_0 <= i; unassignedCols_idx_0
       ++) {
    *cost += costMatrix[(static_cast<int>(assign[unassignedCols_idx_0 - 1]) +
                         costMatrix.size(0) * (static_cast<int>(assign
      [(unassignedCols_idx_0 + assign.size(0)) - 1]) - 1)) - 1];
  }
}

// End of code generation (assign2DRelaxed.cpp)
