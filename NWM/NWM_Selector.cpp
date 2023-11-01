#include "NWM_Selector.h"

// XPMs
#include "../NTK/XPMs/primitive.xpm"
#include "../NTK/XPMs/materials.xpm"
#include "../NTK/XPMs/edit.xpm"
#include "../NTK/XPMs/construct.xpm"
#include "../NTK/XPMs/render.xpm"


//
// Callbacks
//

void SelectorPrimitivesMenuCallback(char E) {
  NWM_Selector::Access -> _PID= NWM_Selector::Access -> _NWM -> Replace(NWM_Selector::Access -> _PID, NWM_WT_Panel_Primitives);
} //eof SelectorPrimitivesMenuCallack()

void SelectorMaterialMenuCallback(char E) {
  NWM_Selector::Access -> _PID= NWM_Selector::Access -> _NWM -> Replace(NWM_Selector::Access -> _PID, NWM_WT_Panel_Materials);
} //eof SelectorMaterialMenuCallback()

void SelectorEditMenuCallback(char E) {
  NWM_Selector::Access -> _PID= NWM_Selector::Access -> _NWM -> Replace(NWM_Selector::Access -> _PID, NWM_WT_Panel_Edit);
} //eof SelectorEditMenuCallback()

void SelectorConstructionMenuCallback(char E) {
  NWM_Selector::Access -> _PID= NWM_Selector::Access -> _NWM -> Replace(NWM_Selector::Access -> _PID, NWM_WT_Panel_Construction);
} //eof SelectorConstructionMenuCallback()

void SelectorRenderMenuCallback(char E) {
  NWM_Selector::Access -> _PID= NWM_Selector::Access -> _NWM -> Replace(NWM_Selector::Access -> _PID, NWM_WT_Panel_Render);
} //eof SelectorRenderMenuCallback()

void SelectorSettingsMenuCallback(char E) {
  NWM_Selector::Access -> _PID= NWM_Selector::Access -> _NWM -> Replace(NWM_Selector::Access -> _PID, NWM_WT_Panel_Settings);
} //eof SelectorSettingsMenuCallback()

//
// EOF Callbacks
//


