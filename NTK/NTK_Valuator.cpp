#include "NTK_Valuator.h"
#include "ntku_Math.h"


NTK_Valuator::NTK_Valuator(NTK_Window *win, int PosX, int PosY, int SizX, int SizY, unsigned char Type) : NTK_Base(win, PosX, PosY, SizX, SizY, Type) {
  _SliderColor[0]= _SliderColor[1]= _SliderColor[2]= 0xff;
  _Precision= _Min= _Max= _Value= 0;
  _Text[0]= NULL;
} //eof NTK_Slider::NTK_Slider()

NTK_Valuator::~NTK_Valuator() {}


void NTK_Valuator::Min(float Min) {
  _Min= Min;
} //eof NTK_Slider::Min()


void NTK_Valuator::Max(float Max) {
  _Max= Max;
} //eof NTK_Slider::Max()


float NTK_Valuator::Value() {
  int	i,Prec= 1;

  for (i= 0; i < _Precision; i++) Prec*= 10;
  return( float(int(_Value*Prec))/float(Prec) );
} //eof NTK_Valuator::Value()
