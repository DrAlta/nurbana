/****************************************************
 * NRT Object Manager
 * Start Date: Jul 27, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Nurbana Window Manager
 ****************************************************/


#ifndef NWM_H
#define NWM_H

#include <string.h>
#include <stdlib.h>
#include "NWM_List.h"
#include "../KeyTable.h"
#include "../MaterialList.h"
#include "../BorderSelect.h"
#include "../NGUtils.h"
#include "../NTimer.h"
#include "../Cursors/Cursors.h"
#include "../FileIO/FileIO.h"
#include "../NRT/NRT_RayTracer.h"


#include "NWM_FileManager.h"
#include "NWM_Handle.h"
#include "NWM_KnotMenu.h"
#include "NWM_Menu.h"
#include "NWM_ModelView.h"
#include "NWM_Panel_Construction.h"
#include "NWM_Panel_Edit.h"
#include "NWM_Panel_Materials.h"
#include "NWM_Panel_Primitives.h"
#include "NWM_Panel_Render.h"
#include "NWM_Panel_Settings.h"
#include "NWM_SceneNotes.h"
#include "NWM_Selector.h"
#include "NWM_StatusBar.h"

#define	NWM_Handle_Thickness	4


class NWM {
  public:

    NWM();
    ~NWM() {}

    void	Init(int W, int H, char *Path, char* Filename);

    //
    // EVENTS
    //
    void			Update();
    void			Visibility(int V);
    void			Reshape(int w, int h);
    void			Key(int x, int y, bool UD);
    void			Mouse(int button, int state, int x, int y);
    void			Motion(int x, int y);
    void			PassiveMotion(int x, int y);

    void			CreateMenu(unsigned char MenuType);
    void			UpdateType(unsigned char WT);
    void			RefreshAll();
    void			RefreshType(unsigned char WT);
    int				Replace(int ID, unsigned char WT);

    char*			Path() { return( _Path ); }
    void			CreateNWindow(char Type);
    void			CalcWinGeom(bool SD);				// Compute Window Geometry
    int				_ActiveWindow;	// ID of Active Window set by ::Passive


  private:
    // Functions
    void			DefaultLayout();
    void			SelfDestructCheck();
    void			ProcessResponse(char R, int WID);
    void			Substitute(int x, int y, char Type);
    void			RemoveSubstitute(int WID);
    int				GetAttribute(int ID, int H);		// Get Window Attribute
    void			ToggleStaticMenus();			// Toggle Display of Static Menus


    // Variables
    NWM				*_self;
    NWM_List::WindowNode*	GetWindow(int ID);
    NWM_List			*_Head;
    char			*_Path;			// Path of Nurbana Binary
    bool			_MenuLock;		// Menu Open Flag

    bool			_StaticMenus;		// Display Static Menus
    int				_StaticHandle[3];	// Static Handle IDs
    int				_StaticSubwindow[3];	// Static Subwindow IDs

    MaterialList		*_MaterialList;
    Object_Manager		*_ObjectList;
    NRT_RayTracer		*_RT;

    int				_MouseX;
    int				_MouseY;

    int				_Width;		// Screen Width
    int				_Height;	// Screen Height

    int				_FocusID;	// ID of Subwindows that Locks Focus
    bool			_LOCK;		// Subwindow Locks Focus for all Events

    int				_DoubleRefresh;
    bool			_UpdateOff;
    int				_lActiveWindow;	// Local version, stores handles ids too

}; //eof class NWM
#endif
