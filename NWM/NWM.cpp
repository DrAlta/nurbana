#include "NWM.h"


NWM::NWM() {
  _self= this;
  _Head= new NWM_List;
  _MaterialList= new MaterialList();
  _ObjectList= new Object_Manager(_MaterialList);
  _MenuLock= 0;

  _RT= new NRT_RayTracer(320,320);
  _RT -> PacketTime(1);
  _RT -> Rays(1);

//      _RT -> Camera(-4,-4,6);
      _RT -> Camera(-3,-16,8);
      _RT -> CameraFocusPt(0,0,2);
      _RT -> CameraFOV(52);
      _RT -> ViewMatrix();


      _RT -> BGColor(0x00, 0x00, 0x00);
      _RT -> MaxDepth(15);
      _RT -> SetAmbient(0.0, 0.0, 0.0);

      _RT -> Lights -> CreateLight(1,NRT_LIGHT_SPHERE,-3,3,5);
      _RT -> Lights -> Radius(1,0.5);
      _RT -> Lights -> Intensity(1,0.2,0.2,0.2);

      _RT -> Lights -> CreateLight(2,NRT_LIGHT_SPHERE,-3,-3,5);
      _RT -> Lights -> Radius(2,0.5);
      _RT -> Lights -> Intensity(2,0.2,0.2,0.2);

      _RT -> Lights -> CreateLight(3,NRT_LIGHT_SPHERE,3,-3,5);
      _RT -> Lights -> Radius(3,0.5);
      _RT -> Lights -> Intensity(3,0.2,0.2,0.2);

      _RT -> Lights -> CreateLight(4,NRT_LIGHT_SPHERE,3,3,5);
      _RT -> Lights -> Radius(4,0.5);
      _RT -> Lights -> Intensity(4,0.2,0.2,0.2);

      _RT -> Lights -> CreateLight(5,NRT_LIGHT_SPHERE,0,0,1);
      _RT -> Lights -> Radius(5,0.25);
      _RT -> Lights -> Intensity(5,0.15,0.15,0.2);

      _RT -> Objects -> CreateObject(1,NRT_OBJECT_SPHERE);
      _RT -> Objects -> Position(1,-3,3,2);
      _RT -> Objects -> Radius(1,1);
      _RT -> Objects -> Diffuse(1,0,1,0);
      _RT -> Objects -> Specular(1,0.8,0.8,0.8);
      _RT -> Objects -> Reflective(1,1);
      _RT -> Objects -> ReflectiveIndex(1,0.1);

      _RT -> Objects -> CreateObject(2,NRT_OBJECT_SPHERE);
      _RT -> Objects -> Position(2,3,3,2);
      _RT -> Objects -> Radius(2,1);
      _RT -> Objects -> Diffuse(2,1,0,0);
      _RT -> Objects -> Specular(2,0.8,0.8,0.8);
      _RT -> Objects -> Reflective(2,1);
      _RT -> Objects -> ReflectiveIndex(2,0.1);

      _RT -> Objects -> CreateObject(3,NRT_OBJECT_SPHERE);
      _RT -> Objects -> Position(3,-3,-3,2);
      _RT -> Objects -> Radius(3,1);
      _RT -> Objects -> Diffuse(3,0,0,1);
      _RT -> Objects -> Specular(3,0.8,0.8,0.8);
      _RT -> Objects -> Reflective(3,1);
      _RT -> Objects -> ReflectiveIndex(3,0.1);

      _RT -> Objects -> CreateObject(6,NRT_OBJECT_SPHERE);
      _RT -> Objects -> Position(6,3,-3,2);
      _RT -> Objects -> Radius(6,1);
      _RT -> Objects -> Diffuse(6,1,1,0);
      _RT -> Objects -> Specular(6,0.8,0.8,0.8);
      _RT -> Objects -> Reflective(6,1);
      _RT -> Objects -> ReflectiveIndex(6,0.1);

      _RT -> Objects -> CreateObject(7,NRT_OBJECT_SPHERE);
      _RT -> Objects -> Position(7,0,0,2);
      _RT -> Objects -> Radius(7,1);
      _RT -> Objects -> Diffuse(7,0.2,0.2,0.2);
//      _RT -> Objects -> Specular(7,0.25,0.25,0.25);
//      _RT -> Objects -> Reflective(7,1);
//      _RT -> Objects -> ReflectiveIndex(7,0);
      _RT -> Objects -> Transparent(7,1);
      _RT -> Objects -> RefractiveIndex(7,0.6666);

      // Floor
      _RT -> Objects -> CreateObject(4,NRT_OBJECT_TRIANGLE);
      _RT -> Objects -> Diffuse(4,1,1,1);
      _RT -> Objects -> SetPt(4,0,-10,-10,0);
      _RT -> Objects -> SetPt(4,1,-10,10,0);
      _RT -> Objects -> SetPt(4,2,10,10,0);
      _RT -> Objects -> Specular(4,0.3,0.3,0.3);
      _RT -> Objects -> Reflective(4,1);
      _RT -> Objects -> ReflectiveIndex(4,0.1);

      _RT -> Objects -> CreateObject(5,NRT_OBJECT_TRIANGLE);
      _RT -> Objects -> Diffuse(5,1,1,1);
      _RT -> Objects -> SetPt(5,0,-10,-10,0);
      _RT -> Objects -> SetPt(5,1,10,10,0);
      _RT -> Objects -> SetPt(5,2,10,-10,0);
      _RT -> Objects -> Specular(5,0.3,0.3,0.3);
      _RT -> Objects -> Reflective(5,1);
      _RT -> Objects -> ReflectiveIndex(5,0.1);

/*
      // Roof
      _RT -> Objects -> CreateObject(8,NRT_OBJECT_TRIANGLE);
      _RT -> Objects -> Diffuse(8,1,1,1);
      _RT -> Objects -> SetPt(8,0,-10,-10,10);
      _RT -> Objects -> SetPt(8,1,10,10,10);
      _RT -> Objects -> SetPt(8,2,-10,10,10);
      _RT -> Objects -> Specular(8,0.8,0.8,0.8);
      _RT -> Objects -> Reflective(8,1);
      _RT -> Objects -> ReflectiveIndex(8,0.1);

      _RT -> Objects -> CreateObject(9,NRT_OBJECT_TRIANGLE);
      _RT -> Objects -> Diffuse(9,1,1,1);
      _RT -> Objects -> SetPt(9,0,-10,-10,10);
      _RT -> Objects -> SetPt(9,1,10,-10,10);
      _RT -> Objects -> SetPt(9,2,10,10,10);
      _RT -> Objects -> Specular(9,0.8,0.8,0.8);
      _RT -> Objects -> Reflective(9,1);
      _RT -> Objects -> ReflectiveIndex(9,0.1);
*/

      // Back
      _RT -> Objects -> CreateObject(10,NRT_OBJECT_TRIANGLE);
      _RT -> Objects -> Diffuse(10,1,1,1);
      _RT -> Objects -> SetPt(10,0,-10,10,0);
      _RT -> Objects -> SetPt(10,1,10,10,4);
      _RT -> Objects -> SetPt(10,2,10,10,0);
      _RT -> Objects -> Specular(10,0.3,0.3,0.3);
      _RT -> Objects -> Reflective(10,1);
      _RT -> Objects -> ReflectiveIndex(10,0.1);

      _RT -> Objects -> CreateObject(11,NRT_OBJECT_TRIANGLE);
      _RT -> Objects -> Diffuse(11,1,1,1);
      _RT -> Objects -> SetPt(11,0,-10,10,0);
      _RT -> Objects -> SetPt(11,1,-10,10,4);
      _RT -> Objects -> SetPt(11,2,10,10,4);
      _RT -> Objects -> Specular(11,0.3,0.3,0.3);
      _RT -> Objects -> Reflective(11,1);
      _RT -> Objects -> ReflectiveIndex(11,0.1);

      // Right
      _RT -> Objects -> CreateObject(12,NRT_OBJECT_TRIANGLE);
      _RT -> Objects -> Diffuse(12,1,1,1);
      _RT -> Objects -> SetPt(12,0,10,10,0);
      _RT -> Objects -> SetPt(12,1,10,-10,4);
      _RT -> Objects -> SetPt(12,2,10,-10,0);
      _RT -> Objects -> Specular(12,0.3,0.3,0.3);
      _RT -> Objects -> Reflective(12,1);
      _RT -> Objects -> ReflectiveIndex(12,0.1);

      _RT -> Objects -> CreateObject(13,NRT_OBJECT_TRIANGLE);
      _RT -> Objects -> Diffuse(13,1,1,1);
      _RT -> Objects -> SetPt(13,0,10,10,0);
      _RT -> Objects -> SetPt(13,1,10,10,4);
      _RT -> Objects -> SetPt(13,2,10,-10,4);
      _RT -> Objects -> Specular(13,0.3,0.3,0.3);
      _RT -> Objects -> Reflective(13,1);
      _RT -> Objects -> ReflectiveIndex(13,0.1);

      // Left
      _RT -> Objects -> CreateObject(14,NRT_OBJECT_TRIANGLE);
      _RT -> Objects -> Diffuse(14,1,1,1);
      _RT -> Objects -> SetPt(14,0,-10,10,0);
      _RT -> Objects -> SetPt(14,1,-10,-10,0);
      _RT -> Objects -> SetPt(14,2,-10,-10,4);
      _RT -> Objects -> Specular(14,0.3,0.3,0.3);
      _RT -> Objects -> Reflective(14,1);
      _RT -> Objects -> ReflectiveIndex(14,0.1);

      _RT -> Objects -> CreateObject(15,NRT_OBJECT_TRIANGLE);
      _RT -> Objects -> Diffuse(15,1,1,1);
      _RT -> Objects -> SetPt(15,0,-10,10,0);
      _RT -> Objects -> SetPt(15,1,-10,-10,4);
      _RT -> Objects -> SetPt(15,2,-10,10,4);
      _RT -> Objects -> Specular(15,0.3,0.3,0.3);
      _RT -> Objects -> Reflective(15,1);
      _RT -> Objects -> ReflectiveIndex(15,0.1);

      // Front
      _RT -> Objects -> CreateObject(16,NRT_OBJECT_TRIANGLE);
      _RT -> Objects -> Diffuse(16,1,1,1);
      _RT -> Objects -> SetPt(16,0,-10,-10,0);
      _RT -> Objects -> SetPt(16,1,10,-10,0);
      _RT -> Objects -> SetPt(16,2,10,-10,4);
      _RT -> Objects -> Specular(16,0.3,0.3,0.3);
      _RT -> Objects -> Reflective(16,1);
      _RT -> Objects -> ReflectiveIndex(16,0.1);

      _RT -> Objects -> CreateObject(17,NRT_OBJECT_TRIANGLE);
      _RT -> Objects -> Diffuse(17,1,1,1);
      _RT -> Objects -> SetPt(17,0,-10,-10,0);
      _RT -> Objects -> SetPt(17,1,10,-10,4);
      _RT -> Objects -> SetPt(17,2,-10,-10,4);
      _RT -> Objects -> Specular(17,0.3,0.3,0.3);
      _RT -> Objects -> Reflective(17,1);
      _RT -> Objects -> ReflectiveIndex(17,0.1);

  _LOCK= 0;
  _FocusID= 0;
  _UpdateOff= 0;
  _ActiveWindow= 0;
  _lActiveWindow= 0;
  _DoubleRefresh= 2;

  _StaticMenus= 1;
} //eof NWM::NRT_NWM()


