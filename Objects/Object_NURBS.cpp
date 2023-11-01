#include "Object_NURBS.h"


Object_NURBS::Object_NURBS(Object_List *obj) : Object_Base(obj,OBJECT_NURBS) {
  int	i;

  _Change[0]= _Change[1]= 0;
  _RecalculateKnotVector= _CyclicUV[0]= _CyclicUV[1]= 0;
  _KnotTypeUV[0]= _KnotTypeUV[1]= NURBS_KV_Open;
  _OrderUV[0]= _OrderUV[1]= 4;

  // allocate memory for HWeights and default to
  // 4 for Curve, 16 for Surface
  _H= (double*)malloc(sizeof(double)*4);
  for (i= 0; i < 4; i++) _H[i]= 1.0;
  _Hold= (double*)malloc(sizeof(double)*4);

  _CtlPtsOld= (Point3d*)malloc(sizeof(Point3d)*4);
  _Sum= (double*)malloc(sizeof(double)*OBJECT_MAX_DENSITY*OBJECT_MAX_DENSITY);
  _SumIPC[0]= (double*)malloc(sizeof(double)*IPCTotPts());
  _SumIPC[1]= (double*)malloc(sizeof(double)*IPCTotPts());

  // The Knot Vector is user dynamic and must be allocated
  // the maximum size which is 2*CP's.
  // So, 8 for Curve, and 32 for Surface
  _KnotVectorU= (double*)malloc(sizeof(double)*8);
  _KnotVectorV= (double*)malloc(sizeof(double)*8);

  // Trim Curves, Let first Curve ID be 1
  _TCID= 1;
  _TCHead= NULL;
} //eof Object_NURBS::Object_NURBS()


Object_NURBS::~Object_NURBS() {
  free(_H);
  free(_Hold);
  free(_CtlPtsOld);
  free(_Sum);
  free(_KnotVectorU);
  free(_KnotVectorV);
  free(_SumIPC[0]);
  free(_SumIPC[1]);

  // Delete Trim Curves
  TrimCurveNode		*ptr;
  while (_TCHead) {
    ptr= _TCHead -> Next;
    delete _TCHead;
    _TCHead= ptr;
  } //eow
} //eof Object_NURBS::~Object_NURBS()


void Object_NURBS::SetLength(int U, int V) {
  int	i;
  // Update _H Weights
  _H= (double*)realloc(_H,sizeof(double)*U*V);
  for (i= 0; i < U*V; i++) _H[i]= 1.0;
  _Hold= (double*)realloc(_Hold,sizeof(double)*U*V);
//      for (i= 0; i < U*V; i++) _Hold[i]= 1.0;

  _CtlPtsOld= (Point3d*)realloc(_CtlPtsOld,sizeof(Point3d)*U*V);

  // Update _KnotVectorUV
  _KnotVectorU= (double*)realloc(_KnotVectorU,sizeof(double)*(2*U+1));
  _KnotVectorV= (double*)realloc(_KnotVectorV,sizeof(double)*(2*V+1));
  iSetLength(U,V);
} //eof Object_NURBS::SetLength()


void Object_NURBS::SetIsoDensity(int V) {
  _IPCDensity= V;

  // Resize Sum Arrays
  _SumIPC[0]= (double*)realloc(_SumIPC[0],sizeof(double)*IPCTotPts());
  _SumIPC[1]= (double*)realloc(_SumIPC[1],sizeof(double)*IPCTotPts());

  // Resize IPCPts and WCIPCPts Arrays
  _IPCPts= (Point3d*)realloc(_IPCPts,sizeof(Point3d)*IPCTotPts());
  _WCIPCPts= (Point3d*)realloc(_WCIPCPts,sizeof(Point3d)*IPCTotPts());
} //eof Object_NURBS::SetIsoDensity()


void Object_NURBS::Order(bool UV, int v) {
  _OrderUV[UV]= v;
} //eof Object_NURBS::Order()


int Object_NURBS::Order(int UV) {
  return(UV) ? _OrderUV[1] : _OrderUV[0];
} //eof Object_NURBS::Order()


void Object_NURBS::KnotType(bool UV, bool t) {
  _KnotTypeUV[UV]= t;
  _RecalculateKnotVector= 1;
} //eof Object_NURBS::KnotType()


bool Object_NURBS::KnotType(bool UV) {
  return(_KnotTypeUV[UV]);
} //eof Object_NURBS::KnotType()


// ******************
// Trim Curve Section
// ******************

void Object_NURBS::AttachCurve(int Length, int Order) {
  if (_TCHead) {
    TrimCurveNode	*ptr;
    ptr= _TCHead;
    _TCHead= new TrimCurveNode;
    _TCHead -> Next= ptr;
  } else {
    _TCHead= new TrimCurveNode;
    _TCHead -> Next= NULL;
  } //fi
  _TCHead -> TC= new NURBS_TrimCurve(Length,Order);
  _TCHead -> ID= _TCID++;
} //eof Object_NURBS::AttachCurve()


NURBS_TrimCurve* Object_NURBS::TrimCurve() {
  return( _TCHead -> TC );
} //eof Object_NURBS::TrimCurve()


NURBS_TrimCurve* Object_NURBS::TrimCurve(int ID) {
  TrimCurveNode		*ptr;

  ptr= _TCHead;
  while (ptr) {
    if (ptr -> ID == ID)
      return(ptr -> TC);
    ptr= ptr -> Next;
  } //eow

  return(NULL);
} //eof Object_NURBS::TrimCurve()


int Object_NURBS::TrimCount() {
  int		i= 0;
  TrimCurveNode	*ptr;

  ptr= _TCHead;
  while (ptr) {
    i++;
    ptr= ptr -> Next;
  } //eow

  return(i);
} //eof Object_NURBS::TrimCount()


void Object_NURBS::TrimIDList(int *List) {
  int		i;
  TrimCurveNode	*ptr;

  ptr= _TCHead;
  i= 0;
  while (ptr) {
    List[i++]= ptr -> ID;
    ptr= ptr -> Next;
  } //eow
} //eof Object_NURBS::TrimIDList()