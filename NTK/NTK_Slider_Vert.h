/****************************************************
 * NTK -  Vertical Sliders
 * Start Date:		Nov 6, 2000
 * Last Modified:	Jan 4, 2001
 * Author: Jim Theriault
 *
 * Description:
 * Draws Vertical Slider
 *****************************************************/


#ifndef NTK_Slider_Vert_H
#define NTK_Slider_Vert_H


#include "NTK_Valuator.h"
#include "NTK_Text.h"


class NTK_Slider_Vert : public NTK_Valuator {
  public:
    NTK_Slider_Vert(NTK_Window *win, int PosX, int PosY, int SizX, int SizY);
    ~NTK_Slider_Vert();

    void	Draw();
    bool	Handle(char Event, int A, int B, int C, int D);
    void	Resize() {}

  private:
    int		LM;


}; //eof class NTK_Slider_Vert
#endif