NWM_List::WindowNode* NWM::GetWindow(int ID) {
  if (_Head) {
    return( _Head -> GetWindowPtr(ID) );
  } //fi
  return(NULL);
} //eof NWM::GetObject()


void NWM::Init(int W, int H, char *Path, char *Filename) {
  // Default Window Layout
  _Width= W;
  _Height= H;

  // Store the location of the nurbana binary
  _Path= (char*)malloc(sizeof(char)*(strlen(Path)+1));
  strcpy(_Path,Path);

  if (Filename[0]) FileIO::OpenFile(Filename, _ObjectList, _MaterialList, NURBANA_FILETYPE_NURBS);

  DefaultLayout();
} //eof NWM::Init()


void NWM::Update() {
  NWM_List::WindowNode	*cur= NULL;
  bool			UF= 0;

  // Update Subwindows in order from Lowest Layer to Highest Layer
  if (!_UpdateOff) {
    for (int i= 0; i <= NWM_Max_Layer; i++) {
      cur= _Head -> GetHead();
      while (cur) {
        if (cur -> Window -> _Visible && cur -> Window -> _Layer == i) {
          if (cur -> Window -> _Update) {
//            printf("ID: %d\n",cur -> Window -> _Type);
            cur -> Window -> Update();
            cur -> Window -> _Update--;
            cur -> Window -> _Event= 1;
            UF= 1;
          } //fi
        } //fi
        cur= _Head -> GetWindowPtr(_Head -> GetNext(cur -> Window -> _ID));
      } //eow
      if (UF) SDL_GL_SwapBuffers();
    } //eof
  } //fi
} //eof NWM::Update()


