#include "NurbanaMath.h"


// Degrees to Radians
double NurbanaMath::DtoR(double deg) { return(deg * 0.01745329252); } //eof NurbanaMath::DtoR()


// Radians to Degrees
double NurbanaMath::RtoD(double rad) { return(rad * 57.2957795131); } //eof NurbanaMath::RtoD()


int NurbanaMath::Max(int a, int b) {
  return( a > b ? a : b );
} //eof NurbanaMath::Max()


int NurbanaMath::Min(int a, int b) {
  return( a < b ? a : b );
} //eof NurbanaMath::Min()


// Factorial of N (Recursive)
int NurbanaMath::Factorial(int n) {
  if (n <= 1)
    return(1);
  return(n*NurbanaMath::Factorial(n-1));
} //eof NurbanaMath::Factorial()


double NurbanaMath::Comb(double n, double r) {
  return( r ? Comb(n-1,r-1)*(n/r) : 1 );
} //eof NurbanaMath::Combination()


//Calculate the Normal of a 3pt triangle face
void NurbanaMath::Normal(Point3d Pts[3], Point3d &Normal) {
  Point3d VectorAB, VectorAC;

  VectorAB.x= Pts[0].x - Pts[1].x;
  VectorAB.y= Pts[0].y - Pts[1].y;
  VectorAB.z= Pts[0].z - Pts[1].z;

  VectorAC.x= Pts[0].x - Pts[2].x;
  VectorAC.y= Pts[0].y - Pts[2].y;
  VectorAC.z= Pts[0].z - Pts[2].z;

  Normal.x= VectorAB.y*VectorAC.z - VectorAB.z*VectorAC.y;
  Normal.y= VectorAB.z*VectorAC.x - VectorAB.x*VectorAC.z;
  Normal.z= VectorAB.x*VectorAC.y - VectorAB.y*VectorAC.x;
} //eof NurbanaMath::Normal()


bool NurbanaMath::Normalize(Point3d &Vector) {
  double	L;

  L= sqrt(Vector.x*Vector.x + Vector.y*Vector.y + Vector.z*Vector.z);
  if (L < 0.000001) return(0);
  Vector.x/= L;
  Vector.y/= L;
  Vector.z/= L;
  return(1);
} //eof NurbanaMath::Normalize()


float NurbanaMath::GetSlope(float y2, float y1, float x2, float x1) {
  return(x2-x1) ? (y2-y1)/(x2-x1) : 0;
} //eof GetSlope()


double NurbanaMath::Sq(double V) {
  return(V*V);
} //eof NurbanaMath::Sq()


void NurbanaMath::Swap(double &A, double &B) {
  double	T;

  T= A;
  A= B;
  B= T;
} //eof NurbanaMath::Swap()


double NurbanaMath::AngleSwept(float AX, float AY, float BX, float BY) {
  // theta= arccos((AdotB)/(|A||B|)) in radians
  // Assume Z component of vector to be 0
  // Extended to return 0 - 360 result instead of 0 - 180
  double	AdotB,absX,absY,theta;
  Point3d	VPA,VPB;

  // Determine side pt is on by comparing with two virtual
  // points, A rotated 90 and B rotated -90
  VPA.x= VPB.x= AX;
  VPA.y= VPB.y= AY;
  VPA.z= VPB.z= 0;

  VertexOps::RotateVertex(0,0,90,VPA);
  VertexOps::RotateVertex(0,0,-90,VPB);

  AdotB= (AX*BX) + (AY*BY);
  absX= sqrt(AX*AX+AY*AY);
  absY= sqrt(BX*BX+BY*BY);
  theta= acos(AdotB/(absX*absY));

  // Determine if pt B is closer to A or B
  VPA.x= BX-VPA.x;
  VPA.y= BY-VPA.y;
  VPB.x= BX-VPB.x;
  VPB.y= BY-VPB.y;

  theta= RtoD(theta);
  if (sqrt(VPA.x*VPA.x+VPA.y*VPA.y) > sqrt(VPB.x*VPB.x+VPB.y*VPB.y))
    theta= 360.0-theta;

  return(theta);
} //eof AngleSwept()


double NurbanaMath::Dist3d(Point3d P1, Point3d P2) {
  return( sqrt((P2.x-P1.x)*(P2.x-P1.x)+(P2.y-P1.y)*(P2.y-P1.y)+(P2.z-P1.z)*(P2.z-P1.z)) );
} //eof Dist3d()

double NurbanaMath::Dist4d(Point3d P1, Point3d P2) {
  return( sqrt((P2.x-P1.x)*(P2.x-P1.x)+(P2.y-P1.y)*(P2.y-P1.y)+(P2.z-P1.z)*(P2.z-P1.z)) );
} //eof Dist4d()



// Structure of Matrix
// 0 1 2 3
// 4 5 6 7
// 8 9 . .

void NurbanaMath::IdentityMatrix(double *M, int S) {
  int	i,j;

  for (i= 0; i < S; i++)
    for (j= 0; j < S; j++)
      M[i*S+j]= (i == j);
} //eof NurbanaMath::IdentityMatrix()


void NurbanaMath::InvertMatrix(double *M, int S) {
  int		i,j;
  double	*A,*I;

  I= (double*)malloc(sizeof(double)*S*S);
  A= (double*)malloc(sizeof(double)*2*S*S);

  // Augment Matrix M with an identity Matrix into A
  for (i= 0; i < S; i++) {
    for (j= 0; j < S; j++)
      A[i*2*S+j]= M[i*S+j];
    for (j= 0; j < S; j++)
      A[i*2*S+j+S]= (i==j);
  } //eof

  // Invert Matrix A
  for (i= 0; i < S; i++) {
    IdentityMatrix(I,S);
    for (j= 0; j < S; j++)
      I[j*S+i]= -A[j*2*S+i]/A[i*2*S+i];
    I[i*S+i]= 1.0/A[i*2*S+i];
    MultMatrix(I,S,S,A,S,2*S,A);
  } //eof

  // Extra Invertex Matrix from Augmented Section of A and place into M
  for (i= 0; i < S; i++) {
    for (j= 0; j < S; j++) {
      M[i*S+j]= A[i*2*S+j+S];
    } //eof
  } //eof

  free(I);
  free(A);
} //eof NurbanaMath::InvertMatrix()


void NurbanaMath::MultMatrix(double *A, int Ar, int Ac, double *B, int Br, int Bc, double *C) {
  int		i,j,k;
  double	*M;

  if (Ac == Br) {
    M= (double*)malloc(sizeof(double)*Ar*Bc);
    for (i= 0; i < Bc; i++)
      for (j= 0; j < Ar; j++) {
        M[j*Bc+i]= 0;
        for (k= 0; k < Br; k++)
          M[j*Bc+i]+= A[j*Ac+k]*B[k*Bc+i];
      } //eof
    memcpy(C,M,sizeof(double)*Ar*Bc);
    free(M);
  } //fi
} //eof NurbanaMath::MultMatrix()
