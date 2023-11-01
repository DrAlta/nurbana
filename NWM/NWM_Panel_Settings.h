/****************************************************
 * NWM Control Panel
 * Start Date: Jul 28, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Control Panel and Header
 ****************************************************/


#ifndef NWM_Panel_Settings_H
#define NWM_Panel_Settings_H

#include "NWM_Base.h"
#include "NWM.h"
#include "../Objects/Object_Manager.h"
#include "../NTK/NTK_Window.h"

#include "../NTK/NTK_Button.h"
#include "../NTK/NTK_Group.h"
#include "../NTK/NTK_Label.h"
#include "../NTK/NTK_Slider_Horz.h"
#include "../NTK/NTK_Slider_Button.h"

class NWM;
class NWM_Panel_Settings : NWM_Base {
  public:
    NWM_Panel_Settings(int H1, int H2, int H3, int H4, NWM_List *obj, NWM *NWMPtr, Object_Manager *ObjectList);
    ~NWM_Panel_Settings() {}

    static		NWM_Panel_Settings	*Access;	// Public Self

    void		Update();
    void		Refresh();
    void		Reshape(int w, int h);
    char		Key(int x, int y);
    char		Mouse(int button, int state, int x, int y);
    char		Motion(int x, int y);
    char		Passive(int x, int y);
    void		Active(bool A);
    void		Misc(int V) {}

    NWM			*_NWM;
    Object_Manager	*_ObjectList;


    // Modelview Background Color
    NTK_Button		*_MBCGRIDTOGGLE;
    NTK_Slider_Horz	*_MBCR;
    NTK_Slider_Horz	*_MBCG;
    NTK_Slider_Horz	*_MBCB;

    // Trackball
    NTK_Button		*_TrackballToggle;

    NTK_Slider_Horz	*_TrackballSpeed;

    // FOV
    NTK_Slider_Button	*_FOVAngle;

    // Tesselation
    NTK_Slider_Button	*_TessSlider;

    // Face Rendering
    NTK_Label		*_FaceRendering;
    NTK_Button		*_Smooth;

    // Save
    NTK_Button		*_SaveSettings;

  private:
    NWM_Panel_Settings	*_self;			// Local Self
    int			_Viewport[4];

    NTK_Window		*_Window;
}; //eof class NWM_Panel_Settings

#endif
