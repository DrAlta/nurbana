/********************************************************
 * Advanced NURBS Construction Algorithms
 * Start Date: Dec 15, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Handles Advanced Surface Construction Algorithms
 ********************************************************/


#ifndef NURBS_Construct_H
#define NURBS_Construct_H

#include "../../NurbanaMath.h"

#include "NURBS_Degree.h"
#include "NURBS_Knot.h"
#include "../Object_NURBS.h"
#include "../Object_WC.h"

#ifndef NGLOBAL_H
#include "../NDefs/NGlobal.h"
#endif

class Object_NURBS;
class NURBS_Construct {
  public:
    static	void	Loft(Object_NURBS **objs, int N, int V);
    static	void	Blend(Object_NURBS *obj1, Object_NURBS *obj2, Object_NURBS *obj3, int Twist);

}; //eof class NURBS_Construct

#endif
