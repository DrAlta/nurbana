#include "NRT_Object_NURBS.h"
#include "../Objects/NURBS/NURBS.h"



NRT_Object_NURBS::NRT_Object_NURBS(int ID,NRT_Object_List *obj) : NRT_Object_Base(ID,obj,NRT_OBJECT_NURBS) {
  _LengthUV[0]= _LengthUV[1]= 0;
} //eof NRT_Object_NURBS::NRT_Object_NURBS()


bool CloseTo(double X, double Y) {
  return( (fabs(X - Y) < DELTA) ? 1 : 0);
} //eof CloseTo()


double MaybeZero(double X) {
  return (CloseTo(X, 0) ? 0 : X);
} //eof MaybeZero()


bool CheckPt(Vec3d StartPt, Vec3d IPt, Vec3d Pts[3], int Pt1, int Pt2) {
  Vec3d		V1,V2,N1;
  double	d1;

  V1=	Pts[Pt1] - IPt;
  V2=	Pts[Pt2] - IPt;
  N1=	V2 / V1;
  N1.Normalize();
  d1=	-(StartPt) * N1;
  if ((IPt * N1 + d1) < 0)
    return(0);
  return(1);
} //eof CheckPt()



bool TriangleTest(Vec3d &StartPt, Vec3d &RayDir, Vec3d Pts[3], Vec3d &IN, double &ID, Vec3d &IP) {
  // There are two popular ways of doing ray to plane intersections.
  // The following is the Algebraic method:
  // Ray: P= P0 + tV
  // Plane: P dot N + d= 0
  // So, (P0 + tV) dot N + d= 0
  // t= -(P0 dot N + d) / (V dot N)
  // P= P0 + tV
  // ----------------------------------------------------------------
  // P0 == StartPt
  // N  == Normal
  // V  == Ray
  // P  == IntersectPt
  // ----------------------------------------------------------------
  double	DotProd, D, t;
  Vec3d		PlaneNormal;


  PlaneNormal= IN;


  DotProd= PlaneNormal * RayDir;
  if (!DotProd) {
    // Ray and plane coincide, so no intersection
    return(0);
  } //fi

  if (DotProd > 0) { // Flip normal if necessary
    PlaneNormal= -(PlaneNormal);
  } //fi

  IN= PlaneNormal;
  D= -(PlaneNormal * Pts[0]);
  t= MaybeZero(-(PlaneNormal * StartPt + D) /
                (PlaneNormal * RayDir));


  if (t <= 0) { // triangle is either on or behind camera
    return(0);  // No intersection
  } //fi


  // Otherwise there was an intersection
  // Calculate the intersection Point
  IP.X= StartPt.X + RayDir.X * t;
  IP.Y= StartPt.Y + RayDir.Y * t;
  IP.Z= StartPt.Z + RayDir.Z * t;
  IN= PlaneNormal;
  ID= t;

  //
  // Now check to see if the point is inside of the triangle
  //


  if (!CheckPt(StartPt,IP,Pts,0,1)) return(0);
  if (!CheckPt(StartPt,IP,Pts,1,2)) return(0);
  if (!CheckPt(StartPt,IP,Pts,2,0)) return(0);
  
  return(1);
} //eof TriangleTest()


bool TriangleTestB(Vec3d &StartPt, Vec3d &RayDir, Vec3d Pts[3], Vec3d IN, double &Distance) {
  double	DotProd, D, t;
  Vec3d		PlaneNormal, IPt;


  PlaneNormal= IN;



  DotProd= PlaneNormal * RayDir;
  if (!DotProd) {
    // Ray and plane coincide, so no intersection
    return(0);
  } //fi

  if (DotProd > 0) { // Flip normal if necessary
    PlaneNormal= -(PlaneNormal);
  } //fi

  D= -(PlaneNormal * Pts[0]);
  t= MaybeZero(-(PlaneNormal * StartPt + D) /
                (PlaneNormal * RayDir));


  if (t <= 0) { // triangle is either on or behind camera
    return(0);  // No intersection
  } //fi


  // Otherwise there was an intersection
  // Calculate the intersection Point

  IPt.X= StartPt.X + RayDir.X * t;
  IPt.Y= StartPt.Y + RayDir.Y * t;
  IPt.Z= StartPt.Z + RayDir.Z * t;

  Distance= t;

  //
  // Now check to see if the point is inside of the triangle
  //

//  if (!CheckPt(StartPt,IPt,Pts,0,1)) return(0);
//  if (!CheckPt(StartPt,IPt,Pts,1,2)) return(0);
//  if (!CheckPt(StartPt,IPt,Pts,2,0)) return(0);

  if (!CheckPt(StartPt,IPt,Pts,2,1)) return(0);
  if (!CheckPt(StartPt,IPt,Pts,1,0)) return(0);
  if (!CheckPt(StartPt,IPt,Pts,0,2)) return(0);

  return(1);
} //eof TriangleTestB()



