//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  kbestSDPartitionSolution.cpp
//
//  Code generation for function 'kbestSDPartitionSolution'
//


// Include files
#include "kbestSDPartitionSolution.h"
#include "ITaskAssignment.h"
#include "assignkbestsd.h"
#include "assignsd.h"
#include "kbestEnforceConstraints.h"
#include "rt_nonfinite.h"

// Function Definitions
void kbestSDPartitionSolution(const cell_3 solution[1], coder::array<cell_3, 2U>
  &partitionedSolution)
{
  int loop_ub;
  int b_loop_ub;
  coder::array<unsigned int, 2U> toPartitionAssignment;
  int i;
  int i1;
  coder::array<double, 3U> constrainedCost;
  coder::array<unsigned int, 2U> b_toPartitionAssignment;
  unsigned int toPartitionAssignment_data[3];
  coder::array<double, 3U> b_constrainedCost;
  coder::array<unsigned int, 2U> soln;
  double cost;
  double newGap;
  if (1 > solution[0].f2.size(0)) {
    loop_ub = 0;
  } else {
    loop_ub = solution[0].f2.size(0);
  }

  b_loop_ub = solution[0].f2.size(1);
  toPartitionAssignment.set_size(loop_ub, solution[0].f2.size(1));
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      toPartitionAssignment[i1 + toPartitionAssignment.size(0) * i] = solution[0]
        .f2[i1 + solution[0].f2.size(0) * i];
    }
  }

  partitionedSolution.set_size(1, 0);
  i = loop_ub - 1;
  for (int k = 0; k <= i; k++) {
    int i2;
    if (1 > k) {
      loop_ub = 0;
    } else {
      loop_ub = k;
    }

    constrainedCost.set_size(solution[0].f1.size(0), solution[0].f1.size(1),
      solution[0].f1.size(2));
    b_loop_ub = solution[0].f1.size(0) * solution[0].f1.size(1) * solution[0].
      f1.size(2);
    for (i1 = 0; i1 < b_loop_ub; i1++) {
      constrainedCost[i1] = solution[0].f1[i1];
    }

    b_loop_ub = solution[0].f2.size(1) - 1;
    b_toPartitionAssignment.set_size(loop_ub, solution[0].f2.size(1));
    for (i1 = 0; i1 <= b_loop_ub; i1++) {
      toPartitionAssignment_data[i1] = toPartitionAssignment[k +
        toPartitionAssignment.size(0) * i1];
      for (i2 = 0; i2 < loop_ub; i2++) {
        b_toPartitionAssignment[i2 + b_toPartitionAssignment.size(0) * i1] =
          toPartitionAssignment[i2 + toPartitionAssignment.size(0) * i1];
      }
    }

    kbestEnforceConstraints(constrainedCost, toPartitionAssignment_data,
      b_toPartitionAssignment);
    b_constrainedCost.set_size(constrainedCost.size(0), constrainedCost.size(1),
      constrainedCost.size(2));
    loop_ub = constrainedCost.size(0) * constrainedCost.size(1) *
      constrainedCost.size(2) - 1;
    for (i1 = 0; i1 <= loop_ub; i1++) {
      b_constrainedCost[i1] = constrainedCost[i1];
    }

    assignsd(b_constrainedCost, soln, &cost, &newGap);
    if ((!rtIsInf(cost)) && (!rtIsNaN(cost))) {
      if (partitionedSolution.size(1) == 0) {
        partitionedSolution.set_size(1, 1);
        partitionedSolution[0] = solution[0];
      } else {
        i1 = partitionedSolution.size(1) + 1;
        partitionedSolution.set_size(1, (partitionedSolution.size(1) + 1));
        partitionedSolution[i1 - 1] = solution[0];
      }

      loop_ub = constrainedCost.size(0) * constrainedCost.size(1) *
        constrainedCost.size(2);
      partitionedSolution[partitionedSolution.size(0) *
        (partitionedSolution.size(1) - 1)].f1.set_size(constrainedCost.size(0),
        constrainedCost.size(1), constrainedCost.size(2));
      i1 = partitionedSolution.size(1) - 1;
      for (i2 = 0; i2 < loop_ub; i2++) {
        partitionedSolution[i1].f1[i2] = constrainedCost[i2];
      }

      loop_ub = soln.size(0) * soln.size(1);
      partitionedSolution[partitionedSolution.size(0) *
        (partitionedSolution.size(1) - 1)].f2.set_size(soln.size(0), soln.size(1));
      i1 = partitionedSolution.size(1) - 1;
      for (i2 = 0; i2 < loop_ub; i2++) {
        partitionedSolution[i1].f2[i2] = soln[i2];
      }

      i1 = partitionedSolution.size(1) - 1;
      partitionedSolution[partitionedSolution.size(1) - 1].f3[0] = cost;
      partitionedSolution[i1].f3[1] = newGap;
      partitionedSolution[partitionedSolution.size(1) - 1].f4 = (static_cast<
        double>(k) + 1.0) - 1.0;
    }
  }
}

// End of code generation (kbestSDPartitionSolution.cpp)
