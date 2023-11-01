#include "ntku_Glyphic.h"


void ntku_Glyphic::Size(unsigned int &Width, unsigned int &Height, unsigned char **glyphic_xpm) {
  int	data[4];

  ntku_XPM::getXPMheader(glyphic_xpm,data);
  Width= data[0];
  Height= data[1];
} //eof ntku_Glyphic::Size()


void ntku_Glyphic::Create(unsigned char **glyphic_xpm, unsigned char *Glyphic) {
  int		data[4],i,n,k,p;
  char		ColString[8], RGBcolor[3], transcolor[3];
  
  ntku_XPM::getXPMheader(glyphic_xpm,data);
 
  p= 0;
  for (i= data[1]+data[2]; i > data[2]; i--) {
    for (n= 0; n < data[0]*data[3]; n+= data[3]) {

      if ((i == data[1]+data[2]) && (!n)) {
        // Set Transparent Color
        for (k= 0; k < data[3]; k++) ColString[k]= glyphic_xpm[i][n+k];
        ntku_XPM::getXPMColor(glyphic_xpm, data, ColString, RGBcolor);
        transcolor[0]= RGBcolor[0];
        transcolor[1]= RGBcolor[1];
        transcolor[2]= RGBcolor[2];
      } //fi

      // Extract Color String
      for (k= 0; k < data[3]; k++) ColString[k]= glyphic_xpm[i][n+k];
      ntku_XPM::getXPMColor(glyphic_xpm, data, ColString, RGBcolor);

      if ((RGBcolor[0] == transcolor[0]) && (RGBcolor[1] == transcolor[1]) && (RGBcolor[2] == transcolor[2])) {
        Glyphic[p+0]= 0;
        Glyphic[p+1]= 0;
        Glyphic[p+2]= 0;
        Glyphic[p+3]= 0;
      } else {
        Glyphic[p+0]= RGBcolor[0];
        Glyphic[p+1]= RGBcolor[1];
        Glyphic[p+2]= RGBcolor[2];
        Glyphic[p+3]= 255;
      } //fi
      p+= 4;
    } //eof
  } //eof
} //eof ntku_Glyphic::Create()


void ntku_Glyphic::Draw(int PosX, int PosY, int SizX, int SizY, int Width, int Height, unsigned char *Glyphic) {
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  //Center Glyphic
  glRasterPos2i(PosX+(SizX-Width)/2,PosY+SizY-(SizY-Height+SizY%2)/2);
  glDrawPixels(Width, Height, GL_RGB, GL_UNSIGNED_BYTE, Glyphic);
} //eof ntku_Glyphic::Draw()

