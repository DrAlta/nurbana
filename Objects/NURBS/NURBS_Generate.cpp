#include "NURBS_Generate.h"
SharedData	NURBS_Generate::ThreadData;
int		NURBS_Generate::ThreadID;



int ThreadLauncher() {
  NURBS_Generate::SurfaceThread(NURBS_Generate::ThreadID++);
  return(1);
} //eof ThreadLauncher


typedef int (Thread)(void*);
void NURBS_Generate::Surface(Object_NURBS *obj) {
  int		Change;

  // Surface Pts
  AssignThreadData(obj, obj -> Tess(0), obj -> Tess(1), obj -> TssPts(), obj -> Sum(), 0, 0, 0);
  Surface();

  // Isoparametric U Curves
  Change= obj -> Change(1);
  AssignThreadData(obj, obj -> IPCResolution(), obj -> IPCDensity(), obj -> IPCPts(), obj -> SumIPC(0), 0, 1, 0);
  Surface();

  // Isoparametric V Curves
  if (!Change) obj -> Change(1,0);
  AssignThreadData(obj, obj -> IPCDensity(), obj -> IPCResolution(), obj -> IPCPts(), obj -> SumIPC(1), obj -> IPCResolution() * obj -> IPCDensity(), 1, 1);
  Surface();
} //eof NURBS_Generate::Surface()


