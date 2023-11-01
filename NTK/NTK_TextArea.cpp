#include "NTK_TextArea.h"



NTK_TextArea::NTK_TextArea(NTK_Window *win, int PosX, int PosY, int SizX, int SizY) : NTK_Base(win, PosX, PosY, SizX, SizY, NTK_WT_TextArea) {
  _WordWrap= NTK_OFF;
  _FontSize= 20;
  _Index= 0;
  _Line= 0;

  // Start off with 1 Text Line
  _CurLine= new TextLine;
  _CurLine -> LF= 0;
  _CurLine -> Text= (char*)malloc(sizeof(char)*1);
  _CurLine -> Text[0]= NULL;
  _CurLine -> Next= NULL;
  _CurLine -> Prev= NULL;

  _Buffer= (char*)malloc(sizeof(char));
  _Buffer[0]= NULL;
  _Load= 0;

  _ScrollBar= new NTK_ScrollBar(_Window,0,0,NTK_TextArea_ScrollBarWidth,200);
  _ScrollBar -> CallbackTrigger(NTK_CT_MOTION | NTK_CT_MOUSE_DOWN);
} //eof TextInput::NTK_TextArea()

NTK_TextArea::~NTK_TextArea() {}


bool NTK_TextArea::Handle(unsigned char Event, int A, int B, int C, int D) {
  TextLine	*cur;
  int		i,n,k;

  switch( Event ) {
    case NTK_EVENT_MOUSE:
      //Functions like a Sticky Button
      if (C < _sX-NTK_TextArea_ScrollBarWidth) {
        if (B) {
          _Index= strlen(_CurLine -> Text);
          _State= !_State;
          if (!_State) _Index= 0;
        } //fi
      } //fi
      _Update= 2;
      break;

    case NTK_EVENT_MOTION:
      if (A > _sX-NTK_TextArea_ScrollBarWidth)
        _Update= 2;
      break;

    case NTK_EVENT_KEY:
      switch( KeyTable::Key() ) {
        case SDLK_BACKSPACE:
          if (_Index) {

            for (i= _Index-1; i < (int)strlen(_CurLine -> Text); i++)
              _CurLine -> Text[i]= _CurLine -> Text[i+1];

            _Index--;
            _CurLine -> Text= (char*)realloc(_CurLine -> Text,sizeof(char)*(strlen(_CurLine -> Text)+1));
          } else {
            if (_CurLine -> Prev) {
              n= (_sX-NTK_TextArea_ScrollBarWidth)/_Font -> CharWidth((_sX-NTK_TextArea_ScrollBarWidth),_FontSize) - (int)strlen(_CurLine -> Prev -> Text);
              if (n > (int)strlen(_CurLine -> Text)) {
                n= strlen(_CurLine -> Text);
              } else {
                while (_CurLine -> Text[n] != 32 && n) n--;
              } //fi

              _CurLine -> Prev -> Text= (char*)realloc(_CurLine -> Prev -> Text,sizeof(char)*(strlen(_CurLine -> Prev -> Text)+n+1));
              k= (int)strlen(_CurLine -> Prev -> Text);
              for (i= 0; i < n; i++)
                _CurLine -> Prev -> Text[k+i]= _CurLine -> Text[i];
              _CurLine -> Prev -> Text[k+i]= NULL;
              for (i= 0; i < (int)strlen(_CurLine -> Text)-n; i++)
                _CurLine -> Text[i]= _CurLine -> Text[i+n];
              _CurLine -> Text[i]= NULL;
              _CurLine -> LF= 0;

              _Index= k;
              if (!strlen(_CurLine -> Text)) {
                _CurLine -> Prev -> Next= _CurLine -> Next;
                if (_CurLine -> Next) _CurLine -> Next -> Prev= _CurLine -> Prev;
                cur= _CurLine;
                _CurLine= _CurLine -> Prev;
                free(cur -> Text);
                delete cur;
              } else {
                _CurLine= _CurLine -> Prev;
              } //fi

            } //fi
          } //fi
         _Update= 2;
          break;

        case SDLK_DELETE:
          if (_Index < (int)strlen(_CurLine -> Text)) {
            for (i= _Index; i < (int)strlen(_CurLine -> Text); i++)
              _CurLine -> Text[i]= _CurLine -> Text[i+1];
            _CurLine -> Text= (char*)realloc(_CurLine -> Text,sizeof(char)*(strlen(_CurLine -> Text)+1));
          } else {
          } //fi
          _Update= 2;
          break;

        case SDLK_ESCAPE:
          _Index= 0;
          _State= 0;
          _Update= 2;
          break;

        case SDLK_RETURN:
        case SDLK_KP_ENTER:
          cur= new TextLine;
          cur -> LF= 0;
          cur -> Text= (char*)malloc(sizeof(char)*((int)strlen(_CurLine -> Text)-_Index+1));

          for (i= _Index; i <= (int)strlen(_CurLine -> Text); i++)
            cur -> Text[i-_Index]= _CurLine -> Text[i];
          _CurLine -> Text[_Index]= NULL;
          _CurLine -> LF= 1;

          cur -> Prev= _CurLine;
          cur -> Next= _CurLine -> Next;
          if (_CurLine -> Next) _CurLine -> Next -> Prev= cur;
          _CurLine -> Next= cur;
          _CurLine= cur;

          _Index= 0;
          _Update= 2;
          break;

        case SDLK_LEFT:
          if (_Index) {
            _Index--;
          } else {
            if (_CurLine -> Prev) {
              _CurLine= _CurLine -> Prev;
              _Index= strlen(_CurLine -> Text);
            } //fi
          } //fi
          _Update= 2;
          break;

        case SDLK_RIGHT:
          if (_Index < (int)strlen(_CurLine -> Text)) {
            _Index++;
          } else {
            if (_CurLine -> Next) {
              _CurLine= _CurLine -> Next;
              _Index= 0;
            } //fi
          } //fi
          _Update= 2;
          break;

        case SDLK_UP:
          if (_CurLine -> Prev) _CurLine= _CurLine -> Prev;
          if (_Index > (int)strlen(_CurLine -> Text)) _Index= strlen(_CurLine -> Text);
          _Update= 2;
          break;

        case SDLK_DOWN:
          if (_CurLine -> Next) _CurLine= _CurLine -> Next;
          if (_Index > (int)strlen(_CurLine -> Text)) _Index= strlen(_CurLine -> Text);
          _Update= 2;
          break;

        case SDLK_HOME:
          _Index= 0;
          _Update= 2;
          break;

        case SDLK_END:
          _Index= (int)strlen(_CurLine -> Text);
          _Update= 2;
          break;

        default:
          char	T[2];
          char	C;

          if (KeyTable::Mod(KMOD_CTRL)) {
            if (KeyTable::Key() == SDLK_k) {
              _Buffer= (char*)realloc(_Buffer,sizeof(char)*(strlen(_CurLine -> Text)+1));
              strcpy(_Buffer,_CurLine -> Text);

              cur= _CurLine;
              if (_CurLine -> Prev) _CurLine -> Prev -> Next= _CurLine -> Next;
              if (_CurLine -> Next) _CurLine= _CurLine -> Next;
              _CurLine -> Prev= cur -> Prev;
              if (_CurLine -> Next) { free(cur -> Text); delete cur; }
              if (!_CurLine -> Next && !_CurLine -> Prev) {
                _CurLine -> Text= (char*)realloc(_CurLine -> Text,sizeof(char));
                _CurLine -> Text[0]= NULL;
                _Index= 0;
              } //fi

              _Update= 2;
            } else if (KeyTable::Key() == SDLK_u) {
              cur= new TextLine;
              cur -> LF= 0;
              cur -> Next= _CurLine;
              cur -> Prev= _CurLine -> Prev;

              if (_CurLine -> Prev) _CurLine -> Prev -> Next= cur;
              _CurLine -> Prev= cur;

              cur -> Text= (char*)malloc(sizeof(char));
              cur -> Text= (char*)realloc(cur -> Text,sizeof(char)*(strlen(_Buffer)+1));
              strcpy(cur -> Text,_Buffer);
              _Update= 2;
            } //fi
          } else {
            C= KeyTable::KeySym();
            if ((int)C) {

              if ((int)strlen(_CurLine -> Text)+1 == (_sX-NTK_TextArea_ScrollBarWidth)/_Font -> CharWidth((_sX-NTK_TextArea_ScrollBarWidth),_FontSize)) {
                _Index= 0;
                _Update= 2;

                cur= new TextLine;
                cur -> LF= 0;
                cur -> Text= (char*)malloc(sizeof(char));
                cur -> Text[0]= NULL;

                // Word Wrap
                i= (int)strlen(_CurLine -> Text);
                while (_CurLine -> Text[i] != 32 && i) i--;
                if (i) {
                  i++;
                  n= 0;
                  cur -> Text= (char*)realloc(cur -> Text,sizeof(char)*(strlen(_CurLine -> Text)-i+1));
                  for (n= 0; n < ((int)strlen(_CurLine -> Text)-i); n++)
                    cur -> Text[n]= _CurLine -> Text[i+n];
                  cur -> Text[n]= NULL;
                  _CurLine -> Text= (char*)realloc(_CurLine -> Text,sizeof(char)*i);
                  _CurLine -> Text[i-1]= NULL;
                  _Index= n;
                } //fi

                cur -> Next= _CurLine -> Next;
                cur -> Prev= _CurLine;

                if (_CurLine -> Next) _CurLine -> Next -> Prev= cur;
                _CurLine -> Next= cur;

                _CurLine= cur;
              } //fi
              _CurLine -> Text= (char*)realloc(_CurLine -> Text,sizeof(char)*(strlen(_CurLine -> Text)+2));
              if ((int)strlen(_CurLine -> Text) == _Index)_Index++;

              // Place text at the location of the _Index
              if (_Index < (int)strlen(_CurLine -> Text)) {
                for (i= strlen(_CurLine -> Text); i >= _Index; i--)
                  _CurLine -> Text[i+1]= _CurLine -> Text[i];
                _CurLine -> Text[_Index]= C;
                _Index++;
              } else {
                T[0]= C;
                T[1]= NULL;
                strcat(_CurLine -> Text,T);
              } //fi
            } //fi
            _Update= 2;
          } //fi
          break;

      } //eos
      break;

    default:
      break;
  } //eos
  return(1);
} //eof NTK_TextArea::Handle()



