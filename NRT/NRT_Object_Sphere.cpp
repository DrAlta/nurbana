#include "NRT_Object_Sphere.h"

NRT_Object_Sphere::NRT_Object_Sphere(int ID,NRT_Object_List *obj) : NRT_Object_Base(ID,obj,NRT_OBJECT_SPHERE) {
  _Radius= 0;
} //eof NRT_Object_Sphere::NRT_Object_Sphere()


bool NRT_Object_Sphere::CloseTo(double X, double Y) {
  return( (fabs(X - Y) < DELTA) ? 1 : 0);
} //eof NRT_Object_Sphere::CloseTo()


double NRT_Object_Sphere::MaybeZero(double X) {
  return (CloseTo(X, 0) ? 0 : X);
} //eof NRT_Object_Sphere::MaybeZero()


bool NRT_Object_Sphere::FindIntersection(Vec3d &StartPt, Vec3d &RayDir) {
  Vec3d		IPt, Normal;
  double	A,B,C,RadiusSq;
  double	Disc,SqrtDisc;
  double	t0,t1;
  double	t;
  bool		InsideSphere;


  RadiusSq= NurbanaMath::Sq(_Radius);
  A= 1.0;
  B= MaybeZero(2 * (RayDir.X * (StartPt.X - _Position.X) +
                    RayDir.Y * (StartPt.Y - _Position.Y) +
                    RayDir.Z * (StartPt.Z - _Position.Z))
              );
  
  C= MaybeZero(NurbanaMath::Sq(StartPt.X - _Position.X) + NurbanaMath::Sq(StartPt.Y - _Position.Y) +
               NurbanaMath::Sq(StartPt.Z - _Position.Z) - RadiusSq
              );



  Disc= NurbanaMath::Sq(B) - 4 * C;
  if (Disc < 0) {
    return(0);
  } //fi

  // Calculate t0 and t1;
  SqrtDisc= sqrt(Disc);
  t0= MaybeZero((-B - SqrtDisc) * 0.5);
  t1= MaybeZero((-B + SqrtDisc) * 0.5);

  // Find the smallest, positive t.
  InsideSphere= 0;

  if (t0 < t1) {
    if (t0 > 0) {
      t= t0;
    } else {
      if (t1 <= 0) {
        // No intersection point on ray.
        return 0;
      } else {
        InsideSphere= 1;
        t= t1;
      } //fi
    } //fi
  } else {
    if (t1 > 0) {
      t= t1;
    } else {
      if (t0 <= 0) {
        // No intersection point on ray.
        return 0;
      } else {
        InsideSphere= 1;
        t= t0;
      } //fi
    } //fi
  } //fi


  // Calculate Intersection Point
  IPt.X= StartPt.X + RayDir.X * t;
  IPt.Y= StartPt.Y + RayDir.Y * t;
  IPt.Z= StartPt.Z + RayDir.Z * t;

  // Calculate Surface Normal
  Normal= IPt - _Position;
  Normal.Normalize();

  // If inside the sphere, flip the normal
  if (InsideSphere) {
    Normal= -Normal;
  } //fi

  // Update Member Variables
  _IntersectDist= t;
  _IntersectPt= IPt;
  _IntersectNormal= Normal;
  return(1);
} //eof NRT_Object_Sphere::FindIntersection()


bool NRT_Object_Sphere::IntersectDistance(Vec3d StartPt, Vec3d RayDir, double &Distance) {
  Vec3d		IPt, Normal;
  double	A,B,C,RadiusSq;
  double	Disc,SqrtDisc;
  double	t0,t1;
  double	t;
  bool		InsideSphere;


  RadiusSq= NurbanaMath::Sq(_Radius);
  A= 1.0;
  B= MaybeZero(2 * (RayDir.X * (StartPt.X - _Position.X) +
                    RayDir.Y * (StartPt.Y - _Position.Y) +
                    RayDir.Z * (StartPt.Z - _Position.Z))
              );
  
  C= MaybeZero(NurbanaMath::Sq(StartPt.X - _Position.X) + NurbanaMath::Sq(StartPt.Y - _Position.Y) +
               NurbanaMath::Sq(StartPt.Z - _Position.Z) - RadiusSq
              );



  Disc= NurbanaMath::Sq(B) - 4 * C;
  if (Disc < 0) {
    return(0);
  } //fi

  // Calculate t0 and t1;
  SqrtDisc= sqrt(Disc);
  t0= MaybeZero((-B - SqrtDisc) * 0.5);
  t1= MaybeZero((-B + SqrtDisc) * 0.5);

  // Find the smallest, positive t.
  InsideSphere= 0;

  if (t0 < t1) {
    if (t0 > 0) {
      t= t0;
    } else {
      if (t1 <= 0) {
        // No intersection point on ray.
        return 0;
      } else {
        InsideSphere= 1;
        t= t1;
      } //fi
    } //fi
  } else {
    if (t1 > 0) {
      t= t1;
    } else {
      if (t0 <= 0) {
        // No intersection point on ray.
        return 0;
      } else {
        InsideSphere= 1;
        t= t0;
      } //fi
    } //fi
  } //fi


  // Calculate Intersection Point
  IPt.X= StartPt.X + RayDir.X * t;
  IPt.Y= StartPt.Y + RayDir.Y * t;
  IPt.Z= StartPt.Z + RayDir.Z * t;

  // Calculate Surface Normal
  Normal= IPt - _Position;
  Normal.Normalize();

  // If inside the sphere, flip the normal
  if (InsideSphere) {
    Normal= -Normal;
  } //fi

  // Update Member Variables
  Distance= t;
  return(1);
} //eof NRT_Object_Sphere::IntersectDistance()


void NRT_Object_Sphere::Radius(double R) {
  _Radius= R;
} //eof NRT_Object_Sphere::Radius()


void NRT_Object_Sphere::Position(double X, double Y, double Z) {
  _Position.X= X;
  _Position.Y= Y;
  _Position.Z= Z;
} //eof NRT_Object_Sphere::Position()
