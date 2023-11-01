#include "NWM_KnotMenu.h"



// +-----------+
// | Callbacks |
// +-----------+

void ApplyCallback(char E) {
  int	i,min;

  if (E == NTK_EVENT_MOUSE_UP) {
    // Apply U Changes
    if (NWM_KnotMenu::Access -> _ObjectList -> GetOrder(0) + NWM_KnotMenu::Access -> _ObjectList -> GetLength(0) > 11) {
      min= 1 + (NWM_KnotMenu::Access -> _ObjectList -> GetOrder(0) + NWM_KnotMenu::Access -> _ObjectList -> GetLength(0) - 11) * NWM_KnotMenu::Access -> _UScrollBar -> Value();
      for (i= min; i < min+11; i++)
        NWM_KnotMenu::Access -> _ObjectList -> SetKnot(i-1,0,NWM_KnotMenu::Access -> _UInput[i-min] -> Value());
    } else {
      for (i= 0; i < NWM_KnotMenu::Access -> _ObjectList -> GetOrder(0) + NWM_KnotMenu::Access -> _ObjectList -> GetLength(0); i++)
        NWM_KnotMenu::Access -> _ObjectList -> SetKnot(i,0,NWM_KnotMenu::Access -> _UInput[i] -> Value());
    } //fi

    // Apply V Changes
    if (NWM_KnotMenu::Access -> _ObjectList -> GetOrder(1) + NWM_KnotMenu::Access -> _ObjectList -> GetLength(1) > 11) {
      min= 1 + (NWM_KnotMenu::Access -> _ObjectList -> GetOrder(1) + NWM_KnotMenu::Access -> _ObjectList -> GetLength(1) - 11) * NWM_KnotMenu::Access -> _VScrollBar -> Value();
      for (i= min; i < min+11; i++)
        NWM_KnotMenu::Access -> _ObjectList -> SetKnot(i-1,1,NWM_KnotMenu::Access -> _VInput[i-min] -> Value());
    } else {
      for (i= 0; i < NWM_KnotMenu::Access -> _ObjectList -> GetOrder(1) + NWM_KnotMenu::Access -> _ObjectList -> GetLength(1); i++)
        NWM_KnotMenu::Access -> _ObjectList -> SetKnot(i,1,NWM_KnotMenu::Access -> _VInput[i] -> Value());
    } //fi

    NWM_KnotMenu::Access -> _ObjectList -> SetChange();
    NWM_KnotMenu::Access -> _ObjectList -> SetRecalcTess();

    NWM_KnotMenu::Access -> _NWM -> UpdateType(NWM_WT_ModelView);
    NWM_KnotMenu::Access -> SelfDestruct(1);
  } //fi
} //eof ScrollBarCallback()


void UScrollBarCallback(char E) {
  int	i,min,max;
  char	label[32];

  // Update Text Input Boxen in U Direction
  min= 1 + (NWM_KnotMenu::Access -> _ObjectList -> GetOrder(0) + NWM_KnotMenu::Access -> _ObjectList -> GetLength(0) - 11) * NWM_KnotMenu::Access -> _UScrollBar -> Value();
  max= min+10;
  for (i= min; i <= max; i++) {
    sprintf(label,"%d:",i);
    NWM_KnotMenu::Access -> _UInput[i-min] -> StaticText(label);
    sprintf(label,"%f",NWM_KnotMenu::Access -> _ObjectList -> GetKnot(i-1,0));
    NWM_KnotMenu::Access -> _UInput[i-min] -> Text(label);
  } //eof
} //eof UScrollBarCallback()


void VScrollBarCallback(char E) {
  int	i,min,max;
  char	label[32];

  // Update Text Input Boxen in V Direction
  min= 1 + (NWM_KnotMenu::Access -> _ObjectList -> GetOrder(1) + NWM_KnotMenu::Access -> _ObjectList -> GetLength(1) - 11) * NWM_KnotMenu::Access -> _VScrollBar -> Value();
  max= min+10;
  for (i= min; i <= max; i++) {
    sprintf(label,"%d:",i);
    NWM_KnotMenu::Access -> _VInput[i-min] -> StaticText(label);
    sprintf(label,"%f",NWM_KnotMenu::Access -> _ObjectList -> GetKnot(i-1,1));
    NWM_KnotMenu::Access -> _VInput[i-min] -> Text(label);
  } //eof
} //eof VScrollBarCallback()


