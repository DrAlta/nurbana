#include "NRT_Light_Sphere.h"


NRT_Light_Sphere::NRT_Light_Sphere(int ID, NRT_Light_List *obj, double X, double Y, double Z) : NRT_Light_Base(ID,obj,X,Y,Z,NRT_LIGHT_SPHERE) {
  _Intensity.Set(0,0,0);
  _Radius= 1;
} //eof NRT_Light_Sphere


bool NRT_Light_Sphere::CloseTo(double X, double Y) {
  return( (fabs(X - Y) < DELTA) ? 1 : 0);
} //eof NRT_Object_Sphere::CloseTo()


double NRT_Light_Sphere::MaybeZero(double X) {
  return (CloseTo(X, 0) ? 0 : X);
} //eof NRT_Object_Sphere::MaybeZero()


bool NRT_Light_Sphere::LightIntersection(Vec3d &StartPt, Vec3d &RayDir) {
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

  return(1);
} //eof NRT_Light_Sphere::LightIntersection()


void NRT_Light_Sphere::CalcContribLight(Vec3d &Color, NRT_Object_Manager *OM, int Jitter, Vec3d &RayDir) {
  double	LinearAtt,ConstantAtt;
  double	DistToLight, Angle;
  Vec3d		CenterDir;
  Vec3d		Up;
  Vec3d		PosVec;
  Vec3d		LightDir;
  Vec3d		IntersectPt,IntersectNormal,Diffuse;
  int		i;
  Point3d	temp,RotAng;

  LinearAtt= 0;
  ConstantAtt= 1;

  Up.Set(0,0,1);

  OM -> IntersectPt(OM -> IntersectedObjectID,IntersectPt);
  OM -> IntersectNormal(OM -> IntersectedObjectID,IntersectNormal);
  OM -> Diffuse(OM -> IntersectedObjectID,Diffuse);

  CenterDir= IntersectPt - _Position;
  CenterDir.Normalize();

//  Constant= 90 * NRT_RMI;
//  Constant= 0;

  
  for (i= 0; i < Jitter; i++) {
    PosVec= CenterDir;

    LightDir= (_Position + PosVec * _Radius) - IntersectPt;
    DistToLight= LightDir.Length();
    LightDir= LightDir / DistToLight;


    RotAng.x= 2*(float)rand()/RAND_MAX-(float)rand()/RAND_MAX;
    RotAng.y= 2*(float)rand()/RAND_MAX-(float)rand()/RAND_MAX;
    RotAng.z= 2*(float)rand()/RAND_MAX-(float)rand()/RAND_MAX;

    temp.x= LightDir.X;
    temp.y= LightDir.Y;
    temp.z= LightDir.Z;
    VertexOps::RotateVertex(RotAng.x,RotAng.y,RotAng.z,temp);
    LightDir.X= temp.x;
    LightDir.Y= temp.y;
    LightDir.Z= temp.z;


    if (!OM -> CastShadowRay(IntersectPt, LightDir, DistToLight)) {
      Angle= IntersectNormal * LightDir;
      if (Angle > 0) {
        Color.X+= (Diffuse.X * Angle) * (_Intensity.X * 1.0/Jitter) / (ConstantAtt + (LinearAtt * DistToLight));
        Color.Y+= (Diffuse.Y * Angle) * (_Intensity.Y * 1.0/Jitter) / (ConstantAtt + (LinearAtt * DistToLight));
        Color.Z+= (Diffuse.Z * Angle) * (_Intensity.Z * 1.0/Jitter) / (ConstantAtt + (LinearAtt * DistToLight));
        // The light is a sphere, create an temporary sphere that matches the sphere
        // light's radius, and cast a ray towards it to see if there is an intersection.
        // If there is an intersection, apply light reflectivity based on objects
        // given specularity

        // Calculate Reflection
        double		c1;
        Vec3d		RefRay,Specular,TmpRay;

        c1= -(IntersectNormal * RayDir);
        RefRay= RayDir + (IntersectNormal * 2 * c1);

        // Perturb the ray a little...
//        for (i= 0; i < 7; i++) {
          RotAng.x= (OM -> ReflectiveIndex(OM -> IntersectedObjectID)*2*(float)rand()/RAND_MAX)-OM -> ReflectiveIndex(OM -> IntersectedObjectID);
          RotAng.y= (OM -> ReflectiveIndex(OM -> IntersectedObjectID)*2*(float)rand()/RAND_MAX)-OM -> ReflectiveIndex(OM -> IntersectedObjectID);
          RotAng.z= (OM -> ReflectiveIndex(OM -> IntersectedObjectID)*2*(float)rand()/RAND_MAX)-OM -> ReflectiveIndex(OM -> IntersectedObjectID);

          temp.x= RefRay.X;
          temp.y= RefRay.Y;
          temp.z= RefRay.Z;
          VertexOps::RotateVertex(RotAng.x,RotAng.y,RotAng.z,temp);
          TmpRay.X= temp.x;
          TmpRay.Y= temp.y;
          TmpRay.Z= temp.z;

          // Check for Intersection
          if (LightIntersection(IntersectPt,TmpRay)) {
            OM -> Specular(OM -> IntersectedObjectID,Specular);
//            Color.X+= (_Intensity.X*Specular.X/(Jitter*Jitter));
//            Color.Y+= (_Intensity.Y*Specular.Y/(Jitter*Jitter));
//            Color.Z+= (_Intensity.Z*Specular.Z/(Jitter*Jitter));
            Color.X+= (_Intensity.X*Specular.X/(Jitter));
            Color.Y+= (_Intensity.Y*Specular.Y/(Jitter));
            Color.Z+= (_Intensity.Z*Specular.Z/(Jitter));
          } //fi
//        } //eof
      } //fi
    } //fi
  } //eof
} //eof NRT_Light_Sphere::CalcContribLight()
