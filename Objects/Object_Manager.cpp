#include "Object_Manager.h"


Object_Manager::Object_Manager(MaterialList *MaterialList) {
  _Mode= 0;
  _ActiveID= 0;
  _MaterialList= MaterialList;

  _Smooth= 1;
  _FaceType= 1;

  _Normals= 0;
  _NormalLength= 0.1f;


  Main= new Object_List;
  Back= new Object_List;
  Head= Main;

  NURBS::OBJList= Head;

  // We're placing ModelView stuff here, because all ModelViews
  // have the same ObjectList in common.
  _BGColor[0]= 0.45f;
  _BGColor[1]= 0.45f;
  _BGColor[2]= 0.45f;

  _GRIDColor[0]= 0.3f;
  _GRIDColor[1]= 0.3f;
  _GRIDColor[2]= 0.3f;

  _FOV= 52;

  _Trackball= 1;
  _TBSpeed= 1.25;

  _Init= 1;

  _SLHead= NULL;
  _Notes= (char*)malloc(sizeof(char)*1);
  _Notes[0]= NULL;
} //eof Constructor



Object_Manager::~Object_Manager() {
  //Delete all Objects
/*
  ObjectData *ptr;
 
  while (Head != NULL) {
    ptr= Head -> Next;
    delete Head;
    Head= ptr;
  } //eow

*/
} //eof Deconstructor


Object_Base* Object_Manager::GetObj() {
  return( GetObject(_ActiveID) -> Object );
} //eof Object_Manager::GetObj()


unsigned int Object_Manager::GetObjCount() {
  unsigned	int	Count= 0;

  _obj= Head -> GetHead();
  while (_obj) {
    Count++;
    _obj= Head -> GetObjectPtr(Head -> GetNext(_obj -> Object -> _ID));
  } //eow

  return(Count);
} //eof Object_Manager::GetObjCount()


bool Object_Manager::GetObjAct() {
  _obj= Head -> GetHead();

  while (_obj) {
    if (_obj -> Object -> _Active) return(1);
    _obj= Head -> GetObjectPtr(Head -> GetNext(_obj -> Object -> _ID));
  } //eow

  return(0);
} //eof Object_Manager::GetObjAct()


void Object_Manager::SetObjActAll(bool A) {
  _obj= Head -> GetHead();

  while (_obj) {
    _obj -> Object -> _Active= A;
    _obj= Head -> GetObjectPtr(Head -> GetNext(_obj -> Object -> _ID));
  } //eow
} //eof Object_Manager::SetObjActAll()


unsigned int Object_Manager::IDList(unsigned int *ID_LIST) {
  unsigned	int	Count= 0;

  _obj= Head -> GetHead();

  while (_obj) {
    ID_LIST[Count]= _obj -> Object -> _ID;
    Count++;
    _obj= Head -> GetObjectPtr(Head -> GetNext(_obj -> Object -> _ID));
  } //eow

  return(Count);
} //eof Object_Manager::IDList()


bool Object_Manager::GetActive() {
  return (_obj=GetObject(_ActiveID)) ? _obj -> Object -> _Active : 0;
} //eof Object_Manager::Active()


void Object_Manager::SetActive(bool State, bool Purge) {
  SelectionList			*sl;
  SelectionList			*sltmp;

  // Purge List
  if (Purge) {
    while (_SLHead) {
      sl= _SLHead -> Next;
      delete sl;
      _SLHead= sl;
    } //eof
  } //eof Purge

  if ((_obj=GetObject(_ActiveID))) {
    if (_obj -> Object -> _Active != State) {
      _obj -> Object -> _Active= State;

      // If State, Push ID into Selection List, else Remove
      if (State) {
        // Push Node
        if (_SLHead) {
          sl= _SLHead;
          _SLHead= new SelectionList;
          _SLHead -> Next= sl;     
        } else {
          _SLHead= new SelectionList;
          _SLHead -> Next= NULL;
        } //fi
        _SLHead -> ID= _ActiveID;
      } else {
        // Pop Node
        if (_SLHead) {
          if (_SLHead -> ID == _ActiveID) {
            sl= _SLHead -> Next;
            delete _SLHead;
            _SLHead= sl;
          } else {
            sl= _SLHead;
            while (sl -> Next) {
              if (sl -> Next -> ID == _ActiveID) {
                sltmp= sl -> Next;
                sl -> Next= sl -> Next -> Next;
                delete sltmp;
                break;
              } //fi
              sl= sl -> Next;
            } //eow
          } //fi
        } //fi
      } //fi
    } //fi
  } //fi
} //eof Object_Manager::Active()


char* Object_Manager::GetName() {
  if ((_obj=GetObject(_ActiveID)))
    return(_obj -> Object -> _Name);
  return("None");
} //eof Object_Manager::GetName()


void Object_Manager::SetName(char *Name) {
  if ((_obj=GetObject(_ActiveID)))
    strcpy(_obj -> Object -> _Name,Name);
} //eof Object_Manager::SetName();


void Object_Manager::CenterCOR() {
  Point3d	Min,Max;
  int		i;

  if ((_obj=GetObject(_ActiveID))) {
    Min.x= Max.x= _obj -> Object -> CtlPts()[0].x;
    Min.y= Max.y= _obj -> Object -> CtlPts()[0].y;
    Min.z= Max.z= _obj -> Object -> CtlPts()[0].z;

    for (i= 1; i < _obj -> Object -> Length(0) * _obj -> Object -> Length(1); i++) {
      if (_obj -> Object -> CtlPts()[i].x > Min.x) Min.x= _obj -> Object -> CtlPts()[i].x;
      if (_obj -> Object -> CtlPts()[i].y > Min.y) Min.y= _obj -> Object -> CtlPts()[i].y;
      if (_obj -> Object -> CtlPts()[i].z > Min.z) Min.z= _obj -> Object -> CtlPts()[i].z;

      if (_obj -> Object -> CtlPts()[i].x < Max.x) Max.x= _obj -> Object -> CtlPts()[i].x;
      if (_obj -> Object -> CtlPts()[i].y < Max.y) Max.y= _obj -> Object -> CtlPts()[i].y;
      if (_obj -> Object -> CtlPts()[i].z < Max.z) Max.z= _obj -> Object -> CtlPts()[i].z;
    } //eof

    // Update COR
    Max.x= (Max.x+Min.x)/2.0;
    Max.y= (Max.y+Min.y)/2.0;
    Max.z= (Max.z+Min.z)/2.0;
    
    _obj -> Object -> COR(Max);
  } //fi

  SetRecalcTess();
} //eof Object_Manager::CenterCOR()


void Object_Manager::CenterPoints() {
  Point3d			Min,Max;
  int				i;

  if ((_obj=GetObject(_ActiveID))) {
    Min.x= Max.x= _obj -> Object -> CtlPts()[0].x;
    Min.y= Max.y= _obj -> Object -> CtlPts()[0].y;
    Min.z= Max.z= _obj -> Object -> CtlPts()[0].z;

    for (i= 0; i < _obj -> Object -> Length(0) * _obj -> Object -> Length(1); i++) {
      if (_obj -> Object -> CtlPts()[i].x > Min.x) Min.x= _obj -> Object -> CtlPts()[i].x;
      if (_obj -> Object -> CtlPts()[i].y > Min.y) Min.y= _obj -> Object -> CtlPts()[i].y;
      if (_obj -> Object -> CtlPts()[i].z > Min.z) Min.z= _obj -> Object -> CtlPts()[i].z;

      if (_obj -> Object -> CtlPts()[i].x < Max.x) Max.x= _obj -> Object -> CtlPts()[i].x;
      if (_obj -> Object -> CtlPts()[i].y < Max.y) Max.y= _obj -> Object -> CtlPts()[i].y;
      if (_obj -> Object -> CtlPts()[i].z < Max.z) Max.z= _obj -> Object -> CtlPts()[i].z;
    } //eof

    // Update Control Points
    for (i= 0; i < _obj -> Object -> _LengthUV[0] * _obj -> Object -> _LengthUV[1]; i++) {
      _obj -> Object -> CtlPts()[i].x-= (Max.x+Min.x)/2.0;
      _obj -> Object -> CtlPts()[i].y-= (Max.y+Min.y)/2.0;
      _obj -> Object -> CtlPts()[i].z-= (Max.z+Min.z)/2.0;
    } //eof
  } //fi

  SetRecalcTess();
} //eof Object_Manager::CenterPoints()