void NURBS_Generate::SurfaceThread(int ThreadID) {
  int		i[2],n[2],k[2],l[2],m[2],j[2];
  double	BasisResult[2];

  if (ThreadData.recalc) {
    // Copy B into Bold
    for (i[ThreadID]= 0; i[ThreadID] < ThreadData.Unpts * ThreadData.Vnpts; i[ThreadID]++) {
      ThreadData.obj -> Hold()[i[ThreadID]]= ThreadData.obj -> H()[i[ThreadID]];
      ThreadData.obj -> CtlPtsOld()[i[ThreadID]]= ThreadData.obj -> CtlPts()[i[ThreadID]];
    } //eof

//    m[ThreadID]= 0;
    m[ThreadID]= ThreadData.Task[ThreadID].TessStart*(ThreadData.TessV+1);
    for (i[ThreadID]= ThreadData.Task[ThreadID].TessStart; i[ThreadID] <= ThreadData.Task[ThreadID].TessStop; i[ThreadID]++) {
//    i[ThreadID]= ThreadData.Task[ThreadID].Tess;

      for (n[ThreadID]= 0; n[ThreadID] <= ThreadData.TessV; n[ThreadID]++) {
        ThreadData.Sum[m[ThreadID]]= Sum(ThreadData.obj -> H(),ThreadData.BasisU[i[ThreadID]],ThreadData.BasisV[n[ThreadID]],ThreadData.Unpts,ThreadData.Vnpts,ThreadID);

        ThreadData.TssPts[ThreadData.Index+m[ThreadID]].x= ThreadData.TssPts[ThreadData.Index+m[ThreadID]].y= ThreadData.TssPts[ThreadData.Index+m[ThreadID]].z= 0;
        j[ThreadID]= 0;

        for (k[ThreadID]= 0; k[ThreadID] < ThreadData.Unpts; k[ThreadID]++) {
          if (ThreadData.BasisU[i[ThreadID]][k[ThreadID]]) {

            for (l[ThreadID]= 0; l[ThreadID] < ThreadData.Vnpts; l[ThreadID]++) {
              if (ThreadData.BasisV[n[ThreadID]][l[ThreadID]]) {
                BasisResult[ThreadID]= ThreadData.obj -> H()[j[ThreadID]]*ThreadData.BasisU[i[ThreadID]][k[ThreadID]]*ThreadData.BasisV[n[ThreadID]][l[ThreadID]]/ThreadData.Sum[m[ThreadID]];

                ThreadData.TssPts[ThreadData.Index+m[ThreadID]].x+= ThreadData.obj -> CtlPts()[j[ThreadID]].x*BasisResult[ThreadID];
                ThreadData.TssPts[ThreadData.Index+m[ThreadID]].y+= ThreadData.obj -> CtlPts()[j[ThreadID]].y*BasisResult[ThreadID];
                ThreadData.TssPts[ThreadData.Index+m[ThreadID]].z+= ThreadData.obj -> CtlPts()[j[ThreadID]].z*BasisResult[ThreadID];
              } //fi
              j[ThreadID]++;
            } //eof

          } else {
            j[ThreadID]+= ThreadData.Vnpts;
          } //eof test

        } //eof

        m[ThreadID]++;
      } //eof
    } //eof
  } else {  // Increment Surface Calculations
    double	dh[2],dx[2],dy[2],dz[2];
    double	*sumold[2], sumratio[2];

    sumold[ThreadID]= (double*)malloc(sizeof(double)*(ThreadData.TessU+1)*(ThreadData.TessV+1));

    m[ThreadID]= 0;
    for (k[ThreadID]= 0; k[ThreadID] < ThreadData.Unpts; k[ThreadID]++) {
      for (l[ThreadID]= 0; l[ThreadID] < ThreadData.Vnpts; l[ThreadID]++) {

        dh[ThreadID]= ThreadData.obj -> H()[m[ThreadID]] - ThreadData.obj -> Hold()[m[ThreadID]];
        dx[ThreadID]= ThreadData.obj -> CtlPts()[m[ThreadID]].x - ThreadData.obj -> CtlPtsOld()[m[ThreadID]].x;
        dy[ThreadID]= ThreadData.obj -> CtlPts()[m[ThreadID]].y - ThreadData.obj -> CtlPtsOld()[m[ThreadID]].y;
        dz[ThreadID]= ThreadData.obj -> CtlPts()[m[ThreadID]].z - ThreadData.obj -> CtlPtsOld()[m[ThreadID]].z;

        if (dh[ThreadID] || dx[ThreadID] || dy[ThreadID] || dz[ThreadID]) {
          if (dh[ThreadID]) {
            // Calculate new sum function
            j[ThreadID]= 0;
            for (i[ThreadID]= 0; i[ThreadID] <= ThreadData.TessU; i[ThreadID]++) {
              for (n[ThreadID]= 0; n[ThreadID] <= ThreadData.TessV; n[ThreadID]++) {
                sumold[ThreadID][j[ThreadID]]= ThreadData.Sum[j[ThreadID]];  // Save old Sum Function
                if (ThreadData.BasisU[i[ThreadID]][k[ThreadID]] && ThreadData.BasisV[n[ThreadID]][l[ThreadID]])
                  ThreadData.Sum[j[ThreadID]]+= ThreadData.BasisU[i[ThreadID]][k[ThreadID]]*ThreadData.BasisV[n[ThreadID]][l[ThreadID]]*dh[ThreadID];
                j[ThreadID]++;
              } //eof
            } //eof
          } //fi

          // Calculate the change in the surface for each u,w
//          j[ThreadID]= 0;

          j[ThreadID]= ThreadData.Task[ThreadID].TessStart*(ThreadData.TessV+1);
          for (i[ThreadID]= ThreadData.Task[ThreadID].TessStart; i[ThreadID] <= ThreadData.Task[ThreadID].TessStop; i[ThreadID]++) {
            if (ThreadData.BasisU[i[ThreadID]][k[ThreadID]]) {
              for (n[ThreadID]= 0; n[ThreadID] <= ThreadData.TessV; n[ThreadID]++) {
                if (ThreadData.BasisV[n[ThreadID]][l[ThreadID]]) {
                  if (dh[ThreadID]) {
                    // Homogeneous coordinate changed
                    BasisResult[ThreadID]= dh[ThreadID]*ThreadData.BasisU[i[ThreadID]][k[ThreadID]]*ThreadData.BasisV[n[ThreadID]][l[ThreadID]]/ThreadData.Sum[j[ThreadID]];
                    sumratio[ThreadID]= sumold[ThreadID][j[ThreadID]]/ThreadData.Sum[j[ThreadID]];
                    ThreadData.TssPts[ThreadData.Index+j[ThreadID]].x= ThreadData.TssPts[ThreadData.Index+j[ThreadID]].x*sumratio[ThreadID] + ThreadData.obj -> CtlPts()[m[ThreadID]].x*BasisResult[ThreadID];
                    ThreadData.TssPts[ThreadData.Index+j[ThreadID]].y= ThreadData.TssPts[ThreadData.Index+j[ThreadID]].y*sumratio[ThreadID] + ThreadData.obj -> CtlPts()[m[ThreadID]].y*BasisResult[ThreadID];
                    ThreadData.TssPts[ThreadData.Index+j[ThreadID]].z= ThreadData.TssPts[ThreadData.Index+j[ThreadID]].z*sumratio[ThreadID] + ThreadData.obj -> CtlPts()[m[ThreadID]].z*BasisResult[ThreadID];
                  } else {
                    // Control net vertex changed
                    BasisResult[ThreadID]= ThreadData.obj -> H()[m[ThreadID]]*ThreadData.BasisU[i[ThreadID]][k[ThreadID]]*ThreadData.BasisV[n[ThreadID]][l[ThreadID]]/ThreadData.Sum[j[ThreadID]];
                    ThreadData.TssPts[ThreadData.Index+j[ThreadID]].x+= dx[ThreadID]*BasisResult[ThreadID];
                    ThreadData.TssPts[ThreadData.Index+j[ThreadID]].y+= dy[ThreadID]*BasisResult[ThreadID];
                    ThreadData.TssPts[ThreadData.Index+j[ThreadID]].z+= dz[ThreadID]*BasisResult[ThreadID];
                  } //fi
                } //fi
                j[ThreadID]++;
              } //eof
            } else {
              j[ThreadID]+= ThreadData.TessV+1;
            } //fi
          } //eof
        } //fi
        m[ThreadID]++;
      } //eof
    } //eof

    free(sumold[ThreadID]);
  } //fi
} //eof NURBS_Generate::SurfaceThread()


