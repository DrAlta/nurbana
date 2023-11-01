/****************************************************
 * Object - NURBS Trim Curve class
 * Start Date: Aug 13, 2002
 * Author: Justin Shumaker
 *
 * Description:
 * Class for storing NURBS Trim Curves
 ****************************************************/


#ifndef NURBS_TrimCurve_H
#define NURBS_TrimCurve_H

#include <stdlib.h>
#include "../NDefs/NGlobal.h"

class NURBS_TrimCurve {
  public:
    NURBS_TrimCurve(int Length, int Order);
    ~NURBS_TrimCurve();

     void	CtlPts(Point3d *C, double *H);

     Point3d*	CtlPts()
		{ return( _CtlPts ); }

     double*	H()
		{ return( _H ); }

     void	KV(double *K);

     double*	KV()
		{ return( _KV ); }

     int	Length()
		{ return( _Length ); }

     int	Order()
		{ return( _Order ); }

  private:
    // We assume knot vectors to be Open and not Periodic
    int		_Length;
    int		_Order;
    double	*_KV;
    double	*_H;
    Point3d	*_CtlPts;
 
}; //eoc NURBS_TrimCurve
#endif
