/********************************************************
 * NURBS Knot Vector Related Functions
 * Start Date: Nov 17, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Functions related to the manipulation of the Knot
 * Vector
 ********************************************************/


#ifndef NURBS_Knot_H
#define NURBS_Knot_H

#include "../../NurbanaMath.h"

#include "../Object_NURBS.h"

#ifndef NGLOBAL_H
#include "../NDefs/NGlobal.h"
#endif


class Object_NURBS;
class NURBS_Knot {
  public:
    static	void	Average(int deg, int n, double *uk, double *U);
    static	void	BasisFuns(int i, double u, int p, double *U, double *N);

    // Chord Length Algorithms
    static	bool	Centripetal(int deg, int n, Point3d *Q, double *uk);
    static	bool	Chord(int deg, int n, Point3d *Q, double *uk);
    static	bool	Universal(int deg, int n, Point3d *Q, double *uk);

    static	void	Difference(double *KV1, int L1, double *KV2, int L2, double *KV3, int &L3);
    static	int	FindSpan(int Unpts, int p, double u, double *KnotVector);
    static	void	Scale(double *KV, int r, double Max);
    static	void	MergeKnotVectors(int Ulen, double *U, int Vlen, double *V, int &Ki, double *KV);
    static	void	Refine(Object_NURBS *obj, double *X, int r, bool UV);
    static	void	Subdivide(Object_NURBS *obj);
    static	void	Insert(Object_NURBS *obj, int U, int V);
    static	void	Remove(Object_NURBS *obj);
    static	void	BezierDecompose(int Npts, int Order, Point3d *CtlPts, double *KV);

  private:
    static	void	_Insert(Object_NURBS *obj, bool UV, double *KV, int KVLen);
    
}; //eof class NURBS_Knot

#endif
