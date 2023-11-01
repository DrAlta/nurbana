/****************************************************
 * NWM Main Panel
 * Start Date: Jul 28, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Main Panel
 ****************************************************/


#ifndef NWM_Panel_Primitives_H
#define NWM_Panel_Primitives_H

#include "NWM_Base.h"
#include "NWM.h"
#include "../NTK/NTK_Window.h"
#include "../MaterialList.h"

//Widgets
#include "../NTK/NTK_Button.h"
#include "../NTK/NTK_Label.h"
#include "../NTK/NTK_Slider_Horz.h"
#include "../NTK/NTK_TextInput.h"

// Load NRT for Rendering
#include "../NRT/NRT_RayTracer.h"

class NWM;
class NWM_Panel_Primitives : NWM_Base {
  public:
    NWM_Panel_Primitives(int H1, int H2, int H3, int H4, NWM_List *obj, NWM *NWMptr, Object_Manager *ObjectList);
    ~NWM_Panel_Primitives() {}


    static		NWM_Panel_Primitives	*Access;	// Public Self


    void		Update();
    void		Refresh();
    void		Reshape(int w, int h);
    char		Key(int x, int y);
    char		Mouse(int button, int state, int x, int y);
    char		Motion(int x, int y);
    char		Passive(int x, int y);
    void		Active(bool A);
    void		Misc(int V);


    NWM			*_NWM;
    Object_Manager	*_ObjectList;

    int			_Viewport[6];


    //
    // Widgets
    //
    NTK_Window		*_Window;

    // Primitive Menu
    NTK_Button		*_NURBSCurve;
    NTK_Button		*_NURBSSurface;
    NTK_Button		*_NURBSEllipse;
    NTK_Button		*_NURBSTube;
    NTK_Button		*_NURBSTorus;
    NTK_Button		*_NURBSSphere;

  private:
    NWM_Panel_Primitives	*_self;			// Local Self
    int			_ActivePanel;

}; //eof class NWM_MainMenu

#endif
