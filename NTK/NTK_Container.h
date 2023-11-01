/****************************************************
 * NTK - Button
 * Start Date: Dec 7, 2000
 * Author: Justin Shumaker
 *
 * Description:
 * A Container to groups Widgets in.
 ****************************************************/


#ifndef NTK_Container_H
#define NTK_Container_H

#include "ntku_include.h"

#define NTK_CONTAINER_INVISIBLE	0
#define	NTK_CONTAINER_VISIBLE	1

class NTK_Container {
  public:
    NTK_Container::NTK_Container(int pX, int pY) { _Visible= NTK_CONTAINER_VISIBLE; _pX= pX; _pY= pY; _Container= NULL; }
    NTK_Container::~NTK_Container();

    void	Position(int pX, int pY) { _pX= pX; _pY= pY; }
    void	Container(NTK_Container *C) { _Container= C; }
    void	Visibility(bool V) { _Visible= V; }
    static	void	ContainerDisplacement(int *X, int *Y, NTK_Container *C, bool Apply);
    static	bool	ContainerVisible(NTK_Container *C);

  protected:
    bool		_Visible;
    int			_pX;
    int			_pY;
    NTK_Container	*_Container;

}; //eof class NTK_Container
#endif
