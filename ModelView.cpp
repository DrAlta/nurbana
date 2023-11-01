#include "ModelView.h"


ModelView::ModelView(Object_Manager *ObjectList) {
  _SM= 1;
  _Axes= 0;
  _zBuffer= 0;
  _View= 0;

  _ScreenHeight= _ScreenWidth= 0;
  _Viewport[0]= _Viewport[1]= _Viewport[2]= _Viewport[3]= 0;

  _ZoomLevel= -20;
  _Angle[0]= _Angle[1]= _Angle[2]= 0;
  _Z_CLIP_NEAR= .20;
  _Z_CLIP_FAR= 2000;
  _ObjectList= ObjectList;

  _Tran[0]= 0;
  _Tran[1]= 0;
  _Tran[2]= 0;
} //eof ModelView


ModelView::~ModelView() {
  delete _ObjectList;
} //eof ModelView


void rotate(double Angle[3]) {
  glRotated(Angle[2],0,0,1);
  glRotated(Angle[1],0,1,0);
  glRotated(Angle[0],1,0,0);
} //eof rotate



void ModelView::ViewMode() {
  float	x, y, view_edge_w, view_edge_h;

  if (_View) {
    view_edge_w= -_ZoomLevel * tan(NurbanaMath::DtoR(_ObjectList -> FOV()) * 0.5) * float(_Viewport[2])/float(_Viewport[3]);
    view_edge_h= -_ZoomLevel * tan(NurbanaMath::DtoR(_ObjectList -> FOV()) * 0.5);
    glOrtho(-view_edge_w,view_edge_w,-view_edge_h,view_edge_h,-_Z_CLIP_FAR,_Z_CLIP_FAR);
  } else {
    y= _Z_CLIP_NEAR * tan(NurbanaMath::DtoR(_ObjectList -> FOV()) * 0.5);
    x= y * (float(_Viewport[2])/float(_Viewport[3]));
    glFrustum(-x,x,-y,y,_Z_CLIP_NEAR,_Z_CLIP_FAR);
  } //fi
} //eof ModelView::ViewMode()



void ModelView::Reshape(int Width, int Height) {
  // Prevent A Divide By Zero If The Window Is Too Small
  if (!Height) Height= 1;
  if (!Width) Width= 1;

  _ScreenWidth= Width;
  _ScreenHeight= Height;
} //eof Reshape



void ModelView::GenerateGrid() {
  int		i;
  
  //Grid Layout
  for (i= 0; i < GRID_XY+1; i++) {
    _GridV[i+(GRID_XY+1)*0][0]= float(i-GRID_XY/2);
    _GridV[i+(GRID_XY+1)*0][1]= float(GRID_XY/2);
    _GridV[i+(GRID_XY+1)*0][2]= float(0);
  } //eof

  for (i= 0; i < GRID_XY+1; i++) {
    _GridV[i+(GRID_XY+1)*1][0]= float(i-GRID_XY/2);
    _GridV[i+(GRID_XY+1)*1][1]= float(-GRID_XY/2);
    _GridV[i+(GRID_XY+1)*1][2]= float(0);
  } //eof

  for (i= 0; i < GRID_XY+1; i++) {
    _GridV[i+(GRID_XY+1)*2][0]= float(-GRID_XY/2);
    _GridV[i+(GRID_XY+1)*2][1]= float(i-GRID_XY/2);
    _GridV[i+(GRID_XY+1)*2][2]= float(0);
  } //eof

  for (i= 0; i < GRID_XY+1; i++) {
    _GridV[i+(GRID_XY+1)*3][0]= float(GRID_XY/2);
    _GridV[i+(GRID_XY+1)*3][1]= float(i-GRID_XY/2);
    _GridV[i+(GRID_XY+1)*3][2]= float(0);
  } //eof
} //eof ModelView::GenerateGridList()


void ModelView::Grid() {
  int	i;

  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
  for (i= 0; i < GRID_XY+1; i++) {
    if(i != GRID_XY/2){
      glColor3f(_ObjectList -> GRIDColor(0),_ObjectList -> GRIDColor(1),_ObjectList -> GRIDColor(2));
    } else {
      glColor3f(.1f,.2f,0.4f);
    } //fi
    glVertex3d(_GridV[i+(GRID_XY+1)*0][0],_GridV[i+(GRID_XY+1)*0][1],_GridV[i+(GRID_XY+1)*0][2]);
    glVertex3d(_GridV[i+(GRID_XY+1)*1][0],_GridV[i+(GRID_XY+1)*1][1],_GridV[i+(GRID_XY+1)*1][2]);

    glVertex3d(_GridV[i+(GRID_XY+1)*2][0],_GridV[i+(GRID_XY+1)*2][1],_GridV[i+(GRID_XY+1)*2][2]);
    glVertex3d(_GridV[i+(GRID_XY+1)*3][0],_GridV[i+(GRID_XY+1)*3][1],_GridV[i+(GRID_XY+1)*3][2]);
  } //eof
  glEnd();
  glEnable(GL_LIGHTING);  
} //eof ModelView::Grid()



void Axes(float AxesV[6][3]) {
  int i;
  
  glDisable(GL_LIGHTING);
    glBegin(GL_LINES);

    for (i= 0; i<6; i++) {
      if (i > 3) { glColor3f(0,0,2.0); }
      else if (i > 1) { glColor3d(0,2.0,0); }
      else { glColor3d(2.0,0,0); }
      glVertex3d(AxesV[i][0],AxesV[i][1],AxesV[i][2]);
    } //eof

    glEnd();
  glEnable(GL_LIGHTING);  
} //eof Axes()


void ModelView::Scene() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  ViewMode();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Apply Zoom
  glTranslatef(0,0,_ZoomLevel);
  // Apply Rotations
  rotate(_Angle);
  if (!_SM) glMultMatrixd(_m);
  // Apply Translations
  glTranslatef(_Tran[0],_Tran[1],_Tran[2]);
} //eof ModelView::Scene()


