/****************************************************
 * Border Select Class
 * Start Date: Dec 15, 2000
 * Author: Justin Shumaker
 *
 * Description:
 * handles the two phases of Border Select:
 * A. CrossHair
 * B. Box
 *
 * Circle may be implemented later
 *****************************************************/

#include "NTK_Base.h"

NTK_Base::NTK_Base() {}


NTK_Base::NTK_Base(NTK_Window *win, int pX, int pY, int sX, int sY, unsigned char Type) {
  _Type= Type;

  _State= NTK_OFF;
  _Visible= NTK_ON;

  _pX= pX;
  _pY= pY;
  _sX= sX;
  _sY= sY;

  _DynPos= 0;
  _DynSiz= 0;

  _DynPosX= 0;
  _DynPosY= 0;
  _DynSizX= 0;
  _DynSizY= 0;

  _ScaleX= 0;
  _ScaleY= 0;

  _Rounded= 0;
  _Anchored= 0;

  _Layer= 0;
  _Update= 2;
  _Color[0]= _Color[1]= _Color[2]= (unsigned char)0x80;
  _CallbackTrigger= 0;
  _callback= NULL;
  _Container= NULL;

  _Font= win -> Font();
  _Window= win -> Window();

  win -> Push(this);
} //eof NTK_Base::NTK_Base()


//NTK_Base::~NTK_Base() {}
