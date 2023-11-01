#include "NWM_Panel_Settings.h"


//
// Local Callbacks
//

void MBCCallback(char E) {
  // Update all the ModelViews BG Scene Color
  if (NWM_Panel_Settings::Access -> _MBCGRIDTOGGLE -> Status()) {
    NWM_Panel_Settings::Access -> _ObjectList -> GRIDColor(NWM_Panel_Settings::Access -> _MBCR -> Value(),NWM_Panel_Settings::Access -> _MBCG -> Value(),NWM_Panel_Settings::Access -> _MBCB -> Value());
  } else {
    NWM_Panel_Settings::Access -> _ObjectList -> BGColor(NWM_Panel_Settings::Access -> _MBCR -> Value(),NWM_Panel_Settings::Access -> _MBCG -> Value(),NWM_Panel_Settings::Access -> _MBCB -> Value());
  } //fi
  NWM_Panel_Settings::Access -> _NWM -> UpdateType(NWM_WT_ModelView);
} //eof MBCCallback()


void TrackballSpeedCallback(char E) {
  NWM_Panel_Settings::Access -> _ObjectList -> TBSpeed(NWM_Panel_Settings::Access -> _TrackballSpeed -> Value());
} //eof TrackballSpeedCallback()

void FOVAngleCallback(char E) {
  NWM_Panel_Settings::Access -> _ObjectList -> FOV(NWM_Panel_Settings::Access -> _FOVAngle -> Value());
  NWM_Panel_Settings::Access -> _NWM -> UpdateType(NWM_WT_ModelView);
} //eof FOVAngleCallback()

void TesselationCallback(char E) {
  if (E == NTK_EVENT_MOUSE_UP) {
    NWM_Panel_Settings::Access -> _ObjectList -> SetTess(NWM_Panel_Settings::Access -> _TessSlider -> Value());

//    NWM_Panel_Settings::Access -> _ObjectList -> ChangeTess(NWM_Panel_Settings::Access -> _ObjectList -> ActiveObject(),NWM_Panel_Settings::Access -> _TessSlider -> Value(),0);
//    NWM_Panel_Settings::Access -> _ObjectList -> ChangeTess(NWM_Panel_Settings::Access -> _ObjectList -> ActiveObject(),NWM_Panel_Settings::Access -> _TessSlider -> Value(),1);

    NWM_Panel_Settings::Access -> _NWM -> UpdateType(NWM_WT_ModelView);
  } //fi
} //eof TesselationCallback()

void SmoothCallback(char E) {
  NWM_Panel_Settings::Access -> _ObjectList -> SetSmooth(NWM_Panel_Settings::Access -> _Smooth -> Status());
  NWM_Panel_Settings::Access -> _NWM -> UpdateType(NWM_WT_ModelView);
} //eof SmoothCallback()


void TrackballToggleCallback(char E) {
  NWM_Panel_Settings::Access -> _ObjectList -> Trackball(NWM_Panel_Settings::Access -> _TrackballToggle -> Status());
} //eof TrackballToggleCallback()