void ModelView::_DrawA() {
  bool	ZB_MODE;
  float	v;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// glClearDepth(1.0f);

  Scene();
  Grid();

  glEnable(GL_DEPTH_TEST);
  // Draw Grid

  ZB_MODE= _ObjectList -> GetMode(OBJECT_MODE_ZBUFFER);
  _ObjectList -> SetMode(OBJECT_MODE_ZBUFFER,_zBuffer);
  _ObjectList -> Draw();
  _ObjectList -> SetMode(OBJECT_MODE_ZBUFFER,ZB_MODE);

  // Draw Axes in Bottom Left Corner Subwindow
  if (_Axes) {
    glEnable(GL_SCISSOR_TEST);
      glViewport(_Viewport[0],_Viewport[1],60,60);
      glScissor(_Viewport[0],_Viewport[1],60,60);

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();

      v= 0.045; // 0.1 * tan(DtoR(45)*0.5)
      glFrustum(-v,v,-v,v,0.1,10);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      // Apply Zoom
      glTranslatef(0,0,-8);

      rotate(_Angle);
      if (!_SM) glMultMatrixd(_m);
      Axes(_AxesV);
    glDisable(GL_SCISSOR_TEST);
  } //fi


//  glDisable(GL_DEPTH_TEST);
//  if (_Axes) Axes(_AxesV);
//  glEnable(GL_DEPTH_TEST);
} //eof ModelView::_DrawA()


void ModelView::_DrawB() {
  bool	ZB_MODE;

  ZB_MODE= _ObjectList -> GetMode(OBJECT_MODE_ZBUFFER);
  _ObjectList -> SetMode(OBJECT_MODE_ZBUFFER,0);
  _ObjectList -> Draw(); // leave z-buffer 0 or it'll be slow as hell!
  _ObjectList -> SetMode(OBJECT_MODE_ZBUFFER,ZB_MODE);
} //eof ModelView::_DrawB()


void ModelView::_DrawC() {
  bool	ZB_MODE;

  ZB_MODE= _ObjectList -> GetMode(OBJECT_MODE_ZBUFFER);
  _ObjectList -> SetMode(OBJECT_MODE_ZBUFFER,0); // leave z-buffer 0 or it'll be slow as hell!
  _ObjectList -> SetMode(OBJECT_MODE_DRAWEDGES,1);
  _ObjectList -> Draw();
  _ObjectList -> SetMode(OBJECT_MODE_DRAWEDGES,0);
  _ObjectList -> SetMode(OBJECT_MODE_ZBUFFER,ZB_MODE);
} //eof ModelView::_DrawB()


void ModelView::Draw() {
  glClearColor(_ObjectList -> BGColor(0), _ObjectList -> BGColor(1), _ObjectList -> BGColor(2), 1.0);
  _DrawA();
  if (_SM) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    rotate(_Angle);
    if (!_SM) glMultMatrixd(_m);
    glGetDoublev(GL_MODELVIEW_MATRIX,_m);
    _SM= 0;
  } //fi
} //eof ModelView::Draw()



void ModelView::Zoom(float level) {
  if ((-_ZoomLevel*level > Z_CLIP_NEAR_MAX) && (-_ZoomLevel*level < Z_CLIP_FAR_MAX)) {
    _ZoomLevel*= level;
    _Z_CLIP_NEAR= -_ZoomLevel/100;
    _Z_CLIP_FAR= -_ZoomLevel*100;
  } //fi
} //eof ModelView:::Zoom()



void ModelView::Translate(int TransID, int x, int y) {
  double		objx,objy,objz;
  double		modelMatrix[16],projMatrix[16];
  Point3d		tempA,tempB,tempC,tempD;
  float			TranX=0,TranY=0,TranZ=0;
  unsigned	int	i,n,Total;
  bool			OldView;
  double		_MRM[16];
  double		_MRMB[16];

  y= _Viewport[3]-y;

  OldView= _View;
  _View= 0;
  Scene();
  _View= OldView;

  glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
  glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

  ProjectUtility::UnProject((double)x, (double)y, 1.0, modelMatrix, projMatrix, _Viewport, &objx, &objy, &objz);

  // Check control key whether to snap-to-grid
  objx/= (_Z_CLIP_FAR/-_ZoomLevel);
  objy/= (_Z_CLIP_FAR/-_ZoomLevel);
  objz/= (_Z_CLIP_FAR/-_ZoomLevel);


  if (_ObjectList -> Init()) {
    _ObjectList -> Init(0);

    _ObjectList -> CopyList(OBJECT_LIST_BACKUP,OBJECT_LIST_MAIN,1);

    _Last[0][0]= objx;
    _Last[0][1]= objy;
    _Last[0][2]= objz;

    _Last[1][0]= _Tran[0];
    _Last[1][1]= _Tran[1];
    _Last[1][2]= _Tran[2];
  } //fi


  if (KeyTable::Key() == SDLK_LCTRL || KeyTable::Key() == SDLK_RCTRL || KeyTable::Mod(KMOD_CTRL)) {
    if (KeyTable::Key() == SDLK_LSHIFT || KeyTable::Key() == SDLK_RSHIFT || KeyTable::Mod(KMOD_SHIFT)) {
      TranX= int(10*(objx - _Last[0][0]))/10.0;
      TranY= int(10*(objy - _Last[0][1]))/10.0;
      TranZ= int(10*(objz - _Last[0][2]))/10.0;
    } else {
      TranX= int(objx - _Last[0][0] + (objx - _Last[0][0] > 0 ? 1 : -1)*0.5);
      TranY= int(objy - _Last[0][1] + (objy - _Last[0][1] > 0 ? 1 : -1)*0.5);
      TranZ= int(objz - _Last[0][2] + (objy - _Last[0][1] > 0 ? 1 : -1)*0.5);
    } //fi
  } else {
    TranX= objx - _Last[0][0];
    TranY= objy - _Last[0][1];
    TranZ= objz - _Last[0][2];
  } //fi


  // Translate Scene
  if (TransID == 1) {
    _Tran[0]= TranX + _Last[1][0];
    _Tran[1]= TranY + _Last[1][1];
    _Tran[2]= TranZ + _Last[1][2];
  } //fi


  // TRANSLATE NURBS

    // FOR UPDATING COR
    if (TransID == 2) {
      if (!_ObjectList -> GetMode(OBJECT_MODE_EDIT)) {
        _ObjectList -> SelectList(OBJECT_LIST_BACKUP);
        tempB= _ObjectList -> GetCOR();
        _ObjectList -> SelectList(OBJECT_LIST_MAIN);
        tempB.x+= TranX;
        tempB.y+= TranY;
        tempB.z+= TranZ;
        _ObjectList -> SetCOR(tempB);
      } //fi
    } //fi
    // EOF UPDATING COR


    // Obtain models current rotation matrix (FROM BACKUP)
    _ObjectList -> SelectList(OBJECT_LIST_BACKUP);
    for (i= 0; i < 16; i++) _MRM[i]= _ObjectList -> GetMRM()[i];

    // Invert Models Matrix
    VertexOps::InvertMatrix(_MRMB,_MRM);

    // Model Size
    tempA= _ObjectList -> GetSIZ();

    tempC.x= TranX;
    tempC.y= TranY;
    tempC.z= TranZ;
    VertexOps::MultPtMatrix(tempD,tempC,_MRMB);
    tempC.x= tempD.x/tempA.x;
    tempC.y= tempD.y/tempA.y;
    tempC.z= tempD.z/tempA.z;
    _ObjectList -> SelectList(OBJECT_LIST_MAIN);

    for (i= 0; i < _ObjectList -> GetLength(0) * _ObjectList -> GetLength(1); i++) {
      if (TransID == 2) {
        if (_ObjectList -> GetMode(OBJECT_MODE_EDIT)) {
          // Move just selected vertice(s)
          if (_ObjectList -> GetCPAct(i)) {

            // Obtain Vertice
            _ObjectList -> SelectList(OBJECT_LIST_BACKUP);
            tempB= _ObjectList -> GetCP(i);
            tempB.x+= tempC.x;
            tempB.y+= tempC.y;
            tempB.z+= tempC.z;

            _ObjectList -> SelectList(OBJECT_LIST_MAIN);
            _ObjectList -> SetCP(i,tempB);

          } //fi
        } //fi
      } //fi
    } //eof
} //eof ModelView::Translate()



