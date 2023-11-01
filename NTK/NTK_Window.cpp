#include "NTK_Window.h"


NTK_Window::NTK_Window(char *Path) {
  Head= NULL;
  _Font= new NTK_Text(Path);
  _Width= _Height= 1;
  _Clear= 1;
  _Scene= 1;
  _OfsetX= 0;
  _OfsetY= 0;
} //eof NTK_Window::NTK_Window()

NTK_Window::~NTK_Window() {
  WidgetNode	*wn;

  // Delete Font
  delete _Font;

  // Delete Widgets
  while (Head) {
    delete Head -> Widget;
    wn= Head -> Next;
    delete Head;
    Head= wn;
  } //eow
} //eof NTK_Window::~NTK_Window()


void NTK_Window::Push(NTK_Base *WidgetPtr) {
  //Add the widget to the list
  NTK_Window::WidgetNode	*cur;

  if (Head == NULL) {
    Head= new NTK_Window::WidgetNode;
    Head -> Next= NULL;
  } else {
    //Structured to create a LIFO stack
    cur= Head;
    Head= NULL;
    Head= new NTK_Window::WidgetNode;

    //Connect the list
    Head -> Next= cur;
  } //fi

  Head -> Widget= WidgetPtr;
} //eof NTK_Window::Push()


void NTK_Window::HideAll() {
  NTK_Window::WidgetNode *cur= NULL;

  if (Head) {
    cur= Head;
    while (cur != NULL) {
      cur -> Widget -> _State= 0;
      cur -> Widget -> _Visible= 0;
      cur= cur -> Next;
    } //eow
  } //fi
} //eof NTK_Window::HideAll()


void NTK_Window::UpdateAll() {
  NTK_Window::WidgetNode *cur= NULL;

  if (Head) {
    cur= Head;
    while (cur != NULL) {
      cur -> Widget -> _Update= 2;
      cur= cur -> Next;
    } //eow
  } //fi
} //eof NTK_Window::UpdateAllUpdate()


void NTK_Window::ScreenSize(int W, int H) {
  NTK_Window::WidgetNode	*cur= NULL;
  float				sX,sY;
  char				DYNPOS;
  char				DYNSIZ;

  cur= Head;

  while (cur) {
    //
    // Check for Scaling
    //
    if (cur -> Widget -> _ScaleX) {
      sX= float(cur -> Widget -> _sX);
      sX*= float(W)/float(_Width);

      cur -> Widget -> _sX= int(sX);
      cur -> Widget -> _Update= 2;
    } //fi

    if (cur -> Widget -> _ScaleY) {
      sY= float(cur -> Widget -> _sY);
      sY*= float(H)/float(_Height);

      cur -> Widget -> _sY= int(sY);
      cur -> Widget -> _Update= 2;
    } //fi

    //
    // Check for Dynamic Positioning and Sizing
    //
    DYNPOS= cur -> Widget -> _DynPos;
    DYNSIZ= cur -> Widget -> _DynSiz;
    // Positioning
    if (DYNPOS & NTK_DYN_BOTTOM)
    { DYNPOS-= NTK_DYN_BOTTOM; cur -> Widget -> _pY= H - (cur -> Widget -> _DynPosY + cur -> Widget -> _sY); }

    if (DYNPOS & NTK_DYN_TOP)
    { DYNPOS-= NTK_DYN_TOP; cur -> Widget -> _pY= 0 + cur -> Widget -> _DynPosY; }

    if (DYNPOS & NTK_DYN_RIGHT)
    { DYNPOS-= NTK_DYN_RIGHT; cur -> Widget -> _pX= W - (cur -> Widget -> _DynPosX + cur -> Widget -> _sX); }

    if (DYNPOS & NTK_DYN_LEFT)
    { DYNPOS-= NTK_DYN_LEFT; cur -> Widget -> _pX= 0 + cur -> Widget -> _DynPosX; }

    // Sizing
    if (DYNSIZ & NTK_DYN_HEIGHT)
    { DYNSIZ-= NTK_DYN_HEIGHT; cur -> Widget -> _sY= H - (cur -> Widget -> _DynSizY + cur -> Widget -> _pY); }

    if (DYNSIZ & NTK_DYN_WIDTH)
    { DYNSIZ-= NTK_DYN_WIDTH; cur -> Widget -> _sX= W - (cur -> Widget -> _DynSizX + cur -> Widget -> _pX); }
    cur -> Widget -> Resize();
    cur= cur -> Next;
  } //eow

  _Width= W;
  _Height= H;
} //eof NTK_Window::ScreenSize()


