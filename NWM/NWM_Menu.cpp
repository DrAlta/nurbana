#include "NWM_Menu.h"
#include "../NTK/XPMs/dash.xpm"


// +-----------+
// | Callbacks |
// +-----------+

void ClearSceneMenuCallback(int V) {
  NWM_Menu::Access -> _ObjectList -> Purge();
  NWM_Menu::Access -> SelfDestruct(1);
} //eof ClearSceneMenuCallback()

void ExtrudeMenuCallback(int V) {
  NWM_Menu::Access -> _ObjectList -> Extrude();
  NWM_Menu::Access -> _ObjectList -> SetMode(OBJECT_MODE_GRAB, 1);
  NWM_Menu::Access -> _ObjectList -> Init(1);
  NWM_Menu::Access -> SelfDestruct(1);
} //eof ExtrudeMenuCallback()

void QuitMenuCallback(int V) {
  printf("Nurbana quit\n");
  exit(0);
} //eof QuitMenuCallback()

void DeleteMenuCallback(int V) {
  if (NWM_Menu::Access -> _ObjectList -> GetMode(OBJECT_MODE_EDIT)) {
    NWM_Menu::Access -> _ObjectList -> DeExtrude();
  } else {
    NWM_Menu::Access -> _ObjectList -> Delete();
    NWM_Menu::Access -> _ObjectList -> SetMode(OBJECT_MODE_EDIT, 0);
  } //fi
  NWM_Menu::Access -> SelfDestruct(1);
} //eof DeleteMenuCallback()

void MaterialsMenuCallback(int V) {
  int	i;
  char	Label[256];
  
  //Assign Object Material or Create New Material and then Assign it
  if (V == -1) {
    //Adding new material...
    i= NWM_Menu::Access -> _MaterialsMenu -> GetFreeID();
    sprintf(Label,"Material.%d",i);
    NWM_Menu::Access -> _MaterialsMenu -> Push(Label,i);
    NWM_Menu::Access -> _MaterialList -> Create(i,Label);

    NWM_Menu::Access -> _MaterialList -> Diff(i)[0]= 192;
    NWM_Menu::Access -> _MaterialList -> Diff(i)[1]= 192;
    NWM_Menu::Access -> _MaterialList -> Diff(i)[2]= 192;

    NWM_Menu::Access -> _MaterialList -> Spec(i)[0]= 192;
    NWM_Menu::Access -> _MaterialList -> Spec(i)[1]= 192;
    NWM_Menu::Access -> _MaterialList -> Spec(i)[2]= 192;

    NWM_Menu::Access -> _MaterialList -> SetName(i, Label);
    V= i;
  } //fi

  NWM_Menu::Access -> _MaterialList -> ActiveMaterial(V);
  NWM_Menu::Access -> _ObjectList -> SetMaterialID(V);
  NWM_Menu::Access -> SelfDestruct(1);
//  NWM_Menu::Access() -> _NWM -> RefreshAll();
} //eof MaterialsMenuCallback()

void NumericsCallback(char E) {
  Point3d	P;

  if (NWM_Menu::Access -> _ObjectList -> GetMode(OBJECT_MODE_EDIT)) {
    if (NWM_Menu::Access -> _ObjectList -> CPActCount() == 1) {
      P.x= NWM_Menu::Access -> _NLocX -> Value();
      P.y= NWM_Menu::Access -> _NLocY -> Value();
      P.z= NWM_Menu::Access -> _NLocZ -> Value();
      NWM_Menu::Access -> _ObjectList -> SetCP(NWM_Menu::Access -> _ObjectList -> GetCPActID(), P);
      NWM_Menu::Access -> _ObjectList -> SetH(NWM_Menu::Access -> _ObjectList -> GetCPActID(), NWM_Menu::Access -> _NLocW -> Value());
    } else {
      P.x= NWM_Menu::Access -> _NLocX -> Value();
      P.y= NWM_Menu::Access -> _NLocY -> Value();
      P.z= NWM_Menu::Access -> _NLocZ -> Value();
      NWM_Menu::Access -> _ObjectList -> CPDelta(P);
    } //fi
  } else {
    P.x= NWM_Menu::Access -> _NLocX -> Value();
    P.y= NWM_Menu::Access -> _NLocY -> Value();
    P.z= NWM_Menu::Access -> _NLocZ -> Value();
    NWM_Menu::Access -> _ObjectList -> SetCOR(P);
    P.x= NWM_Menu::Access -> _NRotX -> Value();
    P.y= NWM_Menu::Access -> _NRotY -> Value();
    P.z= NWM_Menu::Access -> _NRotZ -> Value();
    NWM_Menu::Access -> _ObjectList -> SetROT(P);
    P.x= NWM_Menu::Access -> _NSizX -> Value();
    P.y= NWM_Menu::Access -> _NSizY -> Value();
    P.z= NWM_Menu::Access -> _NSizZ -> Value();
    NWM_Menu::Access -> _ObjectList -> SetSIZ(P);
  } //fi

  NWM_Menu::Access -> SelfDestruct(1);
} //eof NumericsCallback()

