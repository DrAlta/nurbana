#include "NWM_ModelView.h"


//XPMs
#include "../NTK/XPMs/tran.xpm"
#include "../NTK/XPMs/zoom.xpm"


// +-----------+
// | Callbacks |
// +-----------+

void ZoomIdle() {
  NWM_ModelView::Access -> Update(1);
  NWM_ModelView::Access -> _ModelView -> MouseZoom(NWM_ModelView::Access -> _Ypos[1]-NWM_ModelView::Access -> _Ypos[0]);
//  glutPostRedisplay();
} //eof ZoomIdle

void ZoomCallback(char E) {
  if (E == NTK_EVENT_MOUSE_DOWN) {
//    glutIdleFunc(ZoomIdle);
    NIdle::Callback(ZoomIdle);
  } else {
//    glutIdleFunc(NULL);
    NIdle::Callback(NULL);
  } //fi
} //eof ZoomCallback()

void TranslateCallback(char E) {
  if (E == NTK_EVENT_MOUSE_DOWN) {
    NWM_ModelView::Access -> _Tran= 1;
    NWM_ModelView::Access -> _ModelView -> MouseTranslate(NWM_ModelView::Access -> _Xpos[1], NWM_ModelView::Access -> _Ypos[1],1);
  } else if (E == NTK_EVENT_MOUSE_UP) {
    NWM_ModelView::Access -> _Tran= 0;
  } //fi

  NWM_ModelView::Access -> Update(1);
  NWM_ModelView::Access -> _ModelView -> MouseTranslate(NWM_ModelView::Access -> _Xpos[1], NWM_ModelView::Access -> _Ypos[1],0);
} //eof TranslateCallback()

// +---------------+
// | EOF Callbacks |
// +---------------+



NWM_ModelView* NWM_ModelView::Access= 0;
NWM_ModelView::NWM_ModelView(int H1, int H2, int H3, int H4, NWM_List *obj, NWM *NWMPtr, Object_Manager *ObjectList) : NWM_Base(obj,NWM_WT_ModelView) {
  _self= this;
  Access= _self;
  _Splitable= 1;

  _NWM= NWMPtr;
  _ObjectList= ObjectList;
  _ModelView= new ModelView(_ObjectList);

  // Assign Handles
  _H1= H1;
  _H2= H2;
  _H3= H3;
  _H4= H4;

  // Minimum Height restriction of 25
  _MinSizY= 25;

  _ModelView -> Init();
  _ControlWindow= new NTK_Window(_NWM -> Path());


  //
  // Configure Widgets
  //
  _ControlBar= new NTK_Label(_ControlWindow,0,0,0,25);
  _ControlBar -> Bevel(NTK_ON);
  _ControlBar -> Align(NTK_JUSTIFY_LEFT);
  _ControlBar -> Color(0x98,0x98,0x98);
  _ControlBar -> DynSiz(NTK_DYN_WIDTH);

  _ButtonContainer= new NTK_Container(0,0);

  _Translate= new NTK_Button(_ControlWindow,0,1,22,22);
  _Translate -> Layer(1);
  _Translate -> Rounded(NTK_ON);
  _Translate -> Type(NTK_BUTTON_GLYPHIC);
  _Translate -> Color(0x80,0x90,0xA0);
  _Translate -> Glyphic(_ControlWindow -> GlyphicXPM(tran_xpm,NTK_OFF));
  _Translate -> Container(_ButtonContainer);
  _Translate -> CallbackTrigger(NTK_CT_MOUSE_DOWN | NTK_CT_MOUSE_DOWN | NTK_CT_MOTION);
  _Translate -> Callback((NTK_Callback*)TranslateCallback);

  _Zoom= new NTK_Button(_ControlWindow,25,1,23,22);
  _Zoom -> Layer(1);
  _Zoom -> Rounded(NTK_ON);
  _Zoom -> Type(NTK_BUTTON_GLYPHIC);
  _Zoom -> Color(0x80,0x90,0xA0);
  _Zoom -> Glyphic(_ControlWindow -> GlyphicXPM(zoom_xpm,NTK_OFF));
  _Zoom -> Container(_ButtonContainer);
  _Zoom -> CallbackTrigger(NTK_CT_MOUSE_DOWN | NTK_CT_MOUSE_UP);
  _Zoom -> Callback((NTK_Callback*)ZoomCallback);

  _Tran= 0;
} //eof NWM_ModelView::NWM_ModelView()