char ModelView::Key(int x,int y) {
//  printf("key:%d\n",key);
  switch( KeyTable::Key() ) {
    case SDLK_ESCAPE:
      {
        _ObjectList -> CopyList(OBJECT_LIST_MAIN,OBJECT_LIST_BACKUP,0);
        bool EditMode= _ObjectList -> GetMode(OBJECT_MODE_EDIT);
        _ObjectList -> SetModeReset();
        _ObjectList -> SetMode(OBJECT_MODE_EDIT, EditMode);
      }
      break;

    case SDLK_a:
      if (_ObjectList -> GetCPActID()) {
        _ObjectList -> SetCPActAll(0);
      } else {
        _ObjectList -> SetCPActAll(1);
      } //fi
      break;

    case SDLK_c:
      if (_ObjectList -> GetActiveID()) {
        _Tran[0]= -(_ObjectList -> GetCOR().x);
        _Tran[1]= -(_ObjectList -> GetCOR().y);
        _Tran[2]= -(_ObjectList -> GetCOR().z);
      } //fi
      break;

    case SDLK_w:
//      _ObjectList -> Subdivide(_ObjectList -> ActiveObject());
      break;

    case SDLK_z: //z [Toggle zBuffer]
      _zBuffer= !_zBuffer;
      break;

    case SDLK_KP_MULTIPLY: //* [Toggle Axes]
      _Axes= !_Axes;
      break;

    case SDLK_KP_PLUS: //+ [Zoom in]
      Zoom(float(1/1.025));
      break;

    case SDLK_KP_MINUS: //- [Zoom out]
      Zoom(float(1.025));
      break;

    case SDLK_KP5: //5 [Toggle Persp/Ortho]
      _View= !_View;
      ViewMode();
      break;

    case SDLK_KP9: //9 [Reset Scene]
      _Angle[0]= _Angle[1]= _Angle[2]= 0;
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      ViewMode();
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glGetDoublev(GL_MODELVIEW_MATRIX,_m);
//      _ZoomLevel= -20;
      glTranslatef(0,0,_ZoomLevel);
      _Tran[0]= _Tran[1]= _Tran[2]= 0;
      break;

    case SDLK_KP7: //7 [Top View]
      _Angle[0]= _Angle[1]= _Angle[2]= 0;
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      ViewMode();
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glGetDoublev(GL_MODELVIEW_MATRIX,_m);
      glTranslatef(0,0,_ZoomLevel);
      break;

    case SDLK_KP1: //1 [Front View]
      _Angle[0]= _Angle[1]= _Angle[2]= 0;
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      ViewMode();
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glRotatef(-90,1,0,0);
      glGetDoublev(GL_MODELVIEW_MATRIX,_m);
      glTranslatef(0,0,_ZoomLevel);
      break;

    case SDLK_KP3: //3 [Side View]
      _Angle[0]= _Angle[1]= _Angle[2]= 0;
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      ViewMode();
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glRotatef(-90,0,0,1);
      glRotatef(-90,0,1,0);
      glGetDoublev(GL_MODELVIEW_MATRIX,_m);
      glTranslatef(0,0,_ZoomLevel);
      break;

    case SDLK_KP2: //2 [Rotate down]
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
        rotate(_Angle);
      glRotatef(-MV_KEY_ROTATION_ANGLE,1,0,0);
        glMultMatrixd(_m);
      glGetDoublev(GL_MODELVIEW_MATRIX,_m);
      break;

    case SDLK_KP6: //6 [Rotate left]
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
        rotate(_Angle);
        glMultMatrixd(_m);
      glRotatef(MV_KEY_ROTATION_ANGLE,0,0,1);
      glGetDoublev(GL_MODELVIEW_MATRIX,_m);
      break;

    case SDLK_KP4: //4 [Rotate right]
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
        rotate(_Angle);
        glMultMatrixd(_m);
      glRotatef(-MV_KEY_ROTATION_ANGLE,0,0,1);
      glGetDoublev(GL_MODELVIEW_MATRIX,_m);
      break;

    case SDLK_KP8: //8 [Rotate up]
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
        rotate(_Angle);
      glRotatef(MV_KEY_ROTATION_ANGLE,1,0,0);
        glMultMatrixd(_m);
      glGetDoublev(GL_MODELVIEW_MATRIX,_m);
      break;

    case SDLK_b:
//      printf("VP: %d,%d,%d,%d\n",_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3],_ScreenWidth,_ScreenHeight);
//      BorderSelect::Initialize(_Viewport[0],(_ScreenHeight-_Viewport[1])-_Viewport[3],_Viewport[2],_Viewport[3],_ScreenWidth,_ScreenHeight);
      BorderSelect::Initialize(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
      _ObjectList -> SetMode(OBJECT_MODE_BORDERSELECT, 1);
      return(NWM_LOCK | NWM_UPDATEOFF);
      break;

    case SDLK_d:
      {
      if (KeyTable::Mod(KMOD_SHIFT)) {
        if (_ObjectList -> GetActiveID()) {
          _ObjectList -> SetMode(OBJECT_MODE_GRAB, 1);
          _ObjectList -> Init(1);
          _ObjectList -> Duplicate();
          _ObjectList -> CopyList(OBJECT_LIST_BACKUP,OBJECT_LIST_MAIN,1);
          _ObjectList -> SetObjActAll(0);
//          _ObjectList -> SetActive(1);
        } //fi
      } //fi
      } //eoc
      break;


    case SDLK_g:
      if (_ObjectList -> GetActiveID()) {
        if (_ObjectList -> CPActCount() || !_ObjectList -> GetMode(OBJECT_MODE_EDIT)) {
//        printf("border: %d\n",_ObjectList -> Mode(OBJECT_MODE_BORDERSELECT));
          _ObjectList -> SetMode(OBJECT_MODE_GRAB, 1);
//        printf("border: %d\n",_ObjectList -> Mode(OBJECT_MODE_BORDERSELECT));
         _ObjectList -> Init(1);
           _ObjectList -> CopyList(OBJECT_LIST_BACKUP,OBJECT_LIST_MAIN,1);
          return(NWM_LOCK);
        } //fi
      } //fi
      break;

    case SDLK_r:
      if (_ObjectList -> GetActiveID()) {
        _ObjectList -> SetMode(OBJECT_MODE_ROTATE, 1);
        _ObjectList -> Init(1);
        _ObjectList -> CopyList(OBJECT_LIST_BACKUP,OBJECT_LIST_MAIN,1);
        return(NWM_LOCK);
      } //fi
      break;

    case SDLK_t:
      {
/*
        glPushMatrix();
        NTimer::Initialize(5);
        while(NTimer::Check()) {
          _DrawA();
        } //eow
        glPopMatrix();
        printf("fps: %f\n",NTimer::Count()/5.0);
*/
      }
      break;

    case SDLK_s:
      if (_ObjectList -> GetActiveID()) {
        _ObjectList -> SetMode(OBJECT_MODE_SIZE, 1);
        _ObjectList -> Init(1);
        _ObjectList -> CopyList(OBJECT_LIST_BACKUP,OBJECT_LIST_MAIN,1);
      } //fi
      break;

    case SDLK_TAB:
      if (KeyTable::Mod(KMOD_SHIFT)) {
        if (_ObjectList -> GetMode(OBJECT_MODE_EDGE)) {
          _ObjectList -> SetMode(OBJECT_MODE_EDIT, 0);
        } else {
          _ObjectList -> SetMode(OBJECT_MODE_EDIT, 1);
        } //fi
      } else {
        if (_ObjectList -> GetActiveID()) {
          _ObjectList -> SetCPActAll(0);
          if (_ObjectList -> GetMode(OBJECT_MODE_EDIT)) {
            _ObjectList -> SetMode(OBJECT_MODE_EDIT, 0);
//            glutSetCursor(GLUT_CURSOR_INHERIT);
            Cursors::SetCursor(CURSOR_ARROW);
          } else {
            _ObjectList -> SetMode(OBJECT_MODE_EDIT, 1);
//            glutSetCursor(GLUT_CURSOR_CROSSHAIR);
            Cursors::SetCursor(CURSOR_CROSSHAIR);
          } //fi
          _ObjectList -> CopyList(OBJECT_LIST_BACKUP,OBJECT_LIST_MAIN,1);
        } //fi
      } //fi
      break;

    case SDLK_RETURN:
      break;

    default:
      break;

  } //eos

  return(NWM_NOTHING);
} //eof ModelView::Key()



void ModelView::processHitsOne(int hits, int *buffer) {
   int i,Closest[2];

   if (!_ObjectList -> GetMode(OBJECT_MODE_EDIT))
     if (KeyTable::Key() != SDLK_LSHIFT && KeyTable::Key() != SDLK_RSHIFT)
       _ObjectList -> SetObjActAll(0);

   // If there are hits and the shift key is not held down
   if (hits && KeyTable::Key() != SDLK_LSHIFT && KeyTable::Key() != SDLK_RSHIFT) _ObjectList -> SetCPActAll(0);
   if ((!hits) && (!_ObjectList -> GetMode(OBJECT_MODE_EDIT))) _ObjectList -> SetActiveID(0);
   // If we're not in edit-mode assign the Active Object

   // Prevent the Extra ID thrown in for "everything else" from interfering with the # of
   // verts if in edit mode
   Closest[1]= 0;
   for (i= 0; i < hits; i++) {
     if (_ObjectList -> GetMode(OBJECT_MODE_EDIT))
       if (buffer[i*4+3] > _ObjectList -> GetLength(0) * _ObjectList -> GetLength(1))
         i++;

     if (i < hits)
       if ((buffer[i*4+1] < Closest[0] || !Closest[1]) && buffer[i*4+3] != 0) {
         Closest[0]= buffer[i*4+1];
         Closest[1]= buffer[i*4+3];
       } //fi
   } //eof

   if (Closest[1]) {
     if (_ObjectList -> GetMode(OBJECT_MODE_EDIT)) {
       if (KeyTable::Mod(KMOD_SHIFT)) {
         _ObjectList -> SetCPAct(Closest[1]-1,!_ObjectList -> GetCPAct(Closest[1]-1));
       } else {
         _ObjectList -> SetCPAct(Closest[1]-1,1);
       } //fi
     } else {
       _ObjectList -> SetActiveID(Closest[1]);
       if (KeyTable::Key() == SDLK_LSHIFT || KeyTable::Key() == SDLK_RSHIFT) {
         _ObjectList -> SetActive(Closest[1],0);
       } else {
         _ObjectList -> SetActive(Closest[1],1);
       } //fi
     } //fi
   } //fi
} //eof processHitsOne()



void ModelView::processHitsRegion(int hits, int *buffer) {
   int i,SORT_LIST[MAX_OBJECTS][2];

   // If there are hits and the shift key is not held down
   if ((!hits) && (!_ObjectList -> GetMode(OBJECT_MODE_EDIT))) _ObjectList -> SetActiveID(0);
   // If we're not in edit-mode assign the Active Object

   // Distance, ID
   for (i= 0; i < hits; i++) {
     SORT_LIST[i][0]= buffer[i*4+1];
     SORT_LIST[i][1]= buffer[i*4+3];
   } //eof

   for (i= 0; i < hits; i++) {
     if (SORT_LIST[i][1]!=0) {
       if (_ObjectList -> GetMode(OBJECT_MODE_EDIT))  { 
         if (BorderSelect::MouseButton() == SDL_BUTTON_MIDDLE) {
           _ObjectList -> SetCPAct(SORT_LIST[i][1]-1,0);
         } else {
           _ObjectList -> SetCPAct(SORT_LIST[i][1]-1,1);
         } //fi
       } else {
         if (BorderSelect::MouseButton() == SDL_BUTTON_MIDDLE) {
           _ObjectList -> SetActive(0,0);
         } else {
           _ObjectList -> SetActiveID(SORT_LIST[i][1]);
           _ObjectList -> SetActive(1,0);
         } //fi
       } //fi
     } //fi
   } //eof   
} //eof processHitsRegion()


void ModelView::processHitsEdge(int hits, int *buffer) {
   unsigned	int	ID;
   int			i,Closest[2];

   // Prevent the Extra ID thrown in for "everything else" from interfering with the # of
   // verts if in edit mode
   Closest[1]= 0;
   for (i= 0; i < hits; i++) {
     if (i < hits)
       if ((buffer[i*4+1] < Closest[0] || !Closest[1]) && buffer[i*4+3] != 0) {
         Closest[0]= buffer[i*4+1];
         Closest[1]= buffer[i*4+3];
       } //fi
   } //eof

//   printf("Hits: %d, Closest: %d, Object ID: %d, Side: %d\n",hits, Closest[1], Closest[1]/4, Closest[1]%4);

   // Requires integer division
   ID= _ObjectList -> GetActiveID();
   _ObjectList -> SetActiveID(Closest[1]/4);
   _ObjectList -> SetEdge(Closest[1]%4,!_ObjectList -> GetEdge(Closest[1]%4));
   _ObjectList -> SetActiveID(ID);
} //eof ModelView::processHitsEdge()


void ModelView::Select(int x, int y, int SelWidth, int SelHeight, int Type) {
  int	selectBuf[BUFSIZE];
  int	hits;

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0,0,_ZoomLevel);
  rotate(_Angle);
  if (!_SM) glMultMatrixd(_m);
  glTranslatef(_Tran[0],_Tran[1],_Tran[2]);

   
  glSelectBuffer(BUFSIZE, (GLuint*)selectBuf);
  glRenderMode(GL_SELECT);
  glInitNames();
  glPushName(0);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();


   //Selection is based on the viewport sent, and bottom,left=0 basis
//   ProjectUtility::PickMatrix((GLdouble) x, (GLdouble) (ModelView::viewport[3] - y + ModelView::viewport[1]), SelWidth, SelHeight, ModelView::viewport);
  ProjectUtility::PickMatrix((GLdouble) (_Viewport[0] + x), (GLdouble) (_Viewport[3] - y + _Viewport[1]), SelWidth, SelHeight, _Viewport);
  ViewMode();

  if (Type == 2) {
    _DrawC();
  } else {
    _DrawB();
  } //fi

  glPopMatrix();


  hits= glRenderMode(GL_RENDER);
//   printf("hits: %d\n", hits);
  if (hits) {
    switch( Type ) {
      case 0:
        processHitsRegion(hits, selectBuf);
        break;

      case 1:
        processHitsOne(hits, selectBuf);
        break;

      case 2:
        processHitsEdge(hits, selectBuf);
        break;

      default:
        break;
    } //eos
  } //fi
} //eof ModelView::Select()




