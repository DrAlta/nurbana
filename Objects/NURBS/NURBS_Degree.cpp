#include "NURBS_Degree.h"


void NURBS_Degree::Elevate(Object_NURBS *obj, int Uinc, int Vinc) {
  int		Npts,Order,Degree,Curve,UV,Inc,ph,mpi,mh,kind,r,a,b,cind,mul,oldr,lbz,rbz,s,save;
  int		i,j,k,first,last,tr,kj,FnlLU,FnlLV,Length;

  float		ua,ub,alf,gam,bet,temp;
  float		*Uh,*Coef,*Coefb,*FnlKVU,*FnlKVV;

  Point3d	*Nextbpts,*Qw,*Pts,*FnlPts,*bpts,*ebpts;

  // Memory Allocation
  // Resize Surface
  obj -> SetLength(obj -> Length(0)+(obj -> Length(0)-(obj -> Order(0)-1))*Uinc,obj -> Length(1)+(obj -> Length(1)-(obj -> Order(1)-1))*Vinc);

  UV= (obj -> Order(0) > obj -> Order(1)) ? obj -> Order(0) : obj -> Order(1);
    bpts= (Point3d*)malloc(sizeof(Point3d)*64);
    Coefb= (float*)malloc(sizeof(float)*UV);
    Nextbpts= (Point3d*)malloc(sizeof(Point3d)*UV);

  UV= (obj -> Order(0)+Uinc > obj -> Order(1)+Vinc) ? obj -> Order(0)+Uinc : obj -> Order(1)+Vinc;
    ebpts= (Point3d*)malloc(sizeof(Point3d)*64);
    UV*= (obj -> Order(0) > obj -> Order(1)) ? obj -> Order(0) : obj -> Order(1);
    Coef= (float*)malloc(sizeof(float)*(UV));

  UV= (obj -> Length(0)+(obj -> Length(0)-(obj -> Order(0)-1))*Uinc) > (obj -> Length(1)+(obj -> Length(1)-(obj -> Order(1)-1))*Vinc) ? obj -> Length(0)+(obj -> Length(0)-(obj -> Order(0)-1))*Uinc : obj -> Length(1)+(obj -> Length(1)-(obj -> Order(1)-1))*Vinc;
    Qw= (Point3d*)malloc(sizeof(Point3d)*UV);
    Pts= (Point3d*)malloc(sizeof(Point3d)*UV);
  FnlPts= (Point3d*)malloc(sizeof(Point3d)*(obj -> Length(0)+(obj -> Length(0)-(obj -> Order(0)-1))*Uinc)*(obj -> Length(1)+(obj -> Length(1)-(obj -> Order(1)-1))*Vinc));

  FnlKVU= (float*)malloc(sizeof(float)*(obj -> Length(0)+(obj -> Length(0)-(obj -> Order(0)-1))*Uinc+Uinc+obj -> Order(0)));
  FnlKVV= (float*)malloc(sizeof(float)*(obj -> Length(1)+(obj -> Length(1)-(obj -> Order(1)-1))*Vinc+Vinc+obj -> Order(1)));
  Uh= (float*)malloc(sizeof(float)*((obj -> Length(0)+(obj -> Length(0)-(obj -> Order(0)-1))*Uinc+Uinc+obj -> Order(0)) > (obj -> Length(1)+(obj -> Length(1)-(obj -> Order(1)-1))*Vinc+Vinc+obj -> Order(1)) ? obj -> Length(0)+(obj -> Length(0)-(obj -> Order(0)-1))*Uinc+Uinc+obj -> Order(0) : obj -> Length(1)+(obj -> Length(1)-(obj -> Order(1)-1))*Vinc+Vinc+obj -> Order(1)));

  for (UV= 0; UV < 2; UV++) {
    Inc= UV ? Vinc : Uinc;

    if (Inc) {
      Npts= obj -> Length(UV);
      Order= obj -> Order(UV);
      Degree= Order-1;
      ph= Degree+Inc;

      // Compute Bezier degree elevation coefficients
      Coef[0]= Coef[(ph+1)*(Degree+1)-1]= 1.0;
      for (i= 1; i <= ph/2; i++) {
        temp= 1.0/NurbanaMath::Comb(ph,i);
        mpi= NurbanaMath::Min(Degree,i);
        for (j= NurbanaMath::Max(0,i-Inc); j <= mpi; j++)
          Coef[i*(Degree+1)+j]= temp*NurbanaMath::Comb(Degree,j)*NurbanaMath::Comb(Inc,i-j);
      } //eof

      for (i= (ph/2)+1; i <= ph-1; i++) {
        mpi= NurbanaMath::Min(Degree,i);
        for (j= NurbanaMath::Max(0,i-Inc); j <= mpi; j++)
          Coef[i*(Degree+1)+j]= Coef[(ph-i)*(Degree+1)+(Degree-j)];
      } //eof

      // ---------------------------

      Length= obj -> Length(!UV);
      if (UV && Uinc) Length+= (obj -> Length(1)-(obj -> Order(1)-1))*Uinc;
      for (Curve= 0; Curve < Length; Curve++) {
        Npts= obj -> Length(UV);
        mh= ph;
        kind= ph+1;
        r= -1;
        a= Degree;
        b= Order;
        cind= 1;
        ua= obj -> KnotVector(UV)[0];

        // Copy CtlPts into Pts for degree elevation
        if (UV) {
          // V Directional Curves
          for (i= 0; i < obj -> Length(1); i++) {
            Pts[i].x= obj -> CtlPts()[i+obj -> Length(1)*Curve].x;
            Pts[i].y= obj -> CtlPts()[i+obj -> Length(1)*Curve].y;
            Pts[i].z= obj -> CtlPts()[i+obj -> Length(1)*Curve].z;
          } //eof
        } else {
          // U Directional Curves
          for (i= 0; i < obj -> Length(0); i++) {
            Pts[i].x= obj -> CtlPts()[i*obj -> Length(1)+Curve].x;
            Pts[i].y= obj -> CtlPts()[i*obj -> Length(1)+Curve].y;
            Pts[i].z= obj -> CtlPts()[i*obj -> Length(1)+Curve].z;
          } //eof
        } //fi

        Qw[0].x= Pts[0].x;
        Qw[0].y= Pts[0].y;
        Qw[0].z= Pts[0].z;

        for (i= 0; i <= ph; i++)
          Uh[i]= ua;

        // Initialize first Bezier segment
        for (i= 0; i <= Degree; i++) {
          bpts[i].x= Pts[i].x;
          bpts[i].y= Pts[i].y;
          bpts[i].z= Pts[i].z;
        } //eof

        while (b < Npts+Order) {
          i= b;

          while (b < Npts+Degree && obj -> KnotVector(UV)[b] == obj -> KnotVector(UV)[b+1])
            b++;

          mul= b - i + 1;
          mh+= mul + Inc;
          ub= obj -> KnotVector(UV)[b];
          oldr= r;
          r= Degree - mul;

          // Insert Knot u(b) r times
          if (oldr > 0) {
            lbz= (oldr+2)/2;
          } else {
            lbz= 1;
          } //fi

          if (r > 0) {
            rbz= ph - (r + 1)/2;
          } else {
            rbz= ph;
          } //fi

          if (r > 0) {
            // Insert knot to get Bezier segment
            temp= ub - ua;
            for (k= Degree; k > mul; k--)
              Coefb[k - mul - 1]= temp / (obj -> KnotVector(UV)[a+k] - ua);

            for (j= 1; j <= r; j++) {
              save= r - j;
              s= mul + j;

              for (k= Degree; k >= s; k--) {
                bpts[k].x= Coefb[k - s] * bpts[k].x + (1.0 - Coefb[k - s]) * bpts[k - 1].x;
                bpts[k].y= Coefb[k - s] * bpts[k].y + (1.0 - Coefb[k - s]) * bpts[k - 1].y;
                bpts[k].z= Coefb[k - s] * bpts[k].z + (1.0 - Coefb[k - s]) * bpts[k - 1].z;
              } //eof

              Nextbpts[save].x= bpts[Degree].x;
              Nextbpts[save].y= bpts[Degree].y;
              Nextbpts[save].z= bpts[Degree].z;
            } //eof
          } //fi

          for (i= lbz; i <= ph; i++) {
            // Only points lbz,...,ph are used below
            ebpts[i].x= 0;
            ebpts[i].y= 0;
            ebpts[i].z= 0;

            mpi= NurbanaMath::Min(Degree,i);
            for (j= NurbanaMath::Max(0,i-Inc); j <= mpi; j++) {
              ebpts[i].x+= Coef[i*(Degree+1)+j] * bpts[j].x;
              ebpts[i].y+= Coef[i*(Degree+1)+j] * bpts[j].y;
              ebpts[i].z+= Coef[i*(Degree+1)+j] * bpts[j].z;
            } //eof
          } //eof

          if (oldr > 1) {
            // Must remove Knot u=U[a] oldr times
            first= kind - 2;
            last= kind;
            temp= ub - ua;
            bet= (ub - Uh[kind - 1]) / temp;

            for (tr= 1; tr < oldr; tr++) {
              // Knot Removal Loop
              i= first;
              j= last;
              kj= j - kind + 1;

              while (j - i > tr) {
                // Loop and compute the new conrol points for one removal step
                if (i < cind) {
                  alf= (ub - Uh[i]) / (ua - Uh[i]);
                  Qw[i].x= alf * Qw[i].x + (1.0 - alf) * Qw[i - 1].x;
                  Qw[i].y= alf * Qw[i].y + (1.0 - alf) * Qw[i - 1].y;
                  Qw[i].z= alf * Qw[i].z + (1.0 - alf) * Qw[i - 1].z;
                } //fi

                if (j >= lbz) {
                  if (j - tr <= kind - ph + oldr) {
                    gam= (ub - Uh[j - tr]) / temp;
                    ebpts[kj].x= gam * ebpts[kj].x + (1.0 - gam) * ebpts[kj + 1].x;
                    ebpts[kj].y= gam * ebpts[kj].y + (1.0 - gam) * ebpts[kj + 1].y;
                    ebpts[kj].z= gam * ebpts[kj].z + (1.0 - gam) * ebpts[kj + 1].z;
                  } else {
                    ebpts[kj].x= bet * ebpts[kj].x + (1.0 - bet) * ebpts[kj + 1].x;
                    ebpts[kj].y= bet * ebpts[kj].y + (1.0 - bet) * ebpts[kj + 1].y;
                    ebpts[kj].z= bet * ebpts[kj].z + (1.0 - bet) * ebpts[kj + 1].z;
                  } //fi
                } //fi

                i++;
                j--;
                kj--;
              } //eow

              first--;
              last++;
            } //eof
          } //fi

          if (a != Degree) {
            // Load the Knot ua
            for (i= 0; i < ph - oldr; i++) {
              Uh[kind]= ua;
              kind= kind++;
            } //eof
          } //fi

          for (j= lbz; j <= rbz; j++) {
            // Load Ctrl Points into Qw
            Qw[cind].x= ebpts[j].x;
            Qw[cind].y= ebpts[j].y;
            Qw[cind].z= ebpts[j].z;
            cind++;
          } //eof

          // Set up for next pass thru loop
          for (j= 0; j < r; j++) {
            bpts[j].x= Nextbpts[j].x;
            bpts[j].y= Nextbpts[j].y;
            bpts[j].z= Nextbpts[j].z; 
          } //eof

          for (j= r; j <= Degree; j++) {
            if (r >= 0) { // WTF?
              bpts[j].x= Pts[b-Degree+j].x;
              bpts[j].y= Pts[b-Degree+j].y;
              bpts[j].z= Pts[b-Degree+j].z;
            } //fi
          } //eof

          a= b;
          ua= ub;
          b++;
        } //eow

        // End Knot
        for (i= 0; i <= ph; i++)
          Uh[kind+i]= ub;

        Npts= mh-ph;

        for (i= 0; i < Npts; i++) {
          Pts[i].x= Qw[i].x;
          Pts[i].y= Qw[i].y;
          Pts[i].z= Qw[i].z;
        } //eof

        // Save Curve Permanently
        if (UV) {
          // V Directional Curves
          for (i= 0; i < Npts; i++) {
            FnlPts[i+Npts*Curve].x= Pts[i].x;
            FnlPts[i+Npts*Curve].y= Pts[i].y;
            FnlPts[i+Npts*Curve].z= Pts[i].z;
          } //eof
        } else {
          // U Directional Curves
          for (i= 0; i < Npts; i++) {
            FnlPts[i*(obj -> Length(1)+(obj -> Length(1)-(obj -> Order(1)-1))*Vinc)+Curve].x= Pts[i].x;
            FnlPts[i*(obj -> Length(1)+(obj -> Length(1)-(obj -> Order(1)-1))*Vinc)+Curve].y= Pts[i].y;
            FnlPts[i*(obj -> Length(1)+(obj -> Length(1)-(obj -> Order(1)-1))*Vinc)+Curve].z= Pts[i].z;

            // Update Original because we know U is just tacked onto the end of the array and
            // may be needed for V directional elevation if this is a U and V elevation
            obj -> CtlPts()[i*obj -> Length(1)+Curve].x= Pts[i].x;
            obj -> CtlPts()[i*obj -> Length(1)+Curve].y= Pts[i].y;
            obj -> CtlPts()[i*obj -> Length(1)+Curve].z= Pts[i].z;
          } //eof
        } //fi
      } //eof (Curves)

      if (UV) {
        FnlLV= mh-ph;
        for (i= 0; i < FnlLV+obj -> Order(1)+Vinc; i++)
          FnlKVV[i]= Uh[i];
      } else {
        FnlLU= mh-ph;
        for (i= 0; i < FnlLU+obj -> Order(0)+Uinc; i++)
          FnlKVU[i]= Uh[i];
      } //fi
    } //fi
  } //eof UV

  // V - Direction
  if (Vinc) {
    obj -> Length(1,FnlLV);
    for (i= 0; i < obj -> Length(1)+obj -> Order(1)+Vinc; i++)
      obj -> KnotVector(1)[i]= FnlKVV[i];
    obj -> Order(1,obj -> Order(1)+Vinc);
  } //

  // U - Direction
  if (Uinc) {
    obj -> Length(0,FnlLU);
    for (i= 0; i < obj -> Length(0)+obj -> Order(0)+Uinc; i++)
      obj -> KnotVector(0)[i]= FnlKVU[i];
    obj -> Order(0,obj -> Order(0)+Uinc);
  } //fi


  // Copy Final Points into Original Points
  if (Uinc || Vinc) {
    for (i= 0; i < obj -> Length(0) * obj -> Length(1); i++) {
      obj -> CtlPts()[i].x= FnlPts[i].x;
      obj -> CtlPts()[i].y= FnlPts[i].y;
      obj -> CtlPts()[i].z= FnlPts[i].z;
    } //eof
  } //fi

  // Deallocate Memory
  free(FnlPts);
  free(Pts);
  free(Qw);
  free(FnlKVU);
  free(FnlKVV);
  free(bpts);
  free(ebpts);
  free(Nextbpts);
  free(Coefb);
  free(Uh);
  free(Coef);
} //eof NURBS_Degree::Elevate()


