#include "Object_Draw.h"

#define POINTSIZE	3

void Object_Draw::Vertices(Object_Base *obj, Point3d *Pts) {
  int i;

  glDisable(GL_DEPTH_TEST);

  glInitNames();
  // Because LoadName() won't work unless there is a name on the stack
  glPushName(0);


  glPointSize(POINTSIZE);

//  glColor3ub(255,75,225);
  glColor3ub(0,200,200);
  for (i= 0; i < obj -> Length(0) * obj -> Length(1); i++) {
    if (!obj -> _ActPts[i]) {
      glLoadName(i+1); // Name assigned to each Vertice (1 - Size)

      glBegin(GL_POINTS);
        glVertex3d(Pts[i].x,Pts[i].y,Pts[i].z);
      glEnd();
    } //fi
  } //eof

//  glColor3ub(200,200,0);
  glColor3ub(255,255,0);
  for (i= 0; i < obj -> Length(0) * obj -> Length(1); i++) {
    if (obj -> _ActPts[i]) {
      glLoadName(i+1); //Name assigned to each Vertice (1 - Size)

      glBegin(GL_POINTS);
        glVertex3d(Pts[i].x,Pts[i].y,Pts[i].z);
      glEnd();
    } //fi
  } //eof

  // Assign everything else a new ID so it doesn't think other objects
  // are the same as the last point in the object
  glLoadName(i+1); //Name assigned to each Vertice (1 - Size)
} //eof Object_Draw::Vertices()



void Object_Draw::COR(Object_Base *obj) {
  int		viewport[4];
  double	winX,winY,winZ;
  double	modelMatrix[16],projMatrix[16];

  glReadBuffer(GL_FRONT);

  glGetIntegerv(GL_VIEWPORT, (GLint*)viewport);
  glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
  glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

  ProjectUtility::Project(obj -> COR().x, obj -> COR().y, obj -> COR().z,modelMatrix,projMatrix,viewport,&winX,&winY,&winZ);
//  printf("winX: %f, winY:%f\n",winX,winY);
//  glReadPixels(int(winX),int(winY),4,4,GL_RGB,GL_UNSIGNED_BYTE,CORmap);
 

  glPointSize(3);

  if (obj -> Active()) {
    glColor3ub(224,224,224);
  } else {
    glColor3ub(125,125,125);
  } //fi

  glBegin(GL_POINTS);
    glVertex3d(obj -> COR().x, obj -> COR().y, obj -> COR().z);
  glEnd();
} //eof Object_Draw::COR()

/*
void Object_Draw::Box(Object_Base *obj) {
  // Determine Max Min X,Y,Z
  float	MinX,MaxX,MinY,MaxY,MinZ,MaxZ;
  int	i;

  MinX= MaxX= Pts[0][0];
  MinY= MaxY= Pts[0][1];
  MinZ= MaxZ= Pts[0][2];

  for (i= 1; i < 4; i++) {
    if (Pts[i][0] < MinX) MinX= Pts[i][0];
    if (Pts[i][0] > MaxX) MaxX= Pts[i][0];
    if (Pts[i][1] < MinY) MinY= Pts[i][1];
    if (Pts[i][1] > MaxY) MaxY= Pts[i][1];
    if (Pts[i][2] < MinZ) MinZ= Pts[i][2];
    if (Pts[i][2] > MaxZ) MaxZ= Pts[i][2];
  } //eof

  // Now Draw Box (12) lines
  glBegin(GL_LINES);
    glColor3ub(0,255,255);

    // Bottom
    glVertex3f(MinX,MinY,MinZ);
    glVertex3f(MinX,MaxY,MinZ);

    glVertex3f(MinX,MinY,MinZ);
    glVertex3f(MaxX,MinY,MinZ);

    glVertex3f(MinX,MaxY,MinZ);
    glVertex3f(MaxX,MaxY,MinZ);

    glVertex3f(MaxX,MinY,MinZ);
    glVertex3f(MaxX,MaxY,MinZ);

    // Sides
    glVertex3f(MinX,MinY,MinZ);
    glVertex3f(MinX,MinY,MaxZ);

    glVertex3f(MinX,MaxY,MinZ);
    glVertex3f(MinX,MaxY,MaxZ);

    glVertex3f(MaxX,MinY,MinZ);
    glVertex3f(MaxX,MinY,MaxZ);

    glVertex3f(MaxX,MaxY,MinZ);
    glVertex3f(MaxX,MaxY,MaxZ);

    // Top
    glVertex3f(MinX,MinY,MaxZ);
    glVertex3f(MinX,MaxY,MaxZ);

    glVertex3f(MinX,MinY,MaxZ);
    glVertex3f(MaxX,MinY,MaxZ);

    glVertex3f(MinX,MaxY,MaxZ);
    glVertex3f(MaxX,MaxY,MaxZ);

    glVertex3f(MaxX,MinY,MaxZ);
    glVertex3f(MaxX,MaxY,MaxZ);

  glEnd();
} //eof Object_Draw::Box()
*/