char ModelView::Mouse(int button, int state, int x, int y) {
  int		Data[4];
  bool		EditMode;


  if (state) {
    if (button == SDL_BUTTON_MIDDLE) {
      if (!_ObjectList -> GetMode(OBJECT_MODE_BORDERSELECT)) {
        if (KeyTable::Key() == SDLK_LSHIFT || KeyTable::Key() == SDLK_RSHIFT) {
          _ObjectList -> Init(1);
          _ObjectList -> SetMode(OBJECT_MODE_TRANSLATE, 1);
          return(NWM_LOCK);
        } else {
          _ObjectList -> SetMode(OBJECT_MODE_TRACKBALL, 1);
          if (_ObjectList -> Trackball()) {
            Trackball::TrackballA(x, y, _Viewport[2], _Viewport[3], 1, _Angle, _ObjectList -> TBSpeed());
          } else {
            Trackball::TrackballB(x, y, 1, _Angle, _m, _ObjectList -> TBSpeed());
          } //fi
        } //fi
      } else {
        BorderSelect::Anchor(SDL_BUTTON_MIDDLE);
      } //fi
    } else if ((button == SDL_BUTTON_LEFT) && (KeyTable::Key() == SDLK_LALT || KeyTable::Key() == SDLK_RALT)) {
      _ObjectList -> SetMode(OBJECT_MODE_TRACKBALL, 1);
      if (_ObjectList -> Trackball()) {
        Trackball::TrackballA(x, y, _Viewport[2], _Viewport[3], 1, _Angle, _ObjectList -> TBSpeed());
      } else {
        Trackball::TrackballB(x, y, 1, _Angle, _m, _ObjectList -> TBSpeed());
      } //fi
    } else if (button == SDL_BUTTON_RIGHT) {
      if (KeyTable::Key() == SDLK_LCTRL || KeyTable::Key() == SDLK_RCTRL) {
        Select(x, y, 25, 25, 2);
      } else {
        Select(x, y, 25, 25, 1);
      } //fi
    } else if (KeyTable::Mod(KMOD_SHIFT) && ((KeyTable::Mod(KMOD_ALT) && button == SDL_BUTTON_LEFT) || (button == SDL_BUTTON_MIDDLE))) {
      _ObjectList -> SetMode(OBJECT_MODE_TRANSLATE, 1);
      _ObjectList -> Init(1);
    } else if ((button == SDL_BUTTON_LEFT) && KeyTable::Key() != SDLK_LALT && KeyTable::Key() != SDLK_RALT) {
//      _ObjectList -> ModeReset();
      if (_ObjectList -> GetMode(OBJECT_MODE_BORDERSELECT)) {
        BorderSelect::Anchor(SDL_BUTTON_LEFT);
      } else {
        //Turn Modes off / Finalize etc...
        EditMode= _ObjectList -> GetMode(OBJECT_MODE_EDIT);
        _ObjectList -> SetModeReset();
        _ObjectList -> SetMode(OBJECT_MODE_EDIT, EditMode);
        _ObjectList -> SetRecalcTess();
        return(NWM_UNLOCK);
      } //fi
    } //fi
  } else if (!state) {
    if (_ObjectList -> GetMode(OBJECT_MODE_BORDERSELECT)) {
      _ObjectList -> SetMode(OBJECT_MODE_BORDERSELECT, 0);
      BorderSelect::GetRegion(Data);
      MouseBorderSelect(Data);
      return(NWM_UNLOCK | NWM_REFRESH);
    } else {
      if (!_ObjectList -> GetMode(OBJECT_MODE_GRAB)) {
        if (_ObjectList -> GetMode(OBJECT_MODE_EDIT)) {
          _ObjectList -> SetModeReset();
          _ObjectList -> SetMode(OBJECT_MODE_EDIT, 1);
        } else {
          _ObjectList -> SetModeReset();
        } //fi
      } //fi
      if (!_SM) {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        rotate(_Angle);

        glMultMatrixd(_m);

        glGetDoublev(GL_MODELVIEW_MATRIX,_m);
        _Angle[0]= _Angle[1]= _Angle[2]= 0;
      } //fi
    } //fi
    return(NWM_UNLOCK);
  } //fi

  return(NWM_NOTHING);
} //ModelView::Mouse()


