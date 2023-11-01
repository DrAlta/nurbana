#include "NWM_Panel_Materials.h"

//XPMs
#include "../NTK/XPMs/dash.xpm"


// +-----------------+
// | Local Callbacks |
// +-----------------+

// Materials Menu
void MatSliderCallback(char E) {
  // Update color Panel
  NWM_Panel_Materials::Access -> _ColorPanel -> Color(int(NWM_Panel_Materials::Access -> _MatRedSlider -> Value()),int(NWM_Panel_Materials::Access -> _MatGrnSlider -> Value()),int(NWM_Panel_Materials::Access -> _MatBluSlider -> Value()));
  // Update Slider Color
  NWM_Panel_Materials::Access -> _MatRedSlider -> SliderColor(NWM_Panel_Materials::Access -> _MatRedSlider -> Value(),0x00,0x00);
  NWM_Panel_Materials::Access -> _MatGrnSlider -> SliderColor(0x00,NWM_Panel_Materials::Access -> _MatGrnSlider -> Value(),0x00);
  NWM_Panel_Materials::Access -> _MatBluSlider -> SliderColor(0x00,0x00,NWM_Panel_Materials::Access -> _MatBluSlider -> Value());

  // Update ModelView if _RealTime is on
  if (NWM_Panel_Materials::Access -> _Realtime -> Status()) {
    NWM_Panel_Materials::Access -> _NWM -> UpdateType(NWM_WT_ModelView);
    if (NWM_Panel_Materials::Access -> _Diffuse -> Status()) {
      NWM_Panel_Materials::Access -> _MaterialList -> Diff(NWM_Panel_Materials::Access -> _MaterialList -> ActiveMaterial())[0]= NWM_Panel_Materials::Access -> _MatRedSlider -> Value();
      NWM_Panel_Materials::Access -> _MaterialList -> Diff(NWM_Panel_Materials::Access -> _MaterialList -> ActiveMaterial())[1]= NWM_Panel_Materials::Access -> _MatGrnSlider -> Value();
      NWM_Panel_Materials::Access -> _MaterialList -> Diff(NWM_Panel_Materials::Access -> _MaterialList -> ActiveMaterial())[2]= NWM_Panel_Materials::Access -> _MatBluSlider -> Value();
    } else if (NWM_Panel_Materials::Access -> _Specular -> Status()) {
      NWM_Panel_Materials::Access -> _MaterialList -> Spec(NWM_Panel_Materials::Access -> _MaterialList -> ActiveMaterial())[0]= NWM_Panel_Materials::Access -> _MatRedSlider -> Value();
      NWM_Panel_Materials::Access -> _MaterialList -> Spec(NWM_Panel_Materials::Access -> _MaterialList -> ActiveMaterial())[1]= NWM_Panel_Materials::Access -> _MatGrnSlider -> Value();
      NWM_Panel_Materials::Access -> _MaterialList -> Spec(NWM_Panel_Materials::Access -> _MaterialList -> ActiveMaterial())[2]= NWM_Panel_Materials::Access -> _MatBluSlider -> Value();
    } //fi
  } //fi

  // Update MaterialList
  if ((E == NTK_EVENT_MOUSE_UP) || (E == NTK_EVENT_UPDATE_COMPLETE)) {
    // Update ModelView(s)
    NWM_Panel_Materials::Access -> _NWM -> UpdateType(NWM_WT_ModelView);

    if (NWM_Panel_Materials::Access -> _Diffuse -> Status()) {
      NWM_Panel_Materials::Access -> _MaterialList -> Diff(NWM_Panel_Materials::Access -> _ObjectList -> GetMaterialID())[0]= NWM_Panel_Materials::Access -> _MatRedSlider -> Value();
      NWM_Panel_Materials::Access -> _MaterialList -> Diff(NWM_Panel_Materials::Access -> _ObjectList -> GetMaterialID())[1]= NWM_Panel_Materials::Access -> _MatGrnSlider -> Value();
      NWM_Panel_Materials::Access -> _MaterialList -> Diff(NWM_Panel_Materials::Access -> _ObjectList -> GetMaterialID())[2]= NWM_Panel_Materials::Access -> _MatBluSlider -> Value();
    } else if (NWM_Panel_Materials::Access -> _Specular -> Status()) {
      NWM_Panel_Materials::Access -> _MaterialList -> Spec(NWM_Panel_Materials::Access -> _ObjectList -> GetMaterialID())[0]= NWM_Panel_Materials::Access -> _MatRedSlider -> Value();
      NWM_Panel_Materials::Access -> _MaterialList -> Spec(NWM_Panel_Materials::Access -> _ObjectList -> GetMaterialID())[1]= NWM_Panel_Materials::Access -> _MatGrnSlider -> Value();
      NWM_Panel_Materials::Access -> _MaterialList -> Spec(NWM_Panel_Materials::Access -> _ObjectList -> GetMaterialID())[2]= NWM_Panel_Materials::Access -> _MatBluSlider -> Value();
    } //fi

    NRT_RayTracer	RT(40,40);
    RT.Reset();
    RT.PacketSize(9999);
//    RT.Camera(1, -2, 1);
    RT.Camera(0, -2, 1.4);

    RT.CameraFocusPt(0, 0, 0);
    RT.CameraFOV(52);
    RT.ViewMatrix();
    RT.BGColor(0x00, 0x00, 0x00);
    RT.MaxDepth(5);
    RT.Rays(1);
    RT.SetAmbient(0.0, 0.0, 0.0);

    RT.Lights -> CreateLight(1,NRT_LIGHT_SPHERE,0.5,-2,0.8);
    RT.Lights -> Radius(1,0.5);
    RT.Lights -> Intensity(1,1,1,1);

    RT.Objects -> CreateObject(1,NRT_OBJECT_SPHERE);
    RT.Objects -> Position(1,0,0,0);
    RT.Objects -> Radius(1,1);
    RT.Objects -> Diffuse(1,NWM_Panel_Materials::Access -> _MaterialList -> Diff(NWM_Panel_Materials::Access -> _MaterialList -> ActiveMaterial())[0]/255.0,NWM_Panel_Materials::Access -> _MaterialList -> Diff(NWM_Panel_Materials::Access -> _MaterialList -> ActiveMaterial())[1]/255.0,NWM_Panel_Materials::Access -> _MaterialList -> Diff(NWM_Panel_Materials::Access -> _MaterialList -> ActiveMaterial())[2]/255.0);
    RT.Objects -> Specular(1,NWM_Panel_Materials::Access -> _MaterialList -> Spec(NWM_Panel_Materials::Access -> _MaterialList -> ActiveMaterial())[0]/255.0,NWM_Panel_Materials::Access -> _MaterialList -> Spec(NWM_Panel_Materials::Access -> _MaterialList -> ActiveMaterial())[1]/255.0,NWM_Panel_Materials::Access -> _MaterialList -> Spec(NWM_Panel_Materials::Access -> _MaterialList -> ActiveMaterial())[2]/255.0);
    RT.Objects -> Reflective(1,1);

    RT.RenderScene();

    NWM_Panel_Materials::Access -> _RenderPanel -> Glyphic(NWM_Panel_Materials::Access -> _Window -> GlyphicRAW(RT.Image(),40,40,NTK_ON));
  } //fi
} //eof MatSliderCallback()

