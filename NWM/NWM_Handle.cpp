#include "NWM_Handle.h"


NWM_Handle::NWM_Handle(int Pos, int A, int B, int W, int H, NWM_List *Head, NWM *NWMptr, bool VH, bool Anchored) : NWM_Base(Head,NWM_WT_Handle) {
  // if VH == 1, it's horizontal, and Pos applies to (Y), otherwise it's (X)
  _NWM= NWMptr;
  _Head= Head;

  // AB is L->R or T->B
  _VH= VH;
  _Anchored= Anchored;

  _H1= A;
  _H2= B;
  _H3= W;
  _H4= H;

  if (VH) {
    _PosX= GetAttribute(A,1); // (A) -> PosX
    _PosY= Pos;
  } else {
    _PosX= Pos;
    _PosY= GetAttribute(A,3); // (A) -> PosY
  } //fi

  if (VH) {
    _SizX= GetAttribute(B,1) - GetAttribute(A,1);	// (B) -> PosX - (A) -> PosX
    _SizY= NWM_Handle_Thickness;	// Static Thickness (3)
  } else {
    _SizX= NWM_Handle_Thickness;	// Static Thickness (3)
    _SizY= GetAttribute(B,3) - GetAttribute(A,3);	// (B) -> PosY - (A) -> PosY
  } //fi

  // Construct Handle Viewport
  _Viewport[0]= _PosX;
  _Viewport[1]= H-(_PosY+_SizY);
  _Viewport[2]= _SizX;
  _Viewport[3]= _SizY;
} //eof NWM_Handle::NWM_Handle()


NWM_Handle::~NWM_Handle() {

} //eof NWM_Handle::~NWM_Handle()


