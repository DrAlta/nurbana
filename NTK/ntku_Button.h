/****************************************************
 * NTK Utility - Button Drawer
 * Start Date: Nov 6, 2000
 * Author: Justin Shumaker
 *
 * Description:
 * Utility class for drawing buttons
 *****************************************************/


#ifndef ntku_Button_H
#define ntku_Button_H

#include "ntku_include.h"
#include "../NurbanaMath.h"

class ntku_Button {
  public:
    static	void	Rect(int PosX, int PosY, int SizX, int SizY, char Color[3], unsigned char Border, bool State);
    static	void	Rounded(int PosX, int PosY, int SizX, int SizY, char Color[3], unsigned char Border, bool State);

}; //eof class ntku_Button
#endif
