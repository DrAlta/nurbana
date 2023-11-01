/****************************************************
 * Nurbana Idle Utility Class
 * Start Date: Feb 17, 2002
 * Author: Justin Shumaker
 *
 * Description:
 * Stores and Makes callbacks for functions that
 * require idle cpu time.
 *****************************************************/


#ifndef NIdle_H
#define NIdle_H

#include <SDL.h>

typedef	void	(NIdle_Callback)(void);

class NIdle {
  public:
    static	NIdle_Callback	*_Callback;
    static	void		Init() { _Callback= NULL; }
    static	bool		Callback()
				{
				  if (_Callback) {
				    _Callback();
				    return(1);
				  } //fi
				  return(0);
				}
    static	void		Callback(NIdle_Callback C) { _Callback= C; }

}; //eof class NIdle
#endif