void NWM::Visibility(int V) {
  RefreshAll();
//  glutPostRedisplay();
} //eof NWM::Visibility()


void NWM::Reshape(int w, int h) {
  NWM_List::WindowNode	*cur= NULL;

  _DoubleRefresh= 2;
  cur= _Head -> GetHead();

  // Update new Width and Height
  _Width= w;
  _Height= h;
  CalcWinGeom(0);

  while (cur) {
    // Update Position and Size of all Windows.
    // Notify all Windows of Reshape Event.
    cur -> Window -> Reshape(w,h);
    cur= _Head -> GetWindowPtr(_Head -> GetNext(cur -> Window -> _ID));
  } //eow

  DefaultLayout();

  // Update Border Select
  BorderSelect::ScreenSize(w,h);
  SDL_GL_SwapBuffers();
//  glutSwapBuffers();
} //eof NWM::Reshape()


void NWM::Key(int x, int y, bool UD) {
  NWM_List::WindowNode	*cur= NULL;


  if (UD) {
    SelfDestructCheck();
    _MouseX= x;
    _MouseY= y;
 
    if (_LOCK) {
      cur= _Head -> GetWindowPtr(_FocusID);
      ProcessResponse(cur -> Window -> Key(x, y), cur -> Window -> _ID);
    } else {
      switch( KeyTable::Key() ) {
        case SDLK_F1:
          Substitute(x,y,NWM_WT_FileManager_Load);
          break;

        case SDLK_F2:
          Substitute(x,y,NWM_WT_FileManager_Save);
          break;

        case SDLK_F3:
          Substitute(x,y,NWM_WT_SceneNotes);
          break;

        case SDLK_e:
          if (_ObjectList -> GetActiveID())
            CreateMenu(NWM_MENU_EXTRUDE);
          break;

        case SDLK_k:
          if (_ObjectList -> GetActiveID()) {
            new NWM_KnotMenu(_MouseX,_MouseY,_Width,_Height,_Head,0,_ObjectList,_self,_Head);
            _MenuLock= 1;
          } //fi
          break;

        case SDLK_l:
          if (KeyTable::Mod(KMOD_SHIFT)) {
            CreateMenu(NWM_MENU_LOFT);
          } //fi
          break;

        case SDLK_n:
          if (_ObjectList -> GetActiveID())
            CreateMenu(NWM_MENU_NUMERICS);
          break;

        case SDLK_q:
          CreateMenu(NWM_MENU_QUIT);
          break;

        case SDLK_t:
        {
            float	time= 0;

            for (int t= 0; t < 4; t++) {
              NTimer::Start();
              for (int i= 0; i < 250; i++) {
                _ObjectList -> Draw();
              } //eof
              NTimer::Stop();
              time+= NTimer::Report(1);
            } //eof
            printf("AVG TIME: %f\n",time/4);
        }
          break;

        case SDLK_x:
          if (KeyTable::Mod(KMOD_CTRL)) {
            CreateMenu(NWM_MENU_CLEAR_SCENE);
//            _ObjectList -> Purge();
//            RefreshAll();
          } else {
            if (_ObjectList -> GetActiveID()) {
              CreateMenu(NWM_MENU_DELETE);
              UpdateType(NWM_WT_ModelView);
            } //fi
          } //fi
          break;

        case SDLK_SPACE:
          if (KeyTable::Mod(KMOD_CTRL)) {
            DefaultLayout();
          } else {
            ToggleStaticMenus();
          } //fi
          break;

        case SDLK_ESCAPE:
          // Self Destruct Substitute Windows
          cur= _Head -> GetHead();
          while (cur) {
            if (cur -> Window -> _Replacement) cur -> Window -> _SelfDestruct= 1;
            cur= _Head -> GetWindowPtr(_Head -> GetNext(cur -> Window -> _ID));
          } //eow
          SelfDestructCheck();
          break;

        default:
          if (KeyTable::Key() == SDLK_r && KeyTable::Mod(KMOD_SHIFT)) {
            if (_ObjectList -> GetActiveID())
              CreateMenu(NWM_MENU_REBUILD);
          } else if (KeyTable::Key() == SDLK_b && KeyTable::Mod(KMOD_SHIFT)) {
            if (_ObjectList -> GetActiveID())
              CreateMenu(NWM_MENU_BLEND);
          } else {
            cur= _Head -> GetHead();
            while (cur) {
              if (cur -> Window -> _Visible) {
                if (NGUtils::Inside(x, y, cur -> Window -> _PosX, cur -> Window -> _PosY, cur -> Window -> _SizX, cur -> Window -> _SizY)) {
                  ProcessResponse(cur -> Window -> Key(x, y), cur -> Window -> _ID);
                  break;
                } //fi
                cur= _Head -> GetWindowPtr(_Head -> GetNext(cur -> Window -> _ID));
              } //fi
            } //eow
          } //fi
          break;
      } //eos
    } //fi
  } //fi
} //eof NWM::Key()


