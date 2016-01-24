#pragma once
/*
   Copyright (C) 2006, 2007 Sony Computer Entertainment Inc.
   All rights reserved.

   Redistribution and use in source and binary forms,
   with or without modification, are permitted provided that the
   following conditions are met:
 * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
 * Neither the name of the Sony Computer Entertainment Inc nor the names
      of its contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _DVECTORMATH_AOS_C_V_SCALAR_H
#define _DVECTORMATH_AOS_C_V_SCALAR_H

#include <math.h>

#ifdef _DVECTORMATH_DEBUG
#include <stdio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef _DVECTORMATH_AOS_C_TYPES_H
#define _DVECTORMATH_AOS_C_TYPES_H

/* A 3-D vector in array-of-structures format
 */
typedef struct _DmathVector3 {
    double x;
    double y;
    double z;
} DmathVector3;

/* A 4-D vector in array-of-structures format
 */
typedef struct _DmathVector4 {
    double x;
    double y;
    double z;
    double w;
} DmathVector4;

/* A 3-D point in array-of-structures format
 */
typedef struct _DmathPoint3 {
    double x;
    double y;
    double z;
} DmathPoint3;

/* A quaternion in array-of-structures format
 */
typedef struct _DmathQuat {
    double x;
    double y;
    double z;
    double w;
} DmathQuat;

/* A 3x3 matrix in array-of-structures format
 */
typedef struct _DmathMatrix3 {
    DmathVector3 col0;
    DmathVector3 col1;
    DmathVector3 col2;
} DmathMatrix3;

/* A 4x4 matrix in array-of-structures format
 */
typedef struct _DmathMatrix4 {
    DmathVector4 col0;
    DmathVector4 col1;
    DmathVector4 col2;
    DmathVector4 col3;
} DmathMatrix4;

/* A 3x4 transformation matrix in array-of-structures format
 */
typedef struct _DmathTransform3 {
    DmathVector3 col0;
    DmathVector3 col1;
    DmathVector3 col2;
    DmathVector3 col3;
} DmathTransform3;

#endif

/*
 * Construct a 3-D vector from x, y, and z elements
 */
static inline DmathVector3 dmathV3MakeFromElems_V(double x, double y, double z);

/*
 * Copy elements from a 3-D point into a 3-D vector
 */
static inline DmathVector3 dmathV3MakeFromP3_V(DmathPoint3 pnt);

/*
 * Set all elements of a 3-D vector to the same scalar value
 */
static inline DmathVector3 dmathV3MakeFromScalar_V(double scalar);

/*
 * Set the x element of a 3-D vector
 */
static inline void dmathV3SetX_V(DmathVector3* result, double x);

/*
 * Set the y element of a 3-D vector
 */
static inline void dmathV3SetY_V(DmathVector3* result, double y);

/*
 * Set the z element of a 3-D vector
 */
static inline void dmathV3SetZ_V(DmathVector3* result, double z);

/*
 * Get the x element of a 3-D vector
 */
static inline double dmathV3GetX_V(DmathVector3 vec);

/*
 * Get the y element of a 3-D vector
 */
static inline double dmathV3GetY_V(DmathVector3 vec);

/*
 * Get the z element of a 3-D vector
 */
static inline double dmathV3GetZ_V(DmathVector3 vec);

/*
 * Set an x, y, or z element of a 3-D vector by index
 */
static inline void dmathV3SetElem_V(DmathVector3* result, int idx, double value);

/*
 * Get an x, y, or z element of a 3-D vector by index
 */
static inline double dmathV3GetElem_V(DmathVector3 vec, int idx);

/*
 * Add two 3-D vectors
 */
static inline DmathVector3 dmathV3Add_V(DmathVector3 vec0, DmathVector3 vec1);

/*
 * Subtract a 3-D vector from another 3-D vector
 */
static inline DmathVector3 dmathV3Sub_V(DmathVector3 vec0, DmathVector3 vec1);

/*
 * Add a 3-D vector to a 3-D point
 */
static inline DmathPoint3 dmathV3AddP3_V(DmathVector3 vec, DmathPoint3 pnt);

/*
 * Multiply a 3-D vector by a scalar
 */
static inline DmathVector3 dmathV3ScalarMul_V(DmathVector3 vec, double scalar);

/*
 * Divide a 3-D vector by a scalar
 */
static inline DmathVector3 dmathV3ScalarDiv_V(DmathVector3 vec, double scalar);

/*
 * Negate all elements of a 3-D vector
 */
static inline DmathVector3 dmathV3Neg_V(DmathVector3 vec);

/*
 * Construct x axis
 */
static inline DmathVector3 dmathV3MakeXAxis_V();

/*
 * Construct y axis
 */
static inline DmathVector3 dmathV3MakeYAxis_V();

/*
 * Construct z axis
 */
static inline DmathVector3 dmathV3MakeZAxis_V();

/*
 * Multiply two 3-D vectors per element
 */
static inline DmathVector3 dmathV3MulPerElem_V(
    DmathVector3 vec0, DmathVector3 vec1);

/*
 * Divide two 3-D vectors per element
 * NOTE:
 * Floating-point behavior matches standard library function divf4.
 */
static inline DmathVector3 dmathV3DivPerElem_V(
    DmathVector3 vec0, DmathVector3 vec1);

/*
 * Compute the reciprocal of a 3-D vector per element
 * NOTE:
 * Floating-point behavior matches standard library function recipf4.
 */
static inline DmathVector3 dmathV3RecipPerElem_V(DmathVector3 vec);

/*
 * Compute the square root of a 3-D vector per element
 * NOTE:
 * Floating-point behavior matches standard library function sqrtf4.
 */
static inline DmathVector3 dmathV3SqrtPerElem_V(DmathVector3 vec);

/*
 * Compute the reciprocal square root of a 3-D vector per element
 * NOTE:
 * Floating-point behavior matches standard library function rsqrtf4.
 */
static inline DmathVector3 dmathV3RsqrtPerElem_V(DmathVector3 vec);

/*
 * Compute the absolute value of a 3-D vector per element
 */
static inline DmathVector3 dmathV3AbsPerElem_V(DmathVector3 vec);

/*
 * Copy sign from one 3-D vector to another, per element
 */
static inline DmathVector3 dmathV3CopySignPerElem_V(
    DmathVector3 vec0, DmathVector3 vec1);

/*
 * Maximum of two 3-D vectors per element
 */
static inline DmathVector3 dmathV3MaxPerElem_V(
    DmathVector3 vec0, DmathVector3 vec1);

/*
 * Minimum of two 3-D vectors per element
 */
static inline DmathVector3 dmathV3MinPerElem_V(
    DmathVector3 vec0, DmathVector3 vec1);

/*
 * Maximum element of a 3-D vector
 */
static inline double dmathV3MaxElem_V(DmathVector3 vec);

/*
 * Minimum element of a 3-D vector
 */
static inline double dmathV3MinElem_V(DmathVector3 vec);

/*
 * Compute the sum of all elements of a 3-D vector
 */
static inline double dmathV3Sum_V(DmathVector3 vec);

/*
 * Compute the dot product of two 3-D vectors
 */
static inline double dmathV3Dot_V(DmathVector3 vec0, DmathVector3 vec1);

/*
 * Compute the square of the length of a 3-D vector
 */
static inline double dmathV3LengthSqr_V(DmathVector3 vec);

/*
 * Compute the length of a 3-D vector
 */
static inline double dmathV3Length_V(DmathVector3 vec);

/*
 * Normalize a 3-D vector
 * NOTE:
 * The result is unpredictable when all elements of vec are at or near zero.
 */
static inline DmathVector3 dmathV3Normalize_V(DmathVector3 vec);

/*
 * Compute cross product of two 3-D vectors
 */
static inline DmathVector3 dmathV3Cross_V(DmathVector3 vec0, DmathVector3 vec1);

/*
 * Outer product of two 3-D vectors
 */
static inline DmathMatrix3 dmathV3Outer_V(DmathVector3 vec0, DmathVector3 vec1);

/*
 * Pre-multiply a row vector by a 3x3 matrix
 */
static inline DmathVector3 dmathV3RowMul_V(DmathVector3 vec, DmathMatrix3 mat);

/*
 * Cross-product matrix of a 3-D vector
 */
static inline DmathMatrix3 dmathV3CrossMatrix_V(DmathVector3 vec);

/*
 * Create cross-product matrix and multiply
 * NOTE:
 * Faster than separately creating a cross-product matrix and multiplying.
 */
static inline DmathMatrix3 dmathV3CrossMatrixMul_V(
    DmathVector3 vec, DmathMatrix3 mat);

/*
 * Linear interpolation between two 3-D vectors
 * NOTE:
 * Does not clamp t between 0 and 1.
 */
static inline DmathVector3 dmathV3Lerp_V(
    double t, DmathVector3 vec0, DmathVector3 vec1);

/*
 * Spherical linear interpolation between two 3-D vectors
 * NOTE:
 * The result is unpredictable if the vectors point in opposite directions.
 * Does not clamp t between 0 and 1.
 */
static inline DmathVector3 dmathV3Slerp_V(
    double t, DmathVector3 unitVec0, DmathVector3 unitVec1);

/*
 * Conditionally select between two 3-D vectors
 */
static inline DmathVector3 dmathV3Select_V(
    DmathVector3 vec0, DmathVector3 vec1, unsigned int select1);

#ifdef _DVECTORMATH_DEBUG

/*
 * Print a 3-D vector
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathV3Print_V(DmathVector3 vec);

/*
 * Print a 3-D vector and an associated string identifier
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathV3Prints_V(DmathVector3 vec, const char* name);

#endif

/*
 * Construct a 4-D vector from x, y, z, and w elements
 */
static inline DmathVector4 dmathV4MakeFromElems_V(
    double x, double y, double z, double w);

/*
 * Construct a 4-D vector from a 3-D vector and a scalar
 */
static inline DmathVector4 dmathV4MakeFromV3Scalar_V(DmathVector3 xyz, double w);

/*
 * Copy x, y, and z from a 3-D vector into a 4-D vector, and set w to 0
 */
static inline DmathVector4 dmathV4MakeFromV3_V(DmathVector3 vec);

/*
 * Copy x, y, and z from a 3-D point into a 4-D vector, and set w to 1
 */
static inline DmathVector4 dmathV4MakeFromP3_V(DmathPoint3 pnt);

/*
 * Copy elements from a quaternion into a 4-D vector
 */
static inline DmathVector4 dmathV4MakeFromQ_V(DmathQuat quat);

/*
 * Set all elements of a 4-D vector to the same scalar value
 */
static inline DmathVector4 dmathV4MakeFromScalar_V(double scalar);

/*
 * Set the x, y, and z elements of a 4-D vector
 * NOTE:
 * This function does not change the w element.
 */
static inline void dmathV4SetXYZ_V(DmathVector4* result, DmathVector3 vec);

/*
 * Get the x, y, and z elements of a 4-D vector
 */
static inline DmathVector3 dmathV4GetXYZ_V(DmathVector4 vec);

/*
 * Set the x element of a 4-D vector
 */
static inline void dmathV4SetX_V(DmathVector4* result, double x);

/*
 * Set the y element of a 4-D vector
 */
static inline void dmathV4SetY_V(DmathVector4* result, double y);

/*
 * Set the z element of a 4-D vector
 */
static inline void dmathV4SetZ_V(DmathVector4* result, double z);

/*
 * Set the w element of a 4-D vector
 */
static inline void dmathV4SetW_V(DmathVector4* result, double w);

/*
 * Get the x element of a 4-D vector
 */
static inline double dmathV4GetX_V(DmathVector4 vec);

/*
 * Get the y element of a 4-D vector
 */
static inline double dmathV4GetY_V(DmathVector4 vec);

/*
 * Get the z element of a 4-D vector
 */
static inline double dmathV4GetZ_V(DmathVector4 vec);

/*
 * Get the w element of a 4-D vector
 */
static inline double dmathV4GetW_V(DmathVector4 vec);

/*
 * Set an x, y, z, or w element of a 4-D vector by index
 */
static inline void dmathV4SetElem_V(DmathVector4* result, int idx, double value);

/*
 * Get an x, y, z, or w element of a 4-D vector by index
 */
static inline double dmathV4GetElem_V(DmathVector4 vec, int idx);

/*
 * Add two 4-D vectors
 */
static inline DmathVector4 dmathV4Add_V(DmathVector4 vec0, DmathVector4 vec1);

/*
 * Subtract a 4-D vector from another 4-D vector
 */
static inline DmathVector4 dmathV4Sub_V(DmathVector4 vec0, DmathVector4 vec1);

/*
 * Multiply a 4-D vector by a scalar
 */
static inline DmathVector4 dmathV4ScalarMul_V(DmathVector4 vec, double scalar);

/*
 * Divide a 4-D vector by a scalar
 */
static inline DmathVector4 dmathV4ScalarDiv_V(DmathVector4 vec, double scalar);

/*
 * Negate all elements of a 4-D vector
 */
static inline DmathVector4 dmathV4Neg_V(DmathVector4 vec);

/*
 * Construct x axis
 */
static inline DmathVector4 dmathV4MakeXAxis_V();

/*
 * Construct y axis
 */
static inline DmathVector4 dmathV4MakeYAxis_V();

/*
 * Construct z axis
 */
static inline DmathVector4 dmathV4MakeZAxis_V();

/*
 * Construct w axis
 */
static inline DmathVector4 dmathV4MakeWAxis_V();

/*
 * Multiply two 4-D vectors per element
 */
static inline DmathVector4 dmathV4MulPerElem_V(
    DmathVector4 vec0, DmathVector4 vec1);

/*
 * Divide two 4-D vectors per element
 * NOTE:
 * Floating-point behavior matches standard library function divf4.
 */
static inline DmathVector4 dmathV4DivPerElem_V(
    DmathVector4 vec0, DmathVector4 vec1);

/*
 * Compute the reciprocal of a 4-D vector per element
 * NOTE:
 * Floating-point behavior matches standard library function recipf4.
 */
static inline DmathVector4 dmathV4RecipPerElem_V(DmathVector4 vec);

/*
 * Compute the square root of a 4-D vector per element
 * NOTE:
 * Floating-point behavior matches standard library function sqrtf4.
 */
static inline DmathVector4 dmathV4SqrtPerElem_V(DmathVector4 vec);

/*
 * Compute the reciprocal square root of a 4-D vector per element
 * NOTE:
 * Floating-point behavior matches standard library function rsqrtf4.
 */
static inline DmathVector4 dmathV4RsqrtPerElem_V(DmathVector4 vec);

/*
 * Compute the absolute value of a 4-D vector per element
 */
static inline DmathVector4 dmathV4AbsPerElem_V(DmathVector4 vec);

/*
 * Copy sign from one 4-D vector to another, per element
 */
static inline DmathVector4 dmathV4CopySignPerElem_V(
    DmathVector4 vec0, DmathVector4 vec1);

/*
 * Maximum of two 4-D vectors per element
 */
static inline DmathVector4 dmathV4MaxPerElem_V(
    DmathVector4 vec0, DmathVector4 vec1);

/*
 * Minimum of two 4-D vectors per element
 */
static inline DmathVector4 dmathV4MinPerElem_V(
    DmathVector4 vec0, DmathVector4 vec1);

/*
 * Maximum element of a 4-D vector
 */
static inline double dmathV4MaxElem_V(DmathVector4 vec);

/*
 * Minimum element of a 4-D vector
 */
static inline double dmathV4MinElem_V(DmathVector4 vec);

/*
 * Compute the sum of all elements of a 4-D vector
 */
static inline double dmathV4Sum_V(DmathVector4 vec);

/*
 * Compute the dot product of two 4-D vectors
 */
static inline double dmathV4Dot_V(DmathVector4 vec0, DmathVector4 vec1);

/*
 * Compute the square of the length of a 4-D vector
 */
static inline double dmathV4LengthSqr_V(DmathVector4 vec);

/*
 * Compute the length of a 4-D vector
 */
static inline double dmathV4Length_V(DmathVector4 vec);

/*
 * Normalize a 4-D vector
 * NOTE:
 * The result is unpredictable when all elements of vec are at or near zero.
 */
static inline DmathVector4 dmathV4Normalize_V(DmathVector4 vec);

/*
 * Outer product of two 4-D vectors
 */
static inline DmathMatrix4 dmathV4Outer_V(DmathVector4 vec0, DmathVector4 vec1);

/*
 * Linear interpolation between two 4-D vectors
 * NOTE:
 * Does not clamp t between 0 and 1.
 */
static inline DmathVector4 dmathV4Lerp_V(
    double t, DmathVector4 vec0, DmathVector4 vec1);

/*
 * Spherical linear interpolation between two 4-D vectors
 * NOTE:
 * The result is unpredictable if the vectors point in opposite directions.
 * Does not clamp t between 0 and 1.
 */
static inline DmathVector4 dmathV4Slerp_V(
    double t, DmathVector4 unitVec0, DmathVector4 unitVec1);

/*
 * Conditionally select between two 4-D vectors
 */
static inline DmathVector4 dmathV4Select_V(
    DmathVector4 vec0, DmathVector4 vec1, unsigned int select1);

#ifdef _DVECTORMATH_DEBUG

/*
 * Print a 4-D vector
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathV4Print_V(DmathVector4 vec);

/*
 * Print a 4-D vector and an associated string identifier
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathV4Prints_V(DmathVector4 vec, const char* name);

#endif

/*
 * Construct a 3-D point from x, y, and z elements
 */
static inline DmathPoint3 dmathP3MakeFromElems_V(double x, double y, double z);

/*
 * Copy elements from a 3-D vector into a 3-D point
 */
static inline DmathPoint3 dmathP3MakeFromV3_V(DmathVector3 vec);

/*
 * Set all elements of a 3-D point to the same scalar value
 */
static inline DmathPoint3 dmathP3MakeFromScalar_V(double scalar);

/*
 * Set the x element of a 3-D point
 */
static inline void dmathP3SetX_V(DmathPoint3* result, double x);

/*
 * Set the y element of a 3-D point
 */
static inline void dmathP3SetY_V(DmathPoint3* result, double y);

/*
 * Set the z element of a 3-D point
 */
static inline void dmathP3SetZ_V(DmathPoint3* result, double z);

/*
 * Get the x element of a 3-D point
 */
static inline double dmathP3GetX_V(DmathPoint3 pnt);

/*
 * Get the y element of a 3-D point
 */
static inline double dmathP3GetY_V(DmathPoint3 pnt);

/*
 * Get the z element of a 3-D point
 */
static inline double dmathP3GetZ_V(DmathPoint3 pnt);

/*
 * Set an x, y, or z element of a 3-D point by index
 */
static inline void dmathP3SetElem_V(DmathPoint3* result, int idx, double value);

/*
 * Get an x, y, or z element of a 3-D point by index
 */
static inline double dmathP3GetElem_V(DmathPoint3 pnt, int idx);

/*
 * Subtract a 3-D point from another 3-D point
 */
static inline DmathVector3 dmathP3Sub_V(DmathPoint3 pnt0, DmathPoint3 pnt1);

/*
 * Add a 3-D point to a 3-D vector
 */
static inline DmathPoint3 dmathP3AddV3_V(DmathPoint3 pnt, DmathVector3 vec);

/*
 * Subtract a 3-D vector from a 3-D point
 */
static inline DmathPoint3 dmathP3SubV3_V(DmathPoint3 pnt, DmathVector3 vec);

/*
 * Multiply two 3-D points per element
 */
static inline DmathPoint3 dmathP3MulPerElem_V(
    DmathPoint3 pnt0, DmathPoint3 pnt1);

/*
 * Divide two 3-D points per element
 * NOTE:
 * Floating-point behavior matches standard library function divf4.
 */
static inline DmathPoint3 dmathP3DivPerElem_V(
    DmathPoint3 pnt0, DmathPoint3 pnt1);

/*
 * Compute the reciprocal of a 3-D point per element
 * NOTE:
 * Floating-point behavior matches standard library function recipf4.
 */
static inline DmathPoint3 dmathP3RecipPerElem_V(DmathPoint3 pnt);

/*
 * Compute the square root of a 3-D point per element
 * NOTE:
 * Floating-point behavior matches standard library function sqrtf4.
 */
static inline DmathPoint3 dmathP3SqrtPerElem_V(DmathPoint3 pnt);

/*
 * Compute the reciprocal square root of a 3-D point per element
 * NOTE:
 * Floating-point behavior matches standard library function rsqrtf4.
 */
static inline DmathPoint3 dmathP3RsqrtPerElem_V(DmathPoint3 pnt);

/*
 * Compute the absolute value of a 3-D point per element
 */
static inline DmathPoint3 dmathP3AbsPerElem_V(DmathPoint3 pnt);

/*
 * Copy sign from one 3-D point to another, per element
 */
static inline DmathPoint3 dmathP3CopySignPerElem_V(
    DmathPoint3 pnt0, DmathPoint3 pnt1);

/*
 * Maximum of two 3-D points per element
 */
static inline DmathPoint3 dmathP3MaxPerElem_V(
    DmathPoint3 pnt0, DmathPoint3 pnt1);

/*
 * Minimum of two 3-D points per element
 */
static inline DmathPoint3 dmathP3MinPerElem_V(
    DmathPoint3 pnt0, DmathPoint3 pnt1);

/*
 * Maximum element of a 3-D point
 */
static inline double dmathP3MaxElem_V(DmathPoint3 pnt);

/*
 * Minimum element of a 3-D point
 */
static inline double dmathP3MinElem_V(DmathPoint3 pnt);

/*
 * Compute the sum of all elements of a 3-D point
 */
static inline double dmathP3Sum_V(DmathPoint3 pnt);

/*
 * Apply uniform scale to a 3-D point
 */
static inline DmathPoint3 dmathP3Scale_V(DmathPoint3 pnt, double scaleVal);

/*
 * Apply non-uniform scale to a 3-D point
 */
static inline DmathPoint3 dmathP3NonUniformScale_V(
    DmathPoint3 pnt, DmathVector3 scaleVec);

/*
 * Scalar projection of a 3-D point on a unit-length 3-D vector
 */
static inline double dmathP3Projection_V(DmathPoint3 pnt, DmathVector3 unitVec);

/*
 * Compute the square of the distance of a 3-D point from the coordinate-system
 * origin
 */
static inline double dmathP3DistSqrFromOrigin_V(DmathPoint3 pnt);

/*
 * Compute the distance of a 3-D point from the coordinate-system origin
 */
static inline double dmathP3DistFromOrigin_V(DmathPoint3 pnt);

/*
 * Compute the square of the distance between two 3-D points
 */
static inline double dmathP3DistSqr_V(DmathPoint3 pnt0, DmathPoint3 pnt1);

/*
 * Compute the distance between two 3-D points
 */
static inline double dmathP3Dist_V(DmathPoint3 pnt0, DmathPoint3 pnt1);

/*
 * Linear interpolation between two 3-D points
 * NOTE:
 * Does not clamp t between 0 and 1.
 */
static inline DmathPoint3 dmathP3Lerp_V(
    double t, DmathPoint3 pnt0, DmathPoint3 pnt1);

/*
 * Conditionally select between two 3-D points
 */
static inline DmathPoint3 dmathP3Select_V(
    DmathPoint3 pnt0, DmathPoint3 pnt1, unsigned int select1);

#ifdef _DVECTORMATH_DEBUG

/*
 * Print a 3-D point
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathP3Print_V(DmathPoint3 pnt);

/*
 * Print a 3-D point and an associated string identifier
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathP3Prints_V(DmathPoint3 pnt, const char* name);

#endif

#ifdef _DVECTORMATH_DEBUG

/*
 * Print a quaternion
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathQPrint_V(DmathQuat quat);

/*
 * Print a quaternion and an associated string identifier
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathQPrints_V(DmathQuat quat, const char* name);

#endif

/*
 * Construct a 3x3 matrix containing the specified columns
 */
static inline DmathMatrix3 dmathM3MakeFromCols_V(
    DmathVector3 col0, DmathVector3 col1, DmathVector3 col2);

/*
 * Construct a 3x3 rotation matrix from a unit-length quaternion
 */
static inline DmathMatrix3 dmathM3MakeFromQ_V(DmathQuat unitQuat);

/*
 * Set all elements of a 3x3 matrix to the same scalar value
 */
static inline DmathMatrix3 dmathM3MakeFromScalar_V(double scalar);

/*
 * Set column 0 of a 3x3 matrix
 */
static inline void dmathM3SetCol0_V(DmathMatrix3* result, DmathVector3 col0);

/*
 * Set column 1 of a 3x3 matrix
 */
static inline void dmathM3SetCol1_V(DmathMatrix3* result, DmathVector3 col1);

/*
 * Set column 2 of a 3x3 matrix
 */
static inline void dmathM3SetCol2_V(DmathMatrix3* result, DmathVector3 col2);

/*
 * Get column 0 of a 3x3 matrix
 */
static inline DmathVector3 dmathM3GetCol0_V(DmathMatrix3 mat);

/*
 * Get column 1 of a 3x3 matrix
 */
static inline DmathVector3 dmathM3GetCol1_V(DmathMatrix3 mat);

/*
 * Get column 2 of a 3x3 matrix
 */
static inline DmathVector3 dmathM3GetCol2_V(DmathMatrix3 mat);

/*
 * Set the column of a 3x3 matrix referred to by the specified index
 */
static inline void dmathM3SetCol_V(
    DmathMatrix3* result, int col, DmathVector3 vec);

/*
 * Set the row of a 3x3 matrix referred to by the specified index
 */
static inline void dmathM3SetRow_V(
    DmathMatrix3* result, int row, DmathVector3 vec);

/*
 * Get the column of a 3x3 matrix referred to by the specified index
 */
static inline DmathVector3 dmathM3GetCol_V(DmathMatrix3 mat, int col);

/*
 * Get the row of a 3x3 matrix referred to by the specified index
 */
static inline DmathVector3 dmathM3GetRow_V(DmathMatrix3 mat, int row);

/*
 * Set the element of a 3x3 matrix referred to by column and row indices
 */
static inline void dmathM3SetElem_V(
    DmathMatrix3* result, int col, int row, double val);

/*
 * Get the element of a 3x3 matrix referred to by column and row indices
 */
static inline double dmathM3GetElem_V(DmathMatrix3 mat, int col, int row);

/*
 * Add two 3x3 matrices
 */
static inline DmathMatrix3 dmathM3Add_V(DmathMatrix3 mat0, DmathMatrix3 mat1);

/*
 * Subtract a 3x3 matrix from another 3x3 matrix
 */
static inline DmathMatrix3 dmathM3Sub_V(DmathMatrix3 mat0, DmathMatrix3 mat1);

/*
 * Negate all elements of a 3x3 matrix
 */
static inline DmathMatrix3 dmathM3Neg_V(DmathMatrix3 mat);

/*
 * Multiply a 3x3 matrix by a scalar
 */
static inline DmathMatrix3 dmathM3ScalarMul_V(DmathMatrix3 mat, double scalar);

/*
 * Multiply a 3x3 matrix by a 3-D vector
 */
static inline DmathVector3 dmathM3MulV3_V(DmathMatrix3 mat, DmathVector3 vec);

/*
 * Multiply two 3x3 matrices
 */
static inline DmathMatrix3 dmathM3Mul_V(DmathMatrix3 mat0, DmathMatrix3 mat1);

/*
 * Construct an identity 3x3 matrix
 */
static inline DmathMatrix3 dmathM3MakeIdentity_V();

/*
 * Construct a 3x3 matrix to rotate around the x axis
 */
static inline DmathMatrix3 dmathM3MakeRotationX_V(double radians);

/*
 * Construct a 3x3 matrix to rotate around the y axis
 */
static inline DmathMatrix3 dmathM3MakeRotationY_V(double radians);

/*
 * Construct a 3x3 matrix to rotate around the z axis
 */
static inline DmathMatrix3 dmathM3MakeRotationZ_V(double radians);

/*
 * Construct a 3x3 matrix to rotate around the x, y, and z axes
 */
static inline DmathMatrix3 dmathM3MakeRotationZYX_V(DmathVector3 radiansXYZ);

/*
 * Construct a 3x3 matrix to rotate around a unit-length 3-D vector
 */
static inline DmathMatrix3 dmathM3MakeRotationAxis_V(
    double radians, DmathVector3 unitVec);

/*
 * Construct a rotation matrix from a unit-length quaternion
 */
static inline DmathMatrix3 dmathM3MakeRotationQ_V(DmathQuat unitQuat);

/*
 * Construct a 3x3 matrix to perform scaling
 */
static inline DmathMatrix3 dmathM3MakeScale_V(DmathVector3 scaleVec);

/*
 * Append (post-multiply) a scale transformation to a 3x3 matrix
 * NOTE:
 * Faster than creating and multiplying a scale transformation matrix.
 */
static inline DmathMatrix3 dmathM3AppendScale_V(
    DmathMatrix3 mat, DmathVector3 scaleVec);

/*
 * Prepend (pre-multiply) a scale transformation to a 3x3 matrix
 * NOTE:
 * Faster than creating and multiplying a scale transformation matrix.
 */
static inline DmathMatrix3 dmathM3PrependScale_V(
    DmathVector3 scaleVec, DmathMatrix3 mat);

/*
 * Multiply two 3x3 matrices per element
 */
static inline DmathMatrix3 dmathM3MulPerElem_V(
    DmathMatrix3 mat0, DmathMatrix3 mat1);

/*
 * Compute the absolute value of a 3x3 matrix per element
 */
static inline DmathMatrix3 dmathM3AbsPerElem_V(DmathMatrix3 mat);

/*
 * Transpose of a 3x3 matrix
 */
static inline DmathMatrix3 dmathM3Transpose_V(DmathMatrix3 mat);

/*
 * Compute the inverse of a 3x3 matrix
 * NOTE:
 * Result is unpredictable when the determinant of mat is equal to or near 0.
 */
static inline DmathMatrix3 dmathM3Inverse_V(DmathMatrix3 mat);

/*
 * Determinant of a 3x3 matrix
 */
static inline double dmathM3Determinant_V(DmathMatrix3 mat);

/*
 * Conditionally select between two 3x3 matrices
 */
static inline DmathMatrix3 dmathM3Select_V(
    DmathMatrix3 mat0, DmathMatrix3 mat1, unsigned int select1);

#ifdef _DVECTORMATH_DEBUG

/*
 * Print a 3x3 matrix
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathM3Print_V(DmathMatrix3 mat);

/*
 * Print a 3x3 matrix and an associated string identifier
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathM3Prints_V(DmathMatrix3 mat, const char* name);

#endif

/*
 * Construct a 4x4 matrix containing the specified columns
 */
static inline DmathMatrix4 dmathM4MakeFromCols_V(
    DmathVector4 col0, DmathVector4 col1, DmathVector4 col2, DmathVector4 col3);

/*
 * Construct a 4x4 matrix from a 3x4 transformation matrix
 */
static inline DmathMatrix4 dmathM4MakeFromT3_V(DmathTransform3 mat);

/*
 * Construct a 4x4 matrix from a 3x3 matrix and a 3-D vector
 */
static inline DmathMatrix4 dmathM4MakeFromM3V3_V(
    DmathMatrix3 mat, DmathVector3 translateVec);

/*
 * Construct a 4x4 matrix from a unit-length quaternion and a 3-D vector
 */
static inline DmathMatrix4 dmathM4MakeFromQV3_V(
    DmathQuat unitQuat, DmathVector3 translateVec);

/*
 * Set all elements of a 4x4 matrix to the same scalar value
 */
static inline DmathMatrix4 dmathM4MakeFromScalar_V(double scalar);

/*
 * Set the upper-left 3x3 submatrix
 * NOTE:
 * This function does not change the bottom row elements.
 */
static inline void dmathM4SetUpper3x3_V(
    DmathMatrix4* result, DmathMatrix3 mat3);

/*
 * Get the upper-left 3x3 submatrix of a 4x4 matrix
 */
static inline DmathMatrix3 dmathM4GetUpper3x3_V(DmathMatrix4 mat);

/*
 * Set translation component
 * NOTE:
 * This function does not change the bottom row elements.
 */
static inline void dmathM4SetTranslation_V(
    DmathMatrix4* result, DmathVector3 translateVec);

/*
 * Get the translation component of a 4x4 matrix
 */
static inline DmathVector3 dmathM4GetTranslation_V(DmathMatrix4 mat);

/*
 * Set column 0 of a 4x4 matrix
 */
static inline void dmathM4SetCol0_V(DmathMatrix4* result, DmathVector4 col0);

/*
 * Set column 1 of a 4x4 matrix
 */
static inline void dmathM4SetCol1_V(DmathMatrix4* result, DmathVector4 col1);

/*
 * Set column 2 of a 4x4 matrix
 */
static inline void dmathM4SetCol2_V(DmathMatrix4* result, DmathVector4 col2);

/*
 * Set column 3 of a 4x4 matrix
 */
static inline void dmathM4SetCol3_V(DmathMatrix4* result, DmathVector4 col3);

/*
 * Get column 0 of a 4x4 matrix
 */
static inline DmathVector4 dmathM4GetCol0_V(DmathMatrix4 mat);

/*
 * Get column 1 of a 4x4 matrix
 */
static inline DmathVector4 dmathM4GetCol1_V(DmathMatrix4 mat);

/*
 * Get column 2 of a 4x4 matrix
 */
static inline DmathVector4 dmathM4GetCol2_V(DmathMatrix4 mat);

/*
 * Get column 3 of a 4x4 matrix
 */
static inline DmathVector4 dmathM4GetCol3_V(DmathMatrix4 mat);

/*
 * Set the column of a 4x4 matrix referred to by the specified index
 */
static inline void dmathM4SetCol_V(
    DmathMatrix4* result, int col, DmathVector4 vec);

/*
 * Set the row of a 4x4 matrix referred to by the specified index
 */
static inline void dmathM4SetRow_V(
    DmathMatrix4* result, int row, DmathVector4 vec);

/*
 * Get the column of a 4x4 matrix referred to by the specified index
 */
static inline DmathVector4 dmathM4GetCol_V(DmathMatrix4 mat, int col);

/*
 * Get the row of a 4x4 matrix referred to by the specified index
 */
static inline DmathVector4 dmathM4GetRow_V(DmathMatrix4 mat, int row);

/*
 * Set the element of a 4x4 matrix referred to by column and row indices
 */
static inline void dmathM4SetElem_V(
    DmathMatrix4* result, int col, int row, double val);

/*
 * Get the element of a 4x4 matrix referred to by column and row indices
 */
static inline double dmathM4GetElem_V(DmathMatrix4 mat, int col, int row);

/*
 * Add two 4x4 matrices
 */
static inline DmathMatrix4 dmathM4Add_V(DmathMatrix4 mat0, DmathMatrix4 mat1);

/*
 * Subtract a 4x4 matrix from another 4x4 matrix
 */
static inline DmathMatrix4 dmathM4Sub_V(DmathMatrix4 mat0, DmathMatrix4 mat1);

/*
 * Negate all elements of a 4x4 matrix
 */
static inline DmathMatrix4 dmathM4Neg_V(DmathMatrix4 mat);

/*
 * Multiply a 4x4 matrix by a scalar
 */
static inline DmathMatrix4 dmathM4ScalarMul_V(DmathMatrix4 mat, double scalar);

/*
 * Multiply a 4x4 matrix by a 4-D vector
 */
static inline DmathVector4 dmathM4MulV4_V(DmathMatrix4 mat, DmathVector4 vec);

/*
 * Multiply a 4x4 matrix by a 3-D vector
 */
static inline DmathVector4 dmathM4MulV3_V(DmathMatrix4 mat, DmathVector3 vec);

/*
 * Multiply a 4x4 matrix by a 3-D point
 */
static inline DmathVector4 dmathM4MulP3_V(DmathMatrix4 mat, DmathPoint3 pnt);

/*
 * Multiply two 4x4 matrices
 */
static inline DmathMatrix4 dmathM4Mul_V(DmathMatrix4 mat0, DmathMatrix4 mat1);

/*
 * Multiply a 4x4 matrix by a 3x4 transformation matrix
 */
static inline DmathMatrix4 dmathM4MulT3_V(
    DmathMatrix4 mat, DmathTransform3 tfrm);

/*
 * Construct an identity 4x4 matrix
 */
static inline DmathMatrix4 dmathM4MakeIdentity_V();

/*
 * Construct a 4x4 matrix to rotate around the x axis
 */
static inline DmathMatrix4 dmathM4MakeRotationX_V(double radians);

/*
 * Construct a 4x4 matrix to rotate around the y axis
 */
static inline DmathMatrix4 dmathM4MakeRotationY_V(double radians);

/*
 * Construct a 4x4 matrix to rotate around the z axis
 */
static inline DmathMatrix4 dmathM4MakeRotationZ_V(double radians);

/*
 * Construct a 4x4 matrix to rotate around the x, y, and z axes
 */
static inline DmathMatrix4 dmathM4MakeRotationZYX_V(DmathVector3 radiansXYZ);

/*
 * Construct a 4x4 matrix to rotate around a unit-length 3-D vector
 */
static inline DmathMatrix4 dmathM4MakeRotationAxis_V(
    double radians, DmathVector3 unitVec);

/*
 * Construct a rotation matrix from a unit-length quaternion
 */
static inline DmathMatrix4 dmathM4MakeRotationQ_V(DmathQuat unitQuat);

