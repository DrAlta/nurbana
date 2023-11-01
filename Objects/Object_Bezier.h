/****************************************************
 * Object - Bezier class
 * Start Date: Jan 28, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Takes care of Bezier specific information and
 * functions.
 ****************************************************/


#ifndef Object_Bezier_H
#define Object_Bezier_H


#include "Object_Base.h"
#include "Bezier.h"

class Object_Bezier : public Object_Base {
  public:
    Object_Bezier(Object_List *obj, bool CSType);
    ~Object_Bezier() {}

    int		GetLength(char Type, bool UV);
    void	SetLength(char Type, int U, int V);

    void	CalculatePts();

}; //eof class Object_Bezier
#endif