void NWM::Mouse(int button, int state, int x, int y) {
  NWM_List::WindowNode	*cur= NULL;

  SelfDestructCheck();

  _MouseX= x;
  _MouseY= y;

  if (_LOCK) {
    cur= _Head -> GetWindowPtr(_FocusID);
    if (cur -> Window -> _Type == NWM_WT_Handle) {
      ProcessResponse(cur -> Window -> Mouse(button, state, x, y), cur -> Window -> _ID);
    } else {
      ProcessResponse(cur -> Window -> Mouse(button, state, x - cur -> Window -> _PosX, y - cur -> Window -> _PosY), cur -> Window -> _ID);
    } //fi
  } else {
    for (int i= NWM_Max_Layer; i >= 0; i--) {
      cur= _Head -> GetHead();
      while (cur) {
        if (cur -> Window -> _Visible) {
          // Determine which Window the click took place in and propagate it accordingly
          if (NGUtils::Inside(x, y, cur -> Window -> _PosX, cur -> Window -> _PosY, cur -> Window -> _SizX, cur -> Window -> _SizY)) {
            if (cur -> Window -> _Type == NWM_WT_Handle) {
              ProcessResponse(cur -> Window -> Mouse(button, state, x, y), cur -> Window -> _ID);
            } else {
              ProcessResponse(cur -> Window -> Mouse(button, state, x - cur -> Window -> _PosX, y - cur -> Window -> _PosY), cur -> Window -> _ID);
            } //fi
            i= -1;
            break;  // Window was found, no use testing against others
          } //fi
        } //fi
        cur= _Head -> GetWindowPtr(_Head -> GetNext(cur -> Window -> _ID));
      } //eow
    } //fi
  } //fi

//  glutPostRedisplay();
} //eof NWM::Mouse()


