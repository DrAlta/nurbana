/***********************************
 * Nurbana File I/O Class
 ***********************************
 * Author: Justin Shumaker
 * Email: justin@nurbana.cx
 ***********************************/


//#include "UnixFileSystem.h"
#include "WinFileSystem.h"

#include <stdio.h>
#include "FileIO.h"
#include "../OPEN_NURBS/opennurbs.h"
//#include <Rhino/rhinoio.h>

/***** GLOBALS *******************/
//UnixFileSystem		FileSystem;
WinFileSystem		FileSystem;
/*********************************/


FileIO::FileIO() {
} //eof FileIO::FileIO()


FileIO::~FileIO() {
} //eof FileIO::FileIO()


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