/*
 * Construct a 4x4 matrix to perform scaling
 */
static inline DmathMatrix4 dmathM4MakeScale_V(DmathVector3 scaleVec);

/*
 * Construct a 4x4 matrix to perform translation
 */
static inline DmathMatrix4 dmathM4MakeTranslation_V(DmathVector3 translateVec);

/*
 * Construct viewing matrix based on eye position, position looked at, and up
 * direction
 */
static inline DmathMatrix4 dmathM4MakeLookAt_V(
    DmathPoint3 eyePos, DmathPoint3 lookAtPos, DmathVector3 upVec);

/*
 * Construct a perspective projection matrix
 */
static inline DmathMatrix4 dmathM4MakePerspective_V(
    double fovyRadians, double aspect, double zNear, double zFar);

/*
 * Construct a perspective projection matrix based on frustum
 */
static inline DmathMatrix4 dmathM4MakeFrustum_V(
    double left, double right, double bottom, double top, double zNear, double zFar);

/*
 * Construct an orthographic projection matrix
 */
static inline DmathMatrix4 dmathM4MakeOrthographic_V(
    double left, double right, double bottom, double top, double zNear, double zFar);

/*
 * Append (post-multiply) a scale transformation to a 4x4 matrix
 * NOTE:
 * Faster than creating and multiplying a scale transformation matrix.
 */
static inline DmathMatrix4 dmathM4AppendScale_V(
    DmathMatrix4 mat, DmathVector3 scaleVec);

/*
 * Prepend (pre-multiply) a scale transformation to a 4x4 matrix
 * NOTE:
 * Faster than creating and multiplying a scale transformation matrix.
 */
static inline DmathMatrix4 dmathM4PrependScale_V(
    DmathVector3 scaleVec, DmathMatrix4 mat);

/*
 * Multiply two 4x4 matrices per element
 */
static inline DmathMatrix4 dmathM4MulPerElem_V(
    DmathMatrix4 mat0, DmathMatrix4 mat1);

/*
 * Compute the absolute value of a 4x4 matrix per element
 */
static inline DmathMatrix4 dmathM4AbsPerElem_V(DmathMatrix4 mat);

/*
 * Transpose of a 4x4 matrix
 */
static inline DmathMatrix4 dmathM4Transpose_V(DmathMatrix4 mat);

/*
 * Compute the inverse of a 4x4 matrix
 * NOTE:
 * Result is unpredictable when the determinant of mat is equal to or near 0.
 */
static inline DmathMatrix4 dmathM4Inverse_V(DmathMatrix4 mat);

/*
 * Compute the inverse of a 4x4 matrix, which is expected to be an affine matrix
 * NOTE:
 * This can be used to achieve better performance than a general inverse when
 * the specified 4x4 matrix meets the given restrictions.  The result is
 * unpredictable when the determinant of mat is equal to or near 0.
 */
static inline DmathMatrix4 dmathM4AffineInverse_V(DmathMatrix4 mat);

/*
 * Compute the inverse of a 4x4 matrix, which is expected to be an affine matrix
 * with an orthogonal upper-left 3x3 submatrix
 * NOTE:
 * This can be used to achieve better performance than a general inverse when
 * the specified 4x4 matrix meets the given restrictions.
 */
static inline DmathMatrix4 dmathM4OrthoInverse_V(DmathMatrix4 mat);

/*
 * Determinant of a 4x4 matrix
 */
static inline double dmathM4Determinant_V(DmathMatrix4 mat);

/*
 * Conditionally select between two 4x4 matrices
 */
static inline DmathMatrix4 dmathM4Select_V(
    DmathMatrix4 mat0, DmathMatrix4 mat1, unsigned int select1);

#ifdef _DVECTORMATH_DEBUG

/*
 * Print a 4x4 matrix
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathM4Print_V(DmathMatrix4 mat);

/*
 * Print a 4x4 matrix and an associated string identifier
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathM4Prints_V(DmathMatrix4 mat, const char* name);

#endif

/*
 * Construct a 3x4 transformation matrix containing the specified columns
 */
static inline DmathTransform3 dmathT3MakeFromCols_V(
    DmathVector3 col0, DmathVector3 col1, DmathVector3 col2, DmathVector3 col3);

/*
 * Construct a 3x4 transformation matrix from a 3x3 matrix and a 3-D vector
 */
static inline DmathTransform3 dmathT3MakeFromM3V3_V(
    DmathMatrix3 tfrm, DmathVector3 translateVec);

/*
 * Construct a 3x4 transformation matrix from a unit-length quaternion and a 3-D
 * vector
 */
static inline DmathTransform3 dmathT3MakeFromQV3_V(
    DmathQuat unitQuat, DmathVector3 translateVec);

/*
 * Set all elements of a 3x4 transformation matrix to the same scalar value
 */
static inline DmathTransform3 dmathT3MakeFromScalar_V(double scalar);

/*
 * Set the upper-left 3x3 submatrix
 */
static inline void dmathT3SetUpper3x3_V(
    DmathTransform3* result, DmathMatrix3 mat3);

/*
 * Get the upper-left 3x3 submatrix of a 3x4 transformation matrix
 */
static inline DmathMatrix3 dmathT3GetUpper3x3_V(DmathTransform3 tfrm);

/*
 * Set translation component
 */
static inline void dmathT3SetTranslation_V(
    DmathTransform3* result, DmathVector3 translateVec);

/*
 * Get the translation component of a 3x4 transformation matrix
 */
static inline DmathVector3 dmathT3GetTranslation_V(DmathTransform3 tfrm);

/*
 * Set column 0 of a 3x4 transformation matrix
 */
static inline void dmathT3SetCol0_V(DmathTransform3* result, DmathVector3 col0);

/*
 * Set column 1 of a 3x4 transformation matrix
 */
static inline void dmathT3SetCol1_V(DmathTransform3* result, DmathVector3 col1);

/*
 * Set column 2 of a 3x4 transformation matrix
 */
static inline void dmathT3SetCol2_V(DmathTransform3* result, DmathVector3 col2);

/*
 * Set column 3 of a 3x4 transformation matrix
 */
static inline void dmathT3SetCol3_V(DmathTransform3* result, DmathVector3 col3);

/*
 * Get column 0 of a 3x4 transformation matrix
 */
static inline DmathVector3 dmathT3GetCol0_V(DmathTransform3 tfrm);

/*
 * Get column 1 of a 3x4 transformation matrix
 */
static inline DmathVector3 dmathT3GetCol1_V(DmathTransform3 tfrm);

/*
 * Get column 2 of a 3x4 transformation matrix
 */
static inline DmathVector3 dmathT3GetCol2_V(DmathTransform3 tfrm);

/*
 * Get column 3 of a 3x4 transformation matrix
 */
static inline DmathVector3 dmathT3GetCol3_V(DmathTransform3 tfrm);

/*
 * Set the column of a 3x4 transformation matrix referred to by the specified
 * index
 */
static inline void dmathT3SetCol_V(
    DmathTransform3* result, int col, DmathVector3 vec);

/*
 * Set the row of a 3x4 transformation matrix referred to by the specified index
 */
static inline void dmathT3SetRow_V(
    DmathTransform3* result, int row, DmathVector4 vec);

/*
 * Get the column of a 3x4 transformation matrix referred to by the specified
 * index
 */
static inline DmathVector3 dmathT3GetCol_V(DmathTransform3 tfrm, int col);

/*
 * Get the row of a 3x4 transformation matrix referred to by the specified index
 */
static inline DmathVector4 dmathT3GetRow_V(DmathTransform3 tfrm, int row);

/*
 * Set the element of a 3x4 transformation matrix referred to by column and row
 * indices
 */
static inline void dmathT3SetElem_V(
    DmathTransform3* result, int col, int row, double val);

/*
 * Get the element of a 3x4 transformation matrix referred to by column and row
 * indices
 */
static inline double dmathT3GetElem_V(DmathTransform3 tfrm, int col, int row);

/*
 * Multiply a 3x4 transformation matrix by a 3-D vector
 */
static inline DmathVector3 dmathT3MulV3_V(
    DmathTransform3 tfrm, DmathVector3 vec);

/*
 * Multiply a 3x4 transformation matrix by a 3-D point
 */
static inline DmathPoint3 dmathT3MulP3_V(DmathTransform3 tfrm, DmathPoint3 pnt);

/*
 * Multiply two 3x4 transformation matrices
 */
static inline DmathTransform3 dmathT3Mul_V(
    DmathTransform3 tfrm0, DmathTransform3 tfrm1);

/*
 * Construct an identity 3x4 transformation matrix
 */
static inline DmathTransform3 dmathT3MakeIdentity_V();

/*
 * Construct a 3x4 transformation matrix to rotate around the x axis
 */
static inline DmathTransform3 dmathT3MakeRotationX_V(double radians);

/*
 * Construct a 3x4 transformation matrix to rotate around the y axis
 */
static inline DmathTransform3 dmathT3MakeRotationY_V(double radians);

/*
 * Construct a 3x4 transformation matrix to rotate around the z axis
 */
static inline DmathTransform3 dmathT3MakeRotationZ_V(double radians);

/*
 * Construct a 3x4 transformation matrix to rotate around the x, y, and z axes
 */
static inline DmathTransform3 dmathT3MakeRotationZYX_V(DmathVector3 radiansXYZ);

/*
 * Construct a 3x4 transformation matrix to rotate around a unit-length 3-D
 * vector
 */
static inline DmathTransform3 dmathT3MakeRotationAxis_V(
    double radians, DmathVector3 unitVec);

/*
 * Construct a rotation matrix from a unit-length quaternion
 */
static inline DmathTransform3 dmathT3MakeRotationQ_V(DmathQuat unitQuat);

/*
 * Construct a 3x4 transformation matrix to perform scaling
 */
static inline DmathTransform3 dmathT3MakeScale_V(DmathVector3 scaleVec);

/*
 * Construct a 3x4 transformation matrix to perform translation
 */
static inline DmathTransform3 dmathT3MakeTranslation_V(
    DmathVector3 translateVec);

/*
 * Append (post-multiply) a scale transformation to a 3x4 transformation matrix
 * NOTE:
 * Faster than creating and multiplying a scale transformation matrix.
 */
static inline DmathTransform3 dmathT3AppendScale_V(
    DmathTransform3 tfrm, DmathVector3 scaleVec);

/*
 * Prepend (pre-multiply) a scale transformation to a 3x4 transformation matrix
 * NOTE:
 * Faster than creating and multiplying a scale transformation matrix.
 */
static inline DmathTransform3 dmathT3PrependScale_V(
    DmathVector3 scaleVec, DmathTransform3 tfrm);

/*
 * Multiply two 3x4 transformation matrices per element
 */
static inline DmathTransform3 dmathT3MulPerElem_V(
    DmathTransform3 tfrm0, DmathTransform3 tfrm1);

/*
 * Compute the absolute value of a 3x4 transformation matrix per element
 */
static inline DmathTransform3 dmathT3AbsPerElem_V(DmathTransform3 tfrm);

/*
 * Inverse of a 3x4 transformation matrix
 * NOTE:
 * Result is unpredictable when the determinant of the left 3x3 submatrix is
 * equal to or near 0.
 */
static inline DmathTransform3 dmathT3Inverse_V(DmathTransform3 tfrm);

/*
 * Compute the inverse of a 3x4 transformation matrix, expected to have an
 * orthogonal upper-left 3x3 submatrix
 * NOTE:
 * This can be used to achieve better performance than a general inverse when
 * the specified 3x4 transformation matrix meets the given restrictions.
 */
static inline DmathTransform3 dmathT3OrthoInverse_V(DmathTransform3 tfrm);

/*
 * Conditionally select between two 3x4 transformation matrices
 */
static inline DmathTransform3 dmathT3Select_V(
    DmathTransform3 tfrm0, DmathTransform3 tfrm1, unsigned int select1);

#ifdef _DVECTORMATH_DEBUG

/*
 * Print a 3x4 transformation matrix
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathT3Print_V(DmathTransform3 tfrm);

/*
 * Print a 3x4 transformation matrix and an associated string identifier
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathT3Prints_V(DmathTransform3 tfrm, const char* name);

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*
   Copyright (C) 2006, 2007 Sony Computer Entertainment Inc.
   All rights reserved.

   Redistribution and use in source and binary forms,
   with or without modification, are permitted provided that the
   following conditions are met:
 * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
 * Neither the name of the Sony Computer Entertainment Inc nor the names
      of its contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _DVECTORMATH_AOS_C_SCALAR_H
#define _DVECTORMATH_AOS_C_SCALAR_H

#ifdef __cplusplus
extern "C" {
#else

#ifdef _MSC_VER
#define inline __inline
#endif

#endif /* __cplusplus */

#ifndef _DVECTORMATH_AOS_C_TYPES_H
#define _DVECTORMATH_AOS_C_TYPES_H

/* A 3-D vector in array-of-structures format
 */
typedef struct _DmathVector3 {
    double x;
    double y;
    double z;
#ifndef __GNUC__
    double d;
#endif
}
#ifdef __GNUC__
__attribute__((aligned(16)))
#endif
DmathVector3;

/* A 4-D vector in array-of-structures format
 */
typedef struct _DmathVector4 {
    double x;
    double y;
    double z;
    double w;
}
#ifdef __GNUC__
__attribute__((aligned(16)))
#endif
DmathVector4;

/* A 3-D point in array-of-structures format
 */
typedef struct _DmathPoint3 {
    double x;
    double y;
    double z;
#ifndef __GNUC__
    double d;
#endif
}
#ifdef __GNUC__
__attribute__((aligned(16)))
#endif
DmathPoint3;

/* A quaternion in array-of-structures format
 */
typedef struct _DmathQuat {
    double x;
    double y;
    double z;
    double w;
}
#ifdef __GNUC__
__attribute__((aligned(16)))
#endif
DmathQuat;

/* A 3x3 matrix in array-of-structures format
 */
typedef struct _DmathMatrix3 {
    DmathVector3 col0;
    DmathVector3 col1;
    DmathVector3 col2;
} DmathMatrix3;

/* A 4x4 matrix in array-of-structures format
 */
typedef struct _DmathMatrix4 {
    DmathVector4 col0;
    DmathVector4 col1;
    DmathVector4 col2;
    DmathVector4 col3;
} DmathMatrix4;

/* A 3x4 transformation matrix in array-of-structures format
 */
typedef struct _DmathTransform3 {
    DmathVector3 col0;
    DmathVector3 col1;
    DmathVector3 col2;
    DmathVector3 col3;
} DmathTransform3;

#endif

/*
 * Copy a 3-D vector
 */
static inline void dmathV3Copy(DmathVector3* result, const DmathVector3* vec);

/*
 * Construct a 3-D vector from x, y, and z elements
 */
static inline void dmathV3MakeFromElems(
    DmathVector3* result, double x, double y, double z);

/*
 * Copy elements from a 3-D point into a 3-D vector
 */
static inline void dmathV3MakeFromP3(
    DmathVector3* result, const DmathPoint3* pnt);

/*
 * Set all elements of a 3-D vector to the same scalar value
 */
static inline void dmathV3MakeFromScalar(DmathVector3* result, double scalar);

/*
 * Set the x element of a 3-D vector
 */
static inline void dmathV3SetX(DmathVector3* result, double x);

/*
 * Set the y element of a 3-D vector
 */
static inline void dmathV3SetY(DmathVector3* result, double y);

/*
 * Set the z element of a 3-D vector
 */
static inline void dmathV3SetZ(DmathVector3* result, double z);

/*
 * Get the x element of a 3-D vector
 */
static inline double dmathV3GetX(const DmathVector3* vec);

/*
 * Get the y element of a 3-D vector
 */
static inline double dmathV3GetY(const DmathVector3* vec);

/*
 * Get the z element of a 3-D vector
 */
static inline double dmathV3GetZ(const DmathVector3* vec);

/*
 * Set an x, y, or z element of a 3-D vector by index
 */
static inline void dmathV3SetElem(DmathVector3* result, int idx, double value);

/*
 * Get an x, y, or z element of a 3-D vector by index
 */
static inline double dmathV3GetElem(const DmathVector3* vec, int idx);

/*
 * Add two 3-D vectors
 */
static inline void dmathV3Add(
    DmathVector3* result, const DmathVector3* vec0, const DmathVector3* vec1);

/*
 * Subtract a 3-D vector from another 3-D vector
 */
static inline void dmathV3Sub(
    DmathVector3* result, const DmathVector3* vec0, const DmathVector3* vec1);

/*
 * Add a 3-D vector to a 3-D point
 */
static inline void dmathV3AddP3(
    DmathPoint3* result, const DmathVector3* vec, const DmathPoint3* pnt);

/*
 * Multiply a 3-D vector by a scalar
 */
static inline void dmathV3ScalarMul(
    DmathVector3* result, const DmathVector3* vec, double scalar);

/*
 * Divide a 3-D vector by a scalar
 */
static inline void dmathV3ScalarDiv(
    DmathVector3* result, const DmathVector3* vec, double scalar);

/*
 * Negate all elements of a 3-D vector
 */
static inline void dmathV3Neg(DmathVector3* result, const DmathVector3* vec);

/*
 * Construct x axis
 */
static inline void dmathV3MakeXAxis(DmathVector3* result);

/*
 * Construct y axis
 */
static inline void dmathV3MakeYAxis(DmathVector3* result);

/*
 * Construct z axis
 */
static inline void dmathV3MakeZAxis(DmathVector3* result);

/*
 * Multiply two 3-D vectors per element
 */
static inline void dmathV3MulPerElem(
    DmathVector3* result, const DmathVector3* vec0, const DmathVector3* vec1);

/*
 * Divide two 3-D vectors per element
 * NOTE:
 * Floating-point behavior matches standard library function divf4.
 */
static inline void dmathV3DivPerElem(
    DmathVector3* result, const DmathVector3* vec0, const DmathVector3* vec1);

/*
 * Compute the reciprocal of a 3-D vector per element
 * NOTE:
 * Floating-point behavior matches standard library function recipf4.
 */
static inline void dmathV3RecipPerElem(
    DmathVector3* result, const DmathVector3* vec);

/*
 * Compute the square root of a 3-D vector per element
 * NOTE:
 * Floating-point behavior matches standard library function sqrtf4.
 */
static inline void dmathV3SqrtPerElem(
    DmathVector3* result, const DmathVector3* vec);

/*
 * Compute the reciprocal square root of a 3-D vector per element
 * NOTE:
 * Floating-point behavior matches standard library function rsqrtf4.
 */
static inline void dmathV3RsqrtPerElem(
    DmathVector3* result, const DmathVector3* vec);

/*
 * Compute the absolute value of a 3-D vector per element
 */
static inline void dmathV3AbsPerElem(
    DmathVector3* result, const DmathVector3* vec);

/*
 * Copy sign from one 3-D vector to another, per element
 */
static inline void dmathV3CopySignPerElem(
    DmathVector3* result, const DmathVector3* vec0, const DmathVector3* vec1);

/*
 * Maximum of two 3-D vectors per element
 */
static inline void dmathV3MaxPerElem(
    DmathVector3* result, const DmathVector3* vec0, const DmathVector3* vec1);

/*
 * Minimum of two 3-D vectors per element
 */
static inline void dmathV3MinPerElem(
    DmathVector3* result, const DmathVector3* vec0, const DmathVector3* vec1);

/*
 * Maximum element of a 3-D vector
 */
static inline double dmathV3MaxElem(const DmathVector3* vec);

/*
 * Minimum element of a 3-D vector
 */
static inline double dmathV3MinElem(const DmathVector3* vec);

/*
 * Compute the sum of all elements of a 3-D vector
 */
static inline double dmathV3Sum(const DmathVector3* vec);

/*
 * Compute the dot product of two 3-D vectors
 */
static inline double dmathV3Dot(
    const DmathVector3* vec0, const DmathVector3* vec1);

/*
 * Compute the square of the length of a 3-D vector
 */
static inline double dmathV3LengthSqr(const DmathVector3* vec);

/*
 * Compute the length of a 3-D vector
 */
static inline double dmathV3Length(const DmathVector3* vec);

/*
 * Normalize a 3-D vector
 * NOTE:
 * The result is unpredictable when all elements of vec are at or near zero.
 */
static inline void dmathV3Normalize(
    DmathVector3* result, const DmathVector3* vec);

/*
 * Compute cross product of two 3-D vectors
 */
static inline void dmathV3Cross(
    DmathVector3* result, const DmathVector3* vec0, const DmathVector3* vec1);

/*
 * Outer product of two 3-D vectors
 */
static inline void dmathV3Outer(
    DmathMatrix3* result, const DmathVector3* vec0, const DmathVector3* vec1);

/*
 * Pre-multiply a row vector by a 3x3 matrix
 */
static inline void dmathV3RowMul(
    DmathVector3* result, const DmathVector3* vec, const DmathMatrix3* mat);

/*
 * Cross-product matrix of a 3-D vector
 */
static inline void dmathV3CrossMatrix(
    DmathMatrix3* result, const DmathVector3* vec);

/*
 * Create cross-product matrix and multiply
 * NOTE:
 * Faster than separately creating a cross-product matrix and multiplying.
 */
static inline void dmathV3CrossMatrixMul(
    DmathMatrix3* result, const DmathVector3* vec, const DmathMatrix3* mat);

/*
 * Linear interpolation between two 3-D vectors
 * NOTE:
 * Does not clamp t between 0 and 1.
 */
static inline void dmathV3Lerp(DmathVector3* result, double t,
    const DmathVector3* vec0, const DmathVector3* vec1);

/*
 * Spherical linear interpolation between two 3-D vectors
 * NOTE:
 * The result is unpredictable if the vectors point in opposite directions.
 * Does not clamp t between 0 and 1.
 */
static inline void dmathV3Slerp(DmathVector3* result, double t,
    const DmathVector3* unitVec0, const DmathVector3* unitVec1);

/*
 * Conditionally select between two 3-D vectors
 */
static inline void dmathV3Select(DmathVector3* result, const DmathVector3* vec0,
    const DmathVector3* vec1, unsigned int select1);

#ifdef _DVECTORMATH_DEBUG

/*
 * Print a 3-D vector
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathV3Print(const DmathVector3* vec);

/*
 * Print a 3-D vector and an associated string identifier
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathV3Prints(const DmathVector3* vec, const char* name);

#endif

/*
 * Copy a 4-D vector
 */
static inline void dmathV4Copy(DmathVector4* result, const DmathVector4* vec);

/*
 * Construct a 4-D vector from x, y, z, and w elements
 */
static inline void dmathV4MakeFromElems(
    DmathVector4* result, double x, double y, double z, double w);

/*
 * Construct a 4-D vector from a 3-D vector and a scalar
 */
static inline void dmathV4MakeFromV3Scalar(
    DmathVector4* result, const DmathVector3* xyz, double w);

/*
 * Copy x, y, and z from a 3-D vector into a 4-D vector, and set w to 0
 */
static inline void dmathV4MakeFromV3(
    DmathVector4* result, const DmathVector3* vec);

/*
 * Copy x, y, and z from a 3-D point into a 4-D vector, and set w to 1
 */
static inline void dmathV4MakeFromP3(
    DmathVector4* result, const DmathPoint3* pnt);

/*
 * Copy elements from a quaternion into a 4-D vector
 */
static inline void dmathV4MakeFromQ(
    DmathVector4* result, const DmathQuat* quat);

/*
 * Set all elements of a 4-D vector to the same scalar value
 */
static inline void dmathV4MakeFromScalar(DmathVector4* result, double scalar);

/*
 * Set the x, y, and z elements of a 4-D vector
 * NOTE:
 * This function does not change the w element.
 */
static inline void dmathV4SetXYZ(DmathVector4* result, const DmathVector3* vec);

/*
 * Get the x, y, and z elements of a 4-D vector
 */
static inline void dmathV4GetXYZ(DmathVector3* result, const DmathVector4* vec);

/*
 * Set the x element of a 4-D vector
 */
static inline void dmathV4SetX(DmathVector4* result, double x);

/*
 * Set the y element of a 4-D vector
 */
static inline void dmathV4SetY(DmathVector4* result, double y);

/*
 * Set the z element of a 4-D vector
 */
static inline void dmathV4SetZ(DmathVector4* result, double z);

/*
 * Set the w element of a 4-D vector
 */
static inline void dmathV4SetW(DmathVector4* result, double w);

/*
 * Get the x element of a 4-D vector
 */
static inline double dmathV4GetX(const DmathVector4* vec);

/*
 * Get the y element of a 4-D vector
 */
static inline double dmathV4GetY(const DmathVector4* vec);

/*
 * Get the z element of a 4-D vector
 */
static inline double dmathV4GetZ(const DmathVector4* vec);

/*
 * Get the w element of a 4-D vector
 */
static inline double dmathV4GetW(const DmathVector4* vec);

/*
 * Set an x, y, z, or w element of a 4-D vector by index
 */
static inline void dmathV4SetElem(DmathVector4* result, int idx, double value);

/*
 * Get an x, y, z, or w element of a 4-D vector by index
 */
static inline double dmathV4GetElem(const DmathVector4* vec, int idx);

/*
 * Add two 4-D vectors
 */
static inline void dmathV4Add(
    DmathVector4* result, const DmathVector4* vec0, const DmathVector4* vec1);

/*
 * Subtract a 4-D vector from another 4-D vector
 */
static inline void dmathV4Sub(
    DmathVector4* result, const DmathVector4* vec0, const DmathVector4* vec1);

/*
 * Multiply a 4-D vector by a scalar
 */
static inline void dmathV4ScalarMul(
    DmathVector4* result, const DmathVector4* vec, double scalar);

/*
 * Divide a 4-D vector by a scalar
 */
static inline void dmathV4ScalarDiv(
    DmathVector4* result, const DmathVector4* vec, double scalar);

/*
 * Negate all elements of a 4-D vector
 */
static inline void dmathV4Neg(DmathVector4* result, const DmathVector4* vec);

/*
 * Construct x axis
 */
static inline void dmathV4MakeXAxis(DmathVector4* result);

/*
 * Construct y axis
 */
static inline void dmathV4MakeYAxis(DmathVector4* result);

/*
 * Construct z axis
 */
static inline void dmathV4MakeZAxis(DmathVector4* result);

/*
 * Construct w axis
 */
static inline void dmathV4MakeWAxis(DmathVector4* result);

/*
 * Multiply two 4-D vectors per element
 */
static inline void dmathV4MulPerElem(
    DmathVector4* result, const DmathVector4* vec0, const DmathVector4* vec1);

/*
 * Divide two 4-D vectors per element
 * NOTE:
 * Floating-point behavior matches standard library function divf4.
 */
static inline void dmathV4DivPerElem(
    DmathVector4* result, const DmathVector4* vec0, const DmathVector4* vec1);

/*
 * Compute the reciprocal of a 4-D vector per element
 * NOTE:
 * Floating-point behavior matches standard library function recipf4.
 */
static inline void dmathV4RecipPerElem(
    DmathVector4* result, const DmathVector4* vec);

/*
 * Compute the square root of a 4-D vector per element
 * NOTE:
 * Floating-point behavior matches standard library function sqrtf4.
 */
static inline void dmathV4SqrtPerElem(
    DmathVector4* result, const DmathVector4* vec);

/*
 * Compute the reciprocal square root of a 4-D vector per element
 * NOTE:
 * Floating-point behavior matches standard library function rsqrtf4.
 */
static inline void dmathV4RsqrtPerElem(
    DmathVector4* result, const DmathVector4* vec);

/*
 * Compute the absolute value of a 4-D vector per element
 */
static inline void dmathV4AbsPerElem(
    DmathVector4* result, const DmathVector4* vec);

/*
 * Copy sign from one 4-D vector to another, per element
 */
static inline void dmathV4CopySignPerElem(
    DmathVector4* result, const DmathVector4* vec0, const DmathVector4* vec1);

/*
 * Maximum of two 4-D vectors per element
 */
static inline void dmathV4MaxPerElem(
    DmathVector4* result, const DmathVector4* vec0, const DmathVector4* vec1);

/*
 * Minimum of two 4-D vectors per element
 */
static inline void dmathV4MinPerElem(
    DmathVector4* result, const DmathVector4* vec0, const DmathVector4* vec1);

/*
 * Maximum element of a 4-D vector
 */
static inline double dmathV4MaxElem(const DmathVector4* vec);

/*
 * Minimum element of a 4-D vector
 */
static inline double dmathV4MinElem(const DmathVector4* vec);

/*
 * Compute the sum of all elements of a 4-D vector
 */
static inline double dmathV4Sum(const DmathVector4* vec);

/*
 * Compute the dot product of two 4-D vectors
 */
static inline double dmathV4Dot(
    const DmathVector4* vec0, const DmathVector4* vec1);

/*
 * Compute the square of the length of a 4-D vector
 */
static inline double dmathV4LengthSqr(const DmathVector4* vec);

/*
 * Compute the length of a 4-D vector
 */
static inline double dmathV4Length(const DmathVector4* vec);

/*
 * Normalize a 4-D vector
 * NOTE:
 * The result is unpredictable when all elements of vec are at or near zero.
 */
static inline void dmathV4Normalize(
    DmathVector4* result, const DmathVector4* vec);

/*
 * Outer product of two 4-D vectors
 */
static inline void dmathV4Outer(
    DmathMatrix4* result, const DmathVector4* vec0, const DmathVector4* vec1);

/*
 * Linear interpolation between two 4-D vectors
 * NOTE:
 * Does not clamp t between 0 and 1.
 */
static inline void dmathV4Lerp(DmathVector4* result, double t,
    const DmathVector4* vec0, const DmathVector4* vec1);

/*
 * Spherical linear interpolation between two 4-D vectors
 * NOTE:
 * The result is unpredictable if the vectors point in opposite directions.
 * Does not clamp t between 0 and 1.
 */
static inline void dmathV4Slerp(DmathVector4* result, double t,
    const DmathVector4* unitVec0, const DmathVector4* unitVec1);

/*
 * Conditionally select between two 4-D vectors
 */
static inline void dmathV4Select(DmathVector4* result, const DmathVector4* vec0,
    const DmathVector4* vec1, unsigned int select1);

#ifdef _DVECTORMATH_DEBUG

/*
 * Print a 4-D vector
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathV4Print(const DmathVector4* vec);

/*
 * Print a 4-D vector and an associated string identifier
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathV4Prints(const DmathVector4* vec, const char* name);

#endif

/*
 * Copy a 3-D point
 */
static inline void dmathP3Copy(DmathPoint3* result, const DmathPoint3* pnt);

/*
 * Construct a 3-D point from x, y, and z elements
 */
static inline void dmathP3MakeFromElems(
    DmathPoint3* result, double x, double y, double z);

/*
 * Copy elements from a 3-D vector into a 3-D point
 */
static inline void dmathP3MakeFromV3(
    DmathPoint3* result, const DmathVector3* vec);

/*
 * Set all elements of a 3-D point to the same scalar value
 */
static inline void dmathP3MakeFromScalar(DmathPoint3* result, double scalar);

/*
 * Set the x element of a 3-D point
 */
static inline void dmathP3SetX(DmathPoint3* result, double x);

/*
 * Set the y element of a 3-D point
 */
static inline void dmathP3SetY(DmathPoint3* result, double y);

/*
 * Set the z element of a 3-D point
 */
static inline void dmathP3SetZ(DmathPoint3* result, double z);

/*
 * Get the x element of a 3-D point
 */
static inline double dmathP3GetX(const DmathPoint3* pnt);

/*
 * Get the y element of a 3-D point
 */
static inline double dmathP3GetY(const DmathPoint3* pnt);

/*
 * Get the z element of a 3-D point
 */
static inline double dmathP3GetZ(const DmathPoint3* pnt);

/*
 * Set an x, y, or z element of a 3-D point by index
 */
static inline void dmathP3SetElem(DmathPoint3* result, int idx, double value);

/*
 * Get an x, y, or z element of a 3-D point by index
 */
static inline double dmathP3GetElem(const DmathPoint3* pnt, int idx);

/*
 * Subtract a 3-D point from another 3-D point
 */
static inline void dmathP3Sub(
    DmathVector3* result, const DmathPoint3* pnt0, const DmathPoint3* pnt1);

/*
 * Add a 3-D point to a 3-D vector
 */
static inline void dmathP3AddV3(
    DmathPoint3* result, const DmathPoint3* pnt, const DmathVector3* vec);

/*
 * Subtract a 3-D vector from a 3-D point
 */
static inline void dmathP3SubV3(
    DmathPoint3* result, const DmathPoint3* pnt, const DmathVector3* vec);

/*
 * Multiply two 3-D points per element
 */
static inline void dmathP3MulPerElem(
    DmathPoint3* result, const DmathPoint3* pnt0, const DmathPoint3* pnt1);

/*
 * Divide two 3-D points per element
 * NOTE:
 * Floating-point behavior matches standard library function divf4.
 */
static inline void dmathP3DivPerElem(
    DmathPoint3* result, const DmathPoint3* pnt0, const DmathPoint3* pnt1);

/*
 * Compute the reciprocal of a 3-D point per element
 * NOTE:
 * Floating-point behavior matches standard library function recipf4.
 */
static inline void dmathP3RecipPerElem(
    DmathPoint3* result, const DmathPoint3* pnt);

/*
 * Compute the square root of a 3-D point per element
 * NOTE:
 * Floating-point behavior matches standard library function sqrtf4.
 */
static inline void dmathP3SqrtPerElem(
    DmathPoint3* result, const DmathPoint3* pnt);

/*
 * Compute the reciprocal square root of a 3-D point per element
 * NOTE:
 * Floating-point behavior matches standard library function rsqrtf4.
 */
static inline void dmathP3RsqrtPerElem(
    DmathPoint3* result, const DmathPoint3* pnt);

/*
 * Compute the absolute value of a 3-D point per element
 */
static inline void dmathP3AbsPerElem(
    DmathPoint3* result, const DmathPoint3* pnt);

/*
 * Copy sign from one 3-D point to another, per element
 */
static inline void dmathP3CopySignPerElem(
    DmathPoint3* result, const DmathPoint3* pnt0, const DmathPoint3* pnt1);

/*
 * Maximum of two 3-D points per element
 */
static inline void dmathP3MaxPerElem(
    DmathPoint3* result, const DmathPoint3* pnt0, const DmathPoint3* pnt1);

/*
 * Minimum of two 3-D points per element
 */
static inline void dmathP3MinPerElem(
    DmathPoint3* result, const DmathPoint3* pnt0, const DmathPoint3* pnt1);

/*
 * Maximum element of a 3-D point
 */
static inline double dmathP3MaxElem(const DmathPoint3* pnt);

/*
 * Minimum element of a 3-D point
 */
static inline double dmathP3MinElem(const DmathPoint3* pnt);

/*
 * Compute the sum of all elements of a 3-D point
 */
static inline double dmathP3Sum(const DmathPoint3* pnt);

/*
 * Apply uniform scale to a 3-D point
 */
static inline void dmathP3Scale(
    DmathPoint3* result, const DmathPoint3* pnt, double scaleVal);

/*
 * Apply non-uniform scale to a 3-D point
 */
static inline void dmathP3NonUniformScale(
    DmathPoint3* result, const DmathPoint3* pnt, const DmathVector3* scaleVec);

/*
 * Scalar projection of a 3-D point on a unit-length 3-D vector
 */
static inline double dmathP3Projection(
    const DmathPoint3* pnt, const DmathVector3* unitVec);

/*
 * Compute the square of the distance of a 3-D point from the coordinate-system
 * origin
 */
static inline double dmathP3DistSqrFromOrigin(const DmathPoint3* pnt);

/*
 * Compute the distance of a 3-D point from the coordinate-system origin
 */
static inline double dmathP3DistFromOrigin(const DmathPoint3* pnt);

/*
 * Compute the square of the distance between two 3-D points
 */
static inline double dmathP3DistSqr(
    const DmathPoint3* pnt0, const DmathPoint3* pnt1);

/*
 * Compute the distance between two 3-D points
 */
static inline double dmathP3Dist(
    const DmathPoint3* pnt0, const DmathPoint3* pnt1);

/*
 * Linear interpolation between two 3-D points
 * NOTE:
 * Does not clamp t between 0 and 1.
 */
static inline void dmathP3Lerp(DmathPoint3* result, double t,
    const DmathPoint3* pnt0, const DmathPoint3* pnt1);

/*
 * Conditionally select between two 3-D points
 */
static inline void dmathP3Select(DmathPoint3* result, const DmathPoint3* pnt0,
    const DmathPoint3* pnt1, unsigned int select1);

#ifdef _DVECTORMATH_DEBUG

/*
 * Print a 3-D point
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathP3Print(const DmathPoint3* pnt);

/*
 * Print a 3-D point and an associated string identifier
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathP3Prints(const DmathPoint3* pnt, const char* name);

#endif

/*
 * Copy a quaternion
 */
static inline void dmathQCopy(DmathQuat* result, const DmathQuat* quat);

/*
 * Construct a quaternion from x, y, z, and w elements
 */
static inline void dmathQMakeFromElems(
    DmathQuat* result, double x, double y, double z, double w);

/*
 * Construct a quaternion from a 3-D vector and a scalar
 */
static inline void dmathQMakeFromV3Scalar(
    DmathQuat* result, const DmathVector3* xyz, double w);

