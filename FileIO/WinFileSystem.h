/****************************
 * WinFileSystem Class
 * October 4, 2000
 * Justin Shumaker
 *
 * Description:
 * Handles Unix File System
 *
 ****************************/

#include <stdio.h>
#include <io.h>

#ifndef WinFileSystem_H
#define WinFileSystem_H


class WinFileSystem {

  public:
    WinFileSystem();
    ~WinFileSystem();

    bool	SetPath(char *NewPath);
    char*	GetPath(void);
    void	GetCwd(char *path);

    int		Next(void);

    char*	GetFileName(void);
    char*	GetFileSizeS(void);
    int		GetFileSizeI(void);

  private:
    long	mDir;			// directory file descriptor
    struct	_finddata_t mFile;	// dile descriptor struct
    char	Path[256];

}; //eof class WinFileSystem
#endif