void NURBS_Generate::Surface() {
  int		i,OrderU,OrderV;
  Thread	*TF;	// Thread Function Pointer
  SDL_Thread	*Threads[2];

  TF= (Thread*)ThreadLauncher;


  ThreadData.Unpts= ThreadData.obj -> Length(0);
  ThreadData.Vnpts= ThreadData.obj -> Length(1);
  OrderU= ThreadData.obj -> Order(0);
  OrderV= ThreadData.obj -> Order(1);

  ThreadData.BasisU= (double**)malloc(sizeof(double)*ThreadData.TessU);
  for (i= 0; i < ThreadData.TessU; i++)
    ThreadData.BasisU[i]= (double*)malloc(sizeof(double)*ThreadData.Unpts);
  ThreadData.BasisV= (double**)malloc(sizeof(double)*ThreadData.TessV);
  for (i= 0; i < ThreadData.TessV; i++)
    ThreadData.BasisV[i]= (double*)malloc(sizeof(double)*ThreadData.Vnpts);

  ThreadData.TessU--;
  ThreadData.TessV--;

  ThreadData.recalc= (ThreadData.obj -> Change(ThreadData.Change) != ThreadData.Unpts+ThreadData.Vnpts+OrderU+OrderV+ThreadData.TessU+ThreadData.TessV);
  ThreadData.obj -> Change(ThreadData.Change,ThreadData.Unpts+ThreadData.Vnpts+OrderU+OrderV+ThreadData.TessU+ThreadData.TessV);


  if (ThreadData.recalc) {
    for (i= 0; i <= ThreadData.TessU; i++) {
      if (ThreadData.obj -> KnotType(0) == NURBS_KV_Periodic) {
        NURBSBasis(OrderU, (OrderU-1)+i*double(ThreadData.Unpts-(OrderU-1))/double(ThreadData.TessU), ThreadData.Unpts, ThreadData.obj -> KnotVector(0), ThreadData.obj -> H(), ThreadData.BasisU[i], 1);
      } else if (ThreadData.obj -> KnotType(0) == NURBS_KV_Open) {
        NURBSBasis(OrderU, i*double(ThreadData.obj -> KnotVector(0)[ThreadData.Unpts-1+OrderU])/double(ThreadData.TessU), ThreadData.Unpts, ThreadData.obj -> KnotVector(0), ThreadData.obj -> H(), ThreadData.BasisU[i], 1);
      } //fi
    } //eof

    for (i= 0; i <= ThreadData.TessV; i++) {
      if (ThreadData.obj -> KnotType(1) == NURBS_KV_Periodic) {
        NURBSBasis(OrderV, (OrderV-1)+i*double(ThreadData.Vnpts-(OrderV-1))/double(ThreadData.TessV), ThreadData.Vnpts, ThreadData.obj -> KnotVector(1), ThreadData.obj -> H(), ThreadData.BasisV[i], 1);
      } else if (ThreadData.obj -> KnotType(1) == NURBS_KV_Open) {
        NURBSBasis(OrderV, i*double(ThreadData.obj -> KnotVector(1)[ThreadData.Vnpts-1+OrderV])/double(ThreadData.TessV), ThreadData.Vnpts, ThreadData.obj -> KnotVector(1), ThreadData.obj -> H(), ThreadData.BasisV[i], 1);
      } //fi
    } //eof

  } else {

    for (i= 0; i <= ThreadData.TessU; i++) {
      if (ThreadData.obj -> KnotType(0) == NURBS_KV_Periodic) {
        NURBSBasis(OrderU, (OrderU-1)+i*double(ThreadData.Unpts-(OrderU-1))/double(ThreadData.TessU), ThreadData.Unpts, ThreadData.obj -> KnotVector(0), ThreadData.obj -> Hold(), ThreadData.BasisU[i], 1);
      } else if (ThreadData.obj -> KnotType(0) == NURBS_KV_Open) {
        NURBSBasis(OrderU, i*double(ThreadData.obj -> KnotVector(0)[0,ThreadData.Unpts-1+OrderU])/double(ThreadData.TessU), ThreadData.Unpts, ThreadData.obj -> KnotVector(0), ThreadData.obj -> H(), ThreadData.BasisU[i], 1);
      } //fi
    } //eof

    for (i= 0; i <= ThreadData.TessV; i++) {
      if (ThreadData.obj -> KnotType(1) == NURBS_KV_Periodic) {
        NURBSBasis(OrderV, (OrderV-1)+i*double(ThreadData.Vnpts-(OrderV-1))/double(ThreadData.TessV), ThreadData.Vnpts, ThreadData.obj -> KnotVector(1), ThreadData.obj -> Hold(), ThreadData.BasisV[i], 1);
      } else if (ThreadData.obj -> KnotType(1) == NURBS_KV_Open) {
        NURBSBasis(OrderV, i*double(ThreadData.obj -> KnotVector(1)[ThreadData.Vnpts-1+OrderV])/double(ThreadData.TessV), ThreadData.Vnpts, ThreadData.obj -> KnotVector(1), ThreadData.obj -> H(), ThreadData.BasisV[i], 1);
      } //fi
    } //eof
  } //fi

//  recalc= (Change != Unpts+Vnpts+OrderU+OrderV+Ucpts+Vcpts);
// Change= Unpts+Vnpts+OrderU+OrderV+Ucpts+Vcpts;

  // Launch Threads
  ThreadID= 0;

  ThreadData.Task[0].Lock= 0;
  ThreadData.Task[0].TessStart= 0;
  ThreadData.Task[0].TessStop= ThreadData.TessU/1;

  ThreadData.Task[1].Lock= 0;
  ThreadData.Task[1].TessStart= ThreadData.TessU/2+1;
  ThreadData.Task[1].TessStop= ThreadData.TessU;

  Threads[0]= SDL_CreateThread(TF,NULL);
//  Threads[1]= SDL_CreateThread(TF,NULL);
  SDL_WaitThread(Threads[0],NULL);
//  SDL_WaitThread(Threads[1],NULL);

  if (ThreadData.Update) {
    for (i= 0; i < ThreadData.Unpts*ThreadData.Vnpts; i++) {
      // Update Old with New
      ThreadData.obj -> Hold()[i]= ThreadData.obj -> H()[i];
      ThreadData.obj -> CtlPtsOld()[i]= ThreadData.obj -> CtlPts()[i];
    } //eof
  } //fi


  // Free the memory
  for (i= 0; i < ThreadData.TessU; i++)
    free(ThreadData.BasisU[i]);
  for (i= 0; i < ThreadData.TessV; i++)
    free(ThreadData.BasisV[i]);

  free(ThreadData.BasisU);
  free(ThreadData.BasisV);
} //eof NURBS_Generate::Surface()


