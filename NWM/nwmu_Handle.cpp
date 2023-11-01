#include "nwmu_Handle.h"

//----- GLOBAL ---------------
static	int	AnchorLoc[2];
static	int	Lx,Ly;
static	int	WinInfo[6];
static	bool	VH;
//----------------------------


void nwmu_Handle::SetView() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glViewport(WinInfo[0], WinInfo[1], WinInfo[2], WinInfo[3]);
  glOrtho(0, WinInfo[2], WinInfo[3], 0, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glDrawBuffer(GL_FRONT);
} //eof nwmu_Handle::SetView()


void nwmu_Handle::Initialize(int VP1, int VP2, int VP3, int VP4, int Pos, int Siz, bool _VH) {
  WinInfo[0]= VP1;	// Viewport 1
  WinInfo[1]= VP2;	// Viewport 2
  WinInfo[2]= VP3;	// Viewport 3
  WinInfo[3]= VP4;	// Viewport 4
  WinInfo[4]= Pos;	// Position
  WinInfo[5]= Siz;	// Size
  VH= _VH;

  Lx= Ly= -1;
} //eof nwmu_Handle::Initialize()


void nwmu_Handle::Update(int x, int y) {
  SetView();

  //Replace Old Horz Line
  if (VH) {
    if (Lx != -1) InvertBuffer::CursorHorzLine(WinInfo[4],Ly,WinInfo[4]+WinInfo[5],Ly,2);
    InvertBuffer::CursorHorzLine(WinInfo[4],y,WinInfo[4]+WinInfo[5],y,2);
  } else {
    //Replace Old Vert Line
    if (Ly != -1) InvertBuffer::CursorVertLine(Lx,WinInfo[4],Lx,WinInfo[4]+WinInfo[5],2);
    InvertBuffer::CursorVertLine(x,WinInfo[4],x,WinInfo[4]+WinInfo[5],2);
  } //fi


  Lx= x;
  Ly= y;

  glDrawBuffer(GL_BACK);
} //eof nwmu_Handle::Update