void SceneSetup(int Width, int Height, int Clear) {
  glClearColor(NTK_CCRed,NTK_CCGreen,NTK_CCBlue,1);
  if (Clear) glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,Width,Height,0,-1.0,1.0);
 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
} //eof SceneSetup


void NTK_Window::Draw() {
  NTK_Window::WidgetNode	*cur= NULL;
  NTK_Window::WidgetNode	*tmp= NULL;


  if (Head) {
    if (_Clear) {
      _Clear--;
      if (_Scene) SceneSetup(_Width,_Height,1);
    } else {
      if (_Scene) SceneSetup(_Width,_Height,0);
    } //fi

    // Check to see if the container was recently changed, since this is layer and Update
    // independent, set all the widgets _IN_ that container to that same visibility, this causes
    // the widgets in that container to get an Update.
    cur= Head;
    while (cur) {
      if (cur -> Widget -> Container()) { // Must have container to apply
        // Cycle through all the Widgets and Update widgets if needed...
        if (cur -> Widget -> Visible() != NTK_Container::ContainerVisible(cur -> Widget -> _Container)) {
          cur -> Widget -> Visible(!cur -> Widget -> Visible());
        } //fi
      } //fi
      cur= cur -> Next;
    } //eow

    for (int i= 0; i < 256; i++) {  // 256 possible Layers
      cur= Head;
      while (cur) {
        // Widget needs Updating, is it in the current layer, and is its container visible?
        if ((cur -> Widget -> Visible()) && (cur -> Widget -> _Update) && (cur -> Widget -> _Layer == i) && (NTK_Container::ContainerVisible(cur -> Widget -> _Container))) {
          // Time to calculate the Container Displacement.  Tally up all the containers related to
          // the widget and ADD it to the Widgets Position.  Then, SUBTRACT the total upon drawing
          // completing its routine.

          NTK_Container::ContainerDisplacement(&cur -> Widget -> _pX, &cur -> Widget -> _pY, cur -> Widget -> _Container,1);

          // Add Ofset
          if (!cur -> Widget -> Anchored()) {
            cur -> Widget -> _pX+= _OfsetX;
            cur -> Widget -> _pY+= _OfsetY;
          } //fi

          // Draw Widget and decrement Update
          if (cur -> Widget -> _Visible)
            cur -> Widget -> Draw();
          cur -> Widget -> _Update--;
          if (cur -> Widget -> _CallbackTrigger & NTK_CT_UPDATE_COMPLETE)
            if (!cur -> Widget -> _Update)
              cur -> Widget -> Callback(NTK_EVENT_UPDATE_COMPLETE);

          // Check to see if Layers above need updating due to this THIS layer overlapping them
          tmp= Head;
          while (tmp) {
            if (tmp -> Widget -> _Layer > cur -> Widget -> _Layer) {
              // Before we check the 4 points of "tmp" widget if it lies inside of "cur" widget we
              // need to first apply the container displacement to "tmp" as done for "cur"
              NTK_Container::ContainerDisplacement(&tmp -> Widget -> _pX, &tmp -> Widget -> _pY, tmp -> Widget -> _Container,1);

              // Add Ofset
              if (!cur -> Widget -> Anchored()) {
                cur -> Widget -> _pX+= _OfsetX;
                cur -> Widget -> _pY+= _OfsetY;
              } //fi

              // Check to see if any of the 4 points of "tmp" widget lies inside of the "cur" widget
              // 1 tmp -> Widget -> _pX+_sX,_pY+_sY
              // 2 tmp -> Widget -> _pX+_sX,_pY
              // 3 tmp -> Widget -> _pX,_pY
              // 4 tmp -> Widget -> _pX,_pY+_sY
              if ((NGUtils::Inside(tmp -> Widget -> _pX+tmp -> Widget -> _sX,tmp -> Widget -> _pY+tmp -> Widget -> _sY,cur -> Widget -> _pX,cur -> Widget -> _pY,cur -> Widget -> _sX,cur -> Widget -> _sY)) &&
                  (NGUtils::Inside(tmp -> Widget -> _pX+tmp -> Widget -> _sX,tmp -> Widget -> _pY,cur -> Widget -> _pX,cur -> Widget -> _pY,cur -> Widget -> _sX,cur -> Widget -> _sY)) &&
                  (NGUtils::Inside(tmp -> Widget -> _pX,tmp -> Widget -> _pY,tmp -> Widget -> _pX,cur -> Widget -> _pY,cur -> Widget -> _sX,cur -> Widget -> _sY)) &&
                  (NGUtils::Inside(tmp -> Widget -> _pX,tmp -> Widget -> _pY+tmp -> Widget -> _sY,cur -> Widget -> _pX,cur -> Widget -> _pY,cur -> Widget -> _sX,cur -> Widget -> _sY))) {
                  tmp -> Widget -> _Update= 2;
              } //fi

            // Remove Ofset
            if (!cur -> Widget -> Anchored()) {
              cur -> Widget -> _pX-= _OfsetX;
              cur -> Widget -> _pY-= _OfsetY;
            } //fi

            //Remove Displacement
            NTK_Container::ContainerDisplacement(&tmp -> Widget -> _pX, &tmp -> Widget -> _pY, tmp -> Widget -> _Container,0);
            } //fi
            tmp= tmp -> Next;
          } //eow


          // Remove Ofset
          if (!cur -> Widget -> Anchored()) {
            cur -> Widget -> _pX-= _OfsetX;
            cur -> Widget -> _pY-= _OfsetY;
          } //fi
          //Remove Displacement
          NTK_Container::ContainerDisplacement(&cur -> Widget -> _pX, &cur -> Widget -> _pY, cur -> Widget -> _Container,0);
        } //fi
        cur= cur -> Next;
      } //eow
    } //eof

  } //fi
} //eof NTK_Window::Draw()