void Object_Draw::Wireframe(Object_Base *obj, unsigned char Ru, unsigned char Gu, unsigned char Bu, unsigned char Rv, unsigned char Gv, unsigned char Bv, unsigned int LengthU, unsigned int LengthV, Point3d *Pts, bool Edit, bool Hilite, int Index, int Direction) {
  int	i;

  if (Hilite) {
    if (obj -> Active()) {
      Ru= (Ru*255)/216;
      Gu= (Gu*255)/216;
      Bu= (Bu*255)/216;
      Rv= (Rv*255)/216;
      Gv= (Gv*255)/216;
      Bv= (Bv*255)/216;
    } else {
      Ru= (Ru*255)/280;
      Gu= (Gu*255)/280;
      Bu= (Bu*255)/280;
      Rv= (Rv*255)/280;
      Gv= (Rv*255)/280;
      Bv= (Bv*255)/280;
    } //fi
  } //fi


  if (!Edit) glLoadName(obj -> ID());

  // Horizontal Lines
  if (LengthU > 1 && Direction&1) {
    glBegin(GL_LINES);
    glColor3ub(Ru, Gu, Bu);
    for (i= 0; i < (LengthU-1) * LengthV; i++) {
      glVertex3d(Pts[Index+i].x,Pts[Index+i].y,Pts[Index+i].z);
      glVertex3d(Pts[Index+i+LengthV].x,Pts[Index+i+LengthV].y,Pts[Index+i+LengthV].z);
    } //eof
    glEnd();
  } //fi


  // Vertical Lines
  if (LengthV > 1 && Direction&2) {
    glBegin(GL_LINES);
    glColor3ub(Rv, Gv, Bv);
    for (i= 0; i < LengthU * LengthV; i++) {
      glVertex3d(Pts[Index+i].x,Pts[Index+i].y,Pts[Index+i].z);
      glVertex3d(Pts[Index+i+1].x,Pts[Index+i+1].y,Pts[Index+i+1].z);
      if (!((i+2)%LengthV)) i++;
    } //eof
  } //fi
  glEnd();
} //eof Object_Draw::Wireframe()


