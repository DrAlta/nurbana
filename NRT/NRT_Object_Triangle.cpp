#include "NRT_Object_Triangle.h"

NRT_Object_Triangle::NRT_Object_Triangle(int ID,NRT_Object_List *obj) : NRT_Object_Base(ID,obj,NRT_OBJECT_TRIANGLE) {
  // Calculate Plane Normal
  _Pts[0].Set(0,0,0);
  _Pts[1].Set(0,0,0);
  _Pts[2].Set(0,0,0);
} //eof NRT_Object_Triangle::NRT_Object_Triangle()


bool NRT_Object_Triangle::CloseTo(double X, double Y) {
  return( (fabs(X - Y) < DELTA) ? 1 : 0);
} //eof NRT_Object_Triangle::CloseTo()


double NRT_Object_Triangle::MaybeZero(double X) {
  return (CloseTo(X, 0) ? 0 : X);
} //eof NRT_Object_Triangle::MaybeZero()


bool NRT_Object_Triangle::CheckPt(Vec3d StartPt, Vec3d IPt, Vec3d Pts[3], int Pt1, int Pt2) {
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
} //eof NRT_Object_Triangle::CheckPt()


bool NRT_Object_Triangle::FindIntersection(Vec3d &StartPt, Vec3d &RayDir) {
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

  PlaneNormal= _IntersectNormal;


  DotProd= PlaneNormal * RayDir;
  if (!DotProd) {
    // Ray and plane coincide, so no intersection
    return(0);
  } //fi

  if (DotProd > 0) { // Flip normal if necessary
    PlaneNormal= -(PlaneNormal);
  } //fi


  D= -(PlaneNormal * _Pts[0]);
  t= MaybeZero(-(PlaneNormal * StartPt + D) /
                (PlaneNormal * RayDir));


  if (t <= 0) { // triangle is either on or behind camera
    return(0);  // No intersection
  } //fi


  // Otherwise there was an intersection
  // Calculate the intersection Point
  _IntersectPt.X= StartPt.X + RayDir.X * t;
  _IntersectPt.Y= StartPt.Y + RayDir.Y * t;
  _IntersectPt.Z= StartPt.Z + RayDir.Z * t;

  _IntersectDist= t;
  _IntersectNormal= PlaneNormal;

  //
  // Now check to see if the point is inside of the triangle
  //


//  if (!CheckPt(StartPt,_IntersectPt,_Pts,0,1)) return(0);
//  if (!CheckPt(StartPt,_IntersectPt,_Pts,1,2)) return(0);
//  if (!CheckPt(StartPt,_IntersectPt,_Pts,2,0)) return(0);

  if (!CheckPt(StartPt,_IntersectPt,_Pts,2,1)) return(0);
  if (!CheckPt(StartPt,_IntersectPt,_Pts,1,0)) return(0);
  if (!CheckPt(StartPt,_IntersectPt,_Pts,0,2)) return(0);

  return(1);
} //eof NRT_Object_Triangle::FindIntersection()



bool NRT_Object_Triangle::IntersectDistance(Vec3d StartPt, Vec3d RayDir, double &Distance) {
  double	DotProd, D, t;
  Vec3d		PlaneNormal, IPt;

  PlaneNormal= _IntersectNormal;


  DotProd= PlaneNormal * RayDir;
  if (!DotProd) {
    // Ray and plane coincide, so no intersection
    return(0);
  } //fi

  if (DotProd > 0) { // Flip normal if necessary
    PlaneNormal= -(PlaneNormal);
  } //fi

  D= -(PlaneNormal * _Pts[0]);
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

  if (!CheckPt(StartPt,IPt,_Pts,0,1)) return(0);
  if (!CheckPt(StartPt,IPt,_Pts,1,2)) return(0);
  if (!CheckPt(StartPt,IPt,_Pts,2,0)) return(0);

//  if (!CheckPt(StartPt,IPt,_Pts,2,1)) return(0);
//  if (!CheckPt(StartPt,IPt,_Pts,1,0)) return(0);
//  if (!CheckPt(StartPt,IPt,_Pts,0,2)) return(0);

  return(1);
} //eof NRT_Object_Triangle::IntersectDistance()



void NRT_Object_Triangle::SetPt(int P, double X, double Y, double Z) {
  Vec3d		AB,BC;

  _Pts[P].Set(X,Y,Z);

  AB= _Pts[1] - _Pts[0];
  BC= _Pts[2] - _Pts[1];

  _IntersectNormal= AB / BC;
  _IntersectNormal.Normalize();
} //eof NRT_Object_Triangle::SetPt()