NWM_Panel_Settings* NWM_Panel_Settings::Access= 0;
NWM_Panel_Settings::NWM_Panel_Settings(int H1, int H2, int H3, int H4, NWM_List *obj, NWM *NWMPtr, Object_Manager *ObjectList) : NWM_Base(obj,NWM_WT_Panel_Settings) {
  _self= this;
  Access= _self;


  _NWM= NWMPtr;
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

  _MBCGRIDTOGGLE= new NTK_Button(_Window,0,0,150,20);
  _MBCGRIDTOGGLE -> Type(NTK_BUTTON_TEXT);
  _MBCGRIDTOGGLE -> Sticky(NTK_ON);
  _MBCGRIDTOGGLE -> Text("BG / Grid");
  _MBCGRIDTOGGLE -> Color(0xA0,0xA0,0xA0);
  _MBCGRIDTOGGLE -> CallbackTrigger(NTK_CT_MOUSE_DOWN);

  _MBCR= new NTK_Slider_Horz(_Window,0,20,150,20);
  _MBCR -> Type(NTK_HORZ_SLIDER);
  _MBCR -> Min(0);
  _MBCR -> Max(1);
  _MBCR -> Precision(2);
  _MBCR -> Value(.45);
  _MBCR -> Color(0xA0,0xA0,0xA0);
  _MBCR -> SliderColor(0xff,0x00,0x00);
  _MBCR -> CallbackTrigger(NTK_CT_MOTION | NTK_CT_MOUSE_UP);
  _MBCR -> Callback((NTK_Callback*)MBCCallback);

  _MBCG= new NTK_Slider_Horz(_Window,0,40,150,20);
  _MBCG -> Type(NTK_HORZ_SLIDER);
  _MBCG -> Min(0);
  _MBCG -> Max(1);
  _MBCG -> Precision(2);
  _MBCG -> Value(.45);
  _MBCG -> Color(0xA0,0xA0,0xA0);
  _MBCG -> SliderColor(0x00,0xff,0x00);
  _MBCG -> CallbackTrigger(NTK_CT_MOTION | NTK_CT_MOUSE_UP);
  _MBCG -> Callback((NTK_Callback*)MBCCallback);

  _MBCB= new NTK_Slider_Horz(_Window,0,60,150,20);
  _MBCB -> Type(NTK_HORZ_SLIDER);
  _MBCB -> Min(0);
  _MBCB -> Max(1);
  _MBCB -> Precision(2);
  _MBCB -> Value(.45);
  _MBCB -> Color(0xA0,0xA0,0xA0);
  _MBCB -> SliderColor(0x00,0x00,0xff);
  _MBCB -> CallbackTrigger(NTK_CT_MOTION | NTK_CT_MOUSE_UP);
  _MBCB -> Callback((NTK_Callback*)MBCCallback);

  // Trackball
  _TrackballToggle= new NTK_Button(_Window,150,0,150,20);
  _TrackballToggle -> Status(NTK_ON);
  _TrackballToggle -> Sticky(NTK_ON);
  _TrackballToggle -> Type(NTK_BUTTON_TEXT);
  _TrackballToggle -> Text("Trackball");
  _TrackballToggle -> Color(0xA0,0xA0,0xA0);
  _TrackballToggle -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _TrackballToggle -> Callback((NTK_Callback*)TrackballToggleCallback);

  _TrackballSpeed= new NTK_Slider_Horz(_Window,150,20,150,20);
  _TrackballSpeed -> Type(NTK_HORZ_SLIDER);
  _TrackballSpeed -> Min(.5);
  _TrackballSpeed -> Max(2);
  _TrackballSpeed -> Precision(2);
  _TrackballSpeed -> Value(1.25);
  _TrackballSpeed -> Color(0xA0,0xA0,0xA0);
  _TrackballSpeed -> SliderColor(0xff,0xff,0xff);
  _TrackballSpeed -> CallbackTrigger(NTK_CT_MOTION | NTK_CT_MOUSE_UP);
  _TrackballSpeed -> Callback((NTK_Callback*)TrackballSpeedCallback);

  _Smooth= new NTK_Button(_Window,150,40,150,20);
  _Smooth -> Status(NTK_ON);
  _Smooth -> Type(NTK_BUTTON_TEXT);
  _Smooth -> Sticky(NTK_ON);
  _Smooth -> Color(0xA0,0xA0,0xA0);
  _Smooth -> Text("Smooth Shading");
//  _Smooth -> Container(_EditContainer);
  _Smooth -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _Smooth -> Callback((NTK_Callback*)SmoothCallback);

  // Tesselation
  _TessSlider= new NTK_Slider_Button(_Window,150,60,150,20);
  _TessSlider -> Type(NTK_HORZ_SLIDER);
  _TessSlider -> StaticText("Tess:");
  _TessSlider -> Min(2);
  _TessSlider -> Max(128);
  _TessSlider -> Step(2);
  _TessSlider -> Value(40);
  _TessSlider -> Color(0xA0,0xA0,0xA0);
  _TessSlider -> SliderColor(0xA0,0x90,0x80);
//  _TessSlider -> Container(_EditContainer);
  _TessSlider -> CallbackTrigger(NTK_CT_MOTION | NTK_CT_MOUSE_UP | NTK_CT_KEY);
  _TessSlider -> Callback((NTK_Callback*)TesselationCallback);

  // FOV
  _FOVAngle= new NTK_Slider_Button(_Window,300,0,150,20);
  _FOVAngle -> Type(NTK_HORZ_SLIDER);
  _FOVAngle -> StaticText("FOV:");
  _FOVAngle -> Min(30);
  _FOVAngle -> Max(120);
  _FOVAngle -> Step(5);
  _FOVAngle -> Value(52);
  _FOVAngle -> Color(0xA0,0xA0,0xA0);
  _FOVAngle -> SliderColor(0xff,0xff,0xff);
  _FOVAngle -> CallbackTrigger(NTK_CT_MOTION | NTK_CT_MOUSE_UP | NTK_CT_KEY);
  _FOVAngle -> Callback((NTK_Callback*)FOVAngleCallback);

/*
  _SaveSettings= new NTK_Button(_ControlWindow,0,0,125,16);
  _SaveSettings -> Type(NTK_BUTTON_TEXT);
  _SaveSettings -> Color(0xA0,0xA0,0xA0);
  _SaveSettings -> Text("Save Settings");
//  _Smooth -> Container(_EditContainer);
  _SaveSettings -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
//  _SaveSettings -> Callback((NTK_Callback*)SmoothCallback);
  _SaveSettings -> DynPos(NTK_DYN_LEFT | NTK_DYN_BOTTOM);
  _SaveSettings -> DynPosY(35);
  _SaveSettings -> DynPosX(400);
*/
} //eof NWM_Panel_Settings::NWM_Panel_Settings()


