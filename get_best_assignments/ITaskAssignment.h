//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  ITaskAssignment.h
//
//  Code generation for function 'ITaskAssignment'
//


#ifndef ITASKASSIGNMENT_H
#define ITASKASSIGNMENT_H

// Include files
#include <cstddef>
#include <cstdlib>
#include "rtwtypes.h"
#include "get_best_assignments_types.h"
#include "common.h"

// Type Definitions
class GET_BEST_ASSIGNMENTS_DLL_EXPORT ITaskAssignment
{
 public:
  ITaskAssignment();
  ~ITaskAssignment();
  double get_best_assignments(const coder::array<double, 3U> &costMatrix, double k,
    coder::array<cell_wrap_0, 2U> &assignments, coder::array<double, 1U> &cost,
    coder::array<double, 1U> &solutionGap, vector<double> &times);
};

#endif

// End of code generation (ITaskAssignment.h)