void MatSliderUpdateCallback(char E) {
  if (NWM_Panel_Materials::Access -> _Diffuse -> Status()) {
    NWM_Panel_Materials::Access -> _MatRedSlider -> Value(NWM_Panel_Materials::Access -> _MaterialList -> Diff(NWM_Panel_Materials::Access -> _MaterialList -> ActiveMaterial())[0]);
    NWM_Panel_Materials::Access -> _MatGrnSlider -> Value(NWM_Panel_Materials::Access -> _MaterialList -> Diff(NWM_Panel_Materials::Access -> _MaterialList -> ActiveMaterial())[1]);
    NWM_Panel_Materials::Access -> _MatBluSlider -> Value(NWM_Panel_Materials::Access -> _MaterialList -> Diff(NWM_Panel_Materials::Access -> _MaterialList -> ActiveMaterial())[2]);
  } else {
    NWM_Panel_Materials::Access -> _MatRedSlider -> Value(NWM_Panel_Materials::Access -> _MaterialList -> Spec(NWM_Panel_Materials::Access -> _MaterialList -> ActiveMaterial())[0]);
    NWM_Panel_Materials::Access -> _MatGrnSlider -> Value(NWM_Panel_Materials::Access -> _MaterialList -> Spec(NWM_Panel_Materials::Access -> _MaterialList -> ActiveMaterial())[1]);
    NWM_Panel_Materials::Access -> _MatBluSlider -> Value(NWM_Panel_Materials::Access -> _MaterialList -> Spec(NWM_Panel_Materials::Access -> _MaterialList -> ActiveMaterial())[2]);
  } //fi

  NWM_Panel_Materials::Access -> _MatRedSlider -> SliderColor(NWM_Panel_Materials::Access -> _MatRedSlider -> Value(),0x00,0x00);
  NWM_Panel_Materials::Access -> _MatGrnSlider -> SliderColor(0x00,NWM_Panel_Materials::Access -> _MatGrnSlider -> Value(),0x00);
  NWM_Panel_Materials::Access -> _MatBluSlider -> SliderColor(0x00,0x00,NWM_Panel_Materials::Access -> _MatBluSlider -> Value());

  NWM_Panel_Materials::Access -> _ColorPanel -> Color(int(NWM_Panel_Materials::Access -> _MatRedSlider -> Value()),int(NWM_Panel_Materials::Access -> _MatGrnSlider -> Value()),int(NWM_Panel_Materials::Access -> _MatBluSlider -> Value()));
} //eof MatSliderUpdateCallback()

