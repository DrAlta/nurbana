#include "NTK_Slider_Horz.h"
#include "math.h"


NTK_Slider_Horz::NTK_Slider_Horz(NTK_Window *win, int PosX, int PosY, int SizX, int SizY) : NTK_Valuator(win, PosX, PosY, SizX, SizY, NTK_WT_Slider_Horz) {
} //eof NTK_Slider::NTK_Slider()

NTK_Slider_Horz::~NTK_Slider_Horz() {}




void NTK_Slider_Horz::Draw() {
  int		digitsV,digitsM,dispX,i,Prec;
  char		temp[32];
  char		Color[3]={0,0,0};
  float		min, max, valueB;
  /* 3--2
   * |  |
   * 4--1
   */

  min= 0;
  max= _Max - _Min;
  if (!max) max= 1;
  valueB= _Value - _Min;

    digitsV= ntku_Math::NumMag(_Value);
    digitsM= ntku_Math::NumMag(max);

    if (_Precision) {
      dispX= (digitsM+1+_Precision+1)*FontWidth;
    } else {
      dispX= (digitsM+_Precision+1)*FontWidth;
    } //fi

    // Button
    ntku_Button::Rect(_pX, _pY, _sX, _sY, _Color, 2, 0);
    // Crevice
    ntku_Button::Rect(_pX+dispX,_pY+_sY*.3,(_pX+_sX*.95)-(_pX+dispX),_sY*.4,Color,2,1);
    // Slider Button
    ntku_Button::Rounded(dispX+_pX+(_sX*.95-dispX)*(valueB/max),_pY+_sY*.2,8,_sY*.6,_SliderColor,2,0);

    strcpy(temp,"%.");
    sprintf(_Text,"%d",_Precision);
    strcat(temp,_Text);
    strcat(temp,"f");

    Prec= 1;
    for (i= 0; i < _Precision; i++) Prec*= 10;

    sprintf(_Text, temp, float(int(_Value*Prec))/float(Prec));

    _Font -> Draw(_pX,_pY,_sX,_sY,_Text,NTK_JUSTIFY_LEFT,_Color,_State);
} //eof NTK_Slider_Horz::Draw()



bool NTK_Slider_Horz::Handle(unsigned char Event, int A, int B, int C, int D) {
  int		digitsM, dispX;
  float		valueA;
  float		min, max, valueB;

  min= 0;
  max= _Max - _Min;
  valueB= _Value - _Min;

  switch( Event) {
    case NTK_EVENT_MOUSE:
      if (B) {
        digitsM= ntku_Math::NumMag(max);

        if (_Precision) {
          dispX= (digitsM+1+_Precision+1)*FontWidth;
        } else {
          dispX= (digitsM+1)*FontWidth;
        } //fi

        if ((C >= (dispX+_pX+(_sX*.95-dispX)*(valueB/max)-4)) &&
            (C <= (dispX+_pX+(_sX*.95-dispX)*(valueB/max)+4)) &&
            (D >= (_pY+_sY*.2)) &&
            (D <= (_pY+_sY*.8))) {
          _State= 1;
          _Update= 2;
          LM= C;
        } //fi
      } else {
         _State= 0;
         _Update= 2;
         break;
     } //fi
     break;

    case NTK_EVENT_MOTION:
      if (_State) { //IF BUTTON IS ON


        digitsM= ntku_Math::NumMag(max);

        if (_Precision) {
          dispX= (digitsM+1+_Precision+1)*FontWidth;
        } else {
          dispX= (digitsM+1)*FontWidth;
        } //fi

        //Just testing X's cause mouse click verified slider is active
        if ((A >= _pX+dispX) && (A <= _pX+_sX*.95)) { //IF MOUSE IS WITHIN SLIDER LIMITS
          //Update Slider
          _Update= 2;

          //Add or Subtract the difference proportional to last mouse movement
          valueA= (A-LM)/(_sX*.95-dispX);
          valueA*= max;

          if ((valueA+valueB < max) && (valueA+valueB > min)) {
            valueB+= valueA;
          } else {
            if (valueA+valueB >= max) {
              valueB= max;
            } else if (valueA+valueB <= min) {
              valueB= min;
            } //fi
          } //fi
        } else if (A >= _pX+_sX*.95) {
          //If the mouse _IS_ out of slider boundaries, then assign its
          //value to the Max or Min respectively.  This is good if the
          //mouse _exits_ the slider region before reaching its respective
          //Max or Min value.
          valueB= max;
          _Update= 2;
        } else if (A <= _pX+dispX) {
          //Same as above
          valueB= min;
          _Update= 2;
        } //fi
	_Value= valueB + _Min;
      } //fi

      LM= A;
      break;

    default:
      break;
  } //eos
  return(0);
} //eof NTK_Slider_Horz::Handle()
