#include "NWM_Panel_Render.h"

//XPMs
#include "../NTK/XPMs/dash.xpm"


//
// Local Callbacks
//

void RenderCallback(char E) {
  NWM_Panel_Render::Access -> _NWM -> CreateMenu(NWM_MENU_RENDER);
} //eof RenderCallback()

void RPPCallback(char E) {
  NWM_Panel_Render::Access -> _RT -> Rays(NWM_Panel_Render::Access -> _Rays -> Status());
} //eof RPPCallback()

void PacketSizeCallback(char E) {
  NWM_Panel_Render::Access -> _RT -> PacketSize(NWM_Panel_Render::Access -> _PacketSize -> Value());
} //eof PacketSizeCallback()

void PacketTimeCallback(char E) {
  NWM_Panel_Render::Access -> _RT -> PacketTime(NWM_Panel_Render::Access -> _PacketTime -> Value());
} //eof PacketTimeCallback()

void RenderSizeCallback(char E) {
  NWM_Panel_Render::Access -> _RT -> Size(NWM_Panel_Render::Access -> _RenderSizeX -> Value(),NWM_Panel_Render::Access -> _RenderSizeY -> Value());
} //eof RenderSizeCallback()

void ThreadsCallback(char E) {
  NWM_Panel_Render::Access -> _RT -> Threads(NWM_Panel_Render::Access -> _Threads -> Value());
} //eof ThreadsCallback()


NWM_Panel_Render* NWM_Panel_Render::Access= 0;
NWM_Panel_Render::NWM_Panel_Render(int H1, int H2, int H3, int H4, NWM_List *obj, NWM *NWMptr, Object_Manager *ObjectList, NRT_RayTracer *RT) : NWM_Base(obj,NWM_WT_Panel_Render) {
  _self= this;
  Access= _self;

  _NWM= NWMptr;
  _ObjectList= ObjectList;
  _RT= RT;

  // Assign Handles
  _H1= H1;
  _H2= H2;
  _H3= H3;
  _H4= H4;


  _Window= new NTK_Window(_NWM -> Path());

  //
  // Configure Widgets
  //

  _Render= new NTK_Button(_Window,0,0,120,40);
  _Render -> Type(NTK_BUTTON_TEXT);
  _Render -> Color(0x80,0x90,0xA0);
  _Render -> Text("Render");
  _Render -> CallbackTrigger(NTK_CT_MOUSE_UP);
  _Render -> Callback((NTK_Callback*)RenderCallback);

  _Rays= new NTK_Slider_Button(_Window,0,40,120,20);
  _Rays -> Type(NTK_BUTTON_TEXT);
  _Rays -> StaticText("Rays:");
  _Rays -> Color(0xA0,0xA0,0xA0);
  _Rays -> Value(1);
  _Rays -> Min(1);
  _Rays -> Max(16);
  _Rays -> Step(1);
  _Rays -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _Rays -> Callback((NTK_Callback*)RPPCallback);

  _Threads= new NTK_Slider_Button(_Window,0,60,120,20);
  _Threads -> Type(NTK_BUTTON_TEXT);
  _Threads -> Color(0xA0,0xA0,0xA0);
  _Threads -> StaticText("Threads:");
  _Threads -> Value(1);
  _Threads -> Min(1);
  _Threads -> Max(2);
  _Threads -> Step(1);
  _Threads -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _Threads -> Callback((NTK_Callback*)ThreadsCallback);

  _RenderSizeX= new NTK_Slider_Button(_Window,120,0,120,20);
  _RenderSizeX -> Type(NTK_HORZ_SLIDER);
  _RenderSizeX -> StaticText("Size X:");
  _RenderSizeX -> Min(128);
  _RenderSizeX -> Max(512);
  _RenderSizeX -> Step(4);
  _RenderSizeX -> Value(320);
  _RenderSizeX -> Color(0xA0,0xA0,0xA0);
  _RenderSizeX -> SliderColor(0x80,0x90,0xA0);
  _RenderSizeX -> CallbackTrigger(NTK_CT_MOTION | NTK_CT_MOUSE_UP);
  _RenderSizeX -> Callback((NTK_Callback*)RenderSizeCallback);

  _RenderSizeY= new NTK_Slider_Button(_Window,120,20,120,20);
  _RenderSizeY -> Type(NTK_HORZ_SLIDER);
  _RenderSizeY -> StaticText("Size Y:");
  _RenderSizeY -> Min(128);
  _RenderSizeY -> Max(512);
  _RenderSizeY -> Step(4);
  _RenderSizeY -> Value(320);
  _RenderSizeY -> Color(0xA0,0xA0,0xA0);
  _RenderSizeY -> SliderColor(0x80,0x90,0xA0);
  _RenderSizeY -> CallbackTrigger(NTK_CT_MOTION | NTK_CT_MOUSE_UP);
  _RenderSizeY -> Callback((NTK_Callback*)RenderSizeCallback);

  _PacketSize= new NTK_Slider_Button(_Window,120,40,120,20);
  _PacketSize -> Type(NTK_HORZ_SLIDER);
  _PacketSize -> StaticText("P Size:");
  _PacketSize -> Min(16);
  _PacketSize -> Max(1024);
  _PacketSize -> Step(16);
  _PacketSize -> Value(64);
  _PacketSize -> Color(0xA0,0xA0,0xA0);
  _PacketSize -> SliderColor(0x80,0x90,0xA0);
  _PacketSize -> CallbackTrigger(NTK_CT_MOUSE_UP);

  _PacketTime= new NTK_Slider_Button(_Window,120,60,120,20);
  _PacketTime -> Type(NTK_HORZ_SLIDER);
  _PacketTime -> StaticText("P Time:");
  _PacketTime -> Min(0.1);
  _PacketTime -> Max(2);
  _PacketTime -> Value(1);
  _PacketTime -> Step(0.1);
  _PacketTime -> Precision(1);
  _PacketTime -> Color(0xA0,0xA0,0xA0);
  _PacketTime -> SliderColor(0x80,0x90,0xA0);
  _PacketTime -> CallbackTrigger(NTK_CT_MOUSE_UP);
} //eof NWM_Panel_Render::NWM_Panel_Render()