float _Basis(int n, int i, float t) {
  float ti; // t^i
  float tni; // (1 - t)^i

  // handle the special cases to avoid domain problem with pow
  if ((t == 0) && (i == 0)) ti= 1.0; else ti= pow(t,i);

  if ((n == i) && (t == 1)) tni= 1.0; else tni= pow((1-t),(n-i));
  return( NurbanaMath::Comb(n,i)*ti*tni ); // calculate Bernstein basis function
} //eof _Basis()


void NURBS_Degree::BezDegReduce(Point3d *bpts, Point3d *rbpts, int Degree, float &MaxErr) {
  // Reduce Bezier segment and store in rbpts
  int	i,r;
  float	a,a2,x,y,z;

  r= (Degree-1)/2;

  rbpts[0].x= bpts[0].x;
  rbpts[0].y= bpts[0].y;
  rbpts[0].z= bpts[0].z;

  rbpts[Degree-1].x= bpts[Degree].x;
  rbpts[Degree-1].y= bpts[Degree].y;
  rbpts[Degree-1].z= bpts[Degree].z;

  // Two cases, Even or Odd
  if (Degree%2) {
    // Odd
    for (i= 1; i <= r-1; i++) {
      a= float(i)/float(Degree);
      rbpts[i].x= (bpts[i].x-a*rbpts[i-1].x)/(1.0-a);
      rbpts[i].y= (bpts[i].y-a*rbpts[i-1].y)/(1.0-a);
      rbpts[i].z= (bpts[i].z-a*rbpts[i-1].z)/(1.0-a);
    } //eof
    for (i= Degree-2; i >= r+1; i--) {
      a= float(i+1)/float(Degree);
      rbpts[i].x= (bpts[i+1].x-(1.0-a)*rbpts[i+1].x)/a;
      rbpts[i].y= (bpts[i+1].y-(1.0-a)*rbpts[i+1].y)/a;
      rbpts[i].z= (bpts[i+1].z-(1.0-a)*rbpts[i+1].z)/a;
    } //eof
    a= float(r)/(float(Degree));
    rbpts[r].x= (bpts[r].x - a*rbpts[r-1].x)/(1.0-a);
    rbpts[r].y= (bpts[r].y - a*rbpts[r-1].y)/(1.0-a);
    rbpts[r].z= (bpts[r].z - a*rbpts[r-1].z)/(1.0-a);
    a= float(r+1)/(float(Degree));
    rbpts[r].x+= (bpts[r+1].x-(1.0-a)*rbpts[r+1].x)/a;
    rbpts[r].y+= (bpts[r+1].y-(1.0-a)*rbpts[r+1].y)/a;
    rbpts[r].z+= (bpts[r+1].z-(1.0-a)*rbpts[r+1].z)/a;

    rbpts[r].x*= 0.5;
    rbpts[r].y*= 0.5;
    rbpts[r].z*= 0.5;
  } else {
    // Even
    for (i= 1; i <= r; i++) {
      a= float(i)/float(Degree);
      rbpts[i].x= (bpts[i].x-a*rbpts[i-1].x)/(1.0-a);
      rbpts[i].y= (bpts[i].y-a*rbpts[i-1].y)/(1.0-a);
      rbpts[i].z= (bpts[i].z-a*rbpts[i-1].z)/(1.0-a);
    } //eof
    for (i= Degree-2; i >= r+1; i--) {
      a= float(i+1)/float(Degree);
      rbpts[i].x= (bpts[i+1].x - (1.0-a)*rbpts[i+1].x)/a;
      rbpts[i].y= (bpts[i+1].y - (1.0-a)*rbpts[i+1].y)/a;
      rbpts[i].z= (bpts[i+1].z - (1.0-a)*rbpts[i+1].z)/a;
    } //eof
  } //fi

  // Error Calculation
  if (Degree%2) {
    // Odd
    a= float(r)/(float(Degree));
    a2= float(r+1)/(float(Degree));
    x= (0.5)*(1.0-a)*((_Basis(r,Degree,0.5)-_Basis(r+1,Degree,0.5))*(((bpts[r].x-a*rbpts[r-1].x)/(1.0-a))-((bpts[r+1].x-(1.0-a2)*rbpts[r+1].x)/(a2))));
    y= (0.5)*(1.0-a)*((_Basis(r,Degree,0.5)-_Basis(r+1,Degree,0.5))*(((bpts[r].y-a*rbpts[r-1].y)/(1.0-a))-((bpts[r+1].y-(1.0-a2)*rbpts[r+1].y)/(a2))));
    z= (0.5)*(1.0-a)*((_Basis(r,Degree,0.5)-_Basis(r+1,Degree,0.5))*(((bpts[r].z-a*rbpts[r-1].z)/(1.0-a))-((bpts[r+1].z-(1.0-a2)*rbpts[r+1].z)/(a2))));
  } else {
    // Even
    x= _Basis(r+1,Degree,0.5)*(bpts[r+1].x-0.5*(rbpts[r].x+rbpts[r+1].x));
    y= _Basis(r+1,Degree,0.5)*(bpts[r+1].y-0.5*(rbpts[r].y+rbpts[r+1].y));
    z= _Basis(r+1,Degree,0.5)*(bpts[r+1].z-0.5*(rbpts[r].z+rbpts[r+1].z));
  } //fi
  // Assumption, take the length of the error vector???
//  printf("Ex: %f, Ey: %f, Ez: %f\n",x,y,z);
  MaxErr= sqrt(x*x+y*y+z*z);
} //eof NURBS_Dgree::BezDegReduce()