void LoftMenuCallback(char E) {
  NWM_Menu::Access -> _ObjectList -> Loft(NWM_Menu::Access -> _Degree -> Value()+1);
  NWM_Menu::Access -> _NWM -> UpdateType(NWM_WT_ModelView);

  NWM_Menu::Access -> SelfDestruct(1);
} //eof LoftCallback()

void BlendMenuCallback(char E) {
  NWM_Menu::Access -> _ObjectList -> Blend(NWM_Menu::Access -> _TwistButton -> Status());
  NWM_Menu::Access -> _NWM -> UpdateType(NWM_WT_ModelView);

  NWM_Menu::Access -> SelfDestruct(1);
} //eof BlendCallback()

void RebuildMenuCallback(char E) {
  int	UK;

  if (NWM_Menu::Access -> _Centripetal -> Status()) {
    UK= 1;
  } else if (NWM_Menu::Access -> _Chord -> Status()) {
    UK= 0;
  } else if (NWM_Menu::Access -> _Universal -> Status()) {
    UK= 2;
  } //fi

  if (NWM_Menu::Access -> _ObjectList -> GetLength(0) == 1) {
    NWM_Menu::Access -> _ObjectList -> Rebuild(1, NWM_Menu::Access -> _CPVSlider -> Value(), NWM_Menu::Access -> _ObjectList -> GetOrder(0), NWM_Menu::Access -> _DegreeVSlider -> Value()+1, UK);
  } else if (NWM_Menu::Access -> _ObjectList -> GetLength(1) == 1) {
    NWM_Menu::Access -> _ObjectList -> Rebuild(NWM_Menu::Access -> _CPUSlider -> Value(), 1, NWM_Menu::Access -> _DegreeUSlider -> Value()+1, NWM_Menu::Access -> _ObjectList -> GetOrder(1), UK);
  } else {
    NWM_Menu::Access -> _ObjectList -> Rebuild(NWM_Menu::Access -> _CPUSlider -> Value(), NWM_Menu::Access -> _CPVSlider -> Value(), NWM_Menu::Access -> _DegreeUSlider -> Value()+1, NWM_Menu::Access -> _DegreeVSlider -> Value()+1, UK);
  } //fi

  NWM_Menu::Access -> _NWM -> UpdateType(NWM_WT_ModelView);
  NWM_Menu::Access -> SelfDestruct(1);
} //eof RebuildMenuCallback()

void RenderScene() {
  if (!NWM_Menu::Access -> _Complete) {
    NWM_Menu::Access -> _Complete= NWM_Menu::Access -> _RT -> RenderScene();

    NWM_Menu::Access -> _RenderArea -> Glyphic(NWM_Menu::Access -> _MenuWindow -> GlyphicRAW(NWM_Menu::Access -> _RT -> Image(),NWM_Menu::Access -> _RT -> Size(0),NWM_Menu::Access -> _RT -> Size(1),NTK_OFF));
    NWM_Menu::Access -> _RenderArea -> Update(2);

    NWM_Menu::Access -> _NWM -> UpdateType(NWM_WT_Menu);
    NWM_Menu::Access -> _NWM -> Update();
  } else {
    NWM_Menu::Access -> _stop= clock();
    printf("time: %f\n",(float)(NWM_Menu::Access -> _stop-NWM_Menu::Access -> _start)/CLOCKS_PER_SEC);
    NIdle::Callback(NULL);
//    glutIdleFunc(NULL);
  } //fi
} //eof RenderScene()

void RenderMenuCallback(char E) {
//  glutIdleFunc(NULL);
  NIdle::Callback(NULL);
  NWM_Menu::Access -> SelfDestruct(1);
} //eof RenderMenuCallback()

// +---------------+
// | EOF Callbacks |
// +---------------+