bool BoundingBox(Vec3d StartPt, Vec3d RayDir, Vec3d BL, Vec3d BH) {
  double	tnear,tfar,t1,t2;
  double	_StartPt[3], _BL2[3], _BH[3], _RayDir[3];
  int		i;

  tnear= -9999999;
  tfar= 9999999;

  _StartPt[0]= StartPt.X;
  _StartPt[1]= StartPt.Y;
  _StartPt[2]= StartPt.Z;
  _BL2[0]= BL.X;
  _BL2[1]= BL.Y;
  _BL2[2]= BL.Z;
  _BH[0]= BH.X;
  _BH[1]= BH.Y;
  _BH[2]= BH.Z;
  _RayDir[0]= RayDir.X;
  _RayDir[1]= RayDir.Y;
  _RayDir[2]= RayDir.Z;
  

  for (i= 0; i < 3; i++) {
    if (_RayDir[i] == 0) {
      // Ray is parallel to the X planes
      if ((_StartPt[i] < _BL2[i]) || (_StartPt[i] > _BH[i])) return(0);
    } else {
      t1= (_BL2[i]-_StartPt[i])/_RayDir[i];
      t2= (_BH[i]-_StartPt[i])/_RayDir[i];

      if (t1 > t2) NurbanaMath::Swap(t1,t2);
      if (t1 > tnear) tnear= t1;
      if (t2 < tfar) tfar= t2;
      if (tnear > tfar) return(0); // box is missed
      if (tfar < 0) return(0); // box is behind ray and therfore missed
    } //fi
  } //eof

  return(1);
} //eof BoundingBox()



bool NRT_Object_NURBS::FindIntersection(Vec3d &StartPt, Vec3d &RayDir) {
  // Cycle through entire surface as triangles and check for intersection
  // with triangle
  int		i,n,k;
  Vec3d		Pts[3];

  // Create a Bounding Box and check for intersection with that before proceeding
  
  if (BoundingBox(StartPt, RayDir,_BL2,_BH)) {
    k= 0;
    for (i= 0; i < TESSU-1; i++) {
      for (n= 0; n < TESSV-1; n++) {       
        Pts[0].Set(_TssPts[(i+1)*TESSV+n+1].x,_TssPts[(i+1)*TESSV+n+1].y,_TssPts[(i+1)*TESSV+n+1].z);
        Pts[1].Set(_TssPts[i*TESSV+n+1].x,_TssPts[i*TESSV+n+1].y,_TssPts[i*TESSV+n+1].z);
        Pts[2].Set(_TssPts[i*TESSV+n].x,_TssPts[i*TESSV+n].y,_TssPts[i*TESSV+n].z);
        if (TriangleTest(StartPt,RayDir,Pts,_Normals[k],_IntersectDist,_IntersectPt)) {
          _IntersectNormal= _Normals[k];
          return(1);
        } //fi
        k++;

        Pts[0].Set(_TssPts[i*TESSV+n].x,_TssPts[i*TESSV+n].y,_TssPts[i*TESSV+n].z);
        Pts[1].Set(_TssPts[(i+1)*TESSV+n].x,_TssPts[(i+1)*TESSV+n].y,_TssPts[(i+1)*TESSV+n].z);
        Pts[2].Set(_TssPts[(i+1)*TESSV+n+1].x,_TssPts[(i+1)*TESSV+n+1].y,_TssPts[(i+1)*TESSV+n+1].z);
        if (TriangleTest(StartPt,RayDir,Pts,_Normals[k],_IntersectDist,_IntersectPt)) {
          _IntersectNormal= _Normals[k];
          return(1);
        } //fi
        k++;
      } //eof
    } //eof

  } //fi

  return(0);
} //eof NRT_Object_NURBS::FindIntersection()


bool NRT_Object_NURBS::IntersectDistance(Vec3d StartPt, Vec3d RayDir, double &Distance) {
  // Cycle through entire surface as triangles and check for intersection
  // with triangle
  int		i,n,k;
  Vec3d		Pts[3];


  if (BoundingBox(StartPt, RayDir,_BL2,_BH)) {

    k= 0;
    for (i= 0; i < TESSU-1; i++) {
      for (n= 0; n < TESSV-1; n++) {
        Pts[0].Set(_TssPts[(i+1)*TESSV+n+1].x,_TssPts[(i+1)*TESSV+n+1].y,_TssPts[(i+1)*TESSV+n+1].z);
        Pts[1].Set(_TssPts[i*TESSV+n+1].x,_TssPts[i*TESSV+n+1].y,_TssPts[i*TESSV+n+1].z);
        Pts[2].Set(_TssPts[i*TESSV+n].x,_TssPts[i*TESSV+n].y,_TssPts[i*TESSV+n].z);
        if (TriangleTestB(StartPt,RayDir,Pts,_Normals[k],Distance)) return(1);
        k++;

        Pts[0].Set(_TssPts[i*TESSV+n].x,_TssPts[i*TESSV+n].y,_TssPts[i*TESSV+n].z);
        Pts[1].Set(_TssPts[(i+1)*TESSV+n].x,_TssPts[(i+1)*TESSV+n].y,_TssPts[(i+1)*TESSV+n].z);
        Pts[2].Set(_TssPts[(i+1)*TESSV+n+1].x,_TssPts[(i+1)*TESSV+n+1].y,_TssPts[(i+1)*TESSV+n+1].z);
        if (TriangleTestB(StartPt,RayDir,Pts,_Normals[k],Distance)) return(1);
        k++;
      } //eof
    } //eof

  } //fi

  return(0);
} //eof NRT_Object_NURBS::IntersectDistance()


