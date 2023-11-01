/****************************
 * UnixFileSystem Class
 * October 4, 2000
 * Justin Shumaker
 *
 * Description:
 * Handles Unix File System
 *
 ****************************/

#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

#ifndef UnixFileSystem_H
#define UnixFileSystem_H


class UnixFileSystem {

  public:
    UnixFileSystem();
    ~UnixFileSystem();

    bool	SetPath(char *NewPath);
    char*	GetPath(void);
    void	GetCwd(char *path);

    int		Next(void);

    char*	GetFileName(void);
    char*	GetFileSizeS(void);
    int		GetFileSizeI(void);

  private:
    DIR* dirp;
    
    struct dirent* direntp;
    struct stat buf;

    char	Path[256];

}; //eof class UnixFileSystem
#endif
