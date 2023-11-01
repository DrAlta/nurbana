/****************************************************
 * NTK Utility - Bresenham Scanline Renderer
 * Start Date: Apr 20, 2000
 * Author: Justin Shumaker
 *
 * Description:
 * Draws lines and stuff
 *****************************************************/


#ifndef ntku_Bresenham_H
#define ntku_Bresenham_H

class ntku_Bresenham {
  public:
    static	void	Render(unsigned char *Bitmap, int W, int H, int X1, int Y1, int X2, int Y2, unsigned char A);

}; //eof class ntku_Bresenham()
#endif