NWM_ModelView::~NWM_ModelView() {
  delete _ControlWindow;
} //eof NWM_ModelView::~NWM_ModelView


void NWM_ModelView::Update() {
  glEnable(GL_SCISSOR_TEST);
    glViewport(_ViewportA[0],_ViewportA[1],_ViewportA[2],_ViewportA[3]);
    glScissor(_ViewportA[0],_ViewportA[1],_ViewportA[2],_ViewportA[3]);
      _ModelView -> Draw();
    glViewport(_ViewportB[0],_ViewportB[1],_ViewportB[2],_ViewportB[3]);
    glScissor(_ViewportB[0],_ViewportB[1],_ViewportB[2],_ViewportB[3]);
      _ControlWindow -> Draw();
  glDisable(GL_SCISSOR_TEST);
} //eof NWM_ModelView::Update()


void NWM_ModelView::Refresh() {
  _ControlWindow -> SetClear(2);
  _ControlWindow -> UpdateAll();
} //eof NWM_ModelView::Refresh()


void NWM_ModelView::Reshape(int w, int h) {
  // Construct ModelView Viewport
  _ViewportA[0]= _PosX;
  _ViewportA[1]= h-(_PosY+_SizY-25);
  _ViewportA[2]= _SizX;
  _ViewportA[3]= _SizY-25;

  // Construct ControlWindow Viewport
  _ViewportB[0]= _PosX;
  _ViewportB[1]= h-(_PosY+_SizY);
  _ViewportB[2]= _SizX;
  _ViewportB[3]= 25;


  _ModelView -> Reshape(_SizX,_SizY-25);
  _ModelView -> Viewport(_ViewportA[0],_ViewportA[1],_ViewportA[2],_ViewportA[3]);
  
  _ControlWindow -> ScreenSize(_SizX,25);
  _ButtonContainer -> Position((_SizX-50)/2,0);
} //eof NWM_ModelView::Reshape()


char NWM_ModelView::Key(int x, int y) {
  // For callbacks, assign _pself _lself
  Access= _self;
  _Update= 2;

  // Reset _ControlBar Text and Make Button Container Visibile Once again
  if (KeyTable::Key() == SDLK_RETURN || KeyTable::Key() == SDLK_ESCAPE) {
    _ControlBar -> Text("");
    _ButtonContainer -> Visibility(NTK_CONTAINER_VISIBLE);
  } //fi

//  _NWM -> RefreshType(NWM_WT_MainPanel);

  return( _ModelView -> Key(x,y) );
} //eof NWM_ModelView::Key()


char NWM_ModelView::Mouse(int button, int state, int x, int y) {
  // For callbacks, assign _pself _lself
  char		Response;

  Access= _self;
  _Xpos[0]= _Xpos[1]= x;
  _Ypos[0]= _Ypos[1]= y;
  _Button= button;

  // Reset _ControlBar Text and Make Button Container Visibile Once again
  _ControlBar -> Text("");
  _ButtonContainer -> Visibility(NTK_CONTAINER_VISIBLE);


  if (y <= _ViewportA[3]) {
    _ControlWindow -> Handle(NTK_EVENT_MOUSE,button,state,x,y-_ViewportA[3]);
    Response= _ModelView -> Mouse(button, state, x, y);
  } else {
    _ControlWindow -> Handle(NTK_EVENT_MOUSE,button,state,x,y-_ViewportA[3]);
  } //fi

  _Update= 2;

  if (state) {
    if (_Button == SDL_BUTTON_MIDDLE && (KeyTable::Key() == SDLK_LCTRL || KeyTable::Key() == SDLK_RCTRL))
      NIdle::Callback(ZoomIdle);
//      glutIdleFunc(ZoomIdle);
//    _NWM -> RefreshType(NWM_WT_MainPanel);
    return(NWM_LOCK);
  } else {
//    glutIdleFunc(NULL);
    NIdle::Callback(NULL);
    return(NWM_UNLOCK);
  } //fi
} //eof NWM_ModelView::Mouse()


