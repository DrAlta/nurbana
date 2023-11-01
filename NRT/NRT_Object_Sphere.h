/****************************************************
 * NRT Sphere Object Class
 * Start Date: Jul 14, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Deals with all sphere related stuff
 ****************************************************/


#ifndef NRT_Object_Sphere_H
#define NRT_Object_Sphere_H

#include "NRT_Object_Base.h"

class NRT_Object_Sphere : NRT_Object_Base {
  public:
    NRT_Object_Sphere(int ID,NRT_Object_List *obj);
    ~NRT_Object_Sphere() {}

    bool	FindIntersection(Vec3d &StartPt, Vec3d &RayDir);
    bool	IntersectDistance(Vec3d StartPt, Vec3d RayDir, double &Distance);
    void	Position(double X, double Y, double Z);
    void	Radius(double R);


  private:
    bool	CloseTo(double X, double Y);
    double	MaybeZero(double X);


    double	_Radius;
    Vec3d	_Position;

}; //eof class NRT_Object_Sphere

#endif