void NURBS_Generate::TrimSurface(Object_NURBS *obj) {
  int		i,*TCIDList;

  // Activate all Pts
  for (i= 0; i < obj -> Tess(0) * obj -> Tess(1); i++)
    obj -> ActTssPts()[i]= 1;

  // Trimming falls into 2 Cases:
  // Case 1: Single trim curve touching 2 edges of domain
  // Case 2: Group of trim curves forming a loop

  TCIDList= (int*)malloc(sizeof(int)*obj -> TrimCount());
  obj -> TrimIDList(TCIDList);

//  printf("TCCount: %d\n",obj -> TrimCount());
  // For each NURBS Trim Curve
/*
  for (i= 0; i < obj -> TrimCount(); i++) {
    // Assume Case 1 for all Curves right now

    // Intersect Vertical Lines along U tesselation
    for (double j= 0; j <= 1.0; j+= 1.0/obj ->Tess(0)) {
      // For each Intersection of tesselation Line with Control Hull
      for (k
        // Determine point of intersection
        NURBSCurveIntersect(j,obj -> TrimCurve(TCIDList[i]),0.5,0.5,0);
    } //eof

    // Intersect Horizontal Lines along V tesselation

//    printf("CL: %d, CO: %d\n",obj -> TrimCurve(TCIDList[i]) -> Length(), obj -> TrimCurve(TCIDList[i]) -> Order());
  } //eof
*/
/*
  Point3d	test,CtlPts1[4],CtlPts2[4];
  double	KV[8],H[4];

  KV[0]= KV[1]= KV[2]= KV[3]= 0;
  KV[4]= KV[5]= KV[6]= KV[7]= 1;
  H[0]= H[1]= H[2]= H[3]= 1;
  CtlPts1[0].z= CtlPts1[1].z= CtlPts1[2].z= CtlPts1[3].z= 0;
  CtlPts1[0].x= 0;
  CtlPts1[0].y= 1;
  CtlPts1[1].x= 1;
  CtlPts1[1].y= 2;
  CtlPts1[2].x= 3;
  CtlPts1[2].y= 0;
  CtlPts1[3].x= 4;
  CtlPts1[3].y= 1;

  CtlPts2[0].z= CtlPts2[1].z= CtlPts2[2].z= CtlPts2[3].z= 0;
  CtlPts2[0].x= 1;
  CtlPts2[0].y= 0;
  CtlPts2[1].x= 1;
  CtlPts2[1].y= 2;
  CtlPts2[2].x= 3;
  CtlPts2[2].y= 2;
  CtlPts2[3].x= 3;
  CtlPts2[3].y= 0;

  NURBSCurveIntersect(4,4,CtlPts1,H,KV,4,4,CtlPts2,H,KV,1.0,1.0,0);

  double	j= 0;
  for (j= 0; j < 1.0; j+= 0.1) {
    test= NURBSCurvePt(4,4,CtlPts2,H,KV,j,1);
    printf("x[%.2f]: %.3f, y: %.3f\n",j,test.x,test.y);
  } //eof
*/
  free(TCIDList);
} //eof NURBS_Generate::TrimSurface()