char ModelView::Motion(int x, int y) {
  if (_ObjectList -> GetMode(OBJECT_MODE_TRACKBALL)) {
    if (_ObjectList -> Trackball()) {
      Trackball::TrackballA(x, y, _Viewport[2], _Viewport[3], 0, _Angle, _ObjectList -> TBSpeed());
    } else {
      Trackball::TrackballB(x, y, 0, _Angle, _m, _ObjectList -> TBSpeed());
    } //fi
  } else if (_ObjectList -> GetMode(OBJECT_MODE_TRANSLATE)) {
    Translate(1,x,y);
  } else if (_ObjectList -> GetMode(OBJECT_MODE_BORDERSELECT)) {
    BorderSelect::Update(x,y);
  } //fi

  return(NWM_NOTHING);
} //eof Motion()



char ModelView::Passive(int x, int y) {
  if (_ObjectList -> GetMode(OBJECT_MODE_GRAB)) {
    MouseGrab(x,y);
    return(1);
  } else if (_ObjectList -> GetMode(OBJECT_MODE_ROTATE)) {
    MouseRotate(x,y);
    return(1);
  } else if (_ObjectList -> GetMode(OBJECT_MODE_SIZE)) {
    MouseSize(x,y);
    return(1);
  } else if (_ObjectList -> GetMode(OBJECT_MODE_BORDERSELECT)) {
//    BorderSelect::Update(x,y+((_ScreenHeight-_Viewport[1])-_Viewport[3]));
    BorderSelect::Update(x,y);
    return(1);
  } //fi

  return(NWM_NOTHING);
} //eof ModelView::Passive()