void NWM_Panel_Render::Update() {
  glEnable(GL_SCISSOR_TEST);
    glViewport(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
    glScissor(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
      _Window -> Draw();
  glDisable(GL_SCISSOR_TEST);
} //eof NWM_Panel_Render::Update()


void NWM_Panel_Render::Refresh() {
  _Window -> SetClear(2);
  _Window -> UpdateAll();
} //eof NWM_Panel_Render::Refresh()


void NWM_Panel_Render::Reshape(int w, int h) {
  // Construct Main Panel Viewport
  _Viewport[0]= _PosX;
  _Viewport[1]= h-(_PosY+_SizY);
  _Viewport[2]= _SizX;
  _Viewport[3]= _SizY;
  _Viewport[4]= w;
  _Viewport[5]= h;

  _Window -> SetClear(2);  
  _Window -> ScreenSize(_SizX,_SizY);
} //eof NWM_Panel_Render::Reshape()


char NWM_Panel_Render::Key(int x, int y) {
  // For callbacks, assign _pself _lself
  Access= _self;
  _Update= 2;

  _Window -> Handle(NTK_EVENT_KEY,x,y,0,0);
  return(NWM_NOTHING);
} //eof NWM_Panel_Render::Key()


char NWM_Panel_Render::Mouse(int button, int state, int x, int y) {
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
} //eof NWM_Panel_Render::Mouse()


char NWM_Panel_Render::Motion(int x, int y) {
  // For callbacks, assign _pself _lself
  Access= _self;

  _Window -> Handle(NTK_EVENT_MOTION,x,y,0,0);
  _Update= 1;
  return(NWM_NOTHING);
} //eof NWM_Panel_Render::Motion()


char NWM_Panel_Render::Passive(int x, int y) {
  // For callbacks, assign _pself _lself
  Access= _self;

  return(NWM_NOTHING);
} //eof NWM_Panel_Render::Passive()


void NWM_Panel_Render::Active(bool A) {
} //eof NWM_Panel_Render::Active()


void NWM_Panel_Render::Misc(int V) {
  _Window -> SetClear(2);
  _Window -> UpdateAll();
} //eof Panel_Render::Misc()
