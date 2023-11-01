/********************************************************
 * NURBS Main Class
 * Start Date: Nov 17, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Propogates function to respective class
 ********************************************************/


#ifndef NURBS_H
#define NURBS_H

#include <stdarg.h>
#include "../Object_NURBS.h"
#include "NURBS_Construct.h"
#include "NURBS_Degree.h"
#include "NURBS_Fitting.h"
#include "NURBS_Generate.h"
#include "NURBS_Knot.h"


#ifndef NGLOBAL_H
#include "../../NDefs/NGlobal.h"
#endif


#define	NURBS_Generate_Surface		00
#define	NURBS_Generate_KnotVector	01
#define	NURBS_Generate_Cyclic		02
#define	NURBS_Generate_TrimSurface	03

#define	NURBS_Knot_Insert		10
#define	NURBS_Knot_Remove		11
#define	NURBS_Knot_Subdivide		12

#define	NURBS_Degree_Elevate		20
#define	NURBS_Degree_Reduce		21

#define	NURBS_Construct_Loft		30
#define	NURBS_Construct_Blend		31

#define	NURBS_Fitting_GlobalInterp	40



#define	NURBS_TYPE_INT			0
#define	NURBS_TYPE_FLOAT_PTR		1

class Object_NURBS;
class NURBS {
  public:
    static	Object_List	*OBJList;
    static	int		Perform(unsigned char Action,int N,Object_NURBS **objs,int Args,...);

}; //eof class NURBS

#endif