NWM_Selector* NWM_Selector::Access= 0;
NWM_Selector::NWM_Selector(int H1, int H2, int H3, int H4, NWM_List *obj, NWM *NWMptr, Object_Manager *ObjectList, int PID) : NWM_Base(obj,NWM_WT_ControlPanel) {
  _self= this;
  Access= _self;

  _Head= obj;
  _NWM= NWMptr;
  _ObjectList= ObjectList;
  _PID= PID;

  // Assign Handles
  _H1= H1;
  _H2= H2;
  _H3= H3;
  _H4= H4;


  _SelectorWindow= new NTK_Window(_NWM -> Path());


  //
  // Configure Widgets
  //
  _ButtonGroup= new NTK_Group();

  _PrimitiveMenu= new NTK_Button(_SelectorWindow,0,0,52,52);
  _PrimitiveMenu -> Status(NTK_ON);
  _PrimitiveMenu -> Rounded(NTK_ON);
  _PrimitiveMenu -> Sticky(NTK_ON);
  _PrimitiveMenu -> Group(_ButtonGroup);
  _PrimitiveMenu -> Layer(1);
  _PrimitiveMenu -> Type(NTK_BUTTON_GLYPHIC);
  _PrimitiveMenu -> Color(0x80,0x90,0xA0);
  _PrimitiveMenu -> Glyphic(_SelectorWindow -> GlyphicXPM(primitive_xpm,NTK_OFF));
  _PrimitiveMenu -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _PrimitiveMenu -> Callback((NTK_Callback*)SelectorPrimitivesMenuCallback);

  _MaterialMenu= new NTK_Button(_SelectorWindow,0,52,52,52);
  _MaterialMenu -> Sticky(NTK_ON);
  _MaterialMenu -> Rounded(NTK_ON);
  _MaterialMenu -> Group(_ButtonGroup);
  _MaterialMenu -> Layer(1);
  _MaterialMenu -> Type(NTK_BUTTON_GLYPHIC);
  _MaterialMenu -> Color(0x80,0x90,0xA0);
  _MaterialMenu -> Glyphic(_SelectorWindow -> GlyphicXPM(materials_xpm,NTK_OFF));
  _MaterialMenu -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _MaterialMenu -> Callback((NTK_Callback*)SelectorMaterialMenuCallback);

  _EditMenu= new NTK_Button(_SelectorWindow,0,104,52,52);
  _EditMenu -> Sticky(NTK_ON);
  _EditMenu -> Rounded(NTK_ON);
  _EditMenu -> Group(_ButtonGroup);
  _EditMenu -> Layer(1);
  _EditMenu -> Type(NTK_BUTTON_GLYPHIC);
  _EditMenu -> Color(0x80,0x90,0xA0);
  _EditMenu -> Glyphic(_SelectorWindow -> GlyphicXPM(edit_xpm,NTK_OFF));
  _EditMenu -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _EditMenu -> Callback((NTK_Callback*)SelectorEditMenuCallback);

  _ConstructionMenu= new NTK_Button(_SelectorWindow,0,156,52,52);
  _ConstructionMenu -> Sticky(NTK_ON);
  _ConstructionMenu -> Rounded(NTK_ON);
  _ConstructionMenu -> Group(_ButtonGroup);
  _ConstructionMenu -> Layer(1);
  _ConstructionMenu -> Type(NTK_BUTTON_GLYPHIC);
  _ConstructionMenu -> Color(0x80,0x90,0xA0);
  _ConstructionMenu -> Glyphic(_SelectorWindow -> GlyphicXPM(construct_xpm,NTK_OFF));
  _ConstructionMenu -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _ConstructionMenu -> Callback((NTK_Callback*)SelectorConstructionMenuCallback);

  _RenderMenu= new NTK_Button(_SelectorWindow,0,208,52,52);
  _RenderMenu -> Sticky(NTK_ON);
  _RenderMenu -> Rounded(NTK_ON);
  _RenderMenu -> Group(_ButtonGroup);
  _RenderMenu -> Layer(1);
  _RenderMenu -> Type(NTK_BUTTON_GLYPHIC);
  _RenderMenu -> Color(0x80,0x90,0xA0);
  _RenderMenu -> Glyphic(_SelectorWindow -> GlyphicXPM(render_xpm,NTK_OFF));
  _RenderMenu -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _RenderMenu -> Callback((NTK_Callback*)SelectorRenderMenuCallback);

  _CameraMenu= new NTK_Button(_SelectorWindow,0,260,52,52);
  _CameraMenu -> Sticky(NTK_ON);
  _CameraMenu -> Rounded(NTK_ON);
  _CameraMenu -> Group(_ButtonGroup);
  _CameraMenu -> Layer(1);
//  _CameraMenu -> Type(NTK_BUTTON_GLYPHIC);
  _CameraMenu -> Color(0x80,0x90,0xA0);
//  _CameraMenu -> Glyphic(_SelectorWindow -> GlyphicXPM(render_xpm,NTK_OFF));
  _CameraMenu -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
//  _CameraMenu -> Callback((NTK_Callback*)SelectorRenderMenuCallback);

  _LightingMenu= new NTK_Button(_SelectorWindow,0,312,52,52);
  _LightingMenu -> Sticky(NTK_ON);
  _LightingMenu -> Rounded(NTK_ON);
  _LightingMenu -> Group(_ButtonGroup);
  _LightingMenu -> Layer(1);
//  _LightingMenu -> Type(NTK_BUTTON_GLYPHIC);
  _LightingMenu -> Color(0x80,0x90,0xA0);
//  _LightingMenu -> Glyphic(_SelectorWindow -> GlyphicXPM(render_xpm,NTK_OFF));
  _LightingMenu -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
//  _CameraMenu -> Callback((NTK_Callback*)SelectorRenderMenuCallback);

  _SettingsMenu= new NTK_Button(_SelectorWindow,0,364,52,52);
  _SettingsMenu -> Sticky(NTK_ON);
  _SettingsMenu -> Rounded(NTK_ON);
  _SettingsMenu -> Group(_ButtonGroup);
  _SettingsMenu -> Layer(1);
//  _SettingsMenu -> Type(NTK_BUTTON_GLYPHIC);
  _SettingsMenu -> Color(0x80,0x90,0xA0);
//  _SettingsMenu -> Glyphic(_SelectorWindow -> GlyphicXPM(render_xpm,NTK_OFF));
  _SettingsMenu -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _SettingsMenu -> Callback((NTK_Callback*)SelectorSettingsMenuCallback);

  _CommandMenu= new NTK_Button(_SelectorWindow,0,416,52,52);
  _CommandMenu -> Sticky(NTK_ON);
  _CommandMenu -> Rounded(NTK_ON);
  _CommandMenu -> Group(_ButtonGroup);
  _CommandMenu -> Layer(1);
//  _CommandMenu -> Type(NTK_BUTTON_GLYPHIC);
  _CommandMenu -> Color(0x80,0x90,0xA0);
//  _CommandMenu -> Glyphic(_SelectorWindow -> GlyphicXPM(render_xpm,NTK_OFF));
  _CommandMenu -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
//  _CameraMenu -> Callback((NTK_Callback*)SelectorRenderMenuCallback);
} //eof NWM_Selector::NWM_Selector()


NWM_Selector::~NWM_Selector() {
  delete _SelectorWindow;
} //eof NWM_Selector::~NWM_Selector()


void NWM_Selector::Update() {
  glEnable(GL_SCISSOR_TEST);
    glViewport(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
    glScissor(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
      _SelectorWindow -> Draw();
  glDisable(GL_SCISSOR_TEST);
} //eof NWM_Selector::Update()


void NWM_Selector::Refresh() {
  _SelectorWindow -> SetClear(2);
  _SelectorWindow -> UpdateAll();
} //eof NWM_Selector::Refresh()


void NWM_Selector::Reshape(int w, int h) {
  // Construct Control Panel Viewport
  _Viewport[0]= _PosX;
  _Viewport[1]= h-(_PosY+_SizY);
  _Viewport[2]= _SizX;
  _Viewport[3]= _SizY;

  _SelectorWindow -> ScreenSize(_SizX,_SizY);
} //eof NWM_Selector::Reshape()


char NWM_Selector::Key(int x, int y) {
  return(NWM_NOTHING);
} //eof NWM_Selector::Key()


char NWM_Selector::Mouse(int button, int state, int x, int y) {
  _SelectorWindow -> Handle(NTK_EVENT_MOUSE,button,state,x,y);
  _Update= 2;

  return(NWM_NOTHING);
} //eof NWM_Selector::Mouse()


char NWM_Selector::Motion(int x, int y) {
  return(NWM_NOTHING);
} //eof NWM_Selector::Motion()


char NWM_Selector::Passive(int x, int y) {
  return(NWM_NOTHING);
} //eof NWM_Selector::Passive()


void NWM_Selector::Active(bool A) {
} //eof NWM_Selector::Active()
