//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  lapAuction.h
//
//  Code generation for function 'lapAuction'
//


#ifndef LAPAUCTION_H
#define LAPAUCTION_H

// Include files
#include <cstddef>
#include <cstdlib>
#include "rtwtypes.h"
#include "get_best_assignments_types.h"

// Function Declarations
extern void lapAuction(const coder::array<double, 2U> &costMatrix, coder::array<
  double, 1U> &rowSoln, coder::array<double, 2U> &colSoln, coder::array<double,
  2U> &colRedux);

#endif

// End of code generation (lapAuction.h)