NWM_Menu* NWM_Menu::Access= 0;
NWM_Menu::NWM_Menu(int PosX, int PosY, int W, int H, NWM_List *obj, char MenuType, Object_Manager *ObjectList, NWM *NWMPtr, MaterialList *MatList, NRT_RayTracer *RT) : NWM_Base(obj,NWM_WT_Menu) {
  int	NPosX,NPosY;

  _self= this;
  Access= _self;

  _NWM= NWMPtr;
  _ObjectList= ObjectList;
  _MenuType= MenuType;
  _MaterialList= MatList;
  _RT= RT;

  _AutoDestroy= 1;

  _PosX= 0;
  _PosY= 0;
  _SizX= W;
  _SizY= H;

  // Construct Menu Panel Viewport
  _Viewport[0]= 0;
  _Viewport[1]= 0;
  _Viewport[2]= W;
  _Viewport[3]= H;

  // Place the menu on top
  _Layer= 1;


  // Calculate NPosX,NPosY so Menu appears in the middle
  // of Mouse cursor (PosX,PosY);

  NPosX= PosX-25;
  NPosY= PosY-25;
  

  _MenuWindow= new NTK_Window(_NWM -> Path());
  _MenuWindow -> ScreenSize(W,H);
  _MenuWindow -> SetClear(0);


  //
  // Configure Widgets
  //
  switch ( _MenuType ) {
    case NWM_MENU_CLEAR_SCENE:
      _MenuBox[0]= NPosX;
      _MenuBox[1]= NPosY;
      _MenuBox[2]= 175;
      _MenuBox[3]= 50;

      _ClearSceneMenu= new NTK_Menu(_MenuWindow,NPosX,NPosY,175,50);
      _ClearSceneMenu -> MenuCallback((NTK_MenuCallback*)ClearSceneMenuCallback);
      _ClearSceneMenu -> Title("Clear Scene?");
      _ClearSceneMenu -> Push("Make it So!",0);
      break;

    case NWM_MENU_EXTRUDE:
      _MenuBox[0]= NPosX;
      _MenuBox[1]= NPosY;
      _MenuBox[2]= 175;
      _MenuBox[3]= 50;

      _ExtrudeMenu= new NTK_Menu(_MenuWindow,NPosX,NPosY,175,50);
      _ExtrudeMenu -> MenuCallback((NTK_MenuCallback*)ExtrudeMenuCallback);
      _ExtrudeMenu -> Title("Extrude?");
      _ExtrudeMenu -> Push("Make it so!",0);
      break;

    case NWM_MENU_QUIT:
      _MenuBox[0]= NPosX;
      _MenuBox[1]= NPosY;
      _MenuBox[2]= 175;
      _MenuBox[3]= 50;

      _QuitMenu= new NTK_Menu(_MenuWindow,NPosX,NPosY,175,50);
      _QuitMenu -> MenuCallback((NTK_MenuCallback*)QuitMenuCallback);
      _QuitMenu -> Title("Quit Nurbana?");
      _QuitMenu -> Push("Click to Exit",0);
      break;

    case NWM_MENU_DELETE:
      _MenuBox[0]= NPosX;
      _MenuBox[1]= NPosY;
      _MenuBox[2]= 150;
      _MenuBox[3]= 50;

      _DeleteMenu= new NTK_Menu(_MenuWindow,NPosX,NPosY,150,50);
      _DeleteMenu -> MenuCallback((NTK_MenuCallback*)DeleteMenuCallback);
      _DeleteMenu -> Title("Delete Object?");
      _DeleteMenu -> Push("Trashcan!",0);
      break;

    case NWM_MENU_MATERIALS:
    {
      unsigned	int	ID;
      char		Label[32];

      _MenuBox[0]= NPosX;
      _MenuBox[1]= NPosY;
      _MenuBox[2]= 175;
      _MenuBox[3]= 110;

      _MaterialsMenu= new NTK_Menu(_MenuWindow,NPosX,NPosY,175,110);
      _MaterialsMenu -> MenuCallback((NTK_MenuCallback*)MaterialsMenuCallback);
      _MaterialsMenu -> ScrollBar(NTK_ON);
      _MaterialsMenu -> Title("Select Material");
      _MaterialsMenu -> Push("Add New",-1);

      // Populate the Materials Menu
      if (_MaterialList -> GetHead(ID)) {
        do {
          _MaterialList -> GetName(ID,Label);
          _MaterialsMenu -> Push(Label,ID);
        } while (_MaterialList -> GetNext(ID));
      } //fi
      break;
    } //eoc

    case NWM_MENU_NUMERICS:
      _NumericsTitle= new NTK_Label(_MenuWindow,NPosX+5,NPosY+5,175,20);
      _NumericsTitle -> Rounded(NTK_ON);
      _NumericsTitle -> Bevel(NTK_ON);
      _NumericsTitle -> Text("Numerics Menu");
      _NumericsTitle -> Color(0xD0,0xD0,0xD0);

      if (_ObjectList -> GetMode(OBJECT_MODE_EDIT)) {
        _MenuBox[0]= NPosX;
        _MenuBox[1]= NPosY;
        _MenuBox[2]= 175;
        _MenuBox[3]= 110;

        _NumericsBackground= new NTK_Label(_MenuWindow,NPosX,NPosY,185,115);
        _NumericsBackground -> Bevel(NTK_ON);
        _NumericsBackground -> Color(0xA0,0xA0,0xA0);

        _NLocX= new NTK_Slider_Button(_MenuWindow,NPosX+5,NPosY+30,145,20);
        _NLocX -> Layer(1);
        _NLocX -> Rounded(NTK_ON);
        _NLocX -> Precision(3);
        _NLocX -> Min(-1000);
        _NLocX -> Max(1000);
        _NLocX -> Step(1);
        _NLocX -> Color(0xA0,0xA0,0xA0);
        _NLocX -> StaticText("LocX:");
        _NLocX -> CallbackTrigger(NTK_CT_MOUSE_DOWN | NTK_CT_KEY);

        _NLocY= new NTK_Slider_Button(_MenuWindow,NPosX+5,NPosY+50,145,20);
        _NLocY -> Layer(1);
        _NLocY -> Rounded(NTK_ON);
        _NLocY -> Precision(3);
        _NLocY -> Min(-1000);
        _NLocY -> Max(1000);
        _NLocY -> Step(1);
        _NLocY -> Color(0xA0,0xA0,0xA0);
        _NLocY -> StaticText("LocY:");
        _NLocY -> CallbackTrigger(NTK_CT_MOUSE_DOWN | NTK_CT_KEY);

        _NLocZ= new NTK_Slider_Button(_MenuWindow,NPosX+5,NPosY+70,145,20);
        _NLocZ -> Layer(1);
        _NLocZ -> Rounded(NTK_ON);
        _NLocZ -> Precision(3);
        _NLocZ -> Min(-1000);
        _NLocZ -> Max(1000);
        _NLocZ -> Step(1);
        _NLocZ -> Color(0xA0,0xA0,0xA0);
        _NLocZ -> StaticText("LocZ:");
        _NLocZ -> CallbackTrigger(NTK_CT_MOUSE_DOWN | NTK_CT_KEY);

        _NLocW= new NTK_Slider_Button(_MenuWindow,NPosX+5,NPosY+90,145,20);
        _NLocW -> Layer(1);
        _NLocW -> Rounded(NTK_ON);
        _NLocW -> Precision(3);
        _NLocW -> Min(0.1);
        _NLocW -> Max(10);
        _NLocW -> Step(1);
        _NLocW -> Color(0xA0,0xA0,0xA0);
        _NLocW -> StaticText("Wght:");
        _NLocW -> CallbackTrigger(NTK_CT_MOUSE_DOWN | NTK_CT_KEY);

        _NumericsOK= new NTK_Button(_MenuWindow,NPosX+155,NPosY+30,25,80);
        _NumericsOK -> Layer(1);
        _NumericsOK -> Rounded(NTK_ON);
        _NumericsOK -> Type(NTK_BUTTON_TEXT);
        _NumericsOK -> Color(0x80,0x90,0xA0);
        _NumericsOK -> Text("OK");
        _NumericsOK -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
        _NumericsOK -> Callback((NTK_Callback*)NumericsCallback);

        if (_ObjectList -> CPActCount() == 1) {
          _NLocX -> Value(_ObjectList -> GetCP(_ObjectList -> GetCPActID()).x);
          _NLocY -> Value(_ObjectList -> GetCP(_ObjectList -> GetCPActID()).y);
          _NLocZ -> Value(_ObjectList -> GetCP(_ObjectList -> GetCPActID()).z);
          _NLocW -> Value(_ObjectList -> GetH(_ObjectList -> GetCPActID()));
        } else { 
          _NLocX -> Value(0);
          _NLocY -> Value(0);
          _NLocZ -> Value(0);
          _NLocW -> Value(0);
        } //fi

      } else {
        _MenuBox[0]= NPosX;
        _MenuBox[1]= NPosY;
        _MenuBox[2]= 175;
        _MenuBox[3]= 210;

        _NumericsBackground= new NTK_Label(_MenuWindow,NPosX,NPosY,185,215);
        _NumericsBackground -> Bevel(NTK_ON);
        _NumericsBackground -> Color(0xA0,0xA0,0xA0);

        _NLocX= new NTK_Slider_Button(_MenuWindow,NPosX+5,NPosY+30,145,20);
        _NLocX -> Layer(1);
        _NLocX -> Rounded(NTK_ON);
        _NLocX -> Precision(3);
        _NLocX -> Min(-1000);
        _NLocX -> Max(1000);
        _NLocX -> Step(1);
        _NLocX -> Color(0xA0,0xA0,0xA0);
        _NLocX -> StaticText("LocX:");
        _NLocX -> Value(_ObjectList -> GetCOR().x);
        _NLocX -> CallbackTrigger(NTK_CT_MOUSE_DOWN | NTK_CT_KEY);

        _NLocY= new NTK_Slider_Button(_MenuWindow,NPosX+5,NPosY+50,145,20);
        _NLocY -> Layer(1);
        _NLocY -> Rounded(1);
        _NLocY -> Precision(3);
        _NLocY -> Min(-1000);
        _NLocY -> Max(1000);
        _NLocY -> Step(1);
        _NLocY -> Color(0xA0,0xA0,0xA0);
        _NLocY -> StaticText("LocY:");
        _NLocY -> Value(_ObjectList -> GetCOR().y);
        _NLocY -> CallbackTrigger(NTK_CT_MOUSE_DOWN | NTK_CT_KEY);

        _NLocZ= new NTK_Slider_Button(_MenuWindow,NPosX+5,NPosY+70,145,20);
        _NLocZ -> Layer(1);
        _NLocZ -> Rounded(1);
        _NLocZ -> Precision(3);
        _NLocZ -> Min(-1000);
        _NLocZ -> Max(1000);
        _NLocZ -> Step(1);
        _NLocZ -> Color(0xA0,0xA0,0xA0);
        _NLocZ -> StaticText("LocZ:");
        _NLocZ -> Value(_ObjectList -> GetCOR().z);
        _NLocZ -> CallbackTrigger(NTK_CT_MOUSE_DOWN | NTK_CT_KEY);

        _NRotX= new NTK_Slider_Button(_MenuWindow,NPosX+5,NPosY+90,145,20);
        _NRotX -> Layer(1);
        _NRotX -> Rounded(NTK_ON);
        _NRotX -> Precision(3);
        _NRotX -> Min(-360);
        _NRotX -> Max(360);
        _NRotX -> Step(1);
        _NRotX -> Color(0xA0,0xA0,0xA0);
        _NRotX -> StaticText("RotX:");
        _NRotX -> Value(_ObjectList -> GetROT().x);
        _NRotX -> CallbackTrigger(NTK_CT_MOUSE_DOWN | NTK_CT_KEY);

        _NRotY= new NTK_Slider_Button(_MenuWindow,NPosX+5,NPosY+110,145,20);
        _NRotY -> Layer(1);
        _NRotY -> Rounded(NTK_ON);
        _NRotY -> Precision(3);
        _NRotY -> Min(-360);
        _NRotY -> Max(360);
        _NRotY -> Step(1);
        _NRotY -> Color(0xA0,0xA0,0xA0);
        _NRotY -> StaticText("RotY:");
        _NRotY -> Value(_ObjectList -> GetROT().y);
        _NRotY -> CallbackTrigger(NTK_CT_MOUSE_DOWN | NTK_CT_KEY);

        _NRotZ= new NTK_Slider_Button(_MenuWindow,NPosX+5,NPosY+130,145,20);
        _NRotZ -> Layer(1);
        _NRotZ -> Rounded(NTK_ON);
        _NRotZ -> Precision(3);
        _NRotZ -> Min(-360);
        _NRotZ -> Max(360);
        _NRotZ -> Step(1);
        _NRotZ -> Color(0xA0,0xA0,0xA0);
        _NRotZ -> StaticText("RotZ:");
        _NRotZ -> Value(_ObjectList -> GetROT().z);
        _NRotZ -> CallbackTrigger(NTK_CT_MOUSE_DOWN | NTK_CT_KEY);

        _NSizX= new NTK_Slider_Button(_MenuWindow,NPosX+5,NPosY+150,145,20);
        _NSizX -> Layer(1);
        _NSizX -> Rounded(NTK_ON);
        _NSizX -> Precision(3);
        _NSizX -> Min(0.001);
        _NSizX -> Max(1000);
        _NSizX -> Step(1);
        _NSizX -> Color(0xA0,0xA0,0xA0);
        _NSizX -> StaticText("SizX:");
        _NSizX -> Value(_ObjectList -> GetSIZ().x);
        _NSizX -> CallbackTrigger(NTK_CT_MOUSE_DOWN | NTK_CT_KEY);

        _NSizY= new NTK_Slider_Button(_MenuWindow,NPosX+5,NPosY+170,145,20);
        _NSizY -> Layer(1);
        _NSizY -> Rounded(NTK_ON);
        _NSizY -> Precision(3);
        _NSizY -> Min(0.001);
        _NSizY -> Max(1000);
        _NSizY -> Step(1);
        _NSizY -> Color(0xA0,0xA0,0xA0);
        _NSizY -> StaticText("SizY:");
        _NSizY -> Value(_ObjectList -> GetSIZ().y);
        _NSizY -> CallbackTrigger(NTK_CT_MOUSE_DOWN | NTK_CT_KEY);

        _NSizZ= new NTK_Slider_Button(_MenuWindow,NPosX+5,NPosY+190,145,20);
        _NSizZ -> Layer(1);
        _NSizZ -> Rounded(NTK_ON);
        _NSizZ -> Precision(3);
        _NSizZ -> Min(0.001);
        _NSizZ -> Max(1000);
        _NSizZ -> Step(1);
        _NSizZ -> Color(0xA0,0xA0,0xA0);
        _NSizZ -> StaticText("SizZ:");
        _NSizZ -> Value(_ObjectList -> GetSIZ().z);
        _NSizZ -> CallbackTrigger(NTK_CT_MOUSE_DOWN | NTK_CT_KEY);

        _NumericsOK= new NTK_Button(_MenuWindow,NPosX+155,NPosY+30,25,180);
        _NumericsOK -> Layer(1);
        _NumericsOK -> Rounded(NTK_ON);
        _NumericsOK -> Type(NTK_BUTTON_TEXT);
        _NumericsOK -> Color(0x80,0x90,0xA0);
        _NumericsOK -> Text("OK");
        _NumericsOK -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
        _NumericsOK -> Callback((NTK_Callback*)NumericsCallback);
      } //fi
      break;

    case NWM_MENU_LOFT:
      if (_ObjectList -> SelectionTotal() > 1) {
        _MenuBox[0]= NPosX;
        _MenuBox[1]= NPosY;
        _MenuBox[2]= 200;
        _MenuBox[3]= 80;

        _LoftBackground= new NTK_Label(_MenuWindow,NPosX,NPosY,200,80);
        _LoftBackground -> Bevel(NTK_ON);
        _LoftBackground -> Color(0xA0,0xA0,0xA0);

        _LoftTitle= new NTK_Label(_MenuWindow,NPosX+5,NPosY+5,190,20);
        _LoftTitle -> Bevel(NTK_ON);
        _LoftTitle -> Layer(1);
        _LoftTitle -> Text("Loft Surface Menu");
        _LoftTitle -> Color(0xD0,0xD0,0xD0);

        _DegreeLabel= new NTK_Label(_MenuWindow,NPosX+10,NPosY+30,130,20);
        _DegreeLabel -> Layer(1);
        _DegreeLabel -> Align(NTK_JUSTIFY_LEFT);
        _DegreeLabel -> Text("V Surface Degree:");
        _DegreeLabel -> Color(0xA0,0xA0,0xA0);

        _Degree= new NTK_TextInput(_MenuWindow,NPosX+145,NPosY+30,45,20);
        _Degree -> Layer(1);
        _Degree -> Color(0xA0,0xA0,0xA0);
        _Degree -> Text("2");
        _Degree -> CallbackTrigger(NTK_CT_MOUSE_DOWN | NTK_CT_KEY);

        _Loft= new NTK_Button(_MenuWindow,NPosX+5,NPosY+55,190,20);
        _Loft -> Layer(1);
        _Loft -> Type(NTK_BUTTON_TEXT);
        _Loft -> Color(0x80,0x90,0xA0);
        _Loft -> Text("Loft Surface");
        _Loft -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
        _Loft -> Callback((NTK_Callback*)LoftMenuCallback);
      } //fi
      break;

    case NWM_MENU_BLEND:
      _MenuBox[0]= NPosX;
      _MenuBox[1]= NPosY;
      _MenuBox[2]= 200;
      _MenuBox[3]= 80;

      _BlendBackground= new NTK_Label(_MenuWindow,NPosX,NPosY,200,80);
      _BlendBackground -> Bevel(NTK_ON);
      _BlendBackground -> Color(0xA0,0xA0,0xA0);

      _BlendTitle= new NTK_Label(_MenuWindow,NPosX+5,NPosY+5,190,20);
      _BlendTitle -> Bevel(NTK_ON);
      _BlendTitle -> Layer(1);
      _BlendTitle -> Text("Surface Blending Menu");
      _BlendTitle -> Color(0xD0,0xD0,0xD0);

      _TwistLabel= new NTK_Label(_MenuWindow,NPosX+10,NPosY+30,130,20);
      _TwistLabel -> Layer(1);
      _TwistLabel -> Align(NTK_JUSTIFY_LEFT);
      _TwistLabel -> Text("Twist Surface:");
      _TwistLabel -> Color(0xA0,0xA0,0xA0);

      _TwistButton= new NTK_Button(_MenuWindow,NPosX+150,NPosY+30,20,20);
      _TwistButton -> Layer(1);
      _TwistButton -> Sticky(NTK_ON);
      _TwistButton -> Type(NTK_BUTTON_TEXT);
      _TwistButton -> Text("");
      _TwistButton -> Color(0x80,0x90,0xA0);
      _TwistButton -> CallbackTrigger(NTK_CT_MOUSE_DOWN);

      _Blend= new NTK_Button(_MenuWindow,NPosX+5,NPosY+55,190,20);
      _Blend -> Layer(1);
      _Blend -> Type(NTK_BUTTON_TEXT);
      _Blend -> Color(0x80,0x90,0xA0);
      _Blend -> Text("Blend Surfaces");
      _Blend -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
      _Blend -> Callback((NTK_Callback*)BlendMenuCallback);
      break;

    case NWM_MENU_REBUILD:
      _MenuBox[0]= NPosX;
      _MenuBox[1]= NPosY;
      _MenuBox[2]= 250;
      _MenuBox[3]= 165;

      _RebuildBackground= new NTK_Label(_MenuWindow,NPosX,NPosY,250,165);
      _RebuildBackground -> Bevel(NTK_ON);
      _RebuildBackground -> Color(0xA0,0xA0,0xA0);

      _LoftTitle= new NTK_Label(_MenuWindow,NPosX+5,NPosY+5,240,20);
      _LoftTitle -> Bevel(NTK_ON);
      _LoftTitle -> Layer(1);
      _LoftTitle -> Text("Rebuild Menu");
      _LoftTitle -> Color(0xD0,0xD0,0xD0);

      _DegreeULabel= new NTK_Label(_MenuWindow,NPosX+5,NPosY+30,50,20);
      _DegreeULabel -> Layer(1);
      _DegreeULabel -> Align(NTK_JUSTIFY_LEFT);
      _DegreeULabel -> Text("U Deg:");
      _DegreeULabel -> Color(0xA0,0xA0,0xA0);

      _DegreeVLabel= new NTK_Label(_MenuWindow,NPosX+5,NPosY+50,50,20);
      _DegreeVLabel -> Layer(1);
      _DegreeVLabel -> Align(NTK_JUSTIFY_LEFT);
      _DegreeVLabel -> Text("V Deg:");
      _DegreeVLabel -> Color(0xA0,0xA0,0xA0);

      _CPULabel= new NTK_Label(_MenuWindow,NPosX+5,NPosY+70,50,20);
      _CPULabel -> Layer(1);
      _CPULabel -> Align(NTK_JUSTIFY_LEFT);
      _CPULabel -> Text("U CP#:");
      _CPULabel -> Color(0xA0,0xA0,0xA0);

      _CPVLabel= new NTK_Label(_MenuWindow,NPosX+5,NPosY+90,50,20);
      _CPVLabel -> Layer(1);
      _CPVLabel -> Align(NTK_JUSTIFY_LEFT);
      _CPVLabel -> Text("V CP#:");
      _CPVLabel -> Color(0xA0,0xA0,0xA0);

      if (_ObjectList -> GetLength(0) > 1) {
        _DegreeUSlider= new NTK_Slider_Horz(_MenuWindow,NPosX+55,NPosY+30,190,20);
        _DegreeUSlider -> Layer(1);
        _DegreeUSlider -> Type(NTK_HORZ_SLIDER);
        _DegreeUSlider -> Min(1);
        _DegreeUSlider -> Max((_ObjectList -> GetOrder(0)-1)*2);
        _DegreeUSlider -> Value(_ObjectList -> GetOrder(0)-1);
        _DegreeUSlider -> Color(0xA0,0xA0,0xA0);
        _DegreeUSlider -> SliderColor(0x80,0x90,0xA0);

        _CPUSlider= new NTK_Slider_Horz(_MenuWindow,NPosX+55,NPosY+70,190,20);
        _CPUSlider -> Layer(1);
        _CPUSlider -> Type(NTK_HORZ_SLIDER);
        _CPUSlider -> Min(1);
        _CPUSlider -> Max((_ObjectList -> GetLength(0))*2);
        _CPUSlider -> Value(_ObjectList -> GetLength(0));
        _CPUSlider -> Color(0xA0,0xA0,0xA0);
        _CPUSlider -> SliderColor(0x80,0x90,0xA0);
      } //fi

      if (_ObjectList -> GetLength(1) > 1) {
        _DegreeVSlider= new NTK_Slider_Horz(_MenuWindow,NPosX+55,NPosY+50,190,20);
        _DegreeVSlider -> Layer(1);
        _DegreeVSlider -> Type(NTK_HORZ_SLIDER);
        _DegreeVSlider -> Min(1);
        _DegreeVSlider -> Max((_ObjectList -> GetOrder(1)-1)*2);
        _DegreeVSlider -> Value(_ObjectList -> GetOrder(1)-1);
        _DegreeVSlider -> Color(0xA0,0xA0,0xA0);
        _DegreeVSlider -> SliderColor(0x80,0x90,0xA0);

        _CPVSlider= new NTK_Slider_Horz(_MenuWindow,NPosX+55,NPosY+90,190,20);
        _CPVSlider -> Layer(1);
        _CPVSlider -> Type(NTK_HORZ_SLIDER);
        _CPVSlider -> Min(1);
        _CPVSlider -> Max((_ObjectList -> GetLength(1))*2);
        _CPVSlider -> Value(_ObjectList -> GetLength(1));
        _CPVSlider -> Color(0xA0,0xA0,0xA0);
        _CPVSlider -> SliderColor(0x80,0x90,0xA0);
      } //fi

      _RebuildGroup= new NTK_Group();

      _Centripetal= new NTK_Button(_MenuWindow,NPosX+5,NPosY+115,100,20);
      _Centripetal -> Layer(1);
      _Centripetal -> Status(NTK_ON);
      _Centripetal -> Sticky(NTK_ON);
      _Centripetal -> Type(NTK_BUTTON_TEXT);
      _Centripetal -> Group(_RebuildGroup);
      _Centripetal -> Color(0xA0,0xA0,0xA0);
      _Centripetal -> Text("Centripetal");
      _Centripetal -> CallbackTrigger(NTK_CT_MOUSE_DOWN);

      _Chord= new NTK_Button(_MenuWindow,NPosX+105,NPosY+115,55,20);
      _Chord -> Layer(1);
      _Chord -> Sticky(NTK_ON);
      _Chord -> Type(NTK_BUTTON_TEXT);
      _Chord -> Group(_RebuildGroup);
      _Chord -> Color(0xA0,0xA0,0xA0);
      _Chord -> Text("Chord");
      _Chord -> CallbackTrigger(NTK_CT_MOUSE_DOWN);

      _Universal= new NTK_Button(_MenuWindow,NPosX+160,NPosY+115,85,20);
      _Universal -> Layer(1);
      _Universal -> Sticky(NTK_ON);
      _Universal -> Type(NTK_BUTTON_TEXT);
      _Universal -> Group(_RebuildGroup);
      _Universal -> Color(0xA0,0xA0,0xA0);
      _Universal -> Text("Universal");
      _Universal -> CallbackTrigger(NTK_CT_MOUSE_DOWN);

      _Rebuild= new NTK_Button(_MenuWindow,NPosX+5,NPosY+140,240,20);
      _Rebuild -> Layer(1);
      _Rebuild -> Type(NTK_BUTTON_TEXT);
      _Rebuild -> Color(0x80,0x90,0xA0);
      _Rebuild -> Text("Rebuild");
      _Rebuild -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
      _Rebuild -> Callback((NTK_Callback*)RebuildMenuCallback);
      break;

    case NWM_MENU_RENDER:
      _AutoDestroy= 0;

      _MenuBox[0]= (W-_RT -> Size(0)-10)/2;
      _MenuBox[1]= (H-_RT -> Size(1)-60)/2;
      _MenuBox[2]= _RT -> Size(0)+10;
      _MenuBox[3]= _RT -> Size(1)+60;

      _RebuildBackground= new NTK_Label(_MenuWindow,_MenuBox[0],_MenuBox[1],_MenuBox[2],_MenuBox[3]);
      _RebuildBackground -> Bevel(NTK_ON);
      _RebuildBackground -> Color(0xA0,0xA0,0xA0);

      _RenderTitle= new NTK_Label(_MenuWindow,_MenuBox[0]+5,_MenuBox[1]+5,_MenuBox[2]-10,20);
      _RenderTitle -> Bevel(NTK_ON);
      _RenderTitle -> Layer(1);
      _RenderTitle -> Text("Render Menu");
      _RenderTitle -> Color(0xD0,0xD0,0xD0);

      _RenderArea= new NTK_Label(_MenuWindow,_MenuBox[0]+5,_MenuBox[1]+30,_MenuBox[2]-10,_RT -> Size(0));
      _RenderArea -> Bevel(NTK_ON);
      _RenderArea -> Layer(1);
      _RenderArea -> Type(NTK_LABEL_GLYPHIC);
      _RenderArea -> Glyphic(_MenuWindow -> GlyphicXPM(dash_xpm,NTK_OFF));
      _RenderArea -> Color(0x00,0x00,0x00);

      _RenderCancel= new NTK_Button(_MenuWindow,_MenuBox[0]+5,_MenuBox[1]+_MenuBox[3]-25,_MenuBox[2]-10,20);
      _RenderCancel -> Layer(1);
      _RenderCancel -> Type(NTK_BUTTON_TEXT);
      _RenderCancel -> Color(0x80,0x90,0xA0);
      _RenderCancel -> Text("Cancel Render");
      _RenderCancel -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
      _RenderCancel -> Callback((NTK_Callback*)RenderMenuCallback);

      _RT -> Reset();
      _Complete= 0;

      _start= clock();

//      glutIdleFunc(RenderScene);
      NIdle::Callback(RenderScene);

      break;

    default:
      break;
  } //eos

} //eof NWM_Menu::NWM_Menu()


