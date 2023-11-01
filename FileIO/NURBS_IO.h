/****************************
 * NURBS FileIO Class
 * Started: Feb 17, 2002
 * Justin Shumaker
 *
 * Description:
 * NURBS I/O Functions
 ****************************/


#ifndef NURBS_IO_H
#define NURBS_IO_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../Objects/Object_Manager.h"
#include "../MaterialList.h"
#include "../Endian.h"

class NURBS_IO {
  public:
    static	bool	write(char *Filename, Object_Manager *ObjectList, MaterialList *MatList);
    static	bool	read(char *Filename, Object_Manager *ObjectList, MaterialList *MatList);

}; //eof class NURBS_IO
#endif
