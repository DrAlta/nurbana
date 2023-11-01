#include "NRT_Object_Manager.h"

NRT_Object_Manager::NRT_Object_Manager(unsigned short T) {
  int		i;

  _Threads= T;
  if (_Threads)
    _List= (NRT_Object_Manager**)malloc(sizeof(NRT_Object_Manager*)*_Threads);

  for (i= 0; i < _Threads; i++)
    _List[i]= new NRT_Object_Manager(0);

  _Head= NULL;
  if (!_Threads) _Head= new NRT_Object_List;
  IntersectedObjectID= 0;
} //eof NRT_Object_Manager::NRT_Object_Manager()


NRT_Object_Manager::~NRT_Object_Manager() {
  int	i;

  if (_Threads) {
    delete _Head;
    for (i= 0; i < _Threads; i++)
      delete _List[i];
    delete _List;
  } //fi
} //eof NRT_Object_Manager::~NRT_Object_Manager()


NRT_Object_List::ObjectNode* NRT_Object_Manager::GetObject(int ID) {
  if (_Head)
    return( _Head -> GetObjectPtr(ID) );
  return(NULL);
} //eof NRT_Object_Manager::GetObject()


void NRT_Object_Manager::CreateObject(int ID, char Type) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  int				i;

  if (_Threads) {
    for (i= 0; i < _Threads; i++)
      _List[i] -> CreateObject(ID,Type);
  } else {
    switch( Type ) {
      case NRT_OBJECT_TRIANGLE:
        new NRT_Object_Triangle(ID,_Head);
        break;

      case NRT_OBJECT_SPHERE:
        new NRT_Object_Sphere(ID,_Head);
        break;

      case NRT_OBJECT_NURBS:
        new NRT_Object_NURBS(ID,_Head);
        break;

      default:
        break;
    } //eos
  } //fi
} //eof NRT_Object_Manager()


bool NRT_Object_Manager::FindIntersection(Vec3d &StartPt, Vec3d &RayDir) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  double			Dist;
  int				i= 0;

  cur= _Head -> GetHead();
  IntersectedObjectID= 0;

  while (cur) {
//    printf("ID: %d\n",cur -> Object -> _ID);
    if (cur -> Object -> FindIntersection(StartPt,RayDir)) {
      if (!i) {
        IntersectedObjectID= cur -> Object -> _ID;
        Dist= cur -> Object -> _IntersectDist;
      } //fi
      if (cur -> Object -> _IntersectDist < Dist) {
        IntersectedObjectID= cur -> Object -> _ID;
        Dist= cur -> Object -> _IntersectDist;
      } //fi
      i++;
    } //fi
    cur= _Head -> GetObjectPtr(_Head -> GetNext(cur -> Object -> _ID));
  } //eow

  if (IntersectedObjectID) return(1);
  return(0);
} //eof NRT_Object_Manager::FindIntersection()


void NRT_Object_Manager::Diffuse(int ID, Vec3d &Diffuse) {
  NRT_Object_List::ObjectNode	*cur= NULL;

  if ((cur=GetObject(ID)))
    Diffuse= cur -> Object -> _Diffuse;
} //eof NRT_Object_Manager::Diffuse()


void NRT_Object_Manager::IntersectNormal(int ID, Vec3d &IntersectNormal) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  
  if ((cur=GetObject(ID)))
    IntersectNormal= cur -> Object -> _IntersectNormal;
} //eof NRT_Object_Manager::IntersecNormal()


void NRT_Object_Manager::IntersectPt(int ID, Vec3d &IntersectPt) {
  NRT_Object_List::ObjectNode	*cur= NULL;

  if ((cur=GetObject(ID)))
    IntersectPt= cur -> Object -> _IntersectPt;
} //eof NRT_Object_Manager::IntersecPt()


void NRT_Object_Manager::Position(int ID, double X, double Y, double Z) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  int				i;
  
  if (_Threads) {
    for (i= 0; i < _Threads; i++)
      _List[i] -> Position(ID,X,Y,Z);
  } else {
    if ((cur=GetObject(ID)))
      cur -> Object -> Position(X,Y,Z);
  } //fi
} //eof NRT_Object_Manager::Position()


void NRT_Object_Manager::Radius(int ID, double R) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  int				i;
  
  if (_Threads) {
    for (i= 0; i < _Threads; i++)
      _List[i] -> Radius(ID,R);
  } else {
    if ((cur=GetObject(ID)))
      cur -> Object -> Radius(R);
  } //fi
} //eof NRT_Object_Manager::Radius()


void NRT_Object_Manager::SetPt(int ID, int P, double X, double Y, double Z) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  int				i;
  
  if (_Threads) {
    for (i= 0; i < _Threads; i++)
      _List[i] -> SetPt(ID,P,X,Y,Z);
  } else {
    if ((cur=GetObject(ID)))
      cur -> Object -> SetPt(P,X,Y,Z);
  } //fi
} //eof NRT_Object_Manager::SetPt()


bool NRT_Object_Manager::CastShadowRay(Vec3d StartPt, Vec3d LightDir, double DistToLight) {
  // Cast a ray from the Point of intersection to the light source
  // and if any object is blocking the path from the ray to the
  // light source then this pixel is 'shadowed'
  NRT_Object_List::ObjectNode   *cur= NULL;
  double			Distance;

  cur= _Head -> GetHead();
  while (cur) {
    // For each object to for intersection and if there
    // is an intersection see if it's between the light
    // and the point of intersection

    if (cur -> Object -> IntersectDistance(StartPt, LightDir, Distance))
      if (Distance < DistToLight) {
//        printf("shadow\n");
        return(1);
      } //fi

    cur= _Head -> GetObjectPtr(_Head -> GetNext(cur -> Object -> _ID));
  } //eow

  return(0);
} //eof NRT_Object_Manager::CastShadowRay()


