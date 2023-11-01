#include "NTK_TextInput.h"


NTK_TextInput::NTK_TextInput(NTK_Window *win, int PosX, int PosY, int SizX, int SizY) : NTK_Base(win, PosX, PosY, SizX, SizY, NTK_WT_TextInput) {
  _Index= 0;
  _Text[0]= _StaticText[0]= NULL;
} //eof TextInput::NTK_TextInput()

NTK_TextInput::~NTK_TextInput() {}


bool NTK_TextInput::Handle(unsigned char Event, int A, int B, int C, int D) {
  int	i;

  switch( Event ) {
    case NTK_EVENT_MOUSE:
      //Functions like a Sticky Button
      if (B) {
        _Index= strlen(_Text);
        _State= !_State;
        if (!_State) _Index= 0;
        _Update= 2;
      } //fi
      break;

    case NTK_EVENT_KEY:
      _Update= 2;

      switch( KeyTable::Key() ) {
        case SDLK_BACKSPACE:
          if (_Index) {

            for (i= _Index-1; i < (int)strlen(_Text); i++)
              _Text[i]= _Text[i+1];

            _Index--;
          } //fi
          break;

        case SDLK_RETURN:
        case SDLK_KP_ENTER:
          _Index= 0;
          _State= 0;
          _Update= 2;
          break;

        case SDLK_LEFT:
          if (_Index) _Index--;
          break;

        case SDLK_RIGHT:
          if (_Index < (int)strlen(_Text)) _Index++;
          break;

        default:
          char	T[2];
          char	C;

          C= KeyTable::KeySym();
          if ((int)C) {
            if ((int)strlen(_Text) == _Index)_Index++;

            //Place text at the location of the _Index
            if (_Index < (int)strlen(_Text)) {
              for (i= strlen(_Text); i >= _Index; i--)
                _Text[i+1]= _Text[i];
              _Text[_Index]= C;
              _Index++;
            } else {
              T[0]= C;
              T[1]= NULL;
              strcat(_Text,T);
            } //fi
          } //fi
          break;

      } //eos
      break;

    default:
      break;
  } //eos
  return(1);
} //eof NTK_TextInput::Handle()



void NTK_TextInput::Draw() {
  char	Temp[256];
  int	CharWidth,NC,i;
  char	T[2];

  ntku_Button::Rect(_pX, _pY, _sX, _sY, _Color, 2, 1);

  CharWidth= _Font -> CharWidth(_sX,_sY);

  strcpy(Temp,_StaticText);
  if ((_sX/CharWidth)-(int)strlen(_StaticText) > _Index) {
    for (i= 0; (i < (_sX/CharWidth)-(int)strlen(_StaticText) - 1) && (i < (int)strlen(_Text)); i++) {
      T[0]= _Text[i];
      T[1]= NULL;
      strcat(Temp,T);
    } //eof
  } else {
    //Figure out how many characters can be displayed assuming StaticText is there
    NC= (_sX/CharWidth)-strlen(_StaticText);
    for (i= 1; (i < NC) && (i < (int)strlen(_Text)); i++) {
      T[0]= _Text[_Index-NC+i];
      T[1]= NULL;
      strcat(Temp,T);
    } //eof
  } //fi

  _Font -> Draw(_pX,_pY,_sX,_sY,Temp,NTK_JUSTIFY_LEFT,_Color,1);

  //Draw Cursor if Active
  if (_State) {
    int	ofset;

    if ((_sX/CharWidth)-(int)strlen(_StaticText) > _Index) {
      ofset= CharWidth*strlen(_StaticText)+_Index*CharWidth;
    } else {
      ofset= _sX-CharWidth;
    } //fi

    glBegin(GL_QUADS);
      glColor3ub(0,0,128);
      glVertex2i(_pX+ofset+4,_pY+_sY-2);
      glVertex2i(_pX+ofset+4,_pY+2);
      glVertex2i(_pX+ofset+2,_pY+2);
      glVertex2i(_pX+ofset+2,_pY+_sY-2);
    glEnd();
  } //fi
} //eof NTK_TextInput::Draw()


void NTK_TextInput::FullText(char *S) {
  strcpy(S,_StaticText);
  strcat(S,_Text);
} //eof NTK_TextInput::FullText()


void NTK_TextInput::Value(float V) {
  // Convert Value into text
  char		Value[32];

  sprintf(Value,"%f",V);
  strcpy(_Text,Value);
  _Update= 2;
} //eof NTK_TextInput::Value()
