#include "NWM_Panel_Primitives.h"

#include "xpms/curve.xpm"
#include "xpms/ellipse.xpm"
#include "xpms/surface.xpm"
#include "xpms/tube.xpm"
#include "xpms/sphere.xpm"
#include "xpms/torus.xpm"

//
// Local Callbacks
//

void PrimitiveCallback(char E) {
  NWM_Panel_Primitives::Access -> _ObjectList -> SetObjActAll(0);
  if (NWM_Panel_Primitives::Access -> _NURBSCurve -> Status()) {
      NWM_Panel_Primitives::Access -> _ObjectList -> Primitive(OBJECT_PRIMITIVE_CURVE);
  } else if (NWM_Panel_Primitives::Access -> _NURBSSurface -> Status()) {
      NWM_Panel_Primitives::Access -> _ObjectList -> Primitive(OBJECT_PRIMITIVE_SURFACE);
  } else if (NWM_Panel_Primitives::Access -> _NURBSEllipse -> Status()) {
      NWM_Panel_Primitives::Access -> _ObjectList -> Primitive(OBJECT_PRIMITIVE_CIRCLE);
  } else if (NWM_Panel_Primitives::Access -> _NURBSTube -> Status()) {
      NWM_Panel_Primitives::Access -> _ObjectList -> Primitive(OBJECT_PRIMITIVE_TUBE);
  } else if (NWM_Panel_Primitives::Access -> _NURBSTorus -> Status()) {
      NWM_Panel_Primitives::Access -> _ObjectList -> Primitive(OBJECT_PRIMITIVE_TORUS);
  } else if (NWM_Panel_Primitives::Access -> _NURBSSphere -> Status()) {
      NWM_Panel_Primitives::Access -> _ObjectList -> Primitive(OBJECT_PRIMITIVE_SPHERE);
  } //fi

  NWM_Panel_Primitives::Access -> _NWM -> UpdateType(NWM_WT_ModelView);
} //eof PrimitiveCallback()


NWM_Panel_Primitives* NWM_Panel_Primitives::Access= 0;
NWM_Panel_Primitives::NWM_Panel_Primitives(int H1, int H2, int H3, int H4, NWM_List *obj, NWM *NWMptr, Object_Manager *ObjectList) : NWM_Base(obj,NWM_WT_Panel_Primitives) {
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

  _NURBSCurve= new NTK_Button(_Window,0,0,40,40);
  _NURBSCurve -> Type(NTK_BUTTON_GLYPHIC);
  _NURBSCurve -> Rounded(NTK_ON);
  _NURBSCurve -> Color(0x90,0xA0,0xB0);
  _NURBSCurve -> Glyphic(_Window -> GlyphicXPM(curve_xpm,NTK_OFF));
  _NURBSCurve -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _NURBSCurve -> Callback((NTK_Callback*)PrimitiveCallback);

  _NURBSEllipse= new NTK_Button(_Window,0,40,40,40);
  _NURBSEllipse -> Type(NTK_BUTTON_GLYPHIC);
  _NURBSEllipse -> Rounded(NTK_ON);
  _NURBSEllipse -> Color(0x90,0xA0,0xB0);
  _NURBSEllipse -> Glyphic(_Window -> GlyphicXPM(ellipse_xpm,NTK_OFF));
  _NURBSEllipse -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _NURBSEllipse -> Callback((NTK_Callback*)PrimitiveCallback);

  _NURBSSurface= new NTK_Button(_Window,40,0,40,40);
  _NURBSSurface -> Type(NTK_BUTTON_GLYPHIC);
  _NURBSSurface -> Rounded(NTK_ON);
  _NURBSSurface -> Color(0x90,0xA0,0xB0);
  _NURBSSurface -> Glyphic(_Window -> GlyphicXPM(surface_xpm,NTK_OFF));
  _NURBSSurface -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _NURBSSurface -> Callback((NTK_Callback*)PrimitiveCallback);

  _NURBSTube= new NTK_Button(_Window,40,40,40,40);
  _NURBSTube -> Type(NTK_BUTTON_GLYPHIC);
  _NURBSTube -> Rounded(NTK_ON);
  _NURBSTube -> Color(0x90,0xA0,0xB0);
  _NURBSTube -> Glyphic(_Window -> GlyphicXPM(tube_xpm,NTK_OFF));
  _NURBSTube -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _NURBSTube -> Callback((NTK_Callback*)PrimitiveCallback);

  _NURBSTorus= new NTK_Button(_Window,80,0,40,40);
  _NURBSTorus -> Type(NTK_BUTTON_GLYPHIC);
  _NURBSTorus -> Rounded(NTK_ON);
  _NURBSTorus -> Color(0x90,0xA0,0xB0);
  _NURBSTorus -> Glyphic(_Window -> GlyphicXPM(torus_xpm,NTK_OFF));
  _NURBSTorus -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _NURBSTorus -> Callback((NTK_Callback*)PrimitiveCallback);

  _NURBSSphere= new NTK_Button(_Window,80,40,40,40);
  _NURBSSphere -> Type(NTK_BUTTON_GLYPHIC);
  _NURBSSphere -> Rounded(NTK_ON);
  _NURBSSphere -> Color(0x90,0xA0,0xB0);
  _NURBSSphere -> Glyphic(_Window -> GlyphicXPM(sphere_xpm,NTK_OFF));
  _NURBSSphere -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _NURBSSphere -> Callback((NTK_Callback*)PrimitiveCallback);
} //eof NWM_Panel_Primitives::NWM_Panel_Primitives()