void ModelView::Init() {
  int	i;

  GenerateGrid();


  //Axes Layout
  for (i= 0; i < 6; i++) {
    _AxesV[i][0]= 0;
    _AxesV[i][1]= 0;
    _AxesV[i][2]= 0;
  } //eof
  _AxesV[1][0]= _AxesV[3][1]= _AxesV[5][2]= 3;
} //eof ModelView::Init()


void ModelView::MouseZoom(int Z) {
  if (Z >= 0) {
    Zoom(float(1.0+abs(Z)/2500.0));
  } else {
    Zoom(float(1.0/(1.0+abs(Z)/2500.0)));
  } //fi
} //eof MouseZoom()



void ModelView::MouseTranslate(int x, int y, bool init) {
  if (init)
    _ObjectList -> Init(1);
  Translate(1,x,y);
} //eof MouseTranslate()



void ModelView::MouseGrab(int x, int y) {
  Translate(2,x,y);
} //eof ModelView::MouseGrab()



void ModelView::MouseRotate(int x, int y) {
  double	winX,winY,winZ;
  double	modelMatrix[16],projMatrix[16];
  double	PX,NX,PY,NY,PZ,NZ;
  double	_m2[16];
  double	_mb[16];
  double	_zM[16];
  double	_MRM[16];
  double	_MRMB[16];

  Point3d	tempA,tempB,tempC,ofsetA,COR;
  float		vCOR[3];
  float		angleSwept;
  int		i,n,k;


  // Initialize the Scene
  Scene();

  //Get the inverted matrix for _m
  VertexOps::InvertMatrix(_m2,_m);
  
  y= _Viewport[3]-y;

  glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
  glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);


  if (_ObjectList -> Init()) {
    _ObjectList -> Init(0);
    _Last[0][0]= x;
    _Last[0][1]= y;
  } //fi



  if (_ObjectList -> GetMode(OBJECT_MODE_EDIT)) {
    // Rotate Vertices instead of objects by creating a virtual COR by gathering the largest +X,-X,+Y,-Y
    // Then LX+-SX= VCX, LY+-SY= VCY, now you have x,y for VCOR with Z whatever...

    ofsetA.x= 0; ofsetA.y= 0; ofsetA.z= 0;
    vCOR[0]= vCOR[1]= vCOR[2]= n= 0;
    PX= NX= PY= NY= PZ= NZ= 0;
  
    for (i= 0; i < _ObjectList -> GetLength(0)*_ObjectList -> GetLength(1); i++) {
      if (_ObjectList -> GetCPAct(i)) {
        _ObjectList -> SelectList(OBJECT_LIST_BACKUP);

        // Get the Point
        tempB= _ObjectList -> GetCP(i);

        // Get MRM and Mult Vert against it's MRM to get actual 3d location
        for (k= 0; k < 16; k++) _MRM[k]= _ObjectList -> GetMRM()[k];
        VertexOps::MultPtMatrix(tempA,tempB,_MRM);

        if (!n) {
          PX= NX= tempA.x;
          PY= NY= tempA.y;
          PZ= NZ= tempA.z;
        } //fi
        if (tempA.x > PX) PX= tempA.x;
        if (tempA.x < NX) NX= tempA.x;
        if (tempA.y > PY) PY= tempA.y;
        if (tempA.y < NY) NY= tempA.y;
        if (tempA.z > PZ) PZ= tempA.z;
        if (tempA.z < NZ) NZ= tempA.z;

        n++; //Are there any active vertices?  make this bool or something
      } //fi
    } //eof

    if (n) { //if there were any selected vertices
      vCOR[0]= (PX+NX)/2;
      vCOR[1]= (PY+NY)/2;
      vCOR[2]= (PZ+NZ)/2;

      //Now that we have the vCOR, subtract that from each Vertex and rotate them
      //First Determine where the COR is located on the screen using

      ProjectUtility::Project(vCOR[0],vCOR[1],vCOR[2],modelMatrix,projMatrix,_Viewport,&winX,&winY,&winZ);

      winY= winY - _Viewport[1];
      angleSwept= NurbanaMath::AngleSwept(_Last[0][0]-winX,_Last[0][1]-winY,x-winX,y-winY);


      // Incremental 5 degree rotation
      if (KeyTable::Key() == SDLK_LCTRL || KeyTable::Key() == SDLK_RCTRL)
        angleSwept= int(angleSwept) - int(angleSwept)%5;

      for (i= 0; i < _ObjectList -> GetLength(0)*_ObjectList -> GetLength(1); i++) {
        if (_ObjectList -> GetCPAct(i)) {

          _ObjectList -> SelectList(OBJECT_LIST_BACKUP);
          tempA= _ObjectList -> GetCP(i);

          // Obtain models current rotation matrix (FROM BACKUP)
          for (k= 0; k < 16; k++) _MRM[k]= _ObjectList -> GetMRM()[k];
          VertexOps::InvertMatrix(_MRMB,_MRM);

          // Apply the effective rotations to the new matrix
          VertexOps::MultPtMatrix(tempB,tempA,_MRM);

          tempB.x-= vCOR[0];
          tempB.y-= vCOR[1];
          tempB.z-= vCOR[2];

          tempB.x+= _ObjectList -> GetCOR().x;
          tempB.y+= _ObjectList -> GetCOR().y;
          tempB.z+= _ObjectList -> GetCOR().z;

           // Copy in the inverted ModelView matrix
          for (n= 0; n < 16; n++) _mb[n]= _m2[n];

          // Apply the Rotation
          VertexOps::MakeZMatrix(_zM,NurbanaMath::DtoR(angleSwept));

          VertexOps::MultMatrix(_mb,_zM);

          // Revert the matrix
          VertexOps::MultMatrix(_mb,_m);

          // Take the REGULAR rotation matrix and multiply it against the point
          VertexOps::MultPtMatrix(tempA,tempB,_mb);

          tempA.x+= vCOR[0];
          tempA.y+= vCOR[1];
          tempA.z+= vCOR[2];

          tempA.x-= _ObjectList -> GetCOR().x;
          tempA.y-= _ObjectList -> GetCOR().y;
          tempA.z-= _ObjectList -> GetCOR().z;

          VertexOps::MultPtMatrix(tempB,tempA,_MRMB);

          //Update Object's Pt
          _ObjectList -> SelectList(OBJECT_LIST_MAIN);

          _ObjectList -> SetCP(i,tempB);
        } //fi
      } //eof
    } //fi (anything selected)

  } else { // Not in Edit-Mode

    COR= _ObjectList -> GetCOR();

    ProjectUtility::Project(COR.x,COR.y,COR.z,modelMatrix,projMatrix,_Viewport,&winX,&winY,&winZ);
    winY= winY-_Viewport[1];
    angleSwept= NurbanaMath::AngleSwept(_Last[0][0]-winX,_Last[0][1]-winY,x-winX,y-winY);

    // Incremental 5 degree rotation
    if (KeyTable::Key() == SDLK_LCTRL || KeyTable::Key() == SDLK_RCTRL)
      angleSwept= int(angleSwept) - int(angleSwept)%5;

    // Copy in the inverted ModelView matrix
    for (i= 0; i < 16; i++) _mb[i]= _m2[i];

    // Apply the Rotation
    VertexOps::MakeZMatrix(_zM,NurbanaMath::DtoR(angleSwept));
    VertexOps::MultMatrix(_mb,_zM);

    // Revert the matrix
    VertexOps::MultMatrix(_mb,_m);

    // Obtain models current rotation matrix (FROM BACKUP)
    _ObjectList -> SelectList(OBJECT_LIST_BACKUP);
    for (i= 0; i < 16; i++) _MRM[i]= _ObjectList -> GetMRM()[i];

    // Apply the effective rotations to the new matrix
    VertexOps::MultMatrix(_mb,_MRM);

    // Store the new rotation matrix (TO MAIN)
    _ObjectList -> SelectList(OBJECT_LIST_MAIN);
    _ObjectList -> SetMRM(_mb);

    _ObjectList -> SetRecalcWCTess();
  } //fi

} //eof ModelView::MouseRotate()



