/****************************************************
 * NWM Status Bar
 * Start Date: Oct 20, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * 
 ****************************************************/


#ifndef NWM_StatusBar_H
#define NWM_StatusBar_H

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
class NWM_StatusBar : NWM_Base {
  public:
    NWM_StatusBar(int H1, int H2, int H3, int H4, NWM_List *obj, NWM *NWMptr, Object_Manager *ObjectList);
    ~NWM_StatusBar();

    static		NWM_StatusBar	*Access;	// Public Self

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
    NTK_Window		*_StatusBarWindow;

    NTK_Label		*_StatusBarLabel;
    NTK_Label		*_StatusMessage;

    NTK_Button		*_LayerButtons[15];
    NTK_Button		*_ResetViewports;
    NTK_Button		*_SaveViewports;
    NTK_Button		*_Undo;
    NTK_Button		*_Redo;
    NTK_Button		*_LoadFile;
    NTK_Button		*_SaveFile;

  private:
    NWM_StatusBar	*_self;
    int			_Viewport[4];

}; //eof class NWM_StatusBar

#endif
