#include "NURBS_Knot.h"


void NURBS_Knot::Average(int deg, int n, double *uk, double *U) {
  int	i,j;

  for (j= 1; j < n-deg; j++) {
    U[j+deg]= 0;
    for (i= j; i < j+deg; i++)
      U[j+deg]+= uk[i];
    U[j+deg]/= double(deg);
  } //eof

  for (i= 0; i <= deg; i++)
    U[i]= 0;
  for (i= n; i < n+deg+1; i++)
    U[i]= 1;
} //eof Average();


void NURBS_Knot::BasisFuns(int i, double u, int p, double *U, double *N) {
  // Compute the non-vanishing basis functions
  int		j,r;
  double	*left,*right,saved,temp;

  left= (double*)malloc(sizeof(double)*(p+1));
  right= (double*)malloc(sizeof(double)*(p+1));

  N[0]= 1.0;

  for (j= 1; j <= p; j++) {
    left[j]= u-U[i+1-j];
    right[j]= U[i+j]-u;
    saved= 0;
    for (r= 0; r < j; r++) {
      right[r+1]+left[j-r] ? temp= N[r]/(right[r+1]+left[j-r]) : temp= 1;
      N[r]= saved+right[r+1]*temp;
      saved= left[j-r]*temp;
//      printf("saved: %f= %f * %f\n",saved,left[j-r],temp);
    } //eof
    N[j]= saved;
  } //eof

  free(left);
  free(right);
} //eof NURBS_Knot::BasisFuns()


bool NURBS_Knot::Centripetal(int deg, int n, Point3d *Q, double *uk) {
  int		i;
  double	d;

  uk[0]= 0;
  d= 0;
  for (i= 1; i < n; i++)
    d+= sqrt(NurbanaMath::Dist3d(Q[i],Q[i-1]));
  uk[n-1]= 1;

  if (d < 0.0001) return(0);

  for (i= 1; i < n - 1; i++)
    uk[i]= uk[i-1] + sqrt(NurbanaMath::Dist3d(Q[i],Q[i-1]))/d;

  return(1);
} //eof NURBS_Knot::Centripetal()


bool NURBS_Knot::Chord(int deg, int n, Point3d *Q, double *uk) {
  int		i;
  double	T;

  T= 0;
  uk[0]= 0;
  for (i= 1; i < n; i++) {
    T+= NurbanaMath::Dist3d(Q[i],Q[i-1]);
    uk[i]= uk[i-1] + NurbanaMath::Dist3d(Q[i],Q[i-1]);
  } //eof
  uk[n-1]= 1;

  if (T < 0.0001) return(0);

  for (i= 1; i < n-1; i++)
    uk[i]/= T;

  return(1);
} //eof NURBS_Knot::Chord()


bool NURBS_Knot::Universal(int deg, int n, Point3d *Q, double *uk) {
  int		i;

  uk[0]= 0;
  for (i= 1; i < n - 1; i++)
    uk[i]= double(i)/double(n);
  uk[n-1]= 1;

  return(1);
} //eof NURBS_Knot::Universal()


int NURBS_Knot::FindSpan(int Unpts, int p, double u, double *KnotVector) {
  int	low,mid,high;

  if (u >= KnotVector[Unpts]) return(Unpts-1);
  if (u <= KnotVector[p]) return(p);

  low= p;
  high= Unpts+1;
  mid= (low+high)/2;

  while (u < KnotVector[mid] || u >= KnotVector[mid+1]) {
//    printf("%f < %f || %f >= %f\n",u,KnotVector[mid],u,KnotVector[mid+1]);
    if (u < KnotVector[mid]) {
      high= mid;
    } else {
      low= mid;
    } //fi
    mid= (low+high)/2;
  } //eow

  return(mid);
} //eof NURBS_Knot::FindSpan()



void NURBS_Knot::Difference(double *KV1, int L1, double *KV2, int L2, double *KV3, int &L3) {
  // Given two Knot Vectors on same domain, store difference in KV3
  int	KV1i,KV2i;

  KV1i= KV2i= L3= 0;

  while (KV2i < L2) {
    if (KV1[KV1i] == KV2[KV2i]) {
      KV1i++;
      KV2i++;
    } else {
      KV3[L3]= KV2[KV2i++];
      L3++;
    } //fi
  } //eow
} //eof NURBS_Knot::Difference()


void NURBS_Knot::Scale(double *KV, int r, double Max) {
  int	i;

  for (i= 0; i < r; i++)
    KV[i]*= Max/KV[r-1];
} //eof NURBS_Knot::Scale()