/*
void Object_Manager::SelectInvert() {
  if ((_obj=GetObject(ID)))
    for (int i= 0; i < _obj -> Object -> Length(0) * _obj -> Object -> Length(1); i++)
      _obj -> Object -> _ActPts[i]= !_obj -> Object -> _ActPts[i];
} //eof Object_Manager::SelectInvert()
*/

bool CheckRow(int Row, Object_List::ObjectNode *cur) {
  //Checks if the specified Row is active and all other vertices
  //are inactive
  int i,test= 1;

  for (i=0; i < cur -> Object -> _LengthUV[0]; i++) {
    if (!cur -> Object -> _ActPts[i*cur -> Object -> _LengthUV[1]+Row]) test= 0;
  } //eof
  if (test) {
    for (i= 0; i < cur -> Object -> _LengthUV[0] * cur -> Object -> _LengthUV[1]; i++) {
      if ((i+cur -> Object -> _LengthUV[1])%cur -> Object -> _LengthUV[1]!=Row) { //doesn't equal one of the specified row points
        if (cur -> Object -> _ActPts[i]) test= 0;
      } //fi
    } //eof
  } //fi
  return(test);
} //eof CheckRow()


bool CheckCol(int Col, Object_List::ObjectNode *cur) {
  int i,test= 1;

  for (i= 0; i < cur -> Object -> _LengthUV[1]; i++) {
//    if (!cur -> ActPts[i+Col*cur -> LengthUV[0]]) test= 0;
    if (!cur -> Object -> _ActPts[i+Col*cur -> Object -> _LengthUV[1]]) test= 0;
  } //eof
  if (test) {
    for (i= 0; i < cur -> Object -> _LengthUV[0] * cur -> Object -> _LengthUV[1]; i++) {
      if ((i < Col*cur -> Object -> _LengthUV[1]) || (i > (Col+1)*cur -> Object -> _LengthUV[1])) { //doesn't equal one of the specified col points
        if (cur -> Object -> _ActPts[i]) test= 0;
      } //fi
    } //eof
  } //fi
  return(test);
} //eof CheckCol()


bool Object_Manager::Extrude() {
  bool				PASS= 0;
  char				CRS= 0;
  int				i,k;
  Point3d			*Buffer;
  double			*H;

  Buffer= (Point3d*)malloc(sizeof(Point3d)*1024);
  H= (double*)malloc(sizeof(double)*1024);

  if ((_obj=GetObject(_ActiveID))) {

    if (CheckRow(_obj -> Object -> Length(1)-1,_obj)) {
      // TOP
      // EXTRUDE
      // Add a new set of points to the top
      k= 0;
      for (i= 0; i < _obj -> Object -> _LengthUV[0] * _obj -> Object -> _LengthUV[1]; i++) {
        Buffer[i+k].x= _obj -> Object -> _CtlPts[i].x;
        Buffer[i+k].y= _obj -> Object -> _CtlPts[i].y;
        Buffer[i+k].z= _obj -> Object -> _CtlPts[i].z;
        if (_obj -> Object -> _OBType == OBJECT_NURBS) H[i+k]= ((Object_NURBS*)_obj -> Object) -> H()[i];
        if (!((i+1)%_obj -> Object -> _LengthUV[1])) {
          k++;
          Buffer[i+k].x= _obj -> Object -> _CtlPts[i].x;
          Buffer[i+k].y= _obj -> Object -> _CtlPts[i].y;
          Buffer[i+k].z= _obj -> Object -> _CtlPts[i].z;
          if (_obj -> Object -> _OBType == OBJECT_NURBS) H[i+k]= ((Object_NURBS*)_obj -> Object) -> H()[i];
        } //fi
      } //eof

      // Increment # of Rows (V)
      _obj -> Object -> _LengthUV[1]++;
      //Copy Buffer into Orignal Now and Deactivate All Vertices

      //Activate the Top Row of Vertices in GrabMode
      CRS= 0;
      PASS= 1;
    } else if (CheckRow(0,_obj)) {
      // BOTTOM
      // EXTRUDE
      // Move all the points up one, and add a new set of points
      // on the bottom.
      k= 0;
      for (i= 0; i < _obj -> Object -> _LengthUV[0] * _obj -> Object -> _LengthUV[1]; i++) {
        if (!((i+_obj -> Object -> _LengthUV[1])%_obj -> Object -> _LengthUV[1])) {
          Buffer[i+k].x= _obj -> Object -> _CtlPts[i].x;
          Buffer[i+k].y= _obj -> Object -> _CtlPts[i].y;
          Buffer[i+k].z= _obj -> Object -> _CtlPts[i].z;
          if (_obj -> Object -> _OBType == OBJECT_NURBS) H[i+k]= ((Object_NURBS*)_obj -> Object) -> H()[i];
          k++;
        } //fi
        Buffer[i+k].x= _obj -> Object -> _CtlPts[i].x;
        Buffer[i+k].y= _obj -> Object -> _CtlPts[i].y;
        Buffer[i+k].z= _obj -> Object -> _CtlPts[i].z;
        if (_obj -> Object -> _OBType == OBJECT_NURBS) H[i+k]= ((Object_NURBS*)_obj -> Object) -> H()[i];
      } //eof

      //Increment # of Rows (V)
      _obj -> Object -> _LengthUV[1]++;

      //Activate the Bottom Row of Vertices in GrabMode
      CRS= 1;
      PASS= 1;
    } else if (CheckCol(0,_obj)) {
      // LEFT
      // EXTRUDE
      // Move all the points right one, and add a new set of points
      // on the left
      k= 0;
      for (i= 0; i < _obj -> Object -> _LengthUV[0] * _obj -> Object -> _LengthUV[1]; i++) {
        if (i < _obj -> Object -> _LengthUV[1]) {
          Buffer[i].x= _obj -> Object -> _CtlPts[i].x;
          Buffer[i].y= _obj -> Object -> _CtlPts[i].y;
          Buffer[i].z= _obj -> Object -> _CtlPts[i].z;
          if (_obj -> Object -> _OBType == OBJECT_NURBS) H[i]= ((Object_NURBS*)_obj -> Object) -> H()[i];
        } //fi
        Buffer[i+_obj -> Object -> _LengthUV[1]].x= _obj -> Object -> _CtlPts[i].x;
        Buffer[i+_obj -> Object -> _LengthUV[1]].y= _obj -> Object -> _CtlPts[i].y;
        Buffer[i+_obj -> Object -> _LengthUV[1]].z= _obj -> Object -> _CtlPts[i].z;
        if (_obj -> Object -> _OBType == OBJECT_NURBS) H[i+_obj -> Object -> _LengthUV[1]]= ((Object_NURBS*)_obj -> Object) -> H()[i];
      } //eof

      //Increment # of Cols (U)
      _obj -> Object -> _LengthUV[0]++;

      //Activate the Left Row of Vertices in GrabMode
      CRS= 2;
      PASS= 1;
    } else if (CheckCol(_obj -> Object -> _LengthUV[0]-1,_obj)) {
      // RIGHT
      // EXTRUDE
      // Move all the points right one, and add a new set of points
      // on the left
      k= 0;
      for (i= 0; i < _obj -> Object -> _LengthUV[0] * _obj -> Object -> _LengthUV[1]; i++) {
        if (i >= _obj -> Object -> _LengthUV[0] * _obj -> Object -> _LengthUV[1] - _obj -> Object -> _LengthUV[1]) {
          Buffer[i+_obj -> Object -> _LengthUV[1]].x= _obj -> Object -> _CtlPts[i].x;
          Buffer[i+_obj -> Object -> _LengthUV[1]].y= _obj -> Object -> _CtlPts[i].y;
          Buffer[i+_obj -> Object -> _LengthUV[1]].z= _obj -> Object -> _CtlPts[i].z;
          if (_obj -> Object -> _OBType == OBJECT_NURBS) H[i+_obj -> Object -> _LengthUV[1]]= ((Object_NURBS*)_obj -> Object) -> H()[i];
        } //fi
        Buffer[i].x= _obj -> Object -> _CtlPts[i].x;
        Buffer[i].y= _obj -> Object -> _CtlPts[i].y;
        Buffer[i].z= _obj -> Object -> _CtlPts[i].z;
        if (_obj -> Object -> _OBType == OBJECT_NURBS) H[i]= ((Object_NURBS*)_obj -> Object) -> H()[i];
      } //eof

      //Increment # of Cols (U)
      _obj -> Object -> _LengthUV[0]++;

      //Activate the Right Row of Vertices in GrabMode
      CRS= 3;
      PASS= 1;
    } //fi
  } //fi

  if (PASS) {
    // Copy Buffer into Orignal Now and Deactivate All Vertices
    ((Object_NURBS*)_obj -> Object) -> SetLength(_obj -> Object -> _LengthUV[0],_obj -> Object -> _LengthUV[1]);
    for (i= 0; i < _obj -> Object -> _LengthUV[0] * _obj -> Object -> _LengthUV[1]; i++) {
      _obj -> Object -> _CtlPts[i].x= Buffer[i].x;
      _obj -> Object -> _CtlPts[i].y= Buffer[i].y;
      _obj -> Object -> _CtlPts[i].z= Buffer[i].z;
      if (_obj -> Object -> _OBType == OBJECT_NURBS) ((Object_NURBS*)_obj -> Object) -> H()[i]= H[i];
      _obj -> Object -> _ActPts[i]= 0;
    } //eof

    switch( CRS ) {
      case 0:
      for (i= 1; i <= _obj -> Object -> _LengthUV[0]; i++)
        _obj -> Object -> _ActPts[i*_obj -> Object -> _LengthUV[1]-1]= 1;
        break;

      case 1:
      for (i= 0; i < _obj -> Object -> _LengthUV[0]; i++)
        _obj -> Object -> _ActPts[i*_obj -> Object -> _LengthUV[1]]= 1;
        break;

      case 2:
      for (i= 0; i < _obj -> Object -> _LengthUV[1]; i++)
        _obj -> Object -> _ActPts[i]= 1;
        break;

      case 3:
      for (i= 0; i < _obj -> Object -> _LengthUV[1]; i++)
        _obj -> Object -> _ActPts[_obj -> Object -> _LengthUV[0] * _obj -> Object -> _LengthUV[1] - _obj -> Object -> _LengthUV[1] + i]= 1;
        break;

      default:
        break;
    } //eos

    if (_obj -> Object -> _OBType == OBJECT_NURBS) ((Object_NURBS*)_obj -> Object) -> RecalculateKnotVector(1);
    _obj -> Object -> _ReCalcTssPts= 1;
  } //fi

  return(PASS);
} //eof Object_Manager::Extrude()


