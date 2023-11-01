#include "Object_Base.h"


Object_Base::Object_Base(Object_List *obj, char OBType) {
  _ReCalcTssPts= 1;
  _ReCalcWCTssPts= 0;
  _RegenLists= 1;
  _Name[0]= NULL;
  _MaterialID= 0;
  _OBType= OBType;

  _Active= OBJECT_ON;
  _Axes= 0;

  // Load Identity Matrix for Model Rotation Matrix
  VertexOps::IdentMatrix(_MRM);

  // Allocate memory for (4) or (16) _ActPts
  _ActPts= (bool*)malloc(sizeof(bool)*4);
  // Allocate memory for (4) or (16) _CtlPts
  _CtlPts= (Point3d*)malloc(sizeof(Point3d)*4);
  // Allocate memory for (4) or (16) _WCCtlPts
  _WCCtlPts= (Point3d*)malloc(sizeof(Point3d)*4);
  // Allocate memory for Active tesselation Points
  _ActTssPts= (bool*)malloc(sizeof(bool)*OBJECT_MAX_DENSITY*OBJECT_MAX_DENSITY);
  // Allocate memory for tesselation
  _TssPts= (Point3d*)malloc(sizeof(Point3d)*OBJECT_MAX_DENSITY*OBJECT_MAX_DENSITY);
  // Allocate memory for tesselation
  _WCTssPts= (Point3d*)malloc(sizeof(Point3d)*OBJECT_MAX_DENSITY*OBJECT_MAX_DENSITY);
  // Allocate memory for Normals
  _Normals= (Point3d*)malloc(sizeof(Point3d)*(2*OBJECT_MAX_DENSITY*OBJECT_MAX_DENSITY));

  _LengthUV[0]= 0;
  _LengthUV[1]= 0;


  _IPCDensity= 5;
  _IPCResolution= OBJECT_MAX_DENSITY;
  // Allocate memory for ISO Curves
  _IPCPts= (Point3d*)malloc(sizeof(Point3d)*IPCTotPts());
  // Allocate memory for ISO Curves
  _WCIPCPts= (Point3d*)malloc(sizeof(Point3d)*IPCTotPts());


  _Edges[0]= _Edges[1]= _Edges[2]= _Edges[3]= 0;

  _COR.x= _COR.y= _COR.z= 0.0;
  _SIZ.x= _SIZ.y= _SIZ.z= 1.0;
  _ROT.x= _ROT.y= _ROT.z= 0.0;
  _TessUV[0]= _TessUV[1]= 40;

  obj -> Push(this);
} //eof Object_Base::Object_Base()


Object_Base::~Object_Base() {
  free(_ActPts);
  free(_CtlPts);
  free(_WCCtlPts);
  free(_TssPts);
  free(_WCTssPts);
  free(_Normals);
  free(_IPCPts);
  free(_WCIPCPts);
} //eof Object_Base::~Object_Base()


int Object_Base::iGetLength(bool UV) {
  if (UV) {
    return( _LengthUV[1] );
  } else {
    return( _LengthUV[0] );
  } //fi
} //eof Object_Base::iGetLength()


void Object_Base::iSetLength(int U, int V) {
  // realloc _CtlPts
  _CtlPts= (Point3d*)realloc(_CtlPts,sizeof(Point3d)*U*V);
  _WCCtlPts= (Point3d*)realloc(_WCCtlPts,sizeof(Point3d)*U*V);
  _ActPts= (bool*)realloc(_ActPts,sizeof(bool)*U*V);
} //eof Object_Base::iSetLength()
