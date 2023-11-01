/********************************************************
 * NURBS Fitting Algorithms
 * Start Date: Apr 21, 2002
 * Author: Justin Shumaker
 *
 * Description:
 * Handles Global and Local Interpolation and
 * Approximation
 ********************************************************/


#ifndef NURBS_Fitting_H
#define NURBS_Fitting_H

#include "../../NurbanaMath.h"

#include "NURBS_Generate.h"
#include "NURBS_Knot.h"
//#include "../Object_WC.h"

#ifndef NGLOBAL_H
#include "../NDefs/NGlobal.h"
#endif

class Object_NURBS;
class NURBS_Fitting {
  public:
    static	void	GlobalInterp(Object_NURBS *obj, int CPU, int CPV, int UK);

  private:
    static	void	Solve(double *N, double *D, double *P, int L);
    static	void	GlobalInterpCurve(Point3d *Q, int CP, int Deg, double *U, int UK);

}; //eof class NURBS_Fitting

#endif
