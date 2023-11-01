/****************************************************
 * NRT NURBS Object Class
 * Start Date: Jul 18, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Deals with all NURBS stuff
 ****************************************************/


#ifndef NRT_Object_NURBS_H
#define NRT_Object_NURBS_H

#include "NRT_Object_Base.h"
//#include "../Objects/NURBS.h"
//#include "../Objects/KnotVector.h"
#ifndef NGLOBAL_H
#include "../NDefs/NGlobal.h"
#endif

#define		TESSU	25
#define		TESSV	25

class NRT_Object_NURBS : NRT_Object_Base {
  public:
    NRT_Object_NURBS(int ID,NRT_Object_List *obj);
    ~NRT_Object_NURBS() {}

    bool	FindIntersection(Vec3d &StartPt, Vec3d &RayDir);
    bool	IntersectDistance(Vec3d StartPt, Vec3d RayDir, double &Distance);

    void	Order(bool UV, int V);
    void	Length(bool UV, int V);
    void	Knot(bool UV, char T);
    void	H(int I, float V);
    void	Pt(int I, float V[3]);
    void	CalculateSurface();


  private:
    int		_Change;
    int		_LengthUV[2];
    int		_OrderUV[2];
    char	_KnotType[2];

    Point3d	*_CtlPts;
    Point3d	*_CtlPtsOld;

    float	*_H;
    float	*_Hold;

    float	*_Sum;

    Point3d	*_TssPts;
    Vec3d	_Normals[4096];

    Vec3d	_BL2;
    Vec3d	_BH;

}; //eof class NRT_Object_Sphere

#endif
