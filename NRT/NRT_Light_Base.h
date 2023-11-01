/****************************************************
 * Object - Base
 * Start Date: Jul 14, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Base class for all types of lights
 ****************************************************/
#ifndef NRT_Light_Base_H
#define NRT_Light_Base_H

#include <stdlib.h>
#include "NRT_Light_List.h"
#include "NRT_Object_Manager.h"
#include "Vector.h"
#include "Matrix.h"


#define NRT_RMI			1.0 / RAND_MAX

#define	NRT_LIGHT_POINT		0
#define	NRT_LIGHT_SPHERE	1


class NRT_Light_List;
class NRT_Light_Base {
  public:
    NRT_Light_Base() {}
    NRT_Light_Base(int ID, NRT_Light_List *obj, double X, double Y, double Z, char Type);
    virtual	~NRT_Light_Base() {}

    virtual	void	CalcContribLight(Vec3d &Color, NRT_Object_Manager *OM, int Jitter, Vec3d &RayDir)= 0;

    virtual	void	Radius(double R) {};
    virtual	void	Intensity(double R, double G, double B) {};


    //---------- VARIABLES ----------
    int			_ID;
    char		_Type;

    Vec3d		_Position;
//    NRT_Object_Manager	*_ObjectList;
    //-------------------------------
  

}; //eof class NRT_Light_Base

#endif
