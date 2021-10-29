//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  sortrows.cpp
//
//  Code generation for function 'sortrows'
//


// Include files
#include "sortrows.h"
#include "ITaskAssignment.h"
#include "assignkbestsd.h"
#include "assignsd.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include "sortIdx.h"

// Function Definitions
void b_sortrows(coder::array<unsigned int, 2U> &y)
{
  int n;
  int col_size_idx_1;
  int k;
  signed char col_data[3];
  coder::array<int, 1U> idx;
  int i2;
  int pEnd;
  coder::array<int, 1U> iwork;
  coder::array<unsigned int, 1U> ycol;
  int i;
  int j;
  int qEnd;
  n = y.size(1);
  col_size_idx_1 = y.size(1);
  for (k = 0; k < n; k++) {
    col_data[k] = static_cast<signed char>(k + 1);
  }

  n = y.size(0) + 1;
  idx.set_size(y.size(0));
  i2 = y.size(0);
  for (pEnd = 0; pEnd < i2; pEnd++) {
    idx[pEnd] = 0;
  }

  if ((y.size(0) == 0) || (y.size(1) == 0)) {
    for (k = 0; k <= n - 2; k++) {
      idx[k] = k + 1;
    }
  } else {
    boolean_T p;
    int b_k;
    boolean_T exitg1;
    int v1_tmp;
    unsigned int v1;
    unsigned int v2;
    iwork.set_size(y.size(0));
    pEnd = y.size(0) - 1;
    for (k = 1; k <= pEnd; k += 2) {
      p = true;
      b_k = 0;
      exitg1 = false;
      while ((!exitg1) && (b_k <= col_size_idx_1 - 1)) {
        v1_tmp = col_data[b_k] - 1;
        v1 = y[(k + y.size(0) * v1_tmp) - 1];
        v2 = y[k + y.size(0) * v1_tmp];
        if (v1 == v2) {
          b_k++;
        } else {
          p = (v1 <= v2);
          exitg1 = true;
        }
      }

      if (p) {
        idx[k - 1] = k;
        idx[k] = k + 1;
      } else {
        idx[k - 1] = k + 1;
        idx[k] = k;
      }
    }

    if ((y.size(0) & 1) != 0) {
      idx[y.size(0) - 1] = y.size(0);
    }

    i = 2;
    while (i < n - 1) {
      i2 = i << 1;
      j = 1;
      for (pEnd = i + 1; pEnd < n; pEnd = qEnd + i) {
        int b_p;
        int q;
        int kEnd;
        b_p = j;
        q = pEnd;
        qEnd = j + i2;
        if (qEnd > n) {
          qEnd = n;
        }

        k = 0;
        kEnd = qEnd - j;
        while (k + 1 <= kEnd) {
          p = true;
          b_k = 0;
          exitg1 = false;
          while ((!exitg1) && (b_k <= col_size_idx_1 - 1)) {
            v1_tmp = col_data[b_k] - 1;
            v1 = y[(idx[b_p - 1] + y.size(0) * v1_tmp) - 1];
            v2 = y[(idx[q - 1] + y.size(0) * v1_tmp) - 1];
            if (v1 == v2) {
              b_k++;
            } else {
              p = (v1 <= v2);
              exitg1 = true;
            }
          }

          if (p) {
            iwork[k] = idx[b_p - 1];
            b_p++;
            if (b_p == pEnd) {
              while (q < qEnd) {
                k++;
                iwork[k] = idx[q - 1];
                q++;
              }
            }
          } else {
            iwork[k] = idx[q - 1];
            q++;
            if (q == qEnd) {
              while (b_p < pEnd) {
                k++;
                iwork[k] = idx[b_p - 1];
                b_p++;
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

  i2 = y.size(0) - 1;
  n = y.size(1);
  ycol.set_size(y.size(0));
  for (j = 0; j < n; j++) {
    for (i = 0; i <= i2; i++) {
      ycol[i] = y[(idx[i] + y.size(0) * j) - 1];
    }

    for (i = 0; i <= i2; i++) {
      y[i + y.size(0) * j] = ycol[i];
    }
  }
}

void sortrows(coder::array<unsigned int, 2U> &y)
{
  int n;
  coder::array<int, 1U> idx;
  int i2;
  int j;
  coder::array<int, 1U> iwork;
  int k;
  coder::array<unsigned int, 1U> ycol;
  int i;
  int qEnd;
  n = y.size(0) + 1;
  idx.set_size(y.size(0));
  i2 = y.size(0);
  for (j = 0; j < i2; j++) {
    idx[j] = 0;
  }

  if (y.size(0) == 0) {
    for (k = 0; k <= n - 2; k++) {
      idx[k] = k + 1;
    }
  } else {
    boolean_T p;
    int b_k;
    boolean_T exitg1;
    unsigned int v1;
    unsigned int v2;
    iwork.set_size(y.size(0));
    j = y.size(0) - 1;
    for (k = 1; k <= j; k += 2) {
      p = true;
      b_k = 0;
      exitg1 = false;
      while ((!exitg1) && (b_k < 2)) {
        v1 = y[(k + y.size(0) * b_k) - 1];
        v2 = y[k + y.size(0) * b_k];
        if (v1 == v2) {
          b_k++;
        } else {
          p = (v1 <= v2);
          exitg1 = true;
        }
      }

      if (p) {
        idx[k - 1] = k;
        idx[k] = k + 1;
      } else {
        idx[k - 1] = k + 1;
        idx[k] = k;
      }
    }

    if ((y.size(0) & 1) != 0) {
      idx[y.size(0) - 1] = y.size(0);
    }

    i = 2;
    while (i < n - 1) {
      i2 = i << 1;
      j = 1;
      for (int pEnd = i + 1; pEnd < n; pEnd = qEnd + i) {
        int b_p;
        int q;
        int kEnd;
        b_p = j;
        q = pEnd;
        qEnd = j + i2;
        if (qEnd > n) {
          qEnd = n;
        }

        k = 0;
        kEnd = qEnd - j;
        while (k + 1 <= kEnd) {
          p = true;
          b_k = 0;
          exitg1 = false;
          while ((!exitg1) && (b_k < 2)) {
            v1 = y[(idx[b_p - 1] + y.size(0) * b_k) - 1];
            v2 = y[(idx[q - 1] + y.size(0) * b_k) - 1];
            if (v1 == v2) {
              b_k++;
            } else {
              p = (v1 <= v2);
              exitg1 = true;
            }
          }

          if (p) {
            iwork[k] = idx[b_p - 1];
            b_p++;
            if (b_p == pEnd) {
              while (q < qEnd) {
                k++;
                iwork[k] = idx[q - 1];
                q++;
              }
            }
          } else {
            iwork[k] = idx[q - 1];
            q++;
            if (q == qEnd) {
              while (b_p < pEnd) {
                k++;
                iwork[k] = idx[b_p - 1];
                b_p++;
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

  i2 = y.size(0) - 1;
  ycol.set_size(y.size(0));
  for (i = 0; i <= i2; i++) {
    ycol[i] = y[idx[i] - 1];
  }

  for (i = 0; i <= i2; i++) {
    y[i] = ycol[i];
  }

  for (i = 0; i <= i2; i++) {
    ycol[i] = y[(idx[i] + y.size(0)) - 1];
  }

  for (i = 0; i <= i2; i++) {
    y[i + y.size(0)] = ycol[i];
  }
}

// End of code generation (sortrows.cpp)