char NWM_ModelView::Motion(int x, int y) {
  // For callbacks, assign _pself _lself
  Access= _self;
  _Xpos[1]= x;
  _Ypos[1]= y;

  if (_Button == SDL_BUTTON_MIDDLE && (KeyTable::Key() == SDLK_LCTRL || KeyTable::Key() == SDLK_RCTRL)) {
    NWM_ModelView::Access -> _ModelView -> MouseZoom(NWM_ModelView::Access -> _Ypos[1]-NWM_ModelView::Access -> _Ypos[0]);
  } else {
    if (y <= _ViewportA[3]) {
      _ModelView -> Motion(x,y);
      if (_Tran)
        _ControlWindow -> Handle(NTK_EVENT_MOTION,x,y-_ViewportA[3],0,0);
    } else {
      _ControlWindow -> Handle(NTK_EVENT_MOTION,x,y-_ViewportA[3],0,0);
    } //fi
  } //fi

  _Update= 1;
  return(NWM_NOTHING);
} //eof NWM_ModelView::Motion()


char NWM_ModelView::Passive(int x, int y) {
  char	Label[256];
  // For callbacks, assign _pself _lself
  Access= _self;

  if (y <= _ViewportA[3]) {
    if (_ModelView -> Passive(x,y)) {
      _Update= 1;
      _NWM -> UpdateType(NWM_WT_ModelView);
    } //fi

    // Check for Grab, Rotate, and Size Mode to Update _ControlBar
    if (!_ObjectList -> GetMode(OBJECT_MODE_EDIT)) {
      if (_ObjectList -> GetMode(OBJECT_MODE_GRAB)) {
        _ButtonContainer -> Visibility(NTK_CONTAINER_INVISIBLE);
        sprintf(Label,"Loc: X:%.3f, Y:%.3f, Z:%.3f",_ObjectList -> GetCOR().x,_ObjectList -> GetCOR().y,_ObjectList -> GetCOR().z);
        _ControlBar -> Text(Label);
      } //fi

      if (_ObjectList -> GetMode(OBJECT_MODE_ROTATE)) {
        _ButtonContainer -> Visibility(NTK_CONTAINER_INVISIBLE);
        sprintf(Label,"Rot: X:%.3f, Y:%.3f, Z:%.3f",_ObjectList -> GetROT().x,_ObjectList -> GetROT().y,_ObjectList -> GetROT().z);
        _ControlBar -> Text(Label);
      } //fi

      if (_ObjectList -> GetMode(OBJECT_MODE_SIZE)) {
        _ButtonContainer -> Visibility(NTK_CONTAINER_INVISIBLE);
        sprintf(Label,"Size: X:%.3f, Y:%.3f, Z:%.3f",_ObjectList -> GetSIZ().x,_ObjectList -> GetSIZ().y,_ObjectList -> GetSIZ().z);
        _ControlBar -> Text(Label);
      } //fi
    } //fi      

  } else {
    _ControlWindow -> Handle(NTK_EVENT_PASSIVE,x,y-_ViewportA[3],0,0);
  } //fi

  return(NWM_NOTHING);
} //eof NWM_ModelView::Passive()


void NWM_ModelView::Active(bool A) {
  if (A) {
    _ControlBar -> Color(0xB0,0xB0,0xB0);
    if (_ObjectList -> GetMode(OBJECT_MODE_EDIT))
      Cursors::SetCursor(CURSOR_CROSSHAIR);
  } else {
    _ControlBar -> Color(0x98,0x98,0x98);
    Cursors::SetCursor(CURSOR_ARROW);
  } //fi

  _Update= 2;
} //eof NWM_ModelView::Active()
