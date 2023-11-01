#include "NTK_Label.h"


NTK_Label::NTK_Label(NTK_Window *win, int PosX, int PosY, int SizX, int SizY) : NTK_Base(win, PosX, PosY, SizX, SizY, NTK_WT_Label) {
  _Type= NTK_LABEL_TEXT;
  _Bevel= NTK_OFF;
  _Align= NTK_JUSTIFY_CENTER;
  _Text[0]= NULL;
  _Glyphic.TexID= 0;
  _StretchGlyphic= 0;
  _Rounded= 0;
} //eof NTK_Label::NTK_Label()


NTK_Label::~NTK_Label() {
  if (_Glyphic.TexID)
    glDeleteTextures(1,&_Glyphic.TexID);
} //eof NTK_Label::~NTK_Label()


void NTK_Label::Draw() {
  //Draw Bevel
  if (_Bevel)
    if (_Rounded) {
      ntku_Button::Rounded(_pX,_pY,_sX,_sY,_Color,2,_State);
    } else {
      ntku_Button::Rect(_pX,_pY,_sX,_sY,_Color,2,_State);
    } //fi

  switch( _Type ) {
    case NTK_LABEL_TEXT:
      _Font -> Draw(_pX,_pY,_sX,_sY,_Text,_Align,_Color,NTK_OFF);
      break;

    case NTK_LABEL_GLYPHIC:
      glColor4f(1,1,1,1);

      // Draw Quad and Texture it
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, _Glyphic.TexID);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glBegin(GL_QUADS);
        glTexCoord2f(_Glyphic.GW*1.0/_Glyphic.TexW,0);
        _StretchGlyphic ? glVertex2i(_pX+_sX-2,_pY+2) : glVertex2i(_pX+_Glyphic.GW+(_sX-_Glyphic.GW)/2,_pY+_Glyphic.GH+(_sY-_Glyphic.GH)/2);
        glTexCoord2f(_Glyphic.GW*1.0/_Glyphic.TexW,_Glyphic.GH*1.0/_Glyphic.TexH);
        _StretchGlyphic ? glVertex2i(_pX+_sX-2,_pY+_sY-2) : glVertex2i(_pX+_Glyphic.GW+(_sX-_Glyphic.GW)/2,_pY+(_sY-_Glyphic.GH)/2);
        glTexCoord2f(0,_Glyphic.GH*1.0/_Glyphic.TexH);
        _StretchGlyphic ? glVertex2i(_pX+2,_pY+_sY-2) : glVertex2i(_pX+(_sX-_Glyphic.GW)/2,_pY+(_sY-_Glyphic.GH)/2);
        glTexCoord2f(0,0);
        _StretchGlyphic ? glVertex2i(_pX+2,_pY+2) : glVertex2i(_pX+(_sX-_Glyphic.GW)/2,_pY+_Glyphic.GH+(_sY-_Glyphic.GH)/2);
      glEnd();
      glDisable(GL_BLEND);
      glDisable(GL_TEXTURE_2D);

      break;

    default:
      break;
  } //eos
} //eof NTK_Label::Draw()



bool NTK_Label::Handle(unsigned char Event, int A, int B, int C, int D) { return(0); }

void NTK_Label::Glyphic(NTK_Glyphic G) {
  if (_Glyphic.TexID)
    glDeleteTextures(1,&_Glyphic.TexID);

  _Glyphic= G;
  _Update= 2;
} //eof NTK_Label::Glyphic()