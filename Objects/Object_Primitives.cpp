#include "Object_Primitives.h"


void Object_Primitives::Curve(Object_List::ObjectNode *ptr) {
  ptr -> Object -> SetLength(4,1);
  ptr -> Object -> _LengthUV[0]= 4;
  ptr -> Object -> _LengthUV[1]= 1;

  ptr -> Object -> _CtlPts[0].x= -2;
  ptr -> Object -> _CtlPts[0].y= 0;
  ptr -> Object -> _CtlPts[0].z= 0;
  
  ptr -> Object -> _CtlPts[1].x= -1;
  ptr -> Object -> _CtlPts[1].y= 1;
  ptr -> Object -> _CtlPts[1].z= 0;

  ptr -> Object -> _CtlPts[2].x= 1;
  ptr -> Object -> _CtlPts[2].y= -1;
  ptr -> Object -> _CtlPts[2].z= 0;

  ptr -> Object -> _CtlPts[3].x= 2;
  ptr -> Object -> _CtlPts[3].y= 0;
  ptr -> Object -> _CtlPts[3].z= 0;

  ptr -> Object -> Order(0,4);
  ptr -> Object -> Order(1,2);

  ptr -> Object -> RecalculateKnotVector(1);
} //eof Object_Primitives::Curve()


void Object_Primitives::Surface(Object_List::ObjectNode *ptr) {
  int	i;

  ptr -> Object -> SetLength(4,4);
  ptr -> Object -> _LengthUV[0]= 4;
  ptr -> Object -> _LengthUV[1]= 4;

  // X
  for (i= 0; i < 4; i++)
    ptr -> Object -> _CtlPts[i].x= -1.5;
  for (i= 4; i < 8; i++)
    ptr -> Object -> _CtlPts[i].x= -0.5;
  for (i= 8; i < 12; i++)
    ptr -> Object -> _CtlPts[i].x= 0.5;
  for (i= 12; i < 16; i++)
    ptr -> Object -> _CtlPts[i].x= 1.5;

  // Y
  for (i= 0; i < 16; i+= 4)
    ptr -> Object -> _CtlPts[i].y= -1.5;
  for (i= 1; i < 16; i+= 4)
    ptr -> Object -> _CtlPts[i].y= -0.5;
  for (i= 2; i < 16; i+= 4)
    ptr -> Object -> _CtlPts[i].y= 0.5;
  for (i= 3; i < 16; i+= 4)
    ptr -> Object -> _CtlPts[i].y= 1.5;

  // Z
  for (i= 0; i < 16; i++)
    ptr -> Object -> _CtlPts[i].z= 0.0;

  for (i= 5; i < 7; i++)
    ptr -> Object -> _CtlPts[i].z= 1.0;
  for (i= 9; i < 11; i++)
    ptr -> Object -> _CtlPts[i].z= 1.0;


  ptr -> Object -> Order(0,4);
  ptr -> Object -> Order(1,4);

  ptr -> Object -> RecalculateKnotVector(1);
} //eof Object_Primitives::Surface()


void Object_Primitives::Circle(Object_List::ObjectNode *ptr) {
  ptr -> Object -> SetLength(11,1);
  ptr -> Object -> _LengthUV[0]= 8;
  ptr -> Object -> _LengthUV[1]= 1;

  ptr -> Object -> _CtlPts[0].x= 1;
  ptr -> Object -> _CtlPts[0].y= -1;
  ptr -> Object -> _CtlPts[0].z= 0;

  ptr -> Object -> _CtlPts[1].x= 1;
  ptr -> Object -> _CtlPts[1].y= 0;
  ptr -> Object -> _CtlPts[1].z= 0;

  ptr -> Object -> _CtlPts[2].x= 1;
  ptr -> Object -> _CtlPts[2].y= 1;
  ptr -> Object -> _CtlPts[2].z= 0;

  ptr -> Object -> _CtlPts[3].x= 0;
  ptr -> Object -> _CtlPts[3].y= 1;
  ptr -> Object -> _CtlPts[3].z= 0;

  ptr -> Object -> _CtlPts[4].x= -1;
  ptr -> Object -> _CtlPts[4].y= 1;
  ptr -> Object -> _CtlPts[4].z= 0;

  ptr -> Object -> _CtlPts[5].x= -1;
  ptr -> Object -> _CtlPts[5].y= 0;
  ptr -> Object -> _CtlPts[5].z= 0;

  ptr -> Object -> _CtlPts[6].x= -1;
  ptr -> Object -> _CtlPts[6].y= -1;
  ptr -> Object -> _CtlPts[6].z= 0;

  ptr -> Object -> _CtlPts[7].x= 0;
  ptr -> Object -> _CtlPts[7].y= -1;
  ptr -> Object -> _CtlPts[7].z= 0;

  ptr -> Object -> Order(0,4);
  ptr -> Object -> Order(1,2);

  ptr -> Object -> KnotType(0,NURBS_KV_Periodic);

  ptr -> Object -> H()[0]= sqrt(2)/4.0;
  ptr -> Object -> H()[2]= sqrt(2)/4.0;
  ptr -> Object -> H()[4]= sqrt(2)/4.0;
  ptr -> Object -> H()[6]= sqrt(2)/4.0;

  ptr -> Object -> Cyclic(0,1);

  Object_NURBS	*obj[1];
  obj[0]= ((Object_NURBS*)ptr -> Object);
  NURBS::Perform(NURBS_Generate_Cyclic,1,obj,0);

  ptr -> Object -> RecalculateKnotVector(1);
} //eof Object_Primitives::Circle()


