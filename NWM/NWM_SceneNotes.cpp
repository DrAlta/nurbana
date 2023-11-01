#include "NWM_SceneNotes.h"

// +-----------------+
// | Local Callbacks |
// +-----------------+

void SaveNotesCallback(char E) {
  NWM_SceneNotes::Access -> _ObjectList -> Notes(NWM_SceneNotes::Access -> _SceneNotesTextArea -> Text());
  NWM_SceneNotes::Access -> SelfDestruct(1);
} //eof SaveNotesCallback()

// +---------------------+
// | EOF Local Callbacks |
// +---------------------+


NWM_SceneNotes* NWM_SceneNotes::Access= 0;
NWM_SceneNotes::NWM_SceneNotes(int H1, int H2, int H3, int H4, NWM *NWMPtr, NWM_List *obj, Object_Manager *ObjectList) : NWM_Base(obj,NWM_WT_SceneNotes) {
  _self= this;
  Access= _self;


  _NWM= NWMPtr;
  _ObjectList= ObjectList;
//  _MaterialList= MatList;

  // Assign Handles
  _H1= H1;
  _H2= H2;
  _H3= H3;
  _H4= H4;

  _SceneNotesWindow= new NTK_Window(_NWM -> Path());

  //
  // Configure Widgets
  //

  _SceneNotesLabel= new NTK_Label(_SceneNotesWindow,0,0,100,20);
  _SceneNotesLabel -> Bevel(NTK_ON);
  _SceneNotesLabel -> Text("Scene Notes");
  _SceneNotesLabel -> Color(0xB0,0xB0,0xB0);
  _SceneNotesLabel -> DynPos(NTK_DYN_LEFT | NTK_DYN_TOP);
  _SceneNotesLabel -> DynPosX(10);
  _SceneNotesLabel -> DynPosY(5);
  _SceneNotesLabel -> DynSiz(NTK_DYN_WIDTH);
  _SceneNotesLabel -> DynSizX(10);

  _SceneNotesTextArea= new NTK_TextArea(_SceneNotesWindow,0,0,0,0);
  _SceneNotesTextArea -> FontSize(20);
  _SceneNotesTextArea -> Color(0x98,0x98,0x98);
  _SceneNotesTextArea -> DynPos(NTK_DYN_LEFT | NTK_DYN_TOP);
  _SceneNotesTextArea -> DynPosX(10);
  _SceneNotesTextArea -> DynPosY(30);
  _SceneNotesTextArea -> DynSiz(NTK_DYN_WIDTH | NTK_DYN_HEIGHT);
  _SceneNotesTextArea -> DynSizX(10);
  _SceneNotesTextArea -> DynSizY(30);
  _SceneNotesTextArea -> Text(_ObjectList -> Notes());

  _SceneNotesButton= new NTK_Button(_SceneNotesWindow,0,0,75,20);
  _SceneNotesButton -> Type(NTK_BUTTON_TEXT);
  _SceneNotesButton -> Text("Save Notes");
  _SceneNotesButton -> Color(0x80,0x90,0xA0);
  _SceneNotesButton -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _SceneNotesButton -> Callback((NTK_Callback*)SaveNotesCallback);
  _SceneNotesButton -> DynPos(NTK_DYN_LEFT | NTK_DYN_BOTTOM);
  _SceneNotesButton -> DynPosX(10);
  _SceneNotesButton -> DynPosY(5);
  _SceneNotesButton -> DynSiz(NTK_DYN_WIDTH);
  _SceneNotesButton -> DynSizX(10);
} //eof NWM_SceneNotes::NWM_SceneNotes()


NWM_SceneNotes::~NWM_SceneNotes() {
  delete _SceneNotesWindow;
} //eof NWM_SceneNotes::~NWM_SceneNotes()


void NWM_SceneNotes::Update() {
  glEnable(GL_SCISSOR_TEST);
    glViewport(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
    glScissor(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
      _SceneNotesWindow -> Draw();
  glDisable(GL_SCISSOR_TEST);
} //eof NWM_SceneNotes::Update()


void NWM_SceneNotes::Refresh() {
  _SceneNotesWindow -> SetClear(2);
  _SceneNotesWindow -> UpdateAll();
} //eof NWM_SceneNotes::Refresh()


void NWM_SceneNotes::Reshape(int w, int h) {
  // Construct File Manager Viewport
  _Viewport[0]= _PosX;
  _Viewport[1]= h-(_PosY+_SizY);
  _Viewport[2]= _SizX;
  _Viewport[3]= _SizY;

  _SceneNotesWindow -> ScreenSize(_SizX,_SizY);
} //eof NWM_SceneNotes::Reshape()


char NWM_SceneNotes::Key(int x, int y) {
  _SceneNotesWindow -> Handle(NTK_EVENT_KEY,x,y,0,0);
  _Update= 2;

  return(NWM_NOTHING);
} //eof NWM_SceneNotes::Key()


char NWM_SceneNotes::Mouse(int button, int state, int x, int y) {
  _SceneNotesWindow -> Handle(NTK_EVENT_MOUSE,button,state,x,y);
  _Update= 2;

  if (state) {
    return(NWM_LOCK);
  } else {
    if (!_SceneNotesWindow -> TypeActive(NTK_WT_TextArea))
      return(NWM_UNLOCK);
  } //fi

  return(NWM_NOTHING);
} //eof NWM_SceneNotes::Mouse()


char NWM_SceneNotes::Motion(int x, int y) {
  _SceneNotesWindow -> Handle(NTK_EVENT_MOTION,x,y,0,0);
  _Update= 1;

  return(NWM_NOTHING);
} //eof NWM_SceneNotes::Motion()


char NWM_SceneNotes::Passive(int x, int y) {
  return(NWM_NOTHING);
} //eof NWM_SceneNotes::Passive()


void NWM_SceneNotes::Active(bool A) {
  if (A) {
    _SceneNotesLabel -> Color(0xB0,0xB0,0xB0);
  } else {
    _SceneNotesLabel -> Color(0x98,0x98,0x98);
  } //fi
  _Update= 2;
} //eof NWM_SceneNotes::Active()
