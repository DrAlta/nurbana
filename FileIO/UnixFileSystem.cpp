/***********************************
 * UnixFileSystem Class
 ***********************************
 * Date: October 4, 2000
 * Author: Justin Shumaker
 * Email: justin@nurbana.cx
 ***********************************/

#include "UnixFileSystem.h"
#include <string.h>

UnixFileSystem::UnixFileSystem() {
  dirp= opendir("/");
} //eof UnixFileSystem::UnixFileSystem()


UnixFileSystem::~UnixFileSystem() {
  closedir(dirp);
} //eof UnixFileSyste::UnixFileSystem()


bool UnixFileSystem::SetPath(char *NewPath) {
  char OldPath[256];

  if (NewPath[strlen(NewPath)-1] != 47) {
    strcat(NewPath,"/");
  } //if last character != "/" add a "/"

  strcpy(OldPath,Path);
  strcpy(Path,NewPath);

  closedir(dirp);
  dirp= opendir(Path);

  if (dirp == NULL) {
    strcpy(Path,OldPath);
    dirp= opendir(Path);
    return(0);    
  } //fi
  return(1);
} //eof UnixFileSystem::SetPath()


char* UnixFileSystem::GetPath(void) {
  return(Path);
} //eof UnixFileSystem::GetPath()


void UnixFileSystem::GetCwd(char *Path) {
  strcpy(Path,getcwd(0,256));
} //eof UnixFileSystem()


int UnixFileSystem::Next(void) {
  direntp= readdir(dirp);
  if (direntp == NULL) {
    return(0);
  } else {
    return(1);
  } //fi
} //eof UnixFileSystem::Next()


char* UnixFileSystem::GetFileName(void) {
  return(direntp -> d_name);
} //eof UnixFileSystem::GetFileName()


char* UnixFileSystem::GetFileSizeS(void) {
  char Temp[256];
  sprintf(Temp,"%s%s",Path,direntp -> d_name);
  if (stat(Temp, &buf) != -1) {
    sprintf(Temp,"%d",buf.st_size);
    return(Temp);
  } else {
    return(0);
  } //fi
} //eof UnixFileSystem::GetFileSize()


int UnixFileSystem::GetFileSizeI(void) {
  char Temp[256];
  sprintf(Temp,"%s%s",Path,direntp -> d_name);
  if (stat(Temp, &buf) != -1) {
    return(buf.st_size);
  } else {
    return(0);
  } //fi
} //eof UnixFileSystem::GetFileSize()
