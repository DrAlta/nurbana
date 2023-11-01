/*****************************************************
 * Normal Calculations Utility Class
 * Start Date: Oct 27, 2000
 * Author: Justin Shumaker
 *
 * Description:
 * Calculates Normals and Smooth Normals
 *****************************************************/


#ifndef Object_Normals_H
#define Object_Normals_H

#define	OBJECT_FACETED_NORMALS	0
#define	OBJECT_SMOOTH_NORMALS	1

#include <stdio.h>
#include <stdlib.h>
#include "../NurbanaMath.h"
#include "../NDefs/NGlobal.h"


class Object_Normals {
  public:
    static	void	CalculateTriNormals(Point3d *pts, Point3d *Normals, int LengthU, int LengthV, bool SMOOTH, bool EPU, bool EPV);

}; //eof class Object_Normals
#endif
