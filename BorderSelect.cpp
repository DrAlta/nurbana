#include "BorderSelect.h"

//----- GLOBAL ---------------
static	int	AnchorLoc[2];
static	int	Lx,Ly;
static	int	Mode,_MouseButton;
static	int	WinInfo[6];
//----------------------------

#include <stdio.h>
void _SetView() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glViewport(WinInfo[0], WinInfo[1], WinInfo[2], WinInfo[3]);
  glOrtho(0, WinInfo[2], WinInfo[3], 0, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glDrawBuffer(GL_FRONT);
} //eod SetView()


void BorderSelect::Initialize(int WinPosX, int WinPosY, int WinSizX, int WinSizY) {
  WinInfo[0]= WinPosX;
  WinInfo[1]= WinPosY;
  WinInfo[2]= WinSizX;
  WinInfo[3]= WinSizY;

  Mode= BORDER_SELECT_MODE_CROSSHAIR;
  Lx= Ly= -1;
} //eof BorderSelect::Initialize()

void BorderSelect::ScreenSize(int Width, int Height) {
  WinInfo[4]= Width;
  WinInfo[5]= Height;
} //eof BorderSelect::ScreenSize()


void BorderSelect::Anchor(int MB) {
  AnchorLoc[0]= Lx;
  AnchorLoc[1]= Ly;
  _MouseButton= MB;

  //Remove Crosshair
  glDrawBuffer(GL_FRONT);
  InvertBuffer::DrawLine(0,Ly,WinInfo[2],Ly);
  InvertBuffer::DrawLine(Lx,0,Lx,WinInfo[3]);
  glDrawBuffer(GL_BACK);

  Mode= BORDER_SELECT_MODE_BOX;
} //eof BorderSelect::Anchor()


void BorderSelect::Update(int x, int y) {
  _SetView();
  switch( Mode ) {
    case BORDER_SELECT_MODE_CROSSHAIR:
      //Replace Old Horz Line
      if (Lx != -1) InvertBuffer::DrawLine(0,Ly,WinInfo[2],Ly);
      InvertBuffer::DrawLine(0,y,WinInfo[2],y);

      //Replace Old Vert Line
      if (Ly != -1) InvertBuffer::DrawLine(Lx,0,Lx,WinInfo[3]);
      InvertBuffer::DrawLine(x,0,x,WinInfo[3]);

      break;


    case BORDER_SELECT_MODE_BOX:
      //Top,Right,Bottom,Left
      //Replace Old 4 Box Lines
      InvertBuffer::DrawLine(AnchorLoc[0],AnchorLoc[1],Lx,AnchorLoc[1]);
      InvertBuffer::DrawLine(Lx,AnchorLoc[1],Lx,Ly);
      InvertBuffer::DrawLine(AnchorLoc[0],Ly,Lx,Ly);
      InvertBuffer::DrawLine(AnchorLoc[0],AnchorLoc[1],AnchorLoc[0],Ly);

      //Draw New 4 Box Lines
      InvertBuffer::DrawLine(AnchorLoc[0],AnchorLoc[1],x,AnchorLoc[1]);
      InvertBuffer::DrawLine(x,AnchorLoc[1],x,y);
      InvertBuffer::DrawLine(AnchorLoc[0],y,x,y);
      InvertBuffer::DrawLine(AnchorLoc[0],AnchorLoc[1],AnchorLoc[0],y);
      break;

    default:
      break;
  } //eos

  Lx= x;
  Ly= y;

  glDrawBuffer(GL_BACK);
} //eof BorderSelect::Update


void BorderSelect::GetRegion(int Data[4]) {
  //Center the Cursor amongst the box coordinates and
  //return the width/2 and height/2 of the box

  Data[0]= ((AnchorLoc[0])+(Lx))/2;
  Data[1]= ((AnchorLoc[1])+(Ly))/2;
  Data[2]= (Lx-AnchorLoc[0]);
  Data[3]= (Ly-AnchorLoc[1]);
} //eof BorderSelect::GetRegion()


int BorderSelect::MouseButton() {
  return( _MouseButton );
} //eof BorderSelect::MouseButton()
