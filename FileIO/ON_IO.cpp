#include "ON_IO.h"


bool ON_IO::write(char *Filename, Object_Manager *ObjectList, MaterialList *MatList) {
  unsigned		int	*ID_LIST,i,n,k;
  bool				UV;
  ON_Object			**objects;
  ON_NurbsSurface		**nurbs_surfaces;
  ON_NurbsCurve			**nurbs_curves;
  ON_3dmObjectAttributes	**attributes;
  ON_4dPoint			point;

  // Allocate Memory
  ID_LIST= (unsigned int*)malloc(sizeof(unsigned int)*ObjectList -> GetObjCount());
  objects= (ON_Object**)malloc(sizeof(ON_Object*)*ObjectList -> GetObjCount());
  nurbs_surfaces= (ON_NurbsSurface**)malloc(sizeof(ON_NurbsSurface*)*ObjectList -> GetObjCount());
  nurbs_curves= (ON_NurbsCurve**)malloc(sizeof(ON_NurbsCurve*)*ObjectList -> GetObjCount());
  attributes= (ON_3dmObjectAttributes**)malloc(sizeof(ON_3dmObjectAttributes*)*ObjectList -> GetObjCount());

  ObjectList -> IDList(ID_LIST);
  for (i= 0; i < ObjectList -> GetObjCount(); i++) {
    ObjectList -> SetActiveID(ID_LIST[i]);

    // Set Object Attributes
    attributes[i]= new ON_3dmObjectAttributes;
    attributes[i] -> m_name= ObjectList -> GetName();
    attributes[i] -> m_color= ON_Color(192,192,192);
    attributes[i] -> m_material_index= ObjectList -> GetMaterialID();

    if (ObjectList -> GetLength(0) == 1 || ObjectList -> GetLength(1) == 1) {
      // Create New Curve
      UV= (ObjectList -> GetLength(0) == 1);
      nurbs_curves[i]= new ON_NurbsCurve(3,1,ObjectList -> GetOrder(UV),ObjectList -> GetLength(UV));
      
      for (n= 0; n < nurbs_curves[i] -> KnotCount(); n++)
        nurbs_curves[i] -> SetKnot(n, ObjectList -> GetKnot(n+1,UV));

      for (n= 0; n < nurbs_curves[i] -> CVCount(); n++) {
        point.w= ObjectList -> GetH(n);
        point.x= point.w * ObjectList -> GetWCCP(n).x;
        point.y= point.w * ObjectList -> GetWCCP(n).y;
        point.z= point.w * ObjectList -> GetWCCP(n).z;
        nurbs_curves[i] -> SetCV(n,point);
      } //eof

      objects[i]= nurbs_curves[i];
    } else {
      // Create New Surface
      nurbs_surfaces[i]= new ON_NurbsSurface(3,1,ObjectList -> GetOrder(0),ObjectList -> GetOrder(1),ObjectList -> GetLength(0),ObjectList -> GetLength(1));

      // Set Knot Vectors
      for (n= 0; n < nurbs_surfaces[i] -> KnotCount(0); n++)
        nurbs_surfaces[i] -> SetKnot(0, n, ObjectList -> GetKnot(n+1,0));

      for (n= 0; n < nurbs_surfaces[i] -> KnotCount(1); n++)
        nurbs_surfaces[i] -> SetKnot(1, n, ObjectList -> GetKnot(n+1,1));

      // Set Control Points
      for (n= 0; n < nurbs_surfaces[i] -> CVCount(0); n++) {
        for (k= 0; k < nurbs_surfaces[i] -> CVCount(1); k++) {
          point.w= ObjectList -> GetH(n*nurbs_surfaces[i] -> CVCount(1) + k);
          point.x= point.w * ObjectList -> GetWCCP(n*nurbs_surfaces[i] -> CVCount(1) + k).x;
          point.y= point.w * ObjectList -> GetWCCP(n*nurbs_surfaces[i] -> CVCount(1) + k).y;
          point.z= point.w * ObjectList -> GetWCCP(n*nurbs_surfaces[i] -> CVCount(1) + k).z;
          nurbs_surfaces[i] -> SetCV(n,k,point);
        } //eof
      } //eof

//      printf("valid: %d\n",nurbs_surfaces[i] -> IsValid());
      objects[i]= nurbs_surfaces[i];
    } //fi
  } //eof



  if (ObjectList -> GetObjCount()) {
    FILE		*fp;

    fp= ON::OpenFile(Filename, "wb");
    ON_BinaryFile file(ON::write3dm,fp);

    BOOL ok= file.Write3dmStartSection(2,"Written by Nurbana 1.03.1 alpha" __DATE__);

    // Properties
    if (ok) {
      ON_3dmProperties		properties;

      properties.m_Notes.m_notes= ObjectList -> Notes();
      properties.m_Notes.m_bVisible= (properties.m_Notes.m_notes.Length() > 0);

      // Set revision history info
      properties.m_RevisionHistory.NewRevision();

      // Set Application info
      properties.m_Application.m_application_name= "Nurbana 1.03.1 alpha";
      properties.m_Application.m_application_URL= "http://www.nurbana.cx";
      properties.m_Application.m_application_details= "Open NURBS File saved via Nurbana";

      ok= file.Write3dmProperties( properties );
    } //fi

    // Settings
    if (ok) {
      ON_3dmSettings	default_settings;
      ok= file.Write3dmSettings( default_settings );
    } //fi

    // Bitmap
    if (ok && file.BeginWrite3dmBitmapTable()) {
      if (!file.EndWrite3dmBitmapTable())
        ok= 0;
    } //fi

    // Materials
    if (ok && file.BeginWrite3dmMaterialTable()) {
      ON_Material		Mat;
      unsigned		int	ID;

      if (MatList -> GetHead(ID)) {
        do {
          Mat.SetMaterialIndex(ID);
          Mat.SetMaterialName(MatList -> Name(ID));
          Mat.SetDiffuse(ON_Color(MatList -> Diff(ID)[0], MatList -> Diff(ID)[1], MatList -> Diff(ID)[2]));
          Mat.SetSpecular(ON_Color(MatList -> Spec(ID)[0], MatList -> Spec(ID)[1], MatList -> Spec(ID)[2]));
          Mat.SetShine(100);

          file.Write3dmMaterial(Mat);
        } while(MatList -> GetNext(ID));
      } //fi

      if (!file.EndWrite3dmMaterialTable())
        ok= FALSE;
    } //fi

    // Layer Table
    if (ok && file.BeginWrite3dmLayerTable()) {
      ON_Layer	default_layer;

      default_layer.SetLayerIndex(0);
      default_layer.SetLayerName("Default");

      ok= file.Write3dmLayer(default_layer);

      if (!file.EndWrite3dmLayerTable())
        ok= FALSE;
    } //fi

    // Group Table
    if (ok && file.BeginWrite3dmGroupTable()) {
      if (!file.EndWrite3dmGroupTable())
        ok= 0;
    } //fi

    // Light Table
    if (ok && file.BeginWrite3dmLightTable()) {
      if (!file.EndWrite3dmLightTable())
        ok= 0;
    } //fi

    // Object Table
    if (ok && file.BeginWrite3dmObjectTable()) {
      for (i= 0; ok && i < ObjectList -> GetObjCount(); i++)
        ok= file.Write3dmObject(*objects[i],attributes[i]);

      if (!file.EndWrite3dmObjectTable())
        ok= 0;
    } //fi

    // End of file marker
    if (!file.Write3dmEndMark())
      ok= FALSE;

    ON::CloseFile(fp);
  } //fi

  // Free Memory
  free(ID_LIST);
  for (i= 0; i < ObjectList -> GetObjCount(); i++) {
    free(objects[i]);
    free(nurbs_curves[i]);
    free(nurbs_surfaces[i]);
    free(attributes[i]);
  } //eof
  free(objects);
  free(nurbs_curves);
  free(nurbs_surfaces);
  free(attributes);

  return(1);
} //eof ON_IO::write()


