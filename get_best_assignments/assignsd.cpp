//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  assignsd.cpp
//
//  Code generation for function 'assignsd'
//


// Include files
#include "assignsd.h"
#include "ITaskAssignment.h"
#include "assign2DRelaxed.h"
#include "assignAuctionWithPrice.h"
#include "assignkbestsd.h"
#include "bsxfun.h"
#include "minOrMax.h"
#include "rt_nonfinite.h"
#include "sdRemoveImpossibles.h"
#include "sort.h"
#include "sortIdx.h"
#include <cmath>
#include <cstring>

// Type Definitions
struct emxArray_real_T_1x1x51
{
  double data[51];
  int size[3];
};

struct cell_wrap_10
{
  emxArray_real_T_1x1x51 f1;
};

struct emxArray_real_T_51
{
  double data[51];
  int size[1];
};

struct cell_wrap_12
{
  emxArray_real_T_51 f1;
};

struct emxArray_cell_wrap_18_2
{
  cell_wrap_18 data[2];
  int size[1];
};

// Function Declarations
static double b_computeCostGivenAssignment(const coder::array<double, 3U>
  &costMatrix, const coder::array<unsigned int, 2U> &assignments);
static void c_updateLagrangianMultiplierHeu(double l_data[], int l_size[3],
  const double g_data[], const int g_size[1], double dualCost, double
  feasibleCost, const double price_data[], const int price_size[2]);
static double computeCostGivenAssignment(const double costMatrix_data[], const
  int costMatrix_size[2], const coder::array<unsigned int, 2U> &assignments);
static void computeMultiplierGradient(const double sortedIndices_data[], const
  int sortedIndices_size[2], double dimSize, const coder::array<unsigned int, 2U>
  &assignments, double g_data[], int g_size[1]);
static void constructFeasibleCostMatrix(const coder::array<double, 3U>
  &costMatrix, const coder::array<unsigned int, 2U> &assignments, coder::array<
  double, 2U> &feasibleCostMatrix);

// Function Definitions
static double b_computeCostGivenAssignment(const coder::array<double, 3U>
  &costMatrix, const coder::array<unsigned int, 2U> &assignments)
{
  double cost;
  signed char siz_idx_0;
  int a;
  int i;
  siz_idx_0 = static_cast<signed char>(costMatrix.size(0));
  a = siz_idx_0 * static_cast<signed char>(costMatrix.size(1));
  i = assignments.size(0);
  if (assignments.size(0) == 0) {
    cost = 0.0;
  } else {
    cost = costMatrix[((static_cast<int>(assignments[0]) + siz_idx_0 * (
      static_cast<int>(assignments[assignments.size(0)]) - 1)) + a * (
      static_cast<int>(assignments[assignments.size(0) * 2]) - 1)) - 1];
    for (int k = 2; k <= i; k++) {
      cost += costMatrix[((static_cast<int>(assignments[k - 1]) + siz_idx_0 * (
        static_cast<int>(assignments[(k + assignments.size(0)) - 1]) - 1)) + a *
                          (static_cast<int>(assignments[(k + assignments.size(0)
        * 2) - 1]) - 1)) - 1];
    }
  }

  return cost;
}