void NWM_Handle::Update() {
  int		i;

  glEnable(GL_SCISSOR_TEST);
    glViewport(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
    glScissor(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);

    // Configure View
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,_Viewport[2],_Viewport[3],0,-1.0,1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    glBegin(GL_LINES);
    if (_VH) {
      for (i= 1; i <= _Viewport[3]; i++) {
        glColor3ub(224/i,224/i,232/i);
        glVertex2i(0,i);
        glVertex2i(_Viewport[2],i);
      } //eof
    } else {
      for (i= 1; i <= _Viewport[2]; i++) {
        glColor3ub(224/i,224/i,232/i);
        glVertex2i(i-1,_Viewport[3]);
        glVertex2i(i-1,0);
      } //eof
    } //fi
    glEnd();

  glDisable(GL_SCISSOR_TEST);
} //eof NWM_Handle::Update()


void NWM_Handle::Refresh() {
} //eof NWM_Handle::Refresh()


void NWM_Handle::Reshape(int w, int h) {
  if (_VH) {
    _PosX= GetAttribute(_H1,1); // (A) -> PosX
    _SizX= GetAttribute(_H2,1) - GetAttribute(_H1,1);	// (B) -> PosX - (A) -> PosX
  } else {
    _PosY= GetAttribute(_H1,3); // (A) -> PosY
    _SizY= GetAttribute(_H2,3) - GetAttribute(_H1,3);	// (B) -> PosY - (A) -> PosY
  } //fi

  _H3= w;
  _H4= h;


  // Construct Handle Viewport
  _Viewport[0]= _PosX;
  _Viewport[1]= h-(_PosY+_SizY);
  _Viewport[2]= _SizX;
  _Viewport[3]= _SizY;
} //eof NWM_Handle::Reshape()


char NWM_Handle::Key(int x, int y) {
  return(NWM_NOTHING);
} //eof NWM_Handle::Key()


char NWM_Handle::Mouse(int button, int state, int x, int y) {
  // If Mouse Click, Turn updating off, and begin drawing
  // the inverted buffer whose location determined by ::Passive

//  glutSetCursor(GLUT_CURSOR_INHERIT);
  if (_Anchored && (button == SDL_BUTTON_LEFT || button == SDL_BUTTON_MIDDLE)) return(NWM_NOTHING);

  Cursors::SetCursor(CURSOR_ARROW);
  switch( button ) {
    // Moving a Handle
    case SDL_BUTTON_LEFT:
      {
        NWM_List::WindowNode	*cur= NULL;
        int	HIDA,HIDB;
        bool	AB= 0;

        if (state) {
          // Initialize nwmu_Handle for FB inverting
          if (_VH) {
            nwmu_Handle::Initialize(0,0,_H3,_H4,_PosX,_SizX,_VH);
          } else {
            nwmu_Handle::Initialize(0,0,_H3,_H4,_PosY,_SizY,_VH);
          } //fi
          return(NWM_LOCK | NWM_UPDATEOFF);
        } else if (!state) {
          // Recalculate Window Geometry
          HIDA= GetNLH(0,0);
          HIDB= GetNLH(1,0);

          // Get the ID windows A and B and make sure we meet the
          // size restrictions they have defined.  Also make sure
          // that the slider doesn't go past another of it's _VH type.

          if (_VH) {
            // Go through each non NWT_WT_Handle window type and those
            // that have _H4 has _ID and _H3 as HIDA, makre sure _MinSizY
            // restriction is okay.
            bool	exit= 1;

            cur= _Head -> GetHead();
            while (cur && exit) {
              if (cur -> Window -> _Type != NWM_WT_Handle) {

                if (y < _PosY) {
                  if ((cur -> Window -> _H4 == _ID) && (cur -> Window -> _H3 == HIDA)) {
                    if ((y-GetAttribute(HIDA,3)) > (cur -> Window -> _MinSizY)) {
                      _PosY= y;
                      exit= 0;
                    } else {
                      _PosY= GetAttribute(HIDA,3) + cur -> Window -> _MinSizY;
                      if (HIDA > 0) _PosY+= NWM_Handle_Width;
                      exit= 0;
                    } //fi
                  } //fi
                } else {
                  if ((cur -> Window -> _H3 == _ID) && (cur -> Window -> _H4 == HIDB)) {
                    if ((GetAttribute(HIDB,4) - y) > (cur -> Window -> _MinSizY)) {
                      _PosY= y;
                      exit= 0;
                    } else {
                      _PosY= GetAttribute(HIDB,4) - cur -> Window -> _MinSizY;
                      if (HIDB >= 0) _PosY-= 2*NWM_Handle_Width;
                      exit= 0;
                    } //fi
                  } //fi
                } //fi

              } //fi
              cur= _Head -> GetWindowPtr(_Head -> GetNext(cur -> Window -> _ID));
            } //eow
          } else {
            bool	exit= 1;

            cur= _Head -> GetHead();
            while (cur && exit) {
              if (cur -> Window -> _Type != NWM_WT_Handle) {

                if (x > _PosX) {
                  if ((cur -> Window -> _H1 == _ID) && (cur -> Window -> _H2 == HIDB)) {
                    if ((GetAttribute(HIDB,2) - x) > (cur -> Window -> _MinSizX)) {
                      _PosX= x;
                      exit= 0;
                    } else {
                      _PosX= GetAttribute(HIDB,2) - cur -> Window -> _MinSizX;
                      if (HIDB > 0) _PosX-= 2*NWM_Handle_Width;
                      exit= 0;
                    } //fi
                  } //fi
                } else {
                  if ((cur -> Window -> _H2 == _ID) && (cur -> Window -> _H1 == HIDA)) {
                    if ((x - GetAttribute(HIDA,1)) > (cur -> Window -> _MinSizX)) {
                      _PosX= x;
                      exit= 0;
                    } else {
                      _PosX= GetAttribute(HIDA,1) + cur -> Window -> _MinSizX;
                      if (HIDA > 0) _PosX+= NWM_Handle_Width;
                      exit= 0;
                    } //fi
                  } //fi
                } //fi

              } //fi
              cur= _Head -> GetWindowPtr(_Head -> GetNext(cur -> Window -> _ID));
            } //eow
          } //fi

          // Reconstruct Viewport
          _Viewport[0]= _PosX;
          _Viewport[1]= _H4-(_PosY+_SizY);
          _Viewport[2]= _SizX;
          _Viewport[3]= _SizY;

          _NWM -> CalcWinGeom(1);
          return(NWM_UNLOCK | NWM_REFRESH);
        } //fi
      } //eoc
      break;


    // Remove a Handle, Join Windows
    case SDL_BUTTON_MIDDLE:
      {
        if (state) {
          // 1) Calculate a virtual box that encompasses the window opposite
          // the one the cursor was last in.  Once that is complete, self destruct
          // all windows that lie within that region.
          // 2) Update any handles that are in a state of disconnection
          // 3) recalculate the Window Geometry and update all windows
          NWM_List::WindowNode		*cur= NULL;
          int				BpX, BpY, BsX, BsY, HID;
          char				LRTB;
          bool				AB= 0;

          cur= _Head -> GetWindowPtr(_NWM -> _ActiveWindow);
          // Determine whether to send A or B to GetNLH
          if (_VH) {
            // Check _PosY
            if (cur -> Window -> _PosY < _PosY) AB= 1;
          } else {
            // Check _PosX
            if (cur -> Window -> _PosX < _PosX) AB= 1;
          } //fi

          // Get Handle ID for Box Test
          HID= GetNLH(AB,1);

          // Generate Bounding Box
          if (_VH) {	// Horizontal
            if (AB) {
              BpX= _PosX;
              BpY= _PosY;
              BsX= _SizX;
              BsY= GetAttribute(HID,4) - _PosY;
            } else {
              BpX= _PosX;
              BpY= GetAttribute(HID,3);
              BsX= _SizX;
              BsY= _PosY - GetAttribute(HID,3);
            } //fi
          } else {	// Vertical
            if (AB) {
              BpX= _PosX;
              BpY= _PosY;
              BsX= GetAttribute(HID,1) - _PosX;
              BsY= _SizY;
            } else {
              BpX= GetAttribute(HID,1);
              BpY= _PosY;
              BsX= _PosX - GetAttribute(HID,1);
              BsY= _SizY;
            } //fi
          } //fi

          // Remove Everything via Box Test
          cur= _Head -> GetHead();
          while (cur) {
            if (NGUtils::BoxInside(cur -> Window -> _PosX, cur -> Window -> _PosY, cur -> Window -> _SizX, cur -> Window -> _SizY,
                        BpX, BpY, BsX, BsY)) {
              // Make sure we don't destroy one of the Box Borders!!!
                if (cur -> Window -> _Type == NWM_WT_Handle) {
                  if (_VH) {
                    // Check Vertical Ones
                    if ((cur -> Window -> _PosX != BpX) && (cur -> Window -> _PosX != BpX+BsX))
                      cur -> Window -> _SelfDestruct= 1;
                  } else {
                    // Check Horizontal Ones
                    if ((cur -> Window -> _PosY != BpY) && (cur -> Window -> _PosY != BpY+BsY))
                      cur -> Window -> _SelfDestruct= 1;
                  } //fi
                } else {
                  cur -> Window -> _SelfDestruct= 1;
                } //fi
            } //fi
            cur= _Head -> GetWindowPtr(_Head -> GetNext(cur -> Window -> _ID));
          } //eow

          // Reconnect Everything
          cur= _Head -> GetHead();
          while (cur) {
            if (_VH) {
              if (AB) {
                // Remember, Handles and Other Windows use _H differently
                if (cur -> Window -> _Type == NWM_WT_Handle) {
                  if (cur -> Window -> _H2 == _ID) cur -> Window -> _H2= HID;
                } else {
                  if (cur -> Window -> _H4 == _ID) cur -> Window -> _H4= HID;
                } //fi
              } else {
                if (cur -> Window -> _Type == NWM_WT_Handle) {
                  if (cur -> Window -> _H1 == _ID) cur -> Window -> _H1= HID;
                } else {
                  if (cur -> Window -> _H3 == _ID) cur -> Window -> _H3= HID;
                } //fi
              } //fi
            } else {
              if (AB) {
                if (cur -> Window -> _H2 == _ID) cur -> Window -> _H2= HID;
              } else {
                if (cur -> Window -> _H1 == _ID) cur -> Window -> _H1= HID;
              } //fi
            } //fi
            cur= _Head -> GetWindowPtr(_Head -> GetNext(cur -> Window -> _ID));
          } //eow

          // Mark for Self Destruction;
          _SelfDestruct= NWM_ON;

          // Recalculate Window Geometry
          _NWM -> CalcWinGeom(1);
        } //fi
      } //eoc
      break;


    // Add a Handle, Split a Window
    case SDL_BUTTON_RIGHT:
      {
        if (state) {
          NWM_List::WindowNode		*cur= NULL;
          NWM_List::WindowNode		*win= NULL;
          int				ID1;

          // 3 Windows will get created, Original one will Self Destruct
          cur= _Head -> GetWindowPtr(_NWM -> _ActiveWindow);
          if (!cur -> Window -> _Splitable) return(NWM_NOTHING);
          cur -> Window -> _SelfDestruct= NWM_ON;

          if (_VH) {
            // Create a Vertical Handle
//            new NWM_Handle((cur -> Window -> _PosX + cur -> Window -> _SizX)/2,cur -> Window -> _H3, cur -> Window -> _H4,_H3,_H4,_Head,_NWM,0);
            new NWM_Handle(x,cur -> Window -> _H3, cur -> Window -> _H4,_H3,_H4,_Head,_NWM,0,0);
            ID1= _Head -> GetNewWindowID();
            // Now Create two new Windows

            _NWM -> CreateNWindow(cur -> Window -> _Type);
            win= _Head -> GetWindowPtr(_Head -> GetNewWindowID());
            win -> Window -> _H1= cur -> Window -> _H1;
            win -> Window -> _H2= ID1;
            win -> Window -> _H3= cur -> Window -> _H3;
            win -> Window -> _H4= cur -> Window -> _H4;

            _NWM -> CreateNWindow(cur -> Window -> _Type);
            win= _Head -> GetWindowPtr(_Head -> GetNewWindowID());
            win -> Window -> _H1= ID1;
            win -> Window -> _H2= cur -> Window -> _H2;
            win -> Window -> _H3= cur -> Window -> _H3;
            win -> Window -> _H4= cur -> Window -> _H4;

          } else {

            // Create a Horizontal Handle
//            new NWM_Handle((cur -> Window -> _PosY + cur -> Window -> _SizY)/2,cur -> Window -> _H1, cur -> Window -> _H2,_H3,_H4,_Head,_NWM,1);
            new NWM_Handle(y,cur -> Window -> _H1, cur -> Window -> _H2,_H3,_H4,_Head,_NWM,1,0);
            ID1= _Head -> GetNewWindowID();
            // Now Create two new Windows

            _NWM -> CreateNWindow(cur -> Window -> _Type);
            win= _Head -> GetWindowPtr(_Head -> GetNewWindowID());
            win -> Window -> _H1= cur -> Window -> _H1;
            win -> Window -> _H2= cur -> Window -> _H2;
            win -> Window -> _H3= cur -> Window -> _H3;
            win -> Window -> _H4= ID1;

            _NWM -> CreateNWindow(cur -> Window -> _Type);
            win= _Head -> GetWindowPtr(_Head -> GetNewWindowID());
            win -> Window -> _H1= cur -> Window -> _H1;
            win -> Window -> _H2= cur -> Window -> _H2;
            win -> Window -> _H3= ID1;
            win -> Window -> _H4= cur -> Window -> _H4;
          } //fi

          // Recompute Window Geometry for new Windows
          _NWM -> CalcWinGeom(1);
        } //fi
      } //eoc
      break;

    default:
      break;
  } //eos


  if (!state) {
    return(NWM_UNLOCK | NWM_REFRESH);
  } //fi
} //eof NWM_Handle::Mouse()


char NWM_Handle::Motion(int x, int y) {
  nwmu_Handle::Update(x,y);
  return(NWM_NOTHING);
} //eof NWM_Handle::Motion()


char NWM_Handle::Passive(int x, int y) {  
  return(NWM_NOTHING);
} //eof NWM_Handle::Passive()


void NWM_Handle::Active(bool A) {
  if (A) {
    if (_VH) {
      Cursors::SetCursor(CURSOR_UPDOWN);
//      glutSetCursor(GLUT_CURSOR_UP_DOWN);
    } else {
      Cursors::SetCursor(CURSOR_LEFTRIGHT);
//      glutSetCursor(GLUT_CURSOR_LEFT_RIGHT);
    } //fi
  } else {
    Cursors::SetCursor(CURSOR_ARROW);
//    glutSetCursor(GLUT_CURSOR_INHERIT);
  } //fi
} //eof NWM_Handle::Active()


int NWM_Handle::GetAttribute(int ID, int H) {
  NWM_List::WindowNode	*cur= NULL;


  switch( ID ) {
    case -1:
      return(0);
      break;

    case -2:
      return(_H3);
      break;

    case -3:
      return(_H4);
      break;

    case -4:
      return(0);
      break;

    default:
      cur= _Head -> GetWindowPtr(ID);
      if (cur) {
        switch( H ) {
          case 1:
            return(cur -> Window -> _PosX);
            break;

          case 2:
            return(cur -> Window -> _PosX + cur -> Window -> _SizX);
            break;

          case 3:
            return(cur -> Window -> _PosY);
            break;

          case 4:
            return(cur -> Window -> _PosY + cur -> Window -> _SizY);
            break;

          default:
            break;

        } //eos
      } //fi
      break;

  } //eos

  return(0);
} //eof NWM_Handle::GetAttribute()


int NWM_Handle::GetNLH(bool AB, bool PS) {
  // Return the Nearest Larger Handle, for specifically the position
  // must be equal or less than this handle AND the pos+size must be
  // equal to or greater than this handle
  // Note: PS off means we don't about it's position or size
  NWM_List::WindowNode	*cur= NULL;
  bool			Init= 0;
  int			Found= 0;
  int			Dist;


  if (_VH) {	// Horizontal
    // A is Above, and B is Below

    cur= _Head -> GetHead();
    while (cur) {
      if (cur -> Window -> _Type == NWM_WT_Handle) {
        if (AB) {	// (B) Looking for _PosY >

          if (cur -> Window -> _PosY > _PosY) {
            if (PS) {
              if ((cur -> Window -> _PosX <= _PosX) && (cur -> Window -> _SizX >= _SizX)) {	// Make sure if Overlaps
                // We have a candidate
                if (Init) {
                  // Check to see if it's closer than the one that already exists
                  if (cur -> Window -> _PosY < Dist) {
                    Dist= cur -> Window -> _PosY;
                    Found= cur -> Window -> _ID;
                  } //fi
                } else {
                  Init= 1;
                  Dist= cur -> Window -> _PosY;
                  Found= cur -> Window -> _ID;
                } //fi
              } //fi
            } else {
              // We have a candidate
              if (cur -> Window -> VH()) {
                if (Init) {
                  // Check to see if it's closer than the one that already exists
                  if (cur -> Window -> _PosY < Dist) {
                    Dist= cur -> Window -> _PosY;
                    Found= cur -> Window -> _ID;
                  } //fi
                } else {
                  Init= 1;
                  Dist= cur -> Window -> _PosY;
                  Found= cur -> Window -> _ID;
                } //fi
              } //fi
            } //fi
          } //fi

        } else {	// (A) Looking for _PosY <

          if (cur -> Window -> _PosY < _PosY) {
            if (PS) {
              if ((cur -> Window -> _PosX <= _PosX) && (cur -> Window -> _SizX >= _SizX)) {	// Make sure if Overlaps
                // We have a candidate
                if (Init) {
                  // Check to see if it's closer than the one that already exists
                  if (cur -> Window -> _PosY > Dist) {
                    Dist= cur -> Window -> _PosY;
                    Found= cur -> Window -> _ID;
                  } //fi
                } else {
                  Init= 1;
                  Dist= cur -> Window -> _PosY;
                  Found= cur -> Window -> _ID;
                } //fi
              } //fi
            } else {
              // We have a candidate
              if (cur -> Window -> VH()) {
                if (Init) {
                  // Check to see if it's closer than the one that already exists
                  if (cur -> Window -> _PosY > Dist) {
                    Dist= cur -> Window -> _PosY;
                    Found= cur -> Window -> _ID;
                  } //fi
                } else {
                  Init= 1;
                  Dist= cur -> Window -> _PosY;
                  Found= cur -> Window -> _ID;
                } //fi
              } //fi
            } //fi
          } //fi

        } //fi

      } //fi
      cur= _Head -> GetWindowPtr(_Head -> GetNext(cur -> Window -> _ID));
    } //eow

    if (Found) {	// Return the closest ID
      return(Found);
    } else {		// Resort to (-1 or -3) for virtual handle
      if (AB) {
        return(-3);
      } else {
        return(-1);
      } //fi
    } //fi

  } else {	// Vertical

    // A is Left, and B is Right

    cur= _Head -> GetHead();
    while (cur) {
      if (cur -> Window -> _Type == NWM_WT_Handle) {
        if (AB) {	// Looking for _PosX >

          if (cur -> Window -> _PosX > _PosX) {
            if (PS) {
              if ((cur -> Window -> _PosY <= _PosY) && (cur -> Window -> _SizY >= _SizY)) {	// Make sure if Overlaps
                // We have a candidate
                if (Init) {
                  // Check to see if it's closer than the one that already exists
                  if (cur -> Window -> _PosX < Dist) {
                    Dist= cur -> Window -> _PosX;
                    Found= cur -> Window -> _ID;
                  } //fi
                } else {
                  Init= 1;
                  Dist= cur -> Window -> _PosX;
                  Found= cur -> Window -> _ID;
                } //fi
              } //fi
            } else {
              if (!cur -> Window -> VH()) {
                if (Init) {
                  // Check to see if it's closer than the one that already exists
                  if (cur -> Window -> _PosX < Dist) {
                    Dist= cur -> Window -> _PosX;
                    Found= cur -> Window -> _ID;
                  } //fi
                } else {
                  Init= 1;
                  Dist= cur -> Window -> _PosX;
                  Found= cur -> Window -> _ID;
                } //fi
              } //fi
            } //fi
          } //fi

        } else {	// Looking for _PosX <
          
          if (cur -> Window -> _PosX < _PosX) {
            if (PS) {
              if ((cur -> Window -> _PosY <= _PosY) && (cur -> Window -> _SizY >= _SizY)) {	// Make sure if Overlaps
                // We have a candidate
                if (Init) {
                  // Check to see if it's closer than the one that already exists
                  if (cur -> Window -> _PosX > Dist) {
                    Dist= cur -> Window -> _PosX;
                    Found= cur -> Window -> _ID;
                  } //fi
                } else {
                  Init= 1;
                  Dist= cur -> Window -> _PosX;
                  Found= cur -> Window -> _ID;
                } //fi
              } //fi
            } else {
                // We have a candidate
              if (!cur -> Window -> VH()) {
                if (Init) {
                  // Check to see if it's closer than the one that already exists
                  if (cur -> Window -> _PosX > Dist) {
                    Dist= cur -> Window -> _PosX;
                    Found= cur -> Window -> _ID;
                  } //fi
                } else {
                  Init= 1;
                  Dist= cur -> Window -> _PosX;
                  Found= cur -> Window -> _ID;
                } //fi
              } //fi
            } //fi
          } //fi

        } //fi

      } //fi
      cur= _Head -> GetWindowPtr(_Head -> GetNext(cur -> Window -> _ID));
    } //eow

    if (Found) {	// Return the closest ID
      return(Found);
    } else {		// Resort to (-1 or -3) for virtual handle
      if (AB) {
        return(-2);
      } else {
        return(-4);
      } //fi
    } //fi
  } //fi
} //eof NWM_Handle::GetNLH()
