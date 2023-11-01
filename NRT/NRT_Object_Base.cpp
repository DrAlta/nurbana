#include "NRT_Object_Base.h"


NRT_Object_Base::NRT_Object_Base(int ID, NRT_Object_List *obj, char Type) {
  _ID= ID;
  _Type= Type;

  _Reflective= 0;
  _ReflectiveIndex= 0;
  _Transparent= 0;

  // Water:		1.33
  // Glass:		1.50
  // Glycerin:		1.47
  // Quartz Crystal:	1.54
  // Diamond:		2.42
  _RefractionIndex= 1.50;


  _Diffuse.Set(0,0,0);
  _Specular.Set(0,0,0);

  obj -> Push(this,ID);
} //eof Object_Base::Object_Base()
