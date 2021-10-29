//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  bsxfun.cpp
//
//  Code generation for function 'bsxfun'
//


// Include files
#include "bsxfun.h"
#include "ITaskAssignment.h"
#include "assignkbestsd.h"
#include "rt_nonfinite.h"

// Function Definitions
void b_bsxfun(const coder::array<double, 3U> &a, const double b_data[], const
              int b_size[2], coder::array<double, 3U> &c)
{
  int acoef;
  int i;
  if (b_size[1] == 1) {
    acoef = static_cast<signed char>(a.size(1));
  } else if (a.size(1) == 1) {
    acoef = static_cast<signed char>(b_size[1]);
  } else if (a.size(1) == b_size[1]) {
    acoef = static_cast<signed char>(a.size(1));
  } else if (b_size[1] < a.size(1)) {
    acoef = static_cast<signed char>(b_size[1]);
  } else {
    acoef = static_cast<signed char>(a.size(1));
  }

  c.set_size((static_cast<int>(static_cast<signed char>(a.size(0)))), acoef, (
              static_cast<int>(static_cast<signed char>(a.size(2)))));
  acoef = (a.size(2) != 1);
  i = static_cast<signed char>(a.size(2)) - 1;
  for (int k = 0; k <= i; k++) {
    int varargin_2;
    int b_acoef;
    int bcoef;
    int i1;
    varargin_2 = acoef * k;
    b_acoef = (a.size(1) != 1);
    bcoef = (b_size[1] != 1);
    i1 = c.size(1) - 1;
    for (int b_k = 0; b_k <= i1; b_k++) {
      int varargin_3;
      int varargin_5;
      int c_acoef;
      int i2;
      varargin_3 = b_acoef * b_k;
      varargin_5 = bcoef * b_k;
      c_acoef = (a.size(0) != 1);
      i2 = c.size(0) - 1;
      for (int c_k = 0; c_k <= i2; c_k++) {
        c[(c_k + c.size(0) * b_k) + c.size(0) * c.size(1) * k] = a[(c_acoef *
          c_k + a.size(0) * varargin_3) + a.size(0) * a.size(1) * varargin_2] -
          b_data[varargin_5];
      }
    }
  }
}

void bsxfun(const coder::array<double, 3U> &a, const double b_data[], const int
            b_size[1], coder::array<double, 3U> &c)
{
  int acoef;
  int i;
  if (b_size[0] == 1) {
    acoef = static_cast<signed char>(a.size(0));
  } else if (a.size(0) == 1) {
    acoef = static_cast<signed char>(b_size[0]);
  } else if (a.size(0) == b_size[0]) {
    acoef = static_cast<signed char>(a.size(0));
  } else if (b_size[0] < a.size(0)) {
    acoef = static_cast<signed char>(b_size[0]);
  } else {
    acoef = static_cast<signed char>(a.size(0));
  }

  c.set_size(acoef, (static_cast<int>(static_cast<signed char>(a.size(1)))), (
              static_cast<int>(static_cast<signed char>(a.size(2)))));
  acoef = (a.size(2) != 1);
  i = static_cast<signed char>(a.size(2)) - 1;
  for (int k = 0; k <= i; k++) {
    int varargin_2;
    int b_acoef;
    int i1;
    varargin_2 = acoef * k;
    b_acoef = (a.size(1) != 1);
    i1 = c.size(1) - 1;
    for (int b_k = 0; b_k <= i1; b_k++) {
      int varargin_3;
      int c_acoef;
      int bcoef;
      int i2;
      varargin_3 = b_acoef * b_k;
      c_acoef = (a.size(0) != 1);
      bcoef = (b_size[0] != 1);
      i2 = c.size(0) - 1;
      for (int c_k = 0; c_k <= i2; c_k++) {
        c[(c_k + c.size(0) * b_k) + c.size(0) * c.size(1) * k] = a[(c_acoef *
          c_k + a.size(0) * varargin_3) + a.size(0) * a.size(1) * varargin_2] -
          b_data[bcoef * c_k];
      }
    }
  }
}

void c_bsxfun(const coder::array<double, 3U> &a, const double b_data[], const
              int b_size[3], coder::array<double, 3U> &c)
{
  int b_b_size;
  int acoef;
  int bcoef;
  if (b_size[2] == 1) {
    b_b_size = static_cast<signed char>(a.size(2));
  } else if (a.size(2) == 1) {
    b_b_size = static_cast<signed char>(b_size[2]);
  } else if (a.size(2) == b_size[2]) {
    b_b_size = static_cast<signed char>(a.size(2));
  } else if (b_size[2] < a.size(2)) {
    b_b_size = static_cast<signed char>(b_size[2]);
  } else {
    b_b_size = static_cast<signed char>(a.size(2));
  }

  c.set_size((static_cast<int>(static_cast<signed char>(a.size(0)))), (
              static_cast<int>(static_cast<signed char>(a.size(1)))), b_b_size);
  acoef = (a.size(2) != 1);
  bcoef = (b_size[2] != 1);
  if (b_size[2] == 1) {
    b_b_size = static_cast<signed char>(a.size(2));
  } else if (a.size(2) == 1) {
    b_b_size = static_cast<signed char>(b_size[2]);
  } else if (a.size(2) == b_size[2]) {
    b_b_size = static_cast<signed char>(a.size(2));
  } else if (b_size[2] < a.size(2)) {
    b_b_size = static_cast<signed char>(b_size[2]);
  } else {
    b_b_size = static_cast<signed char>(a.size(2));
  }

  b_b_size--;
  for (int k = 0; k <= b_b_size; k++) {
    int varargin_2;
    int varargin_3;
    int b_acoef;
    int i;
    varargin_2 = acoef * k;
    varargin_3 = bcoef * k;
    b_acoef = (a.size(1) != 1);
    i = c.size(1) - 1;
    for (int b_k = 0; b_k <= i; b_k++) {
      int b_varargin_3;
      int c_acoef;
      int i1;
      b_varargin_3 = b_acoef * b_k;
      c_acoef = (a.size(0) != 1);
      i1 = c.size(0) - 1;
      for (int c_k = 0; c_k <= i1; c_k++) {
        c[(c_k + c.size(0) * b_k) + c.size(0) * c.size(1) * k] = a[(c_acoef *
          c_k + a.size(0) * b_varargin_3) + a.size(0) * a.size(1) * varargin_2]
          - b_data[varargin_3];
      }
    }
  }
}

// End of code generation (bsxfun.cpp)
