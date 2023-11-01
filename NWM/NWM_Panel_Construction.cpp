#include "NWM_Panel_Construction.h"


//
// Callbacks
//

void LoftCallback(char E) {
  NWM_Panel_Construction::Access -> _NWM -> CreateMenu(NWM_MENU_LOFT);
} //eof LoftCallback()

void BlendCallback(char E) {
  NWM_Panel_Construction::Access -> _NWM -> CreateMenu(NWM_MENU_BLEND);
} //eof BlendCallback()

void RebuildCallback(char E) {
  NWM_Panel_Construction::Access -> _NWM -> CreateMenu(NWM_MENU_REBUILD);
} //eof RebuildCallback()




NWM_Panel_Construction* NWM_Panel_Construction::Access= 0;
NWM_Panel_Construction::NWM_Panel_Construction(int H1, int H2, int H3, int H4, NWM_List *obj, NWM *NWMptr, Object_Manager *ObjectList) : NWM_Base(obj,NWM_WT_Panel_Construction) {
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

  _Loft= new NTK_Button(_Window,0,0,100,20);
  _Loft -> Type(NTK_BUTTON_TEXT);
  _Loft -> Color(0xA0,0xA0,0xA0);
  _Loft -> Text("Loft");
  _Loft -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _Loft -> Callback((NTK_Callback*)LoftCallback);

  _Blend= new NTK_Button(_Window,0,20,100,20);
  _Blend -> Type(NTK_BUTTON_TEXT);
  _Blend -> Color(0xA0,0xA0,0xA0);
  _Blend -> Text("Blend");
  _Blend -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _Blend -> Callback((NTK_Callback*)BlendCallback);

  _Rebuild= new NTK_Button(_Window,0,40,100,20);
  _Rebuild -> Type(NTK_BUTTON_TEXT);
  _Rebuild -> Color(0xA0,0xA0,0xA0);
  _Rebuild -> Text("Rebuild");
  _Rebuild -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _Rebuild -> Callback((NTK_Callback*)RebuildCallback);

  _Window -> SetClear(2);
  _Window -> UpdateAll();
} //eof NWM_Panel_Construction::NWM_Panel_Construction()


void NWM_Panel_Construction::Update() {
  glEnable(GL_SCISSOR_TEST);
    glViewport(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
    glScissor(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
      _Window -> Draw();
  glDisable(GL_SCISSOR_TEST);
} //eof NWM_Panel_Construction::Update()


void NWM_Panel_Construction::Refresh() {
  _Window -> SetClear(2);
  _Window -> UpdateAll();
} //eof NWM_Panel_Construction::Refresh()


void NWM_Panel_Construction::Reshape(int w, int h) {
  // Construct Main Panel Viewport
  _Viewport[0]= _PosX;
  _Viewport[1]= h-(_PosY+_SizY);
  _Viewport[2]= _SizX;
  _Viewport[3]= _SizY;
  _Viewport[4]= w;
  _Viewport[5]= h;

  _Window -> SetClear(2);  
  _Window -> ScreenSize(_SizX,_SizY);
} //eof NWM_Panel_Construction::Reshape()


char NWM_Panel_Construction::Key(int x, int y) {
  // For callbacks, assign _pself _lself
  Access= _self;
  _Update= 2;

  _Window -> Handle(NTK_EVENT_KEY,x,y,0,0);
  return(NWM_NOTHING);
} //eof NWM_Panel_Construction::Key()


char NWM_Panel_Construction::Mouse(int button, int state, int x, int y) {
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
} //eof NWM_Panel_Construction::Mouse()


char NWM_Panel_Construction::Motion(int x, int y) {
  // For callbacks, assign _pself _lself
  Access= _self;

  _Window -> Handle(NTK_EVENT_MOTION,x,y,0,0);
  _Update= 1;
  return(NWM_NOTHING);
} //eof NWM_Panel_Construction::Motion()


char NWM_Panel_Construction::Passive(int x, int y) {
  // For callbacks, assign _pself _lself
  Access= _self;

  return(NWM_NOTHING);
} //eof NWM_Panel_Construction::Passive()


void NWM_Panel_Construction::Active(bool A) {
} //eof NWM_Panel_Construction::Active()


void NWM_Panel_Construction::Misc(int V) {
} //eof Panel_Construction::Misc()
