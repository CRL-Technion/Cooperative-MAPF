//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  kbestEnforceConstraints.cpp
//
//  Code generation for function 'kbestEnforceConstraints'
//


// Include files
#include "kbestEnforceConstraints.h"
#include "ITaskAssignment.h"
#include "assign2DRelaxed.h"
#include "assignAuctionWithPrice.h"
#include "assignkbestsd.h"
#include "get_best_assignments_rtwutil.h"
#include "rt_nonfinite.h"
#include <cstring>

// Type Definitions
struct emxArray_uint32_T_1x51
{
  unsigned int data[51];
  int size[2];
};

struct cell_wrap_22
{
  emxArray_uint32_T_1x51 f1;
};

// Function Definitions
void kbestEnforceConstraints(coder::array<double, 3U> &costMatrix, const
  unsigned int tuplesToRemove_data[], const coder::array<unsigned int, 2U>
  &tuplesToEnforce)
{
  signed char dimSizes_idx_0;
  signed char dimSizes_idx_1;
  signed char dimSizes_idx_2;
  coder::array<double, 2U> y;
  int loop_ub;
  int i;
  cell_wrap_22 allArgs[3];
  double tempCost;
  unsigned int u;
  cell_wrap_22 indices[3];
  signed char tmp_data[51];
  signed char b_tmp_data[51];
  signed char c_tmp_data[51];
  costMatrix[((static_cast<int>(tuplesToRemove_data[0]) + costMatrix.size(0) * (
    static_cast<int>(tuplesToRemove_data[1]) - 1)) + costMatrix.size(0) *
              costMatrix.size(1) * (static_cast<int>(tuplesToRemove_data[2]) - 1))
    - 1] = rtInf;
  dimSizes_idx_0 = static_cast<signed char>(costMatrix.size(0));
  dimSizes_idx_1 = static_cast<signed char>(costMatrix.size(1));
  dimSizes_idx_2 = static_cast<signed char>(costMatrix.size(2));
  if (dimSizes_idx_0 < 1) {
    y.set_size(1, 0);
  } else {
    y.set_size(1, (static_cast<int>(dimSizes_idx_0)));
    loop_ub = dimSizes_idx_0 - 1;
    for (i = 0; i <= loop_ub; i++) {
      y[i] = static_cast<double>(i) + 1.0;
    }
  }

  allArgs[0].f1.size[0] = 1;
  allArgs[0].f1.size[1] = y.size(1);
  loop_ub = y.size(0) * y.size(1);
  for (i = 0; i < loop_ub; i++) {
    tempCost = rt_roundd_snf(y[i]);
    if (tempCost < 4.294967296E+9) {
      if (tempCost >= 0.0) {
        u = static_cast<unsigned int>(tempCost);
      } else {
        u = 0U;
      }
    } else if (tempCost >= 4.294967296E+9) {
      u = MAX_uint32_T;
    } else {
      u = 0U;
    }

    allArgs[0].f1.data[i] = u;
  }

  if (dimSizes_idx_1 < 1) {
    y.set_size(1, 0);
  } else {
    loop_ub = dimSizes_idx_1 - 1;
    y.set_size(1, (loop_ub + 1));
    for (i = 0; i <= loop_ub; i++) {
      y[i] = static_cast<double>(i) + 1.0;
    }
  }

  allArgs[1].f1.size[0] = 1;
  allArgs[1].f1.size[1] = y.size(1);
  loop_ub = y.size(0) * y.size(1);
  for (i = 0; i < loop_ub; i++) {
    tempCost = rt_roundd_snf(y[i]);
    if (tempCost < 4.294967296E+9) {
      if (tempCost >= 0.0) {
        u = static_cast<unsigned int>(tempCost);
      } else {
        u = 0U;
      }
    } else if (tempCost >= 4.294967296E+9) {
      u = MAX_uint32_T;
    } else {
      u = 0U;
    }

    allArgs[1].f1.data[i] = u;
  }

  if (dimSizes_idx_2 < 1) {
    y.set_size(1, 0);
  } else {
    loop_ub = dimSizes_idx_2 - 1;
    y.set_size(1, (loop_ub + 1));
    for (i = 0; i <= loop_ub; i++) {
      y[i] = static_cast<double>(i) + 1.0;
    }
  }

  allArgs[2].f1.size[0] = 1;
  allArgs[2].f1.size[1] = y.size(1);
  loop_ub = y.size(0) * y.size(1);
  for (i = 0; i < loop_ub; i++) {
    tempCost = rt_roundd_snf(y[i]);
    if (tempCost < 4.294967296E+9) {
      if (tempCost >= 0.0) {
        u = static_cast<unsigned int>(tempCost);
      } else {
        u = 0U;
      }
    } else if (tempCost >= 4.294967296E+9) {
      u = MAX_uint32_T;
    } else {
      u = 0U;
    }

    allArgs[2].f1.data[i] = u;
  }

  i = tuplesToEnforce.size(0);
  for (int b_i = 0; b_i < i; b_i++) {
    int tempCost_tmp;
    int b_tempCost_tmp;
    int c_tempCost_tmp;
    int i1;
    tempCost_tmp = static_cast<int>(tuplesToEnforce[b_i]) - 1;
    b_tempCost_tmp = static_cast<int>(tuplesToEnforce[b_i + tuplesToEnforce.size
      (0)]) - 1;
    c_tempCost_tmp = static_cast<int>(tuplesToEnforce[b_i + tuplesToEnforce.size
      (0) * 2]) - 1;
    tempCost = costMatrix[(tempCost_tmp + costMatrix.size(0) * b_tempCost_tmp) +
      costMatrix.size(0) * costMatrix.size(1) * c_tempCost_tmp];
    i1 = tuplesToEnforce.size(1);
    for (int j = 0; j < i1; j++) {
      std::memcpy(&indices[0], &allArgs[0], 3U * sizeof(cell_wrap_22));
      u = tuplesToEnforce[b_i + tuplesToEnforce.size(0) * j];
      if (u != 1U) {
        int tmp_size_idx_0;
        int i2;
        int b_tmp_size_idx_0;
        int c_tmp_size_idx_0;
        indices[j].f1.size[0] = 1;
        indices[j].f1.size[1] = 1;
        indices[j].f1.data[0] = u;
        tmp_size_idx_0 = indices[0].f1.size[1];
        loop_ub = indices[0].f1.size[1];
        for (i2 = 0; i2 < loop_ub; i2++) {
          tmp_data[i2] = static_cast<signed char>(indices[0].f1.data[i2]);
        }

        b_tmp_size_idx_0 = indices[1].f1.size[1];
        loop_ub = indices[1].f1.size[1];
        for (i2 = 0; i2 < loop_ub; i2++) {
          b_tmp_data[i2] = static_cast<signed char>(indices[1].f1.data[i2]);
        }

        c_tmp_size_idx_0 = indices[2].f1.size[1];
        loop_ub = indices[2].f1.size[1];
        for (i2 = 0; i2 < loop_ub; i2++) {
          c_tmp_data[i2] = static_cast<signed char>(indices[2].f1.data[i2]);
        }

        for (i2 = 0; i2 < c_tmp_size_idx_0; i2++) {
          for (loop_ub = 0; loop_ub < b_tmp_size_idx_0; loop_ub++) {
            for (int i3 = 0; i3 < tmp_size_idx_0; i3++) {
              costMatrix[((tmp_data[i3] + costMatrix.size(0) *
                           (b_tmp_data[loop_ub] - 1)) + costMatrix.size(0) *
                          costMatrix.size(1) * (c_tmp_data[i2] - 1)) - 1] =
                rtInf;
            }
          }
        }
      }
    }

    costMatrix[(tempCost_tmp + costMatrix.size(0) * b_tempCost_tmp) +
      costMatrix.size(0) * costMatrix.size(1) * c_tempCost_tmp] = tempCost;
  }
}

// End of code generation (kbestEnforceConstraints.cpp)