void MatOpenListCallback(char E) {
  NWM_Panel_Materials::Access -> _NWM -> CreateMenu(NWM_MENU_MATERIALS);
} //eof MatOpenListCallback()

void MaterialNameCallback(char E) {
  char	Text[256];
  NWM_Panel_Materials::Access -> _MaterialName -> GetText(Text);
  NWM_Panel_Materials::Access -> _MaterialList -> SetName(NWM_Panel_Materials::Access -> _MaterialList -> ActiveMaterial(),Text);
} //eof MaterialNameCallback()

void MatDeleteCallback(char E) {
  char	Text[256];

  NWM_Panel_Materials::Access -> _MaterialList -> Remove(NWM_Panel_Materials::Access -> _MaterialList -> ActiveMaterial());
  NWM_Panel_Materials::Access -> _MaterialName -> Text("");

  // Update Material Name
  NWM_Panel_Materials::Access -> _MaterialList -> GetName(NWM_Panel_Materials::Access -> _MaterialList -> ActiveMaterial(),Text);
  NWM_Panel_Materials::Access -> _MaterialName -> Text(Text);

  NWM_Panel_Materials::Access -> _NWM -> UpdateType(NWM_WT_ModelView);

  NWM_Panel_Materials::Access -> _MaterialName -> Status(0);
} //eof MatOpenListCallback()


