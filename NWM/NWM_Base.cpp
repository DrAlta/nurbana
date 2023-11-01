#include "NWM_Base.h"


NWM_Base::NWM_Base(NWM_List *obj, char Type) {
  _Type= Type;
  _Visible= 1;
  _Layer= 0;
  _Update= 2;		// Allocate (1) Front/Back Buffer Swap
  _Event= 1;
  _Splitable= 0;	// By Default windows Cannot be split

  _Replacement= 0;
  _SelfDestruct= 0;

  _PosX= 0;
  _PosY= 0;
  _SizX= 0;
  _SizY= 0;

  _H1= 0;
  _H2= 0;
  _H3= 0;
  _H4= 0;

  _MinSizX= 0;
  _MinSizY= 0;

  obj -> Push(this);
} //eof NWM_Base::NWM_Base()
