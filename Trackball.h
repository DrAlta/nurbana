/********************************************************
 * Trackball Utility Class
 * Oct 21, 2000
 * Justin Shumaker
 *
 * Description:
 * Projects 2D Mouse coordinates onto a virtual trackball
 *********************************************************/


#ifndef Trackball_H
#define Trackball_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
//#include "GLUT/glut.h"
#include "NurbanaMath.h"


class Trackball {
  public:
    static void TrackballA(int x, int y, int WindowWidth, int WindowHeight, int Init, double Angle[3], float TB_SPEED);
    static void TrackballB(int x, int y, int Init, double Angle[3], double M[16], float TB_SPEED);

}; //eof class Trackball
#endif
