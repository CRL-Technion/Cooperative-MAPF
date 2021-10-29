//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  get_best_assignments_rtwutil.cpp
//
//  Code generation for function 'get_best_assignments_rtwutil'
//


// Include files
#include "get_best_assignments_rtwutil.h"
#include "ITaskAssignment.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
double rt_roundd_snf(double u)
{
  double y;
  if (std::abs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = std::floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = std::ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

// End of code generation (get_best_assignments_rtwutil.cpp)
