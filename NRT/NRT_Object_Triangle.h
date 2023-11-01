/****************************************************
 * NRT Triangle Object Class
 * Start Date: Jul 15, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Deals with all triangle related stuff
 ****************************************************/


#ifndef NRT_Object_Triangle_H
#define NRT_Object_Triangle_H

#include "NRT_Object_Base.h"

class NRT_Object_Triangle : NRT_Object_Base {
  public:
    NRT_Object_Triangle(int ID,NRT_Object_List *obj);
    ~NRT_Object_Triangle() {}

    bool	FindIntersection(Vec3d &StartPt, Vec3d &RayDir);
    bool	IntersectDistance(Vec3d StartPt, Vec3d RayDir, double &Distance);
    void	SetPt(int P, double X, double Y, double Z);

  private:
    bool	CloseTo(double X, double Y);
    double	MaybeZero(double X);
    bool	CheckPt(Vec3d StartPt, Vec3d IPt, Vec3d Pts[3], int Pt1, int Pt2);

    Vec3d	_Pts[3];

}; //eof class NRT_Object_Sphere

#endif
