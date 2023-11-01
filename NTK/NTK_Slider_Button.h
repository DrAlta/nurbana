/****************************************************
 * NTK - Button Slider
 * Start Date:		June 15, 2002
 * Last Modified:	x
 * Author: Justin Shumaker
 *
 * Description:
 * Draws Horizontal Slider
 *****************************************************/


#ifndef NTK_Slider_Button_H
#define NTK_Slider_Button_H


#include <string.h>
#include "NTK_Valuator.h"
#include "NTK_Text.h"
#include "NTK_TextInput.h"
#include "../KeyTable.h"


class NTK_Slider_Button : public NTK_Valuator {
  public:
    NTK_Slider_Button(NTK_Window *win, int PosX, int PosY, int SizX, int SizY);
    ~NTK_Slider_Button();

    void	Draw();
    bool	Handle(unsigned char Event, int A, int B, int C, int D);
    void	Resize() {}

    void	StaticText(char *T);


    void	Step(float S)
		{ _Step= S; }

  private:
    NTK_TextInput	*_TextInput;

    char		_StaticText[64];
    int			_MouseX;
    float		_Step;
}; //eof class NTK_Slider_Button
#endif
