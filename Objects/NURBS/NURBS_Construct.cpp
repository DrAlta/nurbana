#include "NURBS_Construct.h"


void NURBS_Construct::Loft(Object_NURBS **objs, int N, int V) {
  // obj2 and obj3 are Input curves, obj1 is output surface
  int		i,k,MaxOrder,KVtmplen,KVlen,Difflen;
  double	MaxKnot,KVtmp[128],KVfnl[128],Diff[128];

  // Overwrite CtlPts with WCCtlPts for each Curve
  for (i= 1; i < N; i++)
    for (k= 0; k < objs[i] -> Length(0) * objs[i] -> Length(1); k++) {
      objs[i] -> _CtlPts[k].x= objs[i] -> _WCCtlPts[k].x;
      objs[i] -> _CtlPts[k].y= objs[i] -> _WCCtlPts[k].y;
      objs[i] -> _CtlPts[k].z= objs[i] -> _WCCtlPts[k].z;
    } //eof

  // Since algorithm is built around N x 1 curves, change all 1 x N to N x 1
  for (i= 1; i < N; i++)
    if (objs[i] -> Length(0) == 1) {
      objs[i] -> Length(0,objs[i] -> Length(1));
      objs[i] -> Length(1,1);

      objs[i] -> Order(0,objs[i] -> Order(1));
      objs[i] -> Order(1,2);

      objs[i] -> SetLength(objs[i] -> Length(0),objs[i] -> Length(0));

      for (k= 0; k < objs[i] -> Length(0) + objs[i] -> Order(0); k++)
        objs[i] -> KnotVector(0)[k]= objs[i] -> KnotVector(1)[k];

      objs[i] -> SetLength(objs[i] -> Length(0),1);
    } //fi

  // Match Degree of Curves using highest degree
  MaxOrder= 0;
  for (i= 1; i < N; i++)
    if (objs[i] -> Order(0) > MaxOrder)
      MaxOrder= objs[i] -> Order(0);

  // Raise Degree of Curves with Lower Order than Max Order
  for (i= 1; i < N; i++)
    if (objs[i] -> Order(0) < MaxOrder)
      NURBS_Degree::Elevate(objs[i],MaxOrder-objs[i] -> Order(0),0);

  // Determine largest Knot value in each Curves Knot Vector
  MaxKnot= 0;
  for (i= 1; i < N; i++)
    if (objs[i] -> KnotVector(0)[objs[i] -> Length(0) + objs[i] -> Order(0) - 1] > MaxKnot)
      MaxKnot= objs[i] -> KnotVector(0)[objs[i] -> Length(0) + objs[i] -> Order(0) - 1];

  // Scale Knot Vectors
  for (i= 1; i < N; i++)
    NURBS_Knot::Scale(objs[i] -> KnotVector(0),objs[i] -> Length(0) + objs[i] -> Order(0),MaxKnot);

  // Merge Knot Vectors
  NURBS_Knot::MergeKnotVectors(objs[1] -> Length(0) + objs[1] -> Order(0), objs[1] -> KnotVector(0), objs[2] -> Length(0) + objs[2] -> Order(0), objs[2] -> KnotVector(0), KVtmplen, KVtmp);
//  printf("KVtmplen: %d\n",KVtmplen);

  for (i= 3; i < N; i++) {
    NURBS_Knot::MergeKnotVectors(objs[i] -> Length(0) + objs[i] -> Order(0), objs[i] -> KnotVector(0), KVtmplen, KVtmp, KVlen, KVfnl);
    for (k= 0; k < KVlen; k++)
      KVtmp[k]= KVfnl[k];
    KVtmplen= KVlen;
  } //eof

//  printf("KVlen: %d\n",KVlen);
//  for (i= 0; i < KVlen; i++)
//    printf("[%.3f]",KVfnl[i]);


  // Refine Curves over their new Knot Vector
  for (i= 1; i < N; i++) {
    NURBS_Knot::Difference(objs[i] -> KnotVector(0), objs[i] -> Length(0)+objs[i] -> Order(0),KVfnl,KVlen,Diff,Difflen);
    if (Difflen)
      NURBS_Knot::Refine(objs[i],Diff,Difflen,0);
  } //eof

  // Copy new Points, KnotVector, etc... into obj1
  objs[0] -> SetLength(objs[1] -> Length(0),N-1);
  objs[0] -> Order(0,objs[1] -> Order(0));
  objs[0] -> Order(1,V);

  objs[0] -> Length(0,objs[1] -> Length(0));
  objs[0] -> Length(1,N-1);


  // Assign new KnotVector
  for (i= 0; i < KVlen; i++)
    objs[0] -> KnotVector(0)[i]= KVfnl[i];


  // Generate V Knot Vector
  for (i= 0; i < objs[0] -> Order(1); i++)
    objs[0] -> KnotVector(1)[i]= 0;
  for (i= objs[0] -> Length(1); i < objs[0] -> Length(1) + objs[0] -> Order(1); i++)
    objs[0] -> KnotVector(1)[i]= 1;
  for (i= 0; i < objs[0] -> Length(1) - objs[0] -> Order(1); i++)
    objs[0] -> KnotVector(1)[i+objs[0] -> Order(1)]= (1.0+i)/(objs[0] -> Length(1) - objs[0] -> Order(1) + 1.0);

//  for (i= 0; i < objs[0] -> Length(1) + objs[0] -> Order(1); i++)
//    printf("KV[%d]: %f\n",i,objs[0] -> KnotVector(1)[i]);

  // Assign Control Points
  for (k= 0; k < objs[1] -> Length(0); k++) {
    for (i= 1; i < N; i++) {
      objs[0] -> CtlPts()[k*(N-1)+i-1].x= objs[i] -> CtlPts()[k].x;
      objs[0] -> CtlPts()[k*(N-1)+i-1].y= objs[i] -> CtlPts()[k].y;
      objs[0] -> CtlPts()[k*(N-1)+i-1].z= objs[i] -> CtlPts()[k].z;
    } //eof
  } //eof

  objs[0] -> _ReCalcTssPts= 1;


//  free(KVfnl);
} //eof NURBS_Construct::Loft()