void NTK_Window::GenerateActWBB(int TLBR[4]) {
  NTK_Window::WidgetNode	*cur= NULL;
  bool				Init= 1;

  TLBR[0]= TLBR[1]= TLBR[2]= TLBR[3]= 0;
  if (Head) {
    cur= Head;
    while (cur) {
      if (cur -> Widget -> _Visible && !cur -> Widget -> _Anchored) {
        NTK_Container::ContainerDisplacement(&cur -> Widget -> _pX, &cur -> Widget -> _pY, cur -> Widget -> _Container,1);
        if (Init) {
          TLBR[0]= cur -> Widget -> _pX;
          TLBR[1]= cur -> Widget -> _pY;
          TLBR[2]= cur -> Widget -> _pX + cur -> Widget -> _sX;
          TLBR[3]= cur -> Widget -> _pY + cur -> Widget -> _sY;
          Init= 0;
        } else {
          if (cur -> Widget -> _pX < TLBR[0]) TLBR[0]= cur -> Widget -> _pX;
          if (cur -> Widget -> _pY < TLBR[1]) TLBR[1]= cur -> Widget -> _pY;
          if (cur -> Widget -> _pX+cur -> Widget -> _sX > TLBR[2]) TLBR[2]= cur -> Widget -> _pX + cur -> Widget -> _sX;
          if (cur -> Widget -> _pY+cur -> Widget -> _sY > TLBR[3]) TLBR[3]= cur -> Widget -> _pY + cur -> Widget -> _sY;
        } //fi
        NTK_Container::ContainerDisplacement(&cur -> Widget -> _pX, &cur -> Widget -> _pY, cur -> Widget -> _Container,0);
      } //fi
      cur= cur -> Next;
    } //eow
  } //fi
} //eof NTK_Window::GenerateActWBB()


