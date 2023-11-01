/****************************************************
 * NWM Main Panel
 * Start Date: May 23, 2002
 * Author: Justin Shumaker
 *
 * Description:
 * Edit Panel
 ****************************************************/


#ifndef NWM_Panel_Edit_H
#define NWM_Panel_Edit_H

#include "NWM_Base.h"
#include "NWM.h"
#include "../NTK/NTK_Window.h"

//Widgets
#include "../NTK/NTK_Button.h"
#include "../NTK/NTK_Container.h"
#include "../NTK/NTK_Group.h"
#include "../NTK/NTK_Label.h"
#include "../NTK/NTK_Slider_Button.h"
#include "../NTK/NTK_Slider_Horz.h"


class NWM;
class NWM_Panel_Edit : NWM_Base {
  public:
    NWM_Panel_Edit(int H1, int H2, int H3, int H4, NWM_List *obj, NWM *NWMptr, Object_Manager *ObjectList);
    ~NWM_Panel_Edit() {}


    static		NWM_Panel_Edit	*Access;	// Public Self


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


    // +---------+
    // | Widgets |
    // +---------+
    NTK_Window		*_Window;
    NTK_Container	*_EditVContainer;

    // Edit Menu
    NTK_Button		*_CenterObject;
    NTK_Button		*_CenterPoints;
    NTK_Button		*_Subdivide;

    NTK_Group		*_MirroringGroup;
    NTK_Button		*_MirX;
    NTK_Button		*_MirY;
    NTK_Button		*_MirZ;

    NTK_Button		*_Normals;
    NTK_Slider_Horz	*_NormalLength;

    NTK_Slider_Button	*_DegreeUSlider;
    NTK_Slider_Button	*_DegreeVSlider;

    NTK_Button		*_SetWeight;
    NTK_Slider_Horz	*_WeightSlider;

    NTK_Slider_Button	*_IsoParm;

//    NTK_Label		*_KnotType;
//    NTK_Button		*_KnotPeriodicU;
//    NTK_Button		*_KnotPeriodicV;
//    NTK_Button		*_KnotOpenU;
//    NTK_Button		*_KnotOpenV;

    NTK_Button		*_Axes;

  private:
    NWM_Panel_Edit	*_self;			// Local Self
    int			_ActivePanel;

}; //eof class NWM_MainMenu

#endif