bool ON_IO::read(char *Filename, Object_Manager *ObjectList, MaterialList *MatList) {
  ON_Object			*Object= NULL;
  ON_Geometry			*Geometry;
  ON_3dmObjectAttributes	Attributes;
  ON_String			Comments;
  ON_3dmProperties		Properties;
  ON_3dmSettings		Settings;
  Material			*Materials= NULL;
  FILE				*fp;
  int				i,n,MatCount;
  double			Domain[2];


  fp= ON::OpenFile(Filename,"rb");
  if (!fp) return(0);

  ON_BinaryFile file(ON::read3dm,fp);

  // Read Start Section
  if (!file.Read3dmStartSection(&i,Comments))
    return(0);
//  printf("Version: %d, Comments: %s\n",i,Comments);

  // Read Properties Section
  if (!file.Read3dmProperties(Properties))
    return(0);

  // Read Settings Section
  if (!file.Read3dmSettings(Settings))
    return(0);

  if (file.BeginRead3dmBitmapTable()) {
    if (!file.EndRead3dmBitmapTable())
      return(0);
  } //fi

  // Read Material Table
  if (file.BeginRead3dmMaterialTable()) {
    ON_Material		*Mat;
    char		*Text;

    Materials= (Material*)malloc(sizeof(Material));
    MatCount= 0;

    while (file.Read3dmMaterial(&Mat)) {
      Text= (char*)malloc(sizeof(char)*Mat -> MaterialName().Length());
      Materials= (Material*)realloc(Materials,sizeof(Material)*(MatCount+1));

      Materials[MatCount].Index= Mat -> MaterialIndex();
      for (i= 0; i < Mat -> MaterialName().Length(); i++)
        Text[i]= Mat -> MaterialName()[i];
      Text[i]= 0;
      if (!Text[0]) {
        Text= (char*)malloc(sizeof(char)*32);
        sprintf(Text,"Material.%d\n",MatCount+1);
      } //fi
      Materials[MatCount].MatID= MatList -> Create(Text);

      MatList -> Diff(Materials[MatCount].MatID)[0]= (unsigned char)Mat -> Diffuse().FractionRed()*255;
      MatList -> Diff(Materials[MatCount].MatID)[1]= (unsigned char)Mat -> Diffuse().FractionGreen()*255;
      MatList -> Diff(Materials[MatCount].MatID)[2]= (unsigned char)Mat -> Diffuse().FractionBlue()*255;

      MatList -> Spec(Materials[MatCount].MatID)[0]= (unsigned char)Mat -> Specular().FractionRed()*255;
      MatList -> Spec(Materials[MatCount].MatID)[1]= (unsigned char)Mat -> Specular().FractionGreen()*255;
      MatList -> Spec(Materials[MatCount].MatID)[2]= (unsigned char)Mat -> Specular().FractionBlue()*255;

      free(Text);
      MatCount++;
    } //eof

    if (!file.EndRead3dmMaterialTable())
      return(0);
  } //fi

  // Read Layer Table
  if (file.BeginRead3dmLayerTable()) {
    if (!file.EndRead3dmLayerTable())
      return(0);
  } //fi

  // Read Group Table
  if (file.BeginRead3dmGroupTable()) {
    if (!file.EndRead3dmGroupTable())
      return(0);
  } //fi

  // Read Light Table
  if (file.BeginRead3dmLightTable()) {
    if (!file.EndRead3dmLightTable())
      return(0);
  } //fi



  // Read Object Info
  if (file.BeginRead3dmObjectTable()) {

//    while(file.Read3dmObject(&Object,&Attributes,0)) {
    while (1) {
      i= file.Read3dmObject(&Object,&Attributes,0);
//      printf("code: %d\n",i);

      if (!i) break;

      // Get Geometry
      Geometry= ON_Geometry::Cast(Object);

      // Check if Curve
      if (ON_Curve::Cast(Geometry)) {
//        printf("--- Curve ---\n");
        if (ON_PolyCurve::Cast(ON_Curve::Cast(Geometry))) {
          for (i= 0; i < ON_PolyCurve::Cast(ON_Curve::Cast(Geometry)) -> Count(); i++)
            CreateCurve(ON_NurbsCurve::Cast(ON_PolyCurve::Cast(ON_Curve::Cast(Geometry)) -> SegmentCurve(i)), ObjectList, Attributes, Materials, MatCount);
        } else {
          CreateCurve(ON_NurbsCurve::Cast(ON_Curve::Cast(Geometry)), ObjectList, Attributes, Materials, MatCount);
        } //fi
      } //fi


      // Check if Surface
      if (ON_Surface::Cast(Geometry)) {
//        printf("--- Surface ---\n");
        CreateSurface(ON_NurbsSurface::Cast(ON_Surface::Cast(Geometry)), ObjectList, Attributes, Materials, MatCount);
      } //fi


      // Check if BREP
      if (ON_Brep::Cast(Geometry)) {
//        printf("--- BREP ---\n");
        // For now each surface, create a new one in Nurbana
//        for (i= 0; i < ON_Brep::Cast(Geometry) -> m_F.Count(); i++)
//          CreateSurface(ON_NurbsSurface::Cast(ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_S[ON_Brep::Cast(Geometry) -> m_F[i].m_si]), ObjectList, Attributes, Materials, MatCount);
        for (i= 0; i < ON_Brep::Cast(Geometry) -> m_F.Count(); i++) {
          // Create Surface
          CreateSurface(ON_NurbsSurface::Cast(ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_S[ON_Brep::Cast(Geometry) -> m_F[i].m_si]), ObjectList, Attributes, Materials, MatCount);

          // Attach Trims to Surface
          printf("Trims[%i]: %d\n",i,ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_T.Count());

          for (n= 0; n < ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_T.Count(); n++) {
            switch( ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_T[n].m_type ) {

               case ON_BrepTrim::unknown:
                 printf("  trim unknown: %d,%d\n",ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_T[n].m_vi[0],ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_T[n].m_vi[1]);
                 break;

               case ON_BrepTrim::boundary:
                 printf("  trim boundary: %d,%d\n",ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_T[n].m_vi[0],ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_T[n].m_vi[1]);
                 break;

               case ON_BrepTrim::mated:
                 printf("  trim mated: %d,%d\n",ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_T[n].m_vi[0],ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_T[n].m_vi[1]);
                 break;

               case ON_BrepTrim::seam:
                 printf("  trim seam: %d,%d\n",ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_T[n].m_vi[0],ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_T[n].m_vi[1]);
                 break;

               case ON_BrepTrim::singular:
                 printf("  trim singular: %d,%d\n",ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_T[n].m_vi[0],ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_T[n].m_vi[1]);
                 break;

               default:
                 break;
            } //eos

            // Attach Trim Curve
	    ON_4dPoint		Point;
            Point3d		*Pts;
            double		*H,*KV;

            // Get Parametric Domain of X and Y
            Domain[0]= ON_NurbsSurface::Cast(ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_S[ON_Brep::Cast(Geometry) -> m_F[i].m_si]) -> Domain(0).m_t[1];
            Domain[1]= ON_NurbsSurface::Cast(ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_S[ON_Brep::Cast(Geometry) -> m_F[i].m_si]) -> Domain(1).m_t[1];

            if (ON_NurbsCurve::Cast(ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_C2[ON_Brep::Cast(Geometry) -> m_T[n].m_c2i])) {
              ON_NurbsCurve	*NurbsCurve;

              // NURBS Curve Trim
              NurbsCurve= ON_NurbsCurve::Cast(ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_C2[ON_Brep::Cast(Geometry) -> m_T[n].m_c2i]);

              // Allocate Memory
              Pts= (Point3d*)malloc(sizeof(Point3d)*NurbsCurve -> CVCount());
              H= (double*)malloc(sizeof(double)*NurbsCurve -> CVCount());
              KV= (double*)malloc(sizeof(double)*(NurbsCurve -> KnotCount()+2));
//              printf("NURBS: Length: %d, Order: %d\n",ON_NurbsCurve::Cast(ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_C2[ON_Brep::Cast(Geometry) -> m_T[n].m_c2i]) -> CVCount(),ON_NurbsCurve::Cast(ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_C2[ON_Brep::Cast(Geometry) -> m_T[n].m_c2i]) -> Order());

              // Populate Point Array
              for (int j= 0; j < NurbsCurve -> CVCount(); j++) {
                NurbsCurve -> GetCV(j,Point);
                Pts[j].x= Point.x/Domain[0];
                Pts[j].y= Point.y/Domain[1];
                Pts[j].z= 0;
                H[j]= Point.w;
              } //eof

              // Knot Vector
              KV[0]= 0;
              for (j= 0; j < NurbsCurve -> KnotCount(); j++)
                KV[j+1]= (NurbsCurve -> Knot(j) - NurbsCurve -> Knot(0))/(NurbsCurve -> Knot(NurbsCurve -> KnotCount()-1)-NurbsCurve -> Knot(0));
              KV[j+1]= (NurbsCurve -> Knot(j-1) - NurbsCurve -> Knot(0))/(NurbsCurve -> Knot(NurbsCurve -> KnotCount()-1)-NurbsCurve -> Knot(0));

              // Populate KnotVector Array
              ((Object_NURBS*)ObjectList -> GetObj()) -> AttachCurve(NurbsCurve -> CVCount(),NurbsCurve -> Order());
              ((Object_NURBS*)ObjectList -> GetObj()) -> TrimCurve() -> CtlPts(Pts,H);
              ((Object_NURBS*)ObjectList -> GetObj()) -> TrimCurve() -> KV(KV);

              // Free Memory
              free(Pts);
              free(H);
              free(KV);
            } else if (ON_PolyCurve::Cast(ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_C2[ON_Brep::Cast(Geometry) -> m_T[n].m_c2i])) {
              ON_NurbsCurve	NurbsCurve;
              // Poly Curve Trim

              for (int j= 0; j < ON_PolyCurve::Cast(ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_C2[ON_Brep::Cast(Geometry) -> m_T[n].m_c2i]) -> Count(); j++) {
                ON_PolyCurve::Cast(ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_C2[ON_Brep::Cast(Geometry) -> m_T[n].m_c2i])  -> SegmentCurve(j) -> GetNurbForm(NurbsCurve,0,0);

//                printf("Poly: Length: %d, Order: %d\n", NurbsCurve.CVCount(),NurbsCurve.Order());
                // Allocate Memory
                Pts= (Point3d*)malloc(sizeof(Point3d)*NurbsCurve.CVCount());
                H= (double*)malloc(sizeof(double)*NurbsCurve.CVCount());
                KV= (double*)malloc(sizeof(double)*(NurbsCurve.KnotCount()+2));

                for (int z= 0; z < NurbsCurve.CVCount(); z++) {
                  NurbsCurve.GetCV(z,Point);
                  Pts[z].x= Point.x/Domain[0];
                  Pts[z].y= Point.y/Domain[1];
                  Pts[z].z= 0;
                  H[z]= Point.w;
                } //eof

                // Knot Vector
                KV[0]= 0;
                for (j= 0; j < NurbsCurve.KnotCount(); j++)
                  KV[j+1]= (NurbsCurve.Knot(j) - NurbsCurve.Knot(0))/(NurbsCurve.Knot(NurbsCurve.KnotCount()-1)-NurbsCurve.Knot(0));
                KV[j+1]= (NurbsCurve.Knot(j-1) - NurbsCurve.Knot(0))/(NurbsCurve.Knot(NurbsCurve.KnotCount()-1)-NurbsCurve.Knot(0));

//                for (z= 0; z < NurbsCurve.KnotCount()+2; z++)
//                  printf("KV[%d]: %f\n",z,KV[z]);
                // Populate KnotVector Array
                ((Object_NURBS*)ObjectList -> GetObj()) -> AttachCurve(NurbsCurve.CVCount(),NurbsCurve.Order());
                ((Object_NURBS*)ObjectList -> GetObj()) -> TrimCurve() -> CtlPts(Pts,H);
                ((Object_NURBS*)ObjectList -> GetObj()) -> TrimCurve() -> KV(KV);

                // Free Memory
                free(Pts);
                free(H);
                free(KV);
              } //eof
            } //fi

//          printf("    index: %d\n",ON_Brep::Cast(Geometry) -> m_T[n].m_c2i);
//          printf("span: %d\n",ON_PolyCurve::Cast(ON_Brep::Cast(Geometry) -> m_F[i].Brep() -> m_C2[ON_Brep::Cast(Geometry) -> m_T[n].m_c2i]) -> Count());

          } //eof
        } //eof
      } //fi
    } //eow

    file.EndRead3dmObjectTable();
  } //fi


  return(1);
} //eof ON_IO::read()


