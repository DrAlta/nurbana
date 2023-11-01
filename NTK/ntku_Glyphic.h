/****************************************************
 * NTK Utility - Glyphic
 * Start Date: Nov 11, 2000
 * Author: Justin Shumaker
 *
 * Description:
 * Places a Glyphic at the specified location
 *****************************************************/


#ifndef ntku_Glyphic_H
#define ntku_Glyphic_H


#include "ntku_include.h"
#include "ntku_XPM.h"

class ntku_Glyphic {
  public:
    //SizX and SizY indicate region glyphic is in for centering purposes
    static	void	Size(unsigned int &Width, unsigned int &Height, unsigned char **glyphic_xpm);
    static	void	Create(unsigned char **glyphic_xpm, unsigned char *Glyphic);
    static	void	Draw(int PosX, int PosY, int SizX, int SizY, int Width, int Height, unsigned char *Glyphic);

}; //eof class ntku_Glyphic
#endif
