/****************************
 * FileIO Class
 * Started: August 02, 2000
 * Lasted Modified: Jan 03, 2002
 * Justin Shumaker
 *
 * Description:
 * Main pipe for all File I/O
 ****************************/


#ifndef FileIO_H
#define FileIO_H

#include <stdio.h>
#include <stdlib.h>

#include "../Objects/Object_Manager.h"
#include "../MaterialList.h"

#include "NURBS_IO.h"
//#include "ON_IO.h"
#include "DXF_IO.h"

#define	NURBANA_FILETYPE_NURBS	0
#define	NURBANA_FILETYPE_3DM	1
#define	NURBANA_FILETYPE_DXFBIN	2
#define	NURBANA_FILETYPE_DXFASC	3
#define	NURBANA_FILETYPE_IGES	4
#define	NURBANA_FILETYPE_3DS	5

class FileIO {
  public:
    FileIO();
    ~FileIO();

    static	char*	DirectoryListing(void);		//Return the # of lines the
    static	bool 	UpdatePath(char *NewPath);	//Updates Current Path Info
    static	void	GetCwd(char *Path);
    static	int	OpenFile(char *Filename, Object_Manager *ObjectList, MaterialList *MatList, unsigned char FT);
    static	int	SaveFile(char *FileName, Object_Manager *ObjectList, MaterialList *MatList, unsigned char FT);

}; //eof class FileIO
#endif