void NURBS_Construct::Blend(Object_NURBS *obj1, Object_NURBS *obj2, Object_NURBS *obj3, int Twist) {
  Object_NURBS	*tmp1,*tmp2;
  Point3d	Vector;
  double	MaxKnot,k,C,D1,D2,D3,D4,*KV,*KVDiff;
  int		i,n,u,v,KVLen,KVDiffLen;
  bool		E1,E2;

  // +2+
  // 1+3
  // +0+


  // Create two Temporary Surfaces based on obj1 and obj2
  tmp1= new Object_NURBS(NURBS::OBJList);
  tmp2= new Object_NURBS(NURBS::OBJList);
//  tmp1= obj1;
//  tmp2= obj2;

  NURBS_Generate::Duplicate(tmp1,obj1);
  NURBS_Generate::Duplicate(tmp2,obj2);

  E1= tmp1 -> Edge(0) || tmp1 -> Edge(2);
  E2= tmp2 -> Edge(0) || tmp2 -> Edge(2);

  // Determine Max Order for New Surface
  if (E1) {
    i= tmp1 -> Order(1);
    n= tmp1 -> Order(0);
    MaxKnot= tmp1 -> KnotVector(0)[tmp1 -> Length(0) + tmp1 -> Order(0) - 1];
  } else {
    i= tmp1 -> Order(0);
    n= tmp1 -> Order(1);
    MaxKnot= tmp1 -> KnotVector(1)[tmp1 -> Length(1) + tmp1 -> Order(1) - 1];
  } //fi

  if (E2) {
    u= tmp2 -> Order(1);
    v= tmp2 -> Order(0);
    if (tmp2 -> KnotVector(0)[tmp2 -> Length(0) + tmp2 -> Order(0) - 1] > MaxKnot)
      MaxKnot= tmp2 -> KnotVector(0)[tmp2 -> Length(0) + tmp2 -> Order(0) - 1];
  } else {
    u= tmp2 -> Order(0);
    v= tmp2 -> Order(1);
    if (tmp2 -> KnotVector(1)[tmp2 -> Length(1) + tmp2 -> Order(1) - 1] > MaxKnot)
      MaxKnot= tmp2 -> KnotVector(1)[tmp2 -> Length(1) + tmp2 -> Order(1) - 1];
  } //fi

  u= i > u ? i : u; // Max Order U
  v= n > v ? n : v; // Max Order V

  // Degree Elevate Tmp Surfaces with lower degree
  if (u - tmp1 -> Order(0) || v - tmp1 -> Order(1))
    NURBS_Degree::Elevate(tmp1,u - tmp1 -> Order(0), v - tmp1 -> Order(1));  
  if (u - tmp2 -> Order(0) || v - tmp2 -> Order(1))
    NURBS_Degree::Elevate(tmp2,u - tmp2 -> Order(0), v - tmp2 -> Order(1));  

  obj3 -> Order(0,u);
  obj3 -> Order(1,v);

  obj3 -> Length(0,2*u);

  //
  // Refine Knot Vectors so that obj3 V lengths are the same
  //

  // Scale Knot Vectors
  NURBS_Knot::Scale(tmp1 -> KnotVector(!E1),tmp1 -> Length(!E1) + tmp1 -> Order(!E1),MaxKnot);
  NURBS_Knot::Scale(tmp2 -> KnotVector(!E2),tmp2 -> Length(!E2) + tmp2 -> Order(!E2),MaxKnot);

  // Merge Knot Vectors
  KV= (double*)malloc(sizeof(double)*(tmp1 -> Length(!E1) + tmp1 -> Order(!E1) + tmp2 -> Length(!E2) + tmp2 -> Order(!E2)));
  KVDiff= (double*)malloc(sizeof(double)*(tmp1 -> Length(!E1) + tmp1 -> Order(!E1) + tmp2 -> Length(!E2) + tmp2 -> Order(!E2)));
  NURBS_Knot::MergeKnotVectors(tmp1 -> Length(!E1) + tmp1 -> Order(!E1), tmp1 -> KnotVector(!E1), tmp2 -> Length(!E2) + tmp2 -> Order(!E2), tmp2 -> KnotVector(!E2), KVLen, KV);

  // Refine Knot Vectors of tmp1 and tmp2
  NURBS_Knot::Difference(tmp1 -> KnotVector(!E1), tmp1 -> Length(!E1) + tmp1 -> Order(!E1), KV, KVLen, KVDiff, KVDiffLen);
  if (KVDiffLen)
    NURBS_Knot::Refine(tmp1,KVDiff,KVDiffLen,!E1);

  NURBS_Knot::Difference(tmp2 -> KnotVector(!E2), tmp2 -> Length(!E2) + tmp2 -> Order(!E2), KV, KVLen, KVDiff, KVDiffLen);
  if (KVDiffLen)
    NURBS_Knot::Refine(tmp2,KVDiff,KVDiffLen,!E2);


  // Recompute WC CtlPts
  Object_WC::CtlPts((Object_Base*)tmp1);
  Object_WC::CtlPts((Object_Base*)tmp2);

  // Set Length for Obj3
  obj3 -> SetLength(obj3 -> Length(0),tmp1 -> Length(!E1));
  obj3 -> Length(1,tmp1 -> Length(!E1));

  D1= 999999;
  for (i= 0; i < tmp1 -> Length(0) * tmp1 -> Length(1); i++) {
    for (n= 0; n < tmp2 -> Length(0) * tmp2 -> Length(1); n++) {
      D2= sqrt((tmp1 -> WCCtlPts()[i].x - tmp2 -> WCCtlPts()[n].x)*(tmp1 -> WCCtlPts()[i].x - tmp2 -> WCCtlPts()[n].x) + (tmp1 -> WCCtlPts()[i].y - tmp2 -> WCCtlPts()[n].y)*(tmp1 -> WCCtlPts()[i].y - tmp2 -> WCCtlPts()[n].y) + (tmp1 -> WCCtlPts()[i].z - tmp2 -> WCCtlPts()[n].z)*(tmp1 -> WCCtlPts()[i].z - tmp2 -> WCCtlPts()[n].z));
      if (D2 < D1) D1= D2;
    } //eof
  } //eof

//  printf("Min Dist: %f\n",D1);
  D1/= 2;
  D2= D1;


  D3= 0;
  if (tmp1 -> Edge(0)) {
//printf("A0\n");
    for (i= 0; i < tmp1 -> Length(0); i++) {
      D4= sqrt((tmp1 -> WCCtlPts()[i*tmp1 -> Length(1)].x - tmp1 -> WCCtlPts()[i*tmp1 -> Length(1) + 1].x)*(tmp1 -> WCCtlPts()[i*tmp1 -> Length(1)].x - tmp1 -> WCCtlPts()[i*tmp1 -> Length(1) + 1].x) + (tmp1 -> WCCtlPts()[i*tmp1 -> Length(1)].y - tmp1 -> WCCtlPts()[i*tmp1 -> Length(1) + 1].y)*(tmp1 -> WCCtlPts()[i*tmp1 -> Length(1)].y - tmp1 -> WCCtlPts()[i*tmp1 -> Length(1) + 1].y) + (tmp1 -> WCCtlPts()[i*tmp1 -> Length(1)].z - tmp1 -> WCCtlPts()[i*tmp1 -> Length(1) + 1].z)*(tmp1 -> WCCtlPts()[i*tmp1 -> Length(1)].z - tmp1 -> WCCtlPts()[i*tmp1 -> Length(1) + 1].z));
      if (D4 > D3) D3= D4;
    } //eof

    D3 ? C= D1/D3 : C= 1;

    for (i= 0; i < tmp1 -> Length(0); i++) {
      obj3 -> CtlPts()[i].x= tmp1 -> WCCtlPts()[i*tmp1 -> Length(1)].x;
      obj3 -> CtlPts()[i].y= tmp1 -> WCCtlPts()[i*tmp1 -> Length(1)].y;
      obj3 -> CtlPts()[i].z= tmp1 -> WCCtlPts()[i*tmp1 -> Length(1)].z;

      Vector.x= (tmp1 -> WCCtlPts()[i*tmp1 -> Length(1)].x - tmp1 -> WCCtlPts()[i*tmp1 -> Length(1) + 1].x);
      Vector.y= (tmp1 -> WCCtlPts()[i*tmp1 -> Length(1)].y - tmp1 -> WCCtlPts()[i*tmp1 -> Length(1) + 1].y);
      Vector.z= (tmp1 -> WCCtlPts()[i*tmp1 -> Length(1)].z - tmp1 -> WCCtlPts()[i*tmp1 -> Length(1) + 1].z);

      for (n= 1; n < tmp1 -> Order(1); n++) {
        v= n*obj3 -> Length(1) + i;
        k= double(n)/double(tmp1 -> Order(1)-1);
        obj3 -> CtlPts()[v].x= k * C * Vector.x + tmp1 -> WCCtlPts()[i*tmp1 -> Length(1)].x;
        obj3 -> CtlPts()[v].y= k * C * Vector.y + tmp1 -> WCCtlPts()[i*tmp1 -> Length(1)].y;
        obj3 -> CtlPts()[v].z= k * C * Vector.z + tmp1 -> WCCtlPts()[i*tmp1 -> Length(1)].z;
      } //eof
    } //eof
  } else if (tmp1 -> Edge(1)) {
//printf("A1\n");
    for (i= 0; i < tmp1 -> Length(1); i++) {
      D4= sqrt((tmp1 -> WCCtlPts()[i].x - tmp1 -> WCCtlPts()[tmp1 -> Length(1) + i].x)*(tmp1 -> WCCtlPts()[i].x - tmp1 -> WCCtlPts()[tmp1 -> Length(1) + i].x) + (tmp1 -> WCCtlPts()[i].y - tmp1 -> WCCtlPts()[tmp1 -> Length(1) + i].y)*(tmp1 -> WCCtlPts()[i].y - tmp1 -> WCCtlPts()[tmp1 -> Length(1) + i].y) + (tmp1 -> WCCtlPts()[i].z - tmp1 -> WCCtlPts()[tmp1 -> Length(1) + i].z)*(tmp1 -> WCCtlPts()[i].z - tmp1 -> WCCtlPts()[tmp1 -> Length(1) + i].z));
      if (D4 > D3) D3= D4;
    } //eof

    D3 ? C= D1/D3 : C= 1;

    for (i= 0; i < tmp1 -> Length(1); i++) {
      obj3 -> CtlPts()[i].x= tmp1 -> WCCtlPts()[i].x;
      obj3 -> CtlPts()[i].y= tmp1 -> WCCtlPts()[i].y;
      obj3 -> CtlPts()[i].z= tmp1 -> WCCtlPts()[i].z;

      Vector.x= (tmp1 -> WCCtlPts()[i].x - tmp1 -> WCCtlPts()[tmp1 -> Length(1) + i].x);
      Vector.y= (tmp1 -> WCCtlPts()[i].y - tmp1 -> WCCtlPts()[tmp1 -> Length(1) + i].y);
      Vector.z= (tmp1 -> WCCtlPts()[i].z - tmp1 -> WCCtlPts()[tmp1 -> Length(1) + i].z);

      for (n= 1; n < tmp1 -> Order(0); n++) {
        v= n*obj3 -> Length(1) + i;
        k= (double(n)/double(tmp1 -> Order(0)-1));
        obj3 -> CtlPts()[v].x= k * C * Vector.x + tmp1 -> WCCtlPts()[i].x;
        obj3 -> CtlPts()[v].y= k * C * Vector.y + tmp1 -> WCCtlPts()[i].y;
        obj3 -> CtlPts()[v].z= k * C * Vector.z + tmp1 -> WCCtlPts()[i].z;
      } //eof
    } //eof
  } else if (tmp1 -> Edge(2)) {
//printf("A2\n");
    for (i= 0; i < tmp1 -> Length(0); i++) {
      D4= sqrt((tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1)-1].x - tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1)-2].x)*(tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1)-1].x - tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1)-2].x) + (tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1)-1].y - tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1)-2].y)*(tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1)-1].y - tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1)-2].y) + (tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1)-1].z - tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1)-2].z)*(tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1)-1].z - tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1)-2].z));
      if (D4 > D3) D3= D4;
    } //eof

    D3 ? C= D1/D3 : C= 1;

    for (i= 0; i < tmp1 -> Length(0); i++) {
      obj3 -> CtlPts()[i].x= tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1) - 1].x;
      obj3 -> CtlPts()[i].y= tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1) - 1].y;
      obj3 -> CtlPts()[i].z= tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1) - 1].z;

      Vector.x= (tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1)-1].x - tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1)-2].x);
      Vector.y= (tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1)-1].y - tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1)-2].y);
      Vector.z= (tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1)-1].z - tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1)-2].z);

      for (n= 1; n < tmp1 -> Order(1); n++) {
        v= n*obj3 -> Length(1) + i;
        k= (double(n)/double(tmp1 -> Order(1)-1));
        obj3 -> CtlPts()[v].x= k * C * Vector.x + tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1)-1].x;
        obj3 -> CtlPts()[v].y= k * C * Vector.y + tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1)-1].y;
        obj3 -> CtlPts()[v].z= k * C * Vector.z + tmp1 -> WCCtlPts()[(i+1)*tmp1 -> Length(1)-1].z;
      } //eof
    } //eof
  } else {
//printf("A3\n");
    for (i= 0; i < tmp1 -> Length(1); i++) {
      D4= sqrt((tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-1) * tmp1 -> Length(1) + i].x - tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-2) * tmp1 -> Length(1) + i].x)*(tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-1) * tmp1 -> Length(1) + i].x - tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-2) * tmp1 -> Length(1) + i].x) + (tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-1) * tmp1 -> Length(1) + i].y - tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-2) * tmp1 -> Length(1) + i].y)*(tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-1) * tmp1 -> Length(1) + i].y - tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-2) * tmp1 -> Length(1) + i].y) + (tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-1) * tmp1 -> Length(1) + i].z - tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-2) * tmp1 -> Length(1) + i].z)*(tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-1) * tmp1 -> Length(1) + i].z - tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-2) * tmp1 -> Length(1) + i].z));
      if (D4 > D3) D3= D4;
    } //eof

    D3 ? C= D1/D3 : C= 1;

    for (i= 0; i < tmp1 -> Length(1); i++) {
      obj3 -> CtlPts()[i].x= tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-1) * tmp1 -> Length(1) + i].x;
      obj3 -> CtlPts()[i].y= tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-1) * tmp1 -> Length(1) + i].y;
      obj3 -> CtlPts()[i].z= tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-1) * tmp1 -> Length(1) + i].z;

      Vector.x= (tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-1) * tmp1 -> Length(1) + i].x - tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-2) * tmp1 -> Length(1) + i].x);
      Vector.y= (tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-1) * tmp1 -> Length(1) + i].y - tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-2) * tmp1 -> Length(1) + i].y);
      Vector.z= (tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-1) * tmp1 -> Length(1) + i].z - tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-2) * tmp1 -> Length(1) + i].z);

      for (n= tmp1 -> Order(0)-1; n > 0; n--) {
        v= n*obj3 -> Length(1) + i;
        k= (double(n)/double(tmp1 -> Order(0)-1));
        obj3 -> CtlPts()[v].x= k * C * Vector.x + tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-1) * tmp1 -> Length(1) + i].x;
        obj3 -> CtlPts()[v].y= k * C * Vector.y + tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-1) * tmp1 -> Length(1) + i].y;
        obj3 -> CtlPts()[v].z= k * C * Vector.z + tmp1 -> WCCtlPts()[(tmp1 -> Length(0)-1) * tmp1 -> Length(1) + i].z;
      } //eof
    } //eof
  } //fi



  D3= 0;
  if (tmp2 -> Edge(0)) {
//printf("B0\n");
    for (i= 0; i < tmp2 -> Length(0); i++) {
      D4= sqrt((tmp2 -> WCCtlPts()[i*tmp2 -> Length(1)].x - tmp2 -> WCCtlPts()[i*tmp2 -> Length(1) + 1].x)*(tmp2 -> WCCtlPts()[i*tmp2 -> Length(1)].x - tmp2 -> WCCtlPts()[i*tmp2 -> Length(1) + 1].x) + (tmp2 -> WCCtlPts()[i*tmp2 -> Length(1)].y - tmp2 -> WCCtlPts()[i*tmp2 -> Length(1) + 1].y)*(tmp2 -> WCCtlPts()[i*tmp2 -> Length(1)].y - tmp2 -> WCCtlPts()[i*tmp2 -> Length(1) + 1].y) + (tmp2 -> WCCtlPts()[i*tmp2 -> Length(1)].z - tmp2 -> WCCtlPts()[i*tmp2 -> Length(1) + 1].z)*(tmp2 -> WCCtlPts()[i*tmp2 -> Length(1)].z - tmp2 -> WCCtlPts()[i*tmp2 -> Length(1) + 1].z));
      if (D4 > D3) D3= D4;
    } //eof

    D3 ? C= D2/D3 : C= 1;

    for (i= 0; i < tmp2 -> Length(0); i++) {
      u= Twist ? (obj3 -> Length(0)-1)*obj3 -> Length(1) + obj3 -> Length(1) - i - 1 : (obj3 -> Length(0)-1)*obj3 -> Length(1)+i;
      obj3 -> CtlPts()[u].x= tmp2 -> WCCtlPts()[i*tmp2 -> Length(1)].x;
      obj3 -> CtlPts()[u].y= tmp2 -> WCCtlPts()[i*tmp2 -> Length(1)].y;
      obj3 -> CtlPts()[u].z= tmp2 -> WCCtlPts()[i*tmp2 -> Length(1)].z;

      Vector.x= (tmp2 -> WCCtlPts()[i*tmp2 -> Length(1)].x - tmp2 -> WCCtlPts()[i*tmp2 -> Length(1) + 1].x);
      Vector.y= (tmp2 -> WCCtlPts()[i*tmp2 -> Length(1)].y - tmp2 -> WCCtlPts()[i*tmp2 -> Length(1) + 1].y);
      Vector.z= (tmp2 -> WCCtlPts()[i*tmp2 -> Length(1)].z - tmp2 -> WCCtlPts()[i*tmp2 -> Length(1) + 1].z);

      for (n= 1; n < tmp2 -> Order(1); n++) {
        v= Twist ? (obj3 -> Length(0)-1)*obj3 -> Length(1) - n * obj3 -> Length(1) + obj3 -> Length(1) - i - 1 : (obj3 -> Length(0)-1)*obj3 -> Length(1) - n * obj3 -> Length(1) + i;
        k= (double(n)/double(tmp2 -> Order(1)-1));
        obj3 -> CtlPts()[v].x= k * C * Vector.x + tmp2 -> WCCtlPts()[i*tmp2 -> Length(1)].x;
        obj3 -> CtlPts()[v].y= k * C * Vector.y + tmp2 -> WCCtlPts()[i*tmp2 -> Length(1)].y;
        obj3 -> CtlPts()[v].z= k * C * Vector.z + tmp2 -> WCCtlPts()[i*tmp2 -> Length(1)].z;
      } //eof
    } //eof
  } else if (tmp2 -> Edge(1)) {
//printf("B1\n");
    for (i= 0; i < tmp2 -> Length(1); i++) {
      D4= sqrt((tmp2 -> WCCtlPts()[i].x - tmp2 -> WCCtlPts()[tmp2 -> Length(1) + i].x)*(tmp2 -> WCCtlPts()[i].x - tmp2 -> WCCtlPts()[tmp2 -> Length(1) + i].x) + (tmp2 -> WCCtlPts()[i].y - tmp2 -> WCCtlPts()[tmp2 -> Length(1) + i].y)*(tmp2 -> WCCtlPts()[i].y - tmp2 -> WCCtlPts()[tmp2 -> Length(1) + i].y) + (tmp2 -> WCCtlPts()[i].z - tmp2 -> WCCtlPts()[tmp2 -> Length(1) + i].z)*(tmp2 -> WCCtlPts()[i].z - tmp2 -> WCCtlPts()[tmp2 -> Length(1) + i].z));
      if (D4 > D3) D3= D4;
    } //eof

    D3 ? C= D2/D3 : C= 1;

    for (i= 0; i < tmp2 -> Length(1); i++) {
      u= Twist ? (obj3 -> Length(0)-1)*obj3 -> Length(1) + obj3 -> Length(1) - i - 1 : (obj3 -> Length(0)-1)*obj3 -> Length(1)+i;
      obj3 -> CtlPts()[u].x= tmp2 -> WCCtlPts()[i].x;
      obj3 -> CtlPts()[u].y= tmp2 -> WCCtlPts()[i].y;
      obj3 -> CtlPts()[u].z= tmp2 -> WCCtlPts()[i].z;

      Vector.x= (tmp2 -> WCCtlPts()[i].x - tmp2 -> WCCtlPts()[tmp2 -> Length(1) + i].x);
      Vector.y= (tmp2 -> WCCtlPts()[i].y - tmp2 -> WCCtlPts()[tmp2 -> Length(1) + i].y);
      Vector.z= (tmp2 -> WCCtlPts()[i].z - tmp2 -> WCCtlPts()[tmp2 -> Length(1) + i].z);

      for (n= 1; n < tmp2 -> Order(0); n++) {
        v= Twist ? (obj3 -> Length(0)-1)*obj3 -> Length(1) - n * tmp2 -> Length(1) + tmp2 -> Length(1) - i - 1 : (obj3 -> Length(0)-1)*obj3 -> Length(1) - n * tmp2 -> Length(1) + i;
        k= (double(n)/double(tmp2 -> Order(0)-1));
        obj3 -> CtlPts()[v].x= k * C * Vector.x + tmp2 -> WCCtlPts()[i].x;
        obj3 -> CtlPts()[v].y= k * C * Vector.y + tmp2 -> WCCtlPts()[i].y;
        obj3 -> CtlPts()[v].z= k * C * Vector.z + tmp2 -> WCCtlPts()[i].z;
      } //eof
    } //eof
  } else if (tmp2 -> Edge(2)) {
//printf("B2\n");
    for (i= 0; i < tmp2 -> Length(0); i++) {
      D4= sqrt((tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1)-1].x - tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1)-2].x)*(tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1)-1].x - tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1)-2].x) + (tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1)-1].y - tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1)-2].y)*(tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1)-1].y - tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1)-2].y) + (tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1)-1].z - tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1)-2].z)*(tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1)-1].z - tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1)-2].z));
      if (D4 > D3) D3= D4;
    } //eof

    D3 ? C= D2/D3 : C= 1;

    for (i= 0; i < tmp2 -> Length(0); i++) {
      u= Twist ? (obj3 -> Length(0)-1)*obj3 -> Length(1) + obj3 -> Length(1) - i - 1 : (obj3 -> Length(0)-1)*obj3 -> Length(1)+i;
      obj3 -> CtlPts()[u].x= tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1) - 1].x;
      obj3 -> CtlPts()[u].y= tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1) - 1].y;
      obj3 -> CtlPts()[u].z= tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1) - 1].z;

      Vector.x= (tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1)-1].x - tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1)-2].x);
      Vector.y= (tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1)-1].y - tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1)-2].y);
      Vector.z= (tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1)-1].z - tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1)-2].z);

      for (n= 1; n < tmp2 -> Order(1); n++) {
        v= Twist ? (obj3 -> Length(0)-1)*obj3 -> Length(1) - n * obj3 -> Length(1) + obj3 -> Length(1) - i - 1 : (obj3 -> Length(0)-1)*obj3 -> Length(1) - n * obj3 -> Length(1) + i;
        k= (double(n)/double(tmp2 -> Order(1)-1));
        obj3 -> CtlPts()[v].x= k * C * Vector.x + tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1)-1].x;
        obj3 -> CtlPts()[v].y= k * C * Vector.y + tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1)-1].y;
        obj3 -> CtlPts()[v].z= k * C * Vector.z + tmp2 -> WCCtlPts()[(i+1)*tmp2 -> Length(1)-1].z;
      } //eof
    } //eof
  } else {
//printf("B3\n");
    for (i= 0; i < tmp2 -> Length(1); i++) {
      D4= sqrt((tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-1) * tmp2 -> Length(1) + i].x - tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-2) * tmp2 -> Length(1) + i].x)*(tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-1) * tmp2 -> Length(1) + i].x - tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-2) * tmp2 -> Length(1) + i].x) + (tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-1) * tmp2 -> Length(1) + i].y - tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-2) * tmp2 -> Length(1) + i].y)*(tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-1) * tmp2 -> Length(1) + i].y - tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-2) * tmp2 -> Length(1) + i].y) + (tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-1) * tmp2 -> Length(1) + i].z - tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-2) * tmp2 -> Length(1) + i].z)*(tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-1) * tmp2 -> Length(1) + i].z - tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-2) * tmp2 -> Length(1) + i].z));
      if (D4 > D3) D3= D4;
    } //eof

    D3 ? C= D2/D3 : C= 1;

    for (i= 0; i < tmp2 -> Length(1); i++) {
      u= Twist ? (obj3 -> Length(0)-1)*obj3 -> Length(1) + obj3 -> Length(1) - i - 1 : (obj3 -> Length(0)-1)*obj3 -> Length(1)+i;
      obj3 -> CtlPts()[u].x= tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-1) * tmp2 -> Length(1) + i].x;
      obj3 -> CtlPts()[u].y= tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-1) * tmp2 -> Length(1) + i].y;
      obj3 -> CtlPts()[u].z= tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-1) * tmp2 -> Length(1) + i].z;

      Vector.x= (tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-1) * tmp2 -> Length(1) + i].x - tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-2) * tmp2 -> Length(1) + i].x);
      Vector.y= (tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-1) * tmp2 -> Length(1) + i].y - tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-2) * tmp2 -> Length(1) + i].y);
      Vector.z= (tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-1) * tmp2 -> Length(1) + i].z - tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-2) * tmp2 -> Length(1) + i].z);

      for (n= tmp2 -> Order(0)-1; n > 0; n--) {
        v= Twist ? (obj3 -> Length(0)-1)*obj3 -> Length(1) - n * obj3 -> Length(1) + obj3 -> Length(1) - i - 1 : (obj3 -> Length(0)-1)*obj3 -> Length(1) - n * obj3 -> Length(1) + i;
        k= (double(n)/double(tmp2 -> Order(0)-1));
        obj3 -> CtlPts()[v].x= k * C * Vector.x + tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-1) * tmp2 -> Length(1) + i].x;
        obj3 -> CtlPts()[v].y= k * C * Vector.y + tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-1) * tmp2 -> Length(1) + i].y;
        obj3 -> CtlPts()[v].z= k * C * Vector.z + tmp2 -> WCCtlPts()[(tmp2 -> Length(0)-1) * tmp2 -> Length(1) + i].z;
      } //eof
    } //eof
  } //fi

  obj3 -> RecalculateKnotVector(1);

  NURBS::OBJList -> RemoveNode(tmp1 -> ID());
  NURBS::OBJList -> RemoveNode(tmp2 -> ID());
  free(KV);
  free(KVDiff);
} //eof NURBS_Construct::Blend()