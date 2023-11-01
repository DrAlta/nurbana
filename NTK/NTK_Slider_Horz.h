/****************************************************
 * NTK - Horizontal Sliders
 * Start Date:		Nov 6, 2000
 * Last Modified:	Jan 4, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Draws Horizontal Slider
 *****************************************************/


#ifndef NTK_Slider_Horz_H
#define NTK_Slider_Horz_H


#include "NTK_Valuator.h"
#include "NTK_Text.h"


class NTK_Slider_Horz : public NTK_Valuator {
  public:
    NTK_Slider_Horz(NTK_Window *win, int PosX, int PosY, int SizX, int SizY);
    ~NTK_Slider_Horz();

    void	Draw();
    bool	Handle(unsigned char Event, int A, int B, int C, int D);
    void	Resize() {}

  private:
    int		LM;


}; //eof class NTK_Slider_Horz
#endif
