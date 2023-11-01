/****************************************************
 * Nurbana General Utilities Class
 * Start Date: Jul 29, 2000
 * Author: Justin Shumaker
 *
 * Description:
 * Miscellaneous General Utilities
 *****************************************************/


#ifndef NGUtils_H
#define NGUtils_H

#include <stdlib.h>

class NGUtils {
  public:
    static	bool	Inside(int ApX, int ApY, int BpX, int BpY, int BsX, int BsY);
    static	bool	BoxInside(int ApX, int ApY, int AsX, int AsY, int BpX, int BpY, int BsX, int BsY);

}; //eof class NGUtils
#endif