void ON_IO::CreateSurface(ON_NurbsSurface* srf, Object_Manager *OL, ON_3dmObjectAttributes Attributes, Material *Materials, int MatCount) {
  int		i,n;
  char		*ObjectName;
  ON_3dPoint	Point;
  Point3d	Pt;

  // Create Object
  OL -> Primitive(OBJECT_PRIMITIVE_CURVE);

  // Set Object Name
  ObjectName= (char*)malloc(sizeof(char)*(Attributes.m_name.Length()+1));
  for (n= 0; n < Attributes.m_name.Length(); n++)
    ObjectName[n]= Attributes.m_name[n];
  ObjectName[n]= 0;
  OL -> SetName(ObjectName);

  // Set Material
  for (i= 0; i < MatCount; i++)
    if (Materials[i].Index == Attributes.m_material_index)
      OL -> SetMaterialID(Materials[i].MatID);

  // Set Length and Order
  OL -> SetLength(0,srf -> CVCount(0));
  OL -> SetLength(1,srf -> CVCount(1));
  OL -> SetOrder(srf -> Order(0),0);
  OL -> SetOrder(srf -> Order(1),1);

  // Assign Points
  for (i= 0; i < OL -> GetLength(0); i++)
    for (n= 0; n < OL -> GetLength(1); n++) {
      srf -> GetCV(i,n,Point);
      Pt.x= Point.x;
      Pt.y= Point.y;
      Pt.z= Point.z;
      OL -> SetCP(i*OL -> GetLength(1)+n,Pt);
      OL -> SetH(i*OL -> GetLength(1)+n,srf -> Weight(i,n));
    } //eof

  // Assign Knot Vectors
  OL -> SetKnotType(0,!srf -> IsPeriodic(0));
  OL -> SetKnotType(1,!srf -> IsPeriodic(1));

  OL -> SetRecalcKV(0);

  // U Knot Vector
  OL -> SetKnot(0,0,srf -> SuperfluousKnot(0,0));
  for (i= 0; i < srf -> KnotCount(0); i++)
    OL -> SetKnot(i+1,0,srf -> Knot(0,i));
  OL -> SetKnot(i+1,0,srf -> SuperfluousKnot(0,1));

  // V Knot Vector
  OL -> SetKnot(0,1,srf -> SuperfluousKnot(1,0));
  for (i= 0; i < srf -> KnotCount(1); i++)
    OL -> SetKnot(i+1,1,srf -> Knot(1,i));
  OL -> SetKnot(i+1,1,srf -> SuperfluousKnot(1,1));

  // Center Points
  OL -> CenterCOR();
  OL -> CenterPoints();

  OL -> SetRecalcKV(0);
} //eof ON_IO::CreateSurface()