NWM_Menu::~NWM_Menu() {
  delete _MenuWindow;
} //eof NWM_Menu::~NWM_Menu()


void NWM_Menu::Update() {
  glEnable(GL_SCISSOR_TEST);
    glViewport(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
    glScissor(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
      _MenuWindow -> Draw();
  glDisable(GL_SCISSOR_TEST);
} //eof NWM_Menu::Update()


void NWM_Menu::Refresh() {
  _MenuWindow -> SetClear(2);
  _MenuWindow -> UpdateAll();
} //eof NWM_Menu::Refresh()

void NWM_Menu::Reshape(int w, int h) {
} //eof NWM_Menu::Reshape()


char NWM_Menu::Key(int x, int y) {
  _MenuWindow -> Handle(NTK_EVENT_KEY,x,y,0,0);
  _Update= 2;

  return(NWM_NOTHING);
} //eof NWM_Menu::Key()


char NWM_Menu::Mouse(int button, int state, int x, int y) {
  _MenuWindow -> Handle(NTK_EVENT_MOUSE,button,state,x,y);
  _Update= 2;

  return(NWM_NOTHING);
} //eof NWM_Menu::Mouse()


char NWM_Menu::Motion(int x, int y) {
  _MenuWindow -> Handle(NTK_EVENT_MOTION,x,y,0,0);
  _Update= 1;

  return(NWM_NOTHING);
} //eof NWM_Menu::Motion()


char NWM_Menu::Passive(int x, int y) {
  // Check to see if mouse exits Menu area,
  // and if it does, Self Destruct

  if (_AutoDestroy && !NGUtils::Inside(x,y,_MenuBox[0]-NWM_MENU_PADDING,_MenuBox[1]-NWM_MENU_PADDING,_MenuBox[2]+2*NWM_MENU_PADDING,_MenuBox[3]+2*NWM_MENU_PADDING)) {
    _SelfDestruct= 1;
  } //fi

  return(NWM_NOTHING);
} //eof NWM_Menu::Passive()


void NWM_Menu::Active(bool A) {
} //eof NWM_Menu::Active()