void NWM::Motion(int x, int y) {
  NWM_List::WindowNode	*cur= NULL;

  SelfDestructCheck();

  if (_LOCK) {
    cur= _Head -> GetWindowPtr(_FocusID);
    if (cur -> Window -> _Type == NWM_WT_Handle) {
      ProcessResponse(cur -> Window -> Motion(x, y), cur -> Window -> _ID);
    } else {
      ProcessResponse(cur -> Window -> Motion(x - cur -> Window -> _PosX, y - cur -> Window -> _PosY), cur -> Window -> _ID);
    } //fi
  } else {
    for (int i= NWM_Max_Layer; i >= 0; i--) {
      cur= _Head -> GetHead();
      while (cur) {
        if (cur -> Window -> _Visible) {
          if (NGUtils::Inside(x, y, cur -> Window -> _PosX, cur -> Window -> _PosY, cur -> Window -> _SizX, cur -> Window -> _SizY)) {
            if (cur -> Window -> _Event) {
              if (cur -> Window -> _Type == NWM_WT_Handle) {
                ProcessResponse(cur -> Window -> Motion(x, y), cur -> Window -> _ID);
              } else {
                ProcessResponse(cur -> Window -> Motion(x - cur -> Window -> _PosX, y - cur -> Window -> _PosY), cur -> Window -> _ID);
              } //fi
              cur -> Window -> _Event= 0;
            } //fi
            i= -1;
            break;  // Window was found, no use testing against others
          } //fi
        } //fi
        cur= _Head -> GetWindowPtr(_Head -> GetNext(cur -> Window -> _ID));
      } //eow
    } //fi
  } //fi

//  glutPostRedisplay();
} //eof NWM::Motion()


void NWM::PassiveMotion(int x, int y) {
  NWM_List::WindowNode	*cur= NULL;


  SelfDestructCheck();

  if (_LOCK) {
    cur= _Head -> GetWindowPtr(_FocusID);
    if (cur -> Window -> _Event || _UpdateOff) {
      cur -> Window -> Passive(x - cur -> Window -> _PosX, y - cur -> Window -> _PosY);
      cur -> Window -> _Event= 0;
    } //fi
  } else {
    for (int i= NWM_Max_Layer; i >= 0; i--) {
      cur= _Head -> GetHead();
      while (cur) {
        if (cur -> Window -> _Visible && cur -> Window -> _Layer == i) {
          if (NGUtils::Inside(x, y, cur -> Window -> _PosX, cur -> Window -> _PosY, cur -> Window -> _SizX, cur -> Window -> _SizY)) {

            // Window was found, check to see if it's already active
            if (_lActiveWindow != cur -> Window -> _ID) {
              // Set Cursor to INHERIT and let Window overide if neccessary
              Cursors::SetCursor(CURSOR_ARROW);
              if (cur -> Window -> _Type != NWM_WT_Handle) _ActiveWindow= cur -> Window -> _ID;

              if (!_MenuLock)
                if (_Head -> GetWindowPtr(_lActiveWindow))
                  _Head -> GetWindowPtr(_lActiveWindow) -> Window -> Active(0);

              _lActiveWindow= cur -> Window -> _ID;

              cur -> Window -> Active(1);
            } //fi

            if (cur -> Window -> _Event || _UpdateOff || cur -> Window -> _Type == NWM_WT_Menu) {
              cur -> Window -> Passive(x - cur -> Window -> _PosX, y - cur -> Window -> _PosY);
              cur -> Window -> _Event= 0;
            } //fi
            i= -1;
            break;  // Window was found, no use testing against others
          } //fi
        } //fi

        cur= _Head -> GetWindowPtr(_Head -> GetNext(cur -> Window -> _ID));
      } //eow
    } //eof
  } //fi
} //eof NWM::PassiveMotion()


