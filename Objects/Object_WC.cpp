#include "Object_WC.h"


void Object_WC::CtlPts(Object_Base *obj) {
  Point3d	temp;
  int		i;

  // Phase-1 (SIZE) Collect Underpants
  for (i= 0; i < obj -> _LengthUV[0] * obj -> _LengthUV[1]; i++) {
    obj ->_WCCtlPts[i].x= obj -> _CtlPts[i].x * obj -> _SIZ.x;
    obj ->_WCCtlPts[i].y= obj -> _CtlPts[i].y * obj -> _SIZ.y;
    obj ->_WCCtlPts[i].z= obj -> _CtlPts[i].z * obj -> _SIZ.z;
  } //eof

  // Phase-2 (ROTATE) ??
  for (i= 0; i < obj -> _LengthUV[0] * obj -> _LengthUV[1]; i++) {
    VertexOps::MultPtMatrix(temp, obj ->_WCCtlPts[i], obj -> _MRM);
    obj ->_WCCtlPts[i].x= temp.x;
    obj ->_WCCtlPts[i].y= temp.y;
    obj ->_WCCtlPts[i].z= temp.z;
  } //eof

  // Phase-3 (POSITION COR) Profit
  for (i= 0; i < obj -> _LengthUV[0] * obj -> _LengthUV[1]; i++) {
    obj ->_WCCtlPts[i].x+= obj -> _COR.x;
    obj ->_WCCtlPts[i].y+= obj -> _COR.y;
    obj ->_WCCtlPts[i].z+= obj -> _COR.z;
  } //eof
} //eof Object_WC::CtlPts()


void Object_WC::TssPts(Object_Base *obj) {
  Point3d	temp;
  int		i;

  // Phase-1 (SIZE) Collect Underpants
  for (i= 0; i < obj -> _TessUV[0] * obj -> _TessUV[1]; i++) {
    obj ->_WCTssPts[i].x= obj -> _TssPts[i].x * obj -> _SIZ.x;
    obj ->_WCTssPts[i].y= obj -> _TssPts[i].y * obj -> _SIZ.y;
    obj ->_WCTssPts[i].z= obj -> _TssPts[i].z * obj -> _SIZ.z;
  } //eof

  // Phase-2 (ROTATE) ??
  for (i= 0; i < obj -> _TessUV[0] * obj -> _TessUV[1]; i++) {
    VertexOps::MultPtMatrix(temp, obj ->_WCTssPts[i], obj -> _MRM);
    obj ->_WCTssPts[i].x= temp.x;
    obj ->_WCTssPts[i].y= temp.y;
    obj ->_WCTssPts[i].z= temp.z;
  } //eof

  // Phase-3 (POSITION COR) Profit
  for (i= 0; i < obj -> _TessUV[0] * obj -> _TessUV[1]; i++) {
    obj ->_WCTssPts[i].x+= obj -> _COR.x;
    obj ->_WCTssPts[i].y+= obj -> _COR.y;
    obj ->_WCTssPts[i].z+= obj -> _COR.z;
  } //eof
} //eof Object_WC::TssPts()


void Object_WC::IPLPts(Object_Base *obj) {
  Point3d	temp;
  int		i;

  // Phase-1 (SIZE) Collect Underpants
  for (i= 0; i < obj -> IPCTotPts(); i++) {
    obj -> WCIPCPts()[i].x= obj -> IPCPts()[i].x * obj -> _SIZ.x;
    obj -> WCIPCPts()[i].y= obj -> IPCPts()[i].y * obj -> _SIZ.y;
    obj -> WCIPCPts()[i].z= obj -> IPCPts()[i].z * obj -> _SIZ.z;
  } //eof

  // Phase-2 (ROTATE) ??
  for (i= 0; i < obj -> IPCTotPts(); i++) {
    VertexOps::MultPtMatrix(temp, obj -> WCIPCPts()[i], obj -> _MRM);
    obj -> WCIPCPts()[i].x= temp.x;
    obj -> WCIPCPts()[i].y= temp.y;
    obj -> WCIPCPts()[i].z= temp.z;
  } //eof

  // Phase-3 (POSITION COR) Profit
  for (i= 0; i < obj -> IPCTotPts(); i++) {
    obj -> WCIPCPts()[i].x+= obj -> _COR.x;
    obj -> WCIPCPts()[i].y+= obj -> _COR.y;
    obj -> WCIPCPts()[i].z+= obj -> _COR.z;
  } //eof
} //eof Object_WC::IPLPts()


void Object_WC::AllPts(Object_Base *obj) {
  CtlPts(obj);
  TssPts(obj);
  IPLPts(obj);
} //eof Object_Manager::GetActualPts()
