#include "NURBS.h"
#include <stdio.h>


void _BasisUniform(int Order, float t, int npts, float KnotVector[], float H[], float BasisValues[], bool Type) {
  int	i,k;
  float	temp[100],sum,d,e;

  for (i= 0; i < npts+Order; i++) {
    if (( t >= KnotVector[i]) && (t < KnotVector[i+1])) {
      temp[i]= 1;
    } else {
      temp[i]= 0;
    } //fi
  } //eof


  for (k= 1; k < Order; k++) {
    for (i= 0; i <= (npts+Order)-k; i++) {
      if (temp[i] != 0) {
        d= ((t-KnotVector[i])*temp[i])/(KnotVector[i+k]-KnotVector[i]);
      } else {
        d= 0;
      } //fi

      if (temp[i+1] != 0) {
        e= ((KnotVector[i+k+1]-t)*temp[i+1])/(KnotVector[i+k+1]-KnotVector[i+1]);
      } else {
        e= 0;
      } //fi

      temp[i]= d+e;
    } //eof
  } //eof


  if (t == KnotVector[npts+Order]) temp[npts]= 1;

  if (Type == 0) {
    sum= 0;
    for (i= 0; i <= npts; i++) sum+= temp[i]*H[i];

    for (i= 0; i <= npts; i++) {
      if (sum != 0) {
        BasisValues[i]= (temp[i]*H[i])/sum;
      } else {
        BasisValues[i]= 0;
      } //fi
    } //eof
  } else {
    for (i= 0; i <= npts; i++) {
      BasisValues[i]= temp[i];
    } //eof
  } //fi
} //eof _BasisUniform()


void NURBS::GetNURBSCurve(int npts, float b[][3], int cpts, float p[][3], float H[], int Type, float KnotVectorU[], int Order) {
  float	BasisValues[100];
  int	i,n,j;

  cpts--;

  for (i= 0; i <= cpts; i++) {
    //Calculate the Rational B-Spline basis function
    if (Type == NURBS_Periodic) {
      //Check this
      _BasisUniform(Order, (Order-1)+i*float(npts-(Order-1))/float(cpts), npts-1, KnotVectorU, H, BasisValues, 0);
    } else if (Type == NURBS_Open) {
      _BasisUniform(Order, i*float(KnotVectorU[npts-1+Order])/float(cpts), npts-1, KnotVectorU, H, BasisValues, 0);
    } //fi

    p[i][0]= p[i][1]= p[i][2]= 0;

    for (n= 0; n < npts; n++) {
      p[i][0]+= b[n][0]*BasisValues[n];
      p[i][1]+= b[n][1]*BasisValues[n];
      p[i][2]+= b[n][2]*BasisValues[n];
    } //eof
  } //eof
} //eof NURBS::GetNURBSCurve()



float _getsum(float H[], float BasisU[], float BasisV[], int Unpts, int Vnpts) {
  int	i,n,j;
  float	sum;

  sum= j= 0;
  for (i= 0; i < Unpts; i++) {
    for (n= 0; n < Vnpts; n++) {
      sum+= H[j]*BasisU[i]*BasisV[n];
      j++;
    } //eof
  } //eof
  return(sum);
} //eof _getsum