static void c_updateLagrangianMultiplierHeu(double l_data[], int l_size[3],
  const double g_data[], const int g_size[1], double dualCost, double
  feasibleCost, const double price_data[], const int price_size[2])
{
  double thisGap;
  double dv[1];
  int kend;
  double scale;
  int k;
  int i;
  coder::array<boolean_T, 1U> b;
  double adaptFactor_data[51];
  coder::array<boolean_T, 1U> r;
  boolean_T y;
  boolean_T exitg1;
  thisGap = feasibleCost - dualCost;
  dv[0] = thisGap;
  kend = 0;
  if (rtIsInf(thisGap) || rtIsNaN(thisGap)) {
    kend = 1;
  }

  thisGap = std::abs(dualCost);
  if (0 <= kend - 1) {
    dv[0] = 0.1 * thisGap;
  }

  thisGap = 0.0;
  if (g_size[0] == 1) {
    thisGap = std::abs(g_data[0]);
  } else {
    scale = 3.3121686421112381E-170;
    kend = g_size[0];
    for (k = 0; k < kend; k++) {
      double absxk;
      absxk = std::abs(g_data[k]);
      if (absxk > scale) {
        double t;
        t = scale / absxk;
        thisGap = thisGap * t * t + 1.0;
        scale = absxk;
      } else {
        double t;
        t = absxk / scale;
        thisGap += t * t;
      }
    }

    thisGap = scale * std::sqrt(thisGap);
  }

  thisGap = dv[0] / (thisGap * thisGap);
  kend = price_size[1];
  if (price_size[1] == 0) {
    scale = 0.0;
  } else {
    scale = price_data[0];
    for (k = 2; k <= kend; k++) {
      scale += price_data[k - 1];
    }
  }

  kend = price_size[1];
  k = price_size[1];
  for (i = 0; i < k; i++) {
    adaptFactor_data[i] = thisGap * ((static_cast<double>(price_size[1]) - 1.0) *
      price_data[i] * g_data[i]) / scale;
  }

  b.set_size(price_size[1]);
  for (i = 0; i < kend; i++) {
    b[i] = rtIsInf(adaptFactor_data[i]);
  }

  r.set_size(price_size[1]);
  for (i = 0; i < kend; i++) {
    r[i] = rtIsNaN(adaptFactor_data[i]);
  }

  k = b.size(0);
  for (i = 0; i < k; i++) {
    b[i] = ((!b[i]) && (!r[i]));
  }

  r.set_size(price_size[1]);
  for (i = 0; i < kend; i++) {
    r[i] = rtIsInf(adaptFactor_data[i]);
  }

  y = true;
  k = 0;
  exitg1 = false;
  while ((!exitg1) && (k <= static_cast<signed char>(r.size(0)) - 1)) {
    if (!b[k]) {
      y = false;
      exitg1 = true;
    } else {
      k++;
    }
  }

  if (y) {
    kend = l_size[2];
    l_size[0] = 1;
    l_size[1] = 1;
    l_size[2] = kend;
    for (i = 0; i < kend; i++) {
      l_data[i] += adaptFactor_data[i];
    }
  }
}

static double computeCostGivenAssignment(const double costMatrix_data[], const
  int costMatrix_size[2], const coder::array<unsigned int, 2U> &assignments)
{
  double cost;
  signed char siz_idx_0;
  int i;
  int siz;
  siz_idx_0 = static_cast<signed char>(costMatrix_size[0]);
  i = assignments.size(0);
  if (assignments.size(0) == 0) {
    cost = 0.0;
  } else {
    cost = costMatrix_data[(static_cast<int>(assignments[0]) + siz_idx_0 * (
      static_cast<int>(assignments[assignments.size(0)]) - 1)) - 1];
    if (2 <= assignments.size(0)) {
      siz = siz_idx_0;
    }

    for (int k = 2; k <= i; k++) {
      cost += costMatrix_data[(static_cast<int>(assignments[k - 1]) + siz * (
        static_cast<int>(assignments[(k + assignments.size(0)) - 1]) - 1)) - 1];
    }
  }

  return cost;
}

