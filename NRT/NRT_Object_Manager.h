/****************************************************
 * NRT Object Manager
 * Start Date: Jul 14, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Object Manager
 ****************************************************/


#ifndef NRT_Object_Manager_H
#define NRT_Object_Manager_H


#include "NRT_Object_List.h"
#include "NRT_Object_Sphere.h"
#include "NRT_Object_Triangle.h"
#include "NRT_Object_NURBS.h"


class NRT_Object_Manager {
  public:

    NRT_Object_Manager(unsigned short T);
    ~NRT_Object_Manager();

    NRT_Object_Manager*	List(int i) { return( _List[i] ); }

//    void	Threads(unsigned short T) { _Thread= T; }
    void		CreateObject(int ID, char Type);

    bool		FindIntersection(Vec3d &StartPt, Vec3d &RayDir);

    void		Diffuse(int ID, Vec3d &Diffuse);
    void		IntersectPt(int ID, Vec3d &IntersectPt);
    void		IntersectNormal(int ID, Vec3d &IntersectNormal);
    bool		CastShadowRay(Vec3d StartPt, Vec3d LightDir, double DistToLight);

    void		Position(int ID, double X, double Y, double Z);
    void		Radius(int ID, double R);
    void		SetPt(int ID, int P, double X, double Y, double Z);

    void		Diffuse(int ID, double R, double G, double B);

    void		Specular(int ID, double R, double G, double B);
    void		Specular(int ID, Vec3d &Specular);

    void		Reflective(int ID, bool R);
    bool		Reflective(int ID);

    void		ReflectiveIndex(int ID, double I);
    double		ReflectiveIndex(int ID);

    void		Transparent(int ID, bool T);
    bool		Transparent(int ID);

    void		RefractiveIndex(int ID, double I);
    double		RefractiveIndex(int ID);

    //
    // NURBS Functions
    //
    void		Order(int ID, bool UV, int V);
    void		Length(int ID, bool UV, int V);
    void		Knot(int ID, bool UV, char T);
    void		H(int ID, int I, float V);
    void		Pt(int ID, int I, float V[3]);
    void		CalculateSurface(int ID);

    int			IntersectedObjectID;


  private:
    unsigned	short			_Threads;

    NRT_Object_List::ObjectNode*	GetObject(int ID);

    NRT_Object_Manager			**_List;

    NRT_Object_List			*_Head;


}; //eof class NRT_Object_Manager
#endif
