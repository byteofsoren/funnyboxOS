/*
 * lib_math.c
 *
 * Created: 2018-10-21 15:47:44
 *  Author: Magnus Sörensen
 */ 

#include <math.h>
#include "lib_math.h"

#define loop(C,X) for(C = 0; C < X ; C++)

int lib_floatReflect(int n)
{
	int i = 0;
	float dot_p = 0;
	// Calculate the dot product. 
	//for (i = 0; i < n ; i++)
	loop(i,n)
	{
		dot_p += floatSurf[i]*floatVector[i];	
	}
	//for (i = 0; i < n; i++)
	loop(i,n)
	{
		floatVector[i] = floatSurf[i] - 2 * dot_p * floatSurf[i];	
	}
	return 1;
}

int lib_intReflect(int n)
{
	int i = 0;
	float dot_p = 0;
	// Calculate the dot product. 
	//for (i = 0; i < n ; i++)
	loop(i,n)
	{
		//dot_p += surface[i]*vector[i];	
		dot_p += intSurf[i]*intVector[i];	
	}
	//for (i = 0; i < n; i++)
	loop(i,n)
	{
		//vector[i] = surface[i] - 2 * dot_p * surface[i];	
		intVector[i] = floatSurf[i] - 2 * dot_p * floatSurf[i];	
	}
	return 1;
}

int lib_normalize(float *v, int n)
{
	float sum = 0;
	int i = 0;
	// calculate the normalisation value 1/sqrt([t**2 for t in v].sum())
	//for (i = 0; i < n;i++)
	loop(i,n)
	{
		sum += v[i] * v[i];
	}
	float norm = 1/(sqrt(sum));
	// Apply the normalization to the vertor V. 
	loop(i,n)
	{
		v[i] *= norm;
	}
}
