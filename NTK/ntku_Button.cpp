#include "ntku_Button.h"

#define	Depth	3

void ntku_Button::Rect(int PosX, int PosY, int SizX, int SizY, char Color[3], unsigned char Border, bool State) {
  //PosX and PosY are coordinates for Top Left of Button.
  int		x[4],y[4];
  char		TColor[3];

  x[0]= PosX + SizX;
  y[0]= PosY + SizY;
  x[1]= PosX + SizX;
  y[1]= PosY;
  x[2]= PosX;
  y[2]= PosY;
  x[3]= PosX;
  y[3]= PosY + SizY;

  glBegin(GL_QUADS);
    // Draw Button
    TColor[0]= Color[0];
    TColor[1]= Color[1];
    TColor[2]= Color[2];
    if (State) {
      if (abs(TColor[0]) < 0x20) TColor[0]= 0x20;
      if (abs(TColor[1]) < 0x20) TColor[1]= 0x20;
      if (abs(TColor[2]) < 0x20) TColor[2]= 0x20;
    } //fi
    glColor3ub(TColor[0]-State*0x20,TColor[1]-State*0x20,TColor[2]-State*0x20);
      glVertex2i(x[0],y[0]);
      glVertex2i(x[1],y[1]);
      glVertex2i(x[2],y[2]);
      glVertex2i(x[3],y[3]);
  glEnd();


    if (Border) {
      glBegin(GL_LINES);


      glBegin(GL_LINES);
      if (State) {
        //Top Border Shading (Same Color, so Ignore)
        glColor3ub(0x44,0x44,0x44);
          glVertex2i(x[2],y[1]+1);
          glVertex2i(x[1],y[2]+1);
        glColor3ub(0x66,0x66,0x66);
          glVertex2i(x[2]+1,y[1]+2);
          glVertex2i(x[1]-1,y[2]+2);

        //Left Border Shading
        glColor3ub(0x44,0x44,0x44);
          glVertex2i(x[2],y[2]+2);
          glVertex2i(x[3],y[3]+1);
        glColor3ub(0x66,0x66,0x66);
          glVertex2i(x[2]+1,y[2]+3);
          glVertex2i(x[3]+1,y[3]);

        glColor3ub(0xCC,0xCC,0xCC);
        //Bottom Border Shading
        glVertex2i(x[3]+1,y[3]);
        glVertex2i(x[0],y[0]);

        //Right Border Shading
        glVertex2i(x[1]-1,y[1]+2);
        glVertex2i(x[0]-1,y[0]+1);
      } else {
        glColor3ub(0xCC,0xCC,0xCC);
        //Top Border Shading (Same Color, so Ignore)
        glVertex2i(x[2],y[1]+1);
        glVertex2i(x[1]-1,y[2]+1);

        //Left Border Shading
        glVertex2i(x[2],y[2]+2);
        glVertex2i(x[3],y[3]);

        //Bottom Border Shading
        glColor3ub(0x44,0x44,0x44);
          glVertex2i(x[3],y[3]);
          glVertex2i(x[0],y[0]);
        glColor3ub(0x66,0x66,0x66);
          glVertex2i(x[3]+1,y[3]-1);
          glVertex2i(x[0]-1,y[0]-1);

        //Right Border Shading
        glColor3ub(0x44,0x44,0x44);
          glVertex2i(x[1]-1,y[1]+1);
          glVertex2i(x[0]-1,y[0]+1);
        glColor3ub(0x66,0x66,0x66);
          glVertex2i(x[1]-2,y[1]+2);
          glVertex2i(x[0]-2,y[0]);
      } //fi
      glEnd();

    } //fi
} //eof ntku_Button:Rect()


void ntku_Button::Rounded(int PosX, int PosY, int SizX, int SizY, char Color[3], unsigned char Border, bool State) {
  Rect(PosX,PosY,SizX,SizY,Color,Border,State);

  glBegin(GL_TRIANGLES);
    glColor3f(0.7,0.7,0.7);
    // Top Left
    glVertex2i(PosX,PosY+Depth);
    glVertex2i(PosX,PosY);
    glVertex2i(PosX+Depth,PosY);

    // Top Right
    glVertex2f(PosX+SizX-Depth,PosY);
    glVertex2f(PosX+SizX,PosY);
    glVertex2f(PosX+SizX,PosY+Depth);

    // Bottom Left
    glVertex2f(PosX,PosY+SizY-Depth);
    glVertex2f(PosX,PosY+SizY);
    glVertex2f(PosX+Depth,PosY+SizY);

    // Bottom Right
    glVertex2f(PosX+SizX-Depth,PosY+SizY);
    glVertex2f(PosX+SizX,PosY+SizY);
    glVertex2f(PosX+SizX,PosY+SizY-Depth);

  glEnd();

  glBegin(GL_LINES);
    // Top Left
    State ? glColor3ub(0x44,0x44,0x44) : glColor3ub(0xCC,0xCC,0xCC);
    glVertex2i(PosX,PosY+Depth);
    glVertex2i(PosX+Depth,PosY);

    // Top Right
    glColor3ub(0x66,0x66,0x66);
    glVertex2f(PosX+SizX-Depth,PosY);
    glVertex2f(PosX+SizX,PosY+Depth);

    // Bottom Left
    glColor3ub(0x66,0x66,0x66);
    glVertex2f(PosX,PosY+SizY-Depth);
    glVertex2f(PosX+Depth,PosY+SizY);

    // Bottom Right
    State ? glColor3ub(0xCC,0xCC,0xCC) : glColor3ub(0x44,0x44,0x44);
    glVertex2f(PosX+SizX-Depth,PosY+SizY);
    glVertex2f(PosX+SizX,PosY+SizY-Depth);

  glEnd();

} //eof ntku_Button::Round()
