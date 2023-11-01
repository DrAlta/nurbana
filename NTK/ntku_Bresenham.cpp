#include "ntku_Bresenham.h"


void ntku_Bresenham::Render(unsigned char *Bitmap, int W, int H, int X1, int Y1, int X2, int Y2, unsigned char A) {
  // Note bottom Left is 0, Top right is end of array.  Array proceedes right
  // instead of up.  LHS is i*W and RHS is i*(W-1)
  // Extension of Bresenham's line drawing algorithm
  int	l,dy,dx,stepx,stepy,fraction;

  dy= Y2-Y1;
  dx= X2-X1;

  if (dy < 0) { dy = -dy;  stepy = -1; } else { stepy = 1; }
  if (dx < 0) { dx = -dx;  stepx = -1; } else { stepx = 1; }
  dy <<= 1;	// dy is now 2*dy
  dx <<= 1;	// dx is now 2*dx

  l= (X1+Y1*W);
  Bitmap[l]= A;

  if (dx > dy) {
    fraction = dy - (dx >> 1);	// same as 2*dy - dx
    while (X1 != X2) {
      if (fraction >= 0) {
        Y1 += stepy;
        fraction -= dx;	// same as fraction -= 2*dx
      } //fi
      X1 += stepx;
      fraction += dy;	// same as fraction -= 2*dy

      l= (X1+Y1*W);
      Bitmap[l]= A;
    }
  } else {
    fraction = dx - (dy >> 1);
    while (Y1 != Y2) {
      if (fraction >= 0) {
        X1 += stepx;
        fraction -= dy;
      } //fi
      Y1 += stepy;
      fraction += dx;

      l= (X1+Y1*W);
      Bitmap[l]= A;
    } //eow
  } //fi
} //eof Bresenham::Render()
