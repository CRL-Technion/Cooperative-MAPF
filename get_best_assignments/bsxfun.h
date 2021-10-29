//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  bsxfun.h
//
//  Code generation for function 'bsxfun'
//


#ifndef BSXFUN_H
#define BSXFUN_H

// Include files
#include <cstddef>
#include <cstdlib>
#include "rtwtypes.h"
#include "get_best_assignments_types.h"

// Function Declarations
extern void b_bsxfun(const coder::array<double, 3U> &a, const double b_data[],
                     const int b_size[2], coder::array<double, 3U> &c);
extern void bsxfun(const coder::array<double, 3U> &a, const double b_data[],
                   const int b_size[1], coder::array<double, 3U> &c);
extern void c_bsxfun(const coder::array<double, 3U> &a, const double b_data[],
                     const int b_size[3], coder::array<double, 3U> &c);

#endif

// End of code generation (bsxfun.h)