void NURBS::GetNURBSSurface(int Unpts, int Vnpts, float b[][3], int Ucpts, int Vcpts, float p[][3], float H[], int KnotTypeU, int KnotTypeV, float KnotVectorU[], float KnotVectorV[], int OrderU, int OrderV) {
  int	i,n,k,l,m,j;
  float BasisU[128][128];
  float BasisV[128][128];
  float	sum,BasisResult;

  Ucpts--;
  Vcpts--;


  for (i= 0; i <= Ucpts; i++) {
    if (KnotTypeU == NURBS_Periodic) {
      _BasisUniform(OrderU, (OrderU-1)+i*float(Unpts-(OrderU-1))/float(Ucpts), Unpts-1, KnotVectorU, H, BasisU[i], 1);
    } else if (KnotTypeU == NURBS_Open) {
      _BasisUniform(OrderU, i*float(KnotVectorU[Unpts-1+OrderU])/float(Ucpts), Unpts-1, KnotVectorU, H, BasisU[i], 1);
    } //fi
  } //eof

  for (i= 0; i <= Vcpts; i++) {
    if (KnotTypeV == NURBS_Periodic) {
      _BasisUniform(OrderV, (OrderV-1)+i*float(Vnpts-(OrderV-1))/float(Vcpts), Vnpts-1, KnotVectorV, H, BasisV[i], 1);
    } else if (KnotTypeV == NURBS_Open) {
      _BasisUniform(OrderV, i*float(KnotVectorV[Vnpts-1+OrderV])/float(Vcpts), Vnpts-1, KnotVectorV, H, BasisV[i], 1);
    } //fi
  } //eof


  m= 0;
  for (i= 0; i <= Ucpts; i++) {
    for (n= 0; n <= Vcpts; n++) {
      sum= _getsum(H,BasisU[i],BasisV[n],Unpts,Vnpts);

      p[m][0]= p[m][1]= p[m][2]= 0;

      j= 0;
      for (k= 0; k < Unpts; k++) {
        for (l= 0; l < Vnpts; l++) {
          BasisResult= H[j]*BasisU[i][k]*BasisV[n][l]/sum;

          p[m][0]+= b[j][0]*BasisResult;
          p[m][1]+= b[j][1]*BasisResult;
          p[m][2]+= b[j][2]*BasisResult;

          j++;
        } //eof
      } //eof

      m++;
    } //eof
  } //eof
} //eof NURBS::GetNURSSurface()



float _Oslo(float KnotVectorX[], float KnotVectorY[], int K, int J, int I) {
  float	NA,DA,NB,DB,RA,RB;
  //K ranges from Order-1 to 0 
  //J ranges from 0 to (tot pts - 1)
  //I ranges from 0 ...

  if (K == 1) {
    if ((KnotVectorY[J] >= KnotVectorX[I]) && (KnotVectorY[J] < KnotVectorX[I+1])) {
      return(1);
    } else {
      return(0);
    } //fi
  } else {
    NA= (KnotVectorY[J+K-1] - KnotVectorX[I]);
    DA= (KnotVectorX[I+K-1] - KnotVectorX[I]);
    NB= (KnotVectorX[I+K] - KnotVectorY[J+K-1]);
    DB= (KnotVectorX[I+K] - KnotVectorX[I+1]);

    if (DA) RA= (NA/DA); else RA= 0;
    if (DB) RB= (NB/DB); else RB= 0;
    return( RA*_Oslo(KnotVectorX,KnotVectorY,K-1,J,I) + RB*_Oslo(KnotVectorX,KnotVectorY,K-1,J,I+1) );
  } //fi
} //eof _Oslo()


