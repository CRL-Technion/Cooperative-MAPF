//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  get_best_assignments_types.h
//
//  Code generation for function 'get_best_assignments_types'
//


#ifndef GET_BEST_ASSIGNMENTS_TYPES_H
#define GET_BEST_ASSIGNMENTS_TYPES_H

// Include files
#include "rtwtypes.h"
#include "coder_array.h"
#ifdef GET_BEST_ASSIGNMENTS_XIL_BUILD
#if defined(_MSC_VER) || defined(__LCC__)
#define GET_BEST_ASSIGNMENTS_DLL_EXPORT __declspec(dllimport)
#else
#define GET_BEST_ASSIGNMENTS_DLL_EXPORT
#endif

#elif defined(BUILDING_GET_BEST_ASSIGNMENTS)
#if defined(_MSC_VER) || defined(__LCC__)
#define GET_BEST_ASSIGNMENTS_DLL_EXPORT __declspec(dllexport)
#else
#define GET_BEST_ASSIGNMENTS_DLL_EXPORT __attribute__ ((visibility("default")))
#endif

#else
#define GET_BEST_ASSIGNMENTS_DLL_EXPORT
#endif

#ifdef _MSC_VER

#pragma warning(push)
#pragma warning(disable : 4251)

#endif

// Type Declarations
struct cell_3;
struct emxArray_uint32_T_0x3;
struct cell_wrap_23;
struct cell_wrap_18;
struct cell_wrap_0;
struct cell_wrap_11;
class ITaskAssignment;

// Type Definitions
struct cell_3
{
  coder::array<double, 3U> f1;
  coder::array<unsigned int, 2U> f2;
  double f3[2];
  double f4;
};

struct emxArray_uint32_T_0x3
{
  int size[2];
};

struct cell_wrap_23
{
  emxArray_uint32_T_0x3 f1;
};

struct cell_wrap_18
{
  coder::array<unsigned int, 1U> f1;
};

struct cell_wrap_0
{
  coder::array<unsigned int, 2U> f1;
};

struct cell_wrap_11
{
  coder::array<double, 2U> f1;
};

#ifdef _MSC_VER

#pragma warning(pop)

#endif
#endif

// End of code generation (get_best_assignments_types.h)