bool Object_Manager::DeExtrude() {
  int	i,k;
  bool	PASS= 0;

  if ((_obj=GetObject(_ActiveID))) {
    if (CheckRow(_obj -> Object -> _LengthUV[1]-1,_obj)) { //TOP
      //Decrement # of Rows (V)
      k= 0;
      for (i= 0; i < _obj -> Object -> _LengthUV[0] * _obj -> Object -> _LengthUV[1]; i++) {
        if (!((i+1)%_obj -> Object -> _LengthUV[1])) i++;  //Be careful of Divide by 0
        _obj -> Object -> _CtlPts[k].x= _obj -> Object -> _CtlPts[i].x;
        _obj -> Object -> _CtlPts[k].y= _obj -> Object -> _CtlPts[i].y;
        _obj -> Object -> _CtlPts[k].z= _obj -> Object -> _CtlPts[i].z;
        k++;
      } //eof
      if (((Object_NURBS*)_obj -> Object) -> Order(1) > 2) ((Object_NURBS*)_obj -> Object) -> Order(1,((Object_NURBS*)_obj -> Object) -> Order(1)-1);
      _obj -> Object -> _LengthUV[1]--;
      PASS= 1;
    } else if (CheckRow(0,_obj)) { //BOTTOM
      //Decrement # of Rows (V)
      k= 0;
      for (i= 0; i < _obj -> Object -> _LengthUV[0] * _obj -> Object -> _LengthUV[1]; i++) {
        if (!(i%_obj -> Object -> _LengthUV[1])) i++;  //Be careful of Divide by 0
        _obj -> Object -> _CtlPts[k].x= _obj -> Object -> _CtlPts[i].x;
        _obj -> Object -> _CtlPts[k].y= _obj -> Object -> _CtlPts[i].y;
        _obj -> Object -> _CtlPts[k].z= _obj -> Object -> _CtlPts[i].z;
        k++;
      } //eof
      if (((Object_NURBS*)_obj -> Object) -> Order(1) > 2) ((Object_NURBS*)_obj -> Object) -> Order(1,((Object_NURBS*)_obj -> Object) -> Order(1)-1);
      _obj -> Object -> _LengthUV[1]--;
      PASS= 1;
    } else if (CheckCol(0,_obj)) { //LEFT
      //Decrement # of Cols (U)
      for (i= 0; i < ((_obj -> Object -> _LengthUV[0]-1) * (_obj -> Object -> _LengthUV[1])); i++) {
        _obj -> Object -> _CtlPts[i].x= _obj -> Object -> _CtlPts[i+_obj -> Object -> _LengthUV[1]].x;
        _obj -> Object -> _CtlPts[i].y= _obj -> Object -> _CtlPts[i+_obj -> Object -> _LengthUV[1]].y;
        _obj -> Object -> _CtlPts[i].z= _obj -> Object -> _CtlPts[i+_obj -> Object -> _LengthUV[1]].z;
      } //eof
      if (((Object_NURBS*)_obj -> Object) -> Order(0) > 2) ((Object_NURBS*)_obj -> Object) -> Order(0,((Object_NURBS*)_obj -> Object) -> Order(0)-1);
      _obj -> Object -> _LengthUV[0]--;
      PASS= 1;
    } else if (CheckCol(_obj -> Object -> _LengthUV[0]-1,_obj)) { //RIGHT
      //Decrement # of Cols (U)
      if (((Object_NURBS*)_obj -> Object) -> Order(0) > 2) ((Object_NURBS*)_obj -> Object) -> Order(0,((Object_NURBS*)_obj -> Object) -> Order(0)-1);
      _obj -> Object -> _LengthUV[0]--;
      PASS= 1;
    } //fi

    if (PASS) {
      for (i= 0; i < _obj -> Object -> _LengthUV[0] * _obj -> Object -> _LengthUV[1]; i++)
        _obj -> Object -> _ActPts[i]= 0;

      if (_obj -> Object -> _OBType == OBJECT_NURBS) ((Object_NURBS*)_obj -> Object) -> RecalculateKnotVector(1);
      _obj -> Object -> _ReCalcTssPts= 1;
    } //fi
  } //fi
  return(PASS);
} //eof Object_Manager::DeExtrude()


double* Object_Manager::GetMRM() {
  if ((_obj=GetObject(_ActiveID)))
    return(_obj -> Object -> _MRM);
  return(NULL);
} //eof Object_Manager::GetMRM()


void Object_Manager::SetMRM(double m[16]) {
  if ((_obj=GetObject(_ActiveID)))
    for (int i= 0; i < 16; i++)
      _obj -> Object -> _MRM[i]= m[i];
} //eof Object_Manager::SetMRM()


