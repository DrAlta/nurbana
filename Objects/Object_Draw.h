/****************************************************
 * Draw utilities
 * Start Date: May 13, 2002
 * Author: Justin Shumaker
 *
 * Description:
 * Draws model Axes, Points, etc...
 *****************************************************/


#ifndef Object_Draw_H
#define Object_Draw_H

#include <GL/gl.h>
#include "Object_Base.h"
#include "../ProjectUtility.h"
#include "../NDefs/NGlobal.h"


class Object_Draw {
  public:
    static	void	Vertices(Object_Base *obj, Point3d *Pts);
    static	void	COR(Object_Base *obj);
//    static	void	Box(Object_Base *obj);
    static	void	Wireframe(Object_Base *obj, unsigned char Ru, unsigned char Gu, unsigned char Bu, unsigned char Rv, unsigned char Gv, unsigned char Bv, unsigned int LengthU, unsigned int LengthV, Point3d *Pts, bool Edit, bool Hilite, int Index, int Direction);
    static	void	Edges(Object_Base *obj, bool T);
//    static	void	BoundingBox(Object_Base *obj);
    static	void	Axes(Object_Base *obj);

}; //eof class Object_Draw
#endif