void Object_Draw::Edges(Object_Base *obj, bool T) {
  int		i;

  glDisable(GL_DEPTH_TEST);

  if (obj -> Edge(0) || T) {
    if (T) glLoadName(obj -> ID()*4+0);
    glBegin(GL_LINES);
    glColor3ub(255, 255, 0);
    for (i= 0; i < obj -> Tess(0) * (obj -> Tess(1)-1); i+= obj -> Tess(0)) {
      glVertex3d(obj -> WCTssPts()[i].x,obj -> WCTssPts()[i].y,obj -> WCTssPts()[i].z);
      glVertex3d(obj -> WCTssPts()[i+obj -> Tess(1)].x,obj -> WCTssPts()[i+obj -> Tess(1)].y,obj -> WCTssPts()[i+obj -> Tess(1)].z);
    } //eof
    glEnd();
  } //fi

  if (obj -> Edge(1) || T) {
    if (T) glLoadName(obj -> ID()*4+1);
    glBegin(GL_LINES);
    glColor3ub(255, 255, 0);
    for (i= 0; i < obj -> Tess(1)-1; i++) {
      glVertex3d(obj -> WCTssPts()[i].x,obj -> WCTssPts()[i].y,obj -> WCTssPts()[i].z);
      glVertex3d(obj -> WCTssPts()[i+1].x,obj -> WCTssPts()[i+1].y,obj -> WCTssPts()[i+1].z);
    } //eof
    glEnd();
  } //fi

  if (obj -> Edge(2) || T) {
    if (T) glLoadName(obj -> ID()*4+2);
    glBegin(GL_LINES);
    glColor3ub(255, 255, 0);
    for (i= obj -> Tess(1)-1; i < obj -> Tess(0) * (obj -> Tess(1)-1); i+= obj -> Tess(0)) {
      glVertex3d(obj -> WCTssPts()[i].x,obj -> WCTssPts()[i].y,obj -> WCTssPts()[i].z);
      glVertex3d(obj -> WCTssPts()[i+obj -> Tess(1)].x,obj -> WCTssPts()[i+obj -> Tess(1)].y,obj -> WCTssPts()[i+obj -> Tess(1)].z);
    } //eof
    glEnd();
  } //fi

  if (obj -> Edge(3) || T) {
    if (T) glLoadName(obj -> ID()*4+3);
    glBegin(GL_LINES);
    glColor3ub(255, 255, 0);
    for (i= obj -> Tess(0)*(obj -> Tess(1)-1); i < obj -> Tess(0)*obj -> Tess(1)-1; i++) {
      glVertex3d(obj -> WCTssPts()[i].x,obj -> WCTssPts()[i].y,obj -> WCTssPts()[i].z);
      glVertex3d(obj -> WCTssPts()[i+1].x,obj -> WCTssPts()[i+1].y,obj -> WCTssPts()[i+1].z);
    } //eof
    glEnd();
  } //fi
} //eof Object_Draw::Edges()

/*
void Object_Draw::BoundingBox(Object_Base *obj) {
  int		i;
  double	Min[3],Max[3];

  glEnable(GL_DEPTH_TEST);
  Min[0]= Max[0]= 0;
  Min[1]= Max[1]= 0;
  Min[2]= Max[2]= 0;


  for (i= 0; i < Length; i++) {
    if (pts[i].x > Min[0]) Min[0]= pts[i].x;
    if (pts[i].y > Min[1]) Min[1]= pts[i].y;
    if (pts[i].z > Min[2]) Min[2]= pts[i].z;

    if (pts[i].x < Max[0]) Max[0]= pts[i].x;
    if (pts[i].y < Max[1]) Max[1]= pts[i].y;
    if (pts[i].z < Max[2]) Max[2]= pts[i].z;
  } //eof

  glBegin(GL_LINES);
  glColor3ub(R,G,B);
    // X
    glVertex3d(Min[0],(Min[1]+Max[1])/2.0,(Min[2]+Max[2])/2.0);
    glVertex3d(Max[0],(Min[1]+Max[1])/2.0,(Min[2]+Max[2])/2.0);

    // Y
    glVertex3d((Min[0]+Max[0])/2.0,Min[1],(Min[2]+Max[2])/2.0);
    glVertex3d((Min[0]+Max[0])/2.0,Max[1],(Min[2]+Max[2])/2.0);

    // Z
    glVertex3d((Min[0]+Max[0])/2.0,(Min[1]+Max[1])/2.0,Min[2]);
    glVertex3d((Min[0]+Max[0])/2.0,(Min[1]+Max[1])/2.0,Max[2]);
  glEnd();

  glDisable(GL_DEPTH_TEST);
} //eof Object_Draw::BoundingBox()
*/