void NTK_Window::Handle(char Event, int A, int B, int C, int D) {
  NTK_Window::WidgetNode	*cur= NULL;

  if (Head) {
    // Check to see if ofset should be adjusted, alt + any mouse button
    if (KeyTable::Key() == SDLK_LALT || KeyTable::Key() == SDLK_RALT) {
      if (Event == NTK_EVENT_MOUSE)
        if (B) {
          _Mouse[0]= C;
          _Mouse[1]= D;
        } //fi

      if (Event == NTK_EVENT_MOTION) {
        // Check to see that none of the widgets exit the screen, and that
        // Widgets marked as UnMovable, act as part of the boundary too
        int	TLBR[4];

        GenerateActWBB(TLBR);
        if ((_OfsetX+A-_Mouse[0] + TLBR[0] > 0) &&
            (_OfsetX+A-_Mouse[0] + TLBR[2] < _Width)) {
          _OfsetX+= A-_Mouse[0];
          _Mouse[0]= A;
        } else {
          // Over, bring it to Edge
          if (_OfsetX+A-_Mouse[0] + TLBR[0] < 0) _OfsetX= -TLBR[0];
          if (_OfsetX+A-_Mouse[0] + TLBR[2] > _Width) _OfsetX= _Width-TLBR[0]-(TLBR[2]-TLBR[0]);
          _Mouse[0]= A;
        } //fi
        if ((_OfsetY+B-_Mouse[1] + TLBR[1] > 0) &&
            (_OfsetY+B-_Mouse[1] + TLBR[3] < _Height)) {
          _OfsetY+= B-_Mouse[1];
          _Mouse[1]= B;
        } else {
          if (_OfsetY+B-_Mouse[1] + TLBR[1] < 0) _OfsetY= -TLBR[1];
          if (_OfsetY+B-_Mouse[1] + TLBR[3] > _Height) _OfsetY= _Height-TLBR[1]-(TLBR[3]-TLBR[1]);
          _Mouse[1]= B;
        } //fi

        _Clear= 1;
        UpdateAll();
      } //fi
    } else {

    cur= Head;
    while (cur) {
      if (NTK_Container::ContainerVisible(cur -> Widget -> _Container) && (cur -> Widget -> _Visible)) {
        switch( Event ) {
          case NTK_EVENT_MOUSE:
            //Check for Mouse down/up clicks within the Widgets area
            //A == Button, B == State, C == x, D == y
            NTK_Container::ContainerDisplacement(&cur -> Widget -> _pX, &cur -> Widget -> _pY, cur -> Widget -> _Container,1);

            // Add Ofset
            if (!cur -> Widget -> Anchored()) {
              cur -> Widget -> _pX+= _OfsetX;
              cur -> Widget -> _pY+= _OfsetY;
            } //fi

            if ((C > cur -> Widget -> _pX) && (C < (cur -> Widget -> _pX + cur -> Widget -> _sX)) &&
                (D > cur -> Widget -> _pY) && (D < (cur -> Widget -> _pY + cur -> Widget -> _sY))) {

                cur -> Widget -> Handle(Event, A, B, C, D);
              if (B) {
                if (cur -> Widget -> _CallbackTrigger & NTK_CT_MOUSE_DOWN) cur -> Widget -> Callback(NTK_EVENT_MOUSE_DOWN);
              } else {
                if (cur -> Widget -> _CallbackTrigger & NTK_CT_MOUSE_UP) cur -> Widget -> Callback(NTK_EVENT_MOUSE_UP);
              } //fi
            } //fi

            // Send every Widget the Up State incase you click in the widget, move the mouse
            // outside it's boundaries and let go of the button
            if (!B) {
              if ((cur -> Widget -> _CallbackTrigger & NTK_CT_MOUSE_UP) && (cur -> Widget -> _State)) cur -> Widget -> Callback(NTK_EVENT_MOUSE_UP);
              cur -> Widget -> Handle(Event, A, B, C, D);
            } //fi

            // Remove Ofset
            if (!cur -> Widget -> Anchored()) {
              cur -> Widget -> _pX-= _OfsetX;
              cur -> Widget -> _pY-= _OfsetY;
            } //fi

            NTK_Container::ContainerDisplacement(&cur -> Widget -> _pX, &cur -> Widget -> _pY, cur -> Widget -> _Container,0);
            break;

          case NTK_EVENT_MOTION:
            //Motion assumes Mouse button is down
            NTK_Container::ContainerDisplacement(&cur -> Widget -> _pX, &cur -> Widget -> _pY, cur -> Widget -> _Container,1);

            // Add Ofset
            if (!cur -> Widget -> Anchored()) {
              cur -> Widget -> _pX+= _OfsetX;
              cur -> Widget -> _pY+= _OfsetY;
            } //fi

            cur -> Widget -> Handle(Event, A, B, C, D);
            if ((cur -> Widget -> _CallbackTrigger & NTK_CT_MOTION) && (cur -> Widget -> _State)) cur -> Widget -> Callback(NTK_EVENT_MOTION);

            // Remove Ofset
            if (!cur -> Widget -> Anchored()) {
              cur -> Widget -> _pX-= _OfsetX;
              cur -> Widget -> _pY-= _OfsetY;
            } //fi

            NTK_Container::ContainerDisplacement(&cur -> Widget -> _pX, &cur -> Widget -> _pY, cur -> Widget -> _Container,0);
            break;

          case NTK_EVENT_KEY:
            if (cur -> Widget -> _State) {
              cur -> Widget -> Handle(Event, A, B, C, D);
              if (cur -> Widget -> _CallbackTrigger & NTK_CT_KEY) cur -> Widget -> Callback(NTK_EVENT_KEY);
            } //fi
            break;

          case NTK_EVENT_PASSIVE:
            break;

          default:
            break;
        } //eos

      } //fi
      cur= cur -> Next;
    } //eow
    } //fi
  } //fi
} //eof NTK_Window::Handle()


