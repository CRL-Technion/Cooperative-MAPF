//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  kbestEnforceConstraints.h
//
//  Code generation for function 'kbestEnforceConstraints'
//


#ifndef KBESTENFORCECONSTRAINTS_H
#define KBESTENFORCECONSTRAINTS_H

// Include files
#include <cstddef>
#include <cstdlib>
#include "rtwtypes.h"
#include "get_best_assignments_types.h"

// Function Declarations
extern void kbestEnforceConstraints(coder::array<double, 3U> &costMatrix, const
  unsigned int tuplesToRemove_data[], const coder::array<unsigned int, 2U>
  &tuplesToEnforce);

#endif

// End of code generation (kbestEnforceConstraints.h)