/*
 * Copy elements from a 4-D vector into a quaternion
 */
static inline void dmathQMakeFromV4(DmathQuat* result, const DmathVector4* vec);

/*
 * Convert a rotation matrix to a unit-length quaternion
 */
static inline void dmathQMakeFromM3(
    DmathQuat* result, const DmathMatrix3* rotMat);

/*
 * Set all elements of a quaternion to the same scalar value
 */
static inline void dmathQMakeFromScalar(DmathQuat* result, double scalar);

/*
 * Set the x, y, and z elements of a quaternion
 * NOTE:
 * This function does not change the w element.
 */
static inline void dmathQSetXYZ(DmathQuat* result, const DmathVector3* vec);

/*
 * Get the x, y, and z elements of a quaternion
 */
static inline void dmathQGetXYZ(DmathVector3* result, const DmathQuat* quat);

/*
 * Set the x element of a quaternion
 */
static inline void dmathQSetX(DmathQuat* result, double x);

/*
 * Set the y element of a quaternion
 */
static inline void dmathQSetY(DmathQuat* result, double y);

/*
 * Set the z element of a quaternion
 */
static inline void dmathQSetZ(DmathQuat* result, double z);

/*
 * Set the w element of a quaternion
 */
static inline void dmathQSetW(DmathQuat* result, double w);

/*
 * Get the x element of a quaternion
 */
static inline double dmathQGetX(const DmathQuat* quat);

/*
 * Get the y element of a quaternion
 */
static inline double dmathQGetY(const DmathQuat* quat);

/*
 * Get the z element of a quaternion
 */
static inline double dmathQGetZ(const DmathQuat* quat);

/*
 * Get the w element of a quaternion
 */
static inline double dmathQGetW(const DmathQuat* quat);

/*
 * Set an x, y, z, or w element of a quaternion by index
 */
static inline void dmathQSetElem(DmathQuat* result, int idx, double value);

/*
 * Get an x, y, z, or w element of a quaternion by index
 */
static inline double dmathQGetElem(const DmathQuat* quat, int idx);

/*
 * Add two quaternions
 */
static inline void dmathQAdd(
    DmathQuat* result, const DmathQuat* quat0, const DmathQuat* quat1);

/*
 * Subtract a quaternion from another quaternion
 */
static inline void dmathQSub(
    DmathQuat* result, const DmathQuat* quat0, const DmathQuat* quat1);

/*
 * Multiply two quaternions
 */
static inline void dmathQMul(
    DmathQuat* result, const DmathQuat* quat0, const DmathQuat* quat1);

/*
 * Multiply a quaternion by a scalar
 */
static inline void dmathQScalarMul(
    DmathQuat* result, const DmathQuat* quat, double scalar);

/*
 * Divide a quaternion by a scalar
 */
static inline void dmathQScalarDiv(
    DmathQuat* result, const DmathQuat* quat, double scalar);

/*
 * Negate all elements of a quaternion
 */
static inline void dmathQNeg(DmathQuat* result, const DmathQuat* quat);

/*
 * Construct an identity quaternion
 */
static inline void dmathQMakeIdentity(DmathQuat* result);

/*
 * Construct a quaternion to rotate between two unit-length 3-D vectors
 * NOTE:
 * The result is unpredictable if unitVec0 and unitVec1 point in opposite
 * directions.
 */
static inline void dmathQMakeRotationArc(DmathQuat* result,
    const DmathVector3* unitVec0, const DmathVector3* unitVec1);

/*
 * Construct a quaternion to rotate around a unit-length 3-D vector
 */
static inline void dmathQMakeRotationAxis(
    DmathQuat* result, double radians, const DmathVector3* unitVec);

/*
 * Construct a quaternion to rotate around the x axis
 */
static inline void dmathQMakeRotationX(DmathQuat* result, double radians);

/*
 * Construct a quaternion to rotate around the y axis
 */
static inline void dmathQMakeRotationY(DmathQuat* result, double radians);

/*
 * Construct a quaternion to rotate around the z axis
 */
static inline void dmathQMakeRotationZ(DmathQuat* result, double radians);

/*
 * Compute the conjugate of a quaternion
 */
static inline void dmathQConj(DmathQuat* result, const DmathQuat* quat);

/*
 * Use a unit-length quaternion to rotate a 3-D vector
 */
static inline void dmathQRotate(
    DmathVector3* result, const DmathQuat* unitQuat, const DmathVector3* vec);

/*
 * Compute the dot product of two quaternions
 */
static inline double dmathQDot(const DmathQuat* quat0, const DmathQuat* quat1);

/*
 * Compute the norm of a quaternion
 */
static inline double dmathQNorm(const DmathQuat* quat);

/*
 * Compute the length of a quaternion
 */
static inline double dmathQLength(const DmathQuat* quat);

/*
 * Normalize a quaternion
 * NOTE:
 * The result is unpredictable when all elements of quat are at or near zero.
 */
static inline void dmathQNormalize(DmathQuat* result, const DmathQuat* quat);

/*
 * Linear interpolation between two quaternions
 * NOTE:
 * Does not clamp t between 0 and 1.
 */
static inline void dmathQLerp(
    DmathQuat* result, double t, const DmathQuat* quat0, const DmathQuat* quat1);

/*
 * Spherical linear interpolation between two quaternions
 * NOTE:
 * Interpolates along the shortest path between orientations.
 * Does not clamp t between 0 and 1.
 */
static inline void dmathQSlerp(DmathQuat* result, double t,
    const DmathQuat* unitQuat0, const DmathQuat* unitQuat1);

/*
 * Spherical quadrangle interpolation
 */
static inline void dmathQSquad(DmathQuat* result, double t,
    const DmathQuat* unitQuat0, const DmathQuat* unitQuat1,
    const DmathQuat* unitQuat2, const DmathQuat* unitQuat3);

/*
 * Conditionally select between two quaternions
 */
static inline void dmathQSelect(DmathQuat* result, const DmathQuat* quat0,
    const DmathQuat* quat1, unsigned int select1);

#ifdef _DVECTORMATH_DEBUG

/*
 * Print a quaternion
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathQPrint(const DmathQuat* quat);

/*
 * Print a quaternion and an associated string identifier
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathQPrints(const DmathQuat* quat, const char* name);

#endif

/*
 * Copy a 3x3 matrix
 */
static inline void dmathM3Copy(DmathMatrix3* result, const DmathMatrix3* mat);

/*
 * Construct a 3x3 matrix containing the specified columns
 */
static inline void dmathM3MakeFromCols(DmathMatrix3* result,
    const DmathVector3* col0, const DmathVector3* col1,
    const DmathVector3* col2);

/*
 * Construct a 3x3 rotation matrix from a unit-length quaternion
 */
static inline void dmathM3MakeFromQ(
    DmathMatrix3* result, const DmathQuat* unitQuat);

/*
 * Set all elements of a 3x3 matrix to the same scalar value
 */
static inline void dmathM3MakeFromScalar(DmathMatrix3* result, double scalar);

/*
 * Set column 0 of a 3x3 matrix
 */
static inline void dmathM3SetCol0(
    DmathMatrix3* result, const DmathVector3* col0);

/*
 * Set column 1 of a 3x3 matrix
 */
static inline void dmathM3SetCol1(
    DmathMatrix3* result, const DmathVector3* col1);

/*
 * Set column 2 of a 3x3 matrix
 */
static inline void dmathM3SetCol2(
    DmathMatrix3* result, const DmathVector3* col2);

/*
 * Get column 0 of a 3x3 matrix
 */
static inline void dmathM3GetCol0(
    DmathVector3* result, const DmathMatrix3* mat);

/*
 * Get column 1 of a 3x3 matrix
 */
static inline void dmathM3GetCol1(
    DmathVector3* result, const DmathMatrix3* mat);

/*
 * Get column 2 of a 3x3 matrix
 */
static inline void dmathM3GetCol2(
    DmathVector3* result, const DmathMatrix3* mat);

/*
 * Set the column of a 3x3 matrix referred to by the specified index
 */
static inline void dmathM3SetCol(
    DmathMatrix3* result, int col, const DmathVector3* vec);

/*
 * Set the row of a 3x3 matrix referred to by the specified index
 */
static inline void dmathM3SetRow(
    DmathMatrix3* result, int row, const DmathVector3* vec);

/*
 * Get the column of a 3x3 matrix referred to by the specified index
 */
static inline void dmathM3GetCol(
    DmathVector3* result, const DmathMatrix3* mat, int col);

/*
 * Get the row of a 3x3 matrix referred to by the specified index
 */
static inline void dmathM3GetRow(
    DmathVector3* result, const DmathMatrix3* mat, int row);

/*
 * Set the element of a 3x3 matrix referred to by column and row indices
 */
static inline void dmathM3SetElem(
    DmathMatrix3* result, int col, int row, double val);

/*
 * Get the element of a 3x3 matrix referred to by column and row indices
 */
static inline double dmathM3GetElem(const DmathMatrix3* mat, int col, int row);

/*
 * Add two 3x3 matrices
 */
static inline void dmathM3Add(
    DmathMatrix3* result, const DmathMatrix3* mat0, const DmathMatrix3* mat1);

/*
 * Subtract a 3x3 matrix from another 3x3 matrix
 */
static inline void dmathM3Sub(
    DmathMatrix3* result, const DmathMatrix3* mat0, const DmathMatrix3* mat1);

/*
 * Negate all elements of a 3x3 matrix
 */
static inline void dmathM3Neg(DmathMatrix3* result, const DmathMatrix3* mat);

/*
 * Multiply a 3x3 matrix by a scalar
 */
static inline void dmathM3ScalarMul(
    DmathMatrix3* result, const DmathMatrix3* mat, double scalar);

/*
 * Multiply a 3x3 matrix by a 3-D vector
 */
static inline void dmathM3MulV3(
    DmathVector3* result, const DmathMatrix3* mat, const DmathVector3* vec);

/*
 * Multiply two 3x3 matrices
 */
static inline void dmathM3Mul(
    DmathMatrix3* result, const DmathMatrix3* mat0, const DmathMatrix3* mat1);

/*
 * Construct an identity 3x3 matrix
 */
static inline void dmathM3MakeIdentity(DmathMatrix3* result);

/*
 * Construct a 3x3 matrix to rotate around the x axis
 */
static inline void dmathM3MakeRotationX(DmathMatrix3* result, double radians);

/*
 * Construct a 3x3 matrix to rotate around the y axis
 */
static inline void dmathM3MakeRotationY(DmathMatrix3* result, double radians);

/*
 * Construct a 3x3 matrix to rotate around the z axis
 */
static inline void dmathM3MakeRotationZ(DmathMatrix3* result, double radians);

/*
 * Construct a 3x3 matrix to rotate around the x, y, and z axes
 */
static inline void dmathM3MakeRotationZYX(
    DmathMatrix3* result, const DmathVector3* radiansXYZ);

/*
 * Construct a 3x3 matrix to rotate around a unit-length 3-D vector
 */
static inline void dmathM3MakeRotationAxis(
    DmathMatrix3* result, double radians, const DmathVector3* unitVec);

/*
 * Construct a rotation matrix from a unit-length quaternion
 */
static inline void dmathM3MakeRotationQ(
    DmathMatrix3* result, const DmathQuat* unitQuat);

/*
 * Construct a 3x3 matrix to perform scaling
 */
static inline void dmathM3MakeScale(
    DmathMatrix3* result, const DmathVector3* scaleVec);

/*
 * Append (post-multiply) a scale transformation to a 3x3 matrix
 * NOTE:
 * Faster than creating and multiplying a scale transformation matrix.
 */
static inline void dmathM3AppendScale(DmathMatrix3* result,
    const DmathMatrix3* mat, const DmathVector3* scaleVec);

/*
 * Prepend (pre-multiply) a scale transformation to a 3x3 matrix
 * NOTE:
 * Faster than creating and multiplying a scale transformation matrix.
 */
static inline void dmathM3PrependScale(DmathMatrix3* result,
    const DmathVector3* scaleVec, const DmathMatrix3* mat);

/*
 * Multiply two 3x3 matrices per element
 */
static inline void dmathM3MulPerElem(
    DmathMatrix3* result, const DmathMatrix3* mat0, const DmathMatrix3* mat1);

/*
 * Compute the absolute value of a 3x3 matrix per element
 */
static inline void dmathM3AbsPerElem(
    DmathMatrix3* result, const DmathMatrix3* mat);

/*
 * Transpose of a 3x3 matrix
 */
static inline void dmathM3Transpose(
    DmathMatrix3* result, const DmathMatrix3* mat);

/*
 * Compute the inverse of a 3x3 matrix
 * NOTE:
 * Result is unpredictable when the determinant of mat is equal to or near 0.
 */
static inline void dmathM3Inverse(
    DmathMatrix3* result, const DmathMatrix3* mat);

/*
 * Determinant of a 3x3 matrix
 */
static inline double dmathM3Determinant(const DmathMatrix3* mat);

/*
 * Conditionally select between two 3x3 matrices
 */
static inline void dmathM3Select(DmathMatrix3* result, const DmathMatrix3* mat0,
    const DmathMatrix3* mat1, unsigned int select1);

#ifdef _DVECTORMATH_DEBUG

/*
 * Print a 3x3 matrix
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathM3Print(const DmathMatrix3* mat);

/*
 * Print a 3x3 matrix and an associated string identifier
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathM3Prints(const DmathMatrix3* mat, const char* name);

#endif

/*
 * Copy a 4x4 matrix
 */
static inline void dmathM4Copy(DmathMatrix4* result, const DmathMatrix4* mat);

/*
 * Construct a 4x4 matrix containing the specified columns
 */
static inline void dmathM4MakeFromCols(DmathMatrix4* result,
    const DmathVector4* col0, const DmathVector4* col1,
    const DmathVector4* col2, const DmathVector4* col3);

/*
 * Construct a 4x4 matrix from a 3x4 transformation matrix
 */
static inline void dmathM4MakeFromT3(
    DmathMatrix4* result, const DmathTransform3* mat);

/*
 * Construct a 4x4 matrix from a 3x3 matrix and a 3-D vector
 */
static inline void dmathM4MakeFromM3V3(DmathMatrix4* result,
    const DmathMatrix3* mat, const DmathVector3* translateVec);

/*
 * Construct a 4x4 matrix from a unit-length quaternion and a 3-D vector
 */
static inline void dmathM4MakeFromQV3(DmathMatrix4* result,
    const DmathQuat* unitQuat, const DmathVector3* translateVec);

/*
 * Set all elements of a 4x4 matrix to the same scalar value
 */
static inline void dmathM4MakeFromScalar(DmathMatrix4* result, double scalar);

/*
 * Set the upper-left 3x3 submatrix
 * NOTE:
 * This function does not change the bottom row elements.
 */
static inline void dmathM4SetUpper3x3(
    DmathMatrix4* result, const DmathMatrix3* mat3);

/*
 * Get the upper-left 3x3 submatrix of a 4x4 matrix
 */
static inline void dmathM4GetUpper3x3(
    DmathMatrix3* result, const DmathMatrix4* mat);

/*
 * Set translation component
 * NOTE:
 * This function does not change the bottom row elements.
 */
static inline void dmathM4SetTranslation(
    DmathMatrix4* result, const DmathVector3* translateVec);

/*
 * Get the translation component of a 4x4 matrix
 */
static inline void dmathM4GetTranslation(
    DmathVector3* result, const DmathMatrix4* mat);

/*
 * Set column 0 of a 4x4 matrix
 */
static inline void dmathM4SetCol0(
    DmathMatrix4* result, const DmathVector4* col0);

/*
 * Set column 1 of a 4x4 matrix
 */
static inline void dmathM4SetCol1(
    DmathMatrix4* result, const DmathVector4* col1);

/*
 * Set column 2 of a 4x4 matrix
 */
static inline void dmathM4SetCol2(
    DmathMatrix4* result, const DmathVector4* col2);

/*
 * Set column 3 of a 4x4 matrix
 */
static inline void dmathM4SetCol3(
    DmathMatrix4* result, const DmathVector4* col3);

/*
 * Get column 0 of a 4x4 matrix
 */
static inline void dmathM4GetCol0(
    DmathVector4* result, const DmathMatrix4* mat);

/*
 * Get column 1 of a 4x4 matrix
 */
static inline void dmathM4GetCol1(
    DmathVector4* result, const DmathMatrix4* mat);

/*
 * Get column 2 of a 4x4 matrix
 */
static inline void dmathM4GetCol2(
    DmathVector4* result, const DmathMatrix4* mat);

/*
 * Get column 3 of a 4x4 matrix
 */
static inline void dmathM4GetCol3(
    DmathVector4* result, const DmathMatrix4* mat);

/*
 * Set the column of a 4x4 matrix referred to by the specified index
 */
static inline void dmathM4SetCol(
    DmathMatrix4* result, int col, const DmathVector4* vec);

/*
 * Set the row of a 4x4 matrix referred to by the specified index
 */
static inline void dmathM4SetRow(
    DmathMatrix4* result, int row, const DmathVector4* vec);

/*
 * Get the column of a 4x4 matrix referred to by the specified index
 */
static inline void dmathM4GetCol(
    DmathVector4* result, const DmathMatrix4* mat, int col);

/*
 * Get the row of a 4x4 matrix referred to by the specified index
 */
static inline void dmathM4GetRow(
    DmathVector4* result, const DmathMatrix4* mat, int row);

/*
 * Set the element of a 4x4 matrix referred to by column and row indices
 */
static inline void dmathM4SetElem(
    DmathMatrix4* result, int col, int row, double val);

/*
 * Get the element of a 4x4 matrix referred to by column and row indices
 */
static inline double dmathM4GetElem(const DmathMatrix4* mat, int col, int row);

/*
 * Add two 4x4 matrices
 */
static inline void dmathM4Add(
    DmathMatrix4* result, const DmathMatrix4* mat0, const DmathMatrix4* mat1);

/*
 * Subtract a 4x4 matrix from another 4x4 matrix
 */
static inline void dmathM4Sub(
    DmathMatrix4* result, const DmathMatrix4* mat0, const DmathMatrix4* mat1);

/*
 * Negate all elements of a 4x4 matrix
 */
static inline void dmathM4Neg(DmathMatrix4* result, const DmathMatrix4* mat);

/*
 * Multiply a 4x4 matrix by a scalar
 */
static inline void dmathM4ScalarMul(
    DmathMatrix4* result, const DmathMatrix4* mat, double scalar);

/*
 * Multiply a 4x4 matrix by a 4-D vector
 */
static inline void dmathM4MulV4(
    DmathVector4* result, const DmathMatrix4* mat, const DmathVector4* vec);

/*
 * Multiply a 4x4 matrix by a 3-D vector
 */
static inline void dmathM4MulV3(
    DmathVector4* result, const DmathMatrix4* mat, const DmathVector3* vec);

/*
 * Multiply a 4x4 matrix by a 3-D point
 */
static inline void dmathM4MulP3(
    DmathVector4* result, const DmathMatrix4* mat, const DmathPoint3* pnt);

/*
 * Multiply two 4x4 matrices
 */
static inline void dmathM4Mul(
    DmathMatrix4* result, const DmathMatrix4* mat0, const DmathMatrix4* mat1);

/*
 * Multiply a 4x4 matrix by a 3x4 transformation matrix
 */
static inline void dmathM4MulT3(
    DmathMatrix4* result, const DmathMatrix4* mat, const DmathTransform3* tfrm);

/*
 * Construct an identity 4x4 matrix
 */
static inline void dmathM4MakeIdentity(DmathMatrix4* result);

/*
 * Construct a 4x4 matrix to rotate around the x axis
 */
static inline void dmathM4MakeRotationX(DmathMatrix4* result, double radians);

/*
 * Construct a 4x4 matrix to rotate around the y axis
 */
static inline void dmathM4MakeRotationY(DmathMatrix4* result, double radians);

/*
 * Construct a 4x4 matrix to rotate around the z axis
 */
static inline void dmathM4MakeRotationZ(DmathMatrix4* result, double radians);

/*
 * Construct a 4x4 matrix to rotate around the x, y, and z axes
 */
static inline void dmathM4MakeRotationZYX(
    DmathMatrix4* result, const DmathVector3* radiansXYZ);

/*
 * Construct a 4x4 matrix to rotate around a unit-length 3-D vector
 */
static inline void dmathM4MakeRotationAxis(
    DmathMatrix4* result, double radians, const DmathVector3* unitVec);

/*
 * Construct a rotation matrix from a unit-length quaternion
 */
static inline void dmathM4MakeRotationQ(
    DmathMatrix4* result, const DmathQuat* unitQuat);

/*
 * Construct a 4x4 matrix to perform scaling
 */
static inline void dmathM4MakeScale(
    DmathMatrix4* result, const DmathVector3* scaleVec);

/*
 * Construct a 4x4 matrix to perform translation
 */
static inline void dmathM4MakeTranslation(
    DmathMatrix4* result, const DmathVector3* translateVec);

/*
 * Construct viewing matrix based on eye position, position looked at, and up
 * direction
 */
static inline void dmathM4MakeLookAt(DmathMatrix4* result,
    const DmathPoint3* eyePos, const DmathPoint3* lookAtPos,
    const DmathVector3* upVec);

/*
 * Construct a perspective projection matrix
 */
static inline void dmathM4MakePerspective(DmathMatrix4* result,
    double fovyRadians, double aspect, double zNear, double zFar);

/*
 * Construct a perspective projection matrix based on frustum
 */
static inline void dmathM4MakeFrustum(DmathMatrix4* result, double left,
    double right, double bottom, double top, double zNear, double zFar);

/*
 * Construct an orthographic projection matrix
 */
static inline void dmathM4MakeOrthographic(DmathMatrix4* result, double left,
    double right, double bottom, double top, double zNear, double zFar);

/*
 * Append (post-multiply) a scale transformation to a 4x4 matrix
 * NOTE:
 * Faster than creating and multiplying a scale transformation matrix.
 */
static inline void dmathM4AppendScale(DmathMatrix4* result,
    const DmathMatrix4* mat, const DmathVector3* scaleVec);

/*
 * Prepend (pre-multiply) a scale transformation to a 4x4 matrix
 * NOTE:
 * Faster than creating and multiplying a scale transformation matrix.
 */
static inline void dmathM4PrependScale(DmathMatrix4* result,
    const DmathVector3* scaleVec, const DmathMatrix4* mat);

/*
 * Multiply two 4x4 matrices per element
 */
static inline void dmathM4MulPerElem(
    DmathMatrix4* result, const DmathMatrix4* mat0, const DmathMatrix4* mat1);

/*
 * Compute the absolute value of a 4x4 matrix per element
 */
static inline void dmathM4AbsPerElem(
    DmathMatrix4* result, const DmathMatrix4* mat);

/*
 * Transpose of a 4x4 matrix
 */
static inline void dmathM4Transpose(
    DmathMatrix4* result, const DmathMatrix4* mat);

/*
 * Compute the inverse of a 4x4 matrix
 * NOTE:
 * Result is unpredictable when the determinant of mat is equal to or near 0.
 */
static inline void dmathM4Inverse(
    DmathMatrix4* result, const DmathMatrix4* mat);

/*
 * Compute the inverse of a 4x4 matrix, which is expected to be an affine matrix
 * NOTE:
 * This can be used to achieve better performance than a general inverse when
 * the specified 4x4 matrix meets the given restrictions.  The result is
 * unpredictable when the determinant of mat is equal to or near 0.
 */
static inline void dmathM4AffineInverse(
    DmathMatrix4* result, const DmathMatrix4* mat);

/*
 * Compute the inverse of a 4x4 matrix, which is expected to be an affine matrix
 * with an orthogonal upper-left 3x3 submatrix
 * NOTE:
 * This can be used to achieve better performance than a general inverse when
 * the specified 4x4 matrix meets the given restrictions.
 */
static inline void dmathM4OrthoInverse(
    DmathMatrix4* result, const DmathMatrix4* mat);

/*
 * Determinant of a 4x4 matrix
 */
static inline double dmathM4Determinant(const DmathMatrix4* mat);

/*
 * Conditionally select between two 4x4 matrices
 */
static inline void dmathM4Select(DmathMatrix4* result, const DmathMatrix4* mat0,
    const DmathMatrix4* mat1, unsigned int select1);

#ifdef _DVECTORMATH_DEBUG

/*
 * Print a 4x4 matrix
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathM4Print(const DmathMatrix4* mat);

/*
 * Print a 4x4 matrix and an associated string identifier
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathM4Prints(const DmathMatrix4* mat, const char* name);

#endif

/*
 * Copy a 3x4 transformation matrix
 */
static inline void dmathT3Copy(
    DmathTransform3* result, const DmathTransform3* tfrm);

/*
 * Construct a 3x4 transformation matrix containing the specified columns
 */
static inline void dmathT3MakeFromCols(DmathTransform3* result,
    const DmathVector3* col0, const DmathVector3* col1,
    const DmathVector3* col2, const DmathVector3* col3);

/*
 * Construct a 3x4 transformation matrix from a 3x3 matrix and a 3-D vector
 */
static inline void dmathT3MakeFromM3V3(DmathTransform3* result,
    const DmathMatrix3* tfrm, const DmathVector3* translateVec);

/*
 * Construct a 3x4 transformation matrix from a unit-length quaternion and a 3-D
 * vector
 */
static inline void dmathT3MakeFromQV3(DmathTransform3* result,
    const DmathQuat* unitQuat, const DmathVector3* translateVec);

/*
 * Set all elements of a 3x4 transformation matrix to the same scalar value
 */
static inline void dmathT3MakeFromScalar(DmathTransform3* result, double scalar);

/*
 * Set the upper-left 3x3 submatrix
 */
static inline void dmathT3SetUpper3x3(
    DmathTransform3* result, const DmathMatrix3* mat3);

/*
 * Get the upper-left 3x3 submatrix of a 3x4 transformation matrix
 */
static inline void dmathT3GetUpper3x3(
    DmathMatrix3* result, const DmathTransform3* tfrm);

/*
 * Set translation component
 */
static inline void dmathT3SetTranslation(
    DmathTransform3* result, const DmathVector3* translateVec);

/*
 * Get the translation component of a 3x4 transformation matrix
 */
static inline void dmathT3GetTranslation(
    DmathVector3* result, const DmathTransform3* tfrm);

/*
 * Set column 0 of a 3x4 transformation matrix
 */
static inline void dmathT3SetCol0(
    DmathTransform3* result, const DmathVector3* col0);

/*
 * Set column 1 of a 3x4 transformation matrix
 */
static inline void dmathT3SetCol1(
    DmathTransform3* result, const DmathVector3* col1);

/*
 * Set column 2 of a 3x4 transformation matrix
 */
static inline void dmathT3SetCol2(
    DmathTransform3* result, const DmathVector3* col2);

/*
 * Set column 3 of a 3x4 transformation matrix
 */
static inline void dmathT3SetCol3(
    DmathTransform3* result, const DmathVector3* col3);

/*
 * Get column 0 of a 3x4 transformation matrix
 */
static inline void dmathT3GetCol0(
    DmathVector3* result, const DmathTransform3* tfrm);

/*
 * Get column 1 of a 3x4 transformation matrix
 */
static inline void dmathT3GetCol1(
    DmathVector3* result, const DmathTransform3* tfrm);

/*
 * Get column 2 of a 3x4 transformation matrix
 */
static inline void dmathT3GetCol2(
    DmathVector3* result, const DmathTransform3* tfrm);

/*
 * Get column 3 of a 3x4 transformation matrix
 */
static inline void dmathT3GetCol3(
    DmathVector3* result, const DmathTransform3* tfrm);

/*
 * Set the column of a 3x4 transformation matrix referred to by the specified
 * index
 */
static inline void dmathT3SetCol(
    DmathTransform3* result, int col, const DmathVector3* vec);

/*
 * Set the row of a 3x4 transformation matrix referred to by the specified index
 */
static inline void dmathT3SetRow(
    DmathTransform3* result, int row, const DmathVector4* vec);

/*
 * Get the column of a 3x4 transformation matrix referred to by the specified
 * index
 */
static inline void dmathT3GetCol(
    DmathVector3* result, const DmathTransform3* tfrm, int col);

/*
 * Get the row of a 3x4 transformation matrix referred to by the specified index
 */
static inline void dmathT3GetRow(
    DmathVector4* result, const DmathTransform3* tfrm, int row);

/*
 * Set the element of a 3x4 transformation matrix referred to by column and row
 * indices
 */
static inline void dmathT3SetElem(
    DmathTransform3* result, int col, int row, double val);

/*
 * Get the element of a 3x4 transformation matrix referred to by column and row
 * indices
 */
static inline double dmathT3GetElem(
    const DmathTransform3* tfrm, int col, int row);

/*
 * Multiply a 3x4 transformation matrix by a 3-D vector
 */
static inline void dmathT3MulV3(
    DmathVector3* result, const DmathTransform3* tfrm, const DmathVector3* vec);

/*
 * Multiply a 3x4 transformation matrix by a 3-D point
 */
static inline void dmathT3MulP3(
    DmathPoint3* result, const DmathTransform3* tfrm, const DmathPoint3* pnt);

/*
 * Multiply two 3x4 transformation matrices
 */
static inline void dmathT3Mul(DmathTransform3* result,
    const DmathTransform3* tfrm0, const DmathTransform3* tfrm1);

/*
 * Construct an identity 3x4 transformation matrix
 */
static inline void dmathT3MakeIdentity(DmathTransform3* result);

/*
 * Construct a 3x4 transformation matrix to rotate around the x axis
 */
static inline void dmathT3MakeRotationX(DmathTransform3* result, double radians);

/*
 * Construct a 3x4 transformation matrix to rotate around the y axis
 */
static inline void dmathT3MakeRotationY(DmathTransform3* result, double radians);

/*
 * Construct a 3x4 transformation matrix to rotate around the z axis
 */
static inline void dmathT3MakeRotationZ(DmathTransform3* result, double radians);

/*
 * Construct a 3x4 transformation matrix to rotate around the x, y, and z axes
 */
static inline void dmathT3MakeRotationZYX(
    DmathTransform3* result, const DmathVector3* radiansXYZ);

/*
 * Construct a 3x4 transformation matrix to rotate around a unit-length 3-D
 * vector
 */
static inline void dmathT3MakeRotationAxis(
    DmathTransform3* result, double radians, const DmathVector3* unitVec);

/*
 * Construct a rotation matrix from a unit-length quaternion
 */
static inline void dmathT3MakeRotationQ(
    DmathTransform3* result, const DmathQuat* unitQuat);

/*
 * Construct a 3x4 transformation matrix to perform scaling
 */
static inline void dmathT3MakeScale(
    DmathTransform3* result, const DmathVector3* scaleVec);

/*
 * Construct a 3x4 transformation matrix to perform translation
 */
static inline void dmathT3MakeTranslation(
    DmathTransform3* result, const DmathVector3* translateVec);

/*
 * Append (post-multiply) a scale transformation to a 3x4 transformation matrix
 * NOTE:
 * Faster than creating and multiplying a scale transformation matrix.
 */
static inline void dmathT3AppendScale(DmathTransform3* result,
    const DmathTransform3* tfrm, const DmathVector3* scaleVec);

/*
 * Prepend (pre-multiply) a scale transformation to a 3x4 transformation matrix
 * NOTE:
 * Faster than creating and multiplying a scale transformation matrix.
 */
static inline void dmathT3PrependScale(DmathTransform3* result,
    const DmathVector3* scaleVec, const DmathTransform3* tfrm);

/*
 * Multiply two 3x4 transformation matrices per element
 */
static inline void dmathT3MulPerElem(DmathTransform3* result,
    const DmathTransform3* tfrm0, const DmathTransform3* tfrm1);

/*
 * Compute the absolute value of a 3x4 transformation matrix per element
 */
static inline void dmathT3AbsPerElem(
    DmathTransform3* result, const DmathTransform3* tfrm);

/*
 * Inverse of a 3x4 transformation matrix
 * NOTE:
 * Result is unpredictable when the determinant of the left 3x3 submatrix is
 * equal to or near 0.
 */
static inline void dmathT3Inverse(
    DmathTransform3* result, const DmathTransform3* tfrm);

/*
 * Compute the inverse of a 3x4 transformation matrix, expected to have an
 * orthogonal upper-left 3x3 submatrix
 * NOTE:
 * This can be used to achieve better performance than a general inverse when
 * the specified 3x4 transformation matrix meets the given restrictions.
 */
static inline void dmathT3OrthoInverse(
    DmathTransform3* result, const DmathTransform3* tfrm);

/*
 * Conditionally select between two 3x4 transformation matrices
 */
static inline void dmathT3Select(DmathTransform3* result,
    const DmathTransform3* tfrm0, const DmathTransform3* tfrm1,
    unsigned int select1);

#ifdef _DVECTORMATH_DEBUG

/*
 * Print a 3x4 transformation matrix
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathT3Print(const DmathTransform3* tfrm);

/*
 * Print a 3x4 transformation matrix and an associated string identifier
 * NOTE:
 * Function is only defined when _DVECTORMATH_DEBUG is defined.
 */
static inline void dmathT3Prints(const DmathTransform3* tfrm, const char* name);

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*
   Copyright (C) 2006, 2007 Sony Computer Entertainment Inc.
   All rights reserved.

   Redistribution and use in source and binary forms,
   with or without modification, are permitted provided that the
   following conditions are met:
 * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
 * Neither the name of the Sony Computer Entertainment Inc nor the names
      of its contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _DVECTORMATH_VEC_AOS_C_H
#define _DVECTORMATH_VEC_AOS_C_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*-----------------------------------------------------------------------------
 * Constants
 */
#define _DVECTORMATH_SLERP_TOL 0.999f

/*-----------------------------------------------------------------------------
 * Definitions
 */
#ifndef _DVECTORMATH_INTERNAL_FUNCTIONS
#define _DVECTORMATH_INTERNAL_FUNCTIONS

#endif

static inline void dmathV3Copy(DmathVector3* result, const DmathVector3* vec)
{
    result->x = vec->x;
    result->y = vec->y;
    result->z = vec->z;
}

static inline void dmathV3MakeFromElems(
    DmathVector3* result, double _x, double _y, double _z)
{
    result->x = _x;
    result->y = _y;
    result->z = _z;
}

static inline void dmathV3MakeFromP3(
    DmathVector3* result, const DmathPoint3* pnt)
{
    result->x = pnt->x;
    result->y = pnt->y;
    result->z = pnt->z;
}

static inline void dmathV3MakeFromScalar(DmathVector3* result, double scalar)
{
    result->x = scalar;
    result->y = scalar;
    result->z = scalar;
}

static inline void dmathV3MakeXAxis(DmathVector3* result)
{
    dmathV3MakeFromElems(result, 1.0f, 0.0f, 0.0f);
}

static inline void dmathV3MakeYAxis(DmathVector3* result)
{
    dmathV3MakeFromElems(result, 0.0f, 1.0f, 0.0f);
}

static inline void dmathV3MakeZAxis(DmathVector3* result)
{
    dmathV3MakeFromElems(result, 0.0f, 0.0f, 1.0f);
}

static inline void dmathV3Lerp(DmathVector3* result, double t,
    const DmathVector3* vec0, const DmathVector3* vec1)
{
    DmathVector3 tmpV3_0, tmpV3_1;
    dmathV3Sub(&tmpV3_0, vec1, vec0);
    dmathV3ScalarMul(&tmpV3_1, &tmpV3_0, t);
    dmathV3Add(result, vec0, &tmpV3_1);
}

static inline void dmathV3Slerp(DmathVector3* result, double t,
    const DmathVector3* unitVec0, const DmathVector3* unitVec1)
{
    DmathVector3 tmpV3_0, tmpV3_1;
    double recipSinAngle, scale0, scale1, cosAngle, angle;
    cosAngle = dmathV3Dot(unitVec0, unitVec1);
    if (cosAngle < _DVECTORMATH_SLERP_TOL) {
        angle = acosf(cosAngle);
        recipSinAngle = (1.0f / sinf(angle));
        scale0 = (sinf(((1.0f - t) * angle)) * recipSinAngle);
        scale1 = (sinf((t * angle)) * recipSinAngle);
    } else {
        scale0 = (1.0f - t);
        scale1 = t;
    }
    dmathV3ScalarMul(&tmpV3_0, unitVec0, scale0);
    dmathV3ScalarMul(&tmpV3_1, unitVec1, scale1);
    dmathV3Add(result, &tmpV3_0, &tmpV3_1);
}

static inline void dmathV3SetX(DmathVector3* result, double _x)
{
    result->x = _x;
}

static inline double dmathV3GetX(const DmathVector3* vec) { return vec->x; }

static inline void dmathV3SetY(DmathVector3* result, double _y)
{
    result->y = _y;
}

static inline double dmathV3GetY(const DmathVector3* vec) { return vec->y; }

static inline void dmathV3SetZ(DmathVector3* result, double _z)
{
    result->z = _z;
}

static inline double dmathV3GetZ(const DmathVector3* vec) { return vec->z; }

static inline void dmathV3SetElem(DmathVector3* result, int idx, double value)
{
    *(&result->x + idx) = value;
}

static inline double dmathV3GetElem(const DmathVector3* vec, int idx)
{
    return *(&vec->x + idx);
}

