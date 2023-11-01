#include "Trackball.h"

/****************************************************************
 * Author: Justin Shumaker (justin@nurbana.cx)
 * Date: October 21, 2000
 *
 * Notes:
 * _tbPointToVector() originally written by:
 * David Yu and David Blythe
 * This trackball removes gimbal lock thus allowing you to
 * rotate the scene in any random fashion and your scene will
 * return back to the original position when you mouse your
 * mouse back to the position on the screen where you started
 * the trackball.
 *
 * Description:
 * This is a simple trackball C++ utility class which operates
 * off of two functions, ::InitTrackball() and ::GetResult()
 * Simply call ::InitTrackball() on a Mouse Down event to "Prime"
 * the trackball and call ::GetResult() during your Motion events.
 * This will fill a 3dim array (4th param of function) which will
 * add the new angles to that array.  Essentially you'll have an
 * array that constantly gets updated with the new angle, don't
 * purge that array in your main application until you do another
 * Mouse Down event again.
 *
 * In your main application you rotate the scene on each draw via:
 * glRotatef(Angle[2],0,0,1);
 * glRotatef(Angle[1],0,1,0);
 * glRotatef(Angle[0],1,0,0);
 *
 * You must Pop() / Reset your scene each time since Angle[] has
 * the total sum of the angle.  Lastly, Once you've done a mouse
 * up event this implementation was designed so that you know
 * rotate all your vertices through those angles.  Essentially
 * during the trackball you're rotating the camera, on mouse up
 * you rotate the scene and the camera resets.
 ****************************************************************/


double VecLen(double *v) {
  double temp= (v[0]*v[0]) + (v[1]*v[1]) + (v[2]*v[2]);
  return(temp >= 0.0) ? sqrt(temp) : sqrt(-temp);
} //eof VecLen


void AxisToQuat(double *axis, double angle, double *quat) {
// Converts a rotation from axis to a quaternion
  double mag= VecLen(axis);

  if (mag == 0.0) {
    quat[0]= 0.0;
    quat[1]= 0.0;
    quat[2]= 0.0;
    quat[3]= 0.0;
  } else {
    double	sin_angle= sin(0.5 * angle);

    quat[0]= axis[0] * sin_angle / mag;
    quat[1]= axis[1] * sin_angle / mag;
    quat[2]= axis[2] * sin_angle / mag;
    quat[3]= cos(0.5 * angle);
  } //fi
} //eof AxistoQuat



void QuatToEuler(double *quat, double *euler) {
// Convert a Quaternion back to Euler Angles
  double	matrix[3][3];
  double	cx, cy, cz;
  double	sx, sy, sz;
  double	yr;

  // Convert quaternion(1 by 4) to matrix(3 by 3). Only convert values when we need to
  matrix[2][0] = (1.0 * quat[0] * quat[2]) - (2.0 * quat[3] * quat[1]);

  sy= -matrix[2][0];
  cy= sqrt(1.0 - (sy * sy));
  yr= atan2(sy,cy);
  euler[1]= NurbanaMath::RtoD(yr);

  // Avoid divide by zero error only where y= +-90 or +-270.
  // Not checking cy because of precision errors.
  if (sy != 1.0 && sy != -1.0) {
    matrix[0][0]= float(1.0 - (2.0 * quat[1] * quat[1]) - (2.0 * quat[2] * quat[2]));
    matrix[1][0]= float((2.0 * quat[0] * quat[1]) + (2.0 * quat[3] * quat[2]));
    matrix[2][1]= float((2.0 * quat[1] * quat[2]) + (2.0 * quat[3] * quat[0]));
    matrix[2][2]= float(1.0 - (2.0 * quat[0] * quat[0]) - (2.0 * quat[1] * quat[1]));

    cx= matrix[2][2] / cy;
    sx= matrix[2][1] / cy;
    cz= matrix[0][0] / cy;
    sz= matrix[1][0] / cy;

    euler[0]= NurbanaMath::RtoD(atan2(sx,cx));
    euler[2]= NurbanaMath::RtoD(atan2(sz,cz));
  } else {
    // cos(y) is 0, so assume z = 0.0
    matrix[1][1]= float(1.0 - (2.0 * quat[0] * quat[0]) - (2.0 * quat[2] * quat[2]));
    matrix[1][2]= float((2.0 * quat[1] * quat[2]) - (2.0 * quat[3] * quat[0]));

    euler[0]= NurbanaMath::RtoD(atan2(-matrix[1][2], matrix[1][1]));
    euler[2]= NurbanaMath::RtoD(atan2(0.0, 1.0));
  } //fi
} //eof QuattoEuler



