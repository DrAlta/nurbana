/****************************************************
 * NWM Knot Vector Menu
 * Start Date: Oct 20, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Menu for editing the Knot Vector
 ****************************************************/


#ifndef NWM_KnotMenu_H
#define NWM_KnotMenu_H

#include "NWM_Base.h"
#include "NWM.h"
#include "../NTK/NTK_Window.h"
#include "../NGUtils.h"

#include "../NTK/NTK_Button.h"
#include "../NTK/NTK_Label.h"
#include "../NTK/NTK_ScrollBar.h"
#include "../NTK/NTK_TextInput.h"

// Definitions


class NWM;
class NWM_KnotMenu : NWM_Base {
  public:
    NWM_KnotMenu(int PosX, int PosY, int W, int H, NWM_List *obj, bool Docked, Object_Manager *_ObjectList, NWM *NWMPtr, NWM_List *Head);
    ~NWM_KnotMenu() {}

    static		NWM_KnotMenu	*Access;	// Public Self

    void		Update();
    void		Refresh();
    void		Reshape(int w, int h);
    char		Key(int x, int y);
    char		Mouse(int button, int state, int x, int y);
    char		Motion(int x, int y);
    char		Passive(int x, int y);
    void		Active(bool A);
    void		Misc(int V) {}

    void		SelfDestruct(bool V) { _SelfDestruct= V; }

    NWM			*_NWM;
    NWM_List		*_Head;
    Object_Manager	*_ObjectList;

    // +---------+
    // | Widgets |
    // +---------+
    NTK_Window		*_KnotMenuWindow;

    NTK_Label		*_BackDrop;
    NTK_Label		*_TitleBar;
    NTK_Label		*_UKV;
    NTK_Label		*_VKV;

    NTK_ScrollBar	*_UScrollBar;
    NTK_ScrollBar	*_VScrollBar;

    NTK_TextInput	*_UInput[11];
    NTK_TextInput	*_VInput[11];

    NTK_Button		*_Apply;


  private:
    NWM_KnotMenu	*_self;

    int			_Viewport[4];
    int			_MenuBox[4];

}; //eof class NWM_Knot

#endif
