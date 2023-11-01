#include "NGUtils.h"


bool NGUtils::Inside(int ApX, int ApY, int BpX, int BpY, int BsX, int BsY) {
  // Check to see if ApX,ApY are inside of box defined by BpX,BpY,BsX,BsY
  if ((ApX >= BpX) && (ApX <= BpX+BsX) && (ApY >= BpY) && (ApY <= BpY+BsY))
    return(1);
  return(0);
} //eof NGUtils::Inside()


bool NGUtils::BoxInside(int ApX, int ApY, int AsX, int AsY, int BpX, int BpY, int BsX, int BsY) {
  // Check to see if box A is inside Box B (including if sides overlap)
  if ((ApX >= BpX) && (ApX+AsX <= BpX+BsX) && (ApY >= BpY) && (ApY+AsY <= BpY+BsY))  
    return(1);
  return(0);
} //eof NGUtilis::BoxInside()