void Object_Primitives::Tube(Object_List::ObjectNode *ptr) {
  ptr -> Object -> _LengthUV[0]= 8;
  ptr -> Object -> _LengthUV[1]= 2;

  // Allocate space for Cyclic
  ptr -> Object -> SetLength(ptr -> Object -> _LengthUV[0]+4,ptr -> Object -> _LengthUV[1]);

  ptr -> Object -> _CtlPts[0].x= ptr -> Object -> _CtlPts[1].x= 1;
  ptr -> Object -> _CtlPts[0].y= ptr -> Object -> _CtlPts[1].y= -1;
  ptr -> Object -> _CtlPts[0].z= -1;
  ptr -> Object -> _CtlPts[1].z= 1;

  ptr -> Object -> _CtlPts[2].x= ptr -> Object -> _CtlPts[3].x= 1;
  ptr -> Object -> _CtlPts[2].y= ptr -> Object -> _CtlPts[3].y= 0;
  ptr -> Object -> _CtlPts[2].z= -1;
  ptr -> Object -> _CtlPts[3].z= 1;

  ptr -> Object -> _CtlPts[4].x= ptr -> Object -> _CtlPts[5].x= 1;
  ptr -> Object -> _CtlPts[4].y= ptr -> Object -> _CtlPts[5].y= 1;
  ptr -> Object -> _CtlPts[4].z= -1;
  ptr -> Object -> _CtlPts[5].z= 1;

  ptr -> Object -> _CtlPts[6].x= ptr -> Object -> _CtlPts[7].x= 0;
  ptr -> Object -> _CtlPts[6].y= ptr -> Object -> _CtlPts[7].y= 1;
  ptr -> Object -> _CtlPts[6].z= -1;
  ptr -> Object -> _CtlPts[7].z= 1;

  ptr -> Object -> _CtlPts[8].x= ptr -> Object -> _CtlPts[9].x= -1;
  ptr -> Object -> _CtlPts[8].y= ptr -> Object -> _CtlPts[9].y= 1;
  ptr -> Object -> _CtlPts[8].z= -1;
  ptr -> Object -> _CtlPts[9].z= 1;

  ptr -> Object -> _CtlPts[10].x= ptr -> Object -> _CtlPts[11].x= -1;
  ptr -> Object -> _CtlPts[10].y= ptr -> Object -> _CtlPts[11].y= 0;
  ptr -> Object -> _CtlPts[10].z= -1;
  ptr -> Object -> _CtlPts[11].z= 1;

  ptr -> Object -> _CtlPts[12].x= ptr -> Object -> _CtlPts[13].x= -1;
  ptr -> Object -> _CtlPts[12].y= ptr -> Object -> _CtlPts[13].y= -1;
  ptr -> Object -> _CtlPts[12].z= -1;
  ptr -> Object -> _CtlPts[13].z= 1;

  ptr -> Object -> _CtlPts[14].x= ptr -> Object -> _CtlPts[15].x= 0;
  ptr -> Object -> _CtlPts[14].y= ptr -> Object -> _CtlPts[15].y= -1;
  ptr -> Object -> _CtlPts[14].z= -1;
  ptr -> Object -> _CtlPts[15].z= 1;

  ptr -> Object -> H()[0]= sqrt(2)/4.0;
  ptr -> Object -> H()[1]= sqrt(2)/4.0;

  ptr -> Object -> H()[4]= sqrt(2)/4.0;
  ptr -> Object -> H()[5]= sqrt(2)/4.0;

  ptr -> Object -> H()[8]= sqrt(2)/4.0;
  ptr -> Object -> H()[9]= sqrt(2)/4.0;

  ptr -> Object -> H()[12]= sqrt(2)/4.0;
  ptr -> Object -> H()[13]= sqrt(2)/4.0;

  ptr -> Object -> KnotType(0,NURBS_KV_Periodic);

  ptr -> Object -> Order(0,4);
  ptr -> Object -> Order(1,2);

  ptr -> Object -> Cyclic(0,1);

  Object_NURBS	*obj[1];
  obj[0]= ((Object_NURBS*)ptr -> Object);
  NURBS::Perform(NURBS_Generate_Cyclic,1,obj,0);

  ptr -> Object -> RecalculateKnotVector(1);
} //eof Object_Primitives::Tube()