void NURBS_Generate::Cyclic(Object_NURBS *obj) {
  int		i,n,k,l;
  Point3d	*temp;
  double	*Htemp;
  
  temp= (Point3d*)malloc(sizeof(Point3d)*10240);
  Htemp= (double*)malloc(sizeof(double)*10240);

  if (obj -> Cyclic(0) || obj -> Cyclic(1)) {
    k= 0;

    // Cyclic V (vertical)
    if (obj -> Cyclic(1)) {
      for (i= 0; i < obj -> Length(0); i++) {
        for (n= 0; n < obj -> Length(1); n++) {
          temp[k].x= obj -> CtlPts()[i*obj -> Length(1)+n].x;
          temp[k].y= obj -> CtlPts()[i*obj -> Length(1)+n].y;
          temp[k].z= obj -> CtlPts()[i*obj -> Length(1)+n].z;
          Htemp[k]= obj -> H()[i*obj -> Length(1)+n];
          k++;
        } //eof

        for (l= 0; l < obj -> Order(1)-1; l++) {
          temp[k].x= obj -> CtlPts()[l+i*obj -> Length(1)].x;
          temp[k].y= obj -> CtlPts()[l+i*obj -> Length(1)].y;
          temp[k].z= obj -> CtlPts()[l+i*obj -> Length(1)].z;
          Htemp[k]= obj -> H()[l+i*obj -> Length(1)];
          k++;
        } //eof
      } //eof
      obj -> Length(1,obj -> Length(1)+obj -> Order(1)-1);
    } else {
      for (i= 0; i < obj -> Length(0) * obj -> Length(1); i++) {
        temp[i].x= obj -> CtlPts()[i].x;
        temp[i].y= obj -> CtlPts()[i].y;
        temp[i].z= obj -> CtlPts()[i].z;
        Htemp[i]= obj -> H()[i];
        k++;
      } //eof
    } //fi

    if (obj -> Cyclic(0)) {
      //Cyclic U (horizontal)
      for (i= 0; i < obj -> Order(0)-1; i++) {
        for (n= 0; n < obj -> Length(1); n++) {
          temp[k].x= temp[n+i*obj -> Length(1)].x;
          temp[k].y= temp[n+i*obj -> Length(1)].y;
          temp[k].z= temp[n+i*obj -> Length(1)].z;
          Htemp[k]= Htemp[n+i*obj -> Length(1)];
          k++;
        } //eof
      } //eof
      obj -> Length(0,obj -> Length(0)+obj -> Order(0)-1);
    } //fi
  } //fi

  for (i= 0; i < obj -> Length(0) * obj -> Length(1); i++) {
    obj -> CtlPts()[i].x= temp[i].x;
    obj -> CtlPts()[i].y= temp[i].y;
    obj -> CtlPts()[i].z= temp[i].z;
    obj -> H()[i]= Htemp[i];
  } //eof

  free(temp);
  free(Htemp);
} //eof NURBS_Generate::Cyclic()


void NURBS_Generate::KnotVector(Object_NURBS *obj) {
  if (obj -> KnotType(0) == NURBS_KV_Periodic) {
    PeriodicKnot(obj -> Length(0),obj -> Order(0),obj -> KnotVector(0));
  } else if (obj -> KnotType(0) == NURBS_KV_Open) {
    OpenKnot(obj -> Length(0)-1,obj -> Order(0),obj -> KnotVector(0));
  } //fi

  if (obj -> KnotType(1) == NURBS_KV_Periodic) {
    PeriodicKnot(obj -> Length(1),obj -> Order(1),obj -> KnotVector(1));
  } else if (obj -> KnotType(1) == NURBS_KV_Open) {
    OpenKnot(obj -> Length(1)-1,obj -> Order(1),obj -> KnotVector(1));
  } //fi
} //eof NURBS_Generate::KnotVector()


