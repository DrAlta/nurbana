/****************************************************
 * NTK - Horizontal and Vertical Sliders
 * Start Date: Nov 6, 2000
 * Author: Justin Shumaker
 *
 * Description:
 * Draws Horizontal and Vertical Slider
 *****************************************************/


#ifndef NTK_Valuator_H
#define NTK_Valuator_H

#include <string.h>
#include "NTK_Base.h"
#include "ntku_Button.h"
#include "ntku_Math.h"


#define	NTK_HORZ_SLIDER	0
#define	NTK_VERT_SLIDER	1
#define	BorderSize	2


class NTK_Valuator : public NTK_Base {
  public:
    NTK_Valuator(NTK_Window *win, int PosX, int PosY, int SizX, int SizY, unsigned char Type);
    ~NTK_Valuator();

    void	Min(float);
    void	Max(float);
    void	Precision(unsigned char P) { _Precision= P; }
    void	Type(unsigned char T) { _Type= T; }
    void	SliderColor(unsigned char R, unsigned char G, unsigned char B) { _SliderColor[0]= R; _SliderColor[1]= G; _SliderColor[2]= B; }
    float	Value();
    void	Value(float V)
		{ if (V >= _Min) { _Value= V; } else { _Value= _Min; } _Update= 2; }


  protected:
    unsigned	char	_Type;
    char		_SliderColor[3];
    char		_Text[64];
    unsigned	char	_Precision;
    float		_Min;
    float		_Max;
    float		_Value;

}; //eof class NTK_Valuator
#endif
