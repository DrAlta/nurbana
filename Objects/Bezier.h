/********************************************************
 * Bezier Curves and Surface Utility Class
 * Start Date: Oct 27, 2000
 * Author: Justin Shumaker
 *
 * Description:
 * Computes Bezier Curves and Surface Data
 ********************************************************/


#ifndef Bezier_H
#define Bezier_H

#include <math.h>
#include "../NurbanaMath.h"
#include "../NDefs/NGlobal.h"

class Bezier {
  public:
    static	void	GetBezierCurve(int npts, Point3d *b, int cpts, Point3d *p);
    static	void	GetBezierSurface(int Unpts, int Vnpts, Point3d *b, int Ucpts, int Vcpts, Point3d *p);

}; //eof class Bezier

#endif
