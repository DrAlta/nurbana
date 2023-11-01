#include "NRT_Light_Base.h"


NRT_Light_Base::NRT_Light_Base(int ID, NRT_Light_List *obj, double X, double Y, double Z, char Type) {
  _ID= ID;

  _Position.X= X;
  _Position.Y= Y;
  _Position.Z= Z;

  _Type= Type;
//  _ObjectList= OM;

  obj -> Push(this,ID);
} //eof NRT_Light_Base::NRT_Light_Base()
