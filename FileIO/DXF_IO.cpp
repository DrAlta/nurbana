#include "DXF_IO.h"


bool DXF_IO::writebin(char *Filename, Object_Manager *ObjectList, MaterialList *MatList) {
  FILE		*FH= NULL;
  char		*text;

  printf("DXF_Write: %s\n",Filename);
  FH= fopen(Filename,"wb");

  // Binary DXF Signature
  text= (char*)malloc(sizeof(char)*22);
  strcpy(text,"AutoCAD Binary DXF");
  text[18]= 13;
  text[19]= 10;
  text[20]= 26;
  text[21]= 0;
  fwrite(text,sizeof(char),22,FH);
  free(text);


  fclose(FH);

  return(1);
} //eof DXF_IO::write()


bool DXF_IO::readbin(char *Filename, Object_Manager *ObjectList, MaterialList *MatList) {
  FILE		*FH= NULL;
  char		*text;

  FH= fopen(Filename,"wb");

  text= (char*)malloc(sizeof(char)*22);
  fread(text,sizeof(char),22,FH);
  printf("text: %s\n",text);
  free(text);


  fclose(FH);
  return(1);
} //eof DXF_IO::read()


void writeln(char *text, FILE *FH) {
  fwrite(text,sizeof(char),strlen(text),FH);
} //eof writeln()