void NURBS_Generate::Duplicate(Object_NURBS *obj1, Object_NURBS *obj2) {
  // Copy contents of obj2 into obj1
  int		i;

  obj1 -> SetLength(obj2 -> Length(0),obj2 -> Length(1));
  obj1 -> Length(0,obj2 -> Length(0));
  obj1 -> Length(1,obj2 -> Length(1));

  obj1 -> Order(0,obj2 -> Order(0));
  obj1 -> Order(1,obj2 -> Order(1));

  obj1 -> COR(obj2 -> COR());
  obj1 -> SIZ(obj2 -> SIZ());

  for (i= 0; i < 16; i++)
    obj1 -> MRM()[i]= obj2 -> MRM()[i];

  for (i= 0; i < obj1 -> Length(0) * obj1 -> Length(1); i++) {
    obj1 -> CtlPts()[i].x= obj2 -> CtlPts()[i].x;
    obj1 -> CtlPts()[i].y= obj2 -> CtlPts()[i].y;
    obj1 -> CtlPts()[i].z= obj2 -> CtlPts()[i].z;

    obj1 -> H()[i]= obj2 -> H()[i];

    obj1 -> WCCtlPts()[i].x= obj2 -> WCCtlPts()[i].x;
    obj1 -> WCCtlPts()[i].y= obj2 -> WCCtlPts()[i].y;
    obj1 -> WCCtlPts()[i].z= obj2 -> WCCtlPts()[i].z;
  } //eof

  for (i= 0; i < obj1 -> Length(0) + obj1 -> Order(0); i++)
    obj1 -> KnotVector(0)[i]= obj2 -> KnotVector(0)[i];
  
  for (i= 0; i < obj1 -> Length(1) + obj1 -> Order(1); i++)
    obj1 -> KnotVector(1)[i]= obj2 -> KnotVector(1)[i];

  for (i= 0; i < 4; i++)
    obj1 -> Edge(i, obj2 -> Edge(i));
} //eof NURBS_Generate::Duplicate()


//////////////////////
// Private Functions
//////////////////////

//////////////////////////////////////////////////////////////////////////////
// Solved Sep 15, 2002, Written Oct 05, 2002:
// NURBS Curve Intersections can be done by solving two general equations
// in two variables.  We can apply a 2D version of newton-rhapson to this
// problem by generating a pair of parametric equations using camer's rule
// that utilizes f(), g() and partial derivatives in x and y of each.
// The equation is:
// x1= x0 + (-f*gy+fy*g)/(fx*gy-fygx)
// y1= y0 + (-fx*g+f*gx)/(fx*gy-fygx)
// Since we're solving for roots, we want to know when the two curves equal 0,
// thus our f() and g() are:
// f(t1,t2)= q(t1)-r(t2)= 0;
// g(t1,t2)= s(t1)-t(t2)= 0;
//////////////////////////////////////////////////////////////////////////////

void NURBS_Generate::NURBSCurveIntersect(int NPts1, int Order1, Point3d *CtlPts1, double *H1, double *KV1, int NPts2, int Order2, Point3d *CtlPts2, double *H2, double *KV2, double s, double t, int iteration) {
  Point3d	CP1,CP2,CDP1,CDP2;
  double	si,ti;

  CP1= NURBSCurvePt(Order1, NPts1, CtlPts1, H1, KV1, s, 0);
  CP2= NURBSCurvePt(Order2, NPts2, CtlPts2, H2, KV2, t, 0);
  CDP1= NURBSCurvePt(Order1, NPts1, CtlPts1, H1, KV1, s, 1);
  CDP2= NURBSCurvePt(Order2, NPts2, CtlPts2, H2, KV2, t, 1);
//  printf("2: Pt.x= %.3f, Pt.y= %.3f, Pt.z= %.3f\n",CDP2.x,CDP2.y,CDP2.z);

  // Partial Derivative makes this term negative.
  CDP2.x*= -1;
  CDP2.y*= -1;

//  printf("[%d] : C1: %.4f,%.4f :: C2: %.4f,%.4f\n",iteration,CP1.x,CP1.y,CP2.x,CP2.y);

//  printf("si: %4f / %4f\n",(-(CP1.x-CP2.x)*CDP2.y + CDP1.y*(CP1.y-CP2.y)),(CDP1.x*CDP2.y - CDP1.y*CDP2.x));
  si= s + (-(CP1.x-CP2.x)*CDP2.y + CDP1.y*(CP1.y-CP2.y))/(CDP1.x*CDP2.y - CDP1.y*CDP2.x);
  ti= t + (-CDP1.x*(CP1.y-CP2.y) + (CP1.x-CP2.x)*CDP2.x)/(CDP1.x*CDP2.y - CDP1.y*CDP2.x);

  printf("s,t: %.8f,%.8f\n",si,ti);
  if (iteration < 10) {
    NURBSCurveIntersect(4,4,CtlPts1,H1,KV1,4,4,CtlPts2,H2,KV2,si,ti,iteration+1);
  } //fi
} //eof NURBSCurveIntersect()


