/****************************************************
 * 4 x 4 Matrix
 * Start Date: Jul 9, 2001
 * Author: Justin Shumaker
 *
 * Description:
 *
 ****************************************************/

#ifndef Matrix44_H
#define Matrix44_H

#include "Vector.h"

static const float Matrix44ToRads = 0.0174532f;

template <class Type>
class Matrix44 {
  public:
    Type	M[16];

    Matrix44() {}


    // Matrix Operators
    Vector3<Type> operator * (Vector3<Type> V);

    // Matrix Operators
    Matrix44<Type> operator * (Matrix44<Type> M);


    // Matrix Functions
    void	Identity();
    void	Transpose();
    void	Invert();
    void	Rotate(Type Angle, Vector3<Type> Axis);


    // Miscellaneous Functions
    static void	Swap(Type &A, Type &B)
		{ Type T; T= A; A= B; B= T; }


}; //eof class Matrix44

#include "MatrixImp.h"

typedef Matrix44<float> Mat44f;
typedef Matrix44<double> Mat44d;

#endif