void NURBS_Degree::Reduce(Object_NURBS *obj, int Udec, int Vdec) {
  int		Npts,Order,Curve,UV,Dec,Length,FnlLU,FnlLV;
  int		n,i,j,k,s,kj,first,last,ii,q,K,L;
  int		ph,mh,Degree,kind,r,a,b,cind,mult,m,oldr,lbz,save;

  float		*Uh,*error,*alphas,numer,MaxErr,TOL,alfa,beta,delta,Br,*FnlKVU,*FnlKVV,*KnotVector;
  Point3d	*Pts,*bpts,*Temp,*rbpts,*Nextbpts,A,*FnlPts;

  TOL= 100.0;


  // Memory Allocations

  UV= (obj -> Length(0) > obj -> Length(1) ? obj -> Length(0) : obj -> Length(1));
    Pts= (Point3d*)malloc(sizeof(Point3d)*UV);
    Temp= (Point3d*)malloc(sizeof(Point3d)*UV);
  UV= obj -> Length(0) + obj -> Order(0) > obj -> Length(1) + obj -> Order(1) ? obj -> Length(0) + obj -> Order(0) : obj -> Length(1) + obj -> Order(1);
    error= (float*)malloc(sizeof(float)*UV);
    Uh= (float*)malloc(sizeof(float)*UV);
    KnotVector= (float*)malloc(sizeof(float)*UV);
  UV= (obj -> Order(0) > obj -> Order(1) ? obj -> Order(0) : obj -> Order(1));
    bpts= (Point3d*)malloc(sizeof(Point3d)*UV);
    rbpts= (Point3d*)malloc(sizeof(Point3d)*UV);
    Nextbpts= (Point3d*)malloc(sizeof(Point3d)*UV);
    alphas= (float*)malloc(sizeof(float)*UV);
  FnlPts= (Point3d*)malloc(sizeof(Point3d)*(obj -> Length(0) * obj -> Length(1)));
  FnlKVU= (float*)malloc(sizeof(float)*(obj -> Length(0) + obj -> Order(0)));
  FnlKVV= (float*)malloc(sizeof(float)*(obj -> Length(1) + obj -> Order(1)));


  for (UV= 0; UV < 2; UV++) {
    Dec= UV ? Vdec : Udec;
    if (Dec) {
    Length= obj -> Length(!UV);
    if (UV && Udec) Length-= (obj -> Length(1)-(obj -> Order(1)-1))*Udec;

    for (Curve= 0; Curve < Length; Curve++) {
      // Load CtlPts for Curve into Pts for Degree Reduction
      if (UV) {
        // V Directional Curves
        for (i= 0; i < obj -> Length(1); i++) {
          Pts[i].x= obj -> CtlPts()[i+obj -> Length(1)*Curve].x;
          Pts[i].y= obj -> CtlPts()[i+obj -> Length(1)*Curve].y;
          Pts[i].z= obj -> CtlPts()[i+obj -> Length(1)*Curve].z;
        } //eof
      } else {
        // U Directional Curves
        for (i= 0; i < obj -> Length(0); i++) {
          Pts[i].x= obj -> CtlPts()[i*obj -> Length(1)+Curve].x;
          Pts[i].y= obj -> CtlPts()[i*obj -> Length(1)+Curve].y;
          Pts[i].z= obj -> CtlPts()[i*obj -> Length(1)+Curve].z;
        } //eof
      } //fi
      for (i= 0; i < obj -> Length(UV) + obj -> Order(UV); i++)
        KnotVector[i]= obj -> KnotVector(UV)[i];

      Npts= UV ? obj -> Length(1) : obj -> Length(0);
      Order= UV ? obj -> Order(1) : obj -> Order(0);

      for (n= 0; n < Dec; n++) {
        Degree= Order-1;
        ph= Degree-1;
        mh= ph;

        kind= ph+1;
        r= -1;
        a= Degree;
        b= Order;
        cind= 1;
        mult= Degree;

        m= Npts+Degree+1;

        Temp[0].x= Pts[0].x;
        Temp[0].y= Pts[0].y;
        Temp[0].z= Pts[0].z;

        // Compute left end of knot vector
        for (i= 0; i <= ph; i++)
          Uh[i]= KnotVector[0];

        // Initialize first Bezier segment
        for (i= 0; i <= Degree; i++) {
          bpts[i].x= Pts[i].x;
          bpts[i].y= Pts[i].y;
          bpts[i].z= Pts[i].z;
        } //eof

        // Initialize Error Vector
        for (i= 0; i < m; i++)
          error[i]= 0;

        // Loop through knot vector
        while (b < m) {
          // First compute Knot multiplicity
          i= b;

          while (b < (m-1) && KnotVector[b] == KnotVector[b+1])
            b++;

          mult= b-i+1;
          mh= mh+mult-1;
          oldr= r;
          r= Degree-mult;

          if (oldr > 0) {
            lbz= (oldr+2)/2;
          } else {
            lbz= 1;
          } //fi

          if (r > 0) {
            numer= KnotVector[b]-KnotVector[a];

            for (k= Degree; k >= mult; k--)
              if (k-mult-1 >= 0) // WTF?
                alphas[k-mult-1]= numer/(KnotVector[a+k]-KnotVector[a]);

            for (j= 1; j <= r; j++) {
              save= r-j;
              s= mult+j;
              for (k= Degree; k >= s; k--) {
                bpts[k].x= alphas[k-s]*bpts[k].x + (1.0-alphas[k-s])*bpts[k-1].x;
                bpts[k].y= alphas[k-s]*bpts[k].y + (1.0-alphas[k-s])*bpts[k-1].y;
                bpts[k].z= alphas[k-s]*bpts[k].z + (1.0-alphas[k-s])*bpts[k-1].z;
              } //eof

              Nextbpts[save].x= bpts[Degree].x;
              Nextbpts[save].y= bpts[Degree].y;
              Nextbpts[save].z= bpts[Degree].z;
            } //eof
          } //fi

          // Degree Reduce Bezier Segment
          BezDegReduce(bpts,rbpts,Degree,MaxErr);

          error[a]= error[a]+MaxErr;
          if (error[a] > TOL) {
            // Curve not degree reducible
            printf("bail!\n");
            return;
          } //fi

          if (oldr > 0) {
            // Remove knot KnotVectorU[a] oldr times
            first= kind;
            last= kind;
            for (k= 0; k < oldr; k++) {
              i= first;
              j= last;
              kj= j-kind;
              while (j-i > k) {
                alfa= (KnotVector[a]-Uh[i-1])/(KnotVector[b]-Uh[i-1]);
                beta= (KnotVector[a]-Uh[j-k-1])/(KnotVector[b]-Uh[j-k-1]);
                Temp[i-1].x= (Temp[i-1].x-(1.0-alfa)*Temp[i-2].x)/alfa;
                Temp[i-1].y= (Temp[i-1].y-(1.0-alfa)*Temp[i-2].y)/alfa;
                Temp[i-1].z= (Temp[i-1].z-(1.0-alfa)*Temp[i-2].z)/alfa;

                rbpts[kj].x= (rbpts[kj].x-beta*rbpts[kj+1].x)/(1.0-beta);
                rbpts[kj].y= (rbpts[kj].y-beta*rbpts[kj+1].y)/(1.0-beta);
                rbpts[kj].z= (rbpts[kj].z-beta*rbpts[kj+1].z)/(1.0-beta);

                i++;
                j--;
                kj--;
              } //eow

              if (j-i < k) {
                Br= NurbanaMath::Dist4d(Temp[i-2],rbpts[kj+1]);
              } else {
                delta= (KnotVector[a]-Uh[i-1])/(KnotVector[b]-Uh[i-1]);
                A.x= delta*rbpts[kj+1].x+(1.0-delta)*Temp[i-2].x;
                A.y= delta*rbpts[kj+1].y+(1.0-delta)*Temp[i-2].y;
                A.z= delta*rbpts[kj+1].z+(1.0-delta)*Temp[i-2].z;
                Br= NurbanaMath::Dist4d(Temp[i-1],A);
              } //fi

              // Update the Error Vector
              K= a+oldr-k;
              q= (2*Degree-k+1)/2;
              L= K-q;

              for (ii= L; ii <= a; ii++) {
                // These knot spans were affected
                error[ii]= error[ii] + Br;
                if (error[ii] > TOL) {
                  printf("bail: %f\n",error[ii]);
                  return;  // Curve not degree reducible
                }
              } //eof

              first--;
              last++;
            } //eof

            cind= i-1;
          } //fi

          // Load knot vector and control points
          if (a != Degree) {
            for (i= 0; i < ph-oldr; i++) {
              Uh[kind]= KnotVector[a];
              kind++;
            } //eof
          } //fi

          for (i= lbz; i <= ph; i++) {
            Temp[cind].x= rbpts[i].x;
            Temp[cind].y= rbpts[i].y;
            Temp[cind].z= rbpts[i].z;
            cind++;
          } //eof

          // Set up for next pass through
          for (i= 0; i < r; i++) {
            bpts[i].x= Nextbpts[i].x;
            bpts[i].y= Nextbpts[i].y;
            bpts[i].z= Nextbpts[i].z;
          } //eof

          for (i= r; i <= Degree; i++) {
            if (i >= 0) {
              bpts[i].x= Pts[b-Degree+i].x;
              bpts[i].y= Pts[b-Degree+i].y;
              bpts[i].z= Pts[b-Degree+i].z;
            } //fi
          } //eof

          a= b;
          b++;
        } //eow

        // End Knot
        for (i= 0; i <= ph; i++)
          Uh[kind+i]= KnotVector[b-1];

        Order--;
        Npts= mh-ph;

        for (i= 0; i < Npts; i++) {
          Pts[i].x= Temp[i].x;
          Pts[i].y= Temp[i].y;
          Pts[i].z= Temp[i].z;
        } //eof

        for (i= 0; i < Npts+Order; i++)
          KnotVector[i]= Uh[i];
      } //eof

      // Save Completed Curve
      if (UV) {
        // V Directional Curves
        for (i= 0; i < Npts; i++) {
          FnlPts[i+Npts*Curve].x= Pts[i].x;
          FnlPts[i+Npts*Curve].y= Pts[i].y;
          FnlPts[i+Npts*Curve].z= Pts[i].z;
        } //eof
      } else {
        // U Directional Curves
        for (i= 0; i < Npts; i++) {
          FnlPts[i*(obj -> Length(1)+(obj -> Length(1)-(obj -> Order(1)-1))*Vdec)+Curve].x= Pts[i].x;
          FnlPts[i*(obj -> Length(1)+(obj -> Length(1)-(obj -> Order(1)-1))*Vdec)+Curve].y= Pts[i].y;
          FnlPts[i*(obj -> Length(1)+(obj -> Length(1)-(obj -> Order(1)-1))*Vdec)+Curve].z= Pts[i].z;

          // Update Original because we know U is just tacked onto the end of the array and
          // may be needed for V directional elevation if this is a U and V reduction
          obj -> CtlPts()[i*obj -> Length(1)+Curve].x= Pts[i].x;
          obj -> CtlPts()[i*obj -> Length(1)+Curve].y= Pts[i].y;
          obj -> CtlPts()[i*obj -> Length(1)+Curve].z= Pts[i].z;
        } //eof
      } //fi
    } //eof (Curves)

    if (UV) {
      FnlLV= mh-ph;
      for (i= 0; i < FnlLV+obj -> Order(1)-Vdec; i++)
        FnlKVV[i]= Uh[i];
    } else {
      FnlLU= mh-ph;
      for (i= 0; i < FnlLU+obj -> Order(0)-Udec; i++)
        FnlKVU[i]= Uh[i];
    } //fi
    } //fi
  } //eof UV

  // V - Direction
  if (Vdec) {
    obj -> Length(1,FnlLV);
    for (i= 0; i < obj -> Length(1)+obj -> Order(1)-Vdec; i++)
      obj -> KnotVector(1)[i]= FnlKVV[i];
    obj -> Order(1,obj -> Order(1)-Vdec);
  } //fi

  // U - Direction
  if (Udec) {
    obj -> Length(0,FnlLU);
    for (i= 0; i < obj -> Length(0)+obj -> Order(0)-Udec; i++)
      obj -> KnotVector(0)[i]= FnlKVU[i];
    obj -> Order(0,obj -> Order(0)-Udec);
  } //fi


  // Copy Final Points into Original Points
  for (i= 0; i < obj -> Length(0) * obj -> Length(1); i++) {
    obj -> CtlPts()[i].x= FnlPts[i].x;
    obj -> CtlPts()[i].y= FnlPts[i].y;
    obj -> CtlPts()[i].z= FnlPts[i].z;
  } //eof


  // Free Memory
  free(Pts);
  free(Temp);
  free(Uh);
  free(bpts);
  free(rbpts);
  free(alphas);
  free(Nextbpts);
  free(FnlPts);
  free(FnlKVU);
  free(FnlKVV);
  free(KnotVector);
  free(error);
} //eof NURBS_Degree:Reduce()
