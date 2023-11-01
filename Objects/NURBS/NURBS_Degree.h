/********************************************************
 * NURBS Degree Elevation and Reduction Class
 * Start Date: Nov 17, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Returns degree Reduced/Elevated curve/surface
 ********************************************************/


#ifndef NURBS_Degree_H
#define NURBS_Degree_H

#include "../../NurbanaMath.h"

#include "../Object_NURBS.h"

#ifndef NGLOBAL_H
#include "../NDefs/NGlobal.h"
#endif

class Object_NURBS;
class NURBS_Degree {
  public:
    static	void	Elevate(Object_NURBS *obj, int Uinc, int Vinc);
    static	void	Reduce(Object_NURBS *obj, int Udec, int Vdec);

  private:
    static	void	BezDegReduce(Point3d *bpts, Point3d *rbpts, int Degree, float &MaxErr);
}; //eof class NURBS_Degree

#endif