void NWM::CreateMenu(unsigned char MenuType) { 
  if (!_MenuLock) {
    _MenuLock= 1;
    new NWM_Menu(_MouseX,_MouseY,_Width,_Height,_Head,MenuType,_ObjectList,_self,_MaterialList,_RT);
  } //fi
} //eof NWM::CreateMenu()


void NWM::UpdateType(unsigned char WT) {
  NWM_List::WindowNode	*cur= NULL;
  
  cur= _Head -> GetHead();
  while (cur) {
    if (cur -> Window -> _Type == WT)
      cur -> Window -> _Update= 2;
    cur= _Head -> GetWindowPtr(_Head -> GetNext(cur -> Window -> _ID));
  } //eow
} //eof NWM::UpdateType()


// +-------------------+
// | PRIVATE FUNCTIONS |
// +-------------------+

void NWM::DefaultLayout() {
  // Purge Existing List
  _Head -> PurgeList();

  // Define Handles, and assign handles to Windows
  // Top
  new NWM_Handle(80,-4,-2,_Width,_Height,_Head,_self,1,1);
  _StaticHandle[0]= _Head -> GetNewWindowID();
  // Bottom
  new NWM_Handle(_Height-34-NWM_Handle_Thickness,-4,-2,_Width,_Height,_Head,_self,1,1);
  _StaticHandle[1]= _Head -> GetNewWindowID();
  // Right
  new NWM_Handle(_Width-52-NWM_Handle_Thickness,_StaticHandle[0],_StaticHandle[1],_Width,_Height,_Head,_self,0,1);
  _StaticHandle[2]= _Head -> GetNewWindowID();

  new NWM_Panel_Primitives(-4,-2,-1,_StaticHandle[0],_Head,_self,_ObjectList);
  _StaticSubwindow[0]= _Head -> GetNewWindowID();
  new NWM_Selector(_StaticHandle[2],-2,_StaticHandle[0],_StaticHandle[1],_Head,_self,_ObjectList,_Head -> GetNewWindowID());
  _StaticSubwindow[1]= _Head -> GetNewWindowID();
  new NWM_StatusBar(-4,-2,_StaticHandle[1],-3,_Head,_self,_ObjectList);
  _StaticSubwindow[2]= _Head -> GetNewWindowID();
  new NWM_ModelView(-4,_StaticHandle[2],_StaticHandle[0],_StaticHandle[1],_Head,_self,_ObjectList);

  CalcWinGeom(0);
} //eof NWM::DefaultLayout()


void NWM::SelfDestructCheck() {
  NWM_List::WindowNode	*cur= NULL;
  bool			flag= 0;

  cur= _Head -> GetHead();
  while (cur) {
    if (cur -> Window -> _SelfDestruct) {
      if (cur -> Window -> _Replacement) RemoveSubstitute(cur -> Window -> _ID);
      _Head -> RemoveNode(cur -> Window -> _ID);
      flag= 1;
      // Remove the MenuLock
      _MenuLock= 0;
      break;
    } //fi
    cur= _Head -> GetWindowPtr(_Head -> GetNext(cur -> Window -> _ID));
  } //eow

  if (flag) SelfDestructCheck();
} //eof NWM::SelfDestructCheck()


void NWM::ProcessResponse(char R, int WID) {
  // Process the reponse from an Event Function
  // and Perform/Set instructions received

  if (R & NWM_REFRESH)
  { RefreshAll(); }

  if (R & NWM_LOCK)
  { _LOCK= NWM_ON; _FocusID= WID; }

  if (R & NWM_UNLOCK)
  { _LOCK= NWM_OFF; _UpdateOff= NWM_OFF; }

  if (R & NWM_UPDATEOFF)
  { _UpdateOff= NWM_ON; }

} //eof NWM::ProcessResponse()


void NWM::RefreshAll() {
  NWM_List::WindowNode	*cur= NULL;

  cur= _Head -> GetHead();
  while (cur) {
    // If Window V?
    cur -> Window -> Refresh();
    cur -> Window -> _Update= 2;
    cur= _Head -> GetWindowPtr(_Head -> GetNext(cur -> Window -> _ID));
  } //eow
} //eof NWM::RefreshAll()


void NWM::RefreshType(unsigned char WT) {
  NWM_List::WindowNode	*cur= NULL;

  cur= _Head -> GetHead();
  while (cur) {
    if (cur -> Window -> _Type == WT) {
      cur -> Window -> Refresh();
      cur -> Window -> _Update= 2;
    } //fi
    cur= _Head -> GetWindowPtr(_Head -> GetNext(cur -> Window -> _ID));
  } //eow
} //eof NWM::RefreshType()


