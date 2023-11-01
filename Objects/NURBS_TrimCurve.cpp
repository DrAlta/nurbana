#include "NURBS_TrimCurve.h"


NURBS_TrimCurve::NURBS_TrimCurve(int Length, int Order) {
  // I Chose not to pack X,Y,H into Point3d for
  // reasons of compatibility with nurbs functions
  _Length= Length;
  _Order= Order;
  _KV= (double*)malloc(sizeof(double)*(Length+Order));
  _H= (double*)malloc(sizeof(double)*Length);
  _CtlPts= (Point3d*)malloc(sizeof(Point3d)*Length);
} //eof NURBS_TrimCurve::NURBS_TrimCurve()


NURBS_TrimCurve::~NURBS_TrimCurve() {
  free(_KV);
  free(_H);
  free(_CtlPts);
} //eof NURBS_TrimCurve::~NURBS_TrimCurve()


void NURBS_TrimCurve::CtlPts(Point3d *C, double *H) {
  for (int i= 0; i < _Length; i++) {
    _CtlPts[i]= C[i];
    _H[i]= H[i];
  } //eof
} //eof NURBS_TrimCurve::ControlPoints()


void NURBS_TrimCurve::KV(double *K) {
  for (int i= 0; i < _Length+_Order; i++)
    _KV[i]= K[i];
} //eof NURBS_Trim::KnotVector()