static void computeMultiplierGradient(const double sortedIndices_data[], const
  int sortedIndices_size[2], double dimSize, const coder::array<unsigned int, 2U>
  &assignments, double g_data[], int g_size[1])
{
  int loop_ub;
  emxArray_cell_wrap_18_2 args;
  int ii;
  int idx;
  int nx;
  signed char siz_idx_0;
  coder::array<double, 1U> indicesUsed;
  coder::array<boolean_T, 1U> x;
  coder::array<int, 1U> b_ii;
  loop_ub = static_cast<int>(dimSize);
  g_size[0] = loop_ub;
  if (0 <= loop_ub - 1) {
    std::memset(&g_data[0], 0, loop_ub * sizeof(double));
  }

  args.size[0] = 2;
  if (2 > assignments.size(0)) {
    ii = 0;
    idx = 0;
  } else {
    ii = 1;
    idx = assignments.size(0);
  }

  nx = idx - ii;
  args.data[0].f1.set_size(nx);
  for (idx = 0; idx < nx; idx++) {
    args.data[0].f1[idx] = assignments[ii + idx];
  }

  siz_idx_0 = static_cast<signed char>(sortedIndices_size[0]);
  args.data[1].f1.set_size(nx);
  for (idx = 0; idx < nx; idx++) {
    args.data[1].f1[idx] = assignments[(ii + idx) + assignments.size(0)];
  }

  indicesUsed.set_size(args.data[0].f1.size(0));
  nx = args.data[0].f1.size(0);
  for (ii = 0; ii < nx; ii++) {
    indicesUsed[ii] = sortedIndices_data[(static_cast<int>(args.data[0].f1[ii])
      + siz_idx_0 * (static_cast<int>(args.data[1].f1[ii]) - 1)) - 1];
  }

  for (int i = 0; i <= loop_ub - 2; i++) {
    boolean_T exitg1;
    nx = indicesUsed.size(0);
    x.set_size(indicesUsed.size(0));
    for (ii = 0; ii < nx; ii++) {
      x[ii] = (indicesUsed[ii] == static_cast<double>(i) + 2.0);
    }

    nx = x.size(0);
    idx = 0;
    b_ii.set_size(x.size(0));
    ii = 0;
    exitg1 = false;
    while ((!exitg1) && (ii <= nx - 1)) {
      if (x[ii]) {
        idx++;
        b_ii[idx - 1] = ii + 1;
        if (idx >= nx) {
          exitg1 = true;
        } else {
          ii++;
        }
      } else {
        ii++;
      }
    }

    if (x.size(0) == 1) {
      if (idx == 0) {
        b_ii.set_size(0);
      }
    } else {
      if (1 > idx) {
        idx = 0;
      }

      b_ii.set_size(idx);
    }

    g_data[i + 1] = 1.0 - static_cast<double>(b_ii.size(0));
  }
}

static void constructFeasibleCostMatrix(const coder::array<double, 3U>
  &costMatrix, const coder::array<unsigned int, 2U> &assignments, coder::array<
  double, 2U> &feasibleCostMatrix)
{
  signed char s_idx_2;
  int loop_ub;
  int i;
  s_idx_2 = static_cast<signed char>(costMatrix.size(2));
  feasibleCostMatrix.set_size(assignments.size(0), (static_cast<int>(s_idx_2)));
  loop_ub = assignments.size(0) * s_idx_2;
  for (i = 0; i < loop_ub; i++) {
    feasibleCostMatrix[i] = 0.0;
  }

  i = assignments.size(0);
  for (int b_i = 0; b_i < i; b_i++) {
    int b_assignments;
    int c_assignments;
    b_assignments = static_cast<int>(assignments[b_i]);
    c_assignments = static_cast<int>(assignments[b_i + assignments.size(0)]);
    loop_ub = feasibleCostMatrix.size(1);
    for (int i1 = 0; i1 < loop_ub; i1++) {
      feasibleCostMatrix[b_i + feasibleCostMatrix.size(0) * i1] = costMatrix
        [((b_assignments + costMatrix.size(0) * (c_assignments - 1)) +
          costMatrix.size(0) * costMatrix.size(1) * i1) - 1];
    }
  }
}

