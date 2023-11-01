#include "NTK_ScrollBar.h"

#define	BUTTON_BORDER	2

NTK_ScrollBar::NTK_ScrollBar(NTK_Window *win, int PosX, int PosY, int SizX, int SizY) : NTK_Base(win, PosX, PosY, SizX, SizY, NTK_WT_ScrollBar) {
  _Value= 0;
  _SmallScroll= 0;
  _SmallScrollState[0]= _SmallScrollState[1]= 0;
} //eof NTK_ScrollBar::NTK_ScrollBar()

NTK_ScrollBar::~NTK_ScrollBar() {}


void _GetBoundary(int pX, int pY, int sX, int sY, float V, char T, int Data[4]) {
  // Data elemented define Top,Buttom,Left,Right (exact pixel location)
  switch( T ) {
    case 1: // Small scroll Top
      Data[0]= pX;
      Data[1]= pY;
      Data[2]= pX+sX;
      Data[3]= pY+sX;
      break;

    case 2: // Small scroll Bottom
      Data[0]= pX;
      Data[1]= pY+sY-sX;
      Data[2]= pX+sX;
      Data[3]= pY+sY;
      break;

    case 3: // Button
      Data[0]= pX+BUTTON_BORDER;
      Data[1]= int((BUTTON_BORDER+pY+sX)+V*(sY-4*sX-2*BUTTON_BORDER));
      Data[2]= pX+BUTTON_BORDER+sX-(2*BUTTON_BORDER);
      Data[3]= int((BUTTON_BORDER+pY+sX)+V*(sY-4*sX-2*BUTTON_BORDER)+2*sX);
      break;

    case 4: // Outter Bevling
      Data[0]= pX;
      Data[1]= pY+sX;
      Data[2]= pX+sX;
      Data[3]= pY+sY-sX;
      break;

    default:
      break;
  } //eos
} //eof _GetBoundary()


void NTK_ScrollBar::Draw() {
  int		Data[4],i;
  char		Color[3]= {0x98, 0x98, 0x98};

  _GetBoundary(_pX,_pY,_sX,_sY,_Value,1,Data);
  ntku_Button::Rect(Data[0], Data[1], Data[2]-Data[0], Data[3]-Data[1], Color, BUTTON_BORDER, _SmallScrollState[0]);
  // Draw an Up Arrow
  glColor3ub(0x11,0x33,0x55);
  glBegin(GL_TRIANGLES);
    glVertex2i(Data[0]+4,Data[3]-6);
    glVertex2i((Data[0]+Data[2]-2)/2,Data[1]+4);
    glVertex2i(Data[2]-6,Data[3]-6);
  glEnd();

  _GetBoundary(_pX,_pY,_sX,_sY,_Value,2,Data);
  ntku_Button::Rect(Data[0], Data[1], Data[2]-Data[0], Data[3]-Data[1], Color, BUTTON_BORDER, _SmallScrollState[1]);
  // Draw a Down Arrow
  glColor3ub(0x11,0x33,0x55);
  glBegin(GL_TRIANGLES);
    glVertex2i(Data[0]+4,Data[1]+4);
    glVertex2i((Data[0]+Data[2]-2)/2,Data[3]-6);
    glVertex2i(Data[2]-6,Data[1]+4);
  glEnd();


  // Draw Bevel Area for Slider Button
  _GetBoundary(_pX,_pY,_sX,_sY,_Value,4,Data);
  ntku_Button::Rect(Data[0], Data[1], Data[2]-Data[0], Data[3]-Data[1], Color, BUTTON_BORDER, 1);

  // Draw Slider Button
  _GetBoundary(_pX,_pY,_sX,_sY,_Value,3,Data);
  ntku_Button::Rect(Data[0], Data[1], Data[2]-Data[0], Data[3]-Data[1], Color, BUTTON_BORDER, 0);
  // Draw detail onto Button
  glBegin(GL_LINES);
    for (i= 0; i < 4; i++) {
      glColor3ub(0xC0,0xC0,0xC0);
        glVertex2i(Data[0]+(Data[2]-Data[0])/4,Data[1]+(i+1)*((Data[3]-Data[1])/5));
        glVertex2i(Data[0]+(Data[2]-Data[0])-(Data[2]-Data[0])/4,Data[1]+(i+1)*((Data[3]-Data[1])/5));
      glColor3ub(0x60,0x60,0x60);
        glVertex2i(Data[0]+(Data[2]-Data[0])/4,Data[1]+(i+1)*((Data[3]-Data[1])/5)+1);
        glVertex2i(Data[0]+(Data[2]-Data[0])-(Data[2]-Data[0])/4,Data[1]+(i+1)*((Data[3]-Data[1])/5)+1);
    } //eof

  glEnd();
} //eof NTK_ScrollBar::Draw()



bool NTK_ScrollBar::Handle(unsigned char Event, int A, int B, int C, int D) {
  int		Data[4];

  switch( Event ) {
    case NTK_EVENT_MOUSE:
      if (B) {
        // Check for Small Scroll Button (Top)
        _GetBoundary(_pX,_pY,_sX,_sY,_Value,1,Data);
        if ((C > Data[0]) && (C < Data[2]) && (D > Data[1]) && (D < Data[3])) {
          _SmallScrollState[0]= 1;
          _Update= 2;
          if ((_Value - .1) < 0) {
            _Value= 0;
          } else {
            _Value-= _SmallScroll;
          } //fi
        } //fi


        // Check for Small Scroll Button (Bottom)
        _GetBoundary(_pX,_pY,_sX,_sY,_Value,2,Data);
        if ((C > Data[0]) && (C < Data[2]) && (D > Data[1]) && (D < Data[3])) {
          _SmallScrollState[1]= 1;
          _Update= 2;
          if ((_Value + .1) > 1) {
            _Value= 1;
          } else {
            _Value+= _SmallScroll;
          } //fi
        } //fi


        // Check for Slider Button
        _GetBoundary(_pX,_pY,_sX,_sY,_Value,3,Data);
        if ((C > Data[0]) && (C < Data[2]) && (D > Data[1]) && (D < Data[3])) {
          _State= 1;
          _CDY= D-Data[1];
          _Update= 2;
        } //fi
      } else {
        _State= 0;
        _Update= 2;
        _SmallScrollState[0]= _SmallScrollState[1]= 0;
      } //fi
      break;


    case NTK_EVENT_MOTION:
      if (_State) {
        _GetBoundary(_pX,_pY,_sX,_sY,_Value,4,Data);
        if (B <= Data[1]+_CDY) {
          _Value= 0;
        } else if (B >= Data[3]+_CDY-(2*_sX+BUTTON_BORDER*2)) {
          _Value= 1;
        } else {
          _Value= float(B-Data[1]-_CDY)/float((Data[3]-Data[1])-(2*_sX+BUTTON_BORDER*2));
        } //fi
        _Update= 2;
      } //fi
      break;


    default:
      break;
  } //eos
  return(1);
} //eof NTK_ScrollBar::Handle()
