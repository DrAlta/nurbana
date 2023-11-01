#include "NWM_Panel_Edit.h"


//
// Local Callbacks
//

void CenterObjectCallback(char E) {
  Point3d	COR;

  COR.x= COR.y= COR.z= 0;
  NWM_Panel_Edit::Access -> _ObjectList -> SetCOR(COR);
  NWM_Panel_Edit::Access -> _NWM -> UpdateType(NWM_WT_ModelView);
} //eof CenterObjectCallback()

void CenterPointsCallback(char E) {
  NWM_Panel_Edit::Access -> _ObjectList -> CenterPoints();
  NWM_Panel_Edit::Access -> _NWM -> UpdateType(NWM_WT_ModelView);
} //eof CenterPointsCallback()

void NormalsCallback(char E) {
  NWM_Panel_Edit::Access -> _ObjectList -> SetNormals(NWM_Panel_Edit::Access -> _Normals -> Status());
  NWM_Panel_Edit::Access -> _NWM -> UpdateType(NWM_WT_ModelView);
} //eof NormalsCallback()

void NormalLengthCallback(char E) {
  NWM_Panel_Edit::Access -> _ObjectList -> NormalLength(NWM_Panel_Edit::Access -> _NormalLength -> Value());
  NWM_Panel_Edit::Access -> _NWM -> UpdateType(NWM_WT_ModelView);
} //eof NormalLengthCallback()

void DegreeCallback(char E) {
  if (E == NTK_EVENT_MOUSE_UP || E == NTK_EVENT_KEY) {
    NWM_Panel_Edit::Access -> _ObjectList -> SetOrder(NWM_Panel_Edit::Access -> _DegreeUSlider -> Value()+1,0);
    NWM_Panel_Edit::Access -> _ObjectList -> SetOrder(NWM_Panel_Edit::Access -> _DegreeVSlider -> Value()+1,1);
    NWM_Panel_Edit::Access -> _NWM -> UpdateType(NWM_WT_ModelView);
  } //fi
} //eof OrderCallback()

void WeightCallback(char E) {
  NWM_Panel_Edit::Access -> _ObjectList -> SetHAct(NWM_Panel_Edit::Access -> _WeightSlider -> Value());
  NWM_Panel_Edit::Access -> _NWM -> UpdateType(NWM_WT_ModelView);
} //eof WeightCallback()

/*
void KnotTypeCallback(char E) {
  if (NWM_Panel_Edit::Access -> _KnotPeriodicU -> Status())
    NWM_Panel_Edit::Access -> _ObjectList -> ChangeKnotType(NWM_Panel_Edit::Access -> _ObjectList -> ActiveObject(),NURBS_KV_U,NURBS_KV_Periodic);
  if (NWM_Panel_Edit::Access -> _KnotPeriodicV -> Status())
    NWM_Panel_Edit::Access -> _ObjectList -> ChangeKnotType(NWM_Panel_Edit::Access -> _ObjectList -> ActiveObject(),NURBS_KV_V,NURBS_KV_Periodic);
  if (NWM_Panel_Edit::Access -> _KnotOpenU -> Status())
    NWM_Panel_Edit::Access -> _ObjectList -> ChangeKnotType(NWM_Panel_Edit::Access -> _ObjectList -> ActiveObject(),NURBS_KV_U,NURBS_KV_Open);
  if (NWM_Panel_Edit::Access -> _KnotOpenV -> Status())
    NWM_Panel_Edit::Access -> _ObjectList -> ChangeKnotType(NWM_Panel_Edit::Access -> _ObjectList -> ActiveObject(),NURBS_KV_V,NURBS_KV_Open);
 
  NWM_Panel_Edit::Access -> _NWM -> UpdateType(NWM_WT_ModelView);
} //eof KnotTypeCallback()
*/

