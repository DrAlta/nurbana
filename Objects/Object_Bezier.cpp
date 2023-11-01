#include "Object_Bezier.h"


Object_Bezier::Object_Bezier(Object_List *obj, bool CSType) : Object_Base(obj,CSType,OBJECT_BEZIER) {
} //eof Object_Bezier::Object_Bezier()


int Object_Bezier::GetLength(char Type, bool UV) {
  return( iGetLength(Type,UV) );
} //eof Object_Bezier::GetLength()


void Object_Bezier::SetLength(char Type, int U, int V) {
  iSetLength(Type,U,V);
} //eof Object_Bezier::SetLength()


void Object_Bezier::CalculatePts() {
  if (_CSType == OBJECT_CURVE) {
    Bezier::GetBezierCurve(_LengthUV[0], _CtlPts, _TessUV[0], _TssPts);
  } else {
    Bezier::GetBezierSurface(_LengthUV[0], _LengthUV[1], _CtlPts, _TessUV[0], _TessUV[1], _TssPts);
  } //fi
} //eof Object_Bezier::CalculatePts()