void NURBS::SubdivideCurve(int npts, float b[][3], float p[][3], float KnotVectorX[], float H[], int Order, int *newNpts) {
  float	KnotVectorY[100];
  float NewPoints[100][3];
  int	i,n,j;

  //Calculate New Knot Vector
  j= 0;
  for (i= 0; i < npts+Order-1; i++) {
    if (KnotVectorX[i+1]-KnotVectorX[i]) {
      KnotVectorY[i+j]= KnotVectorX[i];
      j++;
      KnotVectorY[i+j]= (KnotVectorX[i]+KnotVectorX[i+1])/2.0;
    } else {
      KnotVectorY[i+j]= KnotVectorX[i];
    } //fi
  } //eof
  KnotVectorY[i+j]= KnotVectorX[i];


//  printf("Point Total: %d\n",npts+j);

//  printf("Knot Vector X: ");
//  for (i= 0; i < npts+Order; i++) printf("[%f] ",KnotVectorX[i]);
//  printf("\n");

//  printf("Knot Vector Y: ");
//  for (i= 0; i < npts+Order+j; i++) printf("[%f] ",KnotVectorY[i]);
//  printf("\n");

//  printf("Oslo:-----\n");
//  for (n= 0; n < npts+j; n++) {
//    printf("J[%d]: ",n);
//    for (i= 0; i < npts; i++) {
//      printf("i[%d]:%f ",i,_Oslo(KnotVectorX,KnotVectorY,3,n,i));
//      _Oslo(KnotVectorX,KnotVectorY,2,n,i);
//    } //eof
//    printf("\n");
//  } //eof


  //Assign the new Control Point Locations

  //Calculate New Control Points through Recrusive Oslo Algorithm
  for (i= 0; i < npts+j; i++) {
    NewPoints[i][0]= NewPoints[i][1]= NewPoints[i][2]= 0;
    for (n= 0; n < npts; n++) {
//      printf("pts: %f,%f,%f\n",b[n][0],b[n][1],b[n][2]);
//      printf("Oslo: %f\n",_Oslo(KnotVectorX,KnotVectorY,Order,i,n));
      NewPoints[i][0]+= _Oslo(KnotVectorX,KnotVectorY,Order,i,n)*b[n][0];
      NewPoints[i][1]+= _Oslo(KnotVectorX,KnotVectorY,Order,i,n)*b[n][1];
      NewPoints[i][2]+= _Oslo(KnotVectorX,KnotVectorY,Order,i,n)*b[n][2];
    } //eof
  } //eof


  //Copy the New Points back into b[][3]
  for (i= 0; i < npts+j; i++) {
    b[i][0]= NewPoints[i][0];
    b[i][1]= NewPoints[i][1];
    b[i][2]= NewPoints[i][2];
  } //eof

  *newNpts= npts+j;
//  return(npts+j);
} //eof NURBS::SubdivideCurve()



void NURBS::SubdivideSurface(int Unpts, int Vnpts, float b[][3], float p[][3], float KnotVectorU[], float KnotVectorV[], float H[], int OrderU, int OrderV, int *newNptsU, int *newNptsV) {
  float	KnotVectorY[100];
  float NewPoints[100][100][3];
  int	i,n,k,j;

  //---------- IN THE U DIRECTION ----------
  //Calculate New Knot Vector
  j= 0;
  for (i= 0; i < Unpts+OrderU-1; i++) {
    if (KnotVectorU[i+1]-KnotVectorU[i]) {
      KnotVectorY[i+j]= KnotVectorU[i];
      j++;
      KnotVectorY[i+j]= (KnotVectorU[i]+KnotVectorU[i+1])/2.0;
    } else {
      KnotVectorY[i+j]= KnotVectorU[i];
    } //fi
  } //eof
  KnotVectorY[i+j]= KnotVectorU[i];


  //Assign the new Control Point Locations
  //Calculate New Control Points through Recrusive Oslo Algorithm
  for (k= 0; k < Vnpts; k++) {
    for (i= 0; i < Unpts+j; i++) {
      NewPoints[k][i][0]= NewPoints[k][i][1]= NewPoints[k][i][2]= 0;
      for (n= 0; n < Unpts; n++) {
        NewPoints[k][i][0]+= _Oslo(KnotVectorU,KnotVectorY,OrderU,i,n)*b[n*Vnpts+k][0];
        NewPoints[k][i][1]+= _Oslo(KnotVectorU,KnotVectorY,OrderU,i,n)*b[n*Vnpts+k][1];
        NewPoints[k][i][2]+= _Oslo(KnotVectorU,KnotVectorY,OrderU,i,n)*b[n*Vnpts+k][2];
      } //eof
    } //eof
  } //eof


  //Copy the New Points back into b[][3]
  for (k= 0; k < Vnpts; k++) {
    for (i= 0; i < Unpts+j; i++) {
      b[i*Vnpts+k][0]= NewPoints[k][i][0];
      b[i*Vnpts+k][1]= NewPoints[k][i][1];
      b[i*Vnpts+k][2]= NewPoints[k][i][2];
    } //eof
  } //eof

  *newNptsU= Unpts+j;

  //-------------------------
  //Don't forget to Update U!
  Unpts+= j;
  //-------------------------


  //---------- IN THE V DIRECTION ----------
  //Calculate New Knot Vector
  j= 0;
  for (i= 0; i < Vnpts+OrderV-1; i++) {
    if (KnotVectorV[i+1]-KnotVectorV[i]) {
      KnotVectorY[i+j]= KnotVectorV[i];
      j++;
      KnotVectorY[i+j]= (KnotVectorV[i]+KnotVectorV[i+1])/2.0;
    } else {
      KnotVectorY[i+j]= KnotVectorV[i];
    } //fi
  } //eof
  KnotVectorY[i+j]= KnotVectorV[i];


  //Assign the new Control Point Locations
  //Calculate New Control Points through Recrusive Oslo Algorithm
  for (k= 0; k < Unpts; k++) {
    for (i= 0; i < Vnpts+j; i++) {
      NewPoints[k][i][0]= NewPoints[k][i][1]= NewPoints[k][i][2]= 0;
      for (n= 0; n < Vnpts; n++) {
        NewPoints[k][i][0]+= _Oslo(KnotVectorV,KnotVectorY,OrderV,i,n)*b[n+k*Vnpts][0];
        NewPoints[k][i][1]+= _Oslo(KnotVectorV,KnotVectorY,OrderV,i,n)*b[n+k*Vnpts][1];
        NewPoints[k][i][2]+= _Oslo(KnotVectorV,KnotVectorY,OrderV,i,n)*b[n+k*Vnpts][2];
      } //eof
    } //eof
  } //eof


  //Copy the New Points back into b[][3]
  for (k= 0; k < Unpts; k++) {
    for (i= 0; i < Vnpts+j; i++) {
      b[i+k*(Vnpts+j)][0]= NewPoints[k][i][0];
      b[i+k*(Vnpts+j)][1]= NewPoints[k][i][1];
      b[i+k*(Vnpts+j)][2]= NewPoints[k][i][2];
    } //eof
  } //eof

  *newNptsV= Vnpts+j;
} //eof NURBS::SubdivideSurface()