void NURBS_Generate::NURBSCurveIntersect(double j, NURBS_TrimCurve *Curve, double s, double t, int iteration) {
  Point3d	CP1,CDP1;
  double	si,ti;


  CP1= NURBSCurvePt(Curve -> Order(), Curve -> Length(), Curve -> CtlPts(), Curve -> H(), Curve -> KV(), s, 0);
  CDP1= NURBSCurvePt(Curve -> Order(), Curve -> Length(), Curve -> CtlPts(), Curve -> H(), Curve -> KV(), s, 1);

  si= s + (-(CP1.x-j)*(-1) + CDP1.y*(CP1.y-t))/(CDP1.x*(-1));
  ti= t + (-CDP1.x*(CP1.y-t))/(CDP1.x*(-1));

  printf("j: %.3f ::: s,t: %.8f,%.8f ::: x,y: %.3f,%.3f\n",j,si,ti,CP1.x,CP1.y);
  if (iteration < 4) {
    NURBSCurveIntersect(j, Curve, si, ti, iteration+1);
  } //fi
} //eof NURBS_Generate::NURBSCurveIntersect()


Point3d	NURBS_Generate::NURBSCurvePt(int Order, int Npts, Point3d *CtlPts, double *H, double *KV, double t, bool D) {
  int		i;
  double	BasisResult,*Basis,*BasisDer,Sum,SumDer;
  Point3d	TessPt;

  // Allocate memory for Basis
  Basis= (double*)malloc(sizeof(double)*Npts);
  // Allicate memory for Basis Derivative
  if (D)
    BasisDer= (double*)malloc(sizeof(double)*Npts);

  // Assume Open Knot Vector
  NURBSBasis(Order,t,Npts,KV,H,Basis,0);
  if (D)
    NURBSBasisDer1(Order,t,Npts,KV,H,BasisDer,0);

  // Compute Sum Function for Basis
  Sum= SumDer= 0;
  for (i= 0; i < Npts; i++)
    Sum+= H[i]*Basis[i];
  if (D)
    for (i= 0; i < Npts; i++)
      SumDer+= H[i]*BasisDer[i];

  TessPt.x= TessPt.y= TessPt.z= 0;
  for (i= 0; i < Npts; i++) {
    BasisResult= D ? H[i]*BasisDer[i]/Sum - H[i]*Basis[i]*SumDer/(Sum*Sum) : H[i]*Basis[i]/Sum;

    TessPt.x+= CtlPts[i].x*BasisResult;
    TessPt.y+= CtlPts[i].y*BasisResult;
    TessPt.z+= CtlPts[i].z*BasisResult;
  } //eof

  free(Basis);
  if (D)
    free(BasisDer);
  return(TessPt);
} //eof NURBS_Generate::NURBSCurvePt()


void NURBS_Generate::PeriodicKnot(int npts, int Order, double *KnotVector) {
  //Calculate Knot Vector (Periodic)
  for (int i= 0; i < npts+Order; i++) KnotVector[i]= i;
} //eof NURBS_Generate::PeriodicKnot()


void NURBS_Generate::OpenKnot(int npts, int Order, double *KnotVector) {
  //Calculate Knot Vector (Open)
  KnotVector[0]= 0;
  for (int i= 1; i <= (npts+Order); i++) {
    if ((i >= Order) && (i < npts+2)) {
      KnotVector[i]= KnotVector[i-1]+1;
    } else {
      KnotVector[i]= KnotVector[i-1];
    } //fi
  } //eof
} //eof NURBS_Generate::OpenKnot()