void Object_Manager::SetROT(Point3d R) {
  // Set MRM to Identity, and with each angle, construct a matrix for each one
  // and then multiply it against the MRM
  double			m[16];

  if ((_obj=GetObject(_ActiveID))) {
    _obj -> Object -> _ROT.x= R.x;
    _obj -> Object -> _ROT.y= R.y;
    _obj -> Object -> _ROT.z= R.z;

    VertexOps::IdentMatrix(_obj -> Object -> _MRM);

    VertexOps::MakeZMatrix(m,NurbanaMath::DtoR(R.z));
    VertexOps::MultMatrix(_obj -> Object -> _MRM,m);

    VertexOps::MakeYMatrix(m,NurbanaMath::DtoR(R.y));
    VertexOps::MultMatrix(_obj -> Object -> _MRM,m);

    VertexOps::MakeXMatrix(m,NurbanaMath::DtoR(R.x));
    VertexOps::MultMatrix(_obj -> Object -> _MRM,m);

    _obj -> Object -> _ReCalcTssPts= 1;
  } //fi
} //eof Object_Manager::SetROT()


Point3d Object_Manager::GetROT() {
  Point3d			R;
  R.x= R.y= R.z= 0;

  if ((_obj=GetObject(_ActiveID)))
    return(_obj -> Object -> _ROT);
  return( R );
} //eof Object_Manager::GetROT()


Point3d Object_Manager::GetSIZ() {
  Point3d			S;
  S.x= S.y= S.z= 0;

  if ((_obj=GetObject(_ActiveID)))
    return(_obj -> Object -> _SIZ);
  return( S );
} //eof Object_Manager::GetSIZ()


void Object_Manager::SetSIZ(Point3d S) {
  if ((_obj=GetObject(_ActiveID))) {
    _obj -> Object -> _SIZ.x= S.x;
    _obj -> Object -> _SIZ.y= S.y;
    _obj -> Object -> _SIZ.z= S.z;
  } //fi
} //eof Object_Manager::SetSIZ()


unsigned int Object_Manager::CPActCount() {
  unsigned	int	tot;

  tot= 0;
  if ((_obj=GetObject(_ActiveID))) {
    for (int i= 0; i < _obj -> Object -> Length(0) * _obj -> Object -> Length(1); i++) {
      if (_obj -> Object -> _ActPts[i])
        tot++;
    } //eof
  } //fi
  return(tot);
} //eof Object_Manager::CPActCount()


void Object_Manager::SetCPActAll(bool A) {
  if ((_obj=GetObject(_ActiveID)))
    for (int i= 0; i < _obj -> Object -> Length(0) * _obj -> Object -> Length(1); i++)
      _obj -> Object -> _ActPts[i]= A;
} //eof Object_Manager::SetCPActAll()


bool Object_Manager::GetCPAct(unsigned int I) {
  if ((_obj=GetObject(_ActiveID)))
    return(_obj -> Object -> _ActPts[I]);
  return(0);
} //eof Object_Manager::GetCPAct()


void Object_Manager::SetCPAct(unsigned int I, bool A) {
  if ((_obj=GetObject(_ActiveID)))
    _obj -> Object -> _ActPts[I]= A;
} //eof Object_Manager::SetCPAct()


unsigned int Object_Manager::GetCPActID() {
  if ((_obj=GetObject(_ActiveID)))
    for (int i= 0; i < _obj -> Object -> Length(0) * _obj -> Object -> Length(1); i++)
      if (_obj -> Object -> _ActPts[i]) return( i+1 );
  return(0);
} //eof Object_Manager::GetCPActID()


void Object_Manager::CPDelta(Point3d D) {
  Point3d			temp,Pt;
  double			MRMB[16];
  int				i;

  if ((_obj=GetObject(_ActiveID))) {
    for (i= 0; i < _obj -> Object -> Length(0) * _obj -> Object -> Length(1); i++) {
      if (_obj -> Object -> _ActPts[i]) {
        // Generate Points Phase-1 (SIZE)
        Pt.x= _obj -> Object -> _CtlPts[i].x * _obj -> Object -> _SIZ.x;
        Pt.y= _obj -> Object -> _CtlPts[i].y * _obj -> Object -> _SIZ.y;
        Pt.z= _obj -> Object -> _CtlPts[i].z * _obj -> Object -> _SIZ.z;

        // Generate Points Phase-2 (ROTATE)
        VertexOps::MultPtMatrix(temp, Pt, _obj -> Object -> _MRM);
        Pt.x= temp.x;
        Pt.y= temp.y;
        Pt.z= temp.z;

        // Generate Points Phase-3 (POSITION COR)
        Pt.x+= _obj -> Object -> _COR.x;
        Pt.y+= _obj -> Object -> _COR.y;
        Pt.z+= _obj -> Object -> _COR.z;
        
        Pt.x+= D.x;
        Pt.y+= D.y;
        Pt.z+= D.z;

        // Generate Points Phase-3 (POSITION COR)
        Pt.x-= _obj -> Object -> _COR.x;
        Pt.y-= _obj -> Object -> _COR.y;
        Pt.z-= _obj -> Object -> _COR.z;

        // Generate Points Phase-2 (ROTATE)
        VertexOps::InvertMatrix(MRMB, _obj -> Object -> _MRM);
        VertexOps::MultPtMatrix(temp, Pt, MRMB);
        Pt.x= temp.x;
        Pt.y= temp.y;
        Pt.z= temp.z;

        // Generate Points Phase-1 (SIZE)
        Pt.x/= _obj -> Object -> _SIZ.x;
        Pt.y/= _obj -> Object -> _SIZ.y;
        Pt.z/= _obj -> Object -> _SIZ.z;

        _obj -> Object -> _CtlPts[i].x= Pt.x;
        _obj -> Object -> _CtlPts[i].y= Pt.y;
        _obj -> Object -> _CtlPts[i].z= Pt.z;

        _obj -> Object -> _ReCalcTssPts= 1;
      } //fi
    } //fi
  } //fi
} //eof Object_Manager::CPDelta()


Point3d Object_Manager::GetCOR() {
  Point3d			C;
  C.x= C.y= C.z= 0;

  if ((_obj=GetObject(_ActiveID)))
    return(_obj -> Object -> _COR);
  return( C );
} //eof Object_Manager::GetCOR()


void Object_Manager::SetCOR(Point3d C) {
  if ((_obj=GetObject(_ActiveID))) {
    _obj -> Object -> _COR.x= C.x;
    _obj -> Object -> _COR.y= C.y;
    _obj -> Object -> _COR.z= C.z;
    _obj -> Object -> _ReCalcWCTssPts= 1;
  } //fi
} //eof Object_Manager::SetCOR()


Point3d Object_Manager::GetCP(unsigned int I) {
  Point3d			P;
  P.x= P.y= P.z= 0;

  if ((_obj=GetObject(_ActiveID)))
    return(_obj -> Object -> _CtlPts[I]);
  return( P );
} //eof Object_Manager::GetCP()


void Object_Manager::SetCP(unsigned int I, Point3d C) {
  if ((_obj=GetObject(_ActiveID))) {
    _obj -> Object -> _CtlPts[I].x= C.x;
    _obj -> Object -> _CtlPts[I].y= C.y;
    _obj -> Object -> _CtlPts[I].z= C.z;
    _obj -> Object -> _ReCalcTssPts= 1;
  } //fi
} //eof Object_Manager::SetCP()


Point3d Object_Manager::GetWCCP(int I) {
  Point3d			T;

  T.x= T.y= T.z= 0;
  if ((_obj=GetObject(_ActiveID)))
    return( _obj -> Object -> _WCCtlPts[I] );
  return(T);
} //eof Object_Manager::GetWCCP()


bool Object_Manager::GetEdge(unsigned char Edge) {
  if ((_obj=GetObject(_ActiveID)))
    return(_obj -> Object -> _Edges[Edge]);
  return(0);
} //eof Object_Manager::GetEdge()


