//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  assignsd.h
//
//  Code generation for function 'assignsd'
//


#ifndef ASSIGNSD_H
#define ASSIGNSD_H

// Include files
#include <cstddef>
#include <cstdlib>
#include "rtwtypes.h"
#include "get_best_assignments_types.h"

// Function Declarations
extern void assignsd(coder::array<double, 3U> &costMatrix, coder::array<unsigned
                     int, 2U> &assignments, double *costOfAssignment, double
                     *solutionGap);

#endif

// End of code generation (assignsd.h)