void NTK_TextArea::Draw() {
  TextLine	*cur;
  int		CurLine,Line,Cursor,i,n,k;

  // Format new text that has been loaded into the text area.
  if (_Load) {
    _Load= 0;
//    printf("Load Text: %s\n",_Buffer);

    n= 0;
    _CurLine -> Text= (char*)realloc(_CurLine -> Text,sizeof(char)*(strlen(_Buffer)+2));
    for (i= 0; i < (int)strlen(_Buffer); i++) {
      if (_Buffer[i] == 10 || n > (_sX-NTK_TextArea_ScrollBarWidth)/_Font -> CharWidth((_sX-NTK_TextArea_ScrollBarWidth),_FontSize)-2) {
        if (_Buffer[i] == 10) i++;
        if (n > (_sX-NTK_TextArea_ScrollBarWidth)/_Font -> CharWidth((_sX-NTK_TextArea_ScrollBarWidth),_FontSize)-2) {
          k= n;
          while (_CurLine -> Text[n] != 32 && n) n--;
          if (n) {
            _CurLine -> Text[n]= NULL;
            i-= k-n;
            i++;
          } //fi
        } //fi

        _CurLine -> Text= (char*)realloc(_CurLine -> Text,sizeof(char)*(n+2));

        n= 0;
        _CurLine -> Next= new TextLine;
        _CurLine -> Next -> Prev= _CurLine;
        _CurLine= _CurLine -> Next;
        _CurLine -> Next= NULL;
        _CurLine -> LF= 0;
        _CurLine -> Text= (char*)malloc(sizeof(char));
        _CurLine -> Text= (char*)realloc(_CurLine -> Text,sizeof(char)*(strlen(_Buffer)+2));
        _CurLine -> Text[0]= NULL;
      } //fi
      _CurLine -> Text[n]= _Buffer[i];
      _CurLine -> Text[n+1]= 0;
      n++;
    } //eof
  } //fi

  // Update Scroll Bar
  Line= 0;
  cur= _CurLine;
  while (cur -> Prev) { cur= cur -> Prev; }
  while (cur) { Line++; cur= cur -> Next; }
  _ScrollBar -> SmallScroll(1.0/Line);

  // Draw Text Area
  ntku_Button::Rect(_pX, _pY, _sX-NTK_TextArea_ScrollBarWidth, _sY, _Color, 2, 1);

  // Set cur to Head
  cur= _CurLine;
  while (cur -> Prev) cur= cur -> Prev;

  // Determine # of lines in List
  Line= 0;
  while (cur) { Line++; cur= cur -> Next; }
  i= Line;

  k= int(_ScrollBar -> Value()*Line+0.5)-_Line;
  if (abs(k) > 0) {
    if (k >= 0) {
      for (n= 0; n < k && _CurLine -> Next; n++)
        _CurLine= _CurLine -> Next;
      _Line= int(_ScrollBar -> Value()*Line);
    } else {
      for (n= 0; n < -k && _CurLine -> Prev; n++)
        _CurLine= _CurLine -> Prev;
      _Line= int(_ScrollBar -> Value()*Line);
    } //fi
  } //fi

  if (Line > _sY/_FontSize) {
    // More Lines exist than fit in the Text Area
    // Check to see if _CurLine needs to increment or decrement _Line
    CurLine= 0;
    Line= 0;
    cur= _CurLine;
    while (cur -> Prev) { CurLine++; cur= cur -> Prev; }
    CurLine++;

    while (Line != _Line && cur) {
      Line++;
      cur= cur -> Next;
    } //eow

    if ((CurLine - Line) > _sY/_FontSize) {
      _Line+= _sY/_FontSize;
    } //fi
    if (CurLine <= Line) {
      _Line-= _sY/_FontSize;
      if (_Line < 0) _Line= 0;
    } //fi

    Line= 0;
    cur= _CurLine;
    while (cur -> Prev) { cur= cur -> Prev; }
    while (Line != _Line && cur) {
      Line++;
      cur= cur -> Next;
    } //eow

    Cursor= CurLine - Line - 1;
  } else {
    // The number of lines that exist fit in the Text Area
    Cursor= 0;
    cur= _CurLine;
    while (cur -> Prev) {
      Cursor++;
      cur= cur -> Prev;
    } //eow
  } //fi
  

  // Note, fix later... Scrollbar could've been drawn before TextArea
  _ScrollBar -> Value((float)_Line/(float)i);


  Line= 0;
  while (cur)  {
    // Draw Text on appropriate Line
    _Font -> Draw(_pX,_pY+(Line*_FontSize),(_sX-NTK_TextArea_ScrollBarWidth),_FontSize,cur -> Text,NTK_JUSTIFY_LEFT,_Color,1);
    Line++;
    if (Line == _sY/_FontSize) break;
    cur= cur -> Next;
  } //eow 

  // Draw Cursor if Active
  if (_State) {
    int	ofset;
    ofset= _Font -> CharWidth((_sX-NTK_TextArea_ScrollBarWidth),_FontSize)*_Index;

    glBegin(GL_QUADS);
      glColor3ub(0,0,128);
      glVertex2i(_pX+ofset+4,_pY+_FontSize-2+(Cursor*_FontSize));
      glVertex2i(_pX+ofset+4,_pY+2+(Cursor*_FontSize));
      glVertex2i(_pX+ofset+2,_pY+2+(Cursor*_FontSize));
      glVertex2i(_pX+ofset+2,_pY+_FontSize-2+(Cursor*_FontSize));
    glEnd();
  } //fi

} //eof NTK_TextArea::Draw()