void _tbPointToVector(int x, int y, int width, int height, double v[3]) {
  double	d,a;

  /* project x, y onto a hemi-sphere centered within width, height. */
  v[0]= float((2.0 * x - width) / width);
  v[1]= float((height - 2.0 * y) / height);
  d= float(sqrt(v[0] * v[0] + v[1] * v[1]));
  v[2]= float(cos((N_PI / 2.0) * ((d < 1.0) ? d : 1.0)));
  a= float(1.0 / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]));
  v[0]*= a;
  v[1]*= a;
  v[2]*= a;
} //eof _tbPointToVector



void Trackball::TrackballA(int x, int y, int WindowWidth, int WindowHeight, int Init, double Angle[3], float TB_SPEED) {
  static	double	tb_last_position[3], tb3D[3];;
  static	bool	Reset;
  double		tb_current_position[3], Quaternion[4], tb_axis[3], dx, dy, dz, tb_angle;


  if (Init) {
    _tbPointToVector(x, y, WindowWidth, WindowHeight, tb_last_position);
    Reset= 1;
  } else {


    _tbPointToVector(x, y, WindowWidth, WindowHeight, tb_current_position);

    // calculate the angle to rotate by (directly proportional to the
    // length of the mouse movement
    dx= tb_current_position[0] - tb_last_position[0];
    dy= tb_current_position[1] - tb_last_position[1];
    dz= tb_current_position[2] - tb_last_position[2];

    tb_angle= TB_SPEED * sqrt(dx*dx + dy*dy + dz*dz);

    if (tb_angle < TB_SPEED*1.6) { //So Trackball doesn't Blow up
      //This gets executed on the second call to motion();
      if (!Reset) {
        Angle[0]-= tb3D[0];
        Angle[1]-= tb3D[1];
        Angle[2]-= tb3D[2];
      } else { Reset= 0; } //fi

      // calculate the axis of rotation (cross product)
      tb_axis[0]= tb_last_position[1] * tb_current_position[2] -
                  tb_last_position[2] * tb_current_position[1];
      tb_axis[1]= tb_last_position[2] * tb_current_position[0] -
                  tb_last_position[0] * tb_current_position[2];
      tb_axis[2]= tb_last_position[0] * tb_current_position[1] -
                  tb_last_position[1] * tb_current_position[0];

      AxisToQuat(tb_axis,tb_angle,Quaternion);
      QuatToEuler(Quaternion,tb3D);

      Angle[0]= tb3D[0];
      Angle[1]= tb3D[1];
      Angle[2]= tb3D[2];
    } //fi

  } //fi

} //eof TrackballA::GetResult()


void Trackball::TrackballB(int x, int y, int Init, double Angle[3], double M[16], float TB_SPEED) {
  static	int	LastX,LastY;

  if (Init) {
    LastX= x;
    LastY= y;
  } //fi

  // Rotate up/down
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
    glRotated(Angle[2],0,0,1);
    glRotated(Angle[1],0,1,0);
    glRotated(Angle[0],1,0,0);
  glRotated(-(LastY-y)/(1/(TB_SPEED*.25)),1,0,0);
    glMultMatrixd(M);
  glGetDoublev(GL_MODELVIEW_MATRIX,M);

  // Rotate left/right
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
    glRotated(Angle[2],0,0,1);
    glRotated(Angle[1],0,1,0);
    glRotated(Angle[0],1,0,0);
    glMultMatrixd(M);
  glRotated(-(LastX-x)/(1/(TB_SPEED*.25)),0,0,1);
  glGetDoublev(GL_MODELVIEW_MATRIX,M);

  LastX= x;
  LastY= y;
} //eof Trackball::TrackballB()