void MirroringCallback(char E) {
  if (NWM_Panel_Edit::Access -> _MirX -> Status())
    NWM_Panel_Edit::Access -> _ObjectList -> Mirror(0);
  if (NWM_Panel_Edit::Access -> _MirY -> Status())
    NWM_Panel_Edit::Access -> _ObjectList -> Mirror(1);
  if (NWM_Panel_Edit::Access -> _MirZ -> Status())
    NWM_Panel_Edit::Access -> _ObjectList -> Mirror(2);

  NWM_Panel_Edit::Access -> _NWM -> UpdateType(NWM_WT_ModelView);  
} //eof MirroringCallback()

void AxesCallback(char E) {
  NWM_Panel_Edit::Access -> _ObjectList -> Axes(NWM_Panel_Edit::Access -> _Axes -> Status());
  NWM_Panel_Edit::Access -> _NWM -> UpdateType(NWM_WT_ModelView);
} //eof AxesCallback()

void SubdivideCallback(char E) {
  NWM_Panel_Edit::Access -> _ObjectList -> Subdivide();
  NWM_Panel_Edit::Access -> _NWM -> UpdateType(NWM_WT_ModelView);
} //eof SubdivideCallback()

void IsoParmCallback(char E) {
  NWM_Panel_Edit::Access -> _ObjectList -> SetIsoDensity(NWM_Panel_Edit::Access -> _IsoParm -> Value());
  NWM_Panel_Edit::Access -> _NWM -> UpdateType(NWM_WT_ModelView);
} //eof IsoParmCallback()