int NWM::Replace(int ID, unsigned char WT) {
  NWM_List::WindowNode	*win1= NULL;
  NWM_List::WindowNode	*win2= NULL;

  CreateNWindow(WT);
  // Update Static Subwindow if being replaced
  if (ID == _StaticSubwindow[0])
    _StaticSubwindow[0]= _Head -> GetNewWindowID();
  win1= _Head -> GetWindowPtr(_Head -> GetNewWindowID());
  win2= _Head -> GetWindowPtr(ID);

  win1 -> Window -> _H1= win2 -> Window -> _H1;
  win1 -> Window -> _H2= win2 -> Window -> _H2;
  win1 -> Window -> _H3= win2 -> Window -> _H3;
  win1 -> Window -> _H4= win2 -> Window -> _H4;

  win2 -> Window -> _SelfDestruct= NWM_ON;
  RefreshAll();
  CalcWinGeom(1);
  return(_Head -> GetNewWindowID());
} //eof NWM::Replace()


void NWM::Substitute(int x, int y, char Type) {
  NWM_List::WindowNode	*cur= NULL;
  NWM_List::WindowNode	*win= NULL;


  cur= _Head -> GetHead();
  while (cur) {
    if (NGUtils::Inside(x, y, cur -> Window -> _PosX, cur -> Window -> _PosY, cur -> Window -> _SizX, cur -> Window -> _SizY)) {
      if (cur -> Window -> _Type != Type) {
        cur -> Window -> _SelfDestruct= NWM_ON;

        CreateNWindow(Type);
        win= _Head -> GetWindowPtr(_Head -> GetNewWindowID());

        if (cur -> Window -> _Replacement) {
          win -> Window -> _Replacement= cur -> Window -> _Replacement;
        } else {
          win -> Window -> _Replacement= cur -> Window -> _Type;
        } //fi

        // Copy Handles
        win -> Window -> _H1= cur -> Window -> _H1;
        win -> Window -> _H2= cur -> Window -> _H2;
        win -> Window -> _H3= cur -> Window -> _H3;
        win -> Window -> _H4= cur -> Window -> _H4;
      } //fi

      break;
    } //fi
    cur= _Head -> GetWindowPtr(_Head -> GetNext(cur -> Window -> _ID));
  } //eow

  RefreshAll();
  CalcWinGeom(1);
} //eof NWM::Substitute()


void NWM::RemoveSubstitute(int WID) {
  NWM_List::WindowNode	*cur= NULL;
  NWM_List::WindowNode	*win= NULL;

  _LOCK= 0;
  cur= _Head -> GetWindowPtr(WID);
  if (cur) {
    CreateNWindow(cur -> Window -> _Replacement);
    win= _Head -> GetWindowPtr(_Head -> GetNewWindowID());

    // Copy Handles
    win -> Window -> _H1= cur -> Window -> _H1;
    win -> Window -> _H2= cur -> Window -> _H2;
    win -> Window -> _H3= cur -> Window -> _H3;
    win -> Window -> _H4= cur -> Window -> _H4;
  } //fi
  CalcWinGeom(0);
} //eof NWM::RemoveSubstitute()


void NWM::CreateNWindow(char Type) {
  switch( Type ) {
    case NWM_WT_ModelView:
      new NWM_ModelView(0,0,1,1,_Head,_self,_ObjectList);
      break;

    case NWM_WT_Panel_Construction:
      new NWM_Panel_Construction(0,0,1,1,_Head,_self,_ObjectList);
      break;

    case NWM_WT_Panel_Edit:
      new NWM_Panel_Edit(0,0,1,1,_Head,_self,_ObjectList);
      break;

    case NWM_WT_Panel_Materials:
      new NWM_Panel_Materials(0,0,1,1,_Head,_self,_ObjectList,_MaterialList);
      break;

    case NWM_WT_Panel_Primitives:
      new NWM_Panel_Primitives(0,0,1,1,_Head,_self,_ObjectList);
      break;

    case NWM_WT_Panel_Render:
      new NWM_Panel_Render(0,0,1,1,_Head,_self,_ObjectList,_RT);
      break;

    case NWM_WT_Panel_Settings:
      new NWM_Panel_Settings(0,0,1,1,_Head,_self,_ObjectList);
      break;

    case NWM_WT_FileManager_Load:
      new NWM_FileManager(0,0,1,1,_self,_Head,_ObjectList,_MaterialList,0,_Path);
      break;

    case NWM_WT_FileManager_Save:
      new NWM_FileManager(0,0,1,1,_self,_Head,_ObjectList,_MaterialList,1,_Path);
      break;

    case NWM_WT_SceneNotes:
      new NWM_SceneNotes(0,0,1,1,_self,_Head,_ObjectList);
      break;

    default:
      break;
  } //eos
} //eof NWM::CreateNWindow()


