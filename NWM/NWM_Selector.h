/****************************************************
 * NWM Selector
 * Start Date: Oct 20, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Formerly Main Menu
 ****************************************************/


#ifndef NWM_Selector_H
#define NWM_Selector_H

#include "NWM_Base.h"
#include "NWM.h"
#include "../ModelView.h"
#include "../NTK/NTK_Window.h"
#include "../NGUtils.h"
#include "../MaterialList.h"

#include "../NTK/NTK_Button.h"
#include "../NTK/NTK_Group.h"

// Definitions
#define	NWM_MAINMENU_PADDING	15



class NWM;
class NWM_Selector : NWM_Base {
  public:
    NWM_Selector(int H1, int H2, int H3, int H4, NWM_List *obj, NWM *NWMptr, Object_Manager *ObjectList, int PID);
    ~NWM_Selector();

    static		NWM_Selector	*Access;	// Public Self

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

    friend		void	ChangeMenuCallback(char E);

    NWM			*_NWM;
    NWM_List		*_Head;
    Object_Manager	*_ObjectList;
    int			_PID;

    // +---------+
    // | Widgets |
    // +---------+
    NTK_Window		*_SelectorWindow;

    NTK_Group		*_ButtonGroup;

    NTK_Button		*_PrimitiveMenu;
    NTK_Button		*_MaterialMenu;
    NTK_Button		*_EditMenu;
    NTK_Button		*_ConstructionMenu;
    NTK_Button		*_RenderMenu;
    NTK_Button		*_CameraMenu;
    NTK_Button		*_LightingMenu;
    NTK_Button		*_SettingsMenu;
    NTK_Button		*_CommandMenu;

  private:
    NWM_Selector	*_self;
    int			_Viewport[4];

}; //eof class NWM_Selector

#endif
