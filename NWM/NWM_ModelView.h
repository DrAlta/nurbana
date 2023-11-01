/****************************************************
 * NWM ModelView Template
 * Start Date: Jul 27, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * 1 ModelView and 1 NTK Bar for Stuff
 ****************************************************/


#ifndef NWM_ModelView_H
#define NWM_ModelView_H

#include "NWM_Base.h"
#include "NWM.h"
#include "../ModelView.h"
#include "../NTK/NTK_Window.h"
#include "../Objects/Object_Manager.h"
#include "../KeyTable.h"
#include "../Cursors/Cursors.h"
#include "../NIdle.h"

#include "../NTK/NTK_Button.h"
#include "../NTK/NTK_Label.h"

class NWM;
class NWM_ModelView : NWM_Base {
  public:
    NWM_ModelView(int H1, int H2, int H3, int H4, NWM_List *obj, NWM *NWMPtr, Object_Manager *ObjectList);
    ~NWM_ModelView();

    static		NWM_ModelView	*Access;	// Public Self

    void		Update();
    void		Refresh();
    void		Reshape(int w, int h);
    char		Key(int x, int y);
    char		Mouse(int button, int state, int x, int y);
    char		Motion(int x, int y);
    char		Passive(int x, int y);
    void		Active(bool A);
    void		Misc(int V) {}

    void		Update(int V) { _Update= V; }

    NWM			*_NWM;
    Object_Manager	*_ObjectList;
    ModelView		*_ModelView;
    int			_Xpos[2],_Ypos[2];
    bool		_Tran;

    // Widgets
    NTK_Window		*_ControlWindow;

  private:
    NWM_ModelView	*_self;		// Local Self


    int			_ViewportA[4];
    int			_ViewportB[4];

    unsigned	char	_Button;

    // Widgets Associated With ControlBar
    NTK_Label		*_ControlBar;
    NTK_Container	*_ButtonContainer;
    NTK_Button		*_Translate;
    NTK_Button		*_Zoom;


}; //eof class NWM_ModelView

#endif
