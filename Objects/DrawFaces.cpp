#include "DrawFaces.h"


//Draws Triad Faces and Calculates Normals given Linear Quad Data (Higher Quality, More CPU)
void DrawFaces::DrawTriadFaces(Point3d *pts, Point3d *Normals, int LengthU, int LengthV, float mat_diffuse[4], float mat_specular[4], bool SMOOTH) {
  //C|"""/|CB
  // |2 / |
  // | / 1|
  //A|/___|B

  float		mat_shininess[]= { 100.0 };
  int		i, n, k;

  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

//  glEnable(GL_CULL_FACE);
//  glEnable(GL_BLEND);
//  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //Now Draw Faces and get normals
  if (SMOOTH) {
//    glBegin(GL_TRIANGLES);
    for (i= 0; i < LengthU-1; i++) {
      for (n= 0; n < LengthV-1; n++) {
        glBegin(GL_TRIANGLE_STRIP);
        glNormal3d(Normals[i * LengthV + n].x, Normals[i * LengthV + n].y, Normals[i * LengthV + n].z);
          glVertex3d(pts[i * LengthV + n].x, pts[i * LengthV + n].y, pts[i * LengthV + n].z);

        glNormal3d(Normals[i * LengthV + n + LengthV].x, Normals[i * LengthV + n + LengthV].y, Normals[i * LengthV + n + LengthV].z);
          glVertex3d(pts[i * LengthV + n + LengthV].x, pts[i * LengthV + n + LengthV].y, pts[i * LengthV + n + LengthV].z);

        glNormal3d(Normals[i * LengthV + n + 1].x, Normals[i * LengthV + n + 1].y, Normals[i * LengthV + n + 1].z);
          glVertex3d(pts[i * LengthV + n + 1].x, pts[i * LengthV + n + 1].y, pts[i * LengthV + n + 1].z);

        glNormal3d(Normals[i * LengthV + n + LengthV + 1].x, Normals[i * LengthV + n + LengthV + 1].y, Normals[i * LengthV + n + LengthV + 1].z);
          glVertex3d(pts[i * LengthV + n + LengthV + 1].x, pts[i * LengthV + n + LengthV + 1].y, pts[i * LengthV + n + LengthV + 1].z);
        glEnd();
/*
        //Side 1
        glNormal3d(Normals[i * LengthV + n].x, Normals[i * LengthV + n].y, Normals[i * LengthV + n].z);
          glVertex3d(pts[i * LengthV + n].x, pts[i * LengthV + n].y, pts[i * LengthV + n].z);
        glNormal3d(Normals[i * LengthV + n + LengthV].x, Normals[i * LengthV + n + LengthV].y, Normals[i * LengthV + n + LengthV].z);
          glVertex3d(pts[i * LengthV + n + LengthV].x, pts[i * LengthV + n + LengthV].y, pts[i * LengthV + n + LengthV].z);
        glNormal3d(Normals[i * LengthV + n + LengthV + 1].x, Normals[i * LengthV + n + LengthV + 1].y, Normals[i * LengthV + n + LengthV + 1].z);
          glVertex3d(pts[i * LengthV + n + LengthV + 1].x, pts[i * LengthV + n + LengthV + 1].y, pts[i * LengthV + n + LengthV + 1].z);

        //Side 2
        glNormal3d(Normals[i * LengthV + n].x, Normals[i * LengthV + n].y, Normals[i * LengthV + n].z);
          glVertex3d(pts[i * LengthV + n].x, pts[i * LengthV + n].y, pts[i * LengthV + n].z);
        glNormal3d(Normals[i * LengthV + n + LengthV + 1].x, Normals[i * LengthV + n + LengthV + 1].y, Normals[i * LengthV + n + LengthV + 1].z);
          glVertex3d(pts[i * LengthV + n + LengthV + 1].x, pts[i * LengthV + n + LengthV + 1].y, pts[i * LengthV + n + LengthV + 1].z);
        glNormal3d(Normals[i * LengthV + n + 1].x, Normals[i * LengthV + n + 1].y, Normals[i * LengthV + n + 1].z);
          glVertex3d(pts[i * LengthV + n + 1].x, pts[i * LengthV + n + 1].y, pts[i * LengthV + n + 1].z);
*/
      } //eof
    } //eof
  } else {
    k= 0;
    for (i= 0; i < LengthU-1; i++) {
      for (n= 0; n < LengthV-1; n++) {
        glBegin(GL_TRIANGLES);

          //Side 1
          glNormal3d(Normals[k].x, Normals[k].y, Normals[k].z);
            glVertex3d(pts[i * LengthV + n].x, pts[i * LengthV + n].y, pts[i * LengthV + n].z);
            glVertex3d(pts[i * LengthV + n + LengthV].x, pts[i * LengthV + n + LengthV].y, pts[i * LengthV + n + LengthV].z);
            glVertex3d(pts[i * LengthV + n + LengthV + 1].x, pts[i * LengthV + n + LengthV + 1].y, pts[i * LengthV + n + LengthV + 1].z);

          k++;
          //Side 2
          glNormal3d(Normals[k].x, Normals[k].y, Normals[k].z);
            glVertex3d(pts[i * LengthV + n].x, pts[i * LengthV + n].y, pts[i * LengthV + n].z);
            glVertex3d(pts[i * LengthV + n + LengthV + 1].x, pts[i * LengthV + n + LengthV + 1].y, pts[i * LengthV + n + LengthV + 1].z);
            glVertex3d(pts[i * LengthV + n + 1].x, pts[i * LengthV + n + 1].y, pts[i * LengthV + n + 1].z);

          k++;
        glEnd();

     } //fi
   } //fi
  } //fi

  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
} //eof DrawFaces::DrawTriadFaces()


