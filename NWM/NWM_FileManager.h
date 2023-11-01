/****************************************************
 * NWM File Manager
 * Start Date: Aug 4, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * File Manager
 ****************************************************/


#ifndef NWM_FileManager_H
#define NWM_FileManager_H

#include "NWM_Base.h"
#include "NWM.h"
#include "../NTK/NTK_Window.h"
#include "../NTK/NTK_TextInput.h"
#include "../NTK/NTK_Button.h"
#include "../NTK/NTK_Label.h"
#include "../NTK/NTK_ScrollBar.h"
#include "../NTK/NTK_List.h"
#include "../MaterialList.h"
#include "../Objects/Object_Manager.h"
#include "../FileIO/FileIO.h"
#include "../NDefs/NTKDefs.h"

class NWM;
class NWM_FileManager : NWM_Base {
  public:
    NWM_FileManager(int H1, int H2, int H3, int H4, NWM *NWMPtr, NWM_List *obj, Object_Manager *ObjectList, MaterialList *MatList, bool LS, char *Path);
    ~NWM_FileManager();

    static		NWM_FileManager	*Access;	// Public Self

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
    Object_Manager	*_ObjectList;
    MaterialList	*_MaterialList;

    // Widget Objects
    NTK_TextInput	*_TextBox;
    NTK_Button		*_Button;
    NTK_Button		*_CancelButton;
    NTK_Button		*_Parent;
    NTK_List		*_Listing;
    NTK_ScrollBar	*_ScrollBar;
    NTK_Label		*_FileType;
    NTK_Group		*_FileTypeGroup;
    NTK_Button		*_NURBS;
    NTK_Button		*_3DM;
    NTK_Button		*_DXF;
    char		_Directory[256];


  private:
    NWM_FileManager	*_self;			// Local Self
    NTK_Window		*_FMWindow;

    int			_Viewport[4];
    char		*_Path;

}; //eof class NWM_FileManager

#endif