bool DXF_IO::writeasc(char *Filename, Object_Manager *ObjectList, MaterialList *MatList) {
  FILE			*FH= NULL;
  unsigned	int	*ID_LIST;
  int			i,n;
  char			Value[24];

  FH= fopen(Filename,"w");

  // Header
  writeln("0\n",FH);
  writeln("SECTION\n",FH);
  writeln("2\n",FH);
  writeln("HEADER\n",FH);
  writeln("0\n",FH);
  writeln("ENDSEC\n",FH);
//  writeln("999\n",FH);
//  writeln("Created with Nurbana\n",FH);
  writeln("0\n",FH);
  writeln("SECTION\n",FH);
  writeln("2\n",FH);
  writeln("BLOCKS\n",FH);
  writeln("0\n",FH);

  // Models Vertex Data
  ID_LIST= (unsigned int*)malloc(sizeof(unsigned int)*ObjectList -> GetObjCount());
  ObjectList -> IDList(ID_LIST);
  for (i= 0; i < ObjectList -> GetObjCount(); i++) {
    printf("OBJ: %d\n",i);

    writeln("BLOCK\n",FH);
    writeln("2\n",FH);
    sprintf(Value,"Plane_%d\n",i+1);
    writeln(Value,FH);
    writeln("8\n",FH);
    writeln("Meshes\n",FH);
    writeln("70\n",FH);
    writeln("64\n",FH);
    writeln("10\n",FH);
    writeln("0.0\n",FH);
    writeln("20\n",FH);
    writeln("0.0\n",FH);
    writeln("30\n",FH);
    writeln("0.0\n",FH);
    writeln("3\n",FH);
    sprintf(Value,"Plane_%d\n",i+1);
    writeln(Value,FH);
    writeln("0\n",FH);
    writeln("POLYLINE\n",FH);
    writeln("66\n",FH);
    writeln("1\n",FH);
    writeln("8\n",FH);

    writeln("Meshes\n",FH);
    writeln("70\n",FH);
    writeln("64\n",FH);
    writeln("71\n",FH);
    writeln("4\n",FH);
    writeln("72\n",FH);
    writeln("1\n",FH);
    writeln("0\n",FH);

    for (n= 0; n < ObjectList -> GetTess(0) * ObjectList -> GetTess(1); n++) {
      // 2 3
      // 1 4

      writeln("VERTEX\n",FH);
      writeln("8\n",FH);
      writeln("Meshes\n",FH);
      writeln("10\n",FH);
      sprintf(Value,"%.6f\n",ObjectList -> GetWCTess()[n].x - ObjectList -> GetCOR().x);
      writeln(Value,FH);
      writeln("20\n",FH);
      sprintf(Value,"%.6f\n",ObjectList -> GetWCTess()[n].y - ObjectList -> GetCOR().y);
      writeln(Value,FH);
      writeln("30\n",FH);
      sprintf(Value,"%.6f\n",ObjectList -> GetWCTess()[n].z - ObjectList -> GetCOR().z);
      writeln(Value,FH);
      writeln("70\n",FH);
      writeln("192\n",FH);
      writeln("0\n",FH);
    } //eof

    // Describe Faces
    for (n= 0; n < (ObjectList -> GetTess(0)) * (ObjectList -> GetTess(1)-1); n++) {
      writeln("VERTEX\n",FH);
      writeln("8\n",FH);
      writeln("Meshes\n",FH);
      writeln("62\n",FH);
      writeln("254\n",FH);
      writeln("10\n",FH);
      writeln("0.0\n",FH);
      writeln("20\n",FH);
      writeln("0.0\n",FH);
      writeln("30\n",FH);
      writeln("0.0\n",FH);
      writeln("70\n",FH);
      writeln("128\n",FH);
      writeln("71\n",FH);
      sprintf(Value,"%d\n",n+1);
      writeln(Value,FH);
      writeln("72\n",FH);
      sprintf(Value,"%d\n",n+2);
      writeln(Value,FH);
      writeln("73\n",FH);
      sprintf(Value,"%d\n",n+2+ObjectList -> GetTess(0));
      writeln(Value,FH);
      writeln("74\n",FH);
      sprintf(Value,"%d\n",n+1+ObjectList -> GetTess(1));
      writeln(Value,FH);
      writeln("0\n",FH);

      if (!((n+2) % ObjectList -> GetTess(1))) n+= 1;
    } //eof

    writeln("SEQEND\n",FH);
    writeln("0\n",FH);
    writeln("ENDBLK\n",FH);
    writeln("0\n",FH);
  } //eof



  // BREAK into COR/SIZE info
//  writeln("SEQEND\n",FH);
//  writeln("0\n",FH);
//  writeln("ENDBLK\n",FH);
//  writeln("0\n",FH);
  writeln("ENDSEC\n",FH);
  writeln("0\n",FH);
  writeln("SECTION\n",FH);
  writeln("2\n",FH);
  writeln("ENTITIES\n",FH);
  writeln("0\n",FH);

  for (i= ObjectList -> GetObjCount() - 1; i >= 0; i--) {
    printf("OBJ: %d\n",i);

    writeln("INSERT\n",FH);
    writeln("8\n",FH);
    writeln("1\n",FH);
    writeln("2\n",FH);
    sprintf(Value,"Plane_%d\n",i+1);
    writeln(Value,FH);
    writeln("10\n",FH);
    sprintf(Value,"%.6f\n",ObjectList -> GetCOR().x);
    writeln(Value,FH);
    writeln("20\n",FH);
    sprintf(Value,"%.6f\n",ObjectList -> GetCOR().y);
    writeln(Value,FH);
    writeln("30\n",FH);
    sprintf(Value,"%.6f\n",ObjectList -> GetCOR().z);
    writeln(Value,FH);
    writeln("41\n",FH);
    writeln("1.000000\n",FH);
    writeln("42\n",FH);
    writeln("1.000000\n",FH);
    writeln("43\n",FH);
    writeln("1.000000\n",FH);
    writeln("50\n",FH);
    writeln("0.000000\n",FH);
    writeln("0\n",FH);
  } //eof

  writeln("ENDSEC\n",FH);
  writeln("0\n",FH);
  writeln("EOF\n",FH);

  fclose(FH);

  return(1);
} //eof DXF_IO::writeasc()


bool DXF_IO::readasc(char *Filename, Object_Manager *ObjectList, MaterialList *MatList) {
  printf("filename: %s\n",Filename);


  return(1);
} //eof DXF_IO::readasc()