NWM_Panel_Edit* NWM_Panel_Edit::Access= 0;
NWM_Panel_Edit::NWM_Panel_Edit(int H1, int H2, int H3, int H4, NWM_List *obj, NWM *NWMptr, Object_Manager *ObjectList) : NWM_Base(obj,NWM_WT_Panel_Edit) {
  _self= this;
  Access= _self;

  _NWM= NWMptr;
  _ObjectList= ObjectList;

  // Assign Handles
  _H1= H1;
  _H2= H2;
  _H3= H3;
  _H4= H4;

  _Window= new NTK_Window(_NWM -> Path());

  //
  // Configure Widgets
  //

  _EditVContainer= new NTK_Container(0,0);

  _CenterObject= new NTK_Button(_Window,0,0,150,20);
  _CenterObject -> Type(NTK_BUTTON_TEXT);
  _CenterObject -> Color(0xA0,0xA0,0xA0);
  _CenterObject -> Text("Center Object");
  _CenterObject -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _CenterObject -> Callback((NTK_Callback*)CenterObjectCallback);

  _CenterPoints= new NTK_Button(_Window,0,20,150,20);
  _CenterPoints -> Type(NTK_BUTTON_TEXT);
  _CenterPoints -> Color(0xA0,0xA0,0xA0);
  _CenterPoints -> Text("Center Points");
  _CenterPoints -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _CenterPoints -> Callback((NTK_Callback*)CenterPointsCallback);

  _Subdivide= new NTK_Button(_Window,0,40,150,20);
  _Subdivide -> Type(NTK_BUTTON_TEXT);
  _Subdivide -> Color(0xA0,0xA0,0xA0);
  _Subdivide -> Text("Subdivide");
  _Subdivide -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _Subdivide -> Callback((NTK_Callback*)SubdivideCallback);

  _Axes= new NTK_Button(_Window,0,60,150,20);
  _Axes -> Type(NTK_BUTTON_TEXT);
  _Axes -> Sticky(NTK_ON);
  _Axes -> Color(0xA0,0xA0,0xA0);
  _Axes -> Text("Axes");
  _Axes -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _Axes -> Callback((NTK_Callback*)AxesCallback);

//  _Degree= new NTK_Label(_Window,360,30,150,20);
//  _Degree -> Text("Degree");
//  _Degree -> Color(0x66,0x66,0x66);

  _DegreeUSlider= new NTK_Slider_Button(_Window,150,0,150,20);
  _DegreeUSlider -> Type(NTK_HORZ_SLIDER);
  _DegreeUSlider -> StaticText("Deg U:");
  _DegreeUSlider -> Min(1);
  _DegreeUSlider -> Step(1);
  _DegreeUSlider -> Color(0xA0,0xA0,0xA0);
  _DegreeUSlider -> SliderColor(0x80,0x90,0xA0);
  _DegreeUSlider -> CallbackTrigger(NTK_CT_MOTION | NTK_CT_MOUSE_UP | NTK_CT_KEY);
  _DegreeUSlider -> Callback((NTK_Callback*)DegreeCallback);

  _DegreeVSlider= new NTK_Slider_Button(_Window,150,20,150,20);
  _DegreeVSlider -> Type(NTK_HORZ_SLIDER);
  _DegreeVSlider -> StaticText("Deg V:");
  _DegreeVSlider -> Min(1);
  _DegreeVSlider -> Step(1);
  _DegreeVSlider -> Color(0xA0,0xA0,0xA0);
  _DegreeVSlider -> SliderColor(0x80,0x90,0xA0);
  _DegreeVSlider -> CallbackTrigger(NTK_CT_MOTION | NTK_CT_MOUSE_UP | NTK_CT_KEY);
  _DegreeVSlider -> Callback((NTK_Callback*)DegreeCallback);

  _SetWeight= new NTK_Button(_Window,150,40,150,20);
  _SetWeight -> Type(NTK_BUTTON_TEXT);
  _SetWeight -> Color(0x80,0x90,0xA0);
  _SetWeight -> Text("Set Weight");
  _SetWeight -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _SetWeight -> Callback((NTK_Callback*)WeightCallback);

  _WeightSlider= new NTK_Slider_Horz(_Window,150,60,150,20);
  _WeightSlider -> Type(NTK_HORZ_SLIDER);
  _WeightSlider -> Min(0);
  _WeightSlider -> Max(10);
  _WeightSlider -> Precision(1);
  _WeightSlider -> Value(1);
  _WeightSlider -> Color(0xA0,0xA0,0xA0);
  _WeightSlider -> SliderColor(0x80,0x90,0xA0);
  _WeightSlider -> CallbackTrigger(NTK_CT_MOTION | NTK_CT_MOUSE_UP);

  _MirX= new NTK_Button(_Window,300,0,50,20);
  _MirX -> Type(NTK_BUTTON_TEXT);
  _MirX -> Color(0xA0,0xA0,0xA0);
  _MirX -> Text("Mir X");
  _MirX -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _MirX -> Callback((NTK_Callback*)MirroringCallback);

  _MirY= new NTK_Button(_Window,350,0,50,20);
  _MirY -> Type(NTK_BUTTON_TEXT);
  _MirY -> Color(0xA0,0xA0,0xA0);
  _MirY -> Text("Mir Y");
  _MirY -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _MirY -> Callback((NTK_Callback*)MirroringCallback);

  _MirZ= new NTK_Button(_Window,400,0,50,20);
  _MirZ -> Type(NTK_BUTTON_TEXT);
  _MirZ -> Color(0xA0,0xA0,0xA0);
  _MirZ -> Text("Mir Z");
  _MirZ -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _MirZ -> Callback((NTK_Callback*)MirroringCallback);

  _Normals= new NTK_Button(_Window,300,20,150,20);
  _Normals -> Type(NTK_BUTTON_TEXT);
  _Normals -> Sticky(NTK_ON);
  _Normals -> Color(0x80,0x90,0xA0);
  _Normals -> Text("Normals");
  _Normals -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _Normals -> Callback((NTK_Callback*)NormalsCallback);

  _NormalLength= new NTK_Slider_Horz(_Window,300,40,150,20);
  _NormalLength -> Type(NTK_HORZ_SLIDER);
  _NormalLength -> Min(.1);
  _NormalLength -> Max(10);
  _NormalLength -> Value(.1);
  _NormalLength -> Precision(1);
  _NormalLength -> Color(0xA0,0xA0,0xA0);
  _NormalLength -> SliderColor(0x80,0x90,0xA0);
  _NormalLength -> CallbackTrigger(NTK_CT_MOTION | NTK_CT_MOUSE_UP);
  _NormalLength -> Callback((NTK_Callback*)NormalLengthCallback);


  _IsoParm= new NTK_Slider_Button(_Window,300,60,150,20);
  _IsoParm -> Type(NTK_HORZ_SLIDER);
  _IsoParm -> StaticText("ISO Density:");
  _IsoParm -> Min(3);
  _IsoParm -> Max(64);
  _IsoParm -> Step(1);
  _IsoParm -> Color(0xA0,0xA0,0xA0);
  _IsoParm -> SliderColor(0x80,0x90,0xA0);
  _IsoParm -> CallbackTrigger(NTK_CT_MOTION | NTK_CT_MOUSE_UP | NTK_CT_KEY);
  _IsoParm -> Callback((NTK_Callback*)IsoParmCallback);

/*
  _KnotType= new NTK_Label(_MainWindow,575,30,150,20);
  _KnotType -> Text("Knot Type");
  _KnotType -> Color(0x66,0x66,0x66);
  _KnotType -> Container(_EditContainer);

  _KnotPeriodicU= new NTK_Button(_MainWindow,575,50,75,20);
  _KnotPeriodicU -> Status(NTK_ON);
  _KnotPeriodicU -> Type(NTK_BUTTON_TEXT);
  _KnotPeriodicU -> Color(0xA0,0xA0,0xA0);
  _KnotPeriodicU -> Text("Periodic");
  _KnotPeriodicU -> Group(_KnotUGroup);
  _KnotPeriodicU -> Container(_EditContainer);
  _KnotPeriodicU -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _KnotPeriodicU -> Callback((NTK_Callback*)KnotTypeCallback);

  _KnotPeriodicV= new NTK_Button(_MainWindow,575,70,75,20);
  _KnotPeriodicV -> Status(NTK_ON);
  _KnotPeriodicV -> Type(NTK_BUTTON_TEXT);
  _KnotPeriodicV -> Color(0xA0,0xA0,0xA0);
  _KnotPeriodicV -> Text("Periodic");
  _KnotPeriodicV -> Group(_KnotVGroup);
  _KnotPeriodicV -> Container(_EditVContainer);
  _KnotPeriodicV -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _KnotPeriodicV -> Callback((NTK_Callback*)KnotTypeCallback);

  _KnotOpenU= new NTK_Button(_MainWindow,650,50,75,20);
  _KnotOpenU -> Type(NTK_BUTTON_TEXT);
  _KnotOpenU -> Color(0xA0,0xA0,0xA0);
  _KnotOpenU -> Text("Open");
  _KnotOpenU -> Group(_KnotUGroup);
  _KnotOpenU -> Container(_EditContainer);
  _KnotOpenU -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _KnotOpenU -> Callback((NTK_Callback*)KnotTypeCallback);

  _KnotOpenV= new NTK_Button(_MainWindow,650,70,75,20);
  _KnotOpenV -> Type(NTK_BUTTON_TEXT);
  _KnotOpenV -> Color(0xA0,0xA0,0xA0);
  _KnotOpenV -> Text("Open");
  _KnotOpenV -> Group(_KnotVGroup);
  _KnotOpenV -> Container(_EditVContainer);
  _KnotOpenV -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _KnotOpenV -> Callback((NTK_Callback*)KnotTypeCallback);
*/
} //eof NWM_Panel_Edit::NWM_Panel_Edit()


