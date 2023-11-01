/****************************************************
 * 3d Vectors Class
 * Start Date: Jul 8, 2001
 * Author: Justin Shumaker
 *
 * Description:
 *
 ****************************************************/


#ifndef Vector_H
#define Vector_H

#include <stdio.h>

#define NRT_RMI 1.0 / RAND_MAX

template <class Type>
class Vector3 {
  public:
    Type	X,Y,Z;

    Vector3() {}
    Vector3(Type x, Type y, Type z) { X= x; Y= y; Z= z; }

    void	Set(Type x, Type y, Type z) { X= x; Y= y; Z= z; }

    void	Dump() { printf("X: %.3f, Y: %.3f, Z:%.3f\n",X,Y,Z); }



    // -----------------
    // --- OPERATORS ---
    // -----------------
    bool	operator == (Vector3 V)
    		{ return( X == V.X && Y == V.Y && Z == V.Z ); }

    Vector3&	operator = (Vector3 V)
		{ X= V.X; Y= V.Y; Z= V.Z; return(*this); }

    Vector3	operator + (Vector3 V) // PLUS
		{ Vector3 Sum(X + V.X, Y + V.Y, Z + V.Z); return(Sum); }

    Vector3	operator - (Vector3 V) // MINUS
		{ Vector3 Diff(X - V.X, Y - V.Y, Z - V.Z); return(Diff); }

    Type	operator * (Vector3 V) // DOT PRODUCT (vec*vec)
		{ Type DotProd= X*V.X + Y*V.Y + Z*V.Z; return(DotProd); }

    Vector3	operator * (Type V) // DOT PRODUCT (vec*scalar)
		{ Vector3 Scaled(X*V,Y*V,Z*V); return(Scaled); }

    Vector3	operator / (Vector3 V) // CROSS PRODUCT
		{ Vector3 CrossProd(Y*V.Z - Z*V.Y, Z*V.X - X*V.Z, X*V.Y - Y*V.X); return(CrossProd); }

    Vector3	operator / (Type S) // SCALAR DIVIDE
		{ Vector3 Scaled(X/S,Y/S,Z/S); return(Scaled); }

    Vector3	operator += (Vector3 V) // VECTOR ADDITION
		{ X+= V.X; Y+= V.Y; Z+= V.Z; return(*this); }

    Vector3	operator -= (Vector3 V) // VECTOR SUBTRACTION
		{ X-= V.X; Y-= V.Y; Z-= V.Z; return(*this); }

    Vector3	operator -() // VECTOR NEGATION
		{ Vector3 Negated(-X,-Y,-Z); return(Negated); }

    Type& 	operator [] (int i) // ALLOWS VECTOR ACCESS AS AN ARRAY.
		{ return( (i==0)?X:((i==1)?Y:Z) ); };

    // Vector Functions
    Type	Length() { return((Type)sqrt(X*X+Y*Y+Z*Z)); }
    Type	LengthSqr() { return((Type)X*X+Y*Y+Z*Z); }

    Vector3&	Normalize() { Type L= Length(); if (L) { X/= L; Y/= L; Z/= L; } return(*this); }
}; //eof class NRT_RayTracer

typedef Vector3<float> Vec3f;
typedef Vector3<double> Vec3d;

#endif