void NRT_Object_Manager::Diffuse(int ID, double R, double G, double B) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  int				i;

  if (_Threads) {
    for (i= 0; i < _Threads; i++)
      _List[i] -> Diffuse(ID,R,G,B);
  } else {
    if ((cur=GetObject(ID)))
      cur -> Object -> _Diffuse.Set(R,G,B);
  } //fi
} //eof NRT_Object_Manager::Diffuse()


void NRT_Object_Manager::Specular(int ID, double R, double G, double B) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  int				i;
  
  if (_Threads) {
    for (i= 0; i < _Threads; i++)
      _List[i] -> Specular(ID,R,G,B);
  } else {
    if ((cur=GetObject(ID)))
      cur -> Object -> _Specular.Set(R,G,B);
  } //fi
} //eof NRT_Object_Manager::Specular()


void NRT_Object_Manager::Specular(int ID, Vec3d &Specular) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  
  if ((cur=GetObject(ID))) {
    Specular= cur -> Object -> _Specular;
    return;
  } //fi

  Specular.Set(0,0,0);
} //eof NRT_Object_Manager::Specular()


void NRT_Object_Manager::ReflectiveIndex(int ID, double I) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  int				i;  

  if (_Threads) {
    for (i= 0; i < _Threads; i++)
      _List[i] -> ReflectiveIndex(ID,I);
  } else {
    if ((cur=GetObject(ID)))
      cur -> Object -> _ReflectiveIndex= I*15;
  } //fi
} //eof NRT_Object_Manager::ReflectiveIndex()


double NRT_Object_Manager::ReflectiveIndex(int ID) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  
  if ((cur=GetObject(ID)))
    return( cur -> Object -> _ReflectiveIndex );
} //eof NRT_Object_Manager::ReflectiveIndex()


void NRT_Object_Manager::Reflective(int ID, bool R) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  int				i;
  
  if (_Threads) {
    for (i= 0; i < _Threads; i++)
      _List[i] ->Reflective(ID,R);
  } else {
    if ((cur=GetObject(ID)))
      cur -> Object -> _Reflective= R;
  } //fi
} //eof NRT_Object_Manager::Reflective()


bool NRT_Object_Manager::Reflective(int ID) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  
  if ((cur=GetObject(ID)))
    return(cur -> Object -> _Reflective);
  return(0);
} //eof NRT_Object_Manager::Reflective()


void NRT_Object_Manager::Transparent(int ID, bool T) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  int				i;
  
  if (_Threads) {
    for (i= 0; i < _Threads; i++)
      _List[i] -> Transparent(ID,T);
  } else {
    if ((cur=GetObject(ID)))
      cur -> Object -> _Transparent= T;
  } //fi
} //eof NRT_Object_Manager::Transparent()


bool NRT_Object_Manager::Transparent(int ID) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  
  if ((cur=GetObject(ID)))
    return(cur -> Object -> _Transparent);
} //eof NRT_Object_Manager::Transparent()


void NRT_Object_Manager::RefractiveIndex(int ID, double I) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  int				i;
  
  if (_Threads) {
    for (i= 0; i < _Threads; i++)
      _List[i] -> RefractiveIndex(ID,I);
  } else {
    if ((cur=GetObject(ID)))
      cur -> Object -> _RefractionIndex= I;
  } //fi
} //eof NRT_Object_Manager::RefractiveIndex()


double NRT_Object_Manager::RefractiveIndex(int ID) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  
  if ((cur=GetObject(ID)))
    return(cur -> Object -> _RefractionIndex);
} //eof NRT_Object_Manager::RefractiveIndex()

//
// NURBS Functions
//

void NRT_Object_Manager::Order(int ID, bool UV, int V) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  
  if ((cur=GetObject(ID))) {
    cur -> Object -> Order(UV,V);
  } //fi
} //eof NRT_Object_Manager::Order()


void NRT_Object_Manager::Length(int ID, bool UV, int V) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  
  if ((cur=GetObject(ID))) {
    cur -> Object -> Length(UV,V);
  } //fi
} //eof NRT_Object_Manager::Length()


void NRT_Object_Manager::Knot(int ID, bool UV, char T) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  
  if ((cur=GetObject(ID))) {
    cur -> Object -> Knot(UV,T);
  } //fi
} //eof NRT_Object_Manager::Knot()


void NRT_Object_Manager::H(int ID, int I, float V) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  
  if ((cur=GetObject(ID))) {
    cur -> Object -> H(I,V);
  } //fi
} //eof NRT_Object_Manager::H()


void NRT_Object_Manager::Pt(int ID, int I, float V[3]) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  
  if ((cur=GetObject(ID))) {
    cur -> Object -> Pt(I,V);
  } //fi
} //eof NRT_Object_Manager::Pt()


void NRT_Object_Manager::CalculateSurface(int ID) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  
  if ((cur=GetObject(ID))) {
    cur -> Object -> CalculateSurface();
  } //fi
} //eof NRT_Object_Manager::CalculateSurface()
