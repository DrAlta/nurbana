#include "Object_Normals.h"


//Draws Triad Faces and Calculates Normals given Linear Quad Data (Higher Quality, More CPU)
void Object_Normals::CalculateTriNormals(Point3d *pts, Point3d *Normals, int LengthU, int LengthV, bool SMOOTH, bool EPU, bool EPV) {
  //   _______
  //  |  /|2 /|
  //  |4/3| /1|
  //  |/__|/__|
  //  |5 /|  /|
  //  | /6|7/8|
  //  |/__|/__|

  Point3d	Triangle[3], NormalTmp[6];
  int		i, n, k, NormTot, FACE, Loc[3];

  if (SMOOTH) {
  
  for (i= 0; i < LengthU; i++) {
    for (n= 0; n < LengthV; n++) {
      //Calculate Sm00th Normals for each vertex and store in Normal[][3] Array
      //We need to Check if LEFT, LEFT-BOTTOM, and BOTTOM Quads Exist!!!
      //There can be 2, 3, or 6 Normals that go into creating the averaged
      //UPPER-RIGHT= 2, UPPER-LEFT= 1, BOTTOM-LEFT= 2, BOTTOM-RIGHT= 1;
      //smooth Normal.


      NormTot= 0;
      for (k= 1; k <= 6; k++) {
        FACE= 0;
        switch( k ) {
          case 1:
            {
              bool	u,v;
              u= i < LengthU-1;
              v= n < LengthV-1;

              if (u && v) {
                Loc[0]= i * LengthV + n;
                Loc[1]= i * LengthV + n + LengthV;
                Loc[2]= i * LengthV + n + LengthV + 1;
                FACE= 1;
              } else if (!u && v) {
                if (EPU) {
                  Loc[0]= n;
                  Loc[1]= n + LengthV;
                  Loc[2]= n + LengthV + 1;
                  FACE= 1;
                } //fi
              } else if (u && !v) {
                if (EPV) {
                  Loc[0]= i * LengthV;
                  Loc[1]= i * LengthV + LengthV;
                  Loc[2]= i * LengthV + LengthV + 1;             
                  FACE= 1;
                } //fi
              } else {
                if (EPU && EPV) {
                  Loc[0]= 0;
                  Loc[1]= LengthV;
                  Loc[2]= LengthV + 1;
                  FACE= 1;
                } //fi
              } //fi
            } //eoc
            break;

          case 2:
            {
              bool	u,v;
              u= i < LengthU-1;
              v= n < LengthV-1;

              if (u && v) {
                Loc[0]= i * LengthV + n;
                Loc[1]= i * LengthV + n + LengthV + 1;
                Loc[2]= i * LengthV + n + 1;
                FACE= 1;
              } else if (!u && v) {
                if (EPU) {
                  Loc[0]= n;
                  Loc[1]= n + LengthV + 1;
                  Loc[2]= n + 1;
                  FACE= 1;
                } //fi
              } else if (u && !v) {
                if (EPV) {
                  Loc[0]= i * LengthV;
                  Loc[1]= i * LengthV + LengthV + 1;
                  Loc[2]= i * LengthV + 1;
                  FACE= 1;
                } //fi
              } else {
                if (EPU && EPV) {
                  Loc[0]= 0;
                  Loc[1]= LengthV + 1;
                  Loc[2]= 1;
                  FACE= 1;
                } //fi
              } //fi
            } //eoc
            break;

          case 3:
            {
              bool	u,v;
              u= i > 0;
              v= n < LengthV-1;

              if (u && v) {
                Loc[0]= i * LengthV + n;
                Loc[1]= i * LengthV + n + 1;
                Loc[2]= i * LengthV + n - LengthV;
                FACE= 1;
              } else if (!u && v) {
                if (EPU) {
                  Loc[0]= (LengthU-1) * LengthV + n;
                  Loc[1]= (LengthU-1) * LengthV + n + 1;
                  Loc[2]= (LengthU-1) * LengthV + n - LengthV;
                  FACE= 1;
                } //fi
              } else if (u && !v) {
                if (EPV) {
                  Loc[0]= i * LengthV;
                  Loc[1]= i * LengthV + 1;
                  Loc[2]= i * LengthV - LengthV;
                  FACE= 1;
                } //fi
              } else {
                if (EPU && EPV) {
                  Loc[0]= (LengthU-1) * LengthV;
                  Loc[1]= (LengthU-1) * LengthV + 1;
                  Loc[2]= (LengthU-1) * LengthV - LengthV;
                  FACE= 1;
                } //fi
              } //fi
            } //eoc
            break;

          case 4:
            {
              bool	u,v;
              u= i > 0;
              v= n > 0;

              if (u && v) {
                Loc[0]= i * LengthV + n;
                Loc[1]= i * LengthV + n - LengthV;
                Loc[2]= i * LengthV + n - LengthV - 1;
                FACE= 1;
              } else if (!u && v) {
                if (EPU) {
                  Loc[0]= (LengthU-1) * LengthV + n;
                  Loc[1]= (LengthU-1) * LengthV + n - LengthV;
                  Loc[2]= (LengthU-1) * LengthV + n - LengthV - 1;
                  FACE= 1;
                } //fi
              } else if (u && !v) {
                if (EPV) {
                  Loc[0]= (i+1) * LengthV - 1;
                  Loc[1]= (i+1) * LengthV - LengthV - 1;
                  Loc[2]= (i+1) * LengthV - LengthV - 1 - 1;
                  FACE= 1;
                } //fi
              } else {
                if (EPU && EPV) {
                  Loc[0]= (LengthU*LengthV) - 1;
                  Loc[1]= (LengthU*LengthV) - LengthV - 1;
                  Loc[2]= (LengthU*LengthV) - LengthV - 1 - 1;
                  FACE= 1;
                } //fi
              } //fi
            } //eoc
            break;

          case 5:
            {
              bool	u,v;
              u= i > 0;
              v= n > 0;

              if (u && v) {
                Loc[0]= i * LengthV + n;
                Loc[1]= i * LengthV + n - LengthV - 1;
                Loc[2]= i * LengthV + n - 1;
                FACE= 1;
              } else if (!u && v) {
                if (EPU) {
                  Loc[0]= (LengthU-1)*LengthV + n;
                  Loc[1]= (LengthU-1)*LengthV + n - LengthV - 1;
                  Loc[2]= (LengthU-1)*LengthV + n - 1;
                  FACE= 1;
                } //fi
              } else if (u && !v) {
                if (EPV) {
                  Loc[0]= (i+1) * LengthV - 1;
                  Loc[1]= (i+1) * LengthV - LengthV - 1 - 1;
                  Loc[2]= (i+1) * LengthV - 1 - 1;
                  FACE= 1;
                } //fi
              } else {
                if (EPU && EPV) {
                  Loc[0]= (LengthU*LengthV) - 1;
                  Loc[1]= (LengthU*LengthV) - LengthV - 1 - 1;
                  Loc[2]= (LengthU*LengthV) - 1 - 1;
                  FACE= 1;
                } //fi
              } //fi
            } //eoc
            break;

          case 6:
            {
              bool	u,v;
              u= i < LengthU-1;
              v= n > 0;

              if (u && v) {
                Loc[0]= i * LengthV + n;
                Loc[1]= i * LengthV + n - 1;
                Loc[2]= i * LengthV + n + LengthV;
                FACE= 1;
              } else if (!u && v) {
                if (EPU) {
                  Loc[0]= n;
                  Loc[1]= n - 1;
                  Loc[2]= n + LengthV;
                  FACE= 1;
                } //fi
              } else if (u && !v) {
                if (EPV) {
                  Loc[0]= (i+1) * LengthV - 1;
                  Loc[1]= (i+1) * LengthV - 1 - 1;
                  Loc[2]= (i+1) * LengthV - 1 + LengthV;
                  FACE= 1;
                } //fi
              } else {
                if (EPU && EPV) {
                  Loc[0]= LengthV - 1;
                  Loc[1]= LengthV - 1 - 1;
                  Loc[2]= LengthV + LengthV - 1;
                  FACE= 1;
                } //fi
              } //fi
            } //eoc
            break;

          default:
            break;
        } //eos

        if (FACE) {
          Triangle[0].x= pts[Loc[0]].x;
          Triangle[0].y= pts[Loc[0]].y;
          Triangle[0].z= pts[Loc[0]].z;

          Triangle[1].x= pts[Loc[1]].x;
          Triangle[1].y= pts[Loc[1]].y;
          Triangle[1].z= pts[Loc[1]].z;

          Triangle[2].x= pts[Loc[2]].x;
          Triangle[2].y= pts[Loc[2]].y;
          Triangle[2].z= pts[Loc[2]].z;

          NurbanaMath::Normal(Triangle,NormalTmp[NormTot]);
          if (NurbanaMath::Normalize(NormalTmp[NormTot])) {
            NormTot++;
          } //fi
        } //fi
      } //eof

      // Sum the Normals
      for (k= 0; k < NormTot - 1; k++) { //double check from 1 to 0
        NormalTmp[0].x+= NormalTmp[k].x;
        NormalTmp[0].y+= NormalTmp[k].y;
        NormalTmp[0].z+= NormalTmp[k].z;
      } //eof

      // Average the Normals
      NormalTmp[0].x/= NormTot;
      NormalTmp[0].y/= NormTot;
      NormalTmp[0].z/= NormTot;

      // Store the Averaged Normal
      Normals[i * LengthV + n].x= NormalTmp[0].x;
      Normals[i * LengthV + n].y= NormalTmp[0].y;
      Normals[i * LengthV + n].z= NormalTmp[0].z;

    } //eof
  } //eof

  } else {
    k= 0;
    for (i= 0; i < LengthU-1; i++) {
      for (n= 0; n < LengthV-1; n++) {
          Triangle[0].x= pts[i * LengthV + n].x;
          Triangle[0].y= pts[i * LengthV + n].y;
          Triangle[0].z= pts[i * LengthV + n].z;

          Triangle[1].x= pts[i * LengthV + n + LengthV].x;
          Triangle[1].y= pts[i * LengthV + n + LengthV].y;
          Triangle[1].z= pts[i * LengthV + n + LengthV].z;

          Triangle[2].x= pts[i * LengthV + n + LengthV + 1].x;
          Triangle[2].y= pts[i * LengthV + n + LengthV + 1].y;
          Triangle[2].z= pts[i * LengthV + n + LengthV + 1].z;

          NurbanaMath::Normal(Triangle,NormalTmp[0]);
          NurbanaMath::Normalize(NormalTmp[0]);

          //Side 1
          Normals[k].x= NormalTmp[0].x;
          Normals[k].y= NormalTmp[0].y;
          Normals[k].z= NormalTmp[0].z;
          k++;


          Triangle[0].x= pts[i * LengthV + n].x;
          Triangle[0].y= pts[i * LengthV + n].y;
          Triangle[0].z= pts[i * LengthV + n].z;

          Triangle[1].x= pts[i * LengthV + n + LengthV + 1].x;
          Triangle[1].y= pts[i * LengthV + n + LengthV + 1].y;
          Triangle[1].z= pts[i * LengthV + n + LengthV + 1].z;

          Triangle[2].x= pts[i * LengthV + n + 1].x;
          Triangle[2].y= pts[i * LengthV + n + 1].y;
          Triangle[2].z= pts[i * LengthV + n + 1].z;


          NurbanaMath::Normal(Triangle,NormalTmp[0]);
          NurbanaMath::Normalize(NormalTmp[0]);

          //Side 2
          Normals[k].x= NormalTmp[0].x;
          Normals[k].y= NormalTmp[0].y;
          Normals[k].z= NormalTmp[0].z;
          k++;
     } //fi
   } //fi
  } //fi
} //eof Object_Normals::CalculateTriNormals()
