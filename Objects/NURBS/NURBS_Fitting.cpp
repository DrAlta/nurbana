#include "NURBS_Fitting.h"


void NURBS_Fitting::GlobalInterp(Object_NURBS *obj, int CPU, int CPV, int UK) {
  Point3d	*U,*V;
  int		i,n,TessUV[2];

  if (CPU < obj -> Order(0) || CPV < obj ->Order(1)) return;

  U= (Point3d*)malloc(sizeof(Point3d)*CPU);
  V= (Point3d*)malloc(sizeof(Point3d)*CPV);

  TessUV[0]= obj -> Tess(0);
  TessUV[1]= obj -> Tess(1);
  obj -> Tess(0,CPU);
  obj -> Tess(1,CPV);

  // Retesselate object with U and V tesselation to obtain
  // interpolation points.
  NURBS_Generate::Surface(obj);

  // Update Length
  obj -> Length(0,CPU);
  obj -> Length(1,CPV);
  obj -> SetLength(CPU,CPV);

  for (n= 0; n < CPV; n++) {

    // Populate U Curve
    for (i= 0; i < CPU; i++) {
      U[i].x= obj -> TssPts()[i*obj ->Tess(1)+n].x;
      U[i].y= obj -> TssPts()[i*obj ->Tess(1)+n].y;
      U[i].z= obj -> TssPts()[i*obj ->Tess(1)+n].z;
    } //eof

//    for (i= 0; i < U; i++)
//      printf("[%.3f][%.3f][%.3f]",obj -> TssPts()[i].x,obj -> TssPts()[i].y,obj -> TssPts()[i].z);

    GlobalInterpCurve(U,CPU,obj -> Order(0)-1,obj -> KnotVector(0),UK);

    // Store New Control Points
    for (i= 0; i < CPU; i++) {
      obj -> CtlPts()[i*CPV+n].x= U[i].x;
      obj -> CtlPts()[i*CPV+n].y= U[i].y;
      obj -> CtlPts()[i*CPV+n].z= U[i].z;
    } //eof
  } //eof


  if (CPV > 1) {
    // Interpolate through U Control Points
    for (n= 0; n < CPU; n++) {
      // Populate U Curve
      for (i= 0; i < CPV; i++) {
        V[i].x= obj -> CtlPts()[i+CPV*n].x;
        V[i].y= obj -> CtlPts()[i+CPV*n].y;
        V[i].z= obj -> CtlPts()[i+CPV*n].z;
      } //eof
      GlobalInterpCurve(V,CPV,obj -> Order(1)-1,obj -> KnotVector(1),UK);

      // Store New Control Points
      for (i= 0; i < CPV; i++) {
        obj -> CtlPts()[i+CPV*n].x= V[i].x;
        obj -> CtlPts()[i+CPV*n].y= V[i].y;
        obj -> CtlPts()[i+CPV*n].z= V[i].z;
      } //eof
    } //eof
  } //fi

  // Reset Tesselation
  obj -> Tess(0,TessUV[0]);
  obj -> Tess(1,TessUV[1]);

  // Change Knot Type to Open if formerly Periodic
  obj -> KnotType(0,NURBS_KV_Open);
  obj -> KnotType(1,NURBS_KV_Open);

  free(U);
  free(V);
} //eof NURBS_Fitting::GlobalInterp()



void NURBS_Fitting::Solve(double *N, double *D, double *P, int L) {
  int		i,j;
  double	*K,*C;

  K= (double*)malloc(sizeof(double)*L*L);
  C= (double*)malloc(sizeof(double)*L);

  for (i= 0; i < L*L; i++) K[i]= N[i];

  NurbanaMath::InvertMatrix(K,L);
  // Construct a (Lx1) matrix for each dimension of D and multiply
  // against N0, and store each resultant column in P.
  for (i= 0; i < 3; i++) {
    for (j= 0; j < L; j++)
      C[j]= D[j*3+i];
    NurbanaMath::MultMatrix(K,L,L,C,L,1,C);
    for (j= 0; j < L; j++)
      P[j*3+i]= C[j];
  } //eof

  free(K);
  free(C);
} //eof NURBS_Fitting::Solve()


void NURBS_Fitting::GlobalInterpCurve(Point3d *Q, int CP, int Deg, double *U, int UK) {
  double	*N,*D,*P,*V,*uk;
  int		i,j,span;

  // Allocate memory for Coefficient Matrix
  N= (double*)malloc(sizeof(double)*CP*CP);
  D= (double*)malloc(sizeof(double)*CP*3);
  P= (double*)malloc(sizeof(double)*CP*3);
  V= (double*)malloc(sizeof(double)*(Deg+1));
  uk= (double*)malloc(sizeof(double)*CP);
//  for (i= 0; i < CP; i++)
//    printf("Q[%d]: [%.2f][%.2f][%.2f]\n",i,Q[i].x,Q[i].y,Q[i].z);

  // Compute uk by chord length method
  switch( UK ) {
    case 0:
      if (!NURBS_Knot::Chord(Deg,CP,Q,uk))
        return;
      break;

    case 1:
      if (!NURBS_Knot::Centripetal(Deg,CP,Q,uk))
        return;
      break;

    case 2:
      if (!NURBS_Knot::Universal(Deg,CP,Q,uk))
        return;
    default:
      return;
  } //eos

//  printf("\n");
//  for (i= 0; i < CP; i++)
//    printf("[%.3f]",uk[i]);
  // Compute U through averaging
  NURBS_Knot::Average(Deg,CP,uk,U);
//  printf("\n");
//  for (i= 0; i < CP+(Deg+1); i++)
//    printf("[%.3f]",U[i]);

  // Initialize N
  for (i= 0; i < CP*CP; i++)
    N[i]= 0;


  for (i= 0; i < CP; i++) {
    // Setup Coefficient Matrix
    span= NURBS_Knot::FindSpan(CP,Deg,uk[i],U);
    NURBS_Knot::BasisFuns(span,uk[i],Deg,U,V);

    // Copy N into ith Row
    for (j= 0; j <= Deg; j++)
      N[i*CP+(span-Deg+j)]= V[j];
  } //eof
//  N[0]= N[CPU*CPU-1]= 1;
//  if (!N[0]) NurbanaMath::IdentityMatrix(N,CP);
/*
printf("---------------------------\n");
  for (i= 0; i < CP; i++) {
    for (j= 0; j < CP; j++) {
      printf("[%.3f]",N[i*CP+j]);
    } //eof
    printf("\n");
  } //eof
*/

  // Copy points into D
  for (i= 0; i < CP; i++) {
    D[i*3+0]= Q[i].x;
    D[i*3+1]= Q[i].y;
    D[i*3+2]= Q[i].z;
  } //eof

  Solve(N,D,P,CP);

  // Store New Control Points
  for (i= 0; i < CP; i++) {
    Q[i].x= P[i*3+0];
    Q[i].y= P[i*3+1];
    Q[i].z= P[i*3+2];
  } //eof

  free(N);
  free(D);
  free(P);
  free(V);
  free(uk);
} //eof NURBS_Fitting::GlobalInterpCurve()