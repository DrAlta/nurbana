/********************************************************
 * NURBS Generations Class
 * Start Date: Dec 17, 2000
 * Author: Justin Shumaker
 *
 * Description:
 * - Knot Vector
 * - Surface
 * - Cyclic Surface
 ********************************************************/


#ifndef NURBS_Generate_H
#define NURBS_Generate_H

#include <SDL_thread.h>

#include "../../NurbanaMath.h"
#include "NURBS.h"
#include "NURBS_Knot.h"
#include "../Object_NURBS.h"
#include "../NURBS_TrimCurve.h"

#ifndef NGLOBAL_H
#include "../NDefs/NGlobal.h"
#endif

#define	NURBS_KV_Periodic		0
#define	NURBS_KV_Open			1
#define	NURBS_KV_U			0
#define	NURBS_KV_V			1

class Object_NURBS;
struct ThreadInfo {
  bool	Lock;
  int	TessStart;
  int	TessStop;
};
struct SharedData {
  Object_NURBS	*obj;
  bool		Change;
  bool		Update;
  bool		recalc;
  int		Index;
  int		TessU;
  int		TessV;
  int		Unpts;
  int		Vnpts;
  ThreadInfo	Task[2];
  Point3d	*TssPts;
  double	*Sum;
  double	**BasisU;
  double	**BasisV;
};
class NURBS_Generate {
  public:
    static	void		Surface(Object_NURBS *obj);
    static	void		TrimSurface(Object_NURBS *obj);
    static	void		Cyclic(Object_NURBS *obj);
    static	void		KnotVector(Object_NURBS *obj);
    static	void		Duplicate(Object_NURBS *obj1, Object_NURBS *obj2);

    static	void		SurfaceThread(int ThreadID);
    static	int		ThreadID;

  private:
    // Intersecting 2 NURBS Curves
    static	void		NURBSCurveIntersect(int NPts1, int Order1, Point3d *CtlPts1, double *H1, double *KV1, int NPts2, int Order2, Point3d *CtlPts2, double *H2, double *KV2, double s, double t, int iteration);
    // Intersecting a NURBS Curve with a Parametric Line
    static	void		NURBSCurveIntersect(double j, NURBS_TrimCurve *Curve, double s, double t, int iteration);

    static	Point3d		NURBSCurvePt(int Order, int Npts, Point3d *CtlPts, double *H, double *KV, double t, bool D);

    static	void		PeriodicKnot(int npts, int Order, double *KnotVector);
    static	void		OpenKnot(int npts, int Order, double *KnotVector);

    static	void		NURBSBasis(int Order, double t, int npts, double *KnotVector, double *H, double *BasisValues, bool Type);
    static	void		NURBSBasisDer1(int Order, double t, int npts, double *KnotVector, double *H, double *BasisValues, bool Type);
    static	double		Sum(double *H, double *BasisU, double *BasisV, int Unpts, int Vnpts, int ThreadID);

    static	void		AssignThreadData(Object_NURBS *obj, int TessU, int TessV, Point3d *TssPts, double *Sum, int Index, bool Change, bool Update);
    static	void		Surface();
    static	SharedData	ThreadData;


}; //eof class NURBS_Generate
#endif