NWM_Panel_Materials* NWM_Panel_Materials::Access= 0;
NWM_Panel_Materials::NWM_Panel_Materials(int H1, int H2, int H3, int H4, NWM_List *obj, NWM *NWMptr, Object_Manager *ObjectList, MaterialList *MatList) : NWM_Base(obj,NWM_WT_Panel_Materials) {
  _self= this;
  Access= _self;

  _NWM= NWMptr;
  _ObjectList= ObjectList;
  _MaterialList= MatList;

  // Assign Handles
  _H1= H1;
  _H2= H2;
  _H3= H3;
  _H4= H4;

  _Window= new NTK_Window(_NWM -> Path());

  //
  // Configure Widgets
  //


  //
  // Materials Menu
  //
//  if (!_MaterialList -> ActiveMaterial()) _MatContainer -> Visibility(NTK_CONTAINER_INVISIBLE);

  _DiffSpecGroup= new NTK_Group();

  _RenderPanel= new NTK_Label(_Window,0,0,80,80);
  _RenderPanel -> Bevel(NTK_ON);
  _RenderPanel -> Type(NTK_LABEL_GLYPHIC);
  _RenderPanel -> StretchGlyphic(NTK_ON);
  _RenderPanel -> Color(0x00,0x00,0x00);
  _RenderPanel -> Glyphic(_Window -> GlyphicXPM(dash_xpm,NTK_OFF));

  _ColorPanel= new NTK_Label(_Window,80,0,40,20);
  _ColorPanel -> Bevel(NTK_ON);
  _ColorPanel -> Color(0x80,0x80,0x80);

  _Diffuse= new NTK_Button(_Window,80,20,40,20);
  _Diffuse -> Status(NTK_ON);
  _Diffuse -> Type(NTK_BUTTON_TEXT);
  _Diffuse -> Color(0xA0,0xA0,0xA0);
  _Diffuse -> Text("Diff");
  _Diffuse -> Group(_DiffSpecGroup);
  _Diffuse -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _Diffuse -> Callback((NTK_Callback*)MatSliderUpdateCallback);

  _Specular= new NTK_Button(_Window,80,40,40,20);
  _Specular -> Type(NTK_BUTTON_TEXT);
  _Specular -> Color(0xA0,0xA0,0xA0);
  _Specular -> Text("Spec");
  _Specular -> Group(_DiffSpecGroup);
  _Specular -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _Specular -> Callback((NTK_Callback*)MatSliderUpdateCallback);

  _Realtime= new NTK_Button(_Window,80,60,40,20);
  _Realtime -> Type(NTK_BUTTON_TEXT);
  _Realtime -> Sticky(NTK_ON);
  _Realtime -> Color(0xA0,0xA0,0xA0);
  _Realtime -> Text("Real");
  _Realtime -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _Realtime -> Callback((NTK_Callback*)MatSliderUpdateCallback);

  _MatRedSlider= new NTK_Slider_Horz(_Window,120,20,200,20);
  _MatRedSlider -> Type(NTK_HORZ_SLIDER);
  _MatRedSlider -> Min(0);
  _MatRedSlider -> Max(255);
  _MatRedSlider -> Precision(0);
  _MatRedSlider -> Value(192);
  _MatRedSlider -> Color(0xA0,0xA0,0xA0);
  _MatRedSlider -> SliderColor(0x80,0x00,0x00);
  _MatRedSlider -> CallbackTrigger(NTK_CT_MOTION | NTK_CT_MOUSE_UP);
  _MatRedSlider -> Callback((NTK_Callback*)MatSliderCallback);

  _MatGrnSlider= new NTK_Slider_Horz(_Window,120,40,200,20);
  _MatGrnSlider -> Type(NTK_HORZ_SLIDER);
  _MatGrnSlider -> Min(0);
  _MatGrnSlider -> Max(255);
  _MatGrnSlider -> Precision(0);
  _MatGrnSlider -> Value(192);
  _MatGrnSlider -> Color(0xA0,0xA0,0xA0);
  _MatGrnSlider -> SliderColor(0x00,0x80,0x00);
  _MatGrnSlider -> CallbackTrigger(NTK_CT_MOTION | NTK_CT_MOUSE_UP);
  _MatGrnSlider -> Callback((NTK_Callback*)MatSliderCallback);

  _MatBluSlider= new NTK_Slider_Horz(_Window,120,60,200,20);
  _MatBluSlider -> Type(NTK_HORZ_SLIDER);
  _MatBluSlider -> Min(0);
  _MatBluSlider -> Max(255);
  _MatBluSlider -> Precision(0);
  _MatBluSlider -> Value(192);
  _MatBluSlider -> Color(0xA0,0xA0,0xA0);
  _MatBluSlider -> SliderColor(0x00,0x00,0x80);
  _MatBluSlider -> CallbackTrigger(NTK_CT_MOTION | NTK_CT_MOUSE_UP);
  _MatBluSlider -> Callback((NTK_Callback*)MatSliderCallback);

  _MatOpenList= new NTK_Button(_Window,120,0,20,20);
  _MatOpenList -> Layer(1);
  _MatOpenList -> Type(NTK_BUTTON_GLYPHIC);
  _MatOpenList -> Color(0xA0,0xA0,0xA0);
  _MatOpenList -> Glyphic(_Window -> GlyphicXPM(dash_xpm,NTK_OFF));
  _MatOpenList -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _MatOpenList -> Callback((NTK_Callback*)MatOpenListCallback);

  _MaterialName= new NTK_TextInput(_Window,140,0,160,20);
  _MaterialName -> Layer(1);
  _MaterialName -> Color(0xA0,0xA0,0xA0);
  _MaterialName -> StaticText("MA:");
  _MaterialName -> Text("None");
  _MaterialName -> CallbackTrigger(NTK_CT_MOUSE_DOWN | NTK_CT_KEY);
  _MaterialName -> Callback((NTK_Callback*)MaterialNameCallback);

  _MatDelete= new NTK_Button(_Window,300,0,20,20);
  _MatDelete -> Layer(1);
  _MatDelete -> Type(NTK_BUTTON_TEXT);
  _MatDelete -> Color(0xA0,0xA0,0xA0);
  _MatDelete -> Text("X");
  _MatDelete -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _MatDelete -> Callback((NTK_Callback*)MatDeleteCallback);

  MatSliderCallback(NTK_EVENT_UPDATE_COMPLETE);
} //eof NWM_Panel_Materials::NWM_Panel_Materials()