// +---------------+
// | EOF Callbacks |
// +---------------+


NWM_KnotMenu* NWM_KnotMenu::Access= 0;
NWM_KnotMenu::NWM_KnotMenu(int PosX, int PosY, int W, int H, NWM_List *obj, bool Docked, Object_Manager *ObjectList, NWM *NWMPtr, NWM_List *Head) : NWM_Base(obj,NWM_WT_Menu) {
  _self= this;
  Access= _self;

  _Head= Head;
  _NWM= NWMPtr;
  _ObjectList= ObjectList;

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

  _KnotMenuWindow= new NTK_Window(_NWM -> Path());
  _KnotMenuWindow -> ScreenSize(W,H);
  _KnotMenuWindow -> SetClear(0);

  _MenuBox[0]= PosX-130;
  _MenuBox[1]= PosY-45;
  _MenuBox[2]= 260;
  _MenuBox[3]= 300;

  //
  // Configure Widgets
  //
  int	NPosX=10,NPosY=35;

  if (PosX-135 > 0)
    NPosX= PosX-135;
  if (PosX+260 > W)
    NPosX= W-260;
  if (PosY-35 > 0)
    NPosY= PosY-15;
  if (PosY+275 > H)
    NPosY= H-275;

  _MenuBox[0]= NPosX-5;
  _MenuBox[1]= NPosY-30;
  _MenuBox[2]= 260;
  _MenuBox[3]= 300;

  _BackDrop= new NTK_Label(_KnotMenuWindow,NPosX-5,NPosY-30,260,300);
  _BackDrop -> Bevel(NTK_ON);
  _BackDrop -> Color(0xA0,0xA0,0xA0);

  _TitleBar= new NTK_Label(_KnotMenuWindow,NPosX,NPosY-25,250,20);
  _TitleBar -> Layer(1);
  _TitleBar -> Bevel(NTK_ON);
  _TitleBar -> Text("Knot Vector Editor");
  _TitleBar -> Color(0xD0,0xD0,0xD0);

  _UKV= new NTK_Label(_KnotMenuWindow,NPosX,NPosY,125,20);
  _UKV -> Layer(1);
  _UKV -> Bevel(NTK_ON);
  _UKV -> Text("U Vector");
  _UKV -> Color(0x80,0x90,0xA0);

  _VKV= new NTK_Label(_KnotMenuWindow,NPosX+125,NPosY,125,20);
  _VKV -> Layer(1);
  _VKV -> Bevel(NTK_ON);
  _VKV -> Text("V Vector");
  _VKV -> Color(0x80,0x90,0xA0);

  _Apply= new NTK_Button(_KnotMenuWindow,NPosX,NPosY+245,250,20);
  _Apply -> Layer(1);
  _Apply -> Color(0x80,0x90,0xA0);
  _Apply -> Text("Apply");
  _Apply -> CallbackTrigger(NTK_CT_MOUSE_UP);
  _Apply -> Callback((NTK_Callback*)ApplyCallback);

  // Initialize Label and TextInput Widgets for Knot Values
  int	i,min,max;
  char	label[32];

  if (_ObjectList -> GetOrder(0) + _ObjectList -> GetLength(0) > 11) {
    _UScrollBar= new NTK_ScrollBar(_KnotMenuWindow,NPosX+105,NPosY+20,20,220);
    _UScrollBar -> Layer(1);
    _UScrollBar -> CallbackTrigger(NTK_CT_MOTION | NTK_CT_MOUSE_DOWN);
    _UScrollBar -> Callback((NTK_Callback*)UScrollBarCallback);
    _UScrollBar -> SmallScroll(1.0/10);
    // Calculate Min and Max
    min= 1 + (_ObjectList -> GetOrder(0) + _ObjectList -> GetLength(0) - 11) * _UScrollBar -> Value();
    max= min+10;
  } else {
    min= 1;
    max= _ObjectList -> GetOrder(0) + _ObjectList -> GetLength(0);
  } //fi

  for (i= min; i <= max; i++) {
    sprintf(label,"%d:",i);
    _UInput[i-min]= new NTK_TextInput(_KnotMenuWindow,NPosX,NPosY+i*20,105,20);
    _UInput[i-min] -> Layer(1);
    _UInput[i-min] -> Color(0xB0,0xB0,0xB0);
    _UInput[i-min] -> StaticText(label);
    sprintf(label,"%f",_ObjectList -> GetKnot(i-1,0));
    _UInput[i-min] -> Text(label);
    _UInput[i-min] -> CallbackTrigger(NTK_CT_MOUSE_DOWN | NTK_CT_KEY);
  } //eof


  if (_ObjectList -> GetOrder(1) + _ObjectList -> GetLength(1) > 11) {
    _VScrollBar= new NTK_ScrollBar(_KnotMenuWindow,NPosX+230,NPosY+20,20,220);
    _VScrollBar -> Layer(1);
    _VScrollBar -> CallbackTrigger(NTK_CT_MOTION | NTK_CT_MOUSE_DOWN);
    _VScrollBar -> Callback((NTK_Callback*)VScrollBarCallback);
    _VScrollBar -> SmallScroll(1.0/10);
    min= 1 + (_ObjectList -> GetOrder(1) + _ObjectList -> GetLength(1) - 11) * _VScrollBar -> Value();
    max= min+10;
  } else {
    min= 1;
    max= _ObjectList -> GetOrder(1) + _ObjectList -> GetLength(1);
  } //fi
  for (i= min; i <= max; i++) {
    sprintf(label,"%d:",i);
    _VInput[i-1]= new NTK_TextInput(_KnotMenuWindow,NPosX+125,NPosY+i*20,105,20);
    _VInput[i-1] -> Layer(1);
    _VInput[i-1] -> Color(0xB0,0xB0,0xB0);
    _VInput[i-1] -> StaticText(label);
    sprintf(label,"%f",_ObjectList -> GetKnot(i-1,1));
    _VInput[i-1] -> Text(label);
    _VInput[i-1] -> CallbackTrigger(NTK_CT_MOUSE_DOWN | NTK_CT_KEY);
  } //eof
} //eof NWM_KnotMenu::NWM_KnotMenu()