void NTK_TextArea::Resize() {
  _ScrollBar -> pX(_pX+_sX-NTK_TextArea_ScrollBarWidth);
  _ScrollBar -> pY(_pY);
  _ScrollBar -> sY(_sY);
} //eof NTK_TextArea::Resize()


void NTK_TextArea::Text(char *Text) {
  _Buffer= (char*)malloc(sizeof(char)*strlen(Text));
  strcpy(_Buffer,Text);
  _Load= 1;
} //eof NTK_TextArea::Text()


char* NTK_TextArea::Text() {
  TextLine	*cur;
  char		*FullText;
  int		Length;

  Length= 0;

  cur= _CurLine;
  while (cur -> Prev) cur= cur -> Prev;

  while (cur) { Length+= strlen(cur -> Text)+1; cur= cur -> Next; }

  FullText= (char*)malloc(sizeof(char)*Length);
  FullText[0]= NULL;

  cur= _CurLine;
  while (cur -> Prev) cur= cur -> Prev;

  while (cur) {
    strcat(FullText,cur -> Text);
    if (cur -> Next) strcat(FullText," ");
    if (cur -> LF) strcat(FullText,"\n");
    cur= cur -> Next;
  } //eof

//  printf("FullText:\n%s",FullText);
  return(FullText);
} //eof NTK_TextArea::Text()