static inline void dmathV3Add(
    DmathVector3* result, const DmathVector3* vec0, const DmathVector3* vec1)
{
    result->x = (vec0->x + vec1->x);
    result->y = (vec0->y + vec1->y);
    result->z = (vec0->z + vec1->z);
}

static inline void dmathV3Sub(
    DmathVector3* result, const DmathVector3* vec0, const DmathVector3* vec1)
{
    result->x = (vec0->x - vec1->x);
    result->y = (vec0->y - vec1->y);
    result->z = (vec0->z - vec1->z);
}

static inline void dmathV3AddP3(
    DmathPoint3* result, const DmathVector3* vec, const DmathPoint3* pnt1)
{
    result->x = (vec->x + pnt1->x);
    result->y = (vec->y + pnt1->y);
    result->z = (vec->z + pnt1->z);
}

static inline void dmathV3ScalarMul(
    DmathVector3* result, const DmathVector3* vec, double scalar)
{
    result->x = (vec->x * scalar);
    result->y = (vec->y * scalar);
    result->z = (vec->z * scalar);
}

static inline void dmathV3ScalarDiv(
    DmathVector3* result, const DmathVector3* vec, double scalar)
{
    result->x = (vec->x / scalar);
    result->y = (vec->y / scalar);
    result->z = (vec->z / scalar);
}

static inline void dmathV3Neg(DmathVector3* result, const DmathVector3* vec)
{
    result->x = -vec->x;
    result->y = -vec->y;
    result->z = -vec->z;
}

static inline void dmathV3MulPerElem(
    DmathVector3* result, const DmathVector3* vec0, const DmathVector3* vec1)
{
    result->x = (vec0->x * vec1->x);
    result->y = (vec0->y * vec1->y);
    result->z = (vec0->z * vec1->z);
}

static inline void dmathV3DivPerElem(
    DmathVector3* result, const DmathVector3* vec0, const DmathVector3* vec1)
{
    result->x = (vec0->x / vec1->x);
    result->y = (vec0->y / vec1->y);
    result->z = (vec0->z / vec1->z);
}

static inline void dmathV3RecipPerElem(
    DmathVector3* result, const DmathVector3* vec)
{
    result->x = (1.0f / vec->x);
    result->y = (1.0f / vec->y);
    result->z = (1.0f / vec->z);
}

static inline void dmathV3SqrtPerElem(
    DmathVector3* result, const DmathVector3* vec)
{
    result->x = sqrtf(vec->x);
    result->y = sqrtf(vec->y);
    result->z = sqrtf(vec->z);
}

static inline void dmathV3RsqrtPerElem(
    DmathVector3* result, const DmathVector3* vec)
{
    result->x = (1.0f / sqrtf(vec->x));
    result->y = (1.0f / sqrtf(vec->y));
    result->z = (1.0f / sqrtf(vec->z));
}

static inline void dmathV3AbsPerElem(
    DmathVector3* result, const DmathVector3* vec)
{
    result->x = fabs(vec->x);
    result->y = fabs(vec->y);
    result->z = fabs(vec->z);
}

static inline void dmathV3CopySignPerElem(
    DmathVector3* result, const DmathVector3* vec0, const DmathVector3* vec1)
{
    result->x = (vec1->x < 0.0f) ? -fabs(vec0->x) : fabs(vec0->x);
    result->y = (vec1->y < 0.0f) ? -fabs(vec0->y) : fabs(vec0->y);
    result->z = (vec1->z < 0.0f) ? -fabs(vec0->z) : fabs(vec0->z);
}

static inline void dmathV3MaxPerElem(
    DmathVector3* result, const DmathVector3* vec0, const DmathVector3* vec1)
{
    result->x = (vec0->x > vec1->x) ? vec0->x : vec1->x;
    result->y = (vec0->y > vec1->y) ? vec0->y : vec1->y;
    result->z = (vec0->z > vec1->z) ? vec0->z : vec1->z;
}

static inline double dmathV3MaxElem(const DmathVector3* vec)
{
    double result;
    result = (vec->x > vec->y) ? vec->x : vec->y;
    result = (vec->z > result) ? vec->z : result;
    return result;
}

static inline void dmathV3MinPerElem(
    DmathVector3* result, const DmathVector3* vec0, const DmathVector3* vec1)
{
    result->x = (vec0->x < vec1->x) ? vec0->x : vec1->x;
    result->y = (vec0->y < vec1->y) ? vec0->y : vec1->y;
    result->z = (vec0->z < vec1->z) ? vec0->z : vec1->z;
}

static inline double dmathV3MinElem(const DmathVector3* vec)
{
    double result;
    result = (vec->x < vec->y) ? vec->x : vec->y;
    result = (vec->z < result) ? vec->z : result;
    return result;
}

static inline double dmathV3Sum(const DmathVector3* vec)
{
    double result;
    result = (vec->x + vec->y);
    result = (result + vec->z);
    return result;
}

static inline double dmathV3Dot(
    const DmathVector3* vec0, const DmathVector3* vec1)
{
    double result;
    result = (vec0->x * vec1->x);
    result = (result + (vec0->y * vec1->y));
    result = (result + (vec0->z * vec1->z));
    return result;
}

static inline double dmathV3LengthSqr(const DmathVector3* vec)
{
    double result;
    result = (vec->x * vec->x);
    result = (result + (vec->y * vec->y));
    result = (result + (vec->z * vec->z));
    return result;
}

static inline double dmathV3Length(const DmathVector3* vec)
{
    return sqrtf(dmathV3LengthSqr(vec));
}

static inline void dmathV3Normalize(
    DmathVector3* result, const DmathVector3* vec)
{
    double lenSqr, lenInv;
    lenSqr = dmathV3LengthSqr(vec);
    lenInv = (1.0f / sqrtf(lenSqr));
    result->x = (vec->x * lenInv);
    result->y = (vec->y * lenInv);
    result->z = (vec->z * lenInv);
}

static inline void dmathV3Cross(
    DmathVector3* result, const DmathVector3* vec0, const DmathVector3* vec1)
{
    double tmpX, tmpY, tmpZ;
    tmpX = ((vec0->y * vec1->z) - (vec0->z * vec1->y));
    tmpY = ((vec0->z * vec1->x) - (vec0->x * vec1->z));
    tmpZ = ((vec0->x * vec1->y) - (vec0->y * vec1->x));
    dmathV3MakeFromElems(result, tmpX, tmpY, tmpZ);
}

static inline void dmathV3Select(DmathVector3* result, const DmathVector3* vec0,
    const DmathVector3* vec1, unsigned int select1)
{
    result->x = (select1) ? vec1->x : vec0->x;
    result->y = (select1) ? vec1->y : vec0->y;
    result->z = (select1) ? vec1->z : vec0->z;
}

#ifdef _DVECTORMATH_DEBUG

static inline void dmathV3Print(const DmathVector3* vec)
{
    printf("( %f %f %f )\n", vec->x, vec->y, vec->z);
}

static inline void dmathV3Prints(const DmathVector3* vec, const char* name)
{
    printf("%s: ( %f %f %f )\n", name, vec->x, vec->y, vec->z);
}

#endif

static inline void dmathV4Copy(DmathVector4* result, const DmathVector4* vec)
{
    result->x = vec->x;
    result->y = vec->y;
    result->z = vec->z;
    result->w = vec->w;
}

static inline void dmathV4MakeFromElems(
    DmathVector4* result, double _x, double _y, double _z, double _w)
{
    result->x = _x;
    result->y = _y;
    result->z = _z;
    result->w = _w;
}

static inline void dmathV4MakeFromV3Scalar(
    DmathVector4* result, const DmathVector3* xyz, double _w)
{
    dmathV4SetXYZ(result, xyz);
    dmathV4SetW(result, _w);
}

static inline void dmathV4MakeFromV3(
    DmathVector4* result, const DmathVector3* vec)
{
    result->x = vec->x;
    result->y = vec->y;
    result->z = vec->z;
    result->w = 0.0f;
}

static inline void dmathV4MakeFromP3(
    DmathVector4* result, const DmathPoint3* pnt)
{
    result->x = pnt->x;
    result->y = pnt->y;
    result->z = pnt->z;
    result->w = 1.0f;
}

static inline void dmathV4MakeFromQ(DmathVector4* result, const DmathQuat* quat)
{
    result->x = quat->x;
    result->y = quat->y;
    result->z = quat->z;
    result->w = quat->w;
}

static inline void dmathV4MakeFromScalar(DmathVector4* result, double scalar)
{
    result->x = scalar;
    result->y = scalar;
    result->z = scalar;
    result->w = scalar;
}

static inline void dmathV4MakeXAxis(DmathVector4* result)
{
    dmathV4MakeFromElems(result, 1.0f, 0.0f, 0.0f, 0.0f);
}

static inline void dmathV4MakeYAxis(DmathVector4* result)
{
    dmathV4MakeFromElems(result, 0.0f, 1.0f, 0.0f, 0.0f);
}

static inline void dmathV4MakeZAxis(DmathVector4* result)
{
    dmathV4MakeFromElems(result, 0.0f, 0.0f, 1.0f, 0.0f);
}

static inline void dmathV4MakeWAxis(DmathVector4* result)
{
    dmathV4MakeFromElems(result, 0.0f, 0.0f, 0.0f, 1.0f);
}

static inline void dmathV4Lerp(DmathVector4* result, double t,
    const DmathVector4* vec0, const DmathVector4* vec1)
{
    DmathVector4 tmpV4_0, tmpV4_1;
    dmathV4Sub(&tmpV4_0, vec1, vec0);
    dmathV4ScalarMul(&tmpV4_1, &tmpV4_0, t);
    dmathV4Add(result, vec0, &tmpV4_1);
}

static inline void dmathV4Slerp(DmathVector4* result, double t,
    const DmathVector4* unitVec0, const DmathVector4* unitVec1)
{
    DmathVector4 tmpV4_0, tmpV4_1;
    double recipSinAngle, scale0, scale1, cosAngle, angle;
    cosAngle = dmathV4Dot(unitVec0, unitVec1);
    if (cosAngle < _DVECTORMATH_SLERP_TOL) {
        angle = acosf(cosAngle);
        recipSinAngle = (1.0f / sinf(angle));
        scale0 = (sinf(((1.0f - t) * angle)) * recipSinAngle);
        scale1 = (sinf((t * angle)) * recipSinAngle);
    } else {
        scale0 = (1.0f - t);
        scale1 = t;
    }
    dmathV4ScalarMul(&tmpV4_0, unitVec0, scale0);
    dmathV4ScalarMul(&tmpV4_1, unitVec1, scale1);
    dmathV4Add(result, &tmpV4_0, &tmpV4_1);
}

static inline void dmathV4SetXYZ(DmathVector4* result, const DmathVector3* vec)
{
    result->x = vec->x;
    result->y = vec->y;
    result->z = vec->z;
}

static inline void dmathV4GetXYZ(DmathVector3* result, const DmathVector4* vec)
{
    dmathV3MakeFromElems(result, vec->x, vec->y, vec->z);
}

static inline void dmathV4SetX(DmathVector4* result, double _x)
{
    result->x = _x;
}

static inline double dmathV4GetX(const DmathVector4* vec) { return vec->x; }

static inline void dmathV4SetY(DmathVector4* result, double _y)
{
    result->y = _y;
}

static inline double dmathV4GetY(const DmathVector4* vec) { return vec->y; }

static inline void dmathV4SetZ(DmathVector4* result, double _z)
{
    result->z = _z;
}

static inline double dmathV4GetZ(const DmathVector4* vec) { return vec->z; }

static inline void dmathV4SetW(DmathVector4* result, double _w)
{
    result->w = _w;
}

static inline double dmathV4GetW(const DmathVector4* vec) { return vec->w; }

static inline void dmathV4SetElem(DmathVector4* result, int idx, double value)
{
    *(&result->x + idx) = value;
}

static inline double dmathV4GetElem(const DmathVector4* vec, int idx)
{
    return *(&vec->x + idx);
}

static inline void dmathV4Add(
    DmathVector4* result, const DmathVector4* vec0, const DmathVector4* vec1)
{
    result->x = (vec0->x + vec1->x);
    result->y = (vec0->y + vec1->y);
    result->z = (vec0->z + vec1->z);
    result->w = (vec0->w + vec1->w);
}

static inline void dmathV4Sub(
    DmathVector4* result, const DmathVector4* vec0, const DmathVector4* vec1)
{
    result->x = (vec0->x - vec1->x);
    result->y = (vec0->y - vec1->y);
    result->z = (vec0->z - vec1->z);
    result->w = (vec0->w - vec1->w);
}

static inline void dmathV4ScalarMul(
    DmathVector4* result, const DmathVector4* vec, double scalar)
{
    result->x = (vec->x * scalar);
    result->y = (vec->y * scalar);
    result->z = (vec->z * scalar);
    result->w = (vec->w * scalar);
}

static inline void dmathV4ScalarDiv(
    DmathVector4* result, const DmathVector4* vec, double scalar)
{
    result->x = (vec->x / scalar);
    result->y = (vec->y / scalar);
    result->z = (vec->z / scalar);
    result->w = (vec->w / scalar);
}

static inline void dmathV4Neg(DmathVector4* result, const DmathVector4* vec)
{
    result->x = -vec->x;
    result->y = -vec->y;
    result->z = -vec->z;
    result->w = -vec->w;
}

static inline void dmathV4MulPerElem(
    DmathVector4* result, const DmathVector4* vec0, const DmathVector4* vec1)
{
    result->x = (vec0->x * vec1->x);
    result->y = (vec0->y * vec1->y);
    result->z = (vec0->z * vec1->z);
    result->w = (vec0->w * vec1->w);
}

static inline void dmathV4DivPerElem(
    DmathVector4* result, const DmathVector4* vec0, const DmathVector4* vec1)
{
    result->x = (vec0->x / vec1->x);
    result->y = (vec0->y / vec1->y);
    result->z = (vec0->z / vec1->z);
    result->w = (vec0->w / vec1->w);
}

static inline void dmathV4RecipPerElem(
    DmathVector4* result, const DmathVector4* vec)
{
    result->x = (1.0f / vec->x);
    result->y = (1.0f / vec->y);
    result->z = (1.0f / vec->z);
    result->w = (1.0f / vec->w);
}

static inline void dmathV4SqrtPerElem(
    DmathVector4* result, const DmathVector4* vec)
{
    result->x = sqrtf(vec->x);
    result->y = sqrtf(vec->y);
    result->z = sqrtf(vec->z);
    result->w = sqrtf(vec->w);
}

static inline void dmathV4RsqrtPerElem(
    DmathVector4* result, const DmathVector4* vec)
{
    result->x = (1.0f / sqrtf(vec->x));
    result->y = (1.0f / sqrtf(vec->y));
    result->z = (1.0f / sqrtf(vec->z));
    result->w = (1.0f / sqrtf(vec->w));
}

static inline void dmathV4AbsPerElem(
    DmathVector4* result, const DmathVector4* vec)
{
    result->x = fabs(vec->x);
    result->y = fabs(vec->y);
    result->z = fabs(vec->z);
    result->w = fabs(vec->w);
}

static inline void dmathV4CopySignPerElem(
    DmathVector4* result, const DmathVector4* vec0, const DmathVector4* vec1)
{
    result->x = (vec1->x < 0.0f) ? -fabs(vec0->x) : fabs(vec0->x);
    result->y = (vec1->y < 0.0f) ? -fabs(vec0->y) : fabs(vec0->y);
    result->z = (vec1->z < 0.0f) ? -fabs(vec0->z) : fabs(vec0->z);
    result->w = (vec1->w < 0.0f) ? -fabs(vec0->w) : fabs(vec0->w);
}

static inline void dmathV4MaxPerElem(
    DmathVector4* result, const DmathVector4* vec0, const DmathVector4* vec1)
{
    result->x = (vec0->x > vec1->x) ? vec0->x : vec1->x;
    result->y = (vec0->y > vec1->y) ? vec0->y : vec1->y;
    result->z = (vec0->z > vec1->z) ? vec0->z : vec1->z;
    result->w = (vec0->w > vec1->w) ? vec0->w : vec1->w;
}

static inline double dmathV4MaxElem(const DmathVector4* vec)
{
    double result;
    result = (vec->x > vec->y) ? vec->x : vec->y;
    result = (vec->z > result) ? vec->z : result;
    result = (vec->w > result) ? vec->w : result;
    return result;
}

static inline void dmathV4MinPerElem(
    DmathVector4* result, const DmathVector4* vec0, const DmathVector4* vec1)
{
    result->x = (vec0->x < vec1->x) ? vec0->x : vec1->x;
    result->y = (vec0->y < vec1->y) ? vec0->y : vec1->y;
    result->z = (vec0->z < vec1->z) ? vec0->z : vec1->z;
    result->w = (vec0->w < vec1->w) ? vec0->w : vec1->w;
}

static inline double dmathV4MinElem(const DmathVector4* vec)
{
    double result;
    result = (vec->x < vec->y) ? vec->x : vec->y;
    result = (vec->z < result) ? vec->z : result;
    result = (vec->w < result) ? vec->w : result;
    return result;
}

static inline double dmathV4Sum(const DmathVector4* vec)
{
    double result;
    result = (vec->x + vec->y);
    result = (result + vec->z);
    result = (result + vec->w);
    return result;
}

static inline double dmathV4Dot(
    const DmathVector4* vec0, const DmathVector4* vec1)
{
    double result;
    result = (vec0->x * vec1->x);
    result = (result + (vec0->y * vec1->y));
    result = (result + (vec0->z * vec1->z));
    result = (result + (vec0->w * vec1->w));
    return result;
}

static inline double dmathV4LengthSqr(const DmathVector4* vec)
{
    double result;
    result = (vec->x * vec->x);
    result = (result + (vec->y * vec->y));
    result = (result + (vec->z * vec->z));
    result = (result + (vec->w * vec->w));
    return result;
}

static inline double dmathV4Length(const DmathVector4* vec)
{
    return sqrtf(dmathV4LengthSqr(vec));
}

static inline void dmathV4Normalize(
    DmathVector4* result, const DmathVector4* vec)
{
    double lenSqr, lenInv;
    lenSqr = dmathV4LengthSqr(vec);
    lenInv = (1.0f / sqrtf(lenSqr));
    result->x = (vec->x * lenInv);
    result->y = (vec->y * lenInv);
    result->z = (vec->z * lenInv);
    result->w = (vec->w * lenInv);
}

static inline void dmathV4Select(DmathVector4* result, const DmathVector4* vec0,
    const DmathVector4* vec1, unsigned int select1)
{
    result->x = (select1) ? vec1->x : vec0->x;
    result->y = (select1) ? vec1->y : vec0->y;
    result->z = (select1) ? vec1->z : vec0->z;
    result->w = (select1) ? vec1->w : vec0->w;
}

#ifdef _DVECTORMATH_DEBUG

static inline void dmathV4Print(const DmathVector4* vec)
{
    printf("( %f %f %f %f )\n", vec->x, vec->y, vec->z, vec->w);
}

static inline void dmathV4Prints(const DmathVector4* vec, const char* name)
{
    printf("%s: ( %f %f %f %f )\n", name, vec->x, vec->y, vec->z, vec->w);
}

#endif

static inline void dmathP3Copy(DmathPoint3* result, const DmathPoint3* pnt)
{
    result->x = pnt->x;
    result->y = pnt->y;
    result->z = pnt->z;
}

static inline void dmathP3MakeFromElems(
    DmathPoint3* result, double _x, double _y, double _z)
{
    result->x = _x;
    result->y = _y;
    result->z = _z;
}

static inline void dmathP3MakeFromV3(
    DmathPoint3* result, const DmathVector3* vec)
{
    result->x = vec->x;
    result->y = vec->y;
    result->z = vec->z;
}

static inline void dmathP3MakeFromScalar(DmathPoint3* result, double scalar)
{
    result->x = scalar;
    result->y = scalar;
    result->z = scalar;
}

static inline void dmathP3Lerp(DmathPoint3* result, double t,
    const DmathPoint3* pnt0, const DmathPoint3* pnt1)
{
    DmathVector3 tmpV3_0, tmpV3_1;
    dmathP3Sub(&tmpV3_0, pnt1, pnt0);
    dmathV3ScalarMul(&tmpV3_1, &tmpV3_0, t);
    dmathP3AddV3(result, pnt0, &tmpV3_1);
}

static inline void dmathP3SetX(DmathPoint3* result, double _x)
{
    result->x = _x;
}

static inline double dmathP3GetX(const DmathPoint3* pnt) { return pnt->x; }

static inline void dmathP3SetY(DmathPoint3* result, double _y)
{
    result->y = _y;
}

static inline double dmathP3GetY(const DmathPoint3* pnt) { return pnt->y; }

static inline void dmathP3SetZ(DmathPoint3* result, double _z)
{
    result->z = _z;
}

static inline double dmathP3GetZ(const DmathPoint3* pnt) { return pnt->z; }

static inline void dmathP3SetElem(DmathPoint3* result, int idx, double value)
{
    *(&result->x + idx) = value;
}

static inline double dmathP3GetElem(const DmathPoint3* pnt, int idx)
{
    return *(&pnt->x + idx);
}

static inline void dmathP3Sub(
    DmathVector3* result, const DmathPoint3* pnt0, const DmathPoint3* pnt1)
{
    result->x = (pnt0->x - pnt1->x);
    result->y = (pnt0->y - pnt1->y);
    result->z = (pnt0->z - pnt1->z);
}

static inline void dmathP3AddV3(
    DmathPoint3* result, const DmathPoint3* pnt, const DmathVector3* vec1)
{
    result->x = (pnt->x + vec1->x);
    result->y = (pnt->y + vec1->y);
    result->z = (pnt->z + vec1->z);
}

static inline void dmathP3SubV3(
    DmathPoint3* result, const DmathPoint3* pnt, const DmathVector3* vec1)
{
    result->x = (pnt->x - vec1->x);
    result->y = (pnt->y - vec1->y);
    result->z = (pnt->z - vec1->z);
}

static inline void dmathP3MulPerElem(
    DmathPoint3* result, const DmathPoint3* pnt0, const DmathPoint3* pnt1)
{
    result->x = (pnt0->x * pnt1->x);
    result->y = (pnt0->y * pnt1->y);
    result->z = (pnt0->z * pnt1->z);
}

static inline void dmathP3DivPerElem(
    DmathPoint3* result, const DmathPoint3* pnt0, const DmathPoint3* pnt1)
{
    result->x = (pnt0->x / pnt1->x);
    result->y = (pnt0->y / pnt1->y);
    result->z = (pnt0->z / pnt1->z);
}

static inline void dmathP3RecipPerElem(
    DmathPoint3* result, const DmathPoint3* pnt)
{
    result->x = (1.0f / pnt->x);
    result->y = (1.0f / pnt->y);
    result->z = (1.0f / pnt->z);
}

static inline void dmathP3SqrtPerElem(
    DmathPoint3* result, const DmathPoint3* pnt)
{
    result->x = sqrtf(pnt->x);
    result->y = sqrtf(pnt->y);
    result->z = sqrtf(pnt->z);
}

static inline void dmathP3RsqrtPerElem(
    DmathPoint3* result, const DmathPoint3* pnt)
{
    result->x = (1.0f / sqrtf(pnt->x));
    result->y = (1.0f / sqrtf(pnt->y));
    result->z = (1.0f / sqrtf(pnt->z));
}

static inline void dmathP3AbsPerElem(
    DmathPoint3* result, const DmathPoint3* pnt)
{
    result->x = fabs(pnt->x);
    result->y = fabs(pnt->y);
    result->z = fabs(pnt->z);
}

static inline void dmathP3CopySignPerElem(
    DmathPoint3* result, const DmathPoint3* pnt0, const DmathPoint3* pnt1)
{
    result->x = (pnt1->x < 0.0f) ? -fabs(pnt0->x) : fabs(pnt0->x);
    result->y = (pnt1->y < 0.0f) ? -fabs(pnt0->y) : fabs(pnt0->y);
    result->z = (pnt1->z < 0.0f) ? -fabs(pnt0->z) : fabs(pnt0->z);
}

static inline void dmathP3MaxPerElem(
    DmathPoint3* result, const DmathPoint3* pnt0, const DmathPoint3* pnt1)
{
    result->x = (pnt0->x > pnt1->x) ? pnt0->x : pnt1->x;
    result->y = (pnt0->y > pnt1->y) ? pnt0->y : pnt1->y;
    result->z = (pnt0->z > pnt1->z) ? pnt0->z : pnt1->z;
}

static inline double dmathP3MaxElem(const DmathPoint3* pnt)
{
    double result;
    result = (pnt->x > pnt->y) ? pnt->x : pnt->y;
    result = (pnt->z > result) ? pnt->z : result;
    return result;
}

static inline void dmathP3MinPerElem(
    DmathPoint3* result, const DmathPoint3* pnt0, const DmathPoint3* pnt1)
{
    result->x = (pnt0->x < pnt1->x) ? pnt0->x : pnt1->x;
    result->y = (pnt0->y < pnt1->y) ? pnt0->y : pnt1->y;
    result->z = (pnt0->z < pnt1->z) ? pnt0->z : pnt1->z;
}

static inline double dmathP3MinElem(const DmathPoint3* pnt)
{
    double result;
    result = (pnt->x < pnt->y) ? pnt->x : pnt->y;
    result = (pnt->z < result) ? pnt->z : result;
    return result;
}

static inline double dmathP3Sum(const DmathPoint3* pnt)
{
    double result;
    result = (pnt->x + pnt->y);
    result = (result + pnt->z);
    return result;
}

static inline void dmathP3Scale(
    DmathPoint3* result, const DmathPoint3* pnt, double scaleVal)
{
    DmathPoint3 tmpP3_0;
    dmathP3MakeFromScalar(&tmpP3_0, scaleVal);
    dmathP3MulPerElem(result, pnt, &tmpP3_0);
}

static inline void dmathP3NonUniformScale(
    DmathPoint3* result, const DmathPoint3* pnt, const DmathVector3* scaleVec)
{
    DmathPoint3 tmpP3_0;
    dmathP3MakeFromV3(&tmpP3_0, scaleVec);
    dmathP3MulPerElem(result, pnt, &tmpP3_0);
}

static inline double dmathP3Projection(
    const DmathPoint3* pnt, const DmathVector3* unitVec)
{
    double result;
    result = (pnt->x * unitVec->x);
    result = (result + (pnt->y * unitVec->y));
    result = (result + (pnt->z * unitVec->z));
    return result;
}

static inline double dmathP3DistSqrFromOrigin(const DmathPoint3* pnt)
{
    DmathVector3 tmpV3_0;
    dmathV3MakeFromP3(&tmpV3_0, pnt);
    return dmathV3LengthSqr(&tmpV3_0);
}

static inline double dmathP3DistFromOrigin(const DmathPoint3* pnt)
{
    DmathVector3 tmpV3_0;
    dmathV3MakeFromP3(&tmpV3_0, pnt);
    return dmathV3Length(&tmpV3_0);
}

static inline double dmathP3DistSqr(
    const DmathPoint3* pnt0, const DmathPoint3* pnt1)
{
    DmathVector3 tmpV3_0;
    dmathP3Sub(&tmpV3_0, pnt1, pnt0);
    return dmathV3LengthSqr(&tmpV3_0);
}

static inline double dmathP3Dist(
    const DmathPoint3* pnt0, const DmathPoint3* pnt1)
{
    DmathVector3 tmpV3_0;
    dmathP3Sub(&tmpV3_0, pnt1, pnt0);
    return dmathV3Length(&tmpV3_0);
}

static inline void dmathP3Select(DmathPoint3* result, const DmathPoint3* pnt0,
    const DmathPoint3* pnt1, unsigned int select1)
{
    result->x = (select1) ? pnt1->x : pnt0->x;
    result->y = (select1) ? pnt1->y : pnt0->y;
    result->z = (select1) ? pnt1->z : pnt0->z;
}

#ifdef _DVECTORMATH_DEBUG

static inline void dmathP3Print(const DmathPoint3* pnt)
{
    printf("( %f %f %f )\n", pnt->x, pnt->y, pnt->z);
}

static inline void dmathP3Prints(const DmathPoint3* pnt, const char* name)
{
    printf("%s: ( %f %f %f )\n", name, pnt->x, pnt->y, pnt->z);
}

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

/*
   Copyright (C) 2006, 2007 Sony Computer Entertainment Inc.
   All rights reserved.

   Redistribution and use in source and binary forms,
   with or without modification, are permitted provided that the
   following conditions are met:
 * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
 * Neither the name of the Sony Computer Entertainment Inc nor the names
      of its contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _DVECTORMATH_QUAT_AOS_C_H
#define _DVECTORMATH_QUAT_AOS_C_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*-----------------------------------------------------------------------------
 * Definitions
 */
#ifndef _DVECTORMATH_INTERNAL_FUNCTIONS
#define _DVECTORMATH_INTERNAL_FUNCTIONS

#endif

static inline void dmathQCopy(DmathQuat* result, const DmathQuat* quat)
{
    result->x = quat->x;
    result->y = quat->y;
    result->z = quat->z;
    result->w = quat->w;
}

static inline void dmathQMakeFromElems(
    DmathQuat* result, double _x, double _y, double _z, double _w)
{
    result->x = _x;
    result->y = _y;
    result->z = _z;
    result->w = _w;
}

static inline void dmathQMakeFromV3Scalar(
    DmathQuat* result, const DmathVector3* xyz, double _w)
{
    dmathQSetXYZ(result, xyz);
    dmathQSetW(result, _w);
}

static inline void dmathQMakeFromV4(DmathQuat* result, const DmathVector4* vec)
{
    result->x = vec->x;
    result->y = vec->y;
    result->z = vec->z;
    result->w = vec->w;
}

static inline void dmathQMakeFromScalar(DmathQuat* result, double scalar)
{
    result->x = scalar;
    result->y = scalar;
    result->z = scalar;
    result->w = scalar;
}

static inline void dmathQMakeIdentity(DmathQuat* result)
{
    dmathQMakeFromElems(result, 0.0f, 0.0f, 0.0f, 1.0f);
}

static inline void dmathQLerp(
    DmathQuat* result, double t, const DmathQuat* quat0, const DmathQuat* quat1)
{
    DmathQuat tmpQ_0, tmpQ_1;
    dmathQSub(&tmpQ_0, quat1, quat0);
    dmathQScalarMul(&tmpQ_1, &tmpQ_0, t);
    dmathQAdd(result, quat0, &tmpQ_1);
}

static inline void dmathQSlerp(DmathQuat* result, double t,
    const DmathQuat* unitQuat0, const DmathQuat* unitQuat1)
{
    DmathQuat start, tmpQ_0, tmpQ_1;
    double recipSinAngle, scale0, scale1, cosAngle, angle;
    cosAngle = dmathQDot(unitQuat0, unitQuat1);
    if (cosAngle < 0.0f) {
        cosAngle = -cosAngle;
        dmathQNeg(&start, unitQuat0);
    } else {
        dmathQCopy(&start, unitQuat0);
    }
    if (cosAngle < _DVECTORMATH_SLERP_TOL) {
        angle = acosf(cosAngle);
        recipSinAngle = (1.0f / sinf(angle));
        scale0 = (sinf(((1.0f - t) * angle)) * recipSinAngle);
        scale1 = (sinf((t * angle)) * recipSinAngle);
    } else {
        scale0 = (1.0f - t);
        scale1 = t;
    }
    dmathQScalarMul(&tmpQ_0, &start, scale0);
    dmathQScalarMul(&tmpQ_1, unitQuat1, scale1);
    dmathQAdd(result, &tmpQ_0, &tmpQ_1);
}

static inline void dmathQSquad(DmathQuat* result, double t,
    const DmathQuat* unitQuat0, const DmathQuat* unitQuat1,
    const DmathQuat* unitQuat2, const DmathQuat* unitQuat3)
{
    DmathQuat tmp0, tmp1;
    dmathQSlerp(&tmp0, t, unitQuat0, unitQuat3);
    dmathQSlerp(&tmp1, t, unitQuat1, unitQuat2);
    dmathQSlerp(result, ((2.0f * t) * (1.0f - t)), &tmp0, &tmp1);
}

static inline void dmathQSetXYZ(DmathQuat* result, const DmathVector3* vec)
{
    result->x = vec->x;
    result->y = vec->y;
    result->z = vec->z;
}

static inline void dmathQGetXYZ(DmathVector3* result, const DmathQuat* quat)
{
    dmathV3MakeFromElems(result, quat->x, quat->y, quat->z);
}

static inline void dmathQSetX(DmathQuat* result, double _x) { result->x = _x; }

static inline double dmathQGetX(const DmathQuat* quat) { return quat->x; }

static inline void dmathQSetY(DmathQuat* result, double _y) { result->y = _y; }

static inline double dmathQGetY(const DmathQuat* quat) { return quat->y; }

static inline void dmathQSetZ(DmathQuat* result, double _z) { result->z = _z; }

static inline double dmathQGetZ(const DmathQuat* quat) { return quat->z; }

static inline void dmathQSetW(DmathQuat* result, double _w) { result->w = _w; }

static inline double dmathQGetW(const DmathQuat* quat) { return quat->w; }

static inline void dmathQSetElem(DmathQuat* result, int idx, double value)
{
    *(&result->x + idx) = value;
}

static inline double dmathQGetElem(const DmathQuat* quat, int idx)
{
    return *(&quat->x + idx);
}

static inline void dmathQAdd(
    DmathQuat* result, const DmathQuat* quat0, const DmathQuat* quat1)
{
    result->x = (quat0->x + quat1->x);
    result->y = (quat0->y + quat1->y);
    result->z = (quat0->z + quat1->z);
    result->w = (quat0->w + quat1->w);
}

static inline void dmathQSub(
    DmathQuat* result, const DmathQuat* quat0, const DmathQuat* quat1)
{
    result->x = (quat0->x - quat1->x);
    result->y = (quat0->y - quat1->y);
    result->z = (quat0->z - quat1->z);
    result->w = (quat0->w - quat1->w);
}

static inline void dmathQScalarMul(
    DmathQuat* result, const DmathQuat* quat, double scalar)
{
    result->x = (quat->x * scalar);
    result->y = (quat->y * scalar);
    result->z = (quat->z * scalar);
    result->w = (quat->w * scalar);
}

static inline void dmathQScalarDiv(
    DmathQuat* result, const DmathQuat* quat, double scalar)
{
    result->x = (quat->x / scalar);
    result->y = (quat->y / scalar);
    result->z = (quat->z / scalar);
    result->w = (quat->w / scalar);
}

static inline void dmathQNeg(DmathQuat* result, const DmathQuat* quat)
{
    result->x = -quat->x;
    result->y = -quat->y;
    result->z = -quat->z;
    result->w = -quat->w;
}

static inline double dmathQDot(const DmathQuat* quat0, const DmathQuat* quat1)
{
    double result;
    result = (quat0->x * quat1->x);
    result = (result + (quat0->y * quat1->y));
    result = (result + (quat0->z * quat1->z));
    result = (result + (quat0->w * quat1->w));
    return result;
}

static inline double dmathQNorm(const DmathQuat* quat)
{
    double result;
    result = (quat->x * quat->x);
    result = (result + (quat->y * quat->y));
    result = (result + (quat->z * quat->z));
    result = (result + (quat->w * quat->w));
    return result;
}

static inline double dmathQLength(const DmathQuat* quat)
{
    return sqrtf(dmathQNorm(quat));
}

static inline void dmathQNormalize(DmathQuat* result, const DmathQuat* quat)
{
    double lenSqr, lenInv;
    lenSqr = dmathQNorm(quat);
    lenInv = (1.0f / sqrtf(lenSqr));
    result->x = (quat->x * lenInv);
    result->y = (quat->y * lenInv);
    result->z = (quat->z * lenInv);
    result->w = (quat->w * lenInv);
}

static inline void dmathQMakeRotationArc(DmathQuat* result,
    const DmathVector3* unitVec0, const DmathVector3* unitVec1)
{
    DmathVector3 tmpV3_0, tmpV3_1;
    double cosHalfAngleX2, recipCosHalfAngleX2;
    cosHalfAngleX2 = sqrtf((2.0f * (1.0f + dmathV3Dot(unitVec0, unitVec1))));
    recipCosHalfAngleX2 = (1.0f / cosHalfAngleX2);
    dmathV3Cross(&tmpV3_0, unitVec0, unitVec1);
    dmathV3ScalarMul(&tmpV3_1, &tmpV3_0, recipCosHalfAngleX2);
    dmathQMakeFromV3Scalar(result, &tmpV3_1, (cosHalfAngleX2 * 0.5f));
}

static inline void dmathQMakeRotationAxis(
    DmathQuat* result, double radians, const DmathVector3* unitVec)
{
    DmathVector3 tmpV3_0;
    double s, c, angle;
    angle = (radians * 0.5f);
    s = sinf(angle);
    c = cosf(angle);
    dmathV3ScalarMul(&tmpV3_0, unitVec, s);
    dmathQMakeFromV3Scalar(result, &tmpV3_0, c);
}

static inline void dmathQMakeRotationX(DmathQuat* result, double radians)
{
    double s, c, angle;
    angle = (radians * 0.5f);
    s = sinf(angle);
    c = cosf(angle);
    dmathQMakeFromElems(result, s, 0.0f, 0.0f, c);
}

