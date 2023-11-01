/****************************
 * Open NURBS FileIO Class
 * Started: Apr 6, 2002
 * Justin Shumaker
 *
 * Description:
 * Open NURBS I/O Revsion 2
 ****************************/


#ifndef ON_IO_H
#define ON_IO_H

#include <stdio.h>
#include <stdlib.h>
#include <ON/opennurbs.h>

#include "../Objects/Object_Manager.h"
#include "../MaterialList.h"

class ON_IO {
  public:
    static	bool	write(char *Filename, Object_Manager *ObjectList, MaterialList *MatList);
    static	bool	read(char *Filename, Object_Manager *ObjectList, MaterialList *MatList);

  private:
    struct Material {
        int			Index;
        int			MatID;
    };

    static	void	CreateSurface(ON_NurbsSurface* srf, Object_Manager *OL, ON_3dmObjectAttributes Attributes, Material *Materials, int MatCount);
    static	void	CreateCurve(ON_NurbsCurve* crv, Object_Manager *OL, ON_3dmObjectAttributes Attributes, Material *Materials, int MatCount);
}; //eof class ON_IO
#endif
