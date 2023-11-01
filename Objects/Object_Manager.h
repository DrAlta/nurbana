/****************************************************
 * Object Manager
 * Start Date: Jan 27, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Stores a list of pointers to each Object and
 * contains many functions related to those Objects.
 ****************************************************/


#ifndef Object_Manager_H
#define Object_Manager_H

#include <stdio.h>
#include <stdlib.h>

#include "DrawFaces.h"
#include "Object_Draw.h"
#include "Object_List.h"

#include "Object_Normals.h"

#include "Object_NURBS.h"
#include "Object_Primitives.h"
#include "Object_WC.h"

#include "NURBS/NURBS.h"
#include "../MaterialList.h"

#include "../VertexOps.h"
#include "../ProjectUtility.h"
#include "../NDefs/NGlobal.h"

// Definitions
#define	OBJECT_LIST_MAIN		0
#define	OBJECT_LIST_BACKUP		1

#define	OBJECT_MODE_ZBUFFER		0
#define OBJECT_MODE_EDIT		1
#define	OBJECT_MODE_EDGE		2
#define OBJECT_MODE_ROTATE		3
#define OBJECT_MODE_TRANSLATE		4
#define OBJECT_MODE_TRACKBALL		5
#define OBJECT_MODE_SIZE		6
#define OBJECT_MODE_GRAB		7
#define OBJECT_MODE_BORDERSELECT	8

// When only Edges Should be Drawn
#define	OBJECT_MODE_DRAWEDGES		9


class Object_Manager {
  public:

    Object_Manager(MaterialList *MaterialList);
    ~Object_Manager();

     struct SelectionList {
       int		ID;
       SelectionList	*Next;
     };


     // Get Object Pointer (used for manual typecasting)
     Object_Base*	GetObj();

     // Number of Objects
     unsigned	int	GetObjCount();

     // Check if any object's Active
     bool		GetObjAct();

     // Set Active flag for All Objects
     void		SetObjActAll(bool A);

     // Fill array with object ID's and return total number of objects
     unsigned	int	IDList(unsigned int *ID_LIST);

     // Get Active Object's current state
     bool		GetActive();

     // Set active Object's Active Flag
     void		SetActive(bool State, bool Purge);

     // Get Active ID
     unsigned	int	GetActiveID()
			{ return( _ActiveID ); }

     // Set Active ID
     void		SetActiveID(unsigned int ID)
			{ _ActiveID= ID; }

     // Get Object Name
     char*		GetName();

     // Set Object Name
     void		SetName(char* Name);

     // Set COR to Center
     void		CenterCOR();

     // Center Object Points around COR
     void		CenterPoints();

     // Invert selected Points
     void		SelectInvert();

     // Extrude Points
     bool		Extrude();

     // DeExtrude Points
     bool		DeExtrude();

     // Get Model Rotation Matrix
     double*		GetMRM();

     // Set Model Rotation Matrix
     void		SetMRM(double m[16]);

     // Get Rotation
     Point3d		GetROT();

     // Set Rotation
     void		SetROT(Point3d R);

     // Get Size
     Point3d		GetSIZ();

     // Set Size
     void		SetSIZ(Point3d S);

     // Report number of CP's selected
     unsigned	int	CPActCount();

     // Set Active flag on all CP's
     void		SetCPActAll(bool A);

     // Get Activity status of an Object CP
     bool		GetCPAct(unsigned int I);

     // Set Object CP Activity Status
     void		SetCPAct(unsigned int I, bool A);

     // Return first CP ID if any CP's active on object
     unsigned	int	GetCPActID();

     // Appy a relative change in location to all active CP's
     void		CPDelta(Point3d D);

     // Get Object COR
     Point3d		GetCOR();

     // Set Object COR
     void		SetCOR(Point3d C);

     // Get CP
     Point3d		GetCP(unsigned int I);

     // Set CP
     void		SetCP(unsigned int I, Point3d C);


     // Get WC CP
     Point3d		GetWCCP(int I);

     // Get status of Object Edge
     bool		GetEdge(unsigned char Edge);

     // Set Status of Object Edge
     void		SetEdge(unsigned char Edge, bool T);

     // Recalculate Tesselation Points
     void		SetRecalcTess();

     // Recalculation WC Tesselation Points
     void		SetRecalcWCTess();

     // Get TC Tess Points
     Point3d*		GetWCTess();

     // Get Tesselation value
     unsigned	int	GetTess(bool UV);

     // Set Tesselation value
     void		SetTess(int T);

     // Get Iso Density
     int		GetIsoDensity();

     // Set Iso Density
     void		SetIsoDensity(int V);

     // Get UV Length
     unsigned	int	GetLength(bool UV);

     // Set UV Length
     void		SetLength(bool UV, unsigned int L);

     // Get W Coordinate
     double		GetH(unsigned int I);