static inline void dmathQMakeRotationY(DmathQuat* result, double radians)
{
    double s, c, angle;
    angle = (radians * 0.5f);
    s = sinf(angle);
    c = cosf(angle);
    dmathQMakeFromElems(result, 0.0f, s, 0.0f, c);
}

static inline void dmathQMakeRotationZ(DmathQuat* result, double radians)
{
    double s, c, angle;
    angle = (radians * 0.5f);
    s = sinf(angle);
    c = cosf(angle);
    dmathQMakeFromElems(result, 0.0f, 0.0f, s, c);
}

static inline void dmathQMul(
    DmathQuat* result, const DmathQuat* quat0, const DmathQuat* quat1)
{
    double tmpX, tmpY, tmpZ, tmpW;
    tmpX = ((((quat0->w * quat1->x) + (quat0->x * quat1->w)) +
        (quat0->y * quat1->z)) -
        (quat0->z * quat1->y));
    tmpY = ((((quat0->w * quat1->y) + (quat0->y * quat1->w)) +
        (quat0->z * quat1->x)) -
        (quat0->x * quat1->z));
    tmpZ = ((((quat0->w * quat1->z) + (quat0->z * quat1->w)) +
        (quat0->x * quat1->y)) -
        (quat0->y * quat1->x));
    tmpW = ((((quat0->w * quat1->w) - (quat0->x * quat1->x)) -
        (quat0->y * quat1->y)) -
        (quat0->z * quat1->z));
    dmathQMakeFromElems(result, tmpX, tmpY, tmpZ, tmpW);
}

static inline void dmathQRotate(
    DmathVector3* result, const DmathQuat* quat, const DmathVector3* vec)
{
    double tmpX, tmpY, tmpZ, tmpW;
    tmpX = (((quat->w * vec->x) + (quat->y * vec->z)) - (quat->z * vec->y));
    tmpY = (((quat->w * vec->y) + (quat->z * vec->x)) - (quat->x * vec->z));
    tmpZ = (((quat->w * vec->z) + (quat->x * vec->y)) - (quat->y * vec->x));
    tmpW = (((quat->x * vec->x) + (quat->y * vec->y)) + (quat->z * vec->z));
    result->x = ((((tmpW * quat->x) + (tmpX * quat->w)) - (tmpY * quat->z)) +
        (tmpZ * quat->y));
    result->y = ((((tmpW * quat->y) + (tmpY * quat->w)) - (tmpZ * quat->x)) +
        (tmpX * quat->z));
    result->z = ((((tmpW * quat->z) + (tmpZ * quat->w)) - (tmpX * quat->y)) +
        (tmpY * quat->x));
}

static inline void dmathQConj(DmathQuat* result, const DmathQuat* quat)
{
    dmathQMakeFromElems(result, -quat->x, -quat->y, -quat->z, quat->w);
}

static inline void dmathQSelect(DmathQuat* result, const DmathQuat* quat0,
    const DmathQuat* quat1, unsigned int select1)
{
    result->x = (select1) ? quat1->x : quat0->x;
    result->y = (select1) ? quat1->y : quat0->y;
    result->z = (select1) ? quat1->z : quat0->z;
    result->w = (select1) ? quat1->w : quat0->w;
}

#ifdef _DVECTORMATH_DEBUG

static inline void dmathQPrint(const DmathQuat* quat)
{
    printf("( %f %f %f %f )\n", quat->x, quat->y, quat->z, quat->w);
}

static inline void dmathQPrints(const DmathQuat* quat, const char* name)
{
    printf("%s: ( %f %f %f %f )\n", name, quat->x, quat->y, quat->z, quat->w);
}

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

/*
   Copyright (C) 2006, 2007 Sony Computer Entertainment Inc.
   All rights reserved.

   Redistribution and use in source and binary forms,
   with or without modification, are permitted provided that the
   following conditions are met:
 * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
 * Neither the name of the Sony Computer Entertainment Inc nor the names
      of its contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _DVECTORMATH_MAT_AOS_C_H
#define _DVECTORMATH_MAT_AOS_C_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*-----------------------------------------------------------------------------
 * Constants
 */
#define _DVECTORMATH_PI_OVER_2 1.570796327f

/*-----------------------------------------------------------------------------
 * Definitions
 */
static inline void dmathM3Copy(DmathMatrix3* result, const DmathMatrix3* mat)
{
    dmathV3Copy(&result->col0, &mat->col0);
    dmathV3Copy(&result->col1, &mat->col1);
    dmathV3Copy(&result->col2, &mat->col2);
}

static inline void dmathM3MakeFromScalar(DmathMatrix3* result, double scalar)
{
    dmathV3MakeFromScalar(&result->col0, scalar);
    dmathV3MakeFromScalar(&result->col1, scalar);
    dmathV3MakeFromScalar(&result->col2, scalar);
}

static inline void dmathM3MakeFromQ(
    DmathMatrix3* result, const DmathQuat* unitQuat)
{
    double qx, qy, qz, qw, qx2, qy2, qz2, qxqx2, qyqy2, qzqz2, qxqy2, qyqz2,
        qzqw2, qxqz2, qyqw2, qxqw2;
    qx = unitQuat->x;
    qy = unitQuat->y;
    qz = unitQuat->z;
    qw = unitQuat->w;
    qx2 = (qx + qx);
    qy2 = (qy + qy);
    qz2 = (qz + qz);
    qxqx2 = (qx * qx2);
    qxqy2 = (qx * qy2);
    qxqz2 = (qx * qz2);
    qxqw2 = (qw * qx2);
    qyqy2 = (qy * qy2);
    qyqz2 = (qy * qz2);
    qyqw2 = (qw * qy2);
    qzqz2 = (qz * qz2);
    qzqw2 = (qw * qz2);
    dmathV3MakeFromElems(&result->col0, ((1.0f - qyqy2) - qzqz2),
        (qxqy2 + qzqw2), (qxqz2 - qyqw2));
    dmathV3MakeFromElems(&result->col1, (qxqy2 - qzqw2),
        ((1.0f - qxqx2) - qzqz2), (qyqz2 + qxqw2));
    dmathV3MakeFromElems(&result->col2, (qxqz2 + qyqw2), (qyqz2 - qxqw2),
        ((1.0f - qxqx2) - qyqy2));
}

static inline void dmathM3MakeFromCols(DmathMatrix3* result,
    const DmathVector3* _col0, const DmathVector3* _col1,
    const DmathVector3* _col2)
{
    dmathV3Copy(&result->col0, _col0);
    dmathV3Copy(&result->col1, _col1);
    dmathV3Copy(&result->col2, _col2);
}

static inline void dmathM3SetCol0(
    DmathMatrix3* result, const DmathVector3* _col0)
{
    dmathV3Copy(&result->col0, _col0);
}

static inline void dmathM3SetCol1(
    DmathMatrix3* result, const DmathVector3* _col1)
{
    dmathV3Copy(&result->col1, _col1);
}

static inline void dmathM3SetCol2(
    DmathMatrix3* result, const DmathVector3* _col2)
{
    dmathV3Copy(&result->col2, _col2);
}

static inline void dmathM3SetCol(
    DmathMatrix3* result, int col, const DmathVector3* vec)
{
    dmathV3Copy((&result->col0 + col), vec);
}

static inline void dmathM3SetRow(
    DmathMatrix3* result, int row, const DmathVector3* vec)
{
    dmathV3SetElem(&result->col0, row, dmathV3GetElem(vec, 0));
    dmathV3SetElem(&result->col1, row, dmathV3GetElem(vec, 1));
    dmathV3SetElem(&result->col2, row, dmathV3GetElem(vec, 2));
}

static inline void dmathM3SetElem(
    DmathMatrix3* result, int col, int row, double val)
{
    DmathVector3 tmpV3_0;
    dmathM3GetCol(&tmpV3_0, result, col);
    dmathV3SetElem(&tmpV3_0, row, val);
    dmathM3SetCol(result, col, &tmpV3_0);
}

static inline double dmathM3GetElem(const DmathMatrix3* mat, int col, int row)
{
    DmathVector3 tmpV3_0;
    dmathM3GetCol(&tmpV3_0, mat, col);
    return dmathV3GetElem(&tmpV3_0, row);
}

static inline void dmathM3GetCol0(DmathVector3* result, const DmathMatrix3* mat)
{
    dmathV3Copy(result, &mat->col0);
}

static inline void dmathM3GetCol1(DmathVector3* result, const DmathMatrix3* mat)
{
    dmathV3Copy(result, &mat->col1);
}

static inline void dmathM3GetCol2(DmathVector3* result, const DmathMatrix3* mat)
{
    dmathV3Copy(result, &mat->col2);
}

static inline void dmathM3GetCol(
    DmathVector3* result, const DmathMatrix3* mat, int col)
{
    dmathV3Copy(result, (&mat->col0 + col));
}

static inline void dmathM3GetRow(
    DmathVector3* result, const DmathMatrix3* mat, int row)
{
    dmathV3MakeFromElems(result, dmathV3GetElem(&mat->col0, row),
        dmathV3GetElem(&mat->col1, row), dmathV3GetElem(&mat->col2, row));
}

static inline void dmathM3Transpose(
    DmathMatrix3* result, const DmathMatrix3* mat)
{
    DmathMatrix3 tmpResult;
    dmathV3MakeFromElems(
        &tmpResult.col0, mat->col0.x, mat->col1.x, mat->col2.x);
    dmathV3MakeFromElems(
        &tmpResult.col1, mat->col0.y, mat->col1.y, mat->col2.y);
    dmathV3MakeFromElems(
        &tmpResult.col2, mat->col0.z, mat->col1.z, mat->col2.z);
    dmathM3Copy(result, &tmpResult);
}

static inline void dmathM3Inverse(DmathMatrix3* result, const DmathMatrix3* mat)
{
    DmathVector3 tmp0, tmp1, tmp2;
    double detinv;
    dmathV3Cross(&tmp0, &mat->col1, &mat->col2);
    dmathV3Cross(&tmp1, &mat->col2, &mat->col0);
    dmathV3Cross(&tmp2, &mat->col0, &mat->col1);
    detinv = (1.0f / dmathV3Dot(&mat->col2, &tmp2));
    dmathV3MakeFromElems(
        &result->col0, (tmp0.x * detinv), (tmp1.x * detinv), (tmp2.x * detinv));
    dmathV3MakeFromElems(
        &result->col1, (tmp0.y * detinv), (tmp1.y * detinv), (tmp2.y * detinv));
    dmathV3MakeFromElems(
        &result->col2, (tmp0.z * detinv), (tmp1.z * detinv), (tmp2.z * detinv));
}

static inline double dmathM3Determinant(const DmathMatrix3* mat)
{
    DmathVector3 tmpV3_0;
    dmathV3Cross(&tmpV3_0, &mat->col0, &mat->col1);
    return dmathV3Dot(&mat->col2, &tmpV3_0);
}

static inline void dmathM3Add(
    DmathMatrix3* result, const DmathMatrix3* mat0, const DmathMatrix3* mat1)
{
    dmathV3Add(&result->col0, &mat0->col0, &mat1->col0);
    dmathV3Add(&result->col1, &mat0->col1, &mat1->col1);
    dmathV3Add(&result->col2, &mat0->col2, &mat1->col2);
}

static inline void dmathM3Sub(
    DmathMatrix3* result, const DmathMatrix3* mat0, const DmathMatrix3* mat1)
{
    dmathV3Sub(&result->col0, &mat0->col0, &mat1->col0);
    dmathV3Sub(&result->col1, &mat0->col1, &mat1->col1);
    dmathV3Sub(&result->col2, &mat0->col2, &mat1->col2);
}

static inline void dmathM3Neg(DmathMatrix3* result, const DmathMatrix3* mat)
{
    dmathV3Neg(&result->col0, &mat->col0);
    dmathV3Neg(&result->col1, &mat->col1);
    dmathV3Neg(&result->col2, &mat->col2);
}

static inline void dmathM3AbsPerElem(
    DmathMatrix3* result, const DmathMatrix3* mat)
{
    dmathV3AbsPerElem(&result->col0, &mat->col0);
    dmathV3AbsPerElem(&result->col1, &mat->col1);
    dmathV3AbsPerElem(&result->col2, &mat->col2);
}

static inline void dmathM3ScalarMul(
    DmathMatrix3* result, const DmathMatrix3* mat, double scalar)
{
    dmathV3ScalarMul(&result->col0, &mat->col0, scalar);
    dmathV3ScalarMul(&result->col1, &mat->col1, scalar);
    dmathV3ScalarMul(&result->col2, &mat->col2, scalar);
}

static inline void dmathM3MulV3(
    DmathVector3* result, const DmathMatrix3* mat, const DmathVector3* vec)
{
    double tmpX, tmpY, tmpZ;
    tmpX = (((mat->col0.x * vec->x) + (mat->col1.x * vec->y)) +
        (mat->col2.x * vec->z));
    tmpY = (((mat->col0.y * vec->x) + (mat->col1.y * vec->y)) +
        (mat->col2.y * vec->z));
    tmpZ = (((mat->col0.z * vec->x) + (mat->col1.z * vec->y)) +
        (mat->col2.z * vec->z));
    dmathV3MakeFromElems(result, tmpX, tmpY, tmpZ);
}

static inline void dmathM3Mul(
    DmathMatrix3* result, const DmathMatrix3* mat0, const DmathMatrix3* mat1)
{
    DmathMatrix3 tmpResult;
    dmathM3MulV3(&tmpResult.col0, mat0, &mat1->col0);
    dmathM3MulV3(&tmpResult.col1, mat0, &mat1->col1);
    dmathM3MulV3(&tmpResult.col2, mat0, &mat1->col2);
    dmathM3Copy(result, &tmpResult);
}

static inline void dmathM3MulPerElem(
    DmathMatrix3* result, const DmathMatrix3* mat0, const DmathMatrix3* mat1)
{
    dmathV3MulPerElem(&result->col0, &mat0->col0, &mat1->col0);
    dmathV3MulPerElem(&result->col1, &mat0->col1, &mat1->col1);
    dmathV3MulPerElem(&result->col2, &mat0->col2, &mat1->col2);
}

static inline void dmathM3MakeIdentity(DmathMatrix3* result)
{
    dmathV3MakeXAxis(&result->col0);
    dmathV3MakeYAxis(&result->col1);
    dmathV3MakeZAxis(&result->col2);
}

static inline void dmathM3MakeRotationX(DmathMatrix3* result, double radians)
{
    double s, c;
    s = sinf(radians);
    c = cosf(radians);
    dmathV3MakeXAxis(&result->col0);
    dmathV3MakeFromElems(&result->col1, 0.0f, c, s);
    dmathV3MakeFromElems(&result->col2, 0.0f, -s, c);
}

static inline void dmathM3MakeRotationY(DmathMatrix3* result, double radians)
{
    double s, c;
    s = sinf(radians);
    c = cosf(radians);
    dmathV3MakeFromElems(&result->col0, c, 0.0f, -s);
    dmathV3MakeYAxis(&result->col1);
    dmathV3MakeFromElems(&result->col2, s, 0.0f, c);
}

static inline void dmathM3MakeRotationZ(DmathMatrix3* result, double radians)
{
    double s, c;
    s = sinf(radians);
    c = cosf(radians);
    dmathV3MakeFromElems(&result->col0, c, s, 0.0f);
    dmathV3MakeFromElems(&result->col1, -s, c, 0.0f);
    dmathV3MakeZAxis(&result->col2);
}

static inline void dmathM3MakeRotationZYX(
    DmathMatrix3* result, const DmathVector3* radiansXYZ)
{
    double sX, cX, sY, cY, sZ, cZ, tmp0, tmp1;
    sX = sinf(radiansXYZ->x);
    cX = cosf(radiansXYZ->x);
    sY = sinf(radiansXYZ->y);
    cY = cosf(radiansXYZ->y);
    sZ = sinf(radiansXYZ->z);
    cZ = cosf(radiansXYZ->z);
    tmp0 = (cZ * sY);
    tmp1 = (sZ * sY);
    dmathV3MakeFromElems(&result->col0, (cZ * cY), (sZ * cY), -sY);
    dmathV3MakeFromElems(&result->col1, ((tmp0 * sX) - (sZ * cX)),
        ((tmp1 * sX) + (cZ * cX)), (cY * sX));
    dmathV3MakeFromElems(&result->col2, ((tmp0 * cX) + (sZ * sX)),
        ((tmp1 * cX) - (cZ * sX)), (cY * cX));
}

static inline void dmathM3MakeRotationAxis(
    DmathMatrix3* result, double radians, const DmathVector3* unitVec)
{
    double x, y, z, s, c, oneMinusC, xy, yz, zx;
    s = sinf(radians);
    c = cosf(radians);
    x = unitVec->x;
    y = unitVec->y;
    z = unitVec->z;
    xy = (x * y);
    yz = (y * z);
    zx = (z * x);
    oneMinusC = (1.0f - c);
    dmathV3MakeFromElems(&result->col0, (((x * x) * oneMinusC) + c),
        ((xy * oneMinusC) + (z * s)), ((zx * oneMinusC) - (y * s)));
    dmathV3MakeFromElems(&result->col1, ((xy * oneMinusC) - (z * s)),
        (((y * y) * oneMinusC) + c), ((yz * oneMinusC) + (x * s)));
    dmathV3MakeFromElems(&result->col2, ((zx * oneMinusC) + (y * s)),
        ((yz * oneMinusC) - (x * s)), (((z * z) * oneMinusC) + c));
}

static inline void dmathM3MakeRotationQ(
    DmathMatrix3* result, const DmathQuat* unitQuat)
{
    dmathM3MakeFromQ(result, unitQuat);
}

static inline void dmathM3MakeScale(
    DmathMatrix3* result, const DmathVector3* scaleVec)
{
    dmathV3MakeFromElems(&result->col0, scaleVec->x, 0.0f, 0.0f);
    dmathV3MakeFromElems(&result->col1, 0.0f, scaleVec->y, 0.0f);
    dmathV3MakeFromElems(&result->col2, 0.0f, 0.0f, scaleVec->z);
}

static inline void dmathM3AppendScale(
    DmathMatrix3* result, const DmathMatrix3* mat, const DmathVector3* scaleVec)
{
    dmathV3ScalarMul(&result->col0, &mat->col0, dmathV3GetX(scaleVec));
    dmathV3ScalarMul(&result->col1, &mat->col1, dmathV3GetY(scaleVec));
    dmathV3ScalarMul(&result->col2, &mat->col2, dmathV3GetZ(scaleVec));
}

static inline void dmathM3PrependScale(
    DmathMatrix3* result, const DmathVector3* scaleVec, const DmathMatrix3* mat)
{
    dmathV3MulPerElem(&result->col0, &mat->col0, scaleVec);
    dmathV3MulPerElem(&result->col1, &mat->col1, scaleVec);
    dmathV3MulPerElem(&result->col2, &mat->col2, scaleVec);
}

static inline void dmathM3Select(DmathMatrix3* result, const DmathMatrix3* mat0,
    const DmathMatrix3* mat1, unsigned int select1)
{
    dmathV3Select(&result->col0, &mat0->col0, &mat1->col0, select1);
    dmathV3Select(&result->col1, &mat0->col1, &mat1->col1, select1);
    dmathV3Select(&result->col2, &mat0->col2, &mat1->col2, select1);
}

#ifdef _DVECTORMATH_DEBUG

static inline void dmathM3Print(const DmathMatrix3* mat)
{
    DmathVector3 tmpV3_0, tmpV3_1, tmpV3_2;
    dmathM3GetRow(&tmpV3_0, mat, 0);
    dmathV3Print(&tmpV3_0);
    dmathM3GetRow(&tmpV3_1, mat, 1);
    dmathV3Print(&tmpV3_1);
    dmathM3GetRow(&tmpV3_2, mat, 2);
    dmathV3Print(&tmpV3_2);
}

static inline void dmathM3Prints(const DmathMatrix3* mat, const char* name)
{
    printf("%s:\n", name);
    dmathM3Print(mat);
}

#endif

static inline void dmathM4Copy(DmathMatrix4* result, const DmathMatrix4* mat)
{
    dmathV4Copy(&result->col0, &mat->col0);
    dmathV4Copy(&result->col1, &mat->col1);
    dmathV4Copy(&result->col2, &mat->col2);
    dmathV4Copy(&result->col3, &mat->col3);
}

static inline void dmathM4MakeFromScalar(DmathMatrix4* result, double scalar)
{
    dmathV4MakeFromScalar(&result->col0, scalar);
    dmathV4MakeFromScalar(&result->col1, scalar);
    dmathV4MakeFromScalar(&result->col2, scalar);
    dmathV4MakeFromScalar(&result->col3, scalar);
}

static inline void dmathM4MakeFromT3(
    DmathMatrix4* result, const DmathTransform3* mat)
{
    dmathV4MakeFromV3Scalar(&result->col0, &mat->col0, 0.0f);
    dmathV4MakeFromV3Scalar(&result->col1, &mat->col1, 0.0f);
    dmathV4MakeFromV3Scalar(&result->col2, &mat->col2, 0.0f);
    dmathV4MakeFromV3Scalar(&result->col3, &mat->col3, 1.0f);
}

static inline void dmathM4MakeFromCols(DmathMatrix4* result,
    const DmathVector4* _col0, const DmathVector4* _col1,
    const DmathVector4* _col2, const DmathVector4* _col3)
{
    dmathV4Copy(&result->col0, _col0);
    dmathV4Copy(&result->col1, _col1);
    dmathV4Copy(&result->col2, _col2);
    dmathV4Copy(&result->col3, _col3);
}

static inline void dmathM4MakeFromM3V3(DmathMatrix4* result,
    const DmathMatrix3* mat, const DmathVector3* translateVec)
{
    dmathV4MakeFromV3Scalar(&result->col0, &mat->col0, 0.0f);
    dmathV4MakeFromV3Scalar(&result->col1, &mat->col1, 0.0f);
    dmathV4MakeFromV3Scalar(&result->col2, &mat->col2, 0.0f);
    dmathV4MakeFromV3Scalar(&result->col3, translateVec, 1.0f);
}

static inline void dmathM4MakeFromQV3(DmathMatrix4* result,
    const DmathQuat* unitQuat, const DmathVector3* translateVec)
{
    DmathMatrix3 mat;
    dmathM3MakeFromQ(&mat, unitQuat);
    dmathV4MakeFromV3Scalar(&result->col0, &mat.col0, 0.0f);
    dmathV4MakeFromV3Scalar(&result->col1, &mat.col1, 0.0f);
    dmathV4MakeFromV3Scalar(&result->col2, &mat.col2, 0.0f);
    dmathV4MakeFromV3Scalar(&result->col3, translateVec, 1.0f);
}

static inline void dmathM4SetCol0(
    DmathMatrix4* result, const DmathVector4* _col0)
{
    dmathV4Copy(&result->col0, _col0);
}

static inline void dmathM4SetCol1(
    DmathMatrix4* result, const DmathVector4* _col1)
{
    dmathV4Copy(&result->col1, _col1);
}

static inline void dmathM4SetCol2(
    DmathMatrix4* result, const DmathVector4* _col2)
{
    dmathV4Copy(&result->col2, _col2);
}

static inline void dmathM4SetCol3(
    DmathMatrix4* result, const DmathVector4* _col3)
{
    dmathV4Copy(&result->col3, _col3);
}

static inline void dmathM4SetCol(
    DmathMatrix4* result, int col, const DmathVector4* vec)
{
    dmathV4Copy((&result->col0 + col), vec);
}

static inline void dmathM4SetRow(
    DmathMatrix4* result, int row, const DmathVector4* vec)
{
    dmathV4SetElem(&result->col0, row, dmathV4GetElem(vec, 0));
    dmathV4SetElem(&result->col1, row, dmathV4GetElem(vec, 1));
    dmathV4SetElem(&result->col2, row, dmathV4GetElem(vec, 2));
    dmathV4SetElem(&result->col3, row, dmathV4GetElem(vec, 3));
}

static inline void dmathM4SetElem(
    DmathMatrix4* result, int col, int row, double val)
{
    DmathVector4 tmpV3_0;
    dmathM4GetCol(&tmpV3_0, result, col);
    dmathV4SetElem(&tmpV3_0, row, val);
    dmathM4SetCol(result, col, &tmpV3_0);
}

static inline double dmathM4GetElem(const DmathMatrix4* mat, int col, int row)
{
    DmathVector4 tmpV4_0;
    dmathM4GetCol(&tmpV4_0, mat, col);
    return dmathV4GetElem(&tmpV4_0, row);
}

static inline void dmathM4GetCol0(DmathVector4* result, const DmathMatrix4* mat)
{
    dmathV4Copy(result, &mat->col0);
}

static inline void dmathM4GetCol1(DmathVector4* result, const DmathMatrix4* mat)
{
    dmathV4Copy(result, &mat->col1);
}

static inline void dmathM4GetCol2(DmathVector4* result, const DmathMatrix4* mat)
{
    dmathV4Copy(result, &mat->col2);
}

static inline void dmathM4GetCol3(DmathVector4* result, const DmathMatrix4* mat)
{
    dmathV4Copy(result, &mat->col3);
}

static inline void dmathM4GetCol(
    DmathVector4* result, const DmathMatrix4* mat, int col)
{
    dmathV4Copy(result, (&mat->col0 + col));
}

static inline void dmathM4GetRow(
    DmathVector4* result, const DmathMatrix4* mat, int row)
{
    dmathV4MakeFromElems(result, dmathV4GetElem(&mat->col0, row),
        dmathV4GetElem(&mat->col1, row), dmathV4GetElem(&mat->col2, row),
        dmathV4GetElem(&mat->col3, row));
}

static inline void dmathM4Transpose(
    DmathMatrix4* result, const DmathMatrix4* mat)
{
    DmathMatrix4 tmpResult;
    dmathV4MakeFromElems(
        &tmpResult.col0, mat->col0.x, mat->col1.x, mat->col2.x, mat->col3.x);
    dmathV4MakeFromElems(
        &tmpResult.col1, mat->col0.y, mat->col1.y, mat->col2.y, mat->col3.y);
    dmathV4MakeFromElems(
        &tmpResult.col2, mat->col0.z, mat->col1.z, mat->col2.z, mat->col3.z);
    dmathV4MakeFromElems(
        &tmpResult.col3, mat->col0.w, mat->col1.w, mat->col2.w, mat->col3.w);
    dmathM4Copy(result, &tmpResult);
}

static inline void dmathM4Inverse(DmathMatrix4* result, const DmathMatrix4* mat)
{
    DmathVector4 res0, res1, res2, res3;
    double mA, mB, mC, mD, mE, mF, mG, mH, mI, mJ, mK, mL, mM, mN, mO, mP, tmp0,
        tmp1, tmp2, tmp3, tmp4, tmp5, detInv;
    mA = mat->col0.x;
    mB = mat->col0.y;
    mC = mat->col0.z;
    mD = mat->col0.w;
    mE = mat->col1.x;
    mF = mat->col1.y;
    mG = mat->col1.z;
    mH = mat->col1.w;
    mI = mat->col2.x;
    mJ = mat->col2.y;
    mK = mat->col2.z;
    mL = mat->col2.w;
    mM = mat->col3.x;
    mN = mat->col3.y;
    mO = mat->col3.z;
    mP = mat->col3.w;
    tmp0 = ((mK * mD) - (mC * mL));
    tmp1 = ((mO * mH) - (mG * mP));
    tmp2 = ((mB * mK) - (mJ * mC));
    tmp3 = ((mF * mO) - (mN * mG));
    tmp4 = ((mJ * mD) - (mB * mL));
    tmp5 = ((mN * mH) - (mF * mP));
    dmathV4SetX(&res0, (((mJ * tmp1) - (mL * tmp3)) - (mK * tmp5)));
    dmathV4SetY(&res0, (((mN * tmp0) - (mP * tmp2)) - (mO * tmp4)));
    dmathV4SetZ(&res0, (((mD * tmp3) + (mC * tmp5)) - (mB * tmp1)));
    dmathV4SetW(&res0, (((mH * tmp2) + (mG * tmp4)) - (mF * tmp0)));
    detInv = (1.0f / ((((mA * res0.x) + (mE * res0.y)) + (mI * res0.z)) +
        (mM * res0.w)));
    dmathV4SetX(&res1, (mI * tmp1));
    dmathV4SetY(&res1, (mM * tmp0));
    dmathV4SetZ(&res1, (mA * tmp1));
    dmathV4SetW(&res1, (mE * tmp0));
    dmathV4SetX(&res3, (mI * tmp3));
    dmathV4SetY(&res3, (mM * tmp2));
    dmathV4SetZ(&res3, (mA * tmp3));
    dmathV4SetW(&res3, (mE * tmp2));
    dmathV4SetX(&res2, (mI * tmp5));
    dmathV4SetY(&res2, (mM * tmp4));
    dmathV4SetZ(&res2, (mA * tmp5));
    dmathV4SetW(&res2, (mE * tmp4));
    tmp0 = ((mI * mB) - (mA * mJ));
    tmp1 = ((mM * mF) - (mE * mN));
    tmp2 = ((mI * mD) - (mA * mL));
    tmp3 = ((mM * mH) - (mE * mP));
    tmp4 = ((mI * mC) - (mA * mK));
    tmp5 = ((mM * mG) - (mE * mO));
    dmathV4SetX(&res2, (((mL * tmp1) - (mJ * tmp3)) + res2.x));
    dmathV4SetY(&res2, (((mP * tmp0) - (mN * tmp2)) + res2.y));
    dmathV4SetZ(&res2, (((mB * tmp3) - (mD * tmp1)) - res2.z));
    dmathV4SetW(&res2, (((mF * tmp2) - (mH * tmp0)) - res2.w));
    dmathV4SetX(&res3, (((mJ * tmp5) - (mK * tmp1)) + res3.x));
    dmathV4SetY(&res3, (((mN * tmp4) - (mO * tmp0)) + res3.y));
    dmathV4SetZ(&res3, (((mC * tmp1) - (mB * tmp5)) - res3.z));
    dmathV4SetW(&res3, (((mG * tmp0) - (mF * tmp4)) - res3.w));
    dmathV4SetX(&res1, (((mK * tmp3) - (mL * tmp5)) - res1.x));
    dmathV4SetY(&res1, (((mO * tmp2) - (mP * tmp4)) - res1.y));
    dmathV4SetZ(&res1, (((mD * tmp5) - (mC * tmp3)) + res1.z));
    dmathV4SetW(&res1, (((mH * tmp4) - (mG * tmp2)) + res1.w));
    dmathV4ScalarMul(&result->col0, &res0, detInv);
    dmathV4ScalarMul(&result->col1, &res1, detInv);
    dmathV4ScalarMul(&result->col2, &res2, detInv);
    dmathV4ScalarMul(&result->col3, &res3, detInv);
}

static inline void dmathM4AffineInverse(
    DmathMatrix4* result, const DmathMatrix4* mat)
{
    DmathTransform3 affineMat, tmpT3_0;
    DmathVector3 tmpV3_0, tmpV3_1, tmpV3_2, tmpV3_3;
    dmathV4GetXYZ(&tmpV3_0, &mat->col0);
    dmathT3SetCol0(&affineMat, &tmpV3_0);
    dmathV4GetXYZ(&tmpV3_1, &mat->col1);
    dmathT3SetCol1(&affineMat, &tmpV3_1);
    dmathV4GetXYZ(&tmpV3_2, &mat->col2);
    dmathT3SetCol2(&affineMat, &tmpV3_2);
    dmathV4GetXYZ(&tmpV3_3, &mat->col3);
    dmathT3SetCol3(&affineMat, &tmpV3_3);
    dmathT3Inverse(&tmpT3_0, &affineMat);
    dmathM4MakeFromT3(result, &tmpT3_0);
}

static inline void dmathM4OrthoInverse(
    DmathMatrix4* result, const DmathMatrix4* mat)
{
    DmathTransform3 affineMat, tmpT3_0;
    DmathVector3 tmpV3_0, tmpV3_1, tmpV3_2, tmpV3_3;
    dmathV4GetXYZ(&tmpV3_0, &mat->col0);
    dmathT3SetCol0(&affineMat, &tmpV3_0);
    dmathV4GetXYZ(&tmpV3_1, &mat->col1);
    dmathT3SetCol1(&affineMat, &tmpV3_1);
    dmathV4GetXYZ(&tmpV3_2, &mat->col2);
    dmathT3SetCol2(&affineMat, &tmpV3_2);
    dmathV4GetXYZ(&tmpV3_3, &mat->col3);
    dmathT3SetCol3(&affineMat, &tmpV3_3);
    dmathT3OrthoInverse(&tmpT3_0, &affineMat);
    dmathM4MakeFromT3(result, &tmpT3_0);
}

static inline double dmathM4Determinant(const DmathMatrix4* mat)
{
    double dx, dy, dz, dw, mA, mB, mC, mD, mE, mF, mG, mH, mI, mJ, mK, mL, mM,
        mN, mO, mP, tmp0, tmp1, tmp2, tmp3, tmp4, tmp5;
    mA = mat->col0.x;
    mB = mat->col0.y;
    mC = mat->col0.z;
    mD = mat->col0.w;
    mE = mat->col1.x;
    mF = mat->col1.y;
    mG = mat->col1.z;
    mH = mat->col1.w;
    mI = mat->col2.x;
    mJ = mat->col2.y;
    mK = mat->col2.z;
    mL = mat->col2.w;
    mM = mat->col3.x;
    mN = mat->col3.y;
    mO = mat->col3.z;
    mP = mat->col3.w;
    tmp0 = ((mK * mD) - (mC * mL));
    tmp1 = ((mO * mH) - (mG * mP));
    tmp2 = ((mB * mK) - (mJ * mC));
    tmp3 = ((mF * mO) - (mN * mG));
    tmp4 = ((mJ * mD) - (mB * mL));
    tmp5 = ((mN * mH) - (mF * mP));
    dx = (((mJ * tmp1) - (mL * tmp3)) - (mK * tmp5));
    dy = (((mN * tmp0) - (mP * tmp2)) - (mO * tmp4));
    dz = (((mD * tmp3) + (mC * tmp5)) - (mB * tmp1));
    dw = (((mH * tmp2) + (mG * tmp4)) - (mF * tmp0));
    return ((((mA * dx) + (mE * dy)) + (mI * dz)) + (mM * dw));
}

static inline void dmathM4Add(
    DmathMatrix4* result, const DmathMatrix4* mat0, const DmathMatrix4* mat1)
{
    dmathV4Add(&result->col0, &mat0->col0, &mat1->col0);
    dmathV4Add(&result->col1, &mat0->col1, &mat1->col1);
    dmathV4Add(&result->col2, &mat0->col2, &mat1->col2);
    dmathV4Add(&result->col3, &mat0->col3, &mat1->col3);
}

static inline void dmathM4Sub(
    DmathMatrix4* result, const DmathMatrix4* mat0, const DmathMatrix4* mat1)
{
    dmathV4Sub(&result->col0, &mat0->col0, &mat1->col0);
    dmathV4Sub(&result->col1, &mat0->col1, &mat1->col1);
    dmathV4Sub(&result->col2, &mat0->col2, &mat1->col2);
    dmathV4Sub(&result->col3, &mat0->col3, &mat1->col3);
}

static inline void dmathM4Neg(DmathMatrix4* result, const DmathMatrix4* mat)
{
    dmathV4Neg(&result->col0, &mat->col0);
    dmathV4Neg(&result->col1, &mat->col1);
    dmathV4Neg(&result->col2, &mat->col2);
    dmathV4Neg(&result->col3, &mat->col3);
}

static inline void dmathM4AbsPerElem(
    DmathMatrix4* result, const DmathMatrix4* mat)
{
    dmathV4AbsPerElem(&result->col0, &mat->col0);
    dmathV4AbsPerElem(&result->col1, &mat->col1);
    dmathV4AbsPerElem(&result->col2, &mat->col2);
    dmathV4AbsPerElem(&result->col3, &mat->col3);
}

static inline void dmathM4ScalarMul(
    DmathMatrix4* result, const DmathMatrix4* mat, double scalar)
{
    dmathV4ScalarMul(&result->col0, &mat->col0, scalar);
    dmathV4ScalarMul(&result->col1, &mat->col1, scalar);
    dmathV4ScalarMul(&result->col2, &mat->col2, scalar);
    dmathV4ScalarMul(&result->col3, &mat->col3, scalar);
}

static inline void dmathM4MulV4(
    DmathVector4* result, const DmathMatrix4* mat, const DmathVector4* vec)
{
    double tmpX, tmpY, tmpZ, tmpW;
    tmpX = ((((mat->col0.x * vec->x) + (mat->col1.x * vec->y)) +
        (mat->col2.x * vec->z)) +
        (mat->col3.x * vec->w));
    tmpY = ((((mat->col0.y * vec->x) + (mat->col1.y * vec->y)) +
        (mat->col2.y * vec->z)) +
        (mat->col3.y * vec->w));
    tmpZ = ((((mat->col0.z * vec->x) + (mat->col1.z * vec->y)) +
        (mat->col2.z * vec->z)) +
        (mat->col3.z * vec->w));
    tmpW = ((((mat->col0.w * vec->x) + (mat->col1.w * vec->y)) +
        (mat->col2.w * vec->z)) +
        (mat->col3.w * vec->w));
    dmathV4MakeFromElems(result, tmpX, tmpY, tmpZ, tmpW);
}