void NTK_Window::Widgets_Off() {
  //This function is called when the Mouse leaves the window and the
  //Mouse button goes Up, the widgets must go back to their state.
  NTK_Window::WidgetNode	*cur= NULL;

  cur= Head;
  while(cur) {
    if (cur -> Widget -> _State) cur -> Widget -> Handle(NTK_EVENT_MOUSE,0,0,0,0);
    cur= cur -> Next;
  } //eof
} //eof NTK_Window::Widgets_Off()


bool NTK_Window::TypeActive(char T) {
  NTK_Window::WidgetNode	*cur= NULL;

  cur= Head;
  while(cur) {
    if (cur -> Widget -> _Type == T && cur -> Widget -> _State) return(1);
    cur= cur -> Next;
  } //eof

  return(0);
} //eof NTK_Window::TypeActive()


NTK_Glyphic NTK_Window::GlyphicXPM(char **G, bool Linear) {
  unsigned	char	*Glyphic,*Final;
  unsigned	int	i,n;
  NTK_Glyphic		RG;

  ntku_Glyphic::Size(RG.GW,RG.GH,(unsigned char**)G);
  Glyphic= (unsigned char*)malloc(sizeof(unsigned char)*RG.GW*RG.GH*4);
  ntku_Glyphic::Create((unsigned char**)G,Glyphic);

  RG.TexW= 1; while (RG.TexW < RG.GW) RG.TexW*= 2;
  RG.TexH= 1; while (RG.TexH < RG.GH) RG.TexH*= 2;
  Final= (unsigned char*)malloc(sizeof(unsigned char)*RG.TexW*RG.TexH*4);

  for (i= 0; i < RG.TexW*RG.TexH; i++) Final[i*3]= Final[i*3+1]= Final[i*3+2]= 255;

  n= 0;
  for (i= 0; i < RG.GW*RG.GH; i++) {
    if (!(i%RG.GW)) {
      if (i)
      n+= (RG.TexW-RG.GW);
    } //fi
    Final[i*4+n*4]= Glyphic[i*4];
    Final[i*4+1+n*4]= Glyphic[i*4+1];
    Final[i*4+2+n*4]= Glyphic[i*4+2];
    Final[i*4+3+n*4]= Glyphic[i*4+3];
  } //eof

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glGenTextures(1, &RG.TexID);
  glBindTexture(GL_TEXTURE_2D, RG.TexID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,Linear ? GL_LINEAR : GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,Linear ? GL_LINEAR : GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, RG.TexW, RG.TexH, 0, GL_RGBA, GL_UNSIGNED_BYTE, Final);
  free(Final);
  free(Glyphic);

  return(RG);
} //eof NTK_Window::GlyphicXPM()


NTK_Glyphic NTK_Window::GlyphicRAW(unsigned char *G, unsigned int W, unsigned int H, bool Linear) {
  unsigned	char	*Final;
  unsigned	int	i,n;
  NTK_Glyphic		RG;

  RG.GW= W;
  RG.GH= H;

  RG.TexW= 1; while (RG.TexW < RG.GW) RG.TexW*= 2;
  RG.TexH= 1; while (RG.TexH < RG.GH) RG.TexH*= 2;
  Final= (unsigned char*)malloc(sizeof(unsigned char)*RG.TexW*RG.TexH*4);

  for (i= 0; i < RG.TexW*RG.TexH; i++) Final[i*3]= Final[i*3+1]= Final[i*3+2]= 255;

  n= 0;
  for (i= 0; i < RG.GW*RG.GH; i++) {
    if (!(i%RG.GW)) {
      if (i)
      n+= (RG.TexW-RG.GW);
    } //fi
    Final[i*4+n*4]= G[i*4];
    Final[i*4+1+n*4]= G[i*4+1];
    Final[i*4+2+n*4]= G[i*4+2];
    Final[i*4+3+n*4]= G[i*4+3];
  } //eof

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glGenTextures(1, &RG.TexID);
  glBindTexture(GL_TEXTURE_2D, RG.TexID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,Linear ? GL_LINEAR : GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,Linear ? GL_LINEAR : GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, RG.TexW, RG.TexH, 0, GL_RGBA, GL_UNSIGNED_BYTE, Final);
  free(Final);

  return(RG);
} //eof NTK_Window::GlyphicRAW()
