#include "NURBS_IO.h"


union endian {
  unsigned	int	i;
  unsigned	char	c;
};

struct Descriptor {
  unsigned	short	ID;
  unsigned	short	Version;
  unsigned	int	Length;
};


bool NURBS_IO::write(char *Filename, Object_Manager *ObjectList, MaterialList *MatList) {
  FILE			*FH= NULL;
  Descriptor		Block;
  endian		e;
  time_t		timer;
  fpos_t		FPos1,FPos2;
  char			*Text;
  unsigned	int	Num,Pos1,Pos2,*ID_LIST,i,n;
  bool			Num2;
  Point3d		P3D;
  double		Num3;

//  printf("FileName: %s\n",Filename);
  FH= fopen(Filename,"wb");

  // Write 1 byte Endian
  e.i= 1;
  fwrite(&e.c,sizeof(unsigned char),1,FH);

  // =========================
  // Descriptor 0 0 Scene Info
  // =========================
  fgetpos(FH,&FPos1);
  fseek(FH,sizeof(Descriptor),SEEK_CUR);
  Pos1= ftell(FH);

  // Version
  Text= (char*)malloc(sizeof(char)*32);
  strcpy(Text,"Nurbana 1.03.1a");
  fwrite(Text,sizeof(char),32,FH);
  free(Text);
  // Timestamp
  Text= (char*)malloc(sizeof(char)*26);
  timer= time(NULL);
  strcpy(Text,asctime(localtime(&timer)));
  Text[strlen(Text)-1]= 0;
  fwrite(Text,sizeof(char),25,FH);
  free(Text);
  // Scene Notes
  Text= (char*)malloc(sizeof(char)*(strlen(ObjectList -> Notes())+1));
  strcpy(Text,ObjectList -> Notes());
  fwrite(&(Num=strlen(ObjectList -> Notes())+1),sizeof(unsigned int),1,FH);
  fwrite(Text,sizeof(char),(strlen(ObjectList -> Notes())+1),FH);
  free(Text);

  Pos2= ftell(FH);
  fgetpos(FH,&FPos2);
  fsetpos(FH,&FPos1);

  Block.ID= 0;
  Block.Version= 0;
  Block.Length= Pos2-Pos1;
  fwrite(&Block,sizeof(Descriptor),1,FH);

  fsetpos(FH,&FPos2);

  // =========================
  // Descriptor 1 0 Model Info
  // =========================
  ID_LIST= (unsigned int*)malloc(sizeof(unsigned int)*ObjectList -> GetObjCount());
  ObjectList -> IDList(ID_LIST);
  for (i= 0; i < ObjectList -> GetObjCount(); i++) {
    fgetpos(FH,&FPos1);
    fseek(FH,sizeof(Descriptor),SEEK_CUR);
    Pos1= ftell(FH);

    fwrite(&ID_LIST[i],sizeof(unsigned int),1,FH);
    fwrite(&(Num= ObjectList -> GetMaterialID()),sizeof(unsigned int),1,FH);
    fwrite(&(Num= ObjectList -> GetOrder(0)-1),sizeof(unsigned int),1,FH);
    fwrite(&(Num= ObjectList -> GetOrder(1)-1),sizeof(unsigned int),1,FH);
    fwrite(&(Num= ObjectList -> GetLength(0)),sizeof(unsigned int),1,FH);
    fwrite(&(Num= ObjectList -> GetLength(1)),sizeof(unsigned int),1,FH);
    for (n= 0; n < ObjectList -> GetLength(0) * ObjectList -> GetLength(1); n++) {
      fwrite(&(P3D= ObjectList -> GetCP(n)),sizeof(Point3d),1,FH);
      fwrite(&(Num3= ObjectList -> GetH(n)),sizeof(double),1,FH);
    } //eof
    fwrite(&(Num2= ObjectList -> GetActive()),sizeof(bool),1,FH);
    fwrite(ObjectList -> GetName(),sizeof(char),64,FH);
    fwrite(&(P3D= ObjectList -> GetCOR()),sizeof(Point3d),1,FH);
    fwrite(&(P3D= ObjectList -> GetROT()),sizeof(Point3d),1,FH);
    fwrite(&(P3D= ObjectList -> GetSIZ()),sizeof(Point3d),1,FH);
    fwrite(ObjectList -> GetMRM(),sizeof(double),16,FH);
    fwrite(&(Num2= ObjectList -> GetKnotType(0)),sizeof(bool),1,FH);
    fwrite(&(Num2= ObjectList -> GetKnotType(1)),sizeof(bool),1,FH);
    for (n= 0; n < ObjectList -> GetLength(0) + ObjectList -> GetOrder(0); n++)
      fwrite(&(Num3= ObjectList -> GetKnot(n,0)), sizeof(double), 1, FH);
    for (n= 0; n < ObjectList -> GetLength(1) + ObjectList -> GetOrder(1); n++)
      fwrite(&(Num3= ObjectList -> GetKnot(n,1)), sizeof(double), 1, FH);

    Pos2= ftell(FH);
    fgetpos(FH,&FPos2);
    fsetpos(FH,&FPos1);

    Block.ID= 1;
    Block.Version= 0;
    Block.Length= Pos2-Pos1;
    fwrite(&Block,sizeof(Descriptor),1,FH);

    fsetpos(FH,&FPos2);
  } //eof


  // ============================
  // Descriptor 2 0 Material Info
  // ============================
  if (MatList -> GetHead(n)) {
    do {
      fgetpos(FH,&FPos1);
      fseek(FH,sizeof(Descriptor),SEEK_CUR);
      Pos1= ftell(FH);

      // Write ID
      fwrite(&n,sizeof(unsigned int),1,FH);
      // Write Name
      Text= (char*)malloc(sizeof(char)*64);
      MatList -> GetName(n,Text);
      fwrite(Text,sizeof(char),64,FH);
      free(Text);
      // Write Diffuse
      fwrite(MatList -> Diff(n),sizeof(unsigned char),3,FH);
      // Write Specular
      fwrite(MatList -> Spec(n),sizeof(unsigned char),3,FH);

      Pos2= ftell(FH);
      fgetpos(FH,&FPos2);
      fsetpos(FH,&FPos1);

      Block.ID= 2;
      Block.Version= 0;
      Block.Length= Pos2-Pos1;
      fwrite(&Block,sizeof(Descriptor),1,FH);

      fsetpos(FH,&FPos2);
//      printf("Pos: %d\n",Pos2-Pos1);
    } while (MatList -> GetNext(n));
  } //fi

  fclose(FH);
  return(1);
} //eof write()


