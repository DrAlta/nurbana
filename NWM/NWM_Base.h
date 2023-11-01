/****************************************************
 * NWM Base
 * Start Date: Jul 27, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Base class for all Window Groups
 ****************************************************/
#ifndef NWM_Base_H
#define NWM_Base_H

#include "NWM_List.h"
#include "../NDefs/NWMDefs.h"

#include <stdio.h>
class NWM_List;
class NWM_Base {
  public:
    NWM_Base() {}
    NWM_Base(NWM_List *obj, char Type);
    virtual	~NWM_Base() {}

    virtual	void	Update()= 0;
    virtual	void	Refresh()= 0;
    virtual	void	Reshape(int w, int h)= 0;
    virtual	char	Key(int x, int y)= 0;
    virtual	char	Mouse(int button, int state, int x, int y)= 0;
    virtual	char	Motion(int x, int y)= 0;
    virtual	char	Passive(int x, int y)= 0;
    virtual	void	Active(bool A)= 0;
    virtual	void	Misc(int V)= 0;

    // Evil!!!  but I need _VH from handles :(
    virtual	bool	VH() { return(0); };


    //
    // Variables
    //
    int			_ID;
    unsigned	char	_Type;

    bool		_Visible;
    unsigned	char	_Layer;
    int			_Update;
    bool		_Event;

    unsigned	char	_Replacement;		// Window Type to make visible after self destruction

    bool		_SelfDestruct;		// For temporary replacement windows that need to go away

    bool		_Splitable;		// This windows ability to be subdivided or split

    int			_PosX;
    int			_PosY;    
    int			_SizX;
    int			_SizY;

    int			_H1;			// Border Left
    int			_H2;			// Border Right
    int			_H3;			// Border Top
    int			_H4;			// Border Bottom

    int			_MinSizX;		// Least Width of Window
    int			_MinSizY;		// Least Height of Window
}; //eof class NWM_Base

#endif
