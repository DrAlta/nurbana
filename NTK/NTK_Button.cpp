#include "NTK_Button.h"


NTK_Button::NTK_Button(NTK_Window *win, int PosX, int PosY, int SizX, int SizY) : NTK_Base(win, PosX, PosY, SizX, SizY, NTK_WT_Button) {
  _Type= NTK_BUTTON_TEXT;
  _Text[0]= 0;
  _Sticky= 0;
  _BorderSize= 2;
  _Group= NULL;
  _Glyphic.TexID= 0;
  _StretchGlyphic= 0;
  _Rounded= 0;
} //eof NTK_Button::NTK_Button()


NTK_Button::~NTK_Button() {
  if (_Glyphic.TexID)
    glDeleteTextures(1,&_Glyphic.TexID);
} //eof NTK_Button::NTK_Button()



void NTK_Button::Draw() {
  // Draw button
  if (_Rounded) {
    ntku_Button::Rounded(_pX,_pY,_sX,_sY,_Color,_BorderSize,_State);
  } else {
    ntku_Button::Rect(_pX,_pY,_sX,_sY,_Color,_BorderSize,_State);
  } //fi

  // Draw either Text or Glyphic based on _Type
  switch( _Type ) {
    case NTK_BUTTON_TEXT:
      _Font -> Draw(_pX,_pY,_sX,_sY,_Text,NTK_JUSTIFY_CENTER,_Color,_State);
//      ntku_Text::Draw(_pX,_pY+1,_sX,_sY-1,_Text,NTK_JUSTIFY_CENTER,_Color,_State);
      break;

    case NTK_BUTTON_GLYPHIC:
      if (_State) {
        glColor4f(1,1,1,0.75);
      } else {
        glColor4f(1,1,1,1);
      } //fi

      // Draw Quad and Texture it
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, _Glyphic.TexID);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glBegin(GL_QUADS);
        glTexCoord2d(_Glyphic.GW*1.0/_Glyphic.TexW,0);
        glVertex2i(_pX+_Glyphic.GW+(_sX-_Glyphic.GW)/2,_pY+_Glyphic.GH+(_sY-_Glyphic.GH)/2);
        glTexCoord2d(_Glyphic.GW*1.0/_Glyphic.TexW,_Glyphic.GH*1.0/_Glyphic.TexH);
        glVertex2i(_pX+_Glyphic.GW+(_sX-_Glyphic.GW)/2,_pY+(_sY-_Glyphic.GH)/2);
        glTexCoord2d(0,_Glyphic.GH*1.0/_Glyphic.TexH);
        glVertex2i(_pX+(_sX-_Glyphic.GW)/2,_pY+(_sY-_Glyphic.GH)/2);
        glTexCoord2d(0,0);
        glVertex2i(_pX+(_sX-_Glyphic.GW)/2,_pY+_Glyphic.GH+(_sY-_Glyphic.GH)/2);
      glEnd();
      glDisable(GL_BLEND);
      glDisable(GL_TEXTURE_2D);
      break;

    default:
      break;
  } //eos
} //eof NTK_Button::Draw()



bool NTK_Button::Handle(unsigned char Event, int A, int B, int C, int D) {
  switch( Event ) {
    case NTK_EVENT_MOUSE:
      if (A == SDL_BUTTON_LEFT) {
        if (B) {
          if (_Group) _Group -> Off();
          _Sticky ? _State= !_State : _State= 1;
          _Update= 2;
          return(1);
        } else {
          if ((!_Sticky) && (_State)) {
            _State= 0;
            _Update= 2;
          } //fi
          return(0);
        } //fi
      } //fi
      break;

    default:
      break;
  } //eos
  return(0);
} //eof NTK_Button::Handle()


void NTK_Button::Group(NTK_Group *G) {
  G -> Push(this);
  _Group= G;
  _Sticky= NTK_ON;
} //eof NTK_Button::Group()


// Temporary fix
void NTK_Button::GroupOff(void) {
  if (_Group) _Group -> Off();
} //eof NTK_Button::GroupOff()


void NTK_Button::Glyphic(NTK_Glyphic G) {
  if (_Glyphic.TexID)
    glDeleteTextures(1,&_Glyphic.TexID);

  _Glyphic= G;
  _Update= 2;
} //eof NTK_Button::Glyphic()