static inline void dmathM4MulV3(
    DmathVector4* result, const DmathMatrix4* mat, const DmathVector3* vec)
{
    result->x = (((mat->col0.x * vec->x) + (mat->col1.x * vec->y)) +
        (mat->col2.x * vec->z));
    result->y = (((mat->col0.y * vec->x) + (mat->col1.y * vec->y)) +
        (mat->col2.y * vec->z));
    result->z = (((mat->col0.z * vec->x) + (mat->col1.z * vec->y)) +
        (mat->col2.z * vec->z));
    result->w = (((mat->col0.w * vec->x) + (mat->col1.w * vec->y)) +
        (mat->col2.w * vec->z));
}

static inline void dmathM4MulP3(
    DmathVector4* result, const DmathMatrix4* mat, const DmathPoint3* pnt)
{
    result->x = ((((mat->col0.x * pnt->x) + (mat->col1.x * pnt->y)) +
        (mat->col2.x * pnt->z)) +
        mat->col3.x);
    result->y = ((((mat->col0.y * pnt->x) + (mat->col1.y * pnt->y)) +
        (mat->col2.y * pnt->z)) +
        mat->col3.y);
    result->z = ((((mat->col0.z * pnt->x) + (mat->col1.z * pnt->y)) +
        (mat->col2.z * pnt->z)) +
        mat->col3.z);
    result->w = ((((mat->col0.w * pnt->x) + (mat->col1.w * pnt->y)) +
        (mat->col2.w * pnt->z)) +
        mat->col3.w);
}

static inline void dmathM4Mul(
    DmathMatrix4* result, const DmathMatrix4* mat0, const DmathMatrix4* mat1)
{
    DmathMatrix4 tmpResult;
    dmathM4MulV4(&tmpResult.col0, mat0, &mat1->col0);
    dmathM4MulV4(&tmpResult.col1, mat0, &mat1->col1);
    dmathM4MulV4(&tmpResult.col2, mat0, &mat1->col2);
    dmathM4MulV4(&tmpResult.col3, mat0, &mat1->col3);
    dmathM4Copy(result, &tmpResult);
}

static inline void dmathM4MulT3(
    DmathMatrix4* result, const DmathMatrix4* mat, const DmathTransform3* tfrm1)
{
    DmathMatrix4 tmpResult;
    DmathPoint3 tmpP3_0;
    dmathM4MulV3(&tmpResult.col0, mat, &tfrm1->col0);
    dmathM4MulV3(&tmpResult.col1, mat, &tfrm1->col1);
    dmathM4MulV3(&tmpResult.col2, mat, &tfrm1->col2);
    dmathP3MakeFromV3(&tmpP3_0, &tfrm1->col3);
    dmathM4MulP3(&tmpResult.col3, mat, &tmpP3_0);
    dmathM4Copy(result, &tmpResult);
}

static inline void dmathM4MulPerElem(
    DmathMatrix4* result, const DmathMatrix4* mat0, const DmathMatrix4* mat1)
{
    dmathV4MulPerElem(&result->col0, &mat0->col0, &mat1->col0);
    dmathV4MulPerElem(&result->col1, &mat0->col1, &mat1->col1);
    dmathV4MulPerElem(&result->col2, &mat0->col2, &mat1->col2);
    dmathV4MulPerElem(&result->col3, &mat0->col3, &mat1->col3);
}

static inline void dmathM4MakeIdentity(DmathMatrix4* result)
{
    dmathV4MakeXAxis(&result->col0);
    dmathV4MakeYAxis(&result->col1);
    dmathV4MakeZAxis(&result->col2);
    dmathV4MakeWAxis(&result->col3);
}

static inline void dmathM4SetUpper3x3(
    DmathMatrix4* result, const DmathMatrix3* mat3)
{
    dmathV4SetXYZ(&result->col0, &mat3->col0);
    dmathV4SetXYZ(&result->col1, &mat3->col1);
    dmathV4SetXYZ(&result->col2, &mat3->col2);
}

static inline void dmathM4GetUpper3x3(
    DmathMatrix3* result, const DmathMatrix4* mat)
{
    dmathV4GetXYZ(&result->col0, &mat->col0);
    dmathV4GetXYZ(&result->col1, &mat->col1);
    dmathV4GetXYZ(&result->col2, &mat->col2);
}

static inline void dmathM4SetTranslation(
    DmathMatrix4* result, const DmathVector3* translateVec)
{
    dmathV4SetXYZ(&result->col3, translateVec);
}

static inline void dmathM4GetTranslation(
    DmathVector3* result, const DmathMatrix4* mat)
{
    dmathV4GetXYZ(result, &mat->col3);
}

static inline void dmathM4MakeRotationX(DmathMatrix4* result, double radians)
{
    double s, c;
    s = sinf(radians);
    c = cosf(radians);
    dmathV4MakeXAxis(&result->col0);
    dmathV4MakeFromElems(&result->col1, 0.0f, c, s, 0.0f);
    dmathV4MakeFromElems(&result->col2, 0.0f, -s, c, 0.0f);
    dmathV4MakeWAxis(&result->col3);
}

static inline void dmathM4MakeRotationY(DmathMatrix4* result, double radians)
{
    double s, c;
    s = sinf(radians);
    c = cosf(radians);
    dmathV4MakeFromElems(&result->col0, c, 0.0f, -s, 0.0f);
    dmathV4MakeYAxis(&result->col1);
    dmathV4MakeFromElems(&result->col2, s, 0.0f, c, 0.0f);
    dmathV4MakeWAxis(&result->col3);
}

static inline void dmathM4MakeRotationZ(DmathMatrix4* result, double radians)
{
    double s, c;
    s = sinf(radians);
    c = cosf(radians);
    dmathV4MakeFromElems(&result->col0, c, s, 0.0f, 0.0f);
    dmathV4MakeFromElems(&result->col1, -s, c, 0.0f, 0.0f);
    dmathV4MakeZAxis(&result->col2);
    dmathV4MakeWAxis(&result->col3);
}

static inline void dmathM4MakeRotationZYX(
    DmathMatrix4* result, const DmathVector3* radiansXYZ)
{
    double sX, cX, sY, cY, sZ, cZ, tmp0, tmp1;
    sX = sinf(radiansXYZ->x);
    cX = cosf(radiansXYZ->x);
    sY = sinf(radiansXYZ->y);
    cY = cosf(radiansXYZ->y);
    sZ = sinf(radiansXYZ->z);
    cZ = cosf(radiansXYZ->z);
    tmp0 = (cZ * sY);
    tmp1 = (sZ * sY);
    dmathV4MakeFromElems(&result->col0, (cZ * cY), (sZ * cY), -sY, 0.0f);
    dmathV4MakeFromElems(&result->col1, ((tmp0 * sX) - (sZ * cX)),
        ((tmp1 * sX) + (cZ * cX)), (cY * sX), 0.0f);
    dmathV4MakeFromElems(&result->col2, ((tmp0 * cX) + (sZ * sX)),
        ((tmp1 * cX) - (cZ * sX)), (cY * cX), 0.0f);
    dmathV4MakeWAxis(&result->col3);
}

static inline void dmathM4MakeRotationAxis(
    DmathMatrix4* result, double radians, const DmathVector3* unitVec)
{
    double x, y, z, s, c, oneMinusC, xy, yz, zx;
    s = sinf(radians);
    c = cosf(radians);
    x = unitVec->x;
    y = unitVec->y;
    z = unitVec->z;
    xy = (x * y);
    yz = (y * z);
    zx = (z * x);
    oneMinusC = (1.0f - c);
    dmathV4MakeFromElems(&result->col0, (((x * x) * oneMinusC) + c),
        ((xy * oneMinusC) + (z * s)), ((zx * oneMinusC) - (y * s)), 0.0f);
    dmathV4MakeFromElems(&result->col1, ((xy * oneMinusC) - (z * s)),
        (((y * y) * oneMinusC) + c), ((yz * oneMinusC) + (x * s)), 0.0f);
    dmathV4MakeFromElems(&result->col2, ((zx * oneMinusC) + (y * s)),
        ((yz * oneMinusC) - (x * s)), (((z * z) * oneMinusC) + c), 0.0f);
    dmathV4MakeWAxis(&result->col3);
}

static inline void dmathM4MakeRotationQ(
    DmathMatrix4* result, const DmathQuat* unitQuat)
{
    DmathTransform3 tmpT3_0;
    dmathT3MakeRotationQ(&tmpT3_0, unitQuat);
    dmathM4MakeFromT3(result, &tmpT3_0);
}

static inline void dmathM4MakeScale(
    DmathMatrix4* result, const DmathVector3* scaleVec)
{
    dmathV4MakeFromElems(&result->col0, scaleVec->x, 0.0f, 0.0f, 0.0f);
    dmathV4MakeFromElems(&result->col1, 0.0f, scaleVec->y, 0.0f, 0.0f);
    dmathV4MakeFromElems(&result->col2, 0.0f, 0.0f, scaleVec->z, 0.0f);
    dmathV4MakeWAxis(&result->col3);
}

static inline void dmathM4AppendScale(
    DmathMatrix4* result, const DmathMatrix4* mat, const DmathVector3* scaleVec)
{
    dmathV4ScalarMul(&result->col0, &mat->col0, dmathV3GetX(scaleVec));
    dmathV4ScalarMul(&result->col1, &mat->col1, dmathV3GetY(scaleVec));
    dmathV4ScalarMul(&result->col2, &mat->col2, dmathV3GetZ(scaleVec));
    dmathV4Copy(&result->col3, &mat->col3);
}

static inline void dmathM4PrependScale(
    DmathMatrix4* result, const DmathVector3* scaleVec, const DmathMatrix4* mat)
{
    DmathVector4 scale4;
    dmathV4MakeFromV3Scalar(&scale4, scaleVec, 1.0f);
    dmathV4MulPerElem(&result->col0, &mat->col0, &scale4);
    dmathV4MulPerElem(&result->col1, &mat->col1, &scale4);
    dmathV4MulPerElem(&result->col2, &mat->col2, &scale4);
    dmathV4MulPerElem(&result->col3, &mat->col3, &scale4);
}

static inline void dmathM4MakeTranslation(
    DmathMatrix4* result, const DmathVector3* translateVec)
{
    dmathV4MakeXAxis(&result->col0);
    dmathV4MakeYAxis(&result->col1);
    dmathV4MakeZAxis(&result->col2);
    dmathV4MakeFromV3Scalar(&result->col3, translateVec, 1.0f);
}

static inline void dmathM4MakeLookAt(DmathMatrix4* result,
    const DmathPoint3* eyePos, const DmathPoint3* lookAtPos,
    const DmathVector3* upVec)
{
    DmathMatrix4 m4EyeFrame;
    DmathVector3 v3X, v3Y, v3Z, tmpV3_0, tmpV3_1;
    DmathVector4 tmpV4_0, tmpV4_1, tmpV4_2, tmpV4_3;
    dmathV3Normalize(&v3Y, upVec);
    dmathP3Sub(&tmpV3_0, eyePos, lookAtPos);
    dmathV3Normalize(&v3Z, &tmpV3_0);
    dmathV3Cross(&tmpV3_1, &v3Y, &v3Z);
    dmathV3Normalize(&v3X, &tmpV3_1);
    dmathV3Cross(&v3Y, &v3Z, &v3X);
    dmathV4MakeFromV3(&tmpV4_0, &v3X);
    dmathV4MakeFromV3(&tmpV4_1, &v3Y);
    dmathV4MakeFromV3(&tmpV4_2, &v3Z);
    dmathV4MakeFromP3(&tmpV4_3, eyePos);
    dmathM4MakeFromCols(&m4EyeFrame, &tmpV4_0, &tmpV4_1, &tmpV4_2, &tmpV4_3);
    dmathM4OrthoInverse(result, &m4EyeFrame);
}

static inline void dmathM4MakePerspective(DmathMatrix4* result,
    double fovyRadians, double aspect, double zNear, double zFar)
{
    double f, rangeInv;
    f = tanf(((double) (_DVECTORMATH_PI_OVER_2) -(0.5f * fovyRadians)));
    rangeInv = (1.0f / (zNear - zFar));
    dmathV4MakeFromElems(&result->col0, (f / aspect), 0.0f, 0.0f, 0.0f);
    dmathV4MakeFromElems(&result->col1, 0.0f, f, 0.0f, 0.0f);
    dmathV4MakeFromElems(
        &result->col2, 0.0f, 0.0f, ((zNear + zFar) * rangeInv), -1.0f);
    dmathV4MakeFromElems(
        &result->col3, 0.0f, 0.0f, (((zNear * zFar) * rangeInv) * 2.0f), 0.0f);
}

static inline void dmathM4MakeFrustum(DmathMatrix4* result, double left,
    double right, double bottom, double top, double zNear, double zFar)
{
    double sum_rl, sum_tb, sum_nf, inv_rl, inv_tb, inv_nf, n2;
    sum_rl = (right + left);
    sum_tb = (top + bottom);
    sum_nf = (zNear + zFar);
    inv_rl = (1.0f / (right - left));
    inv_tb = (1.0f / (top - bottom));
    inv_nf = (1.0f / (zNear - zFar));
    n2 = (zNear + zNear);
    dmathV4MakeFromElems(&result->col0, (n2 * inv_rl), 0.0f, 0.0f, 0.0f);
    dmathV4MakeFromElems(&result->col1, 0.0f, (n2 * inv_tb), 0.0f, 0.0f);
    dmathV4MakeFromElems(&result->col2, (sum_rl * inv_rl), (sum_tb * inv_tb),
        (sum_nf * inv_nf), -1.0f);
    dmathV4MakeFromElems(
        &result->col3, 0.0f, 0.0f, ((n2 * inv_nf) * zFar), 0.0f);
}

static inline void dmathM4MakeOrthographic(DmathMatrix4* result, double left,
    double right, double bottom, double top, double zNear, double zFar)
{
    double sum_rl, sum_tb, sum_nf, inv_rl, inv_tb, inv_nf;
    sum_rl = (right + left);
    sum_tb = (top + bottom);
    sum_nf = (zNear + zFar);
    inv_rl = (1.0f / (right - left));
    inv_tb = (1.0f / (top - bottom));
    inv_nf = (1.0f / (zNear - zFar));
    dmathV4MakeFromElems(&result->col0, (inv_rl + inv_rl), 0.0f, 0.0f, 0.0f);
    dmathV4MakeFromElems(&result->col1, 0.0f, (inv_tb + inv_tb), 0.0f, 0.0f);
    dmathV4MakeFromElems(&result->col2, 0.0f, 0.0f, (inv_nf + inv_nf), 0.0f);
    dmathV4MakeFromElems(&result->col3, (-sum_rl * inv_rl), (-sum_tb * inv_tb),
        (sum_nf * inv_nf), 1.0f);
}

static inline void dmathM4Select(DmathMatrix4* result, const DmathMatrix4* mat0,
    const DmathMatrix4* mat1, unsigned int select1)
{
    dmathV4Select(&result->col0, &mat0->col0, &mat1->col0, select1);
    dmathV4Select(&result->col1, &mat0->col1, &mat1->col1, select1);
    dmathV4Select(&result->col2, &mat0->col2, &mat1->col2, select1);
    dmathV4Select(&result->col3, &mat0->col3, &mat1->col3, select1);
}

#ifdef _DVECTORMATH_DEBUG

static inline void dmathM4Print(const DmathMatrix4* mat)
{
    DmathVector4 tmpV4_0, tmpV4_1, tmpV4_2, tmpV4_3;
    dmathM4GetRow(&tmpV4_0, mat, 0);
    dmathV4Print(&tmpV4_0);
    dmathM4GetRow(&tmpV4_1, mat, 1);
    dmathV4Print(&tmpV4_1);
    dmathM4GetRow(&tmpV4_2, mat, 2);
    dmathV4Print(&tmpV4_2);
    dmathM4GetRow(&tmpV4_3, mat, 3);
    dmathV4Print(&tmpV4_3);
}

static inline void dmathM4Prints(const DmathMatrix4* mat, const char* name)
{
    printf("%s:\n", name);
    dmathM4Print(mat);
}

#endif

static inline void dmathT3Copy(
    DmathTransform3* result, const DmathTransform3* tfrm)
{
    dmathV3Copy(&result->col0, &tfrm->col0);
    dmathV3Copy(&result->col1, &tfrm->col1);
    dmathV3Copy(&result->col2, &tfrm->col2);
    dmathV3Copy(&result->col3, &tfrm->col3);
}

static inline void dmathT3MakeFromScalar(DmathTransform3* result, double scalar)
{
    dmathV3MakeFromScalar(&result->col0, scalar);
    dmathV3MakeFromScalar(&result->col1, scalar);
    dmathV3MakeFromScalar(&result->col2, scalar);
    dmathV3MakeFromScalar(&result->col3, scalar);
}

static inline void dmathT3MakeFromCols(DmathTransform3* result,
    const DmathVector3* _col0, const DmathVector3* _col1,
    const DmathVector3* _col2, const DmathVector3* _col3)
{
    dmathV3Copy(&result->col0, _col0);
    dmathV3Copy(&result->col1, _col1);
    dmathV3Copy(&result->col2, _col2);
    dmathV3Copy(&result->col3, _col3);
}

static inline void dmathT3MakeFromM3V3(DmathTransform3* result,
    const DmathMatrix3* tfrm, const DmathVector3* translateVec)
{
    dmathT3SetUpper3x3(result, tfrm);
    dmathT3SetTranslation(result, translateVec);
}

static inline void dmathT3MakeFromQV3(DmathTransform3* result,
    const DmathQuat* unitQuat, const DmathVector3* translateVec)
{
    DmathMatrix3 tmpM3_0;
    dmathM3MakeFromQ(&tmpM3_0, unitQuat);
    dmathT3SetUpper3x3(result, &tmpM3_0);
    dmathT3SetTranslation(result, translateVec);
}

static inline void dmathT3SetCol0(
    DmathTransform3* result, const DmathVector3* _col0)
{
    dmathV3Copy(&result->col0, _col0);
}

static inline void dmathT3SetCol1(
    DmathTransform3* result, const DmathVector3* _col1)
{
    dmathV3Copy(&result->col1, _col1);
}

static inline void dmathT3SetCol2(
    DmathTransform3* result, const DmathVector3* _col2)
{
    dmathV3Copy(&result->col2, _col2);
}

static inline void dmathT3SetCol3(
    DmathTransform3* result, const DmathVector3* _col3)
{
    dmathV3Copy(&result->col3, _col3);
}

static inline void dmathT3SetCol(
    DmathTransform3* result, int col, const DmathVector3* vec)
{
    dmathV3Copy((&result->col0 + col), vec);
}

static inline void dmathT3SetRow(
    DmathTransform3* result, int row, const DmathVector4* vec)
{
    dmathV3SetElem(&result->col0, row, dmathV4GetElem(vec, 0));
    dmathV3SetElem(&result->col1, row, dmathV4GetElem(vec, 1));
    dmathV3SetElem(&result->col2, row, dmathV4GetElem(vec, 2));
    dmathV3SetElem(&result->col3, row, dmathV4GetElem(vec, 3));
}

static inline void dmathT3SetElem(
    DmathTransform3* result, int col, int row, double val)
{
    DmathVector3 tmpV3_0;
    dmathT3GetCol(&tmpV3_0, result, col);
    dmathV3SetElem(&tmpV3_0, row, val);
    dmathT3SetCol(result, col, &tmpV3_0);
}

static inline double dmathT3GetElem(
    const DmathTransform3* tfrm, int col, int row)
{
    DmathVector3 tmpV3_0;
    dmathT3GetCol(&tmpV3_0, tfrm, col);
    return dmathV3GetElem(&tmpV3_0, row);
}

static inline void dmathT3GetCol0(
    DmathVector3* result, const DmathTransform3* tfrm)
{
    dmathV3Copy(result, &tfrm->col0);
}

static inline void dmathT3GetCol1(
    DmathVector3* result, const DmathTransform3* tfrm)
{
    dmathV3Copy(result, &tfrm->col1);
}

static inline void dmathT3GetCol2(
    DmathVector3* result, const DmathTransform3* tfrm)
{
    dmathV3Copy(result, &tfrm->col2);
}

static inline void dmathT3GetCol3(
    DmathVector3* result, const DmathTransform3* tfrm)
{
    dmathV3Copy(result, &tfrm->col3);
}

static inline void dmathT3GetCol(
    DmathVector3* result, const DmathTransform3* tfrm, int col)
{
    dmathV3Copy(result, (&tfrm->col0 + col));
}

static inline void dmathT3GetRow(
    DmathVector4* result, const DmathTransform3* tfrm, int row)
{
    dmathV4MakeFromElems(result, dmathV3GetElem(&tfrm->col0, row),
        dmathV3GetElem(&tfrm->col1, row), dmathV3GetElem(&tfrm->col2, row),
        dmathV3GetElem(&tfrm->col3, row));
}

static inline void dmathT3Inverse(
    DmathTransform3* result, const DmathTransform3* tfrm)
{
    DmathVector3 tmp0, tmp1, tmp2, inv0, inv1, inv2, tmpV3_0, tmpV3_1, tmpV3_2,
        tmpV3_3, tmpV3_4, tmpV3_5;
    double detinv;
    dmathV3Cross(&tmp0, &tfrm->col1, &tfrm->col2);
    dmathV3Cross(&tmp1, &tfrm->col2, &tfrm->col0);
    dmathV3Cross(&tmp2, &tfrm->col0, &tfrm->col1);
    detinv = (1.0f / dmathV3Dot(&tfrm->col2, &tmp2));
    dmathV3MakeFromElems(
        &inv0, (tmp0.x * detinv), (tmp1.x * detinv), (tmp2.x * detinv));
    dmathV3MakeFromElems(
        &inv1, (tmp0.y * detinv), (tmp1.y * detinv), (tmp2.y * detinv));
    dmathV3MakeFromElems(
        &inv2, (tmp0.z * detinv), (tmp1.z * detinv), (tmp2.z * detinv));
    dmathV3Copy(&result->col0, &inv0);
    dmathV3Copy(&result->col1, &inv1);
    dmathV3Copy(&result->col2, &inv2);
    dmathV3ScalarMul(&tmpV3_0, &inv0, tfrm->col3.x);
    dmathV3ScalarMul(&tmpV3_1, &inv1, tfrm->col3.y);
    dmathV3ScalarMul(&tmpV3_2, &inv2, tfrm->col3.z);
    dmathV3Add(&tmpV3_3, &tmpV3_1, &tmpV3_2);
    dmathV3Add(&tmpV3_4, &tmpV3_0, &tmpV3_3);
    dmathV3Neg(&tmpV3_5, &tmpV3_4);
    dmathV3Copy(&result->col3, &tmpV3_5);
}

static inline void dmathT3OrthoInverse(
    DmathTransform3* result, const DmathTransform3* tfrm)
{
    DmathVector3 inv0, inv1, inv2, tmpV3_0, tmpV3_1, tmpV3_2, tmpV3_3, tmpV3_4,
        tmpV3_5;
    dmathV3MakeFromElems(&inv0, tfrm->col0.x, tfrm->col1.x, tfrm->col2.x);
    dmathV3MakeFromElems(&inv1, tfrm->col0.y, tfrm->col1.y, tfrm->col2.y);
    dmathV3MakeFromElems(&inv2, tfrm->col0.z, tfrm->col1.z, tfrm->col2.z);
    dmathV3Copy(&result->col0, &inv0);
    dmathV3Copy(&result->col1, &inv1);
    dmathV3Copy(&result->col2, &inv2);
    dmathV3ScalarMul(&tmpV3_0, &inv0, tfrm->col3.x);
    dmathV3ScalarMul(&tmpV3_1, &inv1, tfrm->col3.y);
    dmathV3ScalarMul(&tmpV3_2, &inv2, tfrm->col3.z);
    dmathV3Add(&tmpV3_3, &tmpV3_1, &tmpV3_2);
    dmathV3Add(&tmpV3_4, &tmpV3_0, &tmpV3_3);
    dmathV3Neg(&tmpV3_5, &tmpV3_4);
    dmathV3Copy(&result->col3, &tmpV3_5);
}

static inline void dmathT3AbsPerElem(
    DmathTransform3* result, const DmathTransform3* tfrm)
{
    dmathV3AbsPerElem(&result->col0, &tfrm->col0);
    dmathV3AbsPerElem(&result->col1, &tfrm->col1);
    dmathV3AbsPerElem(&result->col2, &tfrm->col2);
    dmathV3AbsPerElem(&result->col3, &tfrm->col3);
}

static inline void dmathT3MulV3(
    DmathVector3* result, const DmathTransform3* tfrm, const DmathVector3* vec)
{
    double tmpX, tmpY, tmpZ;
    tmpX = (((tfrm->col0.x * vec->x) + (tfrm->col1.x * vec->y)) +
        (tfrm->col2.x * vec->z));
    tmpY = (((tfrm->col0.y * vec->x) + (tfrm->col1.y * vec->y)) +
        (tfrm->col2.y * vec->z));
    tmpZ = (((tfrm->col0.z * vec->x) + (tfrm->col1.z * vec->y)) +
        (tfrm->col2.z * vec->z));
    dmathV3MakeFromElems(result, tmpX, tmpY, tmpZ);
}

static inline void dmathT3MulP3(
    DmathPoint3* result, const DmathTransform3* tfrm, const DmathPoint3* pnt)
{
    double tmpX, tmpY, tmpZ;
    tmpX = ((((tfrm->col0.x * pnt->x) + (tfrm->col1.x * pnt->y)) +
        (tfrm->col2.x * pnt->z)) +
        tfrm->col3.x);
    tmpY = ((((tfrm->col0.y * pnt->x) + (tfrm->col1.y * pnt->y)) +
        (tfrm->col2.y * pnt->z)) +
        tfrm->col3.y);
    tmpZ = ((((tfrm->col0.z * pnt->x) + (tfrm->col1.z * pnt->y)) +
        (tfrm->col2.z * pnt->z)) +
        tfrm->col3.z);
    dmathP3MakeFromElems(result, tmpX, tmpY, tmpZ);
}

static inline void dmathT3Mul(DmathTransform3* result,
    const DmathTransform3* tfrm0, const DmathTransform3* tfrm1)
{
    DmathTransform3 tmpResult;
    DmathPoint3 tmpP3_0, tmpP3_1;
    dmathT3MulV3(&tmpResult.col0, tfrm0, &tfrm1->col0);
    dmathT3MulV3(&tmpResult.col1, tfrm0, &tfrm1->col1);
    dmathT3MulV3(&tmpResult.col2, tfrm0, &tfrm1->col2);
    dmathP3MakeFromV3(&tmpP3_0, &tfrm1->col3);
    dmathT3MulP3(&tmpP3_1, tfrm0, &tmpP3_0);
    dmathV3MakeFromP3(&tmpResult.col3, &tmpP3_1);
    dmathT3Copy(result, &tmpResult);
}

static inline void dmathT3MulPerElem(DmathTransform3* result,
    const DmathTransform3* tfrm0, const DmathTransform3* tfrm1)
{
    dmathV3MulPerElem(&result->col0, &tfrm0->col0, &tfrm1->col0);
    dmathV3MulPerElem(&result->col1, &tfrm0->col1, &tfrm1->col1);
    dmathV3MulPerElem(&result->col2, &tfrm0->col2, &tfrm1->col2);
    dmathV3MulPerElem(&result->col3, &tfrm0->col3, &tfrm1->col3);
}

static inline void dmathT3MakeIdentity(DmathTransform3* result)
{
    dmathV3MakeXAxis(&result->col0);
    dmathV3MakeYAxis(&result->col1);
    dmathV3MakeZAxis(&result->col2);
    dmathV3MakeFromScalar(&result->col3, 0.0f);
}

static inline void dmathT3SetUpper3x3(
    DmathTransform3* result, const DmathMatrix3* tfrm)
{
    dmathV3Copy(&result->col0, &tfrm->col0);
    dmathV3Copy(&result->col1, &tfrm->col1);
    dmathV3Copy(&result->col2, &tfrm->col2);
}

static inline void dmathT3GetUpper3x3(
    DmathMatrix3* result, const DmathTransform3* tfrm)
{
    dmathM3MakeFromCols(result, &tfrm->col0, &tfrm->col1, &tfrm->col2);
}

static inline void dmathT3SetTranslation(
    DmathTransform3* result, const DmathVector3* translateVec)
{
    dmathV3Copy(&result->col3, translateVec);
}

static inline void dmathT3GetTranslation(
    DmathVector3* result, const DmathTransform3* tfrm)
{
    dmathV3Copy(result, &tfrm->col3);
}

static inline void dmathT3MakeRotationX(DmathTransform3* result, double radians)
{
    double s, c;
    s = sinf(radians);
    c = cosf(radians);
    dmathV3MakeXAxis(&result->col0);
    dmathV3MakeFromElems(&result->col1, 0.0f, c, s);
    dmathV3MakeFromElems(&result->col2, 0.0f, -s, c);
    dmathV3MakeFromScalar(&result->col3, 0.0f);
}

static inline void dmathT3MakeRotationY(DmathTransform3* result, double radians)
{
    double s, c;
    s = sinf(radians);
    c = cosf(radians);
    dmathV3MakeFromElems(&result->col0, c, 0.0f, -s);
    dmathV3MakeYAxis(&result->col1);
    dmathV3MakeFromElems(&result->col2, s, 0.0f, c);
    dmathV3MakeFromScalar(&result->col3, 0.0f);
}

static inline void dmathT3MakeRotationZ(DmathTransform3* result, double radians)
{
    double s, c;
    s = sinf(radians);
    c = cosf(radians);
    dmathV3MakeFromElems(&result->col0, c, s, 0.0f);
    dmathV3MakeFromElems(&result->col1, -s, c, 0.0f);
    dmathV3MakeZAxis(&result->col2);
    dmathV3MakeFromScalar(&result->col3, 0.0f);
}

static inline void dmathT3MakeRotationZYX(
    DmathTransform3* result, const DmathVector3* radiansXYZ)
{
    double sX, cX, sY, cY, sZ, cZ, tmp0, tmp1;
    sX = sinf(radiansXYZ->x);
    cX = cosf(radiansXYZ->x);
    sY = sinf(radiansXYZ->y);
    cY = cosf(radiansXYZ->y);
    sZ = sinf(radiansXYZ->z);
    cZ = cosf(radiansXYZ->z);
    tmp0 = (cZ * sY);
    tmp1 = (sZ * sY);
    dmathV3MakeFromElems(&result->col0, (cZ * cY), (sZ * cY), -sY);
    dmathV3MakeFromElems(&result->col1, ((tmp0 * sX) - (sZ * cX)),
        ((tmp1 * sX) + (cZ * cX)), (cY * sX));
    dmathV3MakeFromElems(&result->col2, ((tmp0 * cX) + (sZ * sX)),
        ((tmp1 * cX) - (cZ * sX)), (cY * cX));
    dmathV3MakeFromScalar(&result->col3, 0.0f);
}

static inline void dmathT3MakeRotationAxis(
    DmathTransform3* result, double radians, const DmathVector3* unitVec)
{
    DmathMatrix3 tmpM3_0;
    DmathVector3 tmpV3_0;
    dmathM3MakeRotationAxis(&tmpM3_0, radians, unitVec);
    dmathV3MakeFromScalar(&tmpV3_0, 0.0f);
    dmathT3MakeFromM3V3(result, &tmpM3_0, &tmpV3_0);
}

static inline void dmathT3MakeRotationQ(
    DmathTransform3* result, const DmathQuat* unitQuat)
{
    DmathMatrix3 tmpM3_0;
    DmathVector3 tmpV3_0;
    dmathM3MakeFromQ(&tmpM3_0, unitQuat);
    dmathV3MakeFromScalar(&tmpV3_0, 0.0f);
    dmathT3MakeFromM3V3(result, &tmpM3_0, &tmpV3_0);
}

static inline void dmathT3MakeScale(
    DmathTransform3* result, const DmathVector3* scaleVec)
{
    dmathV3MakeFromElems(&result->col0, scaleVec->x, 0.0f, 0.0f);
    dmathV3MakeFromElems(&result->col1, 0.0f, scaleVec->y, 0.0f);
    dmathV3MakeFromElems(&result->col2, 0.0f, 0.0f, scaleVec->z);
    dmathV3MakeFromScalar(&result->col3, 0.0f);
}

static inline void dmathT3AppendScale(DmathTransform3* result,
    const DmathTransform3* tfrm, const DmathVector3* scaleVec)
{
    dmathV3ScalarMul(&result->col0, &tfrm->col0, dmathV3GetX(scaleVec));
    dmathV3ScalarMul(&result->col1, &tfrm->col1, dmathV3GetY(scaleVec));
    dmathV3ScalarMul(&result->col2, &tfrm->col2, dmathV3GetZ(scaleVec));
    dmathV3Copy(&result->col3, &tfrm->col3);
}

static inline void dmathT3PrependScale(DmathTransform3* result,
    const DmathVector3* scaleVec, const DmathTransform3* tfrm)
{
    dmathV3MulPerElem(&result->col0, &tfrm->col0, scaleVec);
    dmathV3MulPerElem(&result->col1, &tfrm->col1, scaleVec);
    dmathV3MulPerElem(&result->col2, &tfrm->col2, scaleVec);
    dmathV3MulPerElem(&result->col3, &tfrm->col3, scaleVec);
}

static inline void dmathT3MakeTranslation(
    DmathTransform3* result, const DmathVector3* translateVec)
{
    dmathV3MakeXAxis(&result->col0);
    dmathV3MakeYAxis(&result->col1);
    dmathV3MakeZAxis(&result->col2);
    dmathV3Copy(&result->col3, translateVec);
}

static inline void dmathT3Select(DmathTransform3* result,
    const DmathTransform3* tfrm0, const DmathTransform3* tfrm1,
    unsigned int select1)
{
    dmathV3Select(&result->col0, &tfrm0->col0, &tfrm1->col0, select1);
    dmathV3Select(&result->col1, &tfrm0->col1, &tfrm1->col1, select1);
    dmathV3Select(&result->col2, &tfrm0->col2, &tfrm1->col2, select1);
    dmathV3Select(&result->col3, &tfrm0->col3, &tfrm1->col3, select1);
}

#ifdef _DVECTORMATH_DEBUG

static inline void dmathT3Print(const DmathTransform3* tfrm)
{
    DmathVector4 tmpV4_0, tmpV4_1, tmpV4_2;
    dmathT3GetRow(&tmpV4_0, tfrm, 0);
    dmathV4Print(&tmpV4_0);
    dmathT3GetRow(&tmpV4_1, tfrm, 1);
    dmathV4Print(&tmpV4_1);
    dmathT3GetRow(&tmpV4_2, tfrm, 2);
    dmathV4Print(&tmpV4_2);
}

static inline void dmathT3Prints(const DmathTransform3* tfrm, const char* name)
{
    printf("%s:\n", name);
    dmathT3Print(tfrm);
}

#endif

static inline void dmathQMakeFromM3(DmathQuat* result, const DmathMatrix3* tfrm)
{
    double trace, radicand, scale, xx, yx, zx, xy, yy, zy, xz, yz, zz, tmpx,
        tmpy, tmpz, tmpw, qx, qy, qz, qw;
    int negTrace, ZgtX, ZgtY, YgtX;
    int largestXorY, largestYorZ, largestZorX;

    xx = tfrm->col0.x;
    yx = tfrm->col0.y;
    zx = tfrm->col0.z;
    xy = tfrm->col1.x;
    yy = tfrm->col1.y;
    zy = tfrm->col1.z;
    xz = tfrm->col2.x;
    yz = tfrm->col2.y;
    zz = tfrm->col2.z;

    trace = ((xx + yy) + zz);

    negTrace = (trace < 0.0f);
    ZgtX = zz > xx;
    ZgtY = zz > yy;
    YgtX = yy > xx;
    largestXorY = (!ZgtX || !ZgtY) && negTrace;
    largestYorZ = (YgtX || ZgtX) && negTrace;
    largestZorX = (ZgtY || !YgtX) && negTrace;

    if (largestXorY) {
        zz = -zz;
        xy = -xy;
    }
    if (largestYorZ) {
        xx = -xx;
        yz = -yz;
    }
    if (largestZorX) {
        yy = -yy;
        zx = -zx;
    }

    radicand = (((xx + yy) + zz) + 1.0f);
    scale = (0.5f * (1.0f / sqrtf(radicand)));

    tmpx = ((zy - yz) * scale);
    tmpy = ((xz - zx) * scale);
    tmpz = ((yx - xy) * scale);
    tmpw = (radicand * scale);
    qx = tmpx;
    qy = tmpy;
    qz = tmpz;
    qw = tmpw;

    if (largestXorY) {
        qx = tmpw;
        qy = tmpz;
        qz = tmpy;
        qw = tmpx;
    }
    if (largestYorZ) {
        tmpx = qx;
        tmpz = qz;
        qx = qy;
        qy = tmpx;
        qz = qw;
        qw = tmpz;
    }

    result->x = qx;
    result->y = qy;
    result->z = qz;
    result->w = qw;
}

