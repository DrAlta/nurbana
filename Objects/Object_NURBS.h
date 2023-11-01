/****************************************************
 * Object - NURBS class
 * Start Date: Jan 27, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Takes care of NURBS specific information and
 * functions.
 ****************************************************/


#ifndef Object_NURBS_H
#define Object_NURBS_H

#include "Object_Base.h"
#include "NURBS/NURBS.h"
#include "NURBS_TrimCurve.h"

class Object_NURBS : public Object_Base {
  public:
    struct TrimCurveNode {
      int		ID;	// ID of Curve
      short		Group;	// Number of Curves in Group
      NURBS_TrimCurve	*TC;
      TrimCurveNode	*Next;
    };


    Object_NURBS(Object_List *obj);
    ~Object_NURBS();

    void		SetLength(int U, int V);
    void		SetIsoDensity(int V);

    void		Order(bool UV, int v);
    int			Order(int UV);    

    void		KnotType(bool UV, bool t);
    bool		KnotType(bool UV);

    double*		H()
			{ return( _H ); }

    double*		Hold()
			{ return( _Hold ); }

    bool		Cyclic(bool UV)
			{ return(_CyclicUV[UV]); }

    void		Cyclic(bool UV, bool v)
			{ _CyclicUV[UV]= v; }

    double*		KnotVector(bool UV)
			{ return(UV) ? _KnotVectorV : _KnotVectorU; }

    int			Change(bool UV)
			{ return( _Change[UV] ); }

    void		Change(bool UV, int V)
			{ _Change[UV]= V; }

    Point3d*		CtlPtsOld()
			{ return( _CtlPtsOld ); }

    double*		Sum()
			{ return( _Sum ); }

    double*		SumIPC(bool UV)
			{ return( _SumIPC[UV] ); }

    bool		RecalculateKnotVector()
			{ return( _RecalculateKnotVector ); }

    void		RecalculateKnotVector(bool V)
			{ _RecalculateKnotVector= V; }

    // Trim Curve Functions
    void		AttachCurve(int Length, int Order);
    void		TrimIDList(int *List);
    NURBS_TrimCurve*	TrimCurve();
    NURBS_TrimCurve*	TrimCurve(int ID);
    int			TrimCount();

  private:
    int			_OrderUV[2];		// Order of NURBS
    int			_Change[2];		// NPts + Cpts + Order
    bool		_RecalculateKnotVector; // Recalculate Knot Standard Open/Periodic Vector
    bool		_CyclicUV[2];		// Cyclic Curve
    bool		_KnotTypeUV[2];		// Periodic or Open
    Point3d		*_CtlPtsOld;		// Control Points Old
    double		*_H;			// Homogeneous Weighting Factor for each Ctl Pt
    double		*_Hold;			// Homogeneous Weighting Factor for each Ctl Pt (Old)
    double		*_Sum;			// Sum Function for each Ctl Pt
    double		*_SumIPC[2];		// Sum Function for Isoparametric Curves
    double		*_KnotVectorU;		// Knot Vector U
    double		*_KnotVectorV;		// Knot Vector V

    // Trim Curve Section
    int			_TCID;			// Current Trim Curve AutoIncrement ID
    TrimCurveNode	*_TCHead;		// NURBS Trim Curves Head

}; //eof class Object_NURBS
#endif