void NWM_Panel_Settings::Update() {
  glEnable(GL_SCISSOR_TEST);
    glViewport(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
    glScissor(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
      _Window -> Draw();
  glDisable(GL_SCISSOR_TEST);
} //eof NWM_Panel_Settings::Update()


void NWM_Panel_Settings::Refresh() {
  _Window -> SetClear(2);
  _Window -> UpdateAll();
} //eof NWM_Panel_Settings::Refresh()


void NWM_Panel_Settings::Reshape(int w, int h) {
  // Construct Control Panel Viewport
  _Viewport[0]= _PosX;
  _Viewport[1]= h-(_PosY+_SizY);
  _Viewport[2]= _SizX;
  _Viewport[3]= _SizY;

  _Window -> ScreenSize(_SizX,_SizY);
} //eof NWM_Panel_Settings::Reshape()


char NWM_Panel_Settings::Key(int x, int y) {
  Access= _self;
  _Update= 2;

  _Window -> Handle(NTK_EVENT_KEY,x,y,0,0);
  return(NWM_NOTHING);
} //eof NWM_Panel_Settings::Key()


char NWM_Panel_Settings::Mouse(int button, int state, int x, int y) {
  _Window -> Handle(NTK_EVENT_MOUSE,button,state,x,y);
  _Update= 2;

  return(NWM_NOTHING);
} //eof NWM_Panel_Settings::Mouse()


char NWM_Panel_Settings::Motion(int x, int y) {
  _Window -> Handle(NTK_EVENT_MOTION,x,y,0,0);
  _Update= 1;

  return(NWM_NOTHING);
} //eof NWM_Panel_Settings::Motion()


char NWM_Panel_Settings::Passive(int x, int y) {
  return(NWM_NOTHING);
} //eof NWM_Panel_Settings::Passive()


void NWM_Panel_Settings::Active(bool A) {
} //eof NWM_Panel_Settings::Active()