void NWM_KnotMenu::Update() {
  glEnable(GL_SCISSOR_TEST);
    glViewport(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
    glScissor(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
      _KnotMenuWindow -> Draw();
  glDisable(GL_SCISSOR_TEST);
} //eof NWM_KnotMenu::Update()


void NWM_KnotMenu::Refresh() {
} //eof NWM_KnotMenu::Refresh()


void NWM_KnotMenu::Reshape(int w, int h) {
} //eof NWM_KnotMenu::Reshape()


char NWM_KnotMenu::Key(int x, int y) {
  Access= _self;
  _Update= 2;

  _KnotMenuWindow -> Handle(NTK_EVENT_KEY,x,y,0,0);
  return(NWM_NOTHING);
} //eof NWM_KnotMenu::Key()


char NWM_KnotMenu::Mouse(int button, int state, int x, int y) {
  _KnotMenuWindow -> Handle(NTK_EVENT_MOUSE,button,state,x,y);
  _Update= 2;

  return(NWM_NOTHING);
} //eof NWM_KnotMenu::Mouse()


char NWM_KnotMenu::Motion(int x, int y) {
  Access= _self;

  _KnotMenuWindow -> Handle(NTK_EVENT_MOTION,x,y,0,0);
  _Update= 1;
  return(NWM_NOTHING);
} //eof NWM_KnotMenu::Motion()


char NWM_KnotMenu::Passive(int x, int y) {
  if (!NGUtils::Inside(x,y,_MenuBox[0]-NWM_MENU_PADDING,_MenuBox[1]-NWM_MENU_PADDING,_MenuBox[2]+2*NWM_MENU_PADDING,_MenuBox[3]+2*NWM_MENU_PADDING)) {
    _SelfDestruct= 1;
  } //fi

  return(NWM_NOTHING);
} //eof NWM_KnotMenu::Passive()


void NWM_KnotMenu::Active(bool A) {
} //eof NWM_KnotMenu::Active()