void NWM_Panel_Edit::Update() {
  glEnable(GL_SCISSOR_TEST);
    glViewport(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
    glScissor(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
      _Window -> Draw();
  glDisable(GL_SCISSOR_TEST);
} //eof NWM_Panel_Edit::Update()


void NWM_Panel_Edit::Refresh() {
//  if (_ObjectList -> GetActiveID()) {
//    _EditContainer -> Visibility(NTK_CONTAINER_VISIBLE);
//  } else {
//    _EditContainer -> Visibility(NTK_CONTAINER_INVISIBLE);
//  } //fi

  // Edit Menu
  _DegreeUSlider -> Value(_ObjectList -> GetOrder(0)-1);
  _DegreeVSlider -> Value(_ObjectList -> GetOrder(1)-1);
  _DegreeUSlider -> Max(_ObjectList -> GetLength(0)-1);
  _DegreeVSlider -> Max(_ObjectList -> GetLength(1)-1);

  if (_ObjectList -> GetLength(1) > 1) {
    _EditVContainer -> Visibility(NTK_CONTAINER_VISIBLE);
  } else {
    _EditVContainer -> Visibility(NTK_CONTAINER_INVISIBLE);
  } //fi

  _IsoParm -> Value(_ObjectList -> GetIsoDensity());

/*
  // Knot Vector
  _KnotPeriodicU -> Status(NTK_OFF);
  _KnotPeriodicV -> Status(NTK_OFF);
  _KnotOpenU -> Status(NTK_OFF);
  _KnotOpenV -> Status(NTK_OFF);
  if (_ObjectList -> GetKnotType(_ObjectList -> ActiveObject(),0)) {
    _KnotOpenU -> Status(NTK_ON);
  } else {
    _KnotPeriodicU -> Status(NTK_ON);
  } //fi
  if (_ObjectList -> GetKnotType(_ObjectList -> ActiveObject(),1)) {
    _KnotOpenV -> Status(NTK_ON);
  } else {
    _KnotPeriodicV -> Status(NTK_ON);
  } //fi
*/



  _Window -> SetClear(2);
  _Window -> UpdateAll();
} //eof NWM_Panel_Edit::Refresh()


void NWM_Panel_Edit::Reshape(int w, int h) {
  // Construct Main Panel Viewport
  _Viewport[0]= _PosX;
  _Viewport[1]= h-(_PosY+_SizY);
  _Viewport[2]= _SizX;
  _Viewport[3]= _SizY;
  _Viewport[4]= w;
  _Viewport[5]= h;

  _Window -> SetClear(2);  
  _Window -> ScreenSize(_SizX,_SizY);
} //eof NWM_Panel_Edit::Reshape()


char NWM_Panel_Edit::Key(int x, int y) {
  // For callbacks, assign _pself _lself
  Access= _self;
  _Update= 2;

  _Window -> Handle(NTK_EVENT_KEY,x,y,0,0);
  return(NWM_NOTHING);
} //eof NWM_Panel_Edit::Key()


char NWM_Panel_Edit::Mouse(int button, int state, int x, int y) {
  // For callbacks, assign _pself _lself
  Access= _self;
  _Window -> Handle(NTK_EVENT_MOUSE,button,state,x,y);

  _Update= 2;
  if (state) {
    return(NWM_LOCK);
  } else {
    if (!_Window -> TypeActive(NTK_WT_TextInput))
      return(NWM_UNLOCK);
  } //fi
  return(NWM_NOTHING);
} //eof NWM_Panel_Edit::Mouse()


char NWM_Panel_Edit::Motion(int x, int y) {
  // For callbacks, assign _pself _lself
  Access= _self;

  _Window -> Handle(NTK_EVENT_MOTION,x,y,0,0);
  _Update= 1;
  return(NWM_NOTHING);
} //eof NWM_Panel_Edit::Motion()


char NWM_Panel_Edit::Passive(int x, int y) {
  // For callbacks, assign _pself _lself
  Access= _self;

  return(NWM_NOTHING);
} //eof NWM_Panel_Edit::Passive()


void NWM_Panel_Edit::Active(bool A) {
} //eof NWM_Panel_Edit::Active()


void NWM_Panel_Edit::Misc(int V) {
  _Window -> SetClear(2);
  _Window -> UpdateAll();
} //eof Panel_Edit::Misc()
