/********************************************************
 * Project/UnProject Utility Class
 * Feb 17, 1995
 * Marc Buffat
 *
 * Description:
 * Projects Coordinates from 2D to 3D space and vice versa
 *********************************************************/


#ifndef ProjectUtility_H
#define ProjectUtility_H

#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
//#include "GLUT/glut.h"

#define	PU_TRUE		1
#define	PU_FALSE	0

 
/* for Sun: */
#ifdef SUNOS4
#define MEMCPY( DST, SRC, BYTES) \
  memcpy( (char *) (DST), (char *) (SRC), (int) (BYTES) )
#else
#define MEMCPY( DST, SRC, BYTES) \
  memcpy( (void *) (DST), (void *) (SRC), (size_t) (BYTES) )
#endif

class ProjectUtility {
  public:
    static	bool	Project(double objx, double objy, double objz, const double model[16], const double proj[16], const int viewport[4], double *winx, double *winy, double *winz);
    static	bool	UnProject(double winx, double winy, double winz, const double model[16], const double proj[16], const int viewport[4], double  *objx, double  *objy, double *objz);
    static	void	PickMatrix(double x, double y, double width, double height, const int viewport[4]);
}; //eof class ProjectUtility
#endif