void NWM_Panel_Primitives::Update() {
  glEnable(GL_SCISSOR_TEST);
    glViewport(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
    glScissor(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
      _Window -> Draw();
  glDisable(GL_SCISSOR_TEST);
} //eof NWM_Panel_Primitives::Update()


void NWM_Panel_Primitives::Refresh() {
  _Window -> SetClear(2);
  _Window -> UpdateAll();
} //eof NWM_Panel_Primitives::Refresh()


void NWM_Panel_Primitives::Reshape(int w, int h) {
  // Construct Main Panel Viewport
  _Viewport[0]= _PosX;
  _Viewport[1]= h-(_PosY+_SizY);
  _Viewport[2]= _SizX;
  _Viewport[3]= _SizY;
  _Viewport[4]= w;
  _Viewport[5]= h;


  _Window -> SetClear(2);  
  _Window -> ScreenSize(_SizX,_SizY);
} //eof NWM_Panel_Primitives::Reshape()


char NWM_Panel_Primitives::Key(int x, int y) {
  // For callbacks, assign _pself _lself
  Access= _self;
  _Update= 2;

  _Window -> Handle(NTK_EVENT_KEY,x,y,0,0);
  return(NWM_NOTHING);
} //eof NWM_Panel_Primitives::Key()


char NWM_Panel_Primitives::Mouse(int button, int state, int x, int y) {
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
} //eof NWM_Panel_Primitives::Mouse()


char NWM_Panel_Primitives::Motion(int x, int y) {
  // For callbacks, assign _pself _lself
  Access= _self;

  _Window -> Handle(NTK_EVENT_MOTION,x,y,0,0);
  _Update= 1;
  return(NWM_NOTHING);
} //eof NWM_Panel_Primitives::Motion()


char NWM_Panel_Primitives::Passive(int x, int y) {
  // For callbacks, assign _pself _lself
  Access= _self;

  return(NWM_NOTHING);
} //eof NWM_Panel_Primitives::Passive()


void NWM_Panel_Primitives::Active(bool A) {
} //eof NWM_Panel_Primitives::Active()


void NWM_Panel_Primitives::Misc(int V) {
  _Window -> SetClear(2);
  _Window -> UpdateAll();
} //eof Panel_Primitives::Misc()
