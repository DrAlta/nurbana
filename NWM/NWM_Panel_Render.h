/****************************************************
 * NWM Main Panel
 * Start Date: Jul 28, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Main Panel
 ****************************************************/


#ifndef NWM_Panel_Render_H
#define NWM_Panel_Render_H

#include "NWM_Base.h"
#include "NWM.h"
#include "../NTK/NTK_Window.h"
#include "../MaterialList.h"

//Widgets
#include "../NTK/NTK_Button.h"
#include "../NTK/NTK_Label.h"
#include "../NTK/NTK_Slider_Horz.h"
#include "../NTK/NTK_Slider_Button.h"
#include "../NTK/NTK_TextInput.h"

// Load NRT for Rendering
#include "../NRT/NRT_RayTracer.h"

class NWM;
class NWM_Panel_Render : NWM_Base {
  public:
    NWM_Panel_Render(int H1, int H2, int H3, int H4, NWM_List *obj, NWM *NWMptr, Object_Manager *ObjectList, NRT_RayTracer *RT);
    ~NWM_Panel_Render() {}


    static		NWM_Panel_Render	*Access;	// Public Self


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
    NRT_RayTracer	*_RT;

    int			_Viewport[6];


    //
    // Widgets
    //
    NTK_Window		*_Window;

    NTK_Button		*_Render;
    NTK_Slider_Button	*_Rays;

    NTK_Slider_Button	*_Threads;

    NTK_Slider_Button	*_PacketSize;
    NTK_Slider_Button	*_PacketTime;

    NTK_Slider_Button	*_RenderSizeX;
    NTK_Slider_Button	*_RenderSizeY;

  private:
    NWM_Panel_Render	*_self;			// Local Self
    int			_ActivePanel;

}; //eof class NWM_MainMenu

#endif