void ON_IO::CreateCurve(ON_NurbsCurve* crv, Object_Manager *OL, ON_3dmObjectAttributes Attributes, Material *Materials, int MatCount) {
  int		i;
  char		*ObjectName;
  ON_3dPoint	Point;
  Point3d	Pt;

  // Create Object
  OL -> Primitive(OBJECT_PRIMITIVE_CURVE);

  // Set Object Name
  ObjectName= (char*)malloc(sizeof(char)*(Attributes.m_name.Length()+1));
  for (i= 0; i < Attributes.m_name.Length(); i++)
    ObjectName[i]= Attributes.m_name[i];
  ObjectName[i]= 0;
  OL -> SetName(ObjectName);

  // Set Material
  for (i= 0; i < MatCount; i++)
    if (Materials[i].Index == Attributes.m_material_index)
      OL -> SetMaterialID(Materials[i].MatID);

  // Set Length and Order
  OL -> SetLength(0,crv -> CVCount());
  OL -> SetLength(1,1);
  OL -> SetOrder(crv -> Order(),0);
  OL -> SetOrder(2,1);

  // Assign Points
  for (i= 0; i < OL -> GetLength(0); i++) {
    crv -> GetCV(i,Point);
    Pt.x= Point.x;
    Pt.y= Point.y;
    Pt.z= Point.z;
    OL -> SetCP(i, Pt);
    OL -> SetH(i,crv -> Weight(i));
  } //eof

  // Assign Knot Vectors
  OL -> SetKnotType(0,!crv -> IsPeriodic());

  OL -> SetRecalcKV(0);

  // U Knot Vector
  OL -> SetKnot(0,0,crv -> Knot(0));
  for (i= 0; i < crv -> KnotCount(); i++)
    OL -> SetKnot(i+1,0,crv -> Knot(i));
  OL -> SetKnot(i+1,0,crv -> Knot(i-1));

  // V Dummy Knot Vector
  for (i= 0; i < 3; i++)
    OL -> SetKnot(i,1,0);

  // Center Points
  OL -> CenterCOR();
  OL -> CenterPoints();

  OL -> SetRecalcKV(0);
} //eof ON_IO::CreateCurve)