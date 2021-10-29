//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  assign2DRelaxed.h
//
//  Code generation for function 'assign2DRelaxed'
//


#ifndef ASSIGN2DRELAXED_H
#define ASSIGN2DRELAXED_H

// Include files
#include <cstddef>
#include <cstdlib>
#include "rtwtypes.h"
#include "get_best_assignments_types.h"

// Function Declarations
extern void assign2DRelaxed(coder::array<double, 2U> &costMatrix, coder::array<
  unsigned int, 2U> &assign, double *cost, double price_data[], int price_size[2]);

#endif

// End of code generation (assign2DRelaxed.h)