void NRT_Object_NURBS::Order(bool UV, int V) {
  _OrderUV[UV]= V;
} //eof NRT_Object_NURBS::Order()


void NRT_Object_NURBS::Length(bool UV, int V) {
  _LengthUV[UV]= V;
} //eof NRT_Object_NURBS::Length()


void NRT_Object_NURBS::Knot(bool UV, char T) {
  _KnotType[UV]= T;
} //eof NRT_Object_NURBS::Knot()


void NRT_Object_NURBS::H(int I, float V) {
  _H[I]= V;
} //eof NRT_Object_NURBS::H()


void NRT_Object_NURBS::Pt(int I, float V[3]) {
  _CtlPts[I].x= V[0];
  _CtlPts[I].y= V[1];
  _CtlPts[I].z= V[2];
} //eof NRT_Object_NURBS::Pt()


void NRT_Object_NURBS::CalculateSurface() {
  int		i,n,k;
  Vec3d		AB,BC,Pts[3];


  // Generate Knot U and V Knot Vectors for Surface
//  KnotVector::GetKnotVector(_LengthUV[0], _LengthUV[1], _OrderUV[0], _OrderUV[1], _KnotType[0], _KnotType[1], KVU, KVV);
//  NURBS::GetNURBSSurface(_LengthUV[0], _LengthUV[1], _CtlPts, _CtlPtsOld, TESSU, TESSV, _TssPts, _H, _Hold, _KnotType[0], _KnotType[1], KVU, KVV, _OrderUV[0], _OrderUV[1], _Sum, 0,1,0,1, _Change);

  // Calculate Normals for each Triangle and Store
  k= 0;
  for (i= 0; i < TESSU-1; i++) {
    for (n= 0; n < TESSV-1; n++) {

      Pts[0].Set(_TssPts[(i+1)*TESSV+n+1].x,_TssPts[(i+1)*TESSV+n+1].y,_TssPts[(i+1)*TESSV+n+1].z);
      Pts[1].Set(_TssPts[i*TESSV+n+1].x,_TssPts[i*TESSV+n+1].y,_TssPts[i*TESSV+n+1].z);
      Pts[2].Set(_TssPts[i*TESSV+n].x,_TssPts[i*TESSV+n].y,_TssPts[i*TESSV+n].z);

      AB= Pts[1] - Pts[0];
      BC= Pts[2] - Pts[1];

      _Normals[k]= AB / BC;
      _Normals[k].Normalize();

      k++;
      // --------------------

      Pts[0].Set(_TssPts[i*TESSV+n].x,_TssPts[i*TESSV+n].y,_TssPts[i*TESSV+n].z);
      Pts[1].Set(_TssPts[(i+1)*TESSV+n].x,_TssPts[(i+1)*TESSV+n].y,_TssPts[(i+1)*TESSV+n].z);
      Pts[2].Set(_TssPts[(i+1)*TESSV+n+1].x,_TssPts[(i+1)*TESSV+n+1].y,_TssPts[(i+1)*TESSV+n+1].z);

      AB= Pts[1] - Pts[0];
      BC= Pts[2] - Pts[1];

      _Normals[k]= AB / BC;
      _Normals[k].Normalize();

      k++;
    } //eof
  } //eof


  // Compute Bounding Box
  _BL2.Set(_CtlPts[0].x,_CtlPts[0].y,_CtlPts[0].z);
  _BH.Set(_CtlPts[0].x,_CtlPts[0].y,_CtlPts[0].z);

  for (i= 0; i < TESSU*TESSV; i++) {
    if (_CtlPts[i].x < _BL2.X) _BL2.X= _CtlPts[i].x;
    if (_CtlPts[i].y < _BL2.Y) _BL2.Y= _CtlPts[i].y;
    if (_CtlPts[i].z < _BL2.Z) _BL2.Z= _CtlPts[i].z;

    if (_CtlPts[i].x > _BH.X) _BH.X= _CtlPts[i].x;
    if (_CtlPts[i].y > _BH.Y) _BH.Y= _CtlPts[i].y;
    if (_CtlPts[i].z > _BH.Z) _BH.Z= _CtlPts[i].z;
  } //eof
} //eof NRT_Object_NURBS::CalculateSurface()
