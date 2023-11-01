#ifndef InvertBuffer_H
#define InvertBuffer_H

#include <GL/gl.h>

class InvertBuffer {
  public:
    static	void	DrawLine(int ApX, int ApY, int BpX, int BpY);
    static	void	DrawQuad(int ApX, int ApY, int BpX, int BpY, int CpX, int CpY, int DpX, int DpY);

    static	void	CursorHorzLine(int ApX, int ApY, int BpX, int BpY, int Width);
    static	void	CursorVertLine(int ApX, int ApY, int BpX, int BpY, int Width);

}; //eof InvertBuffer Class
#endif