void Object_Manager::SetEdge(unsigned char Edge, bool T) {
  if ((_obj=GetObject(_ActiveID)))
    _obj -> Object -> _Edges[Edge]= T;
} //eof Object_Manager::SetEdge()


void Object_Manager::SetRecalcTess() {
  Object_NURBS			*obj[1];

  if ((_obj=GetObject(_ActiveID))) {
    _obj -> Object -> _ReCalcTssPts= 0;
    obj[0]= (Object_NURBS*)_obj -> Object;

    // Check if KnotVector Needs ReCalculating
    if (_obj -> Object -> RecalculateKnotVector()) {
      NURBS::Perform(NURBS_Generate_KnotVector,1,obj,0);
      _obj -> Object -> RecalculateKnotVector(0);
    } //fi

    // Calculate Tesselation Points on Surface
    NURBS::Perform(NURBS_Generate_Surface,1,obj,0);
    Object_WC::AllPts(_obj -> Object);
    // Use Trimming Curves to Deactivate necessary Points
    NURBS::Perform(NURBS_Generate_TrimSurface,1,obj,0);
 
    Object_Normals::CalculateTriNormals(_obj -> Object -> _WCTssPts, _obj -> Object -> _Normals, _obj -> Object -> _TessUV[0], _obj -> Object -> _TessUV[1], _Smooth, IsClosed(0,_obj -> Object -> _TessUV[0], _obj -> Object -> _TessUV[1], _obj -> Object -> _TssPts), IsClosed(1, _obj -> Object -> _TessUV[0], _obj -> Object -> _TessUV[1], _obj -> Object -> _TssPts));
  } //fi
} //eof Object_Manager::SetRecalcTess()


void Object_Manager::SetRecalcWCTess() {
  if ((_obj=GetObject(_ActiveID)))
    Object_WC::AllPts(_obj -> Object);
} //eof Object_Manager::SetRecalcWCTess()


Point3d* Object_Manager::GetWCTess() {
  if ((_obj=GetObject(_ActiveID)))
    return( _obj -> Object -> WCTssPts() );
  return(NULL);
} //eof Object_Manager::GetWCTess()


unsigned int Object_Manager::GetTess(bool UV) {
  if ((_obj=GetObject(_ActiveID))) {
    if (UV) {
      return(int(_obj -> Object -> _TessUV[1]));
    } else {
      return(int(_obj -> Object -> _TessUV[0]));
    } //fi
  } //fi
  return(0);
} //eof Object_Manager::GetTess()


void Object_Manager::SetTess(int Tess) {
  _obj= Head -> GetHead();

  while (_obj) {
    _obj -> Object -> _TessUV[0]= Tess;
    _obj -> Object -> _TessUV[1]= Tess;
    _obj -> Object -> _ReCalcTssPts= 1;
    _obj= Head -> GetObjectPtr(Head -> GetNext(_obj -> Object -> _ID));
  } //eow
} //eof Object_Manager::SetTess()


int Object_Manager::GetIsoDensity() {
  if ((_obj=GetObject(_ActiveID)))
    return( _obj -> Object -> IPCDensity() );
  return(0);
} //eof Object_Manager::GetIsoDensity()


void Object_Manager::SetIsoDensity(int V) {
  if ((_obj=GetObject(_ActiveID))) {
    ((Object_NURBS*)_obj -> Object) -> SetIsoDensity(V);
    ((Object_NURBS*)_obj -> Object) -> Change(1,0);
    _obj -> Object -> _ReCalcTssPts= 1;
  } //fi
} //eof Object_Manager::SetIsoDensity()


unsigned int Object_Manager::GetLength(bool UV) {
  if ((_obj=GetObject(_ActiveID)))
    return(UV) ? _obj -> Object -> Length(1) : _obj -> Object -> Length(0);
  return(0);
} //eof Object_Manager::Length()


void Object_Manager::SetLength(bool UV, unsigned int L) {
  if ((_obj=GetObject(_ActiveID))) {
    UV ? _obj -> Object -> Length(1,L) : _obj -> Object -> Length(0,L);
    _obj -> Object -> SetLength(_obj -> Object -> Length(0), _obj -> Object -> Length(1));
  } //fi
} //eof Object_Manager::SetLength()


double Object_Manager::GetH(unsigned int I) {
  if ((_obj=GetObject(_ActiveID)))
    return( ((Object_NURBS*)_obj -> Object) -> H()[I] );
  return(0);
} //eof Objects::GetH()


void Object_Manager::SetH(unsigned int I, double V) {
  if ((_obj=GetObject(_ActiveID)))
    ((Object_NURBS*)_obj -> Object) -> H()[I]= V;
} //eof Objects::SetH()


void Object_Manager::SetHAct(double V) {
  if ((_obj=GetObject(_ActiveID)))
    for (int i= 0; i < _obj -> Object -> Length(0) * _obj -> Object -> Length(1); i++)
      if (_obj -> Object -> _ActPts[i])
        ((Object_NURBS*)_obj -> Object) -> H()[i]= V;
  _obj -> Object -> _ReCalcTssPts= 1;
} //eof Objects::SetH()


unsigned int Object_Manager::GetOrder(bool UV) {
  if ((_obj=GetObject(_ActiveID)))
    return UV ? _obj -> Object -> Order(1) : _obj -> Object -> Order(0);
  return(0);
} //eof Object_Manager::GetOrder()


void Object_Manager::SetOrder(unsigned int Order, bool UV) {
  if ((_obj=GetObject(_ActiveID))) {
    if (UV) {
      _obj -> Object -> Order(1,Order);
    } else {
      _obj -> Object -> Order(0,Order);
    } //fi
    _obj -> Object -> RecalculateKnotVector(1);
    _obj -> Object -> _ReCalcTssPts= 1;
  } //fi
} //eof Object_Manager::SetOrder()



bool Object_Manager::GetKnotType(bool UV) {
  if ((_obj=GetObject(_ActiveID)))
    return(_obj -> Object -> KnotType(UV));
  return(0);
} //eof Object_Manager::GetKnotType()


void Object_Manager::SetKnotType(bool UV, bool T) {
  if ((_obj=GetObject(_ActiveID))) {
    ((Object_NURBS*)_obj -> Object) -> KnotType(UV,T);
    _obj -> Object -> _ReCalcTssPts= 1;
  } //fi
} //eof Object_Manager::SetKnotType()


double Object_Manager::GetKnot(unsigned int I, bool UV) {
  if ((_obj=GetObject(_ActiveID)))
    return( ((Object_NURBS*)_obj -> Object) -> KnotVector(UV)[I] );
  return(0);
} //eof Object_Manager::GetKnot()


void Object_Manager::SetKnot(unsigned int I, bool UV, double V) {
  if ((_obj=GetObject(_ActiveID)))
    _obj -> Object -> KnotVector(UV)[I]= V;
} //eof Object_Manager::SetKnot()


void Object_Manager::SetRecalcKV(bool R) {
  if ((_obj=GetObject(_ActiveID)))
    _obj -> Object -> RecalculateKnotVector(R);
} //eof Object_Manager::SetRecalcKV()


void Object_Manager::SetChange() {
  if ((_obj=GetObject(_ActiveID)))
    ((Object_NURBS*)_obj -> Object) -> Change(0);
} //eof Object_Manager::SetChange()


unsigned int Object_Manager::GetMaterialID() {
  if ((_obj=GetObject(_ActiveID)))
    return(_obj -> Object -> _MaterialID);
  return(0);
} //eof Object_Manager::GetMaterialID()


void Object_Manager::SetMaterialID(unsigned int MID) {
  if ((_obj=GetObject(_ActiveID)))
    _obj -> Object -> _MaterialID= MID;
} //eof Object_Manager::SetMaterialID()