void NURBS_Knot::MergeKnotVectors(int Ulen, double *U, int Vlen, double *V, int &Ki, double *KV) {
  // Merges 2 Knot Vectors
  int	Ui,Vi;

  Ui= Vi= Ki= 0;

  while (Ui < Ulen || Vi < Vlen) {
    if (U[Ui] == V[Vi] && Ui < Ulen && Vi < Vlen) {
      KV[Ki]= U[Ui++];
      Vi++;
    } else if ((U[Ui] < V[Vi] && Ui < Ulen) || Vi >= Vlen) {
      KV[Ki]= U[Ui++];
    } else if ((V[Vi] < U[Ui] && Vi < Vlen) || Ui >= Ulen) {
      KV[Ki]= V[Vi++];
    } //fi
    Ki++;
  } //eow
} //eof NURBS_Knot::MergeKnotVectors()



void NURBS_Knot::Refine(Object_NURBS *obj, double *X, int r, bool UV) {
  int		a,b,i,j,k,l,m,n,ind,degree;
  Point3d	*Qw,*Curve;
  double	*Ubar,alfa;

  // Allocate Memory for Qw
  Qw= (Point3d*)malloc(sizeof(Point3d)*(obj -> Length(UV) + r + 1));
  Curve= (Point3d*)malloc(sizeof(Point3d)*(obj -> Length(UV) + r + 1));
  Ubar= (double*)malloc(sizeof(double)*(obj -> Length(UV) + obj -> Order(UV) + r + 1));


  // Allocate memory for curve
  if (UV) {
    obj -> SetLength(obj -> Length(0),obj -> Length(1)+r);
    n= obj -> Length(0) * (obj -> Length(1) + r) - r - 1;
    for (i= obj -> Length(0) * obj -> Length(1) - 1; i >= 0; i--) {
//      printf("n,i: %d,%d\n",n,i);
      obj -> CtlPts()[n].x= obj -> CtlPts()[i].x;
      obj -> CtlPts()[n].y= obj -> CtlPts()[i].y;
      obj -> CtlPts()[n].z= obj -> CtlPts()[i].z;
      !(i % obj -> Length(1)) ? n-= r+1: n--;
    } //eof
  } else {
    obj -> SetLength(obj -> Length(0)+r,obj -> Length(1));
  } //fi

  // Decrement r
  r--;

  for (n= 0; n < obj -> Length(!UV); n++) {
    // Populate Curve
    if (UV) {
      for (i= 0; i < obj -> Length(1); i++) {
        Curve[i].x= obj -> CtlPts()[i + n*(obj -> Length(1) + r + 1)].x;
        Curve[i].y= obj -> CtlPts()[i + n*(obj -> Length(1) + r + 1)].y;
        Curve[i].z= obj -> CtlPts()[i + n*(obj -> Length(1) + r + 1)].z;
      } //eof
    } else {
      for (i= 0; i < obj -> Length(0); i++) {
        Curve[i].x= obj -> CtlPts()[i*obj -> Length(1) + n].x;
        Curve[i].y= obj -> CtlPts()[i*obj -> Length(1) + n].y;
        Curve[i].z= obj -> CtlPts()[i*obj -> Length(1) + n].z;
      } //eof
    } //fi


    m= obj -> Length(UV) + obj -> Order(UV);
    degree= obj -> Order(UV) - 1;

    a= FindSpan(obj -> Length(UV),degree,X[0],obj -> KnotVector(UV));
    b= FindSpan(obj -> Length(UV),degree,X[r],obj -> KnotVector(UV));
    b++;

    // Populate QW with Curve
    for (i= 0; i <= a-degree; i++) {
      Qw[i].x= Curve[i].x;
      Qw[i].y= Curve[i].y;
      Qw[i].z= Curve[i].z;
    } //eof

    for (i= b-1; i <= obj -> Length(UV); i++) {
      Qw[i+r+1].x= Curve[i].x;
      Qw[i+r+1].y= Curve[i].y;
      Qw[i+r+1].z= Curve[i].z;
    } //eof

    for (i= 0; i <= a; i++)
      Ubar[i]= obj -> KnotVector(UV)[i];

    for (i= b+degree; i <= m; i++)
      Ubar[i+r+1]= obj -> KnotVector(UV)[i];

    i= b+degree-1;
    k= b+degree+r;

    for (j= r; j >= 0; j--) {
      while (X[j] <= obj -> KnotVector(UV)[i] && i > a) {
        Qw[k-degree-1].x= Curve[i-degree-1].x;
        Qw[k-degree-1].y= Curve[i-degree-1].y;
        Qw[k-degree-1].z= Curve[i-degree-1].z;

        Ubar[k]= obj -> KnotVector(UV)[i];
        k--;
        i--;
      } //eow

      Qw[k-degree-1].x= Qw[k-degree].x;
      Qw[k-degree-1].y= Qw[k-degree].y;
      Qw[k-degree-1].z= Qw[k-degree].z;

      for (l= 1; l <= degree; l++) {
        ind= k-degree+l;
        alfa= Ubar[k+l]-X[j];

        if (fabs(alfa) == 0.0) {
          Qw[ind-1].x= Qw[ind].x;
          Qw[ind-1].y= Qw[ind].y;
          Qw[ind-1].z= Qw[ind].z;
        } else {
          alfa= alfa/(Ubar[k+l]-obj -> KnotVector(UV)[i-degree+l]);
          Qw[ind-1].x= alfa*Qw[ind-1].x + (1.0-alfa)*Qw[ind].x;
          Qw[ind-1].y= alfa*Qw[ind-1].y + (1.0-alfa)*Qw[ind].y;
          Qw[ind-1].z= alfa*Qw[ind-1].z + (1.0-alfa)*Qw[ind].z;
        } //fi
      } //eof
      Ubar[k]= X[j];
      k--;
    } //eof

//    printf("New Control Points:\n");
    if (UV) {
      for (i= 0; i <= obj -> Length(UV) + r; i++) {
        obj -> CtlPts()[i + n*(obj -> Length(1) + r + 1)].x= Qw[i].x;
        obj -> CtlPts()[i + n*(obj -> Length(1) + r + 1)].y= Qw[i].y;
        obj -> CtlPts()[i + n*(obj -> Length(1) + r + 1)].z= Qw[i].z;
      } //eof
    } else {
      for (i= 0; i <= obj -> Length(UV) + r; i++) {
        obj -> CtlPts()[i*obj -> Length(1) + n].x= Qw[i].x;
        obj -> CtlPts()[i*obj -> Length(1) + n].y= Qw[i].y;
        obj -> CtlPts()[i*obj -> Length(1) + n].z= Qw[i].z;
      } //eof
    } //fi
  } //eof

//  printf("New Knot Vector:\n");
  for (i= 0; i <= obj -> Length(UV) + obj -> Order(UV) + r; i++) {
//    printf("U[%d]: %f\n",i,Ubar[i]);
    obj -> KnotVector(UV)[i]= Ubar[i];
  } //eof

  obj -> Length(UV,obj -> Length(UV) + r + 1);
  free(Qw);
  free(Curve);
  free(Ubar);
} //eof NURBS_Knot::Refine()


