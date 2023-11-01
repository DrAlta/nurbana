/****************************************************
 * Nurbana Key State Table
 * Start Date: Nov 6, 2000
 * Author: Justin Shumaker
 *
 * Description:
 * Maintains State of every key
 *****************************************************/


#ifndef Keytable_H
#define Keytable_H

#include <SDL.h>

class KeyTable {
  public:
    static	SDL_keysym	KS;

    static	void	Update(SDL_keysym NKS) { KS= NKS; }

    static	int	Key()
			{
			  return( KS.sym );
			}


    static	char	KeySym()
			{
			  return( KS.unicode );
			}


    static	bool	Mod(int KMOD)
			{
			  if (KS.mod & KMOD) return(1); return(0);
			}

}; //eof class KeyTable
#endif
