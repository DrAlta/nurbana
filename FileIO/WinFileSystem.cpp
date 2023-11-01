/***********************************
 * WinFileSystem Class
 ***********************************
 * Date: October 4, 2000
 * Author: Justin Shumaker
 * Email: justin@nurbana.cx
 ***********************************/

#include "WinFileSystem.h"
#include <io.h>
#include <direct.h>
#include <stdlib.h>
#include <string.h>


WinFileSystem::WinFileSystem() {
  mDir= 0;
//  _chdir("/");
} //eof WinFileSystem::UnixFileSystem()


WinFileSystem::~WinFileSystem() {
  if (mDir) _findclose(mDir);
} //eof WinFileSystem::UnixFileSystem()


bool WinFileSystem::SetPath(char *NewPath) {
  char OldPath[256];

  if (NewPath[strlen(NewPath)-1] != 47) {
    strcat(NewPath,"/");
  } //if last character != "/" add a "/"

  strcpy(OldPath,Path);
  strcpy(Path,NewPath);

  if (_chdir(Path) == -1) {
    strcpy(Path,OldPath);
    _chdir(Path);
    return(0);
  } else {
    if (mDir) {
      _findclose(mDir);
      mDir = 0;
    } //fi
    mDir= _findfirst("*",&mFile);
    return(1);
  } //fi
} //eof WinFileSystem::SetPath()


char* WinFileSystem::GetPath(void) {
  return(Path);
} //eof WinFileSystem::GetPath()


void WinFileSystem::GetCwd(char *Path) {
  strcpy(Path,getcwd(0,256));
} //eof WinFileSystem()


int WinFileSystem::Next(void) {
  if(_findnext(mDir,&mFile) == -1L) {
    return(0);
  } else {
    return(1);
  } //fi
} //eof WinFileSystem::Next()


char* WinFileSystem::GetFileName(void) {
  return(mFile.name);
} //eof WinFileSystem::GetFileName()


char* WinFileSystem::GetFileSizeS(void) {
  char Size[256];
  sprintf(Size,"%d",mFile.size);
  return(Size);
} //eof WinFileSystem::GetFileSize()


int WinFileSystem::GetFileSizeI(void) {
  return(mFile.size);
} //eof WinFileSystem::GetFileSize()
