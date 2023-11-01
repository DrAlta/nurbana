/****************************************************
 * Face and Normal Vector Drawing Utility Class
 * Start Date: Oct 27, 2000
 * Author: Justin Shumaker
 *
 * Description:
 * Draws either Quad or Triad Faces from linear Quad
 * data as well as Quad or Triad Normal Vectors too.
 *****************************************************/


#ifndef DrawFaces_H
#define DrawFaces_H

#include "../NurbanaMath.h"
#include <GL/gl.h>
//#include "../GLUT/glut.h"

#ifndef NGLOBAL_H
#include "../NDefs/NGlobal.h"
#endif

#define DF_FACETED	0
#define DF_SMOOTH	1

class DrawFaces {
  public:
    static	void	DrawTriadFaces(Point3d *pts, Point3d *Normals, int LengthU, int LengthV, float mat_diffuse[4], float mat_specular[4], bool SMOOTH);
    static	void	DrawTriadNormalVectors(Point3d *pts, int LengthU, int LengthV, float Nlen);

}; //eof class DrawFaces
#endif