void NURBS::MakeCyclic(int *Unpts, int *Vnpts, float b[][3], float H[], int OrderU, int OrderV, bool CyclicU, bool CyclicV) {
  int	i,n,k,l;
  float	temp[1024][3],Htemp[1024];

  if (CyclicU || CyclicV) {
    k= 0;

    //Cyclic V (vertical)
    if (CyclicV) {
      for (i= 0; i < Unpts; i++) {
        for (n= 0; n < Vnpts; n++) {
          temp[k][0]= b[i*Vnpts+n][0];
          temp[k][1]= b[i*Vnpts+n][1];
          temp[k][2]= b[i*Vnpts+n][2];
          Htemp[k]= H[i*Vnpts+n];
          k++;
        } //eof

        for (l= 0; l < OrderV-1; l++) {
          temp[k][0]= b[l+i*Vnpts][0];
          temp[k][1]= b[l+i*Vnpts][1];
          temp[k][2]= b[l+i*Vnpts][2];
          Htemp[k]= H[l+i*Vnpts];
          k++;
        } //eof
      } //eof
      Vnpts+= (OrderV-1);
    } else {
      for (i= 0; i < Unpts * Vnpts; i++) {
        temp[i][0]= b[i][0];
        temp[i][1]= b[i][1];
        temp[i][2]= b[i][2];
        Htemp[i]= H[i];
        k++;
      } //eof
    } //fi

    if (CyclicU) {
      //Cyclic U (horizontal)
      for (i= 0; i < OrderU-1; i++) {
        for (n= 0; n < Vnpts; n++) {
          temp[k][0]= temp[n+i*Vnpts][0];
          temp[k][1]= temp[n+i*Vnpts][1];
          temp[k][2]= temp[n+i*Vnpts][2];
          Htemp[k]= Htemp[n+i*Vnpts];
          k++;
        } //eof
      } //eof
      Unpts+= (OrderU-1);
    } //fi
  } //fi

  for (i= 0; i < Unpts * Vnpts; i++) {
    b[i][0]= temp[i][0];
    b[i][1]= temp[i][1];
    b[i][2]= temp[i][2];
    H[i]= Htemp[i];
  } //eof
} //eof Make::MakeCyclic()
