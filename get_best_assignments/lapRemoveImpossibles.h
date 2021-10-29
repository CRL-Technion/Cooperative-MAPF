//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  lapRemoveImpossibles.h
//
//  Code generation for function 'lapRemoveImpossibles'
//


#ifndef LAPREMOVEIMPOSSIBLES_H
#define LAPREMOVEIMPOSSIBLES_H

// Include files
#include <cstddef>
#include <cstdlib>
#include "rtwtypes.h"
#include "get_best_assignments_types.h"

// Function Declarations
extern void lapRemoveImpossibles(coder::array<double, 2U> &costMatrix, coder::
  array<unsigned int, 1U> &rowIdx, coder::array<unsigned int, 2U> &colIdx, coder::
  array<unsigned int, 1U> &unassignedRows, coder::array<unsigned int, 2U>
  &unassignedColumns);

#endif

// End of code generation (lapRemoveImpossibles.h)