void Object_Primitives::Torus(Object_List::ObjectNode *ptr) {
  ptr -> Object -> _LengthUV[0]= 8;
  ptr -> Object -> _LengthUV[1]= 8;

  // Allocate Length+Order because it's going Cyclic
  ptr -> Object -> SetLength(ptr -> Object -> _LengthUV[0]+3,ptr -> Object -> _LengthUV[1]+3);

  ptr -> Object -> _CtlPts[0].x= 3; ptr -> Object -> _CtlPts[0].y= 0; ptr -> Object -> _CtlPts[0].z= -1;
  ptr -> Object -> _CtlPts[1].x= 3; ptr -> Object -> _CtlPts[1].y= 3; ptr -> Object -> _CtlPts[1].z= -1;
  ptr -> Object -> _CtlPts[2].x= 0; ptr -> Object -> _CtlPts[2].y= 3; ptr -> Object -> _CtlPts[2].z= -1;
  ptr -> Object -> _CtlPts[3].x= -3; ptr -> Object -> _CtlPts[3].y= 3; ptr -> Object -> _CtlPts[3].z= -1;
  ptr -> Object -> _CtlPts[4].x= -3; ptr -> Object -> _CtlPts[4].y= 0; ptr -> Object -> _CtlPts[4].z= -1;
  ptr -> Object -> _CtlPts[5].x= -3; ptr -> Object -> _CtlPts[5].y= -3; ptr -> Object -> _CtlPts[5].z= -1;
  ptr -> Object -> _CtlPts[6].x= 0; ptr -> Object -> _CtlPts[6].y= -3; ptr -> Object -> _CtlPts[6].z= -1;
  ptr -> Object -> _CtlPts[7].x= 3; ptr -> Object -> _CtlPts[7].y= -3; ptr -> Object -> _CtlPts[7].z= -1;

  ptr -> Object -> _CtlPts[8].x= 2; ptr -> Object -> _CtlPts[8].y= 0; ptr -> Object -> _CtlPts[8].z= -1;
  ptr -> Object -> _CtlPts[9].x= 2; ptr -> Object -> _CtlPts[9].y= 2; ptr -> Object -> _CtlPts[9].z= -1;
  ptr -> Object -> _CtlPts[10].x= 0; ptr -> Object -> _CtlPts[10].y= 2; ptr -> Object -> _CtlPts[10].z= -1;
  ptr -> Object -> _CtlPts[11].x= -2; ptr -> Object -> _CtlPts[11].y= 2; ptr -> Object -> _CtlPts[11].z= -1;
  ptr -> Object -> _CtlPts[12].x= -2; ptr -> Object -> _CtlPts[12].y= 0; ptr -> Object -> _CtlPts[12].z= -1;
  ptr -> Object -> _CtlPts[13].x= -2; ptr -> Object -> _CtlPts[13].y= -2; ptr -> Object -> _CtlPts[13].z= -1;
  ptr -> Object -> _CtlPts[14].x= 0; ptr -> Object -> _CtlPts[14].y= -2; ptr -> Object -> _CtlPts[14].z= -1;
  ptr -> Object -> _CtlPts[15].x= 2; ptr -> Object -> _CtlPts[15].y= -2; ptr -> Object -> _CtlPts[15].z= -1;

  ptr -> Object -> _CtlPts[16].x= 2; ptr -> Object -> _CtlPts[16].y= 0; ptr -> Object -> _CtlPts[16].z= 0;
  ptr -> Object -> _CtlPts[17].x= 2; ptr -> Object -> _CtlPts[17].y= 2; ptr -> Object -> _CtlPts[17].z= 0;
  ptr -> Object -> _CtlPts[18].x= 0; ptr -> Object -> _CtlPts[18].y= 2; ptr -> Object -> _CtlPts[18].z= 0;
  ptr -> Object -> _CtlPts[19].x= -2; ptr -> Object -> _CtlPts[19].y= 2; ptr -> Object -> _CtlPts[19].z= 0;
  ptr -> Object -> _CtlPts[20].x= -2; ptr -> Object -> _CtlPts[20].y= 0; ptr -> Object -> _CtlPts[20].z= 0;
  ptr -> Object -> _CtlPts[21].x= -2; ptr -> Object -> _CtlPts[21].y= -2; ptr -> Object -> _CtlPts[21].z= 0;
  ptr -> Object -> _CtlPts[22].x= 0; ptr -> Object -> _CtlPts[22].y= -2; ptr -> Object -> _CtlPts[22].z= 0;
  ptr -> Object -> _CtlPts[23].x= 2; ptr -> Object -> _CtlPts[23].y= -2; ptr -> Object -> _CtlPts[23].z= 0;
  ptr -> Object -> _CtlPts[24].x= 2; ptr -> Object -> _CtlPts[24].y= 0; ptr -> Object -> _CtlPts[24].z= 1;
  ptr -> Object -> _CtlPts[25].x= 2; ptr -> Object -> _CtlPts[25].y= 2; ptr -> Object -> _CtlPts[25].z= 1;
  ptr -> Object -> _CtlPts[26].x= 0; ptr -> Object -> _CtlPts[26].y= 2; ptr -> Object -> _CtlPts[26].z= 1;
  ptr -> Object -> _CtlPts[27].x= -2; ptr -> Object -> _CtlPts[27].y= 2; ptr -> Object -> _CtlPts[27].z= 1;
  ptr -> Object -> _CtlPts[28].x= -2; ptr -> Object -> _CtlPts[28].y= 0; ptr -> Object -> _CtlPts[28].z= 1;
  ptr -> Object -> _CtlPts[29].x= -2; ptr -> Object -> _CtlPts[29].y= -2; ptr -> Object -> _CtlPts[29].z= 1;
  ptr -> Object -> _CtlPts[30].x= 0; ptr -> Object -> _CtlPts[30].y= -2; ptr -> Object -> _CtlPts[30].z= 1;
  ptr -> Object -> _CtlPts[31].x= 2; ptr -> Object -> _CtlPts[31].y= -2; ptr -> Object -> _CtlPts[31].z= 1;

  ptr -> Object -> _CtlPts[32].x= 3; ptr -> Object -> _CtlPts[32].y= 0; ptr -> Object -> _CtlPts[32].z= 1;
  ptr -> Object -> _CtlPts[33].x= 3; ptr -> Object -> _CtlPts[33].y= 3; ptr -> Object -> _CtlPts[33].z= 1;
  ptr -> Object -> _CtlPts[34].x= 0; ptr -> Object -> _CtlPts[34].y= 3; ptr -> Object -> _CtlPts[34].z= 1;
  ptr -> Object -> _CtlPts[35].x= -3; ptr -> Object -> _CtlPts[35].y= 3; ptr -> Object -> _CtlPts[35].z= 1;
  ptr -> Object -> _CtlPts[36].x= -3; ptr -> Object -> _CtlPts[36].y= 0; ptr -> Object -> _CtlPts[36].z= 1;
  ptr -> Object -> _CtlPts[37].x= -3; ptr -> Object -> _CtlPts[37].y= -3; ptr -> Object -> _CtlPts[37].z= 1;
  ptr -> Object -> _CtlPts[38].x= 0; ptr -> Object -> _CtlPts[38].y= -3; ptr -> Object -> _CtlPts[38].z= 1;
  ptr -> Object -> _CtlPts[39].x= 3; ptr -> Object -> _CtlPts[39].y= -3; ptr -> Object -> _CtlPts[39].z= 1;

  ptr -> Object -> _CtlPts[40].x= 4; ptr -> Object -> _CtlPts[40].y= 0; ptr -> Object -> _CtlPts[40].z= 1;
  ptr -> Object -> _CtlPts[41].x= 4; ptr -> Object -> _CtlPts[41].y= 4; ptr -> Object -> _CtlPts[41].z= 1;
  ptr -> Object -> _CtlPts[42].x= 0; ptr -> Object -> _CtlPts[42].y= 4; ptr -> Object -> _CtlPts[42].z= 1;
  ptr -> Object -> _CtlPts[43].x= -4; ptr -> Object -> _CtlPts[43].y= 4; ptr -> Object -> _CtlPts[43].z= 1;
  ptr -> Object -> _CtlPts[44].x= -4; ptr -> Object -> _CtlPts[44].y= 0; ptr -> Object -> _CtlPts[44].z= 1;
  ptr -> Object -> _CtlPts[45].x= -4; ptr -> Object -> _CtlPts[45].y= -4; ptr -> Object -> _CtlPts[45].z= 1;
  ptr -> Object -> _CtlPts[46].x= 0; ptr -> Object -> _CtlPts[46].y= -4; ptr -> Object -> _CtlPts[46].z= 1;
  ptr -> Object -> _CtlPts[47].x= 4; ptr -> Object -> _CtlPts[47].y= -4; ptr -> Object -> _CtlPts[47].z= 1;

  ptr -> Object -> _CtlPts[48].x= 4; ptr -> Object -> _CtlPts[48].y= 0; ptr -> Object -> _CtlPts[48].z= 0;
  ptr -> Object -> _CtlPts[49].x= 4; ptr -> Object -> _CtlPts[49].y= 4; ptr -> Object -> _CtlPts[49].z= 0;
  ptr -> Object -> _CtlPts[50].x= 0; ptr -> Object -> _CtlPts[50].y= 4; ptr -> Object -> _CtlPts[50].z= 0;
  ptr -> Object -> _CtlPts[51].x= -4; ptr -> Object -> _CtlPts[51].y= 4; ptr -> Object -> _CtlPts[51].z= 0;
  ptr -> Object -> _CtlPts[52].x= -4; ptr -> Object -> _CtlPts[52].y= 0; ptr -> Object -> _CtlPts[52].z= 0;
  ptr -> Object -> _CtlPts[53].x= -4; ptr -> Object -> _CtlPts[53].y= -4; ptr -> Object -> _CtlPts[53].z= 0;
  ptr -> Object -> _CtlPts[54].x= 0; ptr -> Object -> _CtlPts[54].y= -4; ptr -> Object -> _CtlPts[54].z= 0;
  ptr -> Object -> _CtlPts[55].x= 4; ptr -> Object -> _CtlPts[55].y= -4; ptr -> Object -> _CtlPts[55].z= 0;

  ptr -> Object -> _CtlPts[56].x= 4; ptr -> Object -> _CtlPts[56].y= 0; ptr -> Object -> _CtlPts[56].z= -1;
  ptr -> Object -> _CtlPts[57].x= 4; ptr -> Object -> _CtlPts[57].y= 4; ptr -> Object -> _CtlPts[57].z= -1;
  ptr -> Object -> _CtlPts[58].x= 0; ptr -> Object -> _CtlPts[58].y= 4; ptr -> Object -> _CtlPts[58].z= -1;
  ptr -> Object -> _CtlPts[59].x= -4; ptr -> Object -> _CtlPts[59].y= 4; ptr -> Object -> _CtlPts[59].z= -1;
  ptr -> Object -> _CtlPts[60].x= -4; ptr -> Object -> _CtlPts[60].y= 0; ptr -> Object -> _CtlPts[60].z= -1;
  ptr -> Object -> _CtlPts[61].x= -4; ptr -> Object -> _CtlPts[61].y= -4; ptr -> Object -> _CtlPts[61].z= -1;
  ptr -> Object -> _CtlPts[62].x= 0; ptr -> Object -> _CtlPts[62].y= -4; ptr -> Object -> _CtlPts[62].z= -1;
  ptr -> Object -> _CtlPts[63].x= 4; ptr -> Object -> _CtlPts[63].y= -4; ptr -> Object -> _CtlPts[63].z= -1;

  ptr -> Object -> H()[1]= sqrt(2)/4.0;
  ptr -> Object -> H()[3]= sqrt(2)/4.0;
  ptr -> Object -> H()[5]= sqrt(2)/4.0;
  ptr -> Object -> H()[7]= sqrt(2)/4.0;

  ptr -> Object -> H()[8]= sqrt(2)/4.0;
  ptr -> Object -> H()[9]= .125;
  ptr -> Object -> H()[10]= sqrt(2)/4.0;
  ptr -> Object -> H()[11]= .125;
  ptr -> Object -> H()[12]= sqrt(2)/4.0;
  ptr -> Object -> H()[13]= .125;
  ptr -> Object -> H()[14]= sqrt(2)/4.0;
  ptr -> Object -> H()[15]= .125;

  ptr -> Object -> H()[17]= sqrt(2)/4.0;
  ptr -> Object -> H()[19]= sqrt(2)/4.0;
  ptr -> Object -> H()[21]= sqrt(2)/4.0;
  ptr -> Object -> H()[23]= sqrt(2)/4.0;

  ptr -> Object -> H()[24]= sqrt(2)/4.0;
  ptr -> Object -> H()[25]= .125;
  ptr -> Object -> H()[26]= sqrt(2)/4.0;
  ptr -> Object -> H()[27]= .125;
  ptr -> Object -> H()[28]= sqrt(2)/4.0;
  ptr -> Object -> H()[29]= .125;
  ptr -> Object -> H()[30]= sqrt(2)/4.0;
  ptr -> Object -> H()[31]= .125;

  ptr -> Object -> H()[33]= sqrt(2)/4.0;
  ptr -> Object -> H()[35]= sqrt(2)/4.0;
  ptr -> Object -> H()[37]= sqrt(2)/4.0;
  ptr -> Object -> H()[39]= sqrt(2)/4.0;

  ptr -> Object -> H()[40]= sqrt(2)/4.0;
  ptr -> Object -> H()[41]= .125;
  ptr -> Object -> H()[42]= sqrt(2)/4.0;
  ptr -> Object -> H()[43]= .125;
  ptr -> Object -> H()[44]= sqrt(2)/4.0;
  ptr -> Object -> H()[45]= .125;
  ptr -> Object -> H()[46]= sqrt(2)/4.0;
  ptr -> Object -> H()[47]= .125;

  ptr -> Object -> H()[49]= sqrt(2)/4.0;
  ptr -> Object -> H()[51]= sqrt(2)/4.0;
  ptr -> Object -> H()[53]= sqrt(2)/4.0;
  ptr -> Object -> H()[55]= sqrt(2)/4.0;

  ptr -> Object -> H()[56]= sqrt(2)/4.0;
  ptr -> Object -> H()[57]= .125;
  ptr -> Object -> H()[58]= sqrt(2)/4.0;
  ptr -> Object -> H()[59]= .125;
  ptr -> Object -> H()[60]= sqrt(2)/4.0;
  ptr -> Object -> H()[61]= .125;
  ptr -> Object -> H()[62]= sqrt(2)/4.0;
  ptr -> Object -> H()[63]= .125;

  ptr -> Object -> Order(0,4);
  ptr -> Object -> Order(1,4);

  ptr -> Object -> KnotType(0,NURBS_KV_Periodic);
  ptr -> Object -> KnotType(1,NURBS_KV_Periodic);

  ptr -> Object -> Cyclic(0,1);
  ptr -> Object -> Cyclic(1,1);

  Object_NURBS	*obj[1];
  obj[0]= ((Object_NURBS*)ptr -> Object);
  NURBS::Perform(NURBS_Generate_Cyclic,1,obj,0);

  ptr -> Object -> RecalculateKnotVector(1);
} //eof Object_Primitives::Torus()