// Divides a Quad into 2 Triangles and draws Normal Factors for each based on center of each Triangle
void DrawFaces::DrawTriadNormalVectors(Point3d *pts, int LengthU, int LengthV, float Nlen) {
  Point3d	Normal[2],Side1[3],Side2[3];
  double	Length[2];
  int		i, n;

  glDisable(GL_LIGHTING);

  //Now Draw Faces and get normals
  
  for (i= 1; i < LengthU; i++) {
    for (n= 0; n < LengthV - 1; n++) {
        //Average all 4 pts to get Center...
        Side1[0].x= (pts[i * LengthV + n].x + pts[i * LengthV + n + 1].x + pts[i * LengthV + n - LengthV].x) / 3.0;
        Side1[0].y= (pts[i * LengthV + n].y + pts[i * LengthV + n + 1].y + pts[i * LengthV + n - LengthV].y) / 3.0;
        Side1[0].z= (pts[i * LengthV + n].z + pts[i * LengthV + n + 1].z + pts[i * LengthV + n - LengthV].z) / 3.0;

        Side2[0].x= (pts[i * LengthV + n - LengthV + 1].x + pts[i * LengthV + n - LengthV].x + pts[i * LengthV + n + 1].x) / 3.0;
        Side2[0].y= (pts[i * LengthV + n - LengthV + 1].y + pts[i * LengthV + n - LengthV].y + pts[i * LengthV + n + 1].y) / 3.0;
        Side2[0].z= (pts[i * LengthV + n - LengthV + 1].z + pts[i * LengthV + n - LengthV].z + pts[i * LengthV + n + 1].z) / 3.0;

        Side1[1].x= Side2[2].x= pts[i * LengthV + n + 1].x;
        Side1[1].y= Side2[2].x= pts[i * LengthV + n + 1].y;
        Side1[1].z= Side2[2].z= pts[i * LengthV + n + 1].z;

        Side1[2].x= Side2[1].x= pts[i * LengthV + n - LengthV].x;
        Side1[2].y= Side2[1].y= pts[i * LengthV + n - LengthV].y;
        Side1[2].z= Side2[1].z= pts[i * LengthV + n - LengthV].z;

        NurbanaMath::Normal(Side1,Normal[0]);
        NurbanaMath::Normal(Side2,Normal[1]);

        NurbanaMath::Normalize(Normal[0]);
        NurbanaMath::Normalize(Normal[1]);

      if (Length[0]) {
      glBegin(GL_LINES);
        glColor3ub(0,255,255);
          Normal[0].x*= Nlen;
          Normal[0].y*= Nlen;
          Normal[0].z*= Nlen;

          glVertex3d(Side1[0].x,Side1[0].y,Side1[0].z);
          glVertex3d(Side1[0].x+Normal[0].x,Side1[0].y+Normal[0].y,Side1[0].z+Normal[0].z);
      glEnd();
      } //fi


      if (Length[1]) {
      glBegin(GL_LINES);
        glColor3ub(0,255,255);
          Normal[1].x*= Nlen;
          Normal[1].y*= Nlen;
          Normal[1].z*= Nlen;

          glVertex3d(Side2[0].x,Side2[0].y,Side2[0].z);
          glVertex3d(Side2[0].x+Normal[1].x,Side2[0].y+Normal[1].y,Side2[0].z+Normal[1].z);
      glEnd();
      } //fi


    } //eof
  } //eof

} //eof DrawFaces::DrawTriadNormalVectors()
