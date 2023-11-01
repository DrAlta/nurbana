/****************************************************
 * Object - Base
 * Start Date: Jul 14, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Base class for all Objects
 ****************************************************/
#ifndef NRT_Object_Base_H
#define NRT_Object_Base_H

#include "../NurbanaMath.h"
#include "NRT_Object_List.h"
#include "Vector.h"

#define	DELTA			.000001
#define	NRT_OBJECT_SPHERE	0
#define	NRT_OBJECT_TRIANGLE	1
#define	NRT_OBJECT_NURBS	2

class NRT_Object_List;
class NRT_Object_Base {
  public:
    NRT_Object_Base() {}
    NRT_Object_Base(int ID, NRT_Object_List *obj, char Type);
    virtual	~NRT_Object_Base() {}

    virtual	bool	FindIntersection(Vec3d &StartPt, Vec3d &RayDir)= 0;
    virtual	bool	IntersectDistance(Vec3d StartPt, Vec3d RayDir, double &Distance)= 0;
    virtual	void	Position(double X, double Y, double Z) {};
    virtual	void	SetPt(int P, double X, double Y, double Z) {};
    virtual	void	Radius(double R) {};

    //
    // NURBS Objects
    //
    virtual	void	Order(bool UV, int V) {};
    virtual	void	Length(bool UV, int V) {};
    virtual	void	Knot(bool UV, char T) {};
    virtual	void	H(int I, float V) {};
    virtual	void	Pt(int I, float V[3]) {};
    virtual	void	CalculateSurface() {};


    //---------- VARIABLES ----------
    int		_ID;
    char	_Type;

    bool	_Reflective;
    double	_ReflectiveIndex;

    bool	_Transparent;
    double	_RefractionIndex;    


    Vec3d	_Diffuse;
    Vec3d	_Specular;


    double	_IntersectDist;
    Vec3d	_IntersectPt;
    Vec3d	_IntersectNormal;
    //-----------------------------
  

}; //eof class NRT_Object_Base

#endif
