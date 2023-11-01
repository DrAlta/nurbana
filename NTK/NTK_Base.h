/****************************************************
 * NTK - Base
 * Start Date: Nov 19, 2000
 * Author: Justin Shumaker
 *
 * Description:
 * Base class for all Widgets
 *****************************************************/
#ifndef NTK_Base_H
#define NTK_Base_H

#include <stdlib.h>
#include "NTK_Window.h"
#include "NTK_Container.h"
#include "NTK_Text.h"
#include "../NDefs/NTKDefs.h"


typedef	void	(NTK_Callback)(unsigned char E);


class NTK_Window;
class NTK_Base {
  public:
    NTK_Base();
    NTK_Base(NTK_Window *win, int pX, int pY, int sX, int sY, unsigned char Type);
    virtual	~NTK_Base() {}

    void	CallbackTrigger(unsigned char v) { _CallbackTrigger= v; }

    void	Callback(NTK_Callback *c) { _callback= c; }
    void	Callback(unsigned char E) { if(_callback) _callback(E); }

    virtual	void	Draw()= 0;
    virtual	bool	Handle(unsigned char Event, int A, int B, int C, int D)= 0;
    virtual	void	Resize()= 0;

    void	Color(unsigned char R, unsigned char G, unsigned char B) { _Color[0]= R; _Color[1]= G; _Color[2]= B; _Update= 2; }

    bool	Status() { return _State; } //eof Status()
    void	Status(bool S) { _State= S; }
    void	Visible(bool V) { _Visible= V; _Update= 2; }
    bool	Visible() { return(_Visible); }
    void	Layer(unsigned char L) { _Layer= L; }
    void	Container(NTK_Container *C) { _Container= C; }
    bool	Container() { if (_Container) { return(1); } else { return(0); } }

    void	Update(unsigned short U) { _Update= U; }
    void	Position(int x, int y) { _pX= x; _pY= y; }
    void        Size(int x, int y) { _sX= x; _sY= y; }

    void	DynPos(unsigned char DP) { _DynPos= DP; }
    void	DynSiz(unsigned char DS) { _DynSiz= DS; }
    void	DynPosX(int X) { _DynPosX= X; }
    void	DynPosY(int Y) { _DynPosY= Y; }
    void	DynSizX(int X) { _DynSizX= X; }
    void	DynSizY(int Y) { _DynSizY= Y; }
    void	Scale(bool X, bool Y) { _ScaleX= X; _ScaleY= Y; }

    void	Rounded(bool R)
		{ _Rounded= R; }

    void	Anchored(bool A) { _Anchored= A; }
    bool	Anchored() { return(_Anchored); }

    int		pX() { return(_pX); }
    int		pY() { return(_pY); }
    int		sX() { return(_sX); }
    int		sY() { return(_sY); }

    void	pX(int V) { _pX= V; }
    void	pY(int V) { _pY= V; }
    void	sX(int V) { _sX= V; }
    void	sY(int V) { _sY= V; }


    //***** Variables *****
    unsigned	char	_Type;
    bool		_State;
    bool		_Visible;
    int			_pX;
    int			_pY;
    int			_sX;
    int			_sY;

    unsigned	char	_DynPos;
    unsigned	char	_DynSiz;

    int			_DynPosX;
    int			_DynPosY;
    int			_DynSizX;
    int			_DynSizY;

    bool		_Rounded;
    bool		_Anchored;

    bool		_ScaleX;
    bool		_ScaleY;

    unsigned	char	_Layer;
    unsigned	char	_Update;
    char		_Color[3];
    unsigned	char	_CallbackTrigger;
    NTK_Callback	*_callback;
    NTK_Container	*_Container;
    NTK_Text		*_Font;
    NTK_Window		*_Window;

}; //eof class NTK_Base

#endif