void Object_Primitives::Sphere(Object_List::ObjectNode *ptr) {
  ptr -> Object -> _LengthUV[0]= 9;
  ptr -> Object -> _LengthUV[1]= 5;
  ptr -> Object -> SetLength(ptr -> Object -> _LengthUV[0],ptr -> Object -> _LengthUV[1]);

  ptr -> Object -> _CtlPts[0].x= 0; ptr -> Object -> _CtlPts[0].y= 0; ptr -> Object -> _CtlPts[0].z= 3;
  ptr -> Object -> _CtlPts[1].x= 3; ptr -> Object -> _CtlPts[1].y= 0; ptr -> Object -> _CtlPts[1].z= 3;
  ptr -> Object -> _CtlPts[2].x= 3; ptr -> Object -> _CtlPts[2].y= 0; ptr -> Object -> _CtlPts[2].z= 0;
  ptr -> Object -> _CtlPts[3].x= 3; ptr -> Object -> _CtlPts[3].y= 0; ptr -> Object -> _CtlPts[3].z= -3;
  ptr -> Object -> _CtlPts[4].x= 0; ptr -> Object -> _CtlPts[4].y= 0; ptr -> Object -> _CtlPts[4].z= -3;
  ptr -> Object -> _CtlPts[5].x= 0; ptr -> Object -> _CtlPts[5].y= 0; ptr -> Object -> _CtlPts[5].z= 3;
  ptr -> Object -> _CtlPts[6].x= 3; ptr -> Object -> _CtlPts[6].y= -3; ptr -> Object -> _CtlPts[6].z= 3;
  ptr -> Object -> _CtlPts[7].x= 3; ptr -> Object -> _CtlPts[7].y= -3; ptr -> Object -> _CtlPts[7].z= 0;
  ptr -> Object -> _CtlPts[8].x= 3; ptr -> Object -> _CtlPts[8].y= -3; ptr -> Object -> _CtlPts[8].z= -3;
  ptr -> Object -> _CtlPts[9].x= 0; ptr -> Object -> _CtlPts[9].y= 0; ptr -> Object -> _CtlPts[9].z= -3;
  ptr -> Object -> _CtlPts[10].x= 0; ptr -> Object -> _CtlPts[10].y= 0; ptr -> Object -> _CtlPts[10].z= 3;
  ptr -> Object -> _CtlPts[11].x= 0; ptr -> Object -> _CtlPts[11].y= -3; ptr -> Object -> _CtlPts[11].z= 3;
  ptr -> Object -> _CtlPts[12].x= 0; ptr -> Object -> _CtlPts[12].y= -3; ptr -> Object -> _CtlPts[12].z= 0;
  ptr -> Object -> _CtlPts[13].x= 0; ptr -> Object -> _CtlPts[13].y= -3; ptr -> Object -> _CtlPts[13].z= -3;
  ptr -> Object -> _CtlPts[14].x= 0; ptr -> Object -> _CtlPts[14].y= 0; ptr -> Object -> _CtlPts[14].z= -3;
  ptr -> Object -> _CtlPts[15].x= 0; ptr -> Object -> _CtlPts[15].y= 0; ptr -> Object -> _CtlPts[15].z= 3;
  ptr -> Object -> _CtlPts[16].x= -3; ptr -> Object -> _CtlPts[16].y= -3; ptr -> Object -> _CtlPts[16].z= 3;
  ptr -> Object -> _CtlPts[17].x= -3; ptr -> Object -> _CtlPts[17].y= -3; ptr -> Object -> _CtlPts[17].z= 0;
  ptr -> Object -> _CtlPts[18].x= -3; ptr -> Object -> _CtlPts[18].y= -3; ptr -> Object -> _CtlPts[18].z= -3;
  ptr -> Object -> _CtlPts[19].x= 0; ptr -> Object -> _CtlPts[19].y= 0; ptr -> Object -> _CtlPts[19].z= -3;
  ptr -> Object -> _CtlPts[20].x= 0; ptr -> Object -> _CtlPts[20].y= 0; ptr -> Object -> _CtlPts[20].z= 3;
  ptr -> Object -> _CtlPts[21].x= -3; ptr -> Object -> _CtlPts[21].y= 0; ptr -> Object -> _CtlPts[21].z= 3;
  ptr -> Object -> _CtlPts[22].x= -3; ptr -> Object -> _CtlPts[22].y= 0; ptr -> Object -> _CtlPts[22].z= 0;
  ptr -> Object -> _CtlPts[23].x= -3; ptr -> Object -> _CtlPts[23].y= 0; ptr -> Object -> _CtlPts[23].z= -3;
  ptr -> Object -> _CtlPts[24].x= 0; ptr -> Object -> _CtlPts[24].y= 0; ptr -> Object -> _CtlPts[24].z= -3;
  ptr -> Object -> _CtlPts[25].x= 0; ptr -> Object -> _CtlPts[25].y= 0; ptr -> Object -> _CtlPts[25].z= 3;
  ptr -> Object -> _CtlPts[26].x= -3; ptr -> Object -> _CtlPts[26].y= 3; ptr -> Object -> _CtlPts[26].z= 3;
  ptr -> Object -> _CtlPts[27].x= -3; ptr -> Object -> _CtlPts[27].y= 3; ptr -> Object -> _CtlPts[27].z= 0;
  ptr -> Object -> _CtlPts[28].x= -3; ptr -> Object -> _CtlPts[28].y= 3; ptr -> Object -> _CtlPts[28].z= -3;
  ptr -> Object -> _CtlPts[29].x= 0; ptr -> Object -> _CtlPts[29].y= 0; ptr -> Object -> _CtlPts[29].z= -3;
  ptr -> Object -> _CtlPts[30].x= 0; ptr -> Object -> _CtlPts[30].y= 0; ptr -> Object -> _CtlPts[30].z= 3;
  ptr -> Object -> _CtlPts[31].x= 0; ptr -> Object -> _CtlPts[31].y= 3; ptr -> Object -> _CtlPts[31].z= 3;
  ptr -> Object -> _CtlPts[32].x= 0; ptr -> Object -> _CtlPts[32].y= 3; ptr -> Object -> _CtlPts[32].z= 0;
  ptr -> Object -> _CtlPts[33].x= 0; ptr -> Object -> _CtlPts[33].y= 3; ptr -> Object -> _CtlPts[33].z= -3;
  ptr -> Object -> _CtlPts[34].x= 0; ptr -> Object -> _CtlPts[34].y= 0; ptr -> Object -> _CtlPts[34].z= -3;
  ptr -> Object -> _CtlPts[35].x= 0; ptr -> Object -> _CtlPts[35].y= 0; ptr -> Object -> _CtlPts[35].z= 3;
  ptr -> Object -> _CtlPts[36].x= 3; ptr -> Object -> _CtlPts[36].y= 3; ptr -> Object -> _CtlPts[36].z= 3;
  ptr -> Object -> _CtlPts[37].x= 3; ptr -> Object -> _CtlPts[37].y= 3; ptr -> Object -> _CtlPts[37].z= 0;
  ptr -> Object -> _CtlPts[38].x= 3; ptr -> Object -> _CtlPts[38].y= 3; ptr -> Object -> _CtlPts[38].z= -3;
  ptr -> Object -> _CtlPts[39].x= 0; ptr -> Object -> _CtlPts[39].y= 0; ptr -> Object -> _CtlPts[39].z= -3;
  ptr -> Object -> _CtlPts[40].x= 0; ptr -> Object -> _CtlPts[40].y= 0; ptr -> Object -> _CtlPts[40].z= 3;
  ptr -> Object -> _CtlPts[41].x= 3; ptr -> Object -> _CtlPts[41].y= 0; ptr -> Object -> _CtlPts[41].z= 3;
  ptr -> Object -> _CtlPts[42].x= 3; ptr -> Object -> _CtlPts[42].y= 0; ptr -> Object -> _CtlPts[42].z= 0;
  ptr -> Object -> _CtlPts[43].x= 3; ptr -> Object -> _CtlPts[43].y= 0; ptr -> Object -> _CtlPts[43].z= -3;
  ptr -> Object -> _CtlPts[44].x= 0; ptr -> Object -> _CtlPts[44].y= 0; ptr -> Object -> _CtlPts[44].z= -3;


  ptr -> Object -> H()[0]= 1;
  ptr -> Object -> H()[1]= sqrt(2)/2;
  ptr -> Object -> H()[2]= 1;
  ptr -> Object -> H()[3]= sqrt(2)/2;
  ptr -> Object -> H()[4]= 1;
  ptr -> Object -> H()[5]= sqrt(2)/2;
  ptr -> Object -> H()[6]= .5;
  ptr -> Object -> H()[7]= sqrt(2)/2;
  ptr -> Object -> H()[8]= .5;
  ptr -> Object -> H()[9]= sqrt(2)/2;
  ptr -> Object -> H()[10]= 1;
  ptr -> Object -> H()[11]= sqrt(2)/2;
  ptr -> Object -> H()[12]= 1;
  ptr -> Object -> H()[13]= sqrt(2)/2;
  ptr -> Object -> H()[14]= 1;
  ptr -> Object -> H()[15]= sqrt(2)/2;
  ptr -> Object -> H()[16]= .5;
  ptr -> Object -> H()[17]= sqrt(2)/2;
  ptr -> Object -> H()[18]= .5;
  ptr -> Object -> H()[19]= sqrt(2)/2;
  ptr -> Object -> H()[20]= 1;
  ptr -> Object -> H()[21]= sqrt(2)/2;
  ptr -> Object -> H()[22]= 1;
  ptr -> Object -> H()[23]= sqrt(2)/2;
  ptr -> Object -> H()[24]= 1;
  ptr -> Object -> H()[25]= sqrt(2)/2;
  ptr -> Object -> H()[26]= .5;
  ptr -> Object -> H()[27]= sqrt(2)/2;
  ptr -> Object -> H()[28]= .5;
  ptr -> Object -> H()[29]= sqrt(2)/2;
  ptr -> Object -> H()[30]= 1;
  ptr -> Object -> H()[31]= sqrt(2)/2;
  ptr -> Object -> H()[32]= 1;
  ptr -> Object -> H()[33]= sqrt(2)/2;
  ptr -> Object -> H()[34]= 1;
  ptr -> Object -> H()[35]= sqrt(2)/2;
  ptr -> Object -> H()[36]= .5;
  ptr -> Object -> H()[37]= sqrt(2)/2;
  ptr -> Object -> H()[38]= .5;
  ptr -> Object -> H()[39]= sqrt(2)/2;
  ptr -> Object -> H()[40]= 1;
  ptr -> Object -> H()[41]= sqrt(2)/2;
  ptr -> Object -> H()[42]= 1;
  ptr -> Object -> H()[43]= sqrt(2)/2;
  ptr -> Object -> H()[44]= 1;


  ptr -> Object -> KnotVector(0)[0]= 0;
  ptr -> Object -> KnotVector(0)[1]= 0;
  ptr -> Object -> KnotVector(0)[2]= 0;
  ptr -> Object -> KnotVector(0)[3]= 0.25;
  ptr -> Object -> KnotVector(0)[4]= 0.25;
  ptr -> Object -> KnotVector(0)[5]= 0.5;
  ptr -> Object -> KnotVector(0)[6]= 0.5;
  ptr -> Object -> KnotVector(0)[7]= 0.75;
  ptr -> Object -> KnotVector(0)[8]= 0.75;
  ptr -> Object -> KnotVector(0)[9]= 1;
  ptr -> Object -> KnotVector(0)[10]= 1;
  ptr -> Object -> KnotVector(0)[11]= 1;

  ptr -> Object -> KnotVector(1)[0]= 0;
  ptr -> Object -> KnotVector(1)[1]= 0;
  ptr -> Object -> KnotVector(1)[2]= 0;
  ptr -> Object -> KnotVector(1)[3]= 0.5;
  ptr -> Object -> KnotVector(1)[4]= 0.5;
  ptr -> Object -> KnotVector(1)[5]= 1;
  ptr -> Object -> KnotVector(1)[6]= 1;
  ptr -> Object -> KnotVector(1)[7]= 1;


  ptr -> Object -> KnotType(0,NURBS_KV_Open);
  ptr -> Object -> KnotType(1,NURBS_KV_Open);

  ptr -> Object -> Order(0,3);
  ptr -> Object -> Order(1,3);

  ptr -> Object -> RecalculateKnotVector(0);
} //eof Object_Primitives::Sphere()


void Object_Primitives::Create(Object_List::ObjectNode *ptr, unsigned char Type) {
  switch (Type) {
    case OBJECT_PRIMITIVE_CURVE:
      Curve(ptr);
      break;

    case OBJECT_PRIMITIVE_SURFACE:
      Surface(ptr);
      break;

    case OBJECT_PRIMITIVE_CIRCLE:
      Circle(ptr);
      break;

    case OBJECT_PRIMITIVE_TUBE:
      Tube(ptr);
      break;

    case OBJECT_PRIMITIVE_TORUS:
      Torus(ptr);
      break;

    case OBJECT_PRIMITIVE_SPHERE:
      Sphere(ptr);
      break;

    default:
      break;
  } //eos
} //eof Object_Primitives::Create()
