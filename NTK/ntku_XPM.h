/***************************************************
 * NTK Utility - XPM Loader
 * Start Date: Nov 6, 2000
 * Author: Justin Shumaker
 *
 * Description:
 * Parces XPM files for info
 *****************************************************/


#ifndef ntku_XPM_H
#define ntku_XPM_H

#include "ntku_include.h"
#include <string.h>

#define BUF_SIZE	8
#define ColChange	0x18

class ntku_XPM {
  public:
    static	void	getXPMheader(unsigned char **temp, int data[4]);
    static	void	getXPMColor(unsigned char *temp[], int data[4], char ColString[8], char rgb[3]);

}; //eof class ntku_XPM
#endif
