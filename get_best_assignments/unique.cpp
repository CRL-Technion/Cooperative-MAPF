//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  unique.cpp
//
//  Code generation for function 'unique'
//


// Include files
#include "unique.h"
#include "ITaskAssignment.h"
#include "assignkbestsd.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include "sortIdx.h"
#include <cmath>
#include <math.h>

// Function Definitions
void unique_vector(const coder::array<double, 1U> &a, coder::array<double, 1U>
                   &b)
{
  int na;
  int n;
  coder::array<int, 1U> idx;
  int i;
  int b_i;
  coder::array<int, 1U> iwork;
  int k;
  int i2;
  int j;
  int pEnd;
  boolean_T exitg1;
  int p;
  double absx;
  int qEnd;
  int exponent;
  na = a.size(0);
  n = a.size(0) + 1;
  idx.set_size(a.size(0));
  i = a.size(0);
  for (b_i = 0; b_i < i; b_i++) {
    idx[b_i] = 0;
  }

  if (a.size(0) != 0) {
    iwork.set_size(a.size(0));
    b_i = a.size(0) - 1;
    for (k = 1; k <= b_i; k += 2) {
      if ((a[k - 1] <= a[k]) || rtIsNaN(a[k])) {
        idx[k - 1] = k;
        idx[k] = k + 1;
      } else {
        idx[k - 1] = k + 1;
        idx[k] = k;
      }
    }

    if ((a.size(0) & 1) != 0) {
      idx[a.size(0) - 1] = a.size(0);
    }

    i = 2;
    while (i < n - 1) {
      i2 = i << 1;
      j = 1;
      for (pEnd = i + 1; pEnd < n; pEnd = qEnd + i) {
        int q;
        int kEnd;
        p = j;
        q = pEnd - 1;
        qEnd = j + i2;
        if (qEnd > n) {
          qEnd = n;
        }

        k = 0;
        kEnd = qEnd - j;
        while (k + 1 <= kEnd) {
          absx = a[idx[q] - 1];
          b_i = idx[p - 1];
          if ((a[b_i - 1] <= absx) || rtIsNaN(absx)) {
            iwork[k] = b_i;
            p++;
            if (p == pEnd) {
              while (q + 1 < qEnd) {
                k++;
                iwork[k] = idx[q];
                q++;
              }
            }
          } else {
            iwork[k] = idx[q];
            q++;
            if (q + 1 == qEnd) {
              while (p < pEnd) {
                k++;
                iwork[k] = idx[p - 1];
                p++;
              }
            }
          }

          k++;
        }

        for (k = 0; k < kEnd; k++) {
          idx[(j + k) - 1] = iwork[k];
        }

        j = qEnd;
      }

      i = i2;
    }
  }

  b.set_size(a.size(0));
  for (k = 0; k < na; k++) {
    b[k] = a[idx[k] - 1];
  }

  k = 0;
  while ((k + 1 <= na) && rtIsInf(b[k]) && (b[k] < 0.0)) {
    k++;
  }

  i2 = k;
  k = a.size(0);
  while ((k >= 1) && rtIsNaN(b[k - 1])) {
    k--;
  }

  pEnd = a.size(0) - k;
  exitg1 = false;
  while ((!exitg1) && (k >= 1)) {
    absx = b[k - 1];
    if (rtIsInf(absx) && (absx > 0.0)) {
      k--;
    } else {
      exitg1 = true;
    }
  }

  i = (a.size(0) - k) - pEnd;
  p = -1;
  if (i2 > 0) {
    p = 0;
  }

  while (i2 + 1 <= k) {
    double x;
    x = b[i2];
    int exitg2;
    do {
      exitg2 = 0;
      i2++;
      if (i2 + 1 > k) {
        exitg2 = 1;
      } else {
        absx = std::abs(x / 2.0);
        if ((!rtIsInf(absx)) && (!rtIsNaN(absx))) {
          if (absx <= 2.2250738585072014E-308) {
            absx = 4.94065645841247E-324;
          } else {
            frexp(absx, &exponent);
            absx = std::ldexp(1.0, exponent - 53);
          }
        } else {
          absx = rtNaN;
        }

        if ((!(std::abs(x - b[i2]) < absx)) && ((!rtIsInf(b[i2])) || (!rtIsInf(x))
             || ((b[i2] > 0.0) != (x > 0.0)))) {
          exitg2 = 1;
        }
      }
    } while (exitg2 == 0);

    p++;
    b[p] = x;
  }

  if (i > 0) {
    p++;
    b[p] = b[k];
  }

  i2 = k + i;
  for (j = 0; j < pEnd; j++) {
    p++;
    b[p] = b[i2 + j];
  }

  if (1 > p + 1) {
    b_i = 0;
  } else {
    b_i = p + 1;
  }

  b.set_size(b_i);
}

// End of code generation (unique.cpp)
