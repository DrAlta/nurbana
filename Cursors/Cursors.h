/****************************************************
 * Nurbana Static Cursor Library
 * Start Date: Feb 17, 2002
 * Author: Justin Shumaker
 *
 * Description:
 * Library of cursors to choose from
 *****************************************************/


#ifndef Cursors_H
#define Cursors_H

#include <SDL.h>

#define	CURSOR_ARROW		0
#define	CURSOR_CROSSHAIR	1
#define	CURSOR_UPDOWN		2
#define	CURSOR_LEFTRIGHT	3
#define	CURSOR_TEXT		4

class Cursors {
  public:
    static	SDL_Cursor	*Cursor;
    static	void		SetCursor(unsigned char C);

}; //eof class Cursors
#endif
