#ifndef BorderSelect_H
#define BorderSelect_H

//#include "GLUT/glut.h"
#include <GL/gl.h>
#include "InvertBuffer.h"

#define	BORDER_SELECT_MODE_CROSSHAIR	0
#define	BORDER_SELECT_MODE_BOX		1

class BorderSelect {
  public:
    static	void	Initialize(int WinPosX, int WinPosY, int WinSizX, int WinSizY);
    static	void	ScreenSize(int Width, int Height);
    static	void	Update(int x, int y);
    static	void	Anchor(int MB);
    static	void	GetRegion(int Data[4]);
    static	int	MouseButton();

}; //eof BorderSelect Class
#endif
