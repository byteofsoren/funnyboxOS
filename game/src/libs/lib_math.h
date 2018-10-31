/*
 * lib_math.h
 *
 * Created: 2018-10-21 15:47:33
 *  Author: Magnus Sörensen
 */ 


#ifndef LIB_MATH_H_
#define LIB_MATH_H_

#include <math.h>
float floatSurf[3];
int intSurf[3];
float floatVector[3];
int intVector[3];

/*******************************
* lib_reflect
* Reflects a first order tensor
* of size n on the surface.
*
* float surface[n]: is the base
* note that this vector must be
* normalized.
* float *vector: is a pointer to
* the vector that is going to 
* be reflected on surface
* int n: is the dimension of the
* first order tensor.
*
* Returns 1;
*******************************/
int lib_floatReflect(int n);
int lib_intReflect(int n);


/*******************************
* lib_normalize 
* Preforms a normalization of the
* vector pointer *v with the
* dimension of fields.
*
* float *v: Vector points out the
* vector that needs to be normalized
* int n: How many dimention the first oder
* tensor is. 
* 
* Returns 1;
*******************************/
int lib_normalize(float *v, int n);




#endif /* LIB_MATH_H_ */