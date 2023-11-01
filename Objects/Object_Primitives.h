/*****************************************************
 * Object Primitives Utility Class
 * Start Date: Nov 30, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Generates Simple and Complex NURBS Primitives
 *****************************************************/


#ifndef Object_Primitives_H
#define Object_Primitives_H

#include <stdio.h>
#include <stdlib.h>
#include "Object_List.h"
#include "../NurbanaMath.h"

#include "Object_NURBS.h"
#include "NURBS/NURBS_Generate.h"
#include "NURBS/NURBS.h"
#include "../NDefs/NGlobal.h"


class Object_NURBS;
class Object_Primitives {
  public:
    static	void	Create(Object_List::ObjectNode *ptr, unsigned char Type);

  private:
    static	void	Curve(Object_List::ObjectNode *ptr);
    static      void	Surface(Object_List::ObjectNode *ptr);
    static	void	Circle(Object_List::ObjectNode *ptr);
    static	void	Tube(Object_List::ObjectNode *ptr);
    static	void	Torus(Object_List::ObjectNode *ptr);
    static	void	Sphere(Object_List::ObjectNode *ptr);
//    static	void	Box(Object_List::ObjectNode *ptr);
//    static	void	Cone(Object_List::ObjectNode *ptr);

}; //eof class Object_Primitives
#endif
