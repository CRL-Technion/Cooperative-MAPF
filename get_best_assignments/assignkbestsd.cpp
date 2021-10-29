//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  assignkbestsd.cpp
//
//  Code generation for function 'assignkbestsd'
//


// Include files
#include "assignkbestsd.h"
#include "ITaskAssignment.h"
#include "assignsd.h"
#include "kbestSDPartitionSolution.h"
#include "repmat.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include "sortrows.h"

// Function Definitions
double assignkbestsd(const coder::array<double, 3U> &cost_matrix, double num_of_assignments,
                   coder::array<cell_wrap_0, 2U> &assignments, coder::array<
                   double, 1U> &cost, coder::array<double, 1U> &gap, vector<double> &times)
{
  clock_t t = clock();
  int k;
  coder::array<double, 3U> b_varargin_1;
  int loop_ub;
  int i;
  coder::array<unsigned int, 2U> soln;
  double currentSweep;
  double g;
  cell_3 r;
  coder::array<cell_3, 2U> solutionList;
  double b_varargin_2[2];
  coder::array<cell_3, 2U> optimalSolutions;
  coder::array<cell_3, 2U> foundSolutions;
  cell_3 b_solutionList[1];
  coder::array<cell_3, 2U> tempSolutionList;
  int b_i;
  int i1;
  coder::array<cell_3, 2U> checkSolutionList;
  coder::array<double, 1U> costs;
  coder::array<boolean_T, 1U> toClearIndex;
  coder::array<int, 1U> iidx;
  coder::array<cell_3, 2U> removedDupsList;
  cell_wrap_23 r1;
  coder::array<cell_3, 2U> newList;
  cell_wrap_23 rv[1];
  coder::array<cell_3, 2U> b_newList;
  double runtime_all = 0.0;

  k = 3;
  if (cost_matrix.size(2) == 1) {
    k = 2;
  }

  b_varargin_1.set_size(cost_matrix.size(0), cost_matrix.size(1), cost_matrix.size
                        (2));
  loop_ub = cost_matrix.size(0) * cost_matrix.size(1) * cost_matrix.size(2) - 1;
  for (i = 0; i <= loop_ub; i++) {
    b_varargin_1[i] = cost_matrix[i];
  }

  assignsd(b_varargin_1, soln, &currentSweep, &g);
  r.f1.set_size(cost_matrix.size(0), cost_matrix.size(1), cost_matrix.size(2));
  loop_ub = cost_matrix.size(0) * cost_matrix.size(1) * cost_matrix.size(2);
  for (i = 0; i < loop_ub; i++) {
    r.f1[i] = cost_matrix[i];
  }

  r.f2.set_size(soln.size(0), soln.size(1));
  loop_ub = soln.size(0) * soln.size(1);
  for (i = 0; i < loop_ub; i++) {
    r.f2[i] = soln[i];
  }

  r.f3[0] = currentSweep;
  r.f3[1] = g;
  r.f4 = 0.0;
  solutionList.set_size(1, 1);
  solutionList[0] = r;
  solutionList[0].f1.set_size(cost_matrix.size(0), cost_matrix.size(1),
    cost_matrix.size(2));
  loop_ub = cost_matrix.size(0) * cost_matrix.size(1) * cost_matrix.size(2);
  for (i = 0; i < loop_ub; i++) {
    solutionList[0].f1[i] = cost_matrix[i];
  }

  solutionList[0].f2.set_size(soln.size(0), soln.size(1));
  loop_ub = soln.size(0) * soln.size(1);
  for (i = 0; i < loop_ub; i++) {
    solutionList[0].f2[i] = soln[i];
  }

  solutionList[0].f3[0] = currentSweep;
  solutionList[0].f3[1] = g;
  solutionList[0].f4 = 0.0;
  b_varargin_2[0] = num_of_assignments;
  b_varargin_2[1] = 1.0;
  repmat((cell_3 *)solutionList.data(), b_varargin_2, optimalSolutions);
  currentSweep = 1.0;
  runtime_all += (double) (clock() - t) / CLOCKS_PER_SEC;
  while ((solutionList.size(1) != 0) && (currentSweep <= num_of_assignments)) {
    t = clock();
    b_solutionList[0] = solutionList[0];
    kbestSDPartitionSolution(b_solutionList, tempSolutionList);
    i = tempSolutionList.size(1);
    for (b_i = 0; b_i < i; b_i++) {
      i1 = solutionList.size(1) + 1;
      solutionList.set_size(1, (solutionList.size(1) + 1));
      solutionList[i1 - 1] = tempSolutionList[b_i];
    }

    checkSolutionList.set_size(1, 0);
    i = static_cast<int>(currentSweep - 1.0);
    for (loop_ub = 0; loop_ub < i; loop_ub++) {
      i1 = checkSolutionList.size(1) + 1;
      checkSolutionList.set_size(1, (checkSolutionList.size(1) + 1));
      checkSolutionList[i1 - 1] = optimalSolutions[loop_ub];
    }

    i = solutionList.size(1);
    for (loop_ub = 0; loop_ub < i; loop_ub++) {
      i1 = checkSolutionList.size(1) + 1;
      checkSolutionList.set_size(1, (checkSolutionList.size(1) + 1));
      checkSolutionList[i1 - 1] = solutionList[loop_ub];
    }

    toClearIndex.set_size(checkSolutionList.size(1));
    loop_ub = checkSolutionList.size(1);
    for (i = 0; i < loop_ub; i++) {
      toClearIndex[i] = false;
    }

    i = checkSolutionList.size(1);
    for (b_i = 0; b_i < i; b_i++) {
      i1 = checkSolutionList.size(1) - b_i;
      for (int j = 0; j <= i1 - 2; j++) {
        unsigned int b_j;
        boolean_T p;
        boolean_T b_p;
        b_j = (static_cast<unsigned int>(b_i) + j) + 2U;
        p = false;
        if ((checkSolutionList[b_i].f2.size(0) == checkSolutionList[static_cast<
             int>(b_j) - 1].f2.size(0)) && (checkSolutionList[b_i].f2.size(1) ==
             checkSolutionList[static_cast<int>(b_j) - 1].f2.size(1))) {
          p = true;
        }

        if (p && ((checkSolutionList[b_i].f2.size(0) != 0) &&
                  (checkSolutionList[b_i].f2.size(1) != 0)) &&
            ((checkSolutionList[static_cast<int>(b_j) - 1].f2.size(0) != 0) &&
             (checkSolutionList[static_cast<int>(b_j) - 1].f2.size(1) != 0))) {
          boolean_T exitg1;
          loop_ub = 0;
          exitg1 = false;
          while ((!exitg1) && (loop_ub <= checkSolutionList[static_cast<int>(b_j)
                               - 1].f2.size(0) * checkSolutionList[static_cast<
                               int>(b_j) - 1].f2.size(1) - 1)) {
            if (checkSolutionList[b_i].f2[loop_ub] != checkSolutionList[
                static_cast<int>(b_j) - 1].f2[loop_ub]) {
              p = false;
              exitg1 = true;
            } else {
              loop_ub++;
            }
          }
        }

        b_p = p;
        if (b_p) {
          loop_ub = static_cast<int>(b_j) - 1;
          if (checkSolutionList[b_i].f4 <= checkSolutionList[loop_ub].f4) {
            toClearIndex[loop_ub] = true;
          } else {
            toClearIndex[b_i] = true;
          }
        }
      }
    }

    removedDupsList.set_size(1, 0);
    i = toClearIndex.size(0);
    for (b_i = 0; b_i < i; b_i++) {
      if (!toClearIndex[b_i]) {
        i1 = removedDupsList.size(1) + 1;
        removedDupsList.set_size(1, (removedDupsList.size(1) + 1));
        removedDupsList[i1 - 1] = checkSolutionList[b_i];
      }
    }

    newList.set_size(1, 0);
    i = static_cast<int>(static_cast<double>(removedDupsList.size(1)) + (1.0 -
      currentSweep));
    for (loop_ub = 0; loop_ub < i; loop_ub++) {
      i1 = newList.size(1) + 1;
      newList.set_size(1, (newList.size(1) + 1));
      newList[i1 - 1] = removedDupsList[static_cast<int>(currentSweep +
        static_cast<double>(loop_ub)) - 1];
    }

    solutionList.set_size(1, newList.size(1));
    loop_ub = newList.size(0) * newList.size(1);
    for (i = 0; i < loop_ub; i++) {
      solutionList[i] = newList[i];
    }

    costs.set_size(newList.size(1));
    loop_ub = newList.size(1);
    for (i = 0; i < loop_ub; i++) {
      costs[i] = 0.0;
    }

    i = newList.size(1);
    for (b_i = 0; b_i < i; b_i++) {
      costs[b_i] = newList[b_i].f3[0];
    }

    sort(costs, iidx);
    costs.set_size(iidx.size(0));
    loop_ub = iidx.size(0);
    for (i = 0; i < loop_ub; i++) {
      costs[i] = iidx[i];
    }

    i = costs.size(0);
    for (b_i = 0; b_i < i; b_i++) {
      solutionList[b_i] = newList[static_cast<int>(costs[b_i]) - 1];
    }

    optimalSolutions[static_cast<int>(currentSweep) - 1].f1.set_size
      (solutionList[0].f1.size(0), solutionList[0].f1.size(1), solutionList[0].
       f1.size(2));
    loop_ub = solutionList[0].f1.size(0) * solutionList[0].f1.size(1) *
      solutionList[0].f1.size(2);
    for (i = 0; i < loop_ub; i++) {
      optimalSolutions[static_cast<int>(currentSweep) - 1].f1[i] = solutionList
        [0].f1[i];
    }

    optimalSolutions[static_cast<int>(currentSweep) - 1].f2.set_size
      (solutionList[0].f2.size(0), solutionList[0].f2.size(1));
    loop_ub = solutionList[0].f2.size(0) * solutionList[0].f2.size(1);
    for (i = 0; i < loop_ub; i++) {
      optimalSolutions[static_cast<int>(currentSweep) - 1].f2[i] = solutionList
        [0].f2[i];
    }

    optimalSolutions[static_cast<int>(currentSweep) - 1].f3[0] = solutionList[0]
      .f3[0];
    optimalSolutions[static_cast<int>(currentSweep) - 1].f3[1] = solutionList[0]
      .f3[1];
    optimalSolutions[static_cast<int>(currentSweep) - 1].f4 = solutionList[0].f4;
    currentSweep++;
    b_newList.set_size(1, 0);
    i = solutionList.size(1);
    for (b_i = 0; b_i <= i - 2; b_i++) {
      i1 = b_newList.size(1) + 1;
      b_newList.set_size(1, (b_newList.size(1) + 1));
      b_newList[i1 - 1] = solutionList[b_i + 1];
    }

    solutionList.set_size(1, b_newList.size(1));
    loop_ub = b_newList.size(0) * b_newList.size(1);
    for (i = 0; i < loop_ub; i++) {
      solutionList[i] = b_newList[i];
    }
    times[int(currentSweep) - 2] = (double) (clock() - t) / CLOCKS_PER_SEC;
  }

  t = clock();

  foundSolutions.set_size(1, 0);
  i = static_cast<int>(currentSweep - 1.0);
  for (b_i = 0; b_i < i; b_i++) {
    i1 = foundSolutions.size(1) + 1;
    foundSolutions.set_size(1, (foundSolutions.size(1) + 1));
    foundSolutions[i1 - 1] = optimalSolutions[b_i];
  }

  solutionList.set_size(1, foundSolutions.size(1));
  loop_ub = foundSolutions.size(0) * foundSolutions.size(1);
  for (i1 = 0; i1 < loop_ub; i1++) {
    solutionList[i1] = foundSolutions[i1];
  }

  costs.set_size(foundSolutions.size(1));
  loop_ub = foundSolutions.size(1);
  for (i1 = 0; i1 < loop_ub; i1++) {
    costs[i1] = 0.0;
  }

  i1 = foundSolutions.size(1);
  for (b_i = 0; b_i < i1; b_i++) {
    costs[b_i] = foundSolutions[b_i].f3[0];
  }

  sort(costs, iidx);
  costs.set_size(iidx.size(0));
  loop_ub = iidx.size(0);
  for (i1 = 0; i1 < loop_ub; i1++) {
    costs[i1] = iidx[i1];
  }

  i1 = costs.size(0);
  for (b_i = 0; b_i < i1; b_i++) {
    solutionList[b_i] = foundSolutions[static_cast<int>(costs[b_i]) - 1];
  }

  r1.f1.size[0] = 0;
  r1.f1.size[1] = k;
  rv[0] = r1;
  b_varargin_2[0] = currentSweep - 1.0;
  b_varargin_2[1] = 1.0;
  b_repmat(rv, b_varargin_2, assignments);
  cost.set_size(i);
  gap.set_size(i);
  for (b_i = 0; b_i < i; b_i++) {
    assignments[b_i].f1.set_size(solutionList[solutionList.size(0) * b_i].
      f2.size(0), solutionList[solutionList.size(0) * b_i].f2.size(1));
    loop_ub = solutionList[b_i].f2.size(0) * solutionList[b_i].f2.size(1);
    for (i1 = 0; i1 < loop_ub; i1++) {
      assignments[b_i].f1[i1] = solutionList[b_i].f2[i1];
    }

    b_sortrows(assignments[b_i].f1);
    cost[b_i] = solutionList[b_i].f3[0];
    gap[b_i] = solutionList[b_i].f3[1];
  }
  runtime_all += (double) (clock() - t) / CLOCKS_PER_SEC;
  return runtime_all;
}

// End of code generation (assignkbestsd.cpp)
