/****************************************************
 * Nurbana Math Utility Class
 * Start Date: Oct 27, 2000
 * Author: Justin Shumaker
 *
 * Description:
 * Assorted Math operations not in math.h
 *****************************************************/


#ifndef NurbanaMath_H
#define NurbanaMath_H


#define	N_PI	3.1415926535897932384626433832795f


#ifndef NGLOBAL_H
#include "NDefs/NGlobal.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "VertexOps.h"

class NurbanaMath {
  public:
    static	double	DtoR(double deg);
    static	double	RtoD(double rad);
    static	int	Factorial(int n);
    static	int	Min(int a, int b);
    static	int	Max(int a, int b);
    static	double	Comb(double n, double r);	// Combination
    static	void	Normal(Point3d Pts[3], Point3d &Normal);
    static	bool	Normalize(Point3d &Vector);
    static	float	GetSlope(float y2, float y1, float x2, float x1);
    static	double	Sq(double V);
    static	void	Swap(double &A, double &B);
    static	double	AngleSwept(float AX, float AY, float BX, float BY);
    static	double	Dist3d(Point3d P1, Point3d P2);
    static	double	Dist4d(Point3d P1, Point3d P2);

    static	void	IdentityMatrix(double *M, int S);
    static	void	InvertMatrix(double *M, int S);
    static	void	MultMatrix(double *A, int Ar, int Ac, double *B, int Br, int Bc, double *C);
}; //eof class NurbanaMath
#endif
