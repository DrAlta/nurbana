/****************************************************
 * World Coordinate Calculation Utility Class
 * Start Date: Mar 09, 2002
 * Author: Justin Shumaker
 *
 * Description:
 * Contains 3 functions for calculating just Control
 * Points, or Tesselation Points, or Both at once
 *****************************************************/


#ifndef Object_WC_H
#define Object_WC_H

#include "Object_Base.h"
#include "../NurbanaMath.h"
#include "../NDefs/NGlobal.h"


class Object_WC {
  public:
    static	void	CtlPts(Object_Base *obj);
    static	void	TssPts(Object_Base *obj);
    static	void	IPLPts(Object_Base *obj);
    static	void	AllPts(Object_Base *obj);

}; //eof class Object_WC
#endif