void Object_Manager::Duplicate() {
  Object_List::ObjectNode	*cur= NULL;
  Object_List::ObjectNode	*ptr= NULL;
  int i;

  cur= GetObject(_ActiveID);

  switch( cur -> Object -> _OBType ) {
    case OBJECT_NURBS:
      new Object_NURBS(Head);
      break;

    default:
      break;
  } //eos

  ptr= GetObject(Head -> GetNewObjectID());

  ptr -> Object -> _List= glGenLists(1);

  ptr -> Object -> _Active= 1;
  ptr -> Object -> _Axes= cur -> Object -> _Axes;
  ptr -> Object -> _OBType= cur -> Object -> _OBType;

  ptr -> Object -> _LengthUV[0]= cur -> Object -> _LengthUV[0];
  ptr -> Object -> _LengthUV[1]= cur -> Object -> _LengthUV[1];
  ptr -> Object -> SetLength(ptr -> Object -> _LengthUV[0],ptr -> Object -> _LengthUV[1]);

  for (i= 0; i < 16; i++) ptr -> Object -> _MRM[i]= cur -> Object -> _MRM[i];

  for (i= 0; i < cur -> Object -> _LengthUV[0] * cur -> Object -> _LengthUV[1]; i++) {
    ptr -> Object -> _ActPts[i]= cur -> Object -> _ActPts[i];
  } //eof
   
  for (i= 0; i < cur -> Object -> _LengthUV[0] * cur -> Object -> _LengthUV[1]; i++) {
    ptr -> Object -> _CtlPts[i].x= cur -> Object -> _CtlPts[i].x;
    ptr -> Object -> _CtlPts[i].y= cur -> Object -> _CtlPts[i].y;
    ptr -> Object -> _CtlPts[i].z= cur -> Object -> _CtlPts[i].z;
  } //eof

  for (i= 0; i < cur -> Object -> _LengthUV[0] * cur -> Object -> _LengthUV[1]; i++) {
    ptr -> Object -> _WCCtlPts[i].x= cur -> Object -> _WCCtlPts[i].x;
    ptr -> Object -> _WCCtlPts[i].y= cur -> Object -> _WCCtlPts[i].y;
    ptr -> Object -> _WCCtlPts[i].z= cur -> Object -> _WCCtlPts[i].z;
  } //eof

  ptr -> Object -> _MaterialID= cur -> Object -> _MaterialID;

  ptr -> Object -> _TessUV[0]= cur -> Object -> _TessUV[0];
  ptr -> Object -> _TessUV[1]= cur -> Object -> _TessUV[1];

  ptr -> Object -> _COR.x= cur -> Object -> _COR.x;
  ptr -> Object -> _COR.y= cur -> Object -> _COR.y;
  ptr -> Object -> _COR.z= cur -> Object -> _COR.z;

  ptr -> Object -> _ROT.x= cur -> Object -> _ROT.x;
  ptr -> Object -> _ROT.y= cur -> Object -> _ROT.y;
  ptr -> Object -> _ROT.z= cur -> Object -> _ROT.z;

  ptr -> Object -> _SIZ.x= cur -> Object -> _SIZ.x;
  ptr -> Object -> _SIZ.y= cur -> Object -> _SIZ.y;
  ptr -> Object -> _SIZ.z= cur -> Object -> _SIZ.z;

  if (ptr -> Object -> _OBType == OBJECT_NURBS) {
    //Copy Knot Vector U
    for (i= 0; i < cur -> Object -> _LengthUV[0] + cur -> Object -> Order(0); i++)
      ptr -> Object -> KnotVector(0)[i]= cur -> Object -> KnotVector(0)[i];

    //Copy Knot Vector V
    for (i= 0; i < cur -> Object -> _LengthUV[1] + cur -> Object -> Order(1); i++)
      ptr -> Object -> KnotVector(1)[i]= cur -> Object -> KnotVector(1)[i];

    ptr -> Object -> KnotType(0,cur -> Object -> KnotType(0));
    ptr -> Object -> KnotType(1,cur -> Object -> KnotType(1));

    ptr -> Object -> Cyclic(0,cur -> Object -> Cyclic(0));
    ptr -> Object -> Cyclic(1,cur -> Object -> Cyclic(1));

    ptr -> Object -> Order(0,cur -> Object -> Order(0));
    ptr -> Object -> Order(1,cur -> Object -> Order(1));

    for (i= 0; i < cur -> Object -> _LengthUV[0] * cur -> Object -> _LengthUV[1]; i++) ptr -> Object -> H()[i]= cur -> Object -> H()[i];

    ptr -> Object -> RecalculateKnotVector(0);
  } //fi

  ptr -> Object -> _ReCalcTssPts= 1;
  _ActiveID= Head -> GetNewObjectID();
} //eof Objects::Duplicate()


void Object_Manager::Delete() {
  if (_ActiveID) Head -> RemoveNode(_ActiveID);
  _ActiveID= 0;
} //eof Object_Manager::Delete()


void Object_Manager::Primitive(unsigned char Type) {
  new Object_NURBS(Head);
  _obj= GetObject(Head -> GetNewObjectID());
  Object_Primitives::Create(_obj,Type);
  _ActiveID= Head -> GetNewObjectID();
} //eof Object_Manager::Primitive()


void Object_Manager::Purge() {
  _ActiveID= 0;
  Head -> PurgeList();
} //eof Object_Manager::Purge()


void Object_Manager::CopyList(char LtA, char LtB, bool AB) {
  Object_List::ObjectNode	*ptr= NULL;
  Object_List::ObjectNode	*cur= NULL;
  int				i;

  Object_Manager::SelectList(LtA);

  // First destroy LtA contents
  Head -> PurgeList();


  Object_Manager::SelectList(LtB);

  cur= Head -> GetHead();

    while (cur) {
      Object_Manager::SelectList(LtA);
      //For Every Object, create a duplicate item
      switch( cur -> Object -> _OBType ) {
        case OBJECT_BEZIER:
//          new Object_Bezier(Head,cur -> Object -> _CSType);
          break;

        case OBJECT_NURBS:
          new Object_NURBS(Head);
          break;

        default:
          break;
      } //eos

      //Now, with the newly created object, get all of the cooresponsing properties
      //from LtB and copy them in the new LtA object.

      ptr= GetObject(Head -> GetNewObjectID());

      ptr -> Object -> _ID= cur -> Object -> _ID;
      ptr -> Object -> _RegenLists= 1;

      ptr -> Object -> _Active= cur -> Object -> _Active;
      ptr -> Object -> _Axes= cur -> Object -> _Axes;
      ptr -> Object -> _OBType= cur -> Object -> _OBType;

      ptr -> Object -> _LengthUV[0]= cur -> Object -> _LengthUV[0];
      ptr -> Object -> _LengthUV[1]= cur -> Object -> _LengthUV[1];
      ptr -> Object -> SetLength(ptr -> Object -> _LengthUV[0],ptr -> Object -> _LengthUV[1]);

      for (i= 0; i < 16; i++) ptr -> Object -> _MRM[i]= cur -> Object -> _MRM[i];

      for (i= 0; i < cur -> Object -> _LengthUV[0] * cur -> Object -> _LengthUV[1]; i++) {
        ptr -> Object -> _ActPts[i]= cur -> Object -> _ActPts[i];
      } //eof
   
      for (i= 0; i < cur -> Object -> _LengthUV[0] * cur -> Object -> _LengthUV[1]; i++) {
        ptr -> Object -> _CtlPts[i].x= cur -> Object -> _CtlPts[i].x;
        ptr -> Object -> _CtlPts[i].y= cur -> Object -> _CtlPts[i].y;
        ptr -> Object -> _CtlPts[i].z= cur -> Object -> _CtlPts[i].z;
      } //eof

      for (i= 0; i < cur -> Object -> _LengthUV[0] * cur -> Object -> _LengthUV[1]; i++) {
        ptr -> Object -> _WCCtlPts[i].x= cur -> Object -> _WCCtlPts[i].x;
        ptr -> Object -> _WCCtlPts[i].y= cur -> Object -> _WCCtlPts[i].y;
        ptr -> Object -> _WCCtlPts[i].z= cur -> Object -> _WCCtlPts[i].z;
      } //eof

      ptr -> Object -> _MaterialID= cur -> Object -> _MaterialID;

      ptr -> Object -> _TessUV[0]= cur -> Object -> _TessUV[0];
      ptr -> Object -> _TessUV[1]= cur -> Object -> _TessUV[1];

      ptr -> Object -> _COR.x= cur -> Object -> _COR.x;
      ptr -> Object -> _COR.y= cur -> Object -> _COR.y;
      ptr -> Object -> _COR.z= cur -> Object -> _COR.z;

      ptr -> Object -> _ROT.x= cur -> Object -> _ROT.x;
      ptr -> Object -> _ROT.y= cur -> Object -> _ROT.y;
      ptr -> Object -> _ROT.z= cur -> Object -> _ROT.z;

      ptr -> Object -> _SIZ.x= cur -> Object -> _SIZ.x;
      ptr -> Object -> _SIZ.y= cur -> Object -> _SIZ.y;
      ptr -> Object -> _SIZ.z= cur -> Object -> _SIZ.z;

      if (ptr -> Object -> _OBType == OBJECT_NURBS) {
        //Copy Knot Vector U
        for (i= 0; i < cur -> Object -> _LengthUV[0] + cur -> Object -> Order(0); i++)
          ptr -> Object -> KnotVector(0)[i]= cur -> Object -> KnotVector(0)[i];

        //Copy Knot Vector V
        for (i= 0; i < cur -> Object -> _LengthUV[1] + cur -> Object -> Order(1); i++)
          ptr -> Object -> KnotVector(1)[i]= cur -> Object -> KnotVector(1)[i];

        ptr -> Object -> KnotType(0,cur -> Object -> KnotType(0));
        ptr -> Object -> KnotType(1,cur -> Object -> KnotType(1));

        ptr -> Object -> Cyclic(0,cur -> Object -> Cyclic(0));
        ptr -> Object -> Cyclic(1,cur -> Object -> Cyclic(1));

        ptr -> Object -> Order(0,cur -> Object -> Order(0));
        ptr -> Object -> Order(1,cur -> Object -> Order(1));

        for (i= 0; i < cur -> Object -> _LengthUV[0] * cur -> Object -> _LengthUV[1]; i++) ptr -> Object -> H()[i]= cur -> Object -> H()[i];

        ptr -> Object -> RecalculateKnotVector(0);
      } //fi

      ptr -> Object -> _ReCalcTssPts= 1;

      Object_Manager::SelectList(LtB);
      cur= Head -> GetObjectPtr(Head -> GetNext(cur -> Object -> _ID));
    } //eow

  if (AB) {
    Object_Manager::SelectList(LtB);
  } else {
    Object_Manager::SelectList(LtA);
  } //fi
} //eof Object_Manager::CopyList()