void NWM::CalcWinGeom(bool SD) {
  // Cycle through each window and compute it's relative
  // Position and Size given it's Handle counterparts
  // Calculate Handles first, then others
  NWM_List::WindowNode	*cur= NULL;

  // NWM_Handle needs the self destruct check
  if (SD) SelfDestructCheck();

  // Update Handles First
  cur= _Head -> GetHead();
  while (cur) {
    if (cur -> Window -> _Type == NWM_WT_Handle)
      cur -> Window -> Reshape(_Width,_Height);
    cur -> Window -> _Update= 2;
    cur= _Head -> GetWindowPtr(_Head -> GetNext(cur -> Window -> _ID));
  } //eow


  // Now update Windows
  cur= _Head -> GetHead();
  while (cur) {
    if (cur -> Window -> _Type != NWM_WT_Handle) {
      cur -> Window -> _PosX= GetAttribute(cur -> Window -> _H1,1);
      cur -> Window -> _PosY= GetAttribute(cur -> Window -> _H3,3);
      cur -> Window -> _SizX= GetAttribute(cur -> Window -> _H2,2) - GetAttribute(cur -> Window -> _H1,1);
      cur -> Window -> _SizY= GetAttribute(cur -> Window -> _H4,4) - GetAttribute(cur -> Window -> _H3,3);

      cur -> Window -> Reshape(_Width,_Height);
    } //fi
    cur -> Window -> _Update= 2;
    cur= _Head -> GetWindowPtr(_Head -> GetNext(cur -> Window -> _ID));
  } //eow
} //eof NWM::CalcWinGeom()


int NWM::GetAttribute(int ID, int H) {
  NWM_List::WindowNode	*cur= NULL;

  switch( ID ) {
    case -1:
      return(0);
      break;

    case -2:
      return(_Width);
      break;

    case -3:
      return(_Height);
      break;

    case -4:
      return(0);
      break;

    default:
      cur= _Head -> GetWindowPtr(ID);
      switch( H ) {
        case 1:
          return(cur -> Window -> _PosX + NWM_Handle_Thickness);
          break;

        case 2:
          return(cur -> Window -> _PosX + cur -> Window -> _SizX - NWM_Handle_Thickness);
          break;

        case 3:
          return(cur -> Window -> _PosY + NWM_Handle_Thickness);
          break;

        case 4:
          return(cur -> Window -> _PosY + cur -> Window -> _SizY - NWM_Handle_Thickness);
          break;

        default:
          break;

      } //eos
      break;

  } //eos

  return(0);
} //eof NWM::GetAttribute()


void NWM::ToggleStaticMenus() {
  NWM_List::WindowNode	*cur= NULL;

  if ((_StaticMenus= !_StaticMenus)) {
    // Assign Top,Right,Bottom Handle IDs of Subwindows former Handle IDs
    cur= _Head -> GetHead();
    while (cur) {
      if (cur -> Window -> _Visible) {
        if (cur -> Window -> _H3 == -1)
          cur -> Window -> _H3= _StaticHandle[0];
        if (cur -> Window -> _H4 == -3)
          cur -> Window -> _H4= _StaticHandle[1];
        if (cur -> Window -> _H2 == -2)
          cur -> Window -> _H2= _StaticHandle[2];
      } //fi
      cur= _Head -> GetWindowPtr(_Head -> GetNext(cur -> Window -> _ID));
    } //eow

    // UnHide Static Subwindows and Static Handles
    for (int i= 0; i < 3; i++) {
      _Head -> GetWindowPtr(_StaticSubwindow[i]) -> Window -> _Visible= 1;
      _Head -> GetWindowPtr(_StaticHandle[i]) -> Window -> _Visible= 1;
    } //eof
  } else {
    // Hide Static Subwindows and Static Handles
    for (int i= 0; i < 3; i++) {
      _Head -> GetWindowPtr(_StaticSubwindow[i]) -> Window -> _Visible= 0;
      _Head -> GetWindowPtr(_StaticHandle[i]) -> Window -> _Visible= 0;
    } //eof

    // Assign Top,Right,Bottom Handle IDs of Subwindows from Handle IDs to Window
    cur= _Head -> GetHead();
    while (cur) {
      if (cur -> Window -> _Visible) {
        if (cur -> Window -> _H3 == _StaticHandle[0])
          cur -> Window -> _H3= -1;
        if (cur -> Window -> _H4 == _StaticHandle[1])
          cur -> Window -> _H4= -3;
        if (cur -> Window -> _H2 == _StaticHandle[2])
          cur -> Window -> _H2= -2;
      } //fi
      cur= _Head -> GetWindowPtr(_Head -> GetNext(cur -> Window -> _ID));
    } //eow
  } //fi

  // Recalculate Geometry for Remaining Subwindows and Refresh
  CalcWinGeom(0);
  RefreshAll();
} //eof NWM::ToggleStaticMenus()
