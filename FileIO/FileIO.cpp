/***********************************
 * Nurbana File I/O Class
 ***********************************
 * Author: Justin Shumaker
 * Email: justin@nurbana.cx
 ***********************************/


#include <stdio.h>
#include "FileIO.h"

#include "UnixFileSystem.h"
//#include "WinFileSystem.h"

/***** GLOBALS *******************/
UnixFileSystem		FileSystem;
//WinFileSystem		FileSystem;
/*********************************/


FileIO::FileIO() {
} //eof FileIO::FileIO()


FileIO::~FileIO() {
} //eof FileIO::~FileIO()


char* FileIO::DirectoryListing(void) {  
  if (FileSystem.Next()) {
    return(FileSystem.GetFileName());
//    strcpy(TextA,FileSystem.GetFileName());
//    strcpy(TextB,FileSystem.GetFileSizeS());
  } else {
    return(0);
  } //fi
} //eof FileIO::DirectoryListing()


bool FileIO::UpdatePath(char *NewPath) {
  return(FileSystem.SetPath(NewPath));
} //eof FileIO::UpdatePath()


void FileIO::GetCwd(char *Path) {
  FileSystem.GetCwd(Path);
} //eof FileIO::GetCwd()


int FileIO::OpenFile(char *Filename, Object_Manager *ObjectList, MaterialList *MatList, unsigned char FT) {
  switch( FT ) {
    case NURBANA_FILETYPE_NURBS:
      NURBS_IO::read(Filename,ObjectList,MatList);
      break;

    case NURBANA_FILETYPE_3DM:
//      ON_IO::read(Filename,ObjectList,MatList);
      break;

    case NURBANA_FILETYPE_DXFASC:
      DXF_IO::readasc(Filename,ObjectList,MatList);
      break;

    default:
      break;
  } //eos

  return(0);
} //eof FileIO::OpenFile()


int FileIO::SaveFile(char *Filename, Object_Manager *ObjectList, MaterialList *MatList, unsigned char FT) {
  switch( FT ) {
    case NURBANA_FILETYPE_NURBS:
      NURBS_IO::write(Filename,ObjectList,MatList);
      break;

    case NURBANA_FILETYPE_3DM:
//      ON_IO::write(Filename,ObjectList,MatList);
      break;

    case NURBANA_FILETYPE_DXFASC:
      DXF_IO::writeasc(Filename,ObjectList,MatList);
      break;

    default:
      break;
  } //eos

  return(0);
} //eof FileIO::SaveFile()