void Object_Manager::SelectList(unsigned char T) {
  switch( T ) {
    case OBJECT_LIST_MAIN:
      Head= Main;
      break;

    case OBJECT_LIST_BACKUP:
      Head= Back;
      break;

    default:
      break;
  } //eos
} //eof Object_Manager::SelectList()


unsigned int Object_Manager::SelectionTotal() {
  SelectionList			*sl;
  unsigned		int	n= 0;

  sl= _SLHead;
  while(sl) {
    n++;
    sl= sl -> Next;
  } //eof

  return(n);
} //eof Object_Manager::SelectionTotal()


void Object_Manager::SetSmooth(bool v) {
  _Smooth= v;

  _obj= Head -> GetHead();
  while (_obj)
    _obj= Head -> GetObjectPtr(Head -> GetNext(_obj -> Object -> _ID));
} //eof Object_Manager::SetSmooth();


void Object_Manager::Axes(bool S) {
  if ((_obj=GetObject(_ActiveID)))
    _obj -> Object -> _Axes= S;
} //eof Object_Manager::Axes()


void Object_Manager::Mirror(unsigned char XYZ) {
  if ((_obj=GetObject(_ActiveID)))
    for (int i= 0; i < _obj -> Object -> Length(0) * _obj -> Object -> Length(1); i++) {
      if (XYZ == 0) {
        _obj -> Object -> CtlPts()[i].x*= -1;
      } else if (XYZ == 1) {
        _obj -> Object -> CtlPts()[i].y*= -1;
      } else if (XYZ == 2) {
        _obj -> Object -> CtlPts()[i].z*= -1;
      } //fi
      _obj -> Object -> _ReCalcTssPts= 1;
    } //eof
} //eof Object_Manager::Mirror()


void Object_Manager::Draw() {
  unsigned	int	ID;
  float			Diff[4],Spec[4];

  _obj= Head -> GetHead();

  ID= _ActiveID;
  while (_obj) {
    _ActiveID= _obj -> Object -> ID();
    // Check to see if Object Tss Pts need recalculated
    if (_obj -> Object -> _ReCalcTssPts)
      SetRecalcTess();

    if (_obj -> Object -> _ReCalcWCTssPts)
      SetRecalcWCTess();

      glDisable(GL_LIGHTING);
      glDisable(GL_DEPTH_TEST);

      Diff[0]= _MaterialList -> Diff(_obj -> Object -> _MaterialID)[0]/255.0f;
      Diff[1]= _MaterialList -> Diff(_obj -> Object -> _MaterialID)[1]/255.0f;
      Diff[2]= _MaterialList -> Diff(_obj -> Object -> _MaterialID)[2]/255.0f;
      Diff[3]= 0;

      Spec[0]= _MaterialList -> Spec(_obj -> Object -> _MaterialID)[0]/255.0f;
      Spec[1]= _MaterialList -> Spec(_obj -> Object -> _MaterialID)[1]/255.0f;
      Spec[2]= _MaterialList -> Spec(_obj -> Object -> _MaterialID)[2]/255.0f;
      Spec[3]= 0;

      if (GetMode(OBJECT_MODE_ZBUFFER)) {
        // Shaded Surface, or Wire frame if U||V length == 1
        switch(_obj -> Object -> _OBType) {
          case OBJECT_NURBS:
            if ((_obj -> Object -> Length(0) == 1) || (_obj -> Object -> Length(1) == 1)) {
              Object_Draw::Wireframe(_obj -> Object, 160, 160, 174, 160, 160, 174, _obj -> Object -> IPCDensity(), _obj -> Object -> IPCDensity(), _obj -> Object -> WCIPCPts(), GetMode(OBJECT_MODE_EDIT), 1, 0, 3);
            } else {
              if (_FaceType)
                DrawFaces::DrawTriadFaces(_obj -> Object -> _WCTssPts, _obj -> Object -> _Normals, _obj -> Object -> _TessUV[0], _obj -> Object -> _TessUV[1], Diff, Spec, _Smooth);
            } //fi
            break;

          default:
            break;
        } //eos
      } else {
        glEnable(GL_DEPTH_TEST);
        // Wire-frame for Surface
        switch(_obj -> Object -> _OBType) {
          case OBJECT_NURBS:
              if (!GetMode(OBJECT_MODE_DRAWEDGES)) {
                // Draw Isoparametric Curves
                Object_Draw::Wireframe(_obj -> Object, 160, 160, 174, 160, 160, 174, _obj -> Object -> IPCResolution(), _obj -> Object -> IPCDensity(), _obj -> Object -> WCIPCPts(), GetMode(OBJECT_MODE_EDIT), 1, 0, 1);
                Object_Draw::Wireframe(_obj -> Object, 160, 160, 174, 160, 160, 174, _obj -> Object -> IPCDensity(), _obj -> Object -> IPCResolution(), _obj -> Object -> WCIPCPts(), GetMode(OBJECT_MODE_EDIT), 1, _obj -> Object -> IPCDensity()*_obj -> Object -> IPCResolution(),2);
              } //fi
              Object_Draw::Edges(_obj -> Object, GetMode(OBJECT_MODE_DRAWEDGES));
            break;

          default:
            break;
        } //eos
        glDisable(GL_DEPTH_TEST);
      } //fi

      if (!GetMode(OBJECT_MODE_DRAWEDGES)) {
        // Control Hull AND COR
        if (!GetMode(OBJECT_MODE_EDIT)) glLoadName(_obj -> Object -> ID());
        Object_Draw::COR(_obj -> Object);  //Draw Center of Rotation

        if (GetMode(OBJECT_MODE_EDIT) && _obj -> Object -> ID() == ID)
          Object_Draw::Wireframe(_obj -> Object, 255, 255, 225, 0, 0, 225, _obj -> Object -> Length(0), _obj -> Object -> Length(1), _obj -> Object -> WCCtlPts(), GetMode(OBJECT_MODE_EDIT), 0, 0, 3); // Draw Control Hull
        if (_Normals) {
          if (_FaceType)
            DrawFaces::DrawTriadNormalVectors(_obj -> Object -> WCTssPts(), _obj -> Object -> Tess(0), _obj -> Object -> Tess(1), _NormalLength);
        } //fi


        // Control Points
        if (GetMode(OBJECT_MODE_EDIT) && _obj -> Object -> ID() == _ActiveID)
          if (_obj -> Object -> _Active) Object_Draw::Vertices(_obj -> Object, _obj -> Object -> WCCtlPts()); //Draw Control Points

        // Model XYZ Axes
        if (_obj -> Object -> _Axes)
          Object_Draw::Axes(_obj -> Object);
      } //fi

    _obj= Head -> GetObjectPtr(Head -> GetNext(_obj -> Object -> _ID));
  } //eow
  _ActiveID= ID;
} //eof Object_Manager::Draw()