void assignsd(coder::array<double, 3U> &costMatrix, coder::array<unsigned int,
              2U> &assignments, double *costOfAssignment, double *solutionGap)
{
  coder::array<double, 3U> b_costMatrix;
  int idx;
  int i;
  signed char dimSize_idx_2;
  cell_wrap_10 lagrangianMultipliers[1];
  double bestDualCost;
  cell_wrap_12 dummyAssignments[1];
  double bestFeasibleCost;
  unsigned int currentIter;
  double currentGap;
  double bestGap;
  boolean_T exitg1;
  coder::array<double, 3U> matrix;
  double ex_data[2601];
  int ex_size[2];
  int idx_data[2601];
  int idx_size[2];
  cell_wrap_11 constrainedCost[1];
  int ii;
  coder::array<unsigned int, 2U> b_assignments;
  coder::array<boolean_T, 1U> x;
  int nx;
  coder::array<signed char, 1U> b_ii;
  signed char negIndices_data[51];
  coder::array<double, 2U> b_constrainedCost;
  cell_wrap_10 r;
  coder::array<unsigned int, 2U> c_assignments;
  double price_data[51];
  int price_size[2];
  cell_wrap_12 gradients[1];
  boolean_T tmp_data[51];
  signed char validDummies_data[51];
  double gradients_data[51];
  coder::array<unsigned int, 2U> d_assignments;
  cell_wrap_0 reshapes[2];
  coder::array<unsigned int, 1U> e_assignments;
  b_costMatrix.set_size(costMatrix.size(0), costMatrix.size(1), costMatrix.size
                        (2));
  idx = costMatrix.size(0) * costMatrix.size(1) * costMatrix.size(2);
  for (i = 0; i < idx; i++) {
    b_costMatrix[i] = costMatrix[i];
  }

  costMatrix[0] = 0.0;
  dimSize_idx_2 = static_cast<signed char>(b_costMatrix.size(2));
  sdRemoveImpossibles(costMatrix);
  lagrangianMultipliers[0].f1.size[0] = 1;
  lagrangianMultipliers[0].f1.size[1] = 1;
  lagrangianMultipliers[0].f1.size[2] = dimSize_idx_2;
  idx = dimSize_idx_2;
  for (i = 0; i < idx; i++) {
    lagrangianMultipliers[0].f1.data[i] = 0.0;
  }

  idx = dimSize_idx_2;
  for (i = 0; i < idx; i++) {
    dummyAssignments[0].f1.data[i] = 0.0;
  }

  bestDualCost = rtMinusInf;
  bestFeasibleCost = rtInf;
  assignments.set_size(0, 3);
  currentIter = 1U;
  currentGap = rtInf;
  bestGap = rtInf;
  exitg1 = false;
  while ((!exitg1) && ((currentGap > 0.01) && (currentIter - 1U < 100U))) {
    boolean_T exitg2;
    double dualCost;
    double feasibleCost;
    boolean_T y;
    unsigned int qY;
    c_bsxfun(costMatrix, lagrangianMultipliers[0].f1.data,
             lagrangianMultipliers[0].f1.size, matrix);
    minimum(matrix, ex_data, ex_size, idx_data, idx_size);
    constrainedCost[0].f1.set_size(ex_size[0], ex_size[1]);
    idx = ex_size[0] * ex_size[1];
    for (i = 0; i < idx; i++) {
      constrainedCost[0].f1[i] = ex_data[i];
    }

    x.set_size((static_cast<int>(dimSize_idx_2)));
    idx = dimSize_idx_2;
    for (i = 0; i < idx; i++) {
      x[i] = (matrix[matrix.size(0) * matrix.size(1) * i] < 0.0);
    }

    nx = x.size(0);
    idx = 0;
    b_ii.set_size(x.size(0));
    ii = 0;
    exitg2 = false;
    while ((!exitg2) && (ii <= nx - 1)) {
      if (x[ii]) {
        idx++;
        b_ii[idx - 1] = static_cast<signed char>(ii + 1);
        if (idx >= nx) {
          exitg2 = true;
        } else {
          ii++;
        }
      } else {
        ii++;
      }
    }

    if (x.size(0) == 1) {
      if (idx == 0) {
        b_ii.set_size(0);
      }
    } else {
      if (1 > idx) {
        idx = 0;
      }

      b_ii.set_size(idx);
    }

    nx = b_ii.size(0);
    idx = b_ii.size(0);
    for (i = 0; i < idx; i++) {
      negIndices_data[i] = b_ii[i];
    }

    idx = dimSize_idx_2;
    for (i = 0; i < idx; i++) {
      dummyAssignments[0].f1.data[i] = 0.0;
    }

    if (1 > b_ii.size(0)) {
      idx = 0;
    } else {
      idx = nx;
    }

    for (i = 0; i < idx; i++) {
      dummyAssignments[0].f1.data[i] = b_ii[i];
    }

    if (nx == 0) {
      currentGap = 0.0;
    } else {
      currentGap = matrix[matrix.size(0) * matrix.size(1) * (negIndices_data[0]
        - 1)];
      for (idx = 2; idx <= nx; idx++) {
        currentGap += matrix[matrix.size(0) * matrix.size(1) *
          (negIndices_data[idx - 1] - 1)];
      }
    }

    constrainedCost[0].f1[0] = currentGap;
    idx = lagrangianMultipliers[0].f1.size[0] * lagrangianMultipliers[0]
      .f1.size[1] * lagrangianMultipliers[0].f1.size[2];
    for (i = 0; i < idx; i++) {
      r.f1.data[i] = lagrangianMultipliers[0].f1.data[i];
    }

    b_constrainedCost.set_size(constrainedCost[0].f1.size(0), constrainedCost[0]
      .f1.size(1));
    idx = constrainedCost[0].f1.size(0) * constrainedCost[0].f1.size(1) - 1;
    for (i = 0; i <= idx; i++) {
      b_constrainedCost[i] = constrainedCost[0].f1[i];
    }

    assign2DRelaxed(b_constrainedCost, c_assignments, &currentGap, price_data,
                    price_size);
    nx = lagrangianMultipliers[0].f1.size[2];
    if (lagrangianMultipliers[0].f1.size[2] == 0) {
      currentGap = 0.0;
    } else {
      currentGap = r.f1.data[0];
      for (idx = 2; idx <= nx; idx++) {
        currentGap += r.f1.data[idx - 1];
      }
    }

    dualCost = computeCostGivenAssignment((double *)constrainedCost[0].f1.data(),
      constrainedCost[0].f1.size(), c_assignments) + currentGap;
    if ((dualCost > bestDualCost) || rtIsNaN(bestDualCost)) {
      bestDualCost = dualCost;
    }

    ex_size[0] = idx_size[0];
    ex_size[1] = idx_size[1];
    idx = idx_size[0] * idx_size[1];
    for (i = 0; i < idx; i++) {
      ex_data[i] = idx_data[i];
    }

    computeMultiplierGradient(ex_data, ex_size, static_cast<double>
      (dimSize_idx_2), c_assignments, gradients[0].f1.data, gradients[0].f1.size);
    idx = dimSize_idx_2;
    for (i = 0; i < idx; i++) {
      tmp_data[i] = (dummyAssignments[0].f1.data[i] > 1.0);
    }

    nx = dimSize_idx_2 - 1;
    ii = 0;
    for (i = 0; i <= nx; i++) {
      if (tmp_data[i]) {
        ii++;
      }
    }

    idx = 0;
    for (i = 0; i <= nx; i++) {
      if (tmp_data[i]) {
        validDummies_data[idx] = static_cast<signed char>(dummyAssignments[0].
          f1.data[i]);
        idx++;
      }
    }

    for (i = 0; i < ii; i++) {
      gradients_data[i] = gradients[0].f1.data[validDummies_data[i] - 1] - 1.0;
    }

    for (i = 0; i < ii; i++) {
      gradients[0].f1.data[validDummies_data[i] - 1] = gradients_data[i];
    }

    constructFeasibleCostMatrix(costMatrix, c_assignments, b_constrainedCost);
    assign2DRelaxed(b_constrainedCost, d_assignments, &currentGap, price_data,
                    price_size);
    idx = d_assignments.size(0);
    b_assignments.set_size(d_assignments.size(0), 2);
    for (i = 0; i < idx; i++) {
      b_assignments[i] = c_assignments[static_cast<int>(d_assignments[i]) - 1];
    }

    for (i = 0; i < idx; i++) {
      b_assignments[i + b_assignments.size(0)] = c_assignments[(static_cast<int>
        (d_assignments[i]) + c_assignments.size(0)) - 1];
    }

    reshapes[0].f1.set_size(d_assignments.size(0), 2);
    idx = d_assignments.size(0) << 1;
    for (i = 0; i < idx; i++) {
      reshapes[0].f1[i] = b_assignments[i];
    }

    idx = d_assignments.size(0);
    e_assignments.set_size(d_assignments.size(0));
    for (i = 0; i < idx; i++) {
      e_assignments[i] = d_assignments[i + d_assignments.size(0)];
    }

    reshapes[1].f1.set_size(d_assignments.size(0), 1);
    idx = d_assignments.size(0);
    for (i = 0; i < idx; i++) {
      reshapes[1].f1[i] = e_assignments[i];
    }

    b_assignments.set_size(reshapes[0].f1.size(0), (reshapes[0].f1.size(1) +
      reshapes[1].f1.size(1)));
    idx = reshapes[0].f1.size(1);
    for (i = 0; i < idx; i++) {
      nx = reshapes[0].f1.size(0);
      for (ii = 0; ii < nx; ii++) {
        b_assignments[ii + b_assignments.size(0) * i] = reshapes[0].f1[ii +
          reshapes[0].f1.size(0) * i];
      }
    }

    idx = reshapes[1].f1.size(1);
    for (i = 0; i < idx; i++) {
      nx = reshapes[1].f1.size(0);
      for (ii = 0; ii < nx; ii++) {
        b_assignments[ii + b_assignments.size(0) * (i + reshapes[0].f1.size(1))]
          = reshapes[1].f1[ii + reshapes[1].f1.size(0) * i];
      }
    }

    feasibleCost = b_computeCostGivenAssignment(costMatrix, b_assignments);
    if ((!(bestFeasibleCost < feasibleCost)) && (!rtIsNaN(feasibleCost))) {
      bestFeasibleCost = feasibleCost;
    }

    y = false;
    nx = 0;
    exitg2 = false;
    while ((!exitg2) && (nx + 1 <= gradients[0].f1.size[0])) {
      if ((gradients[0].f1.data[nx] == 0.0) || rtIsNaN(gradients[0].f1.data[nx]))
      {
        nx++;
      } else {
        y = true;
        exitg2 = true;
      }
    }

    c_updateLagrangianMultiplierHeu(lagrangianMultipliers[0].f1.data,
      lagrangianMultipliers[0].f1.size, gradients[0].f1.data, gradients[0].
      f1.size, dualCost, feasibleCost, price_data, price_size);
    qY = currentIter + 1U;
    if (qY < currentIter) {
      qY = MAX_uint32_T;
    }

    currentIter = qY;
    currentGap = bestFeasibleCost;
    if (bestFeasibleCost < 0.0) {
      currentGap = -1.0;
    } else if (bestFeasibleCost > 0.0) {
      currentGap = 1.0;
    } else {
      if (bestFeasibleCost == 0.0) {
        currentGap = 0.0;
      }
    }

    currentGap -= bestDualCost / std::abs(bestFeasibleCost +
      2.2250738585072014E-308);
    if ((0.0 > currentGap) || rtIsNaN(currentGap)) {
      currentGap = 0.0;
    }

    if ((currentGap < bestGap) || rtIsNaN(bestGap)) {
      bestGap = currentGap;
    }

    if ((bestFeasibleCost == feasibleCost) || (qY == 2U)) {
      assignments.set_size(b_assignments.size(0), b_assignments.size(1));
      idx = b_assignments.size(0) * b_assignments.size(1);
      for (i = 0; i < idx; i++) {
        assignments[i] = b_assignments[i];
      }

      bestGap = currentGap;
    }

    if (!y) {
      bestGap = 0.0;
      exitg1 = true;
    }
  }

  if (b_costMatrix[0] >= 0.0) {
    if (2 > assignments.size(0)) {
      i = 0;
      ii = 0;
    } else {
      i = 1;
      ii = assignments.size(0);
    }

    idx = ii - i;
    b_assignments.set_size(idx, 3);
    for (ii = 0; ii < 3; ii++) {
      for (nx = 0; nx < idx; nx++) {
        b_assignments[nx + b_assignments.size(0) * ii] = assignments[(i + nx) +
          assignments.size(0) * ii];
      }
    }

    assignments.set_size(b_assignments.size(0), b_assignments.size(1));
    idx = b_assignments.size(0) * b_assignments.size(1);
    for (i = 0; i < idx; i++) {
      assignments[i] = b_assignments[i];
    }
  }

  costMatrix[0] = b_costMatrix[0];
  *costOfAssignment = b_computeCostGivenAssignment(costMatrix, assignments);
  *solutionGap = bestGap;
}

// End of code generation (assignsd.cpp)
