//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  ITaskAssignment.cpp
//
//  Code generation for function 'ITaskAssignment'
//


// Include files
#include "ITaskAssignment.h"
#include "assignkbestsd.h"
#include "rt_nonfinite.h"


// Function Definitions
ITaskAssignment::~ITaskAssignment()
{
  // (no terminate code required)
}

ITaskAssignment::ITaskAssignment()
{
  rt_InitInfAndNaN();
}

double ITaskAssignment::get_best_assignments(const coder::array<double, 3U>
  &costMatrix, double k, coder::array<cell_wrap_0, 2U> &assignments, coder::
  array<double, 1U> &cost, coder::array<double, 1U> &solutionGap, vector<double> &times)
{
  coder::array<double, 3U> fullMatrix;
  int loop_ub;
  int i;
  int i1;
  int i2;
  int i3;
  int i4;
  int unnamed_idx_0;
  int unnamed_idx_1;
  int unnamed_idx_2;

  //  fid = fopen('cost_matrix.txt');
  //  C = textscan(fid, '%s');
  //  fclose(fid);
  //  C = C{1, 1};
  //  K = sqrt(length(C));
  //
  //  costMatrix = zeros(K, K, K);
  //
  //  for i = 1:K
  //      line_idx = (i-1) * K + 1;
  //      for j = 1:K
  //          row_index = line_idx + j - 1;
  //          row = cellfun(@(x) str2double(x), strsplit(C{row_index}, ','));
  //          for k = 1:K
  //              costMatrix(i, j, k) = row(k);
  //          end
  //      end
  //  end
  fullMatrix.set_size((costMatrix.size(0) + 1), (costMatrix.size(0) + 1),
                      (costMatrix.size(0) + 1));
  loop_ub = (costMatrix.size(0) + 1) * (costMatrix.size(0) + 1) *
    (costMatrix.size(0) + 1);
  for (i = 0; i < loop_ub; i++) {
    fullMatrix[i] = rtInf;
  }

  if (2 > costMatrix.size(0) + 1) {
    i = 0;
    i1 = -1;
    loop_ub = 0;
    i2 = -1;
    i3 = 0;
    i4 = -1;
  } else {
    i = 1;
    i1 = costMatrix.size(0);
    loop_ub = 1;
    i2 = costMatrix.size(0);
    i3 = 1;
    i4 = costMatrix.size(0);
  }

  unnamed_idx_0 = (i1 - i) + 1;
  unnamed_idx_1 = (i2 - loop_ub) + 1;
  unnamed_idx_2 = (i4 - i3) + 1;
  for (i1 = 0; i1 < unnamed_idx_2; i1++) {
    for (i2 = 0; i2 < unnamed_idx_1; i2++) {
      for (i4 = 0; i4 < unnamed_idx_0; i4++) {
        fullMatrix[((i + i4) + fullMatrix.size(0) * (loop_ub + i2)) +
          fullMatrix.size(0) * fullMatrix.size(1) * (i3 + i1)] = costMatrix[(i4
          + unnamed_idx_0 * i2) + unnamed_idx_0 * unnamed_idx_1 * i1];
      }
    }
  }

  fullMatrix[0] = 0.0;

  //  [assignments, cost, gapAchieved] = assignsd(fullMatrix,0.01,100);
  double runtime_all = assignkbestsd(fullMatrix, k, assignments, cost, solutionGap, times);
  return runtime_all;
}

// End of code generation (ITaskAssignment.cpp)