     // Set W Coordinate
     void		SetH(unsigned int I, double V);

     // Set W Coordinate for all active CP's
     void		SetHAct(double V);

     // Get UV Order
     unsigned	int	GetOrder(bool UV);

     // Set UV Order
     void		SetOrder(unsigned int Order, bool UV);

     // Get Knot Type
     bool		GetKnotType(bool UV);

     // Set Knot Type
     void		SetKnotType(bool UV, bool KnotType);

     // Get Knot Value
     double		GetKnot(unsigned int I, bool UV);

     // Set Knot Value
     void		SetKnot(unsigned int I, bool UV, double V);

     // Set Recalculate Knot Vector Flag
     void		SetRecalcKV(bool R);

     // Update Change Flag
     void		SetChange();

     // Reset Modes to 0
     void		SetModeReset()
			{ _Mode= 0; }

     // Get Mode
     bool		GetMode(unsigned char Bit)
			{ return( _Mode & 1<<Bit ); }

     // Set Mode
     void		SetMode(unsigned char Bit, bool State)
			{ State ? _Mode|= 1<<Bit : _Mode&= 65535-(1<<Bit); }


    // Get Material ID
    unsigned	int	GetMaterialID();

    // Set Material ID
    void		SetMaterialID(unsigned int MID);

    // Duplicate Object
    void		Duplicate();

    // Delete Active Object
    void		Delete();

    // Create a Primitive
    void		Primitive(unsigned char Type);

    // Purge all Objects
    void		Purge();

    // Copy Object Lists
    void		CopyList(char LtA, char LtB, bool AB);

    // Get Normals Display Flag
    bool		GetNormals()
			{ return( _Normals ); }

    // Set Normals Display Flag
    void		SetNormals(bool V)
			{ _Normals= V; }

    // Get Normal Length
    float		NormalLength()
			{ return( _NormalLength ); }

    // Set Normal Length
    void		NormalLength(float V)
			{ _NormalLength= V; }

    // Set Working List
    void		SelectList(unsigned char T);

    // Total number of selected objects
    unsigned	int	SelectionTotal();

    // Get Smooth faces flag
    bool		GetSmooth()
			{ return(_Smooth); }

    // Set Smooth faces flag
    void		SetSmooth(bool V);

    // Set Model Axes Toggle
    void		Axes(bool S);

    // Mirror Object about Axis
    void		Mirror(unsigned char XYZ);

    // Render Objects to Screen
    void		Draw();

    // Rebuild
    void		Rebuild(int CPU, int CPV, int OrderU, int OrderV, int UK);

    // Loft
    void		Loft(int V);

    // Blend
    void		Blend(int Twist);

    // Subdivide
    void		Subdivide();



    // Modelview Stuff
    void		Init(bool V)
			{ _Init= V; }

    bool		Init()
			{ return( _Init ); }

    void		BGColor(float R, float G, float B)
			{ _BGColor[0]= R; _BGColor[1]= G; _BGColor[2]= B; }

    float		BGColor(int RGB)
			{ return( _BGColor[RGB] ); }

    void		Trackball(bool V)
			{ _Trackball= V; }

    bool		Trackball()
			{ return( _Trackball ); }

    void		TBSpeed(float V)
			{ _TBSpeed= V; }

    float		TBSpeed()
			{ return( _TBSpeed ); }

    void		GRIDColor(float R, float G, float B)
			{ _GRIDColor[0]= R; _GRIDColor[1]= G; _GRIDColor[2]= B; }

    float		GRIDColor(int RGB)
			{ return( _GRIDColor[RGB] ); }

    void		FOV(float V)
			{ _FOV= V; }

    float		FOV()
			{ return( _FOV ); }

    // Other
    char*		Notes()
			{ return( _Notes ); }

    void		Notes(char *Text)
			{ _Notes= (char*)realloc(_Notes,sizeof(char)*(strlen(Text)+2));  strcpy(_Notes,Text); }



    Object_List*	GetList()
			{ return(Head); }


  private:
    // Get Object Pointer
    Object_List::ObjectNode*	GetObject(int ID);

    // Check if Surface is closed
    bool			IsClosed(bool UV, int ULen, int VLen, Point3d *Pts);


    Object_List::ObjectNode	*_obj;

    unsigned	int		_ActiveID;

    bool			_FaceType;
    bool			_Smooth;

    bool			_Normals;
    float			_NormalLength;

    // ModelView Stuff
    bool			_Init;
    bool			_Trackball;
    float			_BGColor[3];
    float			_GRIDColor[3];
    float			_TBSpeed;
    float			_FOV;
    // ---------------

    char			*_Notes;

    MaterialList		*_MaterialList;
    Object_List			*Head;
    Object_List			*Main;
    Object_List			*Back;

    unsigned	short		_Mode;

    SelectionList		*_SLHead;

}; //eof class Object_Manager
#endif