static inline void dmathV3Outer(
    DmathMatrix3* result, const DmathVector3* tfrm0, const DmathVector3* tfrm1)
{
    dmathV3ScalarMul(&result->col0, tfrm0, dmathV3GetX(tfrm1));
    dmathV3ScalarMul(&result->col1, tfrm0, dmathV3GetY(tfrm1));
    dmathV3ScalarMul(&result->col2, tfrm0, dmathV3GetZ(tfrm1));
}

static inline void dmathV4Outer(
    DmathMatrix4* result, const DmathVector4* tfrm0, const DmathVector4* tfrm1)
{
    dmathV4ScalarMul(&result->col0, tfrm0, dmathV4GetX(tfrm1));
    dmathV4ScalarMul(&result->col1, tfrm0, dmathV4GetY(tfrm1));
    dmathV4ScalarMul(&result->col2, tfrm0, dmathV4GetZ(tfrm1));
    dmathV4ScalarMul(&result->col3, tfrm0, dmathV4GetW(tfrm1));
}

static inline void dmathV3RowMul(
    DmathVector3* result, const DmathVector3* vec, const DmathMatrix3* mat)
{
    double tmpX, tmpY, tmpZ;
    tmpX = (((vec->x * mat->col0.x) + (vec->y * mat->col0.y)) +
        (vec->z * mat->col0.z));
    tmpY = (((vec->x * mat->col1.x) + (vec->y * mat->col1.y)) +
        (vec->z * mat->col1.z));
    tmpZ = (((vec->x * mat->col2.x) + (vec->y * mat->col2.y)) +
        (vec->z * mat->col2.z));
    dmathV3MakeFromElems(result, tmpX, tmpY, tmpZ);
}

static inline void dmathV3CrossMatrix(
    DmathMatrix3* result, const DmathVector3* vec)
{
    dmathV3MakeFromElems(&result->col0, 0.0f, vec->z, -vec->y);
    dmathV3MakeFromElems(&result->col1, -vec->z, 0.0f, vec->x);
    dmathV3MakeFromElems(&result->col2, vec->y, -vec->x, 0.0f);
}