void Object_Draw::Axes(Object_Base *obj) {
  int		i;
  float		Length;
  Point3d	Min,Max,temp,Xaxis[5],Yaxis[5],Zaxis[5];


  // Determine Max X,Y,Z
  Min.x= Max.x= obj -> CtlPts()[0].x;
  Min.y= Max.y= obj -> CtlPts()[0].y;
  Min.z= Max.z= obj -> CtlPts()[0].z;

  for (i= 0; i < obj -> Tess(0) * obj -> Tess(1); i++) {
    if (obj -> TssPts()[i].x > Min.x) Min.x= obj -> TssPts()[i].x;
    if (obj -> TssPts()[i].y > Min.y) Min.y= obj -> TssPts()[i].y;
    if (obj -> TssPts()[i].z > Min.z) Min.z= obj -> TssPts()[i].z;

    if (obj -> TssPts()[i].x < Max.x) Max.x= obj -> TssPts()[i].x;
    if (obj -> TssPts()[i].y < Max.y) Max.y= obj -> TssPts()[i].y;
    if (obj -> TssPts()[i].z < Max.z) Max.z= obj -> TssPts()[i].z;
  } //eof

  // Find Least
  Length= fabs(Max.x-Min.x);
  if (fabs(Max.y-Min.y) < Length)
    Length= fabs(Max.y-Min.y);
  if (fabs(Max.z-Min.z) < Length)
    Length= fabs(Max.z-Min.z);

  // Construct X Vector
  Xaxis[0].x= Length;
  Xaxis[0].y= 0;
  Xaxis[0].z= 0;

  Xaxis[1].x= Length*0.9;
  Xaxis[1].y= Length*0.1;
  Xaxis[1].z= 0;

  Xaxis[2].x= Length*0.9;
  Xaxis[2].y= 0;
  Xaxis[2].z= Length*0.1;

  Xaxis[3].x= Length*0.9;
  Xaxis[3].y= -Length*0.1;
  Xaxis[3].z= 0;

  Xaxis[4].x= Length*0.9;
  Xaxis[4].y= 0;
  Xaxis[4].z= -Length*0.1;

  // Construct Y Vector
  for (i= 0; i < 5; i++) {
    temp.x= Xaxis[i].x;
    temp.y= Xaxis[i].y;
    temp.z= Xaxis[i].z;

    VertexOps::RotateVertex(0,0,90,temp);

    Yaxis[i].x= temp.x;
    Yaxis[i].y= temp.y;
    Yaxis[i].z= temp.z;
  } //eof

  // Construct Z Vector
  for (i= 0; i < 5; i++) {
    temp.x= Xaxis[i].x;
    temp.y= Xaxis[i].y;
    temp.z= Xaxis[i].z;

    VertexOps::RotateVertex(0,-90,0,temp);

    Zaxis[i].x= temp.x;
    Zaxis[i].y= temp.y;
    Zaxis[i].z= temp.z;
  } //eof

  for (i= 0; i < 5; i++) {
    VertexOps::MultPtMatrix(temp, Xaxis[i], obj -> MRM());
    Xaxis[i].x= temp.x;
    Xaxis[i].y= temp.y;
    Xaxis[i].z= temp.z;

    VertexOps::MultPtMatrix(temp, Yaxis[i], obj -> MRM());
    Yaxis[i].x= temp.x;
    Yaxis[i].y= temp.y;
    Yaxis[i].z= temp.z;

    VertexOps::MultPtMatrix(temp, Zaxis[i], obj -> MRM());
    Zaxis[i].x= temp.x;
    Zaxis[i].y= temp.y;
    Zaxis[i].z= temp.z;
  } //eof

  glDisable(GL_DEPTH_TEST);
  glBegin(GL_LINES);
  glColor3ub(255,96,96);
    // X Vector
    glVertex3d(obj -> COR().x,obj -> COR().y,obj -> COR().z);
    glVertex3d(obj -> COR().x+Xaxis[0].x,obj -> COR().y+Xaxis[0].y,obj -> COR().z+Xaxis[0].z);

    glVertex3d(obj -> COR().x+Xaxis[0].x,obj -> COR().y+Xaxis[0].y,obj -> COR().z+Xaxis[0].z);
    glVertex3d(obj -> COR().x+Xaxis[1].x,obj -> COR().y+Xaxis[1].y,obj -> COR().z+Xaxis[1].z);

    glVertex3d(obj -> COR().x+Xaxis[0].x,obj -> COR().y+Xaxis[0].y,obj -> COR().z+Xaxis[0].z);
    glVertex3d(obj -> COR().x+Xaxis[2].x,obj -> COR().y+Xaxis[2].y,obj -> COR().z+Xaxis[2].z);

    glVertex3d(obj -> COR().x+Xaxis[0].x,obj -> COR().y+Xaxis[0].y,obj -> COR().z+Xaxis[0].z);
    glVertex3d(obj -> COR().x+Xaxis[3].x,obj -> COR().y+Xaxis[3].y,obj -> COR().z+Xaxis[3].z);

    glVertex3d(obj -> COR().x+Xaxis[0].x,obj -> COR().y+Xaxis[0].y,obj -> COR().z+Xaxis[0].z);
    glVertex3d(obj -> COR().x+Xaxis[4].x,obj -> COR().y+Xaxis[4].y,obj -> COR().z+Xaxis[4].z);

  glColor3ub(96,255,96);
    // Y Vector
    glVertex3d(obj -> COR().x,obj -> COR().y,obj -> COR().z);
    glVertex3d(obj -> COR().x+Yaxis[0].x,obj -> COR().y+Yaxis[0].y,obj -> COR().z+Yaxis[0].z);

    glVertex3d(obj -> COR().x+Yaxis[0].x,obj -> COR().y+Yaxis[0].y,obj -> COR().z+Yaxis[0].z);
    glVertex3d(obj -> COR().x+Yaxis[1].x,obj -> COR().y+Yaxis[1].y,obj -> COR().z+Yaxis[1].z);

    glVertex3d(obj -> COR().x+Yaxis[0].x,obj -> COR().y+Yaxis[0].y,obj -> COR().z+Yaxis[0].z);
    glVertex3d(obj -> COR().x+Yaxis[2].x,obj -> COR().y+Yaxis[2].y,obj -> COR().z+Yaxis[2].z);

    glVertex3d(obj -> COR().x+Yaxis[0].x,obj -> COR().y+Yaxis[0].y,obj -> COR().z+Yaxis[0].z);
    glVertex3d(obj -> COR().x+Yaxis[3].x,obj -> COR().y+Yaxis[3].y,obj -> COR().z+Yaxis[3].z);

    glVertex3d(obj -> COR().x+Yaxis[0].x,obj -> COR().y+Yaxis[0].y,obj -> COR().z+Yaxis[0].z);
    glVertex3d(obj -> COR().x+Yaxis[4].x,obj -> COR().y+Yaxis[4].y,obj -> COR().z+Yaxis[4].z);

  glColor3ub(96,96,255);
    // Z Vector
    glVertex3d(obj -> COR().x,obj -> COR().y,obj -> COR().z);
    glVertex3d(obj -> COR().x+Zaxis[0].x,obj -> COR().y+Zaxis[0].y,obj -> COR().z+Zaxis[0].z);

    glVertex3d(obj -> COR().x+Zaxis[0].x,obj -> COR().y+Zaxis[0].y,obj -> COR().z+Zaxis[0].z);
    glVertex3d(obj -> COR().x+Zaxis[1].x,obj -> COR().y+Zaxis[1].y,obj -> COR().z+Zaxis[1].z);

    glVertex3d(obj -> COR().x+Zaxis[0].x,obj -> COR().y+Zaxis[0].y,obj -> COR().z+Zaxis[0].z);
    glVertex3d(obj -> COR().x+Zaxis[2].x,obj -> COR().y+Zaxis[2].y,obj -> COR().z+Zaxis[2].z);

    glVertex3d(obj -> COR().x+Zaxis[0].x,obj -> COR().y+Zaxis[0].y,obj -> COR().z+Zaxis[0].z);
    glVertex3d(obj -> COR().x+Zaxis[3].x,obj -> COR().y+Zaxis[3].y,obj -> COR().z+Zaxis[3].z);

    glVertex3d(obj -> COR().x+Zaxis[0].x,obj -> COR().y+Zaxis[0].y,obj -> COR().z+Zaxis[0].z);
    glVertex3d(obj -> COR().x+Zaxis[4].x,obj -> COR().y+Zaxis[4].y,obj -> COR().z+Zaxis[4].z);
  glEnd();
} //eof Object_Draw::Axes()