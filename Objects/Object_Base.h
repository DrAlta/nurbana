/****************************************************
 * Object - Base
 * Start Date: Jan 25, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Base class for all Objects
 ****************************************************/
#ifndef Object_Base_H
#define Object_Base_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Object_List.h"
#include "../VertexOps.h"


#include "../NDefs/NGlobal.h"


#define	OBJECT_MAX_DENSITY		128	// Line Density

#define	OBJECT_OFF			0
#define	OBJECT_ON			1

#define	OBJECT_BEZIER			0
#define	OBJECT_NURBS			1

#define	OBJECT_PRIMITIVE_CURVE		0
#define	OBJECT_PRIMITIVE_SURFACE	1
#define	OBJECT_PRIMITIVE_CIRCLE		2
#define OBJECT_PRIMITIVE_TUBE		3
#define OBJECT_PRIMITIVE_TORUS		4
#define	OBJECT_PRIMITIVE_SPHERE		5


class Object_List;
class Object_Base {
  public:
    Object_Base() {}
    Object_Base(Object_List *obj, char OBType);
    virtual ~Object_Base();

    int			ID()
			{ return(_ID); }

    bool		Active()
			{ return( _Active ); }

    Point3d		COR()
			{ return( _COR ); }

    void		COR(Point3d V)
			{ _COR= V; }

    Point3d		SIZ()
			{ return( _SIZ ); }

    void		SIZ(Point3d V)
			{ _SIZ= V; }

    double*		MRM()
			{ return( _MRM ); }

    bool		Edge(unsigned char E)
			{ return( _Edges[E] ); }

    void		Edge(unsigned char E, bool V)
			{ _Edges[E]= V; }

    int			Length(bool UV)
			{ return(UV) ? _LengthUV[1] : _LengthUV[0]; }

    void		Length(bool UV, int V)
			{ UV ? _LengthUV[1]= V : _LengthUV[0]= V; }

    virtual	void	SetLength(int U, int V)= 0;

    virtual	void	RecalculateKnotVector(bool V) {};
    virtual	bool	RecalculateKnotVector() { return(0); };

    virtual	void	Order(bool UV, int v) {};
    virtual	int	Order(int UV) { return(0); };

    virtual	void	KnotType(bool UV, bool t) {};
    virtual	bool	KnotType(bool UV) { return(0); };

    virtual	double*	H() { return(0); };

    virtual	bool	Cyclic(bool UV) { return(0); };
    virtual	void	Cyclic(bool UV, bool v) {};

    virtual	double*	KnotVector(bool UV) { return(0); };


    int			iGetLength(bool UV);
    void		iSetLength(int U, int V);

    void		Tess(bool UV, int T)
			{ _TessUV[UV]= T; }

    int			Tess(bool UV)
			{ return(UV) ? _TessUV[1] : _TessUV[0]; }

    int			IPCDensity()
			{ return( _IPCDensity ); }

    void		IPCDensity(int V)
			{ _IPCDensity= V; }

    int			IPCResolution()
			{ return( _IPCResolution ); }

    void		IPCResolution(int V)
			{ _IPCResolution= V; }

    int			IPCTotPts()
			{ return( _IPCDensity * _IPCDensity * _IPCResolution ); }

    Point3d*		CtlPts()
			{ return( _CtlPts ); }

    Point3d*		WCCtlPts()
			{ return( _WCCtlPts ); }

    Point3d*		TssPts()
			{ return( _TssPts ); }

    Point3d*		WCTssPts()
			{ return( _WCTssPts ); }

    bool*		ActTssPts()
			{ return( _ActTssPts ); }

    Point3d*		IPCPts()
			{ return( _IPCPts ); }

    Point3d*		WCIPCPts()
			{ return( _WCIPCPts ); }

    //---------- VARIABLES ----------
    int			_ID;					// Object ID
    int			_List;					// GL Display List
    int			_MaterialID;				// Object's Material ID
    int			_LengthUV[2];				// Length of UV segments
    int			_TessUV[2];				// Level of Tesselation in UV
    int			_IPCDensity;				// Isoparametric Line Density
    int			_IPCResolution;				// Isoparametric Line Resolution
    bool		_Axes;					// Model XYZ Axes Toggle
    bool		_ReCalcTssPts;				// Recalculate Tesselation Points
    bool		_ReCalcWCTssPts;			// Reclcaulte WC Tess Pts
    bool		_RegenLists;				// Renerate List?
    bool		_Active;				// Is Object Active
    bool		*_ActPts;				// Which points are Active
    bool		_Edges[4];				// Which of the 4 Edges are Active
    char		_Name[64];				// Name of Object
    char		_OBType;				// Type of Object
    Point3d		_COR;					// XYZ Center Of Rotation Values
    Point3d		_ROT;					// XYZ Rotation Values
    Point3d		_SIZ;					// XYZ Size/Scale Values
    Point3d		*_CtlPts;				// Control Points
    Point3d		*_WCCtlPts;				// Control Points
    bool		*_ActTssPts;				// Active Tesselation Points as defined by Trimming Curves
    Point3d		*_TssPts;				// Vertices that make up Curve/Surface
    Point3d		*_WCTssPts;				// Vertices that make up Curve/Surface
    Point3d		*_IPCPts;				// Isoparametric Lines
    Point3d		*_WCIPCPts;				// Isoparametric Lines
    Point3d		*_Normals;				// Normal Lines for each face
    double		_MRM[16];				// Model Rotation Matrix
    //-----------------------------
  
}; //eof class Object_Base

#endif
