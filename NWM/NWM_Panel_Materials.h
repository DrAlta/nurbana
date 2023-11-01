/****************************************************
 * NWM Main Panel
 * Start Date: Jul 28, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Main Panel
 ****************************************************/


#ifndef NWM_Panel_Materials_H
#define NWM_Panel_Materials_H

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
class NWM_Panel_Materials : NWM_Base {
  public:
    NWM_Panel_Materials(int H1, int H2, int H3, int H4, NWM_List *obj, NWM *NWMptr, Object_Manager *ObjectList, MaterialList *MatList);
    ~NWM_Panel_Materials() {}


    static		NWM_Panel_Materials	*Access;	// Public Self


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
    MaterialList	*_MaterialList;

    int			_Viewport[6];

    //
    // Widgets
    //
    NTK_Window		*_Window;

    // Materials Menu
    NTK_Group		*_DiffSpecGroup;
    NTK_Slider_Horz	*_MatRedSlider;
    NTK_Slider_Horz	*_MatGrnSlider;
    NTK_Slider_Horz	*_MatBluSlider;
    NTK_Label		*_ColorPanel;
    NTK_Label		*_RenderPanel;
    NTK_Button		*_Diffuse;
    NTK_Button		*_Specular;
    NTK_Button		*_Realtime;
    NTK_Button		*_MatOpenList;
    NTK_TextInput	*_MaterialName;
    NTK_Button		*_MatDelete;

  private:
    NWM_Panel_Materials	*_self;			// Local Self
    int			_ActivePanel;

}; //eof class NWM_MainMenu

#endif
