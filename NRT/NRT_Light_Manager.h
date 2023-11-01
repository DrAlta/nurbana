/****************************************************
 * NRT Light Manager
 * Start Date: Jul 14, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Stores a list of pointers to each Object and
 * contains many functions related to those Objects.
 ****************************************************/


#ifndef NRT_Light_Manager_H
#define NRT_Light_Manager_H

#include <stdio.h>
#include <stdlib.h>

#include "NRT_Object_Manager.h"
#include "NRT_Light_List.h"
#include "NRT_Light_Sphere.h"


class NRT_Light_Manager {
  public:

    NRT_Light_Manager(unsigned short T);
    ~NRT_Light_Manager();

    NRT_Light_Manager*	List(int i) { return( _List[i] ); }
    void		OM(NRT_Object_Manager *O) { _ObjectList= O; }

//    void		Threads(unsigned short T) { _Threads= T; }
    void		CreateLight(int ID, char Type, double X, double Y, double Z);

    void		CalculateLighting(Vec3d &Color, int Jitter, Vec3d &RayDir);

    void		Radius(int ID, double R);
    void		Intensity(int ID, double R, double G, double B);

  private:
    unsigned	short			_Threads;
    NRT_Light_List::LightNode*		GetLight(int ID);

    NRT_Light_Manager			**_List;

    NRT_Light_List			*_Head;
    NRT_Object_Manager			*_ObjectList;

}; //eof class NRT_Light_Manager
#endif
