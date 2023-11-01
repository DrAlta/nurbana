#include "NWM_StatusBar.h"


// +-----------+
// | Callbacks |
// +-----------+


// +---------------+
// | EOF Callbacks |
// +---------------+


NWM_StatusBar* NWM_StatusBar::Access= 0;
NWM_StatusBar::NWM_StatusBar(int H1, int H2, int H3, int H4, NWM_List *obj, NWM *NWMptr, Object_Manager *ObjectList) : NWM_Base(obj,NWM_WT_ControlPanel) {
  _self= this;
  Access= _self;

  _Head= obj;
  _NWM= NWMptr;
  _ObjectList= ObjectList;

  // Assign Handles
  _H1= H1;
  _H2= H2;
  _H3= H3;
  _H4= H4;


  _StatusBarWindow= new NTK_Window(_NWM -> Path());


  //
  // Configure Widgets
  //

  _StatusBarLabel= new NTK_Label(_StatusBarWindow,0,0,0,0);
  _StatusBarLabel -> Bevel(NTK_ON);
  _StatusBarLabel -> Align(NTK_JUSTIFY_LEFT);
  _StatusBarLabel -> Color(160,160,160);
  _StatusBarLabel -> DynSiz(NTK_DYN_WIDTH | NTK_DYN_HEIGHT);

  for (int i= 0; i < 5; i++) {
    for (int n= 0; n < 3; n++) {
      _LayerButtons[n+i*3]= new NTK_Button(_StatusBarWindow,2+i*10,2+n*10,10,10);
      _LayerButtons[n+i*3] -> Layer(1);
//      _LayerButtons[n+i*3] -> Rounded(NTK_ON);
      _LayerButtons[n+i*3] -> Type(NTK_BUTTON_TEXT);
      _LayerButtons[n+i*3] -> Sticky(NTK_ON);
      _LayerButtons[n+i*3] -> Color(0x80,0x90,0xA0);
      _LayerButtons[n+i*3] -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
//      _LayerButtons[n+i*3] -> Callback((NTK_Callback*)SmoothCallback);
    } //eof
  } //eof

  _ResetViewports= new NTK_Button(_StatusBarWindow,100,2,30,30);
  _ResetViewports -> Layer(1);
  _ResetViewports -> Rounded(NTK_ON);
  _ResetViewports -> Type(NTK_BUTTON_TEXT);
  _ResetViewports -> Color(0x80,0x90,0xA0);
  _ResetViewports -> CallbackTrigger(NTK_CT_MOUSE_DOWN);

  _SaveViewports= new NTK_Button(_StatusBarWindow,130,2,30,30);
  _SaveViewports -> Layer(1);
  _SaveViewports -> Rounded(NTK_ON);
  _SaveViewports -> Type(NTK_BUTTON_TEXT);
  _SaveViewports -> Color(0x80,0x90,0xA0);
  _SaveViewports -> CallbackTrigger(NTK_CT_MOUSE_DOWN);

  _Undo= new NTK_Button(_StatusBarWindow,160,2,30,30);
  _Undo -> Layer(1);
  _Undo -> Rounded(NTK_ON);
  _Undo -> Type(NTK_BUTTON_TEXT);
  _Undo -> Color(0x80,0x90,0xA0);
  _Undo -> CallbackTrigger(NTK_CT_MOUSE_DOWN);

  _Redo= new NTK_Button(_StatusBarWindow,190,2,30,30);
  _Redo -> Layer(1);
  _Redo -> Rounded(NTK_ON);
  _Redo -> Type(NTK_BUTTON_TEXT);
  _Redo -> Color(0x80,0x90,0xA0);
  _Redo -> CallbackTrigger(NTK_CT_MOUSE_DOWN);

  _LoadFile= new NTK_Button(_StatusBarWindow,220,2,30,30);
  _LoadFile -> Layer(1);
  _LoadFile -> Rounded(NTK_ON);
  _LoadFile -> Type(NTK_BUTTON_TEXT);
  _LoadFile -> Color(0x80,0x90,0xA0);
  _LoadFile -> CallbackTrigger(NTK_CT_MOUSE_DOWN);

  _SaveFile= new NTK_Button(_StatusBarWindow,250,2,30,30);
  _SaveFile -> Layer(1);
  _SaveFile -> Rounded(NTK_ON);
  _SaveFile -> Type(NTK_BUTTON_TEXT);
  _SaveFile -> Color(0x80,0x90,0xA0);
  _SaveFile -> CallbackTrigger(NTK_CT_MOUSE_DOWN);

  _StatusMessage= new NTK_Label(_StatusBarWindow,0,0,150,20);
  _StatusMessage -> Layer(1);
  _StatusMessage -> Rounded(NTK_ON);
  _StatusMessage -> Status(NTK_ON);
  _StatusMessage -> Bevel(NTK_ON);
  _StatusMessage -> Text("Rendering...10%");
  _StatusMessage -> Align(NTK_JUSTIFY_LEFT);
  _StatusMessage -> Color(192,192,192);
  _StatusMessage -> DynPos(NTK_DYN_RIGHT | NTK_DYN_BOTTOM);
  _StatusMessage -> DynPosX(4);
  _StatusMessage -> DynPosY(4);

} //eof NWM_StatusBar::NWM_StatusBar()


NWM_StatusBar::~NWM_StatusBar() {
  delete _StatusBarWindow;
} //eof NWM_StatusBar::~NWM_StatusBar()


void NWM_StatusBar::Update() {
  glEnable(GL_SCISSOR_TEST);
    glViewport(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
    glScissor(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
      _StatusBarWindow -> Draw();
  glDisable(GL_SCISSOR_TEST);
} //eof NWM_StatusBar::Update()


void NWM_StatusBar::Refresh() {
  _StatusBarWindow -> SetClear(2);
  _StatusBarWindow -> UpdateAll();
} //eof NWM_StatusBar::Refresh()


void NWM_StatusBar::Reshape(int w, int h) {
  // Construct Control Panel Viewport
  _Viewport[0]= _PosX;
  _Viewport[1]= h-(_PosY+_SizY);
  _Viewport[2]= _SizX;
  _Viewport[3]= _SizY;

  _StatusBarWindow -> ScreenSize(_SizX,_SizY);
} //eof NWM_StatusBar::Reshape()


char NWM_StatusBar::Key(int x, int y) {
  return(NWM_NOTHING);
} //eof NWM_StatusBar::Key()


char NWM_StatusBar::Mouse(int button, int state, int x, int y) {
  _StatusBarWindow -> Handle(NTK_EVENT_MOUSE,button,state,x,y);
  _Update= 2;

  return(NWM_NOTHING);
} //eof NWM_StatusBar::Mouse()


char NWM_StatusBar::Motion(int x, int y) {
  return(NWM_NOTHING);
} //eof NWM_StatusBar::Motion()


char NWM_StatusBar::Passive(int x, int y) {
  return(NWM_NOTHING);
} //eof NWM_StatusBar::Passive()


void NWM_StatusBar::Active(bool A) {
  if (A) {
    _StatusBarLabel -> Color(176,176,176);
  } else {
    _StatusBarLabel -> Color(160,160,160);
  } //fi
  _Update= 2;
} //eof NWM_StatusBar::Active()