void NURBS_Generate::NURBSBasis(int Order, double t, int npts, double *KnotVector, double *H, double *BasisValues, bool Type) {
  int		i,k;
  double	*temp,sum,d,e;

  // Allocate memory for temp
  temp= (double*)malloc(sizeof(double)*(npts+Order));

  for (i= 0; i < npts+Order; i++)
    temp[i]= (t >= KnotVector[i]) && (t < KnotVector[i+1]) ? 1 : 0;

  // Populate N(i,k)
  for (k= 1; k < Order; k++)
    for (i= 0; i < (npts+Order)-k; i++) {
      d= temp[i] ? ((t-KnotVector[i])*temp[i])/(KnotVector[i+k]-KnotVector[i]) : 0;
      e= temp[i+1] ? ((KnotVector[i+k+1]-t)*temp[i+1])/(KnotVector[i+k+1]-KnotVector[i+1]) : 0;
      temp[i]= d+e;
    } //eof

  if (t == KnotVector[npts+Order-1]) temp[npts-1]= 1;

  if (Type) {
    for (i= 0; i < npts; i++)
      BasisValues[i]= temp[i];
  } else {
    sum= 0;
    for (i= 0; i < npts; i++)
      sum+= temp[i]*H[i];
    for (i= 0; i < npts; i++)
      BasisValues[i]= sum ? (temp[i]*H[i])/sum : 0;
  } //fi

  free(temp);
} //eof NURBS_Generate::NURBSBasis()


void NURBS_Generate::NURBSBasisDer1(int Order, double t, int npts, double *KnotVector, double *H, double *BasisValues, bool Type) {
  int		i,k;
  double	*temp,*temp1,*Basis,d,e,sum;
  double	b1,b2,f1,f2,f3,f4;

  // Allocate memory for temp
  temp= (double*)malloc(sizeof(double)*(npts+Order));
  temp1= (double*)malloc(sizeof(double)*(npts+Order));

  // Allocate memory for Basis
  Basis= (double*)malloc(sizeof(double)*npts);

  // We require the Basis Values before calculating
  // the first derivative basis values
  NURBSBasis(Order,t,npts,KnotVector,H,Basis,Type);

  for (i= 0; i < npts+Order; i++) {
    temp[i]= (t >= KnotVector[i]) && (t < KnotVector[i+1]) ? 1 : 0;
    temp1[i]= 0;
  } //eof

  if (t == KnotVector[npts+Order-1]) temp[npts-1]= 1;

  // Populate N'(i,k)
  for (k= 1; k < Order; k++)
    for (i= 0; i < (npts+Order)-k; i++) {
      b1= temp[i] ? (t - KnotVector[i])*temp[i]/(KnotVector[i+k] - KnotVector[i]) : 0;
      b2= temp[i+1] ? (KnotVector[i+k+1] - t)*temp[i+1]/(KnotVector[i+k+1] - KnotVector[i+1]) : 0;


      f1= temp[i] ? temp[i]/(KnotVector[i+k]-KnotVector[i]) : 0;
      f2= temp[i+1] ? -temp[i+1]/(KnotVector[i+k+1]-KnotVector[i+1]) : 0;
      f3= temp1[i] ? (t - KnotVector[i])*temp1[i]/(KnotVector[i+k] - KnotVector[i]) : 0;
      f4= temp1[i+1] ? (KnotVector[i+k+1] - t)*temp1[i+1]/(KnotVector[i+k+1]-KnotVector[i+1]) : 0;

      temp[i]= b1+b2;
      temp1[i]= f1+f2+f3+f4;
//      printf("[%d]::: %f ::: %f ::: %f ::: %f\n",i,f1,f2,f3,f4);
    } //eof

  for (i= 0; i < npts; i++)
    BasisValues[i]= temp1[i];

  free(temp);
  free(temp1);
  free(Basis);
} //eof NURBS_Generate::NURBSBasisDer1()


double NURBS_Generate::Sum(double *H, double *BasisU, double *BasisV, int Unpts, int Vnpts, int ThreadID) {
  int		i[2],n[2],j[2];
  double	sum[2];

  sum[ThreadID]= 0;
  j[ThreadID]= 0;

  for (i[ThreadID]= 0; i[ThreadID] < Unpts; i[ThreadID]++) {
    for (n[ThreadID]= 0; n[ThreadID] < Vnpts; n[ThreadID]++) {
      sum[ThreadID]+= H[j[ThreadID]]*BasisU[i[ThreadID]]*BasisV[n[ThreadID]];
      j[ThreadID]++;
    } //eof
  } //eof
  return(sum[ThreadID]);
} //eof NURBS_Generate::Sum()


void NURBS_Generate::AssignThreadData(Object_NURBS *obj, int TessU, int TessV, Point3d *TssPts, double *Sum, int Index, bool Change, bool Update) {
  ThreadData.obj= obj;
  ThreadData.TessU= TessU;
  ThreadData.TessV= TessV;
  ThreadData.TssPts= TssPts;
  ThreadData.Sum= Sum;
  ThreadData.Index= Index;
  ThreadData.Change= Change;
  ThreadData.Update= Update;
} //eof NURBS_Generate::AssignThreaddata();
