//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  assignAuctionWithPrice.h
//
//  Code generation for function 'assignAuctionWithPrice'
//


#ifndef ASSIGNAUCTIONWITHPRICE_H
#define ASSIGNAUCTIONWITHPRICE_H

// Include files
#include <cstddef>
#include <cstdlib>
#include "rtwtypes.h"
#include "get_best_assignments_types.h"

// Function Declarations
extern void assignAuctionWithPrice(coder::array<double, 2U> &costMatrix, coder::
  array<unsigned int, 2U> &assignments, coder::array<unsigned int, 2U>
  &unassignedRows, coder::array<unsigned int, 2U> &unassignedColumns, coder::
  array<double, 2U> &price);

#endif

// End of code generation (assignAuctionWithPrice.h)