void ModelView::MouseSize(int x, int y) {
  Point3d	tempA, tempB,ofsetA,COR;
  double	LengthA, LengthB, vCOR[3];
  int		i,n;
  double	winX,winY,winZ;
  double	PX,NX,PY,NY,PZ,NZ;
  double	modelMatrix[16],projMatrix[16];

  if (_ObjectList -> Init()) {
    _ObjectList -> Init(0);

    _Last[0][0]= x;
    _Last[0][1]= y;
  } //fi


  Scene();

  glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
  glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

  if (_ObjectList -> GetMode(OBJECT_MODE_EDIT)) {
    //Rotate Vertices instead of objects by creating a virtual COR from averaging each
    //of the selected vertices x and y locations
    ofsetA.x=0; ofsetA.y=0; ofsetA.z=0;
    vCOR[0]= vCOR[1]= vCOR[2]= n= 0;
    PX= NX= PY= NY= PZ= NZ= 0;
    _ObjectList -> SelectList(OBJECT_LIST_BACKUP);
    for (i= 0; i < _ObjectList -> GetLength(0)*_ObjectList -> GetLength(1); i++) {
      if (_ObjectList -> GetCPAct(i)) {
        tempA= _ObjectList -> GetCP(i);
        if (!n) {
          PX= NX= tempA.x;
          PY= NY= tempA.y;
          PZ= NZ= tempA.z;
        } //fi
        if (tempA.x > PX) PX= tempA.x;
        if (tempA.x < NX) NX= tempA.x;
        if (tempA.y > PY) PY= tempA.y;
        if (tempA.y < NY) NY= tempA.y;
        if (tempA.z > PZ) PZ= tempA.z;
        if (tempA.z < NZ) NZ= tempA.z;
        n++; //Are there any active vertices?  make this bool or something
      } //fi
    } //eof

    if (n) { //if there were any selected vertices
      //Use this vCOR and SUBTRACT it from each CP as sizing takes place.
      vCOR[0]= (PX+NX)/2;
      vCOR[1]= (PY+NY)/2;
      vCOR[2]= (PZ+NZ)/2;

      y= _Viewport[3]-y;

      tempA= _ObjectList -> GetCOR();
      vCOR[0]+= tempA.x;
      vCOR[1]+= tempA.y;
      vCOR[2]+= tempA.z;

      ProjectUtility::Project(vCOR[0],vCOR[1],vCOR[2],modelMatrix,projMatrix,_Viewport,&winX,&winY,&winZ);

      vCOR[0]-= tempA.x;
      vCOR[1]-= tempA.y;
      vCOR[2]-= tempA.z;

      winY= winY-_Viewport[1];

      // Use Pythagorean Thereom c^2 = a^2 + b^2 to get lines hypotenuse length
      LengthA= sqrt(fabs(_Last[0][0]-winX)*fabs(_Last[0][0]-winX) + fabs(_Last[0][1]-winY)*fabs(_Last[0][1]-winY));
      LengthB= sqrt(fabs(x-winX)*fabs(x-winX) + fabs(y-winY)*fabs(y-winY));

      // Now that virtual COR has been calculated, and LengthA/B of size vector, transfer delta size
      // to Backup pts and update those in the main list.

      for (i= 0; i < _ObjectList -> GetLength(0) * _ObjectList -> GetLength(1); i++) {
        if (_ObjectList -> GetCPAct(i)) {
          _ObjectList -> SelectList(OBJECT_LIST_BACKUP);
          tempA= _ObjectList -> GetCP(i);
          tempA.x-= vCOR[0];
          tempA.y-= vCOR[1];
          tempA.z-= vCOR[2];

           if (KeyTable::Key() == SDLK_LCTRL || KeyTable::Key() == SDLK_RCTRL) {
              tempA.x*= int(10*LengthB/LengthA)/10.0;
              tempA.y*= int(10*LengthB/LengthA)/10.0;
              tempA.z*= int(10*LengthB/LengthA)/10.0;
           } else {
              tempA.x*= (LengthB/LengthA);
              tempA.y*= (LengthB/LengthA);
              tempA.z*= (LengthB/LengthA);
           } //fi
           tempA.x+= vCOR[0];
           tempA.y+= vCOR[1];
           tempA.z+= vCOR[2];
           _ObjectList -> SelectList(OBJECT_LIST_MAIN);
           _ObjectList -> SetCP(i,tempA);
        } //fi
      } //eof
    } //fi
  } else {
    COR= _ObjectList -> GetCOR();

    y= _Viewport[3]-y;
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

    ProjectUtility::Project(COR.x,COR.y,COR.z,modelMatrix,projMatrix,_Viewport,&winX,&winY,&winZ);
    winY= winY-_Viewport[1];


    // Use Pythagorean Thereom c^2 = a^2 + b^2 to get lines hypotenuse length
    LengthA= sqrt(fabs(_Last[0][0]-winX)*fabs(_Last[0][0]-winX) + fabs(_Last[0][1]-winY)*fabs(_Last[0][1]-winY));
    LengthB= sqrt(fabs(x-winX)*fabs(x-winX) + fabs(y-winY)*fabs(y-winY));

    _ObjectList -> SelectList(OBJECT_LIST_BACKUP);
    tempB= _ObjectList -> GetSIZ();
    _ObjectList -> SelectList(OBJECT_LIST_MAIN);

    if (KeyTable::Key() == SDLK_LCTRL || KeyTable::Key() == SDLK_RCTRL) {
      // Size Incremental by .1 int(x*10)/10.0
      tempA.x= int(LengthB/LengthA*10)/10.0*tempB.x;
      tempA.y= int(LengthB/LengthA*10)/10.0*tempB.y;
      tempA.z= int(LengthB/LengthA*10)/10.0*tempB.z;
      _ObjectList -> SetSIZ(tempA);
    } else {
      tempA.x= (LengthB/LengthA*10)/10.0*tempB.x;
      tempA.y= (LengthB/LengthA*10)/10.0*tempB.y;
      tempA.z= (LengthB/LengthA*10)/10.0*tempB.z;
      _ObjectList -> SetSIZ(tempA);
    } //fi
    _ObjectList -> SetRecalcWCTess();
  } //fi
//  printf("Scale Factor: %f - (%f/%f)\n",LengthB/LengthA,LengthB,LengthA);
} //eof ModelView::MouseSize()



void ModelView::MouseBorderSelect(int Data[4]) {
  Select(Data[0], Data[1], Data[2], Data[3], 0);
} //eof ModelView::MouseBorderSelect()
