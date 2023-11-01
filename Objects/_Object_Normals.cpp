#include "Object_Normals.h"

void AddNormal(int i1, int i2, int i3, Point3d *pts, Point3d &NormalFnl, int &NT) {
  Point3d	Triangle[3],Normal;

  Triangle[0].x= pts[i1].x;
  Triangle[0].y= pts[i1].y;
  Triangle[0].z= pts[i1].z;
  Triangle[1].x= pts[i2].x;
  Triangle[1].y= pts[i2].y;
  Triangle[1].z= pts[i2].z;
  Triangle[2].x= pts[i3].x;
  Triangle[2].y= pts[i3].y;
  Triangle[2].z= pts[i3].z;

  NurbanaMath::Normal(Triangle,Normal);
  if (NurbanaMath::Normalize(Normal)) {
    NormalFnl.x+= Normal.x;
    NormalFnl.y+= Normal.y;
    NormalFnl.z+= Normal.z;
    NT++;
  } //fi
} //eof AddNormal()


//Draws Triad Faces and Calculates Normals given Linear Quad Data (Higher Quality, More CPU)
void Object_Normals::CalculateTriNormals(Point3d *pts, Point3d *Normals, int LengthU, int LengthV, bool SMOOTH, bool EPU, bool EPV) {
  //   _______
  //  |  /|2 /|
  //  |4/3| /1|
  //  |/__|/__|
  //  |5 /|  /|
  //  | /6|7/8|
  //  |/__|/__|

  Point3d	Triangle[3], Normal,NormalFnl;
  int		i, n, k, NT, index[9];

  if (SMOOTH) {
    // Generate 8 face smooth normals for main region of surface
    index[0]= 2*LengthV+1;
    index[1]= 2*LengthV+2;
    index[2]= LengthV+2;
    index[3]= 2;
    index[4]= 1;
    index[5]= 0;
    index[6]= LengthV;
    index[7]= 2*LengthV;
    index[8]= LengthV+1;

    // Body
    for (i= 1; i < LengthU-1; i++) {
      for (n= 1; n < LengthV-1; n++) {
//        printf("c[%d]\n",index[8]);
        NT= 0;
        NormalFnl.x= NormalFnl.y= NormalFnl.z= 0;
/*
        AddNormal(index[8],index[0],index[1],pts,NormalFnl);
        AddNormal(index[8],index[1],index[2],pts,NormalFnl);
        AddNormal(index[8],index[2],index[4],pts,NormalFnl);
        AddNormal(index[2],index[3],index[4],pts,NormalFnl);
        AddNormal(index[8],index[4],index[5],pts,NormalFnl);
        AddNormal(index[8],index[5],index[6],pts,NormalFnl);
        AddNormal(index[8],index[6],index[0],pts,NormalFnl);
        AddNormal(index[6],index[7],index[0],pts,NormalFnl);
*/
        AddNormal(index[8],index[0],index[2],pts,NormalFnl,NT);
        AddNormal(index[8],index[2],index[4],pts,NormalFnl,NT);
        AddNormal(index[8],index[4],index[6],pts,NormalFnl,NT);
        AddNormal(index[8],index[6],index[0],pts,NormalFnl,NT);

        // No clue why -LengthV is in there!  Take it out and it breaks!
        // Print out the indexes, they're right, add -LengthV they're wrong!
        // But it works!  WTF?!
        Normals[index[0]-LengthV].x= NormalFnl.x/NT;
        Normals[index[0]-LengthV].y= NormalFnl.y/NT;
        Normals[index[0]-LengthV].z= NormalFnl.z/NT;

        for (k= 0; k < 9; k++) index[k]++;
      } //eof
      for (k= 0; k < 9; k++) index[k]+= 2;
    } //eof

    // Vertical Sides
    for (n= 1; n < LengthV-1; n++) {
      // Left
//      printf("c1[%d]\n",n);
      NT= 0;
      NormalFnl.x= NormalFnl.y= NormalFnl.z= 0;
      AddNormal(n-1,n+LengthV,n,pts,NormalFnl,NT);
      AddNormal(n,n+LengthV,n+1,pts,NormalFnl,NT);

      if (!EPU) {
        Normals[n].x= NormalFnl.x/NT;
        Normals[n].y= NormalFnl.y/NT;
        Normals[n].z= NormalFnl.z/NT;
      } //fi

      // Right
//      printf("c2[%d]\n",LengthU*(LengthV-1)+n);
      if (!EPU) NT= 0;
      if (!EPU) NormalFnl.x= NormalFnl.y= NormalFnl.z= 0;
      AddNormal(LengthU*(LengthV-1)+n,LengthU*(LengthV-2)+n,LengthU*(LengthV-1)+n-1,pts,NormalFnl,NT);
      AddNormal(LengthU*(LengthV-1)+n+1,LengthU*(LengthV-2)+n,LengthU*(LengthV-1)+n,pts,NormalFnl,NT);

      Normals[LengthU*(LengthV-1)+n].x= NormalFnl.x/NT;
      Normals[LengthU*(LengthV-1)+n].y= NormalFnl.y/NT;
      Normals[LengthU*(LengthV-1)+n].z= NormalFnl.z/NT;

      if (EPU) {
        Normals[n].x= NormalFnl.x/NT;
        Normals[n].y= NormalFnl.y/NT;
        Normals[n].z= NormalFnl.z/NT;
      } //fi
    } //eof

    // Horizontal Sides
    for (n= 1; n < LengthU-1; n++) {
      // Bottom
//      printf("c3[%d]\n",n*LengthV);
      NT= 0;
      NormalFnl.x= NormalFnl.y= NormalFnl.z= 0;
      AddNormal((n-1)*LengthV,n*LengthV,n*LengthV+1,pts,NormalFnl,NT);
      AddNormal(n*LengthV+1,n*LengthV,(n+1)*LengthV,pts,NormalFnl,NT);
      
      if (!EPV) {
        Normals[n*LengthV].x= NormalFnl.x/NT;
        Normals[n*LengthV].y= NormalFnl.y/NT;
        Normals[n*LengthV].z= NormalFnl.z/NT;
      } //fi

      // Top
//      printf("c4[%d]\n",(n+1)*LengthV-1);
      if (!EPV) NT= 0;
      if (!EPV) NormalFnl.x= NormalFnl.y= NormalFnl.z= 0;
      AddNormal(n*LengthV-1,(n+1)*LengthV-2,(n+1)*LengthV-1,pts,NormalFnl,NT);
      AddNormal((n+1)*LengthV-1,(n+1)*LengthV-2,(n+2)*LengthV-1,pts,NormalFnl,NT);
      
      Normals[(n+1)*LengthV-1].x= NormalFnl.x/NT;
      Normals[(n+1)*LengthV-1].y= NormalFnl.y/NT;
      Normals[(n+1)*LengthV-1].z= NormalFnl.z/NT;

      if (EPV) {
        Normals[n*LengthV].x= NormalFnl.x/NT;
        Normals[n*LengthV].y= NormalFnl.y/NT;
        Normals[n*LengthV].z= NormalFnl.z/NT;
      } //fi
    } //eof

    // Corner 0
    NT= 0;
    NormalFnl.x= NormalFnl.y= NormalFnl.z= 0;
    AddNormal(1,0,LengthV,pts,NormalFnl,NT);
    Normals[0].x= NormalFnl.x;
    Normals[0].y= NormalFnl.y;
    Normals[0].z= NormalFnl.z;

    // Corner 1
    NT= 0;
    NormalFnl.x= NormalFnl.y= NormalFnl.z= 0;
    AddNormal(2*LengthV-1,LengthV-1,LengthV-2,pts,NormalFnl,NT);
    Normals[LengthV-1].x= NormalFnl.x;
    Normals[LengthV-1].y= NormalFnl.y;
    Normals[LengthV-1].z= NormalFnl.z;

    // Corner 2
    NT= 0;
    NormalFnl.x= NormalFnl.y= NormalFnl.z= 0;
    AddNormal(LengthU*LengthV-2,LengthU*LengthV-1,LengthU*(LengthV-1)-1,pts,NormalFnl,NT);
    Normals[LengthU*LengthV-1].x= NormalFnl.x;
    Normals[LengthU*LengthV-1].y= NormalFnl.y;
    Normals[LengthU*LengthV-1].z= NormalFnl.z;

    // Corner 3
    NT= 0;
    NormalFnl.x= NormalFnl.y= NormalFnl.z= 0;
    AddNormal(LengthU*(LengthV-2),LengthU*(LengthV-1),LengthU*(LengthV-1)+1,pts,NormalFnl,NT);
    Normals[LengthU*(LengthV-1)].x= NormalFnl.x;
    Normals[LengthU*(LengthV-1)].y= NormalFnl.y;
    Normals[LengthU*(LengthV-1)].z= NormalFnl.z;

    if (EPU && EPV) {
      Normal.x= Normals[0].x;
      Normal.y= Normals[0].y;
      Normal.z= Normals[0].z;
      Normal.x+= Normals[LengthV-1].x;
      Normal.y+= Normals[LengthV-1].y;
      Normal.z+= Normals[LengthV-1].z;
      Normal.x+= Normals[LengthU*LengthV-1].x;
      Normal.y+= Normals[LengthU*LengthV-1].y;
      Normal.z+= Normals[LengthU*LengthV-1].z;
      Normal.x+= Normals[LengthU*(LengthV-1)].x;
      Normal.y+= Normals[LengthU*(LengthV-1)].y;
      Normal.z+= Normals[LengthU*(LengthV-1)].z;

      Normal.x/= 4;
      Normal.y/= 4;
      Normal.z/= 4;

      Normals[0].x= Normals[LengthV-1].x= Normals[LengthU*LengthV-1].x= Normals[LengthU*(LengthV-1)].x= Normal.x;
      Normals[0].y= Normals[LengthV-1].y= Normals[LengthU*LengthV-1].y= Normals[LengthU*(LengthV-1)].y= Normal.x;
      Normals[0].z= Normals[LengthV-1].z= Normals[LengthU*LengthV-1].z= Normals[LengthU*(LengthV-1)].z= Normal.x;
    } else if (EPU) {
      Normal.x= Normals[0].x;
      Normal.y= Normals[0].y;
      Normal.z= Normals[0].z;
      Normal.x+= Normals[LengthU*(LengthV-1)].x;
      Normal.y+= Normals[LengthU*(LengthV-1)].y;
      Normal.z+= Normals[LengthU*(LengthV-1)].z;
      Normal.x/= 2;
      Normal.y/= 2;
      Normal.z/= 2;
      Normals[0].x= Normals[LengthU*(LengthV-1)].x= Normal.x;
      Normals[0].y= Normals[LengthU*(LengthV-1)].y= Normal.y;
      Normals[0].z= Normals[LengthU*(LengthV-1)].z= Normal.z;

      Normal.x= Normals[LengthV-1].x;
      Normal.y= Normals[LengthV-1].y;
      Normal.z= Normals[LengthV-1].z;
      Normal.x+= Normals[LengthU*LengthV-1].x;
      Normal.y+= Normals[LengthU*LengthV-1].y;
      Normal.z+= Normals[LengthU*LengthV-1].z;
      Normal.x/= 2;
      Normal.y/= 2;
      Normal.z/= 2;
      Normals[LengthV-1].x= Normals[LengthU*LengthV-1].x= Normal.x;
      Normals[LengthV-1].y= Normals[LengthU*LengthV-1].y= Normal.y;
      Normals[LengthV-1].z= Normals[LengthU*LengthV-1].z= Normal.z;
    } else if (EPV) {
      Normal.x= Normals[0].x;
      Normal.y= Normals[0].y;
      Normal.z= Normals[0].z;
      Normal.x+= Normals[LengthV-1].x;
      Normal.y+= Normals[LengthV-1].y;
      Normal.z+= Normals[LengthV-1].z;
      Normal.x/= 2;
      Normal.y/= 2;
      Normal.z/= 2;
      Normals[0].x= Normals[LengthV-1].x= Normal.x;
      Normals[0].y= Normals[LengthV-1].y= Normal.y;
      Normals[0].z= Normals[LengthV-1].z= Normal.z;

      Normal.x= Normals[LengthU*LengthV-1].x;
      Normal.y= Normals[LengthU*LengthV-1].y;
      Normal.z= Normals[LengthU*LengthV-1].z;
      Normal.x+= Normals[LengthU*(LengthV-1)].x;
      Normal.y+= Normals[LengthU*(LengthV-1)].y;
      Normal.z+= Normals[LengthU*(LengthV-1)].z;
      Normal.x/= 2;
      Normal.y/= 2;
      Normal.z/= 2;
      Normals[LengthU*LengthV-1].x= Normals[LengthU*(LengthV-1)].x= Normal.x;
      Normals[LengthU*LengthV-1].y= Normals[LengthU*(LengthV-1)].y= Normal.y;
      Normals[LengthU*LengthV-1].z= Normals[LengthU*(LengthV-1)].z= Normal.z;
    } //fi

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

        NurbanaMath::Normal(Triangle,Normal);
        NurbanaMath::Normalize(Normal);

        // Side 1
        Normals[k].x= Normal.x;
        Normals[k].y= Normal.y;
        Normals[k].z= Normal.z;
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


        NurbanaMath::Normal(Triangle,Normal);
        NurbanaMath::Normalize(Normal);

        // Side 2
        Normals[k].x= Normal.x;
        Normals[k].y= Normal.y;
        Normals[k].z= Normal.z;
        k++;
     } //fi
   } //fi
  } //fi
} //eof Object_Normals::CalculateTriNormals()
