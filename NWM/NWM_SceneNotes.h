/****************************************************
 * NWM File Manager
 * Start Date: Aug 4, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * File Manager
 ****************************************************/


#ifndef NWM_SceneNotes_H
#define NWM_SceneNotes_H

#include "NWM_Base.h"
#include "NWM.h"
#include "../NTK/NTK_Window.h"
#include "../NTK/NTK_TextArea.h"
#include "../NTK/NTK_Button.h"
#include "../NTK/NTK_Label.h"
//#include "../NTK/NTK_ScrollBar.h"
#include "../Objects/Object_Manager.h"
#include "../NDefs/NTKDefs.h"

class NWM;
class NWM_SceneNotes : NWM_Base {
  public:
    NWM_SceneNotes(int H1, int H2, int H3, int H4, NWM *NWMPtr, NWM_List *obj, Object_Manager *ObjectList);
    ~NWM_SceneNotes();

    static		NWM_SceneNotes	*Access;	// Public Self

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

    // Widget Objects
    NTK_Label		*_SceneNotesLabel;
    NTK_TextArea	*_SceneNotesTextArea;
    NTK_Button		*_SceneNotesButton;

  private:
    NWM_SceneNotes	*_self;			// Local Self
    NTK_Window		*_SceneNotesWindow;

    int			_Viewport[4];

}; //eof class NWM_FileManager

#endif
