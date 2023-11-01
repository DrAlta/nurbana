/****************************************************
 * NTK - Menu
 * Start Date: Mar 16, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Creates and Manges Menus and their respective
 * callback functions
 ****************************************************/


#ifndef NTK_Menu_H
#define NTK_Menu_H


#include <string.h>
#include "NTK_Base.h"
#include "NTK_Text.h"
#include "ntku_Button.h"
#include "../KeyTable.h"

typedef	void	(NTK_MenuCallback)(int V);

class NTK_Menu : public NTK_Base {
  public:

    struct MenuNode {
      char	_Text[256];
      int       _Value;
      MenuNode	*Next;
    };

    NTK_Menu::NTK_Menu(NTK_Window *win, int PosX, int PosY, int SizX, int SizY);
    NTK_Menu::~NTK_Menu();

    void	Draw();
    bool	Handle(unsigned char Event, int A, int B, int C, int D);
    void	Resize() {}

    bool	Push(char *T, int V);
    void	Pop(int V);

    void	Text(int V, char T[]);
    void	PurgeList();
    void	Title(char T[]) { strcpy(_Title,T); _Update= 2; }
    void	Value(float v) { _Value= v; _Update= 2; }
    int		GetFreeID();

    void	MenuCallback(NTK_MenuCallback *MC) { _MenuCallback= MC; }
    void	MenuCallback(int V) { if(_MenuCallback) _MenuCallback(V); }

    void	ScrollBar(bool V) { _ScrollBar= V; }

  private:
    bool		_ScrollBar;
    int			_Total;
    int			_CDY;
    char		_Title[256];
    float		_Value;
    MenuNode		*_Head;
    NTK_MenuCallback	*_MenuCallback;
}; //eof class NTK_Menu
#endif
