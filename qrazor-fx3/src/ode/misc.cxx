/*************************************************************************
 *                                                                       *
 * Open Dynamics Engine, Copyright (C) 2001,2002 Russell L. Smith.       *
 * All rights reserved.  Email: russ@q12.org   Web: www.q12.org          *
 *                                                                       *
 * This library is free software; you can redistribute it and/or         *
 * modify it under the terms of EITHER:                                  *
 *   (1) The GNU Lesser General Public License as published by the Free  *
 *       Software Foundation; either version 2.1 of the License, or (at  *
 *       your option) any later version. The text of the GNU Lesser      *
 *       General Public License is included with this library in the     *
 *       file LICENSE.TXT.                                               *
 *   (2) The BSD-style license that is included with this library in     *
 *       the file LICENSE-BSD.TXT.                                       *
 *                                                                       *
 * This library is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files    *
 * LICENSE.TXT and LICENSE-BSD.TXT for more details.                     *
 *                                                                       *
 *************************************************************************/

#include "config.h"
#include "misc.h"
#include "matrix.h"

//****************************************************************************
// random numbers

static unsigned long seed = 0;

unsigned long dRand()
{
  seed = (1664525L*seed + 1013904223L) & 0xffffffff;
  return seed;
}


unsigned long  dRandGetSeed()
{
  return seed;
}


void dRandSetSeed (unsigned long s)
{
  seed = s;
}


int dTestRand()
{
  unsigned long oldseed = seed;
  int ret = 1;
  seed = 0;
  if (dRand() != 0x3c6ef35f || dRand() != 0x47502932 ||
      dRand() != 0xd1ccf6e9 || dRand() != 0xaaf95334 ||
      dRand() != 0x6252e503) ret = 0;
  seed = oldseed;
  return ret;
}


int dRandInt (int n)
{
  double a = double(n) / 4294967296.0;
  return (int) (double(dRand()) * a);
}


vec_t dRanvec_t()
{
  return ((vec_t) dRand()) / ((vec_t) 0xffffffff);
}

//****************************************************************************
// matrix utility stuff

void dPrintMatrix (const vec_t *A, int n, int m, char *fmt, FILE *f)
{
  int i,j;
  int skip = dPAD(m);
  for (i=0; i<n; i++) {
    for (j=0; j<m; j++) fprintf (f,fmt,A[i*skip+j]);
    fprintf (f,"\n");
  }
}


void dMakeRandomVector (vec_t *A, int n, vec_t range)
{
  int i;
  for (i=0; i<n; i++) A[i] = (dRanvec_t()*REAL(2.0)-REAL(1.0))*range;
}


void dMakeRandomMatrix (vec_t *A, int n, int m, vec_t range)
{
  int i,j;
  int skip = dPAD(m);
  dSetZero (A,n*skip);
  for (i=0; i<n; i++) {
    for (j=0; j<m; j++) A[i*skip+j] = (dRanvec_t()*REAL(2.0)-REAL(1.0))*range;
  }
}


void dClearUpperTriangle (vec_t *A, int n)
{
  int i,j;
  int skip = dPAD(n);
  for (i=0; i<n; i++) {
    for (j=i+1; j<n; j++) A[i*skip+j] = 0;
  }
}


vec_t dMaxDifference (const vec_t *A, const vec_t *B, int n, int m)
{
  int i,j;
  int skip = dPAD(m);
  vec_t diff,max;
  max = 0;
  for (i=0; i<n; i++) {
    for (j=0; j<m; j++) {
      diff = X_fabs(A[i*skip+j] - B[i*skip+j]);
      if (diff > max) max = diff;
    }
  }
  return max;
}


vec_t dMaxDifferenceLowerTriangle (const vec_t *A, const vec_t *B, int n)
{
  int i,j;
  int skip = dPAD(n);
  vec_t diff,max;
  max = 0;
  for (i=0; i<n; i++) {
    for (j=0; j<=i; j++) {
      diff = X_fabs(A[i*skip+j] - B[i*skip+j]);
      if (diff > max) max = diff;
    }
  }
  return max;
}