bool NURBS_IO::read(char *Filename, Object_Manager *ObjectList, MaterialList *MatList) {
  FILE			*FH= NULL;
  Descriptor		Block;
  unsigned	char	ec,MAT[3];
  char			*Text;
  unsigned	int	Num,ID;
  bool			Num2,convert;
  Point3d		P3D;
  int			i,n;
  double		Num3;
  double		MRM[16];
  endian		e;

  FH= fopen(Filename,"rb");
  e.i= 1;

  // Retreive Endian
  fread(&ec,sizeof(unsigned char),1,FH);
  convert= (ec != e.c);
//  printf("Convert: %d\n",convert);

  while (!feof(FH)) {
    fread(&Block,sizeof(Descriptor),1,FH);
    if (convert) {
      Endian::RB(Block.ID);
      Endian::RB(Block.Version);
      Endian::RB(Block.Length);
    } //fi

    if (feof(FH)) break;
//    printf("Block: %d,%d,%d\n",Block.ID,Block.Version,Block.Length);
    switch( Block.ID ) {
      case 0:
        // =========================
        // Descriptor 0 0 Scene Info
        // =========================
        // Version
        Text= (char*)malloc(sizeof(char)*32);
        fread(Text,sizeof(char),32,FH);
//        printf("Version: %s\n",Text);
        free(Text);
        // Timestamp
        Text= (char*)malloc(sizeof(char)*26);
        fread(Text,sizeof(char),25,FH);
//        printf("Timestamp: -%s-\n",Text);
        free(Text);
        // Notes
        fread(&Num,sizeof(unsigned int),1,FH);
        if (convert) Endian::RB(Num);
        Text= (char*)malloc(sizeof(char)*Num);
        fread(Text,sizeof(char),Num,FH);
        ObjectList -> Notes(Text);
//        printf("Notes: %s\n",Text);
        free(Text);
        break;

      case 1:
        // =========================
        // Descriptor 1 0 Model Info
        // =========================
        fread(&Num,sizeof(unsigned int),1,FH);
        if (convert) Endian::RB(Num);
        ObjectList -> Primitive(OBJECT_PRIMITIVE_CURVE);
        fread(&Num,sizeof(unsigned int),1,FH);
        if (convert) Endian::RB(Num);
        ObjectList -> SetMaterialID(Num);
        fread(&Num,sizeof(unsigned int),1,FH);
        if (convert) Endian::RB(Num);
        ObjectList -> SetOrder(Num+1,0);
        fread(&Num,sizeof(unsigned int),1,FH);
        if (convert) Endian::RB(Num);
        ObjectList -> SetOrder(Num+1,1);
        fread(&Num,sizeof(unsigned int),1,FH);
        if (convert) Endian::RB(Num);
        ObjectList -> SetLength(0,Num);
        fread(&Num,sizeof(unsigned int),1,FH);
        if (convert) Endian::RB(Num);
        ObjectList -> SetLength(1,Num);

        for (n= 0; n < ObjectList -> GetLength(0) * ObjectList -> GetLength(1); n++) {
          fread(&P3D,sizeof(Point3d),1,FH);
          if (convert) {
            Endian::RB(P3D.x);
            Endian::RB(P3D.y);
            Endian::RB(P3D.z);
          } //fi
          ObjectList -> SetCP(n,P3D);
          fread(&Num3,sizeof(double),1,FH);
          if (convert) Endian::RB(Num3);
          ObjectList -> SetH(n,Num3);
        } //eof

        fread(&Num2,sizeof(bool),1,FH);
        if (convert) Endian::RB(Num2);
        ObjectList -> SetActive(Num2,1);

        Text= (char*)malloc(sizeof(char)*64);
        fread(Text,sizeof(char),64,FH);
        ObjectList -> SetName(Text);
        free(Text);

        fread(&P3D,sizeof(Point3d),1,FH);
        if (convert) {
          Endian::RB(P3D.x);
          Endian::RB(P3D.y);
          Endian::RB(P3D.z);
        } //fi
        ObjectList -> SetCOR(P3D);
        fread(&P3D,sizeof(Point3d),1,FH);
        if (convert) {
          Endian::RB(P3D.x);
          Endian::RB(P3D.y);
          Endian::RB(P3D.z);
        } //fi
        ObjectList -> SetROT(P3D);
        fread(&P3D,sizeof(Point3d),1,FH);
        if (convert) {
          Endian::RB(P3D.x);
          Endian::RB(P3D.y);
          Endian::RB(P3D.z);
        } //fi
        ObjectList -> SetSIZ(P3D);
        fread(&MRM,sizeof(double),16,FH);
        if (convert)
          for (i= 0; i < 16; i++) Endian::RB(MRM[i]);
        ObjectList -> SetMRM(MRM);

        fread(&Num2,sizeof(bool),1,FH);
        if (convert) Endian::RB(Num2);
        ObjectList -> SetKnotType(0,Num2);
        fread(&Num2,sizeof(bool),1,FH);
        if (convert) Endian::RB(Num2);
        ObjectList -> SetKnotType(1,Num2);

        for (n= 0; n < ObjectList -> GetLength(0) + ObjectList -> GetOrder(0); n++) {
          fread(&Num3,sizeof(double),1,FH);
          if (convert) Endian::RB(Num3);
          ObjectList -> SetKnot(n,0,Num3);
        } //eof

        for (n= 0; n < ObjectList -> GetLength(1) + ObjectList -> GetOrder(1); n++) {
          fread(&Num3,sizeof(double),1,FH);
          if (convert) Endian::RB(Num3);
          ObjectList -> SetKnot(n,1,Num3);
        } //eof

        // Do NOT recalculate Knot Vector
        ObjectList -> SetRecalcKV(0);
        break;

      case 2:
        // Material Info
        fread(&Num,sizeof(unsigned int),1,FH);
        if (convert) Endian::RB(Num);
        Text= (char*)malloc(sizeof(char)*64);
        fread(Text,sizeof(char),64,FH);
        MatList -> Create(Num,Text);
        free(Text);
        fread(MAT,sizeof(unsigned char),3,FH);
        MatList -> Diff(Num)[0]= MAT[0];
        MatList -> Diff(Num)[1]= MAT[1];
        MatList -> Diff(Num)[2]= MAT[2];
        fread(MAT,sizeof(unsigned char),3,FH);
        MatList -> Spec(Num)[0]= MAT[0];
        MatList -> Spec(Num)[1]= MAT[1];
        MatList -> Spec(Num)[2]= MAT[2];
        break;

      default:
        printf("Unrecognized Block ID: %d\n",Block.ID);
        fseek(FH,Block.Length,SEEK_CUR);
        break;
    } //eos
  } //eof



  fclose(FH);
  return(1);
} //eof FT_NURBS::read()
