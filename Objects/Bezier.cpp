#include "Bezier.h"


// Function to calculate the factorial function for Bernstein basis
double Ni(int n,int i) {
  return( NurbanaMath::Factorial(n)/(NurbanaMath::Factorial(i)*NurbanaMath::Factorial(n-i)) );
} //eof Ni


// Calculates Berstein Basis Function
double Basis(int n, int i, double t) {
  double ti; // t^i
  double tni; // (1 - t)^i

  // handle the special cases to avoid domain problem with pow
  if ((t == 0) && (i == 0)) ti= 1.0; else ti= pow(t,i);

  if ((n == i) && (t == 1)) tni= 1.0; else tni= pow((1-t),(n-i));
  return( Ni(n,i)*ti*tni ); // calculate Bernstein basis function
} //eof Basis()


void Bezier::GetBezierCurve(int npts, Point3d *b, int cpts, Point3d *p) {
  int		i, n;
  double	BasisResult;

  cpts--; //We want 0 -> CPTS, so we need to decrement by 1 and use <= to get 0 -> 1 for t
  for (i= 0; i <= cpts; i++) {
    p[i].x= p[i].y= p[i].z= 0;
    for (n= 0; n < npts; n++) {
      BasisResult= Basis(npts-1, n, (double)i/double(cpts));
      p[i].x+= b[n].x*BasisResult;
      p[i].y+= b[n].y*BasisResult;
      p[i].z+= b[n].z*BasisResult;
    } //eof
  } //eof
} //eof Bezier::GetBezierCurve()


void Bezier::GetBezierSurface(int Unpts, int Vnpts, Point3d *b, int Ucpts, int Vcpts, Point3d *p) {
  int i, n, k, l, m, j;
  double BasisA[128][128];
  double BasisB[128][128];
  double BasisResult;

  Ucpts--; //We want 0 -> CPTS, so we need to decrement by 1 and use <= to get 0 -> 1 for t
  Vcpts--; //We want 0 -> CPTS, so we need to decrement by 1 and use <= to get 0 -> 1 for t


  // This is done for efficency purposes
  // For Example, for a Tess of 25x25 and a net of 4x4, Basis A would've been calculuated
  // 25x25x4 times or 2,500.  BasisB would've been calculated 25x25x4x4 times or 10,000 times
  // for a total of 12,500 times, with this it would only have to be calculated 200 times
  // thus having to only calculate only 1.6% the original # of basis functions

  for (i= 0; i <= Ucpts; i++) {
    for (k= 0; k < Unpts; k++) {
      BasisA[i][k]= Basis(Unpts-1, k, (double)i/double(Ucpts));
    } //eof
  } //eof

  for (n= 0; n <= Vcpts; n++) {
    for (l= 0; l < Vnpts; l++) {
      BasisB[n][l]= Basis(Vnpts-1, l, (double)n/double(Vcpts));
    } //eof
  } //eof


  m= 0;
  for (i= 0; i <= Ucpts; i++) {
    for (n= 0; n <= Vcpts; n++) {

      p[m].x= p[m].y= p[m].z= 0;

      j= 0;
      for (k= 0; k < Unpts; k++) {
        for (l= 0; l < Vnpts; l++) {
          BasisResult= BasisA[i][k] * BasisB[n][l];
          p[m].x+= b[j].x * BasisResult;
          p[m].y+= b[j].y * BasisResult;
          p[m].z+= b[j].z * BasisResult;

//          p[m][0]+= b[k*Vnpts+l][0] * BasisResult;
//          p[m][1]+= b[k*Vnpts+l][1] * BasisResult;
//          p[m][2]+= b[k*Vnpts+l][2] * BasisResult;
          j++;
        } //eof
      } //eof

      m++;
    } //eof
  } //eof

} //eof Bezier::GetBezierSurface()