void NWM_Panel_Materials::Update() {
  glEnable(GL_SCISSOR_TEST);
    glViewport(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
    glScissor(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
      _Window -> Draw();
  glDisable(GL_SCISSOR_TEST);
} //eof NWM_Panel_Materials::Update()


void NWM_Panel_Materials::Refresh() {
  // Update all Dynamic Items
  char	Text[256];

//  if (_ActivePanel == NWM_Panel_Material) {
//    if (_MaterialList -> ActiveMaterial()) {
//      _MatContainer -> Visibility(NTK_CONTAINER_VISIBLE);
//    } else {
//      _MatContainer -> Visibility(NTK_CONTAINER_INVISIBLE);
//    } //fi
//  } //fi

  // Material Menu
  if (_Diffuse -> Status()) {
    _MatRedSlider -> Value(float(_MaterialList -> Diff(_ObjectList -> GetMaterialID())[0]));
    _MatGrnSlider -> Value(float(_MaterialList -> Diff(_ObjectList -> GetMaterialID())[1]));
    _MatBluSlider -> Value(float(_MaterialList -> Diff(_ObjectList -> GetMaterialID())[2]));

    NWM_Panel_Materials::Access -> _MatRedSlider -> SliderColor(NWM_Panel_Materials::Access -> _MatRedSlider -> Value(),0x00,0x00);
    NWM_Panel_Materials::Access -> _MatGrnSlider -> SliderColor(0x00,NWM_Panel_Materials::Access -> _MatGrnSlider -> Value(),0x00);
    NWM_Panel_Materials::Access -> _MatBluSlider -> SliderColor(0x00,0x00,NWM_Panel_Materials::Access -> _MatBluSlider -> Value());
  } else {
    _MatRedSlider -> Value(float(_MaterialList -> Spec(_ObjectList -> GetMaterialID())[0]));
    _MatGrnSlider -> Value(float(_MaterialList -> Spec(_ObjectList -> GetMaterialID())[1]));
    _MatBluSlider -> Value(float(_MaterialList -> Spec(_ObjectList -> GetMaterialID())[2]));

    NWM_Panel_Materials::Access -> _MatRedSlider -> SliderColor(NWM_Panel_Materials::Access -> _MatRedSlider -> Value(),0x00,0x00);
    NWM_Panel_Materials::Access -> _MatGrnSlider -> SliderColor(0x00,NWM_Panel_Materials::Access -> _MatGrnSlider -> Value(),0x00);
    NWM_Panel_Materials::Access -> _MatBluSlider -> SliderColor(0x00,0x00,NWM_Panel_Materials::Access -> _MatBluSlider -> Value());
  } //fi
  _ColorPanel -> Color(_MatRedSlider -> Value(), _MatGrnSlider -> Value(), _MatBluSlider -> Value());

  // Material Name
  _MaterialList -> GetName(_ObjectList -> GetMaterialID(),Text);
  _MaterialName -> Text(Text);

  _Window -> SetClear(2);
  _Window -> UpdateAll();
} //eof NWM_Panel_Materials::Refresh()


void NWM_Panel_Materials::Reshape(int w, int h) {
  // Construct Main Panel Viewport
  _Viewport[0]= _PosX;
  _Viewport[1]= h-(_PosY+_SizY);
  _Viewport[2]= _SizX;
  _Viewport[3]= _SizY;
  _Viewport[4]= w;
  _Viewport[5]= h;

  _Window -> SetClear(2);  
  _Window -> ScreenSize(_SizX,_SizY);
} //eof NWM_Panel_Materials::Reshape()


char NWM_Panel_Materials::Key(int x, int y) {
  // For callbacks, assign _pself _lself
  Access= _self;
  _Update= 2;

  _Window -> Handle(NTK_EVENT_KEY,x,y,0,0);
  return(NWM_NOTHING);
} //eof NWM_Panel_Materials::Key()


char NWM_Panel_Materials::Mouse(int button, int state, int x, int y) {
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
} //eof NWM_Panel_Materials::Mouse()


char NWM_Panel_Materials::Motion(int x, int y) {
  // For callbacks, assign _pself _lself
  Access= _self;

  _Window -> Handle(NTK_EVENT_MOTION,x,y,0,0);
  _Update= 1;
  return(NWM_NOTHING);
} //eof NWM_Panel_Materials::Motion()


char NWM_Panel_Materials::Passive(int x, int y) {
  // For callbacks, assign _pself _lself
  Access= _self;

  return(NWM_NOTHING);
} //eof NWM_Panel_Materials::Passive()


void NWM_Panel_Materials::Active(bool A) {
} //eof NWM_Panel_Materials::Active()


void NWM_Panel_Materials::Misc(int V) {
} //eof Panel_Materials::Misc()