void Object_Manager::Rebuild(int CPU, int CPV, int OrderU, int OrderV, int UK) {
  // First check to see if Degree was changed, if so then
  // Degree elevate and roll back the CP's through Knot Removal
  // If just CP's are changed, knot insertion/knot removal respectively
  Object_NURBS			*obj[1];

  if ((_obj=GetObject(_ActiveID))) {
    obj[0]= (Object_NURBS*)_obj -> Object;

    // Perform necessary Degree Elevation
    if ((OrderU > _obj -> Object -> Order(0)) || (OrderV > _obj -> Object -> Order(1))) {
      // Degree Elevation
      NURBS::Perform(NURBS_Degree_Elevate,1,obj,2,NURBS_TYPE_INT,OrderU-_obj -> Object -> Order(0),NURBS_TYPE_INT,OrderV-_obj -> Object -> Order(1));
      _obj -> Object -> _ReCalcTssPts= 1;
    } //fi
    if ((OrderU < _obj -> Object -> Order(0)) || (OrderV < _obj -> Object -> Order(1))) {
      // Degree Reduction
      NURBS::Perform(NURBS_Degree_Reduce,1,obj,2,NURBS_TYPE_INT,_obj -> Object -> Order(0)-OrderU,NURBS_TYPE_INT,_obj -> Object -> Order(1)-OrderV);
      _obj -> Object -> _ReCalcTssPts= 1;
    } //fi

    // Perform post knot insertion/removal if CP count differs
    if ((CPU != _obj -> Object -> _LengthUV[0]) || (CPV != _obj -> Object -> _LengthUV[1])) {
      NURBS::Perform(NURBS_Fitting_GlobalInterp,1,obj,3,NURBS_TYPE_INT,CPU,NURBS_TYPE_INT,CPV,NURBS_TYPE_INT,UK);
      _obj -> Object -> _ReCalcTssPts= 1;
    } //fi
  } //fi
} //eof Object_Manager::Rebuild()


void Object_Manager::Loft(int V) {
  Object_List::ObjectNode	*cur= NULL;
  Object_NURBS			**obj;
  SelectionList			*sl;
  int				i,total= 0;

  // Determine how many curves are in Selection List
  sl= _SLHead;
  while (sl) {
    sl= sl -> Next;
    total++;
  } //eow

  if (total > 2) {
    // Allocate memory
    obj= (Object_NURBS**)malloc(sizeof(Object_NURBS)*(total+1));
    // Create new Surface
    new Object_NURBS(Head);
    obj[0]= (Object_NURBS*)GetObject(Head -> GetNewObjectID()) -> Object;

    // Assign Selected Curves
    i= 1;
    sl= _SLHead;
    while (sl) {
      obj[i++]= (Object_NURBS*)GetObject(sl -> ID) -> Object;
      sl= sl -> Next;
    } //eow

    // Verify V Degree is valid
    if (V < 1) V= 1;
    if (V > total) V= total;

    NURBS::Perform(NURBS_Construct_Loft,total+1,obj,1,NURBS_TYPE_INT,V);
    _ActiveID= obj[0] -> _ID;

    // Destroy Curves
    for (i= 1; i < total+1; i++) {
      SetActiveID(obj[i] -> ID());
      Delete();
    } //eof
  } //fi
} //eof Object_Manager::Loft()


void Object_Manager::Blend(int Twist) {
  Object_List::ObjectNode	*cur= NULL;
  Object_NURBS			*obj[3];
  int				Count;

  cur= Head -> GetHead();
  Count= 0;
  while (cur) {
    if (cur -> Object -> _Edges[0]) {
      obj[Count]= (Object_NURBS*)cur -> Object;
      Count++;
    } //fi

    if (cur -> Object -> _Edges[1]) {
      obj[Count]= (Object_NURBS*)cur -> Object;
      Count++;
    } //fi

    if (cur -> Object -> _Edges[2]) {
      obj[Count]= (Object_NURBS*)cur -> Object;
      Count++;
    } //fi

    if (cur -> Object -> _Edges[3]) {
      obj[Count]= (Object_NURBS*)cur -> Object;
      Count++;
    } //fi

    if (Count > 1) {
      obj[2]= new Object_NURBS(Head);

      NURBS::Perform(NURBS_Construct_Blend,3,obj,1,NURBS_TYPE_INT,Twist);

      cur= Head -> GetHead();
      while (cur) {
        cur -> Object -> _Edges[0]= cur -> Object -> _Edges[1]= cur -> Object -> _Edges[2]= cur -> Object -> _Edges[3]= 0;
        cur= Head -> GetObjectPtr(Head -> GetNext(cur -> Object -> _ID));
      } //eow
      break;
    } //fi
    cur= Head -> GetObjectPtr(Head -> GetNext(cur -> Object -> _ID));
  } //eow
} //eof Object_Manager::Blend()


void Object_Manager::Subdivide() {
  Object_NURBS			*obj[1];

  if ((_obj=GetObject(_ActiveID))) {
    obj[0]= (Object_NURBS*)_obj -> Object;
    NURBS::Perform(NURBS_Knot_Subdivide,1,obj,0);
    SetRecalcTess();
  } //fi
} //eof Object_Manager::Subdivide()


Object_List::ObjectNode* Object_Manager::GetObject(int ID) {
  if (Head)
    return( Head -> GetObjectPtr(ID) );
  return(NULL);
} //eof Object_Manager::GetObject()


bool Object_Manager::IsClosed(bool UV, int ULen, int VLen, Point3d *Pts) {
  int	i;

  if (UV) {
    for (i= 0; i < ULen; i++) {
      if (fabs(Pts[i*VLen].x - Pts[(i+1)*VLen-1].x) > 0.001)
        return(0);
      if (fabs(Pts[i*VLen].y - Pts[(i+1)*VLen-1].y) > 0.001)
        return(0);
      if (fabs(Pts[i*VLen].z - Pts[(i+1)*VLen-1].z) > 0.001)
        return(0);
    } //eof
  } else {
    for (i= 0; i < VLen; i++) {
      if (fabs(Pts[i].x - Pts[ULen*(VLen-1)+i].x) > 0.001)
        return(0);
      if (fabs(Pts[i].y - Pts[ULen*(VLen-1)+i].y) > 0.001)
        return(0);
      if (fabs(Pts[i].z - Pts[ULen*(VLen-1)+i].z) > 0.001)
        return(0);
    } //eof
  } //fi

  return(1);
} //eof Object_Manager::IsClosed()
