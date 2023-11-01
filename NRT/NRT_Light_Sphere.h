/****************************************************
 * Light Class
 * Start Date: Jul 8, 2001
 * Author: Justin Shumaker
 *
 * Description:
 *
 ****************************************************/


#ifndef NRT_Light_Sphere_H
#define NRT_Light_Sphere_H

#include "NRT_Light_Base.h"
#include "../VertexOps.h"

class NRT_Light_Sphere : NRT_Light_Base {
  public:
    NRT_Light_Sphere(int ID, NRT_Light_List *obj, double X, double Y, double Z);
    ~NRT_Light_Sphere() {}

    void	CalcContribLight(Vec3d &Color, NRT_Object_Manager *OM, int Jitter, Vec3d &RayDir);
    void	Radius(double R) { _Radius= R; }
    void	Intensity(double R, double G, double B)
		{ _Intensity.X= R; _Intensity.Y= G; _Intensity.Z= B; }


  private:
    bool	NRT_Light_Sphere::LightIntersection(Vec3d &StartPt, Vec3d &RayDir);
    bool	NRT_Light_Sphere::CloseTo(double X, double Y);
    double	NRT_Light_Sphere::MaybeZero(double X);

    Vec3d	_Intensity;
    double	_Radius;

}; //eof class NRT_Light

#endif
