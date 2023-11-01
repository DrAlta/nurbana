#include "NRT_Light_Manager.h"

NRT_Light_Manager::NRT_Light_Manager(unsigned short T) {
  int		i;

  _Threads= T;
  if (_Threads)
    _List= (NRT_Light_Manager**)malloc(sizeof(NRT_Light_Manager*)*_Threads);

  for (i= 0; i < _Threads; i++)
    _List[i]= new NRT_Light_Manager(0);

  _Head= NULL;
  if (!_Threads) _Head= new NRT_Light_List;
  _ObjectList= NULL;
} //eof NRT_Light_Manager::NRT_Light_Manager()


NRT_Light_Manager::~NRT_Light_Manager() {
  int	i;

  if (_Threads) {
    delete _Head;
    for (i= 0; i < _Threads; i++)
      delete _List[i];
    delete _List;
  } //fi
} //eof NRT_Light_Manager::~NRT_Light_Manager()


NRT_Light_List::LightNode* NRT_Light_Manager::GetLight(int ID) {
  if (_Head)
    return( _Head -> GetLightPtr(ID) );
  return(NULL);
} //eof NRT_Light_Manager::GetLight()


void NRT_Light_Manager::CreateLight(int ID, char Type, double X, double Y, double Z) {
  NRT_Light_List::LightNode	*cur= NULL;
  int				i;

  if (_Threads) {
    for (i= 0; i < _Threads; i++)
      _List[i] -> CreateLight(ID,Type,X,Y,Z);
  } else {
    new NRT_Light_Sphere(ID,_Head,X,Y,Z);
  } //fi

//  cur= GetLight(Head -> GetNewObjectID());
//  cur -> Object -> RecalculateKnotVector(1);
} //eof NRT_Light_Manager()


void NRT_Light_Manager::CalculateLighting(Vec3d &Color, int Jitter, Vec3d &RayDir) {
  NRT_Light_List::LightNode	*cur= NULL;

  cur= _Head -> GetHead();
  while (cur) {
    cur -> Object -> CalcContribLight(Color, _ObjectList, Jitter, RayDir);
    cur= _Head -> GetLightPtr(_Head -> GetNext(cur -> Object -> _ID));
  } //eow
} //eof NRT_Light_Manager::CalculateLighting()


void NRT_Light_Manager::Radius(int ID, double R) {
  NRT_Light_List::LightNode	*cur= NULL;
  int				i;

  if (_Threads) {
    for (i= 0; i < _Threads; i++)
      _List[i] -> Radius(ID,R); 
  } else {
    if ((cur=GetLight(ID)))
      cur -> Object -> Radius(R);
  } //eof
} //eof NRT_Light_Manager::Radius()


void NRT_Light_Manager::Intensity(int ID, double R, double G, double B) {
  NRT_Light_List::LightNode	*cur= NULL;
  int				i;  

  if (_Threads) {
    for (i= 0; i < _Threads; i++)
      _List[i] -> Intensity(ID,R,G,B);
  } else {
    if ((cur=GetLight(ID)))
      cur -> Object -> Intensity(R,G,B);
  } //eof
} //eof NRT_Light_Manager::Intensity()
