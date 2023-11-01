#include "NTK_Container.h"

void NTK_Container::ContainerDisplacement(int *X, int *Y, NTK_Container *C, bool Apply) {
  int	dX,dY;

  dX= dY= 0;
  while (C) {
    dX+= C -> _pX;
    dY+= C -> _pY;
    C= C -> _Container;
  } //eow

  if (Apply) {
    *X+= dX; *Y+= dY;
  } else {
    *X-= dX; *Y-= dY;
  } //fi
} //eof NTK_Container::_ContainerDisplacement()


bool NTK_Container::ContainerVisible(NTK_Container *C) {
  bool	Visible;

  Visible= 1;
  while (C) {
    if (C -> _Visible == NTK_CONTAINER_INVISIBLE) Visible= 0;
    C= C -> _Container;
  } //eow
  return(Visible);
} //eof NTK_Container::ContainerVisible()
