/****************************************************
 * NTK - Scroll Bar
 * Start Date: Mar 12, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Creates and Manages a Vertical Scroll Bar
 ****************************************************/


#ifndef NTK_ScrollBar_H
#define NTK_ScrollBar_H


#include <string.h>
#include "NTK_Base.h"
#include "ntku_Button.h"

class NTK_ScrollBar : public NTK_Base {
  public:
    NTK_ScrollBar::NTK_ScrollBar(NTK_Window *win, int PosX, int PosY, int SizX, int SizY);
    NTK_ScrollBar::~NTK_ScrollBar();

    void	Draw();
    bool	Handle(unsigned char Event, int A, int B, int C, int D);
    void	Resize() {}

    void	Value(float v) { _Value= v; _Update= 2; }
    float	Value() { return (_Value); }
    void	SmallScroll(float V) { _SmallScroll= V; }

  private:
    int		_CDY;	//Click Delta Y
    float	_Value;
    float	_SmallScroll;
    bool	_SmallScrollState[2];

}; //eof class NTK_ScrollBar
#endif