static inline void dmathV3CrossMatrixMul(
    DmathMatrix3* result, const DmathVector3* vec, const DmathMatrix3* mat)
{
    DmathVector3 tmpV3_0, tmpV3_1, tmpV3_2;
    dmathV3Cross(&tmpV3_0, vec, &mat->col0);
    dmathV3Cross(&tmpV3_1, vec, &mat->col1);
    dmathV3Cross(&tmpV3_2, vec, &mat->col2);
    dmathM3MakeFromCols(result, &tmpV3_0, &tmpV3_1, &tmpV3_2);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

#endif

#endif

/*-----------------------------------------------------------------------------
 * Constants
 */
#define _DVECTORMATH_SLERP_TOL 0.999f

/*-----------------------------------------------------------------------------
 * Definitions
 */
#ifndef _DVECTORMATH_INTERNAL_FUNCTIONS
#define _DVECTORMATH_INTERNAL_FUNCTIONS

#endif

static inline DmathVector3 dmathV3MakeFromElems_V(double _x, double _y, double _z)
{
    DmathVector3 result;
    dmathV3MakeFromElems(&result, _x, _y, _z);
    return result;
}

static inline DmathVector3 dmathV3MakeFromP3_V(DmathPoint3 pnt)
{
    DmathVector3 result;
    dmathV3MakeFromP3(&result, &pnt);
    return result;
}

static inline DmathVector3 dmathV3MakeFromScalar_V(double scalar)
{
    DmathVector3 result;
    dmathV3MakeFromScalar(&result, scalar);
    return result;
}

static inline DmathVector3 dmathV3MakeXAxis_V()
{
    DmathVector3 result;
    dmathV3MakeXAxis(&result);
    return result;
}

static inline DmathVector3 dmathV3MakeYAxis_V()
{
    DmathVector3 result;
    dmathV3MakeYAxis(&result);
    return result;
}

static inline DmathVector3 dmathV3MakeZAxis_V()
{
    DmathVector3 result;
    dmathV3MakeZAxis(&result);
    return result;
}

static inline DmathVector3 dmathV3Lerp_V(
    double t, DmathVector3 vec0, DmathVector3 vec1)
{
    DmathVector3 result;
    dmathV3Lerp(&result, t, &vec0, &vec1);
    return result;
}

static inline DmathVector3 dmathV3Slerp_V(
    double t, DmathVector3 unitVec0, DmathVector3 unitVec1)
{
    DmathVector3 result;
    dmathV3Slerp(&result, t, &unitVec0, &unitVec1);
    return result;
}

static inline void dmathV3SetX_V(DmathVector3* result, double _x)
{
    dmathV3SetX(result, _x);
}

static inline double dmathV3GetX_V(DmathVector3 vec)
{
    return dmathV3GetX(&vec);
}

static inline void dmathV3SetY_V(DmathVector3* result, double _y)
{
    dmathV3SetY(result, _y);
}

static inline double dmathV3GetY_V(DmathVector3 vec)
{
    return dmathV3GetY(&vec);
}

static inline void dmathV3SetZ_V(DmathVector3* result, double _z)
{
    dmathV3SetZ(result, _z);
}

static inline double dmathV3GetZ_V(DmathVector3 vec)
{
    return dmathV3GetZ(&vec);
}

static inline void dmathV3SetElem_V(DmathVector3* result, int idx, double value)
{
    dmathV3SetElem(result, idx, value);
}

static inline double dmathV3GetElem_V(DmathVector3 vec, int idx)
{
    return dmathV3GetElem(&vec, idx);
}

static inline DmathVector3 dmathV3Add_V(DmathVector3 vec0, DmathVector3 vec1)
{
    DmathVector3 result;
    dmathV3Add(&result, &vec0, &vec1);
    return result;
}

static inline DmathVector3 dmathV3Sub_V(DmathVector3 vec0, DmathVector3 vec1)
{
    DmathVector3 result;
    dmathV3Sub(&result, &vec0, &vec1);
    return result;
}

static inline DmathPoint3 dmathV3AddP3_V(DmathVector3 vec, DmathPoint3 pnt1)
{
    DmathPoint3 result;
    dmathV3AddP3(&result, &vec, &pnt1);
    return result;
}

static inline DmathVector3 dmathV3ScalarMul_V(DmathVector3 vec, double scalar)
{
    DmathVector3 result;
    dmathV3ScalarMul(&result, &vec, scalar);
    return result;
}

static inline DmathVector3 dmathV3ScalarDiv_V(DmathVector3 vec, double scalar)
{
    DmathVector3 result;
    dmathV3ScalarDiv(&result, &vec, scalar);
    return result;
}

static inline DmathVector3 dmathV3Neg_V(DmathVector3 vec)
{
    DmathVector3 result;
    dmathV3Neg(&result, &vec);
    return result;
}

static inline DmathVector3 dmathV3MulPerElem_V(
    DmathVector3 vec0, DmathVector3 vec1)
{
    DmathVector3 result;
    dmathV3MulPerElem(&result, &vec0, &vec1);
    return result;
}

static inline DmathVector3 dmathV3DivPerElem_V(
    DmathVector3 vec0, DmathVector3 vec1)
{
    DmathVector3 result;
    dmathV3DivPerElem(&result, &vec0, &vec1);
    return result;
}

static inline DmathVector3 dmathV3RecipPerElem_V(DmathVector3 vec)
{
    DmathVector3 result;
    dmathV3RecipPerElem(&result, &vec);
    return result;
}

static inline DmathVector3 dmathV3SqrtPerElem_V(DmathVector3 vec)
{
    DmathVector3 result;
    dmathV3SqrtPerElem(&result, &vec);
    return result;
}

static inline DmathVector3 dmathV3RsqrtPerElem_V(DmathVector3 vec)
{
    DmathVector3 result;
    dmathV3RsqrtPerElem(&result, &vec);
    return result;
}

static inline DmathVector3 dmathV3AbsPerElem_V(DmathVector3 vec)
{
    DmathVector3 result;
    dmathV3AbsPerElem(&result, &vec);
    return result;
}

static inline DmathVector3 dmathV3CopySignPerElem_V(
    DmathVector3 vec0, DmathVector3 vec1)
{
    DmathVector3 result;
    dmathV3CopySignPerElem(&result, &vec0, &vec1);
    return result;
}

static inline DmathVector3 dmathV3MaxPerElem_V(
    DmathVector3 vec0, DmathVector3 vec1)
{
    DmathVector3 result;
    dmathV3MaxPerElem(&result, &vec0, &vec1);
    return result;
}

static inline double dmathV3MaxElem_V(DmathVector3 vec)
{
    return dmathV3MaxElem(&vec);
}

static inline DmathVector3 dmathV3MinPerElem_V(
    DmathVector3 vec0, DmathVector3 vec1)
{
    DmathVector3 result;
    dmathV3MinPerElem(&result, &vec0, &vec1);
    return result;
}

static inline double dmathV3MinElem_V(DmathVector3 vec)
{
    return dmathV3MinElem(&vec);
}

static inline double dmathV3Sum_V(DmathVector3 vec) { return dmathV3Sum(&vec); }

static inline double dmathV3Dot_V(DmathVector3 vec0, DmathVector3 vec1)
{
    return dmathV3Dot(&vec0, &vec1);
}

static inline double dmathV3LengthSqr_V(DmathVector3 vec)
{
    return dmathV3LengthSqr(&vec);
}

static inline double dmathV3Length_V(DmathVector3 vec)
{
    return dmathV3Length(&vec);
}

static inline DmathVector3 dmathV3Normalize_V(DmathVector3 vec)
{
    DmathVector3 result;
    dmathV3Normalize(&result, &vec);
    return result;
}

static inline DmathVector3 dmathV3Cross_V(DmathVector3 vec0, DmathVector3 vec1)
{
    DmathVector3 result;
    dmathV3Cross(&result, &vec0, &vec1);
    return result;
}

static inline DmathVector3 dmathV3Select_V(
    DmathVector3 vec0, DmathVector3 vec1, unsigned int select1)
{
    DmathVector3 result;
    dmathV3Select(&result, &vec0, &vec1, select1);
    return result;
}

#ifdef _DVECTORMATH_DEBUG

static inline void dmathV3Print_V(DmathVector3 vec) { dmathV3Print(&vec); }

static inline void dmathV3Prints_V(DmathVector3 vec, const char* name)
{
    dmathV3Prints(&vec, name);
}

#endif

static inline DmathVector4 dmathV4MakeFromElems_V(
    double _x, double _y, double _z, double _w)
{
    DmathVector4 result;
    dmathV4MakeFromElems(&result, _x, _y, _z, _w);
    return result;
}

static inline DmathVector4 dmathV4MakeFromV3Scalar_V(DmathVector3 xyz, double _w)
{
    DmathVector4 result;
    dmathV4MakeFromV3Scalar(&result, &xyz, _w);
    return result;
}

static inline DmathVector4 dmathV4MakeFromV3_V(DmathVector3 vec)
{
    DmathVector4 result;
    dmathV4MakeFromV3(&result, &vec);
    return result;
}

static inline DmathVector4 dmathV4MakeFromP3_V(DmathPoint3 pnt)
{
    DmathVector4 result;
    dmathV4MakeFromP3(&result, &pnt);
    return result;
}

static inline DmathVector4 dmathV4MakeFromQ_V(DmathQuat quat)
{
    DmathVector4 result;
    dmathV4MakeFromQ(&result, &quat);
    return result;
}

static inline DmathVector4 dmathV4MakeFromScalar_V(double scalar)
{
    DmathVector4 result;
    dmathV4MakeFromScalar(&result, scalar);
    return result;
}

static inline DmathVector4 dmathV4MakeXAxis_V()
{
    DmathVector4 result;
    dmathV4MakeXAxis(&result);
    return result;
}

static inline DmathVector4 dmathV4MakeYAxis_V()
{
    DmathVector4 result;
    dmathV4MakeYAxis(&result);
    return result;
}

static inline DmathVector4 dmathV4MakeZAxis_V()
{
    DmathVector4 result;
    dmathV4MakeZAxis(&result);
    return result;
}

static inline DmathVector4 dmathV4MakeWAxis_V()
{
    DmathVector4 result;
    dmathV4MakeWAxis(&result);
    return result;
}

static inline DmathVector4 dmathV4Lerp_V(
    double t, DmathVector4 vec0, DmathVector4 vec1)
{
    DmathVector4 result;
    dmathV4Lerp(&result, t, &vec0, &vec1);
    return result;
}

static inline DmathVector4 dmathV4Slerp_V(
    double t, DmathVector4 unitVec0, DmathVector4 unitVec1)
{
    DmathVector4 result;
    dmathV4Slerp(&result, t, &unitVec0, &unitVec1);
    return result;
}

static inline void dmathV4SetXYZ_V(DmathVector4* result, DmathVector3 vec)
{
    dmathV4SetXYZ(result, &vec);
}

static inline DmathVector3 dmathV4GetXYZ_V(DmathVector4 vec)
{
    DmathVector3 result;
    dmathV4GetXYZ(&result, &vec);
    return result;
}

static inline void dmathV4SetX_V(DmathVector4* result, double _x)
{
    dmathV4SetX(result, _x);
}

static inline double dmathV4GetX_V(DmathVector4 vec)
{
    return dmathV4GetX(&vec);
}

static inline void dmathV4SetY_V(DmathVector4* result, double _y)
{
    dmathV4SetY(result, _y);
}

static inline double dmathV4GetY_V(DmathVector4 vec)
{
    return dmathV4GetY(&vec);
}

static inline void dmathV4SetZ_V(DmathVector4* result, double _z)
{
    dmathV4SetZ(result, _z);
}

static inline double dmathV4GetZ_V(DmathVector4 vec)
{
    return dmathV4GetZ(&vec);
}

static inline void dmathV4SetW_V(DmathVector4* result, double _w)
{
    dmathV4SetW(result, _w);
}

static inline double dmathV4GetW_V(DmathVector4 vec)
{
    return dmathV4GetW(&vec);
}

static inline void dmathV4SetElem_V(DmathVector4* result, int idx, double value)
{
    dmathV4SetElem(result, idx, value);
}

static inline double dmathV4GetElem_V(DmathVector4 vec, int idx)
{
    return dmathV4GetElem(&vec, idx);
}

static inline DmathVector4 dmathV4Add_V(DmathVector4 vec0, DmathVector4 vec1)
{
    DmathVector4 result;
    dmathV4Add(&result, &vec0, &vec1);
    return result;
}

static inline DmathVector4 dmathV4Sub_V(DmathVector4 vec0, DmathVector4 vec1)
{
    DmathVector4 result;
    dmathV4Sub(&result, &vec0, &vec1);
    return result;
}

static inline DmathVector4 dmathV4ScalarMul_V(DmathVector4 vec, double scalar)
{
    DmathVector4 result;
    dmathV4ScalarMul(&result, &vec, scalar);
    return result;
}

static inline DmathVector4 dmathV4ScalarDiv_V(DmathVector4 vec, double scalar)
{
    DmathVector4 result;
    dmathV4ScalarDiv(&result, &vec, scalar);
    return result;
}

static inline DmathVector4 dmathV4Neg_V(DmathVector4 vec)
{
    DmathVector4 result;
    dmathV4Neg(&result, &vec);
    return result;
}

static inline DmathVector4 dmathV4MulPerElem_V(
    DmathVector4 vec0, DmathVector4 vec1)
{
    DmathVector4 result;
    dmathV4MulPerElem(&result, &vec0, &vec1);
    return result;
}

static inline DmathVector4 dmathV4DivPerElem_V(
    DmathVector4 vec0, DmathVector4 vec1)
{
    DmathVector4 result;
    dmathV4DivPerElem(&result, &vec0, &vec1);
    return result;
}

static inline DmathVector4 dmathV4RecipPerElem_V(DmathVector4 vec)
{
    DmathVector4 result;
    dmathV4RecipPerElem(&result, &vec);
    return result;
}

static inline DmathVector4 dmathV4SqrtPerElem_V(DmathVector4 vec)
{
    DmathVector4 result;
    dmathV4SqrtPerElem(&result, &vec);
    return result;
}

static inline DmathVector4 dmathV4RsqrtPerElem_V(DmathVector4 vec)
{
    DmathVector4 result;
    dmathV4RsqrtPerElem(&result, &vec);
    return result;
}

static inline DmathVector4 dmathV4AbsPerElem_V(DmathVector4 vec)
{
    DmathVector4 result;
    dmathV4AbsPerElem(&result, &vec);
    return result;
}

static inline DmathVector4 dmathV4CopySignPerElem_V(
    DmathVector4 vec0, DmathVector4 vec1)
{
    DmathVector4 result;
    dmathV4CopySignPerElem(&result, &vec0, &vec1);
    return result;
}

static inline DmathVector4 dmathV4MaxPerElem_V(
    DmathVector4 vec0, DmathVector4 vec1)
{
    DmathVector4 result;
    dmathV4MaxPerElem(&result, &vec0, &vec1);
    return result;
}

static inline double dmathV4MaxElem_V(DmathVector4 vec)
{
    return dmathV4MaxElem(&vec);
}

static inline DmathVector4 dmathV4MinPerElem_V(
    DmathVector4 vec0, DmathVector4 vec1)
{
    DmathVector4 result;
    dmathV4MinPerElem(&result, &vec0, &vec1);
    return result;
}

static inline double dmathV4MinElem_V(DmathVector4 vec)
{
    return dmathV4MinElem(&vec);
}

static inline double dmathV4Sum_V(DmathVector4 vec) { return dmathV4Sum(&vec); }

static inline double dmathV4Dot_V(DmathVector4 vec0, DmathVector4 vec1)
{
    return dmathV4Dot(&vec0, &vec1);
}

static inline double dmathV4LengthSqr_V(DmathVector4 vec)
{
    return dmathV4LengthSqr(&vec);
}

static inline double dmathV4Length_V(DmathVector4 vec)
{
    return dmathV4Length(&vec);
}

static inline DmathVector4 dmathV4Normalize_V(DmathVector4 vec)
{
    DmathVector4 result;
    dmathV4Normalize(&result, &vec);
    return result;
}

static inline DmathVector4 dmathV4Select_V(
    DmathVector4 vec0, DmathVector4 vec1, unsigned int select1)
{
    DmathVector4 result;
    dmathV4Select(&result, &vec0, &vec1, select1);
    return result;
}

#ifdef _DVECTORMATH_DEBUG

static inline void dmathV4Print_V(DmathVector4 vec) { dmathV4Print(&vec); }

static inline void dmathV4Prints_V(DmathVector4 vec, const char* name)
{
    dmathV4Prints(&vec, name);
}

#endif

static inline DmathPoint3 dmathP3MakeFromElems_V(double _x, double _y, double _z)
{
    DmathPoint3 result;
    dmathP3MakeFromElems(&result, _x, _y, _z);
    return result;
}

static inline DmathPoint3 dmathP3MakeFromV3_V(DmathVector3 vec)
{
    DmathPoint3 result;
    dmathP3MakeFromV3(&result, &vec);
    return result;
}

static inline DmathPoint3 dmathP3MakeFromScalar_V(double scalar)
{
    DmathPoint3 result;
    dmathP3MakeFromScalar(&result, scalar);
    return result;
}

static inline DmathPoint3 dmathP3Lerp_V(
    double t, DmathPoint3 pnt0, DmathPoint3 pnt1)
{
    DmathPoint3 result;
    dmathP3Lerp(&result, t, &pnt0, &pnt1);
    return result;
}

static inline void dmathP3SetX_V(DmathPoint3* result, double _x)
{
    dmathP3SetX(result, _x);
}

static inline double dmathP3GetX_V(DmathPoint3 pnt) { return dmathP3GetX(&pnt); }

static inline void dmathP3SetY_V(DmathPoint3* result, double _y)
{
    dmathP3SetY(result, _y);
}

static inline double dmathP3GetY_V(DmathPoint3 pnt) { return dmathP3GetY(&pnt); }

static inline void dmathP3SetZ_V(DmathPoint3* result, double _z)
{
    dmathP3SetZ(result, _z);
}

static inline double dmathP3GetZ_V(DmathPoint3 pnt) { return dmathP3GetZ(&pnt); }

static inline void dmathP3SetElem_V(DmathPoint3* result, int idx, double value)
{
    dmathP3SetElem(result, idx, value);
}

static inline double dmathP3GetElem_V(DmathPoint3 pnt, int idx)
{
    return dmathP3GetElem(&pnt, idx);
}

static inline DmathVector3 dmathP3Sub_V(DmathPoint3 pnt0, DmathPoint3 pnt1)
{
    DmathVector3 result;
    dmathP3Sub(&result, &pnt0, &pnt1);
    return result;
}

static inline DmathPoint3 dmathP3AddV3_V(DmathPoint3 pnt, DmathVector3 vec1)
{
    DmathPoint3 result;
    dmathP3AddV3(&result, &pnt, &vec1);
    return result;
}

static inline DmathPoint3 dmathP3SubV3_V(DmathPoint3 pnt, DmathVector3 vec1)
{
    DmathPoint3 result;
    dmathP3SubV3(&result, &pnt, &vec1);
    return result;
}

static inline DmathPoint3 dmathP3MulPerElem_V(
    DmathPoint3 pnt0, DmathPoint3 pnt1)
{
    DmathPoint3 result;
    dmathP3MulPerElem(&result, &pnt0, &pnt1);
    return result;
}

static inline DmathPoint3 dmathP3DivPerElem_V(
    DmathPoint3 pnt0, DmathPoint3 pnt1)
{
    DmathPoint3 result;
    dmathP3DivPerElem(&result, &pnt0, &pnt1);
    return result;
}

static inline DmathPoint3 dmathP3RecipPerElem_V(DmathPoint3 pnt)
{
    DmathPoint3 result;
    dmathP3RecipPerElem(&result, &pnt);
    return result;
}

static inline DmathPoint3 dmathP3SqrtPerElem_V(DmathPoint3 pnt)
{
    DmathPoint3 result;
    dmathP3SqrtPerElem(&result, &pnt);
    return result;
}

static inline DmathPoint3 dmathP3RsqrtPerElem_V(DmathPoint3 pnt)
{
    DmathPoint3 result;
    dmathP3RsqrtPerElem(&result, &pnt);
    return result;
}

static inline DmathPoint3 dmathP3AbsPerElem_V(DmathPoint3 pnt)
{
    DmathPoint3 result;
    dmathP3AbsPerElem(&result, &pnt);
    return result;
}

static inline DmathPoint3 dmathP3CopySignPerElem_V(
    DmathPoint3 pnt0, DmathPoint3 pnt1)
{
    DmathPoint3 result;
    dmathP3CopySignPerElem(&result, &pnt0, &pnt1);
    return result;
}

static inline DmathPoint3 dmathP3MaxPerElem_V(
    DmathPoint3 pnt0, DmathPoint3 pnt1)
{
    DmathPoint3 result;
    dmathP3MaxPerElem(&result, &pnt0, &pnt1);
    return result;
}

static inline double dmathP3MaxElem_V(DmathPoint3 pnt)
{
    return dmathP3MaxElem(&pnt);
}

static inline DmathPoint3 dmathP3MinPerElem_V(
    DmathPoint3 pnt0, DmathPoint3 pnt1)
{
    DmathPoint3 result;
    dmathP3MinPerElem(&result, &pnt0, &pnt1);
    return result;
}

static inline double dmathP3MinElem_V(DmathPoint3 pnt)
{
    return dmathP3MinElem(&pnt);
}

static inline double dmathP3Sum_V(DmathPoint3 pnt) { return dmathP3Sum(&pnt); }

static inline DmathPoint3 dmathP3Scale_V(DmathPoint3 pnt, double scaleVal)
{
    DmathPoint3 result;
    dmathP3Scale(&result, &pnt, scaleVal);
    return result;
}

static inline DmathPoint3 dmathP3NonUniformScale_V(
    DmathPoint3 pnt, DmathVector3 scaleVec)
{
    DmathPoint3 result;
    dmathP3NonUniformScale(&result, &pnt, &scaleVec);
    return result;
}

static inline double dmathP3Projection_V(DmathPoint3 pnt, DmathVector3 unitVec)
{
    return dmathP3Projection(&pnt, &unitVec);
}

static inline double dmathP3DistSqrFromOrigin_V(DmathPoint3 pnt)
{
    return dmathP3DistSqrFromOrigin(&pnt);
}

static inline double dmathP3DistFromOrigin_V(DmathPoint3 pnt)
{
    return dmathP3DistFromOrigin(&pnt);
}

static inline double dmathP3DistSqr_V(DmathPoint3 pnt0, DmathPoint3 pnt1)
{
    return dmathP3DistSqr(&pnt0, &pnt1);
}

static inline double dmathP3Dist_V(DmathPoint3 pnt0, DmathPoint3 pnt1)
{
    return dmathP3Dist(&pnt0, &pnt1);
}

static inline DmathPoint3 dmathP3Select_V(
    DmathPoint3 pnt0, DmathPoint3 pnt1, unsigned int select1)
{
    DmathPoint3 result;
    dmathP3Select(&result, &pnt0, &pnt1, select1);
    return result;
}

#ifdef _DVECTORMATH_DEBUG

static inline void dmathP3Print_V(DmathPoint3 pnt) { dmathP3Print(&pnt); }

static inline void dmathP3Prints_V(DmathPoint3 pnt, const char* name)
{
    dmathP3Prints(&pnt, name);
}

#endif

/*-----------------------------------------------------------------------------
 * Constants
 */
#define _DVECTORMATH_PI_OVER_2 1.570796327f

/*-----------------------------------------------------------------------------
 * Definitions
 */
static inline DmathMatrix3 dmathM3MakeFromScalar_V(double scalar)
{
    DmathMatrix3 result;
    dmathM3MakeFromScalar(&result, scalar);
    return result;
}

static inline DmathMatrix3 dmathM3MakeFromQ_V(DmathQuat unitQuat)
{
    DmathMatrix3 result;
    dmathM3MakeFromQ(&result, &unitQuat);
    return result;
}

static inline DmathMatrix3 dmathM3MakeFromCols_V(
    DmathVector3 _col0, DmathVector3 _col1, DmathVector3 _col2)
{
    DmathMatrix3 result;
    dmathM3MakeFromCols(&result, &_col0, &_col1, &_col2);
    return result;
}

static inline void dmathM3SetCol0_V(DmathMatrix3* result, DmathVector3 _col0)
{
    dmathM3SetCol0(result, &_col0);
}

static inline void dmathM3SetCol1_V(DmathMatrix3* result, DmathVector3 _col1)
{
    dmathM3SetCol1(result, &_col1);
}

static inline void dmathM3SetCol2_V(DmathMatrix3* result, DmathVector3 _col2)
{
    dmathM3SetCol2(result, &_col2);
}

static inline void dmathM3SetCol_V(
    DmathMatrix3* result, int col, DmathVector3 vec)
{
    dmathM3SetCol(result, col, &vec);
}

static inline void dmathM3SetRow_V(
    DmathMatrix3* result, int row, DmathVector3 vec)
{
    dmathM3SetRow(result, row, &vec);
}

static inline void dmathM3SetElem_V(
    DmathMatrix3* result, int col, int row, double val)
{
    dmathM3SetElem(result, col, row, val);
}

static inline double dmathM3GetElem_V(DmathMatrix3 mat, int col, int row)
{
    return dmathM3GetElem(&mat, col, row);
}

static inline DmathVector3 dmathM3GetCol0_V(DmathMatrix3 mat)
{
    DmathVector3 result;
    dmathM3GetCol0(&result, &mat);
    return result;
}

static inline DmathVector3 dmathM3GetCol1_V(DmathMatrix3 mat)
{
    DmathVector3 result;
    dmathM3GetCol1(&result, &mat);
    return result;
}

static inline DmathVector3 dmathM3GetCol2_V(DmathMatrix3 mat)
{
    DmathVector3 result;
    dmathM3GetCol2(&result, &mat);
    return result;
}

static inline DmathVector3 dmathM3GetCol_V(DmathMatrix3 mat, int col)
{
    DmathVector3 result;
    dmathM3GetCol(&result, &mat, col);
    return result;
}

static inline DmathVector3 dmathM3GetRow_V(DmathMatrix3 mat, int row)
{
    DmathVector3 result;
    dmathM3GetRow(&result, &mat, row);
    return result;
}

static inline DmathMatrix3 dmathM3Transpose_V(DmathMatrix3 mat)
{
    DmathMatrix3 result;
    dmathM3Transpose(&result, &mat);
    return result;
}

static inline DmathMatrix3 dmathM3Inverse_V(DmathMatrix3 mat)
{
    DmathMatrix3 result;
    dmathM3Inverse(&result, &mat);
    return result;
}

static inline double dmathM3Determinant_V(DmathMatrix3 mat)
{
    return dmathM3Determinant(&mat);
}

static inline DmathMatrix3 dmathM3Add_V(DmathMatrix3 mat0, DmathMatrix3 mat1)
{
    DmathMatrix3 result;
    dmathM3Add(&result, &mat0, &mat1);
    return result;
}

static inline DmathMatrix3 dmathM3Sub_V(DmathMatrix3 mat0, DmathMatrix3 mat1)
{
    DmathMatrix3 result;
    dmathM3Sub(&result, &mat0, &mat1);
    return result;
}

static inline DmathMatrix3 dmathM3Neg_V(DmathMatrix3 mat)
{
    DmathMatrix3 result;
    dmathM3Neg(&result, &mat);
    return result;
}

static inline DmathMatrix3 dmathM3AbsPerElem_V(DmathMatrix3 mat)
{
    DmathMatrix3 result;
    dmathM3AbsPerElem(&result, &mat);
    return result;
}

static inline DmathMatrix3 dmathM3ScalarMul_V(DmathMatrix3 mat, double scalar)
{
    DmathMatrix3 result;
    dmathM3ScalarMul(&result, &mat, scalar);
    return result;
}

static inline DmathVector3 dmathM3MulV3_V(DmathMatrix3 mat, DmathVector3 vec)
{
    DmathVector3 result;
    dmathM3MulV3(&result, &mat, &vec);
    return result;
}

static inline DmathMatrix3 dmathM3Mul_V(DmathMatrix3 mat0, DmathMatrix3 mat1)
{
    DmathMatrix3 result;
    dmathM3Mul(&result, &mat0, &mat1);
    return result;
}

static inline DmathMatrix3 dmathM3MulPerElem_V(
    DmathMatrix3 mat0, DmathMatrix3 mat1)
{
    DmathMatrix3 result;
    dmathM3MulPerElem(&result, &mat0, &mat1);
    return result;
}

static inline DmathMatrix3 dmathM3MakeIdentity_V()
{
    DmathMatrix3 result;
    dmathM3MakeIdentity(&result);
    return result;
}

static inline DmathMatrix3 dmathM3MakeRotationX_V(double radians)
{
    DmathMatrix3 result;
    dmathM3MakeRotationX(&result, radians);
    return result;
}

static inline DmathMatrix3 dmathM3MakeRotationY_V(double radians)
{
    DmathMatrix3 result;
    dmathM3MakeRotationY(&result, radians);
    return result;
}

static inline DmathMatrix3 dmathM3MakeRotationZ_V(double radians)
{
    DmathMatrix3 result;
    dmathM3MakeRotationZ(&result, radians);
    return result;
}

static inline DmathMatrix3 dmathM3MakeRotationZYX_V(DmathVector3 radiansXYZ)
{
    DmathMatrix3 result;
    dmathM3MakeRotationZYX(&result, &radiansXYZ);
    return result;
}

static inline DmathMatrix3 dmathM3MakeRotationAxis_V(
    double radians, DmathVector3 unitVec)
{
    DmathMatrix3 result;
    dmathM3MakeRotationAxis(&result, radians, &unitVec);
    return result;
}

static inline DmathMatrix3 dmathM3MakeRotationQ_V(DmathQuat unitQuat)
{
    DmathMatrix3 result;
    dmathM3MakeRotationQ(&result, &unitQuat);
    return result;
}

static inline DmathMatrix3 dmathM3MakeScale_V(DmathVector3 scaleVec)
{
    DmathMatrix3 result;
    dmathM3MakeScale(&result, &scaleVec);
    return result;
}

static inline DmathMatrix3 dmathM3AppendScale_V(
    DmathMatrix3 mat, DmathVector3 scaleVec)
{
    DmathMatrix3 result;
    dmathM3AppendScale(&result, &mat, &scaleVec);
    return result;
}

static inline DmathMatrix3 dmathM3PrependScale_V(
    DmathVector3 scaleVec, DmathMatrix3 mat)
{
    DmathMatrix3 result;
    dmathM3PrependScale(&result, &scaleVec, &mat);
    return result;
}

static inline DmathMatrix3 dmathM3Select_V(
    DmathMatrix3 mat0, DmathMatrix3 mat1, unsigned int select1)
{
    DmathMatrix3 result;
    dmathM3Select(&result, &mat0, &mat1, select1);
    return result;
}

#ifdef _DVECTORMATH_DEBUG

static inline void dmathM3Print_V(DmathMatrix3 mat) { dmathM3Print(&mat); }

static inline void dmathM3Prints_V(DmathMatrix3 mat, const char* name)
{
    dmathM3Prints(&mat, name);
}

#endif

static inline DmathMatrix4 dmathM4MakeFromScalar_V(double scalar)
{
    DmathMatrix4 result;
    dmathM4MakeFromScalar(&result, scalar);
    return result;
}

static inline DmathMatrix4 dmathM4MakeFromT3_V(DmathTransform3 mat)
{
    DmathMatrix4 result;
    dmathM4MakeFromT3(&result, &mat);
    return result;
}

static inline DmathMatrix4 dmathM4MakeFromCols_V(DmathVector4 _col0,
    DmathVector4 _col1, DmathVector4 _col2, DmathVector4 _col3)
{
    DmathMatrix4 result;
    dmathM4MakeFromCols(&result, &_col0, &_col1, &_col2, &_col3);
    return result;
}

static inline DmathMatrix4 dmathM4MakeFromM3V3_V(
    DmathMatrix3 mat, DmathVector3 translateVec)
{
    DmathMatrix4 result;
    dmathM4MakeFromM3V3(&result, &mat, &translateVec);
    return result;
}

static inline DmathMatrix4 dmathM4MakeFromQV3_V(
    DmathQuat unitQuat, DmathVector3 translateVec)
{
    DmathMatrix4 result;
    dmathM4MakeFromQV3(&result, &unitQuat, &translateVec);
    return result;
}

static inline void dmathM4SetCol0_V(DmathMatrix4* result, DmathVector4 _col0)
{
    dmathM4SetCol0(result, &_col0);
}

static inline void dmathM4SetCol1_V(DmathMatrix4* result, DmathVector4 _col1)
{
    dmathM4SetCol1(result, &_col1);
}

static inline void dmathM4SetCol2_V(DmathMatrix4* result, DmathVector4 _col2)
{
    dmathM4SetCol2(result, &_col2);
}

static inline void dmathM4SetCol3_V(DmathMatrix4* result, DmathVector4 _col3)
{
    dmathM4SetCol3(result, &_col3);
}

static inline void dmathM4SetCol_V(
    DmathMatrix4* result, int col, DmathVector4 vec)
{
    dmathM4SetCol(result, col, &vec);
}

static inline void dmathM4SetRow_V(
    DmathMatrix4* result, int row, DmathVector4 vec)
{
    dmathM4SetRow(result, row, &vec);
}

static inline void dmathM4SetElem_V(
    DmathMatrix4* result, int col, int row, double val)
{
    dmathM4SetElem(result, col, row, val);
}

static inline double dmathM4GetElem_V(DmathMatrix4 mat, int col, int row)
{
    return dmathM4GetElem(&mat, col, row);
}

static inline DmathVector4 dmathM4GetCol0_V(DmathMatrix4 mat)
{
    DmathVector4 result;
    dmathM4GetCol0(&result, &mat);
    return result;
}

static inline DmathVector4 dmathM4GetCol1_V(DmathMatrix4 mat)
{
    DmathVector4 result;
    dmathM4GetCol1(&result, &mat);
    return result;
}

static inline DmathVector4 dmathM4GetCol2_V(DmathMatrix4 mat)
{
    DmathVector4 result;
    dmathM4GetCol2(&result, &mat);
    return result;
}

static inline DmathVector4 dmathM4GetCol3_V(DmathMatrix4 mat)
{
    DmathVector4 result;
    dmathM4GetCol3(&result, &mat);
    return result;
}

static inline DmathVector4 dmathM4GetCol_V(DmathMatrix4 mat, int col)
{
    DmathVector4 result;
    dmathM4GetCol(&result, &mat, col);
    return result;
}

static inline DmathVector4 dmathM4GetRow_V(DmathMatrix4 mat, int row)
{
    DmathVector4 result;
    dmathM4GetRow(&result, &mat, row);
    return result;
}

static inline DmathMatrix4 dmathM4Transpose_V(DmathMatrix4 mat)
{
    DmathMatrix4 result;
    dmathM4Transpose(&result, &mat);
    return result;
}

static inline DmathMatrix4 dmathM4Inverse_V(DmathMatrix4 mat)
{
    DmathMatrix4 result;
    dmathM4Inverse(&result, &mat);
    return result;
}

static inline DmathMatrix4 dmathM4AffineInverse_V(DmathMatrix4 mat)
{
    DmathMatrix4 result;
    dmathM4AffineInverse(&result, &mat);
    return result;
}

static inline DmathMatrix4 dmathM4OrthoInverse_V(DmathMatrix4 mat)
{
    DmathMatrix4 result;
    dmathM4OrthoInverse(&result, &mat);
    return result;
}

static inline double dmathM4Determinant_V(DmathMatrix4 mat)
{
    return dmathM4Determinant(&mat);
}

static inline DmathMatrix4 dmathM4Add_V(DmathMatrix4 mat0, DmathMatrix4 mat1)
{
    DmathMatrix4 result;
    dmathM4Add(&result, &mat0, &mat1);
    return result;
}

static inline DmathMatrix4 dmathM4Sub_V(DmathMatrix4 mat0, DmathMatrix4 mat1)
{
    DmathMatrix4 result;
    dmathM4Sub(&result, &mat0, &mat1);
    return result;
}

static inline DmathMatrix4 dmathM4Neg_V(DmathMatrix4 mat)
{
    DmathMatrix4 result;
    dmathM4Neg(&result, &mat);
    return result;
}

static inline DmathMatrix4 dmathM4AbsPerElem_V(DmathMatrix4 mat)
{
    DmathMatrix4 result;
    dmathM4AbsPerElem(&result, &mat);
    return result;
}

static inline DmathMatrix4 dmathM4ScalarMul_V(DmathMatrix4 mat, double scalar)
{
    DmathMatrix4 result;
    dmathM4ScalarMul(&result, &mat, scalar);
    return result;
}

static inline DmathVector4 dmathM4MulV4_V(DmathMatrix4 mat, DmathVector4 vec)
{
    DmathVector4 result;
    dmathM4MulV4(&result, &mat, &vec);
    return result;
}

static inline DmathVector4 dmathM4MulV3_V(DmathMatrix4 mat, DmathVector3 vec)
{
    DmathVector4 result;
    dmathM4MulV3(&result, &mat, &vec);
    return result;
}

static inline DmathVector4 dmathM4MulP3_V(DmathMatrix4 mat, DmathPoint3 pnt)
{
    DmathVector4 result;
    dmathM4MulP3(&result, &mat, &pnt);
    return result;
}

static inline DmathMatrix4 dmathM4Mul_V(DmathMatrix4 mat0, DmathMatrix4 mat1)
{
    DmathMatrix4 result;
    dmathM4Mul(&result, &mat0, &mat1);
    return result;
}

static inline DmathMatrix4 dmathM4MulT3_V(
    DmathMatrix4 mat, DmathTransform3 tfrm1)
{
    DmathMatrix4 result;
    dmathM4MulT3(&result, &mat, &tfrm1);
    return result;
}

static inline DmathMatrix4 dmathM4MulPerElem_V(
    DmathMatrix4 mat0, DmathMatrix4 mat1)
{
    DmathMatrix4 result;
    dmathM4MulPerElem(&result, &mat0, &mat1);
    return result;
}

static inline DmathMatrix4 dmathM4MakeIdentity_V()
{
    DmathMatrix4 result;
    dmathM4MakeIdentity(&result);
    return result;
}

static inline void dmathM4SetUpper3x3_V(DmathMatrix4* result, DmathMatrix3 mat3)
{
    dmathM4SetUpper3x3(result, &mat3);
}

static inline DmathMatrix3 dmathM4GetUpper3x3_V(DmathMatrix4 mat)
{
    DmathMatrix3 result;
    dmathM4GetUpper3x3(&result, &mat);
    return result;
}

static inline void dmathM4SetTranslation_V(
    DmathMatrix4* result, DmathVector3 translateVec)
{
    dmathM4SetTranslation(result, &translateVec);
}

static inline DmathVector3 dmathM4GetTranslation_V(DmathMatrix4 mat)
{
    DmathVector3 result;
    dmathM4GetTranslation(&result, &mat);
    return result;
}

static inline DmathMatrix4 dmathM4MakeRotationX_V(double radians)
{
    DmathMatrix4 result;
    dmathM4MakeRotationX(&result, radians);
    return result;
}

static inline DmathMatrix4 dmathM4MakeRotationY_V(double radians)
{
    DmathMatrix4 result;
    dmathM4MakeRotationY(&result, radians);
    return result;
}

static inline DmathMatrix4 dmathM4MakeRotationZ_V(double radians)
{
    DmathMatrix4 result;
    dmathM4MakeRotationZ(&result, radians);
    return result;
}

static inline DmathMatrix4 dmathM4MakeRotationZYX_V(DmathVector3 radiansXYZ)
{
    DmathMatrix4 result;
    dmathM4MakeRotationZYX(&result, &radiansXYZ);
    return result;
}

static inline DmathMatrix4 dmathM4MakeRotationAxis_V(
    double radians, DmathVector3 unitVec)
{
    DmathMatrix4 result;
    dmathM4MakeRotationAxis(&result, radians, &unitVec);
    return result;
}

static inline DmathMatrix4 dmathM4MakeRotationQ_V(DmathQuat unitQuat)
{
    DmathMatrix4 result;
    dmathM4MakeRotationQ(&result, &unitQuat);
    return result;
}

static inline DmathMatrix4 dmathM4MakeScale_V(DmathVector3 scaleVec)
{
    DmathMatrix4 result;
    dmathM4MakeScale(&result, &scaleVec);
    return result;
}

static inline DmathMatrix4 dmathM4AppendScale_V(
    DmathMatrix4 mat, DmathVector3 scaleVec)
{
    DmathMatrix4 result;
    dmathM4AppendScale(&result, &mat, &scaleVec);
    return result;
}

static inline DmathMatrix4 dmathM4PrependScale_V(
    DmathVector3 scaleVec, DmathMatrix4 mat)
{
    DmathMatrix4 result;
    dmathM4PrependScale(&result, &scaleVec, &mat);
    return result;
}

static inline DmathMatrix4 dmathM4MakeTranslation_V(DmathVector3 translateVec)
{
    DmathMatrix4 result;
    dmathM4MakeTranslation(&result, &translateVec);
    return result;
}

static inline DmathMatrix4 dmathM4MakeLookAt_V(
    DmathPoint3 eyePos, DmathPoint3 lookAtPos, DmathVector3 upVec)
{
    DmathMatrix4 result;
    dmathM4MakeLookAt(&result, &eyePos, &lookAtPos, &upVec);
    return result;
}

static inline DmathMatrix4 dmathM4MakePerspective_V(
    double fovyRadians, double aspect, double zNear, double zFar)
{
    DmathMatrix4 result;
    dmathM4MakePerspective(&result, fovyRadians, aspect, zNear, zFar);
    return result;
}

static inline DmathMatrix4 dmathM4MakeFrustum_V(
    double left, double right, double bottom, double top, double zNear, double zFar)
{
    DmathMatrix4 result;
    dmathM4MakeFrustum(&result, left, right, bottom, top, zNear, zFar);
    return result;
}

static inline DmathMatrix4 dmathM4MakeOrthographic_V(
    double left, double right, double bottom, double top, double zNear, double zFar)
{
    DmathMatrix4 result;
    dmathM4MakeOrthographic(&result, left, right, bottom, top, zNear, zFar);
    return result;
}

static inline DmathMatrix4 dmathM4Select_V(
    DmathMatrix4 mat0, DmathMatrix4 mat1, unsigned int select1)
{
    DmathMatrix4 result;
    dmathM4Select(&result, &mat0, &mat1, select1);
    return result;
}

#ifdef _DVECTORMATH_DEBUG

static inline void dmathM4Print_V(DmathMatrix4 mat) { dmathM4Print(&mat); }

static inline void dmathM4Prints_V(DmathMatrix4 mat, const char* name)
{
    dmathM4Prints(&mat, name);
}

#endif

static inline DmathTransform3 dmathT3MakeFromScalar_V(double scalar)
{
    DmathTransform3 result;
    dmathT3MakeFromScalar(&result, scalar);
    return result;
}

static inline DmathTransform3 dmathT3MakeFromCols_V(DmathVector3 _col0,
    DmathVector3 _col1, DmathVector3 _col2, DmathVector3 _col3)
{
    DmathTransform3 result;
    dmathT3MakeFromCols(&result, &_col0, &_col1, &_col2, &_col3);
    return result;
}

static inline DmathTransform3 dmathT3MakeFromM3V3_V(
    DmathMatrix3 tfrm, DmathVector3 translateVec)
{
    DmathTransform3 result;
    dmathT3MakeFromM3V3(&result, &tfrm, &translateVec);
    return result;
}

static inline DmathTransform3 dmathT3MakeFromQV3_V(
    DmathQuat unitQuat, DmathVector3 translateVec)
{
    DmathTransform3 result;
    dmathT3MakeFromQV3(&result, &unitQuat, &translateVec);
    return result;
}

static inline void dmathT3SetCol0_V(DmathTransform3* result, DmathVector3 _col0)
{
    dmathT3SetCol0(result, &_col0);
}

static inline void dmathT3SetCol1_V(DmathTransform3* result, DmathVector3 _col1)
{
    dmathT3SetCol1(result, &_col1);
}

static inline void dmathT3SetCol2_V(DmathTransform3* result, DmathVector3 _col2)
{
    dmathT3SetCol2(result, &_col2);
}

static inline void dmathT3SetCol3_V(DmathTransform3* result, DmathVector3 _col3)
{
    dmathT3SetCol3(result, &_col3);
}

static inline void dmathT3SetCol_V(
    DmathTransform3* result, int col, DmathVector3 vec)
{
    dmathT3SetCol(result, col, &vec);
}

static inline void dmathT3SetRow_V(
    DmathTransform3* result, int row, DmathVector4 vec)
{
    dmathT3SetRow(result, row, &vec);
}

static inline void dmathT3SetElem_V(
    DmathTransform3* result, int col, int row, double val)
{
    dmathT3SetElem(result, col, row, val);
}

static inline double dmathT3GetElem_V(DmathTransform3 tfrm, int col, int row)
{
    return dmathT3GetElem(&tfrm, col, row);
}

static inline DmathVector3 dmathT3GetCol0_V(DmathTransform3 tfrm)
{
    DmathVector3 result;
    dmathT3GetCol0(&result, &tfrm);
    return result;
}

static inline DmathVector3 dmathT3GetCol1_V(DmathTransform3 tfrm)
{
    DmathVector3 result;
    dmathT3GetCol1(&result, &tfrm);
    return result;
}

static inline DmathVector3 dmathT3GetCol2_V(DmathTransform3 tfrm)
{
    DmathVector3 result;
    dmathT3GetCol2(&result, &tfrm);
    return result;
}

static inline DmathVector3 dmathT3GetCol3_V(DmathTransform3 tfrm)
{
    DmathVector3 result;
    dmathT3GetCol3(&result, &tfrm);
    return result;
}

static inline DmathVector3 dmathT3GetCol_V(DmathTransform3 tfrm, int col)
{
    DmathVector3 result;
    dmathT3GetCol(&result, &tfrm, col);
    return result;
}

static inline DmathVector4 dmathT3GetRow_V(DmathTransform3 tfrm, int row)
{
    DmathVector4 result;
    dmathT3GetRow(&result, &tfrm, row);
    return result;
}

static inline DmathTransform3 dmathT3Inverse_V(DmathTransform3 tfrm)
{
    DmathTransform3 result;
    dmathT3Inverse(&result, &tfrm);
    return result;
}

static inline DmathTransform3 dmathT3OrthoInverse_V(DmathTransform3 tfrm)
{
    DmathTransform3 result;
    dmathT3OrthoInverse(&result, &tfrm);
    return result;
}

static inline DmathTransform3 dmathT3AbsPerElem_V(DmathTransform3 tfrm)
{
    DmathTransform3 result;
    dmathT3AbsPerElem(&result, &tfrm);
    return result;
}

static inline DmathVector3 dmathT3MulV3_V(
    DmathTransform3 tfrm, DmathVector3 vec)
{
    DmathVector3 result;
    dmathT3MulV3(&result, &tfrm, &vec);
    return result;
}

static inline DmathPoint3 dmathT3MulP3_V(DmathTransform3 tfrm, DmathPoint3 pnt)
{
    DmathPoint3 result;
    dmathT3MulP3(&result, &tfrm, &pnt);
    return result;
}

static inline DmathTransform3 dmathT3Mul_V(
    DmathTransform3 tfrm0, DmathTransform3 tfrm1)
{
    DmathTransform3 result;
    dmathT3Mul(&result, &tfrm0, &tfrm1);
    return result;
}

static inline DmathTransform3 dmathT3MulPerElem_V(
    DmathTransform3 tfrm0, DmathTransform3 tfrm1)
{
    DmathTransform3 result;
    dmathT3MulPerElem(&result, &tfrm0, &tfrm1);
    return result;
}

static inline DmathTransform3 dmathT3MakeIdentity_V()
{
    DmathTransform3 result;
    dmathT3MakeIdentity(&result);
    return result;
}

static inline void dmathT3SetUpper3x3_V(
    DmathTransform3* result, DmathMatrix3 tfrm)
{
    dmathT3SetUpper3x3(result, &tfrm);
}

static inline DmathMatrix3 dmathT3GetUpper3x3_V(DmathTransform3 tfrm)
{
    DmathMatrix3 result;
    dmathT3GetUpper3x3(&result, &tfrm);
    return result;
}

static inline void dmathT3SetTranslation_V(
    DmathTransform3* result, DmathVector3 translateVec)
{
    dmathT3SetTranslation(result, &translateVec);
}

static inline DmathVector3 dmathT3GetTranslation_V(DmathTransform3 tfrm)
{
    DmathVector3 result;
    dmathT3GetTranslation(&result, &tfrm);
    return result;
}

static inline DmathTransform3 dmathT3MakeRotationX_V(double radians)
{
    DmathTransform3 result;
    dmathT3MakeRotationX(&result, radians);
    return result;
}

static inline DmathTransform3 dmathT3MakeRotationY_V(double radians)
{
    DmathTransform3 result;
    dmathT3MakeRotationY(&result, radians);
    return result;
}

static inline DmathTransform3 dmathT3MakeRotationZ_V(double radians)
{
    DmathTransform3 result;
    dmathT3MakeRotationZ(&result, radians);
    return result;
}

static inline DmathTransform3 dmathT3MakeRotationZYX_V(DmathVector3 radiansXYZ)
{
    DmathTransform3 result;
    dmathT3MakeRotationZYX(&result, &radiansXYZ);
    return result;
}

static inline DmathTransform3 dmathT3MakeRotationAxis_V(
    double radians, DmathVector3 unitVec)
{
    DmathTransform3 result;
    dmathT3MakeRotationAxis(&result, radians, &unitVec);
    return result;
}

static inline DmathTransform3 dmathT3MakeRotationQ_V(DmathQuat unitQuat)
{
    DmathTransform3 result;
    dmathT3MakeRotationQ(&result, &unitQuat);
    return result;
}

static inline DmathTransform3 dmathT3MakeScale_V(DmathVector3 scaleVec)
{
    DmathTransform3 result;
    dmathT3MakeScale(&result, &scaleVec);
    return result;
}

static inline DmathTransform3 dmathT3AppendScale_V(
    DmathTransform3 tfrm, DmathVector3 scaleVec)
{
    DmathTransform3 result;
    dmathT3AppendScale(&result, &tfrm, &scaleVec);
    return result;
}

static inline DmathTransform3 dmathT3PrependScale_V(
    DmathVector3 scaleVec, DmathTransform3 tfrm)
{
    DmathTransform3 result;
    dmathT3PrependScale(&result, &scaleVec, &tfrm);
    return result;
}

static inline DmathTransform3 dmathT3MakeTranslation_V(
    DmathVector3 translateVec)
{
    DmathTransform3 result;
    dmathT3MakeTranslation(&result, &translateVec);
    return result;
}

static inline DmathTransform3 dmathT3Select_V(
    DmathTransform3 tfrm0, DmathTransform3 tfrm1, unsigned int select1)
{
    DmathTransform3 result;
    dmathT3Select(&result, &tfrm0, &tfrm1, select1);
    return result;
}

#ifdef _DVECTORMATH_DEBUG

static inline void dmathT3Print_V(DmathTransform3 tfrm) { dmathT3Print(&tfrm); }

static inline void dmathT3Prints_V(DmathTransform3 tfrm, const char* name)
{
    dmathT3Prints(&tfrm, name);
}

#endif

static inline DmathQuat dmathQMakeFromM3_V(DmathMatrix3 tfrm)
{
    DmathQuat result;
    dmathQMakeFromM3(&result, &tfrm);
    return result;
}

static inline DmathMatrix3 dmathV3Outer_V(
    DmathVector3 tfrm0, DmathVector3 tfrm1)
{
    DmathMatrix3 result;
    dmathV3Outer(&result, &tfrm0, &tfrm1);
    return result;
}

static inline DmathMatrix4 dmathV4Outer_V(
    DmathVector4 tfrm0, DmathVector4 tfrm1)
{
    DmathMatrix4 result;
    dmathV4Outer(&result, &tfrm0, &tfrm1);
    return result;
}

static inline DmathVector3 dmathV3RowMul_V(DmathVector3 vec, DmathMatrix3 mat)
{
    DmathVector3 result;
    dmathV3RowMul(&result, &vec, &mat);
    return result;
}

static inline DmathMatrix3 dmathV3CrossMatrix_V(DmathVector3 vec)
{
    DmathMatrix3 result;
    dmathV3CrossMatrix(&result, &vec);
    return result;
}

static inline DmathMatrix3 dmathV3CrossMatrixMul_V(
    DmathVector3 vec, DmathMatrix3 mat)
{
    DmathMatrix3 result;
    dmathV3CrossMatrixMul(&result, &vec, &mat);
    return result;
}

// May the Sony authors forgive us...
// We need shortened (yes, namespace-polluting) names for our book.

#define DPoint3 DmathPoint3
#define DVector3 DmathVector3
#define DVector4 DmathVector4
#define DQuaternion DmathQuat
#define DMatrix3 DmathMatrix3
#define DMatrix4 DmathMatrix4
#define DTransform3 DmathTransform3
#define DV3MakeFromElems dmathV3MakeFromElems_V
#define DV3MakeFromP3 dmathV3MakeFromP3_V
#define DV3MakeFromScalar dmathV3MakeFromScalar_V
#define DV3SetX dmathV3SetX_V
#define DV3SetY dmathV3SetY_V
#define DV3SetZ dmathV3SetZ_V
#define DV3GetX dmathV3GetX_V
#define DV3GetY dmathV3GetY_V
#define DV3GetZ dmathV3GetZ_V
#define DV3SetElem dmathV3SetElem_V
#define DV3GetElem dmathV3GetElem_V
#define DV3Add dmathV3Add_V
#define DV3Sub dmathV3Sub_V
#define DV3AddP3 dmathV3AddP3_V
#define DV3ScalarMul dmathV3ScalarMul_V
#define DV3ScalarDiv dmathV3ScalarDiv_V
#define DV3Neg dmathV3Neg_V
#define DV3MakeXAxis dmathV3MakeXAxis_V
#define DV3MakeYAxis dmathV3MakeYAxis_V
#define DV3MakeZAxis dmathV3MakeZAxis_V
#define DV3MulPerElem dmathV3MulPerElem_V
#define DV3DivPerElem dmathV3DivPerElem_V
#define DV3RecipPerElem dmathV3RecipPerElem_V
#define DV3SqrtPerElem dmathV3SqrtPerElem_V
#define DV3RsqrtPerElem dmathV3RsqrtPerElem_V
#define DV3AbsPerElem dmathV3AbsPerElem_V
#define DV3CopySignPerElem dmathV3CopySignPerElem_V
#define DV3MaxPerElem dmathV3MaxPerElem_V
#define DV3MinPerElem dmathV3MinPerElem_V
#define DV3MaxElem dmathV3MaxElem_V
#define DV3MinElem dmathV3MinElem_V
#define DV3Sum dmathV3Sum_V
#define DV3Dot dmathV3Dot_V
#define DV3LengthSqr dmathV3LengthSqr_V
#define DV3Length dmathV3Length_V
#define DV3Normalize dmathV3Normalize_V
#define DV3Cross dmathV3Cross_V
#define DV3Outer dmathV3Outer_V
#define DV3RowMul dmathV3RowMul_V
#define DV3CrossMatrix dmathV3CrossMatrix_V
#define DV3CrossMatrixMul dmathV3CrossMatrixMul_V
#define DV3Lerp dmathV3Lerp_V
#define DV3Slerp dmathV3Slerp_V
#define DV3Select dmathV3Select_V
#define DV4MakeFromElems dmathV4MakeFromElems_V
#define DV4MakeFromV3Scalar dmathV4MakeFromV3Scalar_V
#define DV4MakeFromV3 dmathV4MakeFromV3_V
#define DV4MakeFromP3 dmathV4MakeFromP3_V
#define DV4MakeFromQ dmathV4MakeFromQ_V
#define DV4MakeFromScalar dmathV4MakeFromScalar_V
#define DV4SetXYZ dmathV4SetXYZ_V
#define DV4GetXYZ dmathV4GetXYZ_V
#define DV4SetX dmathV4SetX_V
#define DV4SetY dmathV4SetY_V
#define DV4SetZ dmathV4SetZ_V
#define DV4SetW dmathV4SetW_V
#define DV4GetX dmathV4GetX_V
#define DV4GetY dmathV4GetY_V
#define DV4GetZ dmathV4GetZ_V
#define DV4GetW dmathV4GetW_V
#define DV4SetElem dmathV4SetElem_V
#define DV4GetElem dmathV4GetElem_V
#define DV4Add dmathV4Add_V
#define DV4Sub dmathV4Sub_V
#define DV4ScalarMul dmathV4ScalarMul_V
#define DV4ScalarDiv dmathV4ScalarDiv_V
#define DV4Neg dmathV4Neg_V
#define DV4MakeXAxis dmathV4MakeXAxis_V
#define DV4MakeYAxis dmathV4MakeYAxis_V
#define DV4MakeZAxis dmathV4MakeZAxis_V
#define DV4MakeWAxis dmathV4MakeWAxis_V
#define DV4MulPerElem dmathV4MulPerElem_V
#define DV4DivPerElem dmathV4DivPerElem_V
#define DV4RecipPerElem dmathV4RecipPerElem_V
#define DV4SqrtPerElem dmathV4SqrtPerElem_V
#define DV4RsqrtPerElem dmathV4RsqrtPerElem_V
#define DV4AbsPerElem dmathV4AbsPerElem_V
#define DV4CopySignPerElem dmathV4CopySignPerElem_V
#define DV4MaxPerElem dmathV4MaxPerElem_V
#define DV4MinPerElem dmathV4MinPerElem_V
#define DV4MaxElem dmathV4MaxElem_V
#define DV4MinElem dmathV4MinElem_V
#define DV4Sum dmathV4Sum_V
#define DV4Dot dmathV4Dot_V
#define DV4LengthSqr dmathV4LengthSqr_V
#define DV4Length dmathV4Length_V
#define DV4Normalize dmathV4Normalize_V
#define DV4Outer dmathV4Outer_V
#define DV4Lerp dmathV4Lerp_V
#define DV4Slerp dmathV4Slerp_V
#define DV4Select dmathV4Select_V
#define DP3MakeFromElems dmathP3MakeFromElems_V
#define DP3MakeFromV3 dmathP3MakeFromV3_V
#define DP3MakeFromScalar dmathP3MakeFromScalar_V
#define DP3SetX dmathP3SetX_V
#define DP3SetY dmathP3SetY_V
#define DP3SetZ dmathP3SetZ_V
#define DP3GetX dmathP3GetX_V
#define DP3GetY dmathP3GetY_V
#define DP3GetZ dmathP3GetZ_V
#define DP3SetElem dmathP3SetElem_V
#define DP3GetElem dmathP3GetElem_V
#define DP3Sub dmathP3Sub_V
#define DP3AddV3 dmathP3AddV3_V
#define DP3SubV3 dmathP3SubV3_V
#define DP3MulPerElem dmathP3MulPerElem_V
#define DP3DivPerElem dmathP3DivPerElem_V
#define DP3RecipPerElem dmathP3RecipPerElem_V
#define DP3SqrtPerElem dmathP3SqrtPerElem_V
#define DP3RsqrtPerElem dmathP3RsqrtPerElem_V
#define DP3AbsPerElem dmathP3AbsPerElem_V
#define DP3CopySignPerElem dmathP3CopySignPerElem_V
#define DP3MaxPerElem dmathP3MaxPerElem_V
#define DP3MinPerElem dmathP3MinPerElem_V
#define DP3MaxElem dmathP3MaxElem_V
#define DP3MinElem dmathP3MinElem_V
#define DP3Sum dmathP3Sum_V
#define DP3Scale dmathP3Scale_V
#define DP3NonUniformScale dmathP3NonUniformScale_V
#define DP3Projection dmathP3Projection_V
#define DP3DistSqrFromOrigin dmathP3DistSqrFromOrigin_V
#define DP3DistFromOrigin dmathP3DistFromOrigin_V
#define DP3DistSqr dmathP3DistSqr_V
#define DP3Dist dmathP3Dist_V
#define DP3Lerp dmathP3Lerp_V
#define DP3Select dmathP3Select_V
#define DQMakeFromElems dmathQMakeFromElems_V
#define DQMakeFromV3Scalar dmathQMakeFromV3Scalar_V
#define DQMakeFromV4 dmathQMakeFromV4_V
#define DQMakeFromM3 dmathQMakeFromM3_V
#define DQMakeFromScalar dmathQMakeFromScalar_V
#define DQSetXYZ dmathQSetXYZ_V
#define DQGetXYZ dmathQGetXYZ_V
#define DQSetX dmathQSetX_V
#define DQSetY dmathQSetY_V
#define DQSetZ dmathQSetZ_V
#define DQSetW dmathQSetW_V
#define DQGetX dmathQGetX_V
#define DQGetY dmathQGetY_V
#define DQGetZ dmathQGetZ_V
#define DQGetW dmathQGetW_V
#define DQSetElem dmathQSetElem_V
#define DQGetElem dmathQGetElem_V
#define DQAdd dmathQAdd_V
#define DQSub dmathQSub_V
#define DQMul dmathQMul_V
#define DQScalarMul dmathQScalarMul_V
#define DQScalarDiv dmathQScalarDiv_V
#define DQNeg dmathQNeg_V
#define DQMakeIdentity dmathQMakeIdentity_V
#define DQMakeRotationArc dmathQMakeRotationArc_V
#define DQMakeRotationAxis dmathQMakeRotationAxis_V
#define DQMakeRotationX dmathQMakeRotationX_V
#define DQMakeRotationY dmathQMakeRotationY_V
#define DQMakeRotationZ dmathQMakeRotationZ_V
#define DQConj dmathQConj_V
#define DQRotate dmathQRotate_V
#define DQDot dmathQDot_V
#define DQNorm dmathQNorm_V
#define DQNormalize dmathQNormalize_V
#define DQLerp dmathQLerp_V
#define DQSlerp dmathQSlerp_V
#define DQSquad dmathQSquad_V
#define DQSelect dmathQSelect_V
#define DM3MakeFromCols dmathM3MakeFromCols_V
#define DM3MakeFromQ dmathM3MakeFromQ_V
#define DM3MakeFromScalar dmathM3MakeFromScalar_V
#define DM3SetCol0 dmathM3SetCol0_V
#define DM3SetCol1 dmathM3SetCol1_V
#define DM3SetCol2 dmathM3SetCol2_V
#define DM3GetCol0 dmathM3GetCol0_V
#define DM3GetCol1 dmathM3GetCol1_V
#define DM3GetCol2 dmathM3GetCol2_V
#define DM3SetCol dmathM3SetCol_V
#define DM3SetRow dmathM3SetRow_V
#define DM3GetCol dmathM3GetCol_V
#define DM3GetRow dmathM3GetRow_V
#define DM3SetElem dmathM3SetElem_V
#define DM3GetElem dmathM3GetElem_V
#define DM3Add dmathM3Add_V
#define DM3Sub dmathM3Sub_V
#define DM3Neg dmathM3Neg_V
#define DM3ScalarMul dmathM3ScalarMul_V
#define DM3MulV3 dmathM3MulV3_V
#define DM3Mul dmathM3Mul_V
#define DM3MakeIdentity dmathM3MakeIdentity_V
#define DM3MakeRotationX dmathM3MakeRotationX_V
#define DM3MakeRotationY dmathM3MakeRotationY_V
#define DM3MakeRotationZ dmathM3MakeRotationZ_V
#define DM3MakeRotationZYX dmathM3MakeRotationZYX_V
#define DM3MakeRotationAxis dmathM3MakeRotationAxis_V
#define DM3MakeRotationQ dmathM3MakeRotationQ_V
#define DM3MakeScale dmathM3MakeScale_V
#define DM3AppendScale dmathM3AppendScale_V
#define DM3PrependScale dmathM3PrependScale_V
#define DM3MulPerElem dmathM3MulPerElem_V
#define DM3AbsPerElem dmathM3AbsPerElem_V
#define DM3Transpose dmathM3Transpose_V
#define DM3Inverse dmathM3Inverse_V
#define DM3Determinant dmathM3Determinant_V
#define DM3Select dmathM3Select_V
#define DM4MakeFromCols dmathM4MakeFromCols_V
#define DM4MakeFromT3 dmathM4MakeFromT3_V
#define DM4MakeFromM3V3 dmathM4MakeFromM3V3_V
#define DM4MakeFromQV3 dmathM4MakeFromQV3_V
#define DM4MakeFromScalar dmathM4MakeFromScalar_V
#define DM4SetUpper3x3 dmathM4SetUpper3x3_V
#define DM4GetUpper3x3 dmathM4GetUpper3x3_V
#define DM4SetTranslation dmathM4SetTranslation_V
#define DM4GetTranslation dmathM4GetTranslation_V
#define DM4SetCol0 dmathM4SetCol0_V
#define DM4SetCol1 dmathM4SetCol1_V
#define DM4SetCol2 dmathM4SetCol2_V
#define DM4SetCol3 dmathM4SetCol3_V
#define DM4GetCol0 dmathM4GetCol0_V
#define DM4GetCol1 dmathM4GetCol1_V
#define DM4GetCol2 dmathM4GetCol2_V
#define DM4GetCol3 dmathM4GetCol3_V
#define DM4SetCol dmathM4SetCol_V
#define DM4SetRow dmathM4SetRow_V
#define DM4GetCol dmathM4GetCol_V
#define DM4GetRow dmathM4GetRow_V
#define DM4SetElem dmathM4SetElem_V
#define DM4GetElem dmathM4GetElem_V
#define DM4Add dmathM4Add_V
#define DM4Sub dmathM4Sub_V
#define DM4Neg dmathM4Neg_V
#define DM4ScalarMul dmathM4ScalarMul_V
#define DM4MulV4 dmathM4MulV4_V
#define DM4MulV3 dmathM4MulV3_V
#define DM4MulP3 dmathM4MulP3_V
#define DM4Mul dmathM4Mul_V
#define DM4MulT3 dmathM4MulT3_V
#define DM4MakeIdentity dmathM4MakeIdentity_V
#define DM4MakeRotationX dmathM4MakeRotationX_V
#define DM4MakeRotationY dmathM4MakeRotationY_V
#define DM4MakeRotationZ dmathM4MakeRotationZ_V
#define DM4MakeRotationZYX dmathM4MakeRotationZYX_V
#define DM4MakeRotationAxis dmathM4MakeRotationAxis_V
#define DM4MakeRotationQ dmathM4MakeRotationQ_V
#define DM4MakeScale dmathM4MakeScale_V
#define DM4MakeTranslation dmathM4MakeTranslation_V
#define DM4MakeLookAt dmathM4MakeLookAt_V
#define DM4MakePerspective dmathM4MakePerspective_V
#define DM4MakeFrustum dmathM4MakeFrustum_V
#define DM4MakeOrthographic dmathM4MakeOrthographic_V
#define DM4AppendScale dmathM4AppendScale_V
#define DM4PrependScale dmathM4PrependScale_V
#define DM4MulPerElem dmathM4MulPerElem_V
#define DM4AbsPerElem dmathM4AbsPerElem_V
#define DM4Transpose dmathM4Transpose_V
#define DM4Inverse dmathM4Inverse_V
#define DM4AffineInverse dmathM4AffineInverse_V
#define DM4OrthoInverse dmathM4OrthoInverse_V
#define DM4Determinant dmathM4Determinant_V
#define DM4Select dmathM4Select_V
#define DT3MakeFromCols dmathT3MakeFromCols_V
#define DT3MakeFromM3V3 dmathT3MakeFromM3V3_V
#define DT3MakeFromQV3 dmathT3MakeFromQV3_V
#define DT3MakeFromScalar dmathT3MakeFromScalar_V
#define DT3SetUpper3x3 dmathT3SetUpper3x3_V
#define DT3GetUpper3x3 dmathT3GetUpper3x3_V
#define DT3SetTranslation dmathT3SetTranslation_V
#define DT3GetTranslation dmathT3GetTranslation_V
#define DT3SetCol0 dmathT3SetCol0_V
#define DT3SetCol1 dmathT3SetCol1_V
#define DT3SetCol2 dmathT3SetCol2_V
#define DT3SetCol3 dmathT3SetCol3_V
#define DT3GetCol0 dmathT3GetCol0_V
#define DT3GetCol1 dmathT3GetCol1_V
#define DT3GetCol2 dmathT3GetCol2_V
#define DT3GetCol3 dmathT3GetCol3_V
#define DT3SetCol dmathT3SetCol_V
#define DT3SetRow dmathT3SetRow_V
#define DT3GetCol dmathT3GetCol_V
#define DT3GetRow dmathT3GetRow_V
#define DT3SetElem dmathT3SetElem_V
#define DT3GetElem dmathT3GetElem_V
#define DT3MulV3 dmathT3MulV3_V
#define DT3MulP3 dmathT3MulP3_V
#define DT3Mul dmathT3Mul_V
#define DT3MakeIdentity dmathT3MakeIdentity_V
#define DT3MakeRotationX dmathT3MakeRotationX_V
#define DT3MakeRotationY dmathT3MakeRotationY_V
#define DT3MakeRotationZ dmathT3MakeRotationZ_V
#define DT3MakeRotationZYX dmathT3MakeRotationZYX_V
#define DT3MakeRotationAxis dmathT3MakeRotationAxis_V
#define DT3MakeRotationQ dmathT3MakeRotationQ_V
#define DT3MakeScale dmathT3MakeScale_V
#define DT3MakeTranslation dmathT3MakeTranslation_V
#define DT3AppendScale dmathT3AppendScale_V
#define DT3PrependScale dmathT3PrependScale_V
#define DT3MulPerElem dmathT3MulPerElem_V
#define DT3AbsPerElem dmathT3AbsPerElem_V
#define DT3Inverse dmathT3Inverse_V
#define DT3OrthoInverse dmathT3OrthoInverse_V
#define DT3Select dmathT3Select_V

static inline Matrix4 M4MakeFromDM4(DMatrix4 dmat)
{
    Matrix4 m;
    m.col0.x = dmat.col0.x;
    m.col0.y = dmat.col0.y;
    m.col0.z = dmat.col0.z;
    m.col0.w = dmat.col0.w;
    m.col1.x = dmat.col1.x;
    m.col1.y = dmat.col1.y;
    m.col1.z = dmat.col1.z;
    m.col1.w = dmat.col1.w;
    m.col2.x = dmat.col2.x;
    m.col2.y = dmat.col2.y;
    m.col2.z = dmat.col2.z;
    m.col2.w = dmat.col2.w;
    m.col3.x = dmat.col3.x;
    m.col3.y = dmat.col3.y;
    m.col3.z = dmat.col3.z;
    m.col3.w = dmat.col3.w;
    return m;
}

static inline DMatrix4 DM4MakeFromM4(Matrix4 fmat)
{
    DMatrix4 m;
    m.col0.x = fmat.col0.x;
    m.col0.y = fmat.col0.y;
    m.col0.z = fmat.col0.z;
    m.col0.w = fmat.col0.w;
    m.col1.x = fmat.col1.x;
    m.col1.y = fmat.col1.y;
    m.col1.z = fmat.col1.z;
    m.col1.w = fmat.col1.w;
    m.col2.x = fmat.col2.x;
    m.col2.y = fmat.col2.y;
    m.col2.z = fmat.col2.z;
    m.col2.w = fmat.col2.w;
    m.col3.x = fmat.col3.x;
    m.col3.y = fmat.col3.y;
    m.col3.z = fmat.col3.z;
    m.col3.w = fmat.col3.w;
    return m;
}

static inline Point3 P3MakeFromDP3(DPoint3 dv)
{
    Point3 v;
    v.x = dv.x;
    v.y = dv.y;
    v.z = dv.z;
    return v;
}

static inline DPoint3 DP3MakeFromP3(Point3 sv)
{
    DPoint3 v;
    v.x = sv.x;
    v.y = sv.y;
    v.z = sv.z;
    return v;
}

static inline Vector3 V3MakeFromDV3(DVector3 dv)
{
    Vector3 v;
    v.x = dv.x;
    v.y = dv.y;
    v.z = dv.z;
    return v;
}

static inline DVector3 DV3MakeFromV3(Vector3 sv)
{
    DVector3 v;
    v.x = sv.x;
    v.y = sv.y;
    v.z = sv.z;
    return v;
}