float _Oslo(double *KnotVectorX, double *KnotVectorY, int K, int J, int I) {
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



void NURBS_Knot::_Insert(Object_NURBS *obj, bool UV, double *KV, int KVLen) {
  Point3d	*NewPts;
  int		i,n,k,ind1,ind2;
  double	oslo;

  UV ? obj -> SetLength(obj -> Length(0),KVLen - obj -> Order(1)) : obj -> SetLength(KVLen - obj -> Order(0),obj -> Length(1));
  NewPts= (Point3d*)malloc(sizeof(Point3d)*(KVLen - obj -> Order(UV))*obj -> Length(!UV));

  // Assign the new Control Point Locations
  // Calculate New Control Points through Recursive Oslo Algorithm
  for (i= 0; i < obj -> Length(!UV); i++) {
    for (n= 0; n < KVLen - obj -> Order(UV); n++) {
      UV ? ind1= n+i*(KVLen - obj -> Order(UV)) : ind1= i+n*obj -> Length(!UV);
      NewPts[ind1].x= NewPts[ind1].y= NewPts[ind1].z= 0;
      for (k= 0; k < obj -> Length(UV); k++) {
        UV ? ind2= k+i*obj -> Length(UV) : ind2= i+k*obj -> Length(!UV);
        oslo= _Oslo(obj -> KnotVector(UV),KV,obj -> Order(UV),n,k);
        NewPts[ind1].x+= oslo * obj -> CtlPts()[ind2].x;
        NewPts[ind1].y+= oslo * obj -> CtlPts()[ind2].y;
        NewPts[ind1].z+= oslo * obj -> CtlPts()[ind2].z;
      } //eof
    } //eof
  } //eof

  obj -> Length(UV,KVLen - obj -> Order(UV));

  // Save new points
  for (i= 0; i < obj -> Length(0) * obj -> Length(1); i++) {
    obj -> CtlPts()[i].x= NewPts[i].x;
    obj -> CtlPts()[i].y= NewPts[i].y;
    obj -> CtlPts()[i].z= NewPts[i].z;
//    printf("pt[%d]: [%.3f][%.3f][%.3f]\n",i,NewPts[i].x,NewPts[i].y,NewPts[i].z);
  } //eof

  // Save new Knot Vector (U)
  for (i= 0; i < KVLen; i++)
    obj -> KnotVector(UV)[i]= KV[i];

  free(NewPts);
} //eof NURBS_Knot::_Insert()


void NURBS_Knot::Subdivide(Object_NURBS *obj) {
  int		i,n;
  double	*KV;

  // Subdivide U
  if (obj -> Length(0) > 1) {
    KV= (double*)malloc(sizeof(double)*2*(obj -> Length(0) + obj -> Order(0)));
    for (i= n= 0; i < obj -> Length(0) + obj -> Order(0) - 1; i++) {
      KV[n++]= obj -> KnotVector(0)[i];
      if (obj -> KnotVector(0)[i+1] - obj -> KnotVector(0)[i])
        KV[n++]= (obj -> KnotVector(0)[i+1] + obj -> KnotVector(0)[i])/2.0;
    } //eof
    KV[n++]= obj -> KnotVector(0)[i];
    _Insert(obj,0,KV,n);
    free(KV);
  } //fi

  // Subdivide V
  if (obj -> Length(1) > 1) {
    KV= (double*)malloc(sizeof(double)*2*(obj -> Length(1) + obj -> Order(1)));
    for (i= n= 0; i < obj -> Length(1) + obj -> Order(1) - 1; i++) {
      KV[n++]= obj -> KnotVector(1)[i];
      if (obj -> KnotVector(1)[i+1] - obj -> KnotVector(1)[i])
        KV[n++]= (obj -> KnotVector(1)[i+1] + obj -> KnotVector(1)[i])/2.0;
    } //eof
    KV[n++]= obj -> KnotVector(1)[i];
    _Insert(obj,1,KV,n);
  } //fi
} //eof NURBS_Knot::Subdivide()


void NURBS_Knot::Insert(Object_NURBS *obj, int U, int V) {
  int		IntPos,i,n,k;
  double	IntLen,Length,KV[1];

  if (U) {
    IntLen= 0;
    for (i= 0; i < obj -> Length(0) - obj -> Order(0) + 1; i++) {
      Length= 0;
      for (n= 0; n < obj -> Order(0) - 1; n++)
        for (k= 0; k < obj -> Length(1); k++)
          Length+= NurbanaMath::Dist3d(obj -> CtlPts()[(i+n)*obj -> Length(1) + k], obj -> CtlPts()[(i+n+1)*obj -> Length(1) + k]);
      Length/= obj -> Length(1);

      if (Length > IntLen) {
        IntLen= Length;
        IntPos= i;
      } //fi
    } //eof
//    printf("New Knot: %f\n",(obj -> KnotVector(0)[obj -> Order(0) + IntPos - 1] + obj -> KnotVector(0)[obj -> Order(0) + IntPos])/2.0);
    KV[0]= (obj -> KnotVector(0)[obj -> Order(0) + IntPos - 1] + obj -> KnotVector(0)[obj -> Order(0) + IntPos])/2.0;
    Refine(obj,KV,1,0);
    Insert(obj,U-1,V);
  } else if (V) {
    IntLen= 0;
    for (i= 0; i < obj -> Length(1) - obj -> Order(1) + 1; i++) {
      Length= 0;
      for (n= 0; n < obj -> Order(1) - 1; n++)
        for (k= 0; k < obj -> Length(0); k++)
          Length+= NurbanaMath::Dist3d(obj -> CtlPts()[(k)*obj -> Length(1) + i+n], obj -> CtlPts()[(k)*obj -> Length(1) + i+n+1]);
      Length/= obj -> Length(1);

      if (Length > IntLen) {
        IntLen= Length;
        IntPos= i;
      } //fi
    } //eof
//    printf("New Knot: %f\n",(obj -> KnotVector(0)[obj -> Order(0) + IntPos - 1] + obj -> KnotVector(0)[obj -> Order(0) + IntPos])/2.0);
    KV[0]= (obj -> KnotVector(1)[obj -> Order(1) + IntPos - 1] + obj -> KnotVector(1)[obj -> Order(1) + IntPos])/2.0;
    Refine(obj,KV,1,1);
    Insert(obj,U,V-1);
  } //fi
} //eof NURBS_Knot::Insert()


float _Dist4d(Point3d P1, Point3d P2) {
  return( sqrt((P2.x-P1.x)*(P2.x-P1.x)+(P2.y-P1.y)*(P2.y-P1.y)+(P2.z-P1.z)*(P2.z-P1.z)) );
} //eof _Dist4d()


void NURBS_Knot::Remove(Object_NURBS *obj) {
  int		mult,deg,kind,i,j;
  double	alfi,alfj,kval;
  Point3d	Temp[32];

  mult= 1;
  deg= obj -> Order(0) - 1;
  kind= 3;
  kval= 1.0;

  i= kind - deg;
  j= kind - mult;

  Temp[i-1].x= obj -> CtlPts()[1].x;
  Temp[i-1].y= obj -> CtlPts()[1].y;
  Temp[i-1].z= obj -> CtlPts()[1].z;

  Temp[j+1].x= obj -> CtlPts()[4].x;
  Temp[j+1].y= obj -> CtlPts()[4].y;
  Temp[j+1].z= obj -> CtlPts()[4].z;

  while (j - i > 0) {
    alfi= (kval-obj -> KnotVector(0)[i])/(obj -> KnotVector(0)[i+obj -> Order(0)]-obj -> KnotVector(0)[i]);
    alfj= (kval-obj -> KnotVector(0)[j])/(obj -> KnotVector(0)[j+obj -> Order(0)]-obj -> KnotVector(0)[j]);

    Temp[i].x= (obj -> CtlPts()[i].x-(1-alfi)*Temp[i-1].x)/alfi;
    Temp[i].y= (obj -> CtlPts()[i].y-(1-alfi)*Temp[i-1].y)/alfi;
    Temp[i].z= (obj -> CtlPts()[i].z-(1-alfi)*Temp[i-1].z)/alfi;

    Temp[j].x= (obj -> CtlPts()[j].x-alfj*Temp[j+1].x)/(1-alfj);
    Temp[j].y= (obj -> CtlPts()[j].y-alfj*Temp[j+1].y)/(1-alfj);
    Temp[j].z= (obj -> CtlPts()[j].z-alfj*Temp[j+1].z)/(1-alfj);
    printf("(%d,%d)\n",i,j);
    printf("i: [%.2f][%.2f][%.2f], j: [%.2f][%.2f][%.2f]\n",Temp[i].x,Temp[i].y,Temp[i].z,Temp[j].x,Temp[j].y,Temp[j].z);

    i++;
    j--;
  } //eow

} //eof NURBS_Knot::Remove()


void NURBS_Knot::BezierDecompose(int Npts, int Order, Point3d *CtlPts, double *KV) {
  int		a,b,i,j,k,m,r,s,save,mult,deg,BezInd;
  Point3d	BezCurves[16][16];
  double	numer,alpha,alphas[16];

  m= Npts + Order;
  a= Order - 1;
  b= Order;
  deg= Order - 1;
  BezInd= 0;

  for (i= 0; i < Order; i++)
    BezCurves[BezInd][i]= CtlPts[i];

  while (b < m) {
    i= b;
    while (b < m && KV[b+1] == KV[b])
      b++;
    mult= b-i+1;
    if (mult < deg) {
      numer= KV[b]-KV[a];
      for (j= deg; j > mult; j--)
        alphas[j-mult-1]= numer/(KV[a+j]-KV[a]);
      r= deg-mult;
      for (j= 1; j <= r; j++) {
        save= r-j;
        s= mult+j;
        for (k= deg; k >= s; k--) {
          alpha= alphas[k-s];
          BezCurves[BezInd][k].x= alpha*BezCurves[BezInd][k].x + (1.0-alpha)*BezCurves[BezInd][k-1].x;
          BezCurves[BezInd][k].y= alpha*BezCurves[BezInd][k].y + (1.0-alpha)*BezCurves[BezInd][k-1].y;
        } //eof
        if (b < m)
          BezCurves[BezInd][save]= BezCurves[BezInd][deg];
      } //eof
    } //fi
    BezInd++;
    if (b < m) {
      for (i= deg - mult; i <= deg; i++)
        BezCurves[BezInd][i]= CtlPts[b-deg+i];
      a= b;
      b++;
    } //fi
  } //eow

  printf("Bez Curves: %d\n",BezInd);
} //eof NURBS_Knot::BezierDecompose()