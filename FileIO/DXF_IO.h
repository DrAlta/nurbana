/****************************
 * DXF FileIO Class
 * Started: Mar 17, 2002
 * Justin Shumaker
 *
 * Description:
 * DXF I/O functions
 ****************************/


#ifndef DXF_IO_H
#define DXF_IO_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../Objects/Object_Manager.h"
#include "../MaterialList.h"


class DXF_IO {
  public:
    static	bool	writebin(char *Filename, Object_Manager *ObjectList, MaterialList *MatList);
    static	bool	readbin(char *Filename, Object_Manager *ObjectList, MaterialList *MatList);

    static	bool	writeasc(char *Filename, Object_Manager *ObjectList, MaterialList *MatList);
    static	bool	readasc(char *Filename, Object_Manager *ObjectList, MaterialList *MatList);

}; //eof class DXF_IO
#endif
