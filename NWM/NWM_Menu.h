/****************************************************
 * NWM Menu's
 * Start Date: Jul 29, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * All Menu's
 ****************************************************/


#ifndef NWM_Menu_H
#define NWM_Menu_H

#include <time.h>
#include "NWM_Base.h"
#include "NWM.h"
#include "../ModelView.h"
#include "../NTK/NTK_Window.h"
#include "../NGUtils.h"
#include "../MaterialList.h"
#include "../NIdle.h"

#include "../NTK/NTK_Menu.h"
#include "../NTK/NTK_Button.h"
#include "../NTK/NTK_Group.h"
#include "../NTK/NTK_Label.h"
#include "../NTK/NTK_TextInput.h"
#include "../NTK/NTK_Slider_Horz.h"
#include "../NTK/NTK_Slider_Button.h"

// Load NRT for Rendering
#include "../NRT/NRT_RayTracer.h"

// Definitions
#define	NWM_MENU_PADDING	20


#define	NWM_MENU_QUIT		0
#define	NWM_MENU_MAIN		1
#define	NWM_MENU_EXTRUDE	2
#define	NWM_MENU_DELETE		3
#define	NWM_MENU_PURGE		4
#define	NWM_MENU_MATERIALS	5
#define	NWM_MENU_NUMERICS	6
#define	NWM_MENU_CLEAR_SCENE	7
#define	NWM_MENU_LOFT		8
#define	NWM_MENU_BLEND		9
#define	NWM_MENU_REBUILD	10
#define	NWM_MENU_RENDER		11

class NWM;
class NWM_Menu : NWM_Base {
  public:
    NWM_Menu(int PosX, int PosY, int W, int H, NWM_List *obj, char MenuType, Object_Manager *_ObjectList, NWM *NWMPtr, MaterialList *MatList, NRT_RayTracer *RT);
    ~NWM_Menu();

    static		NWM_Menu	*Access;	// Public Self

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
    NRT_RayTracer	*_RT;

    // +---------+
    // | Widgets |
    // +---------+

    NTK_Window		*_MenuWindow;

    // Menus
    NTK_Menu		*_ClearSceneMenu;
    NTK_Menu		*_ExtrudeMenu;
    NTK_Menu		*_QuitMenu;
    NTK_Menu		*_DeleteMenu;
    NTK_Menu		*_MaterialsMenu;

    // Numerics Menu
    NTK_Label		*_NumericsTitle;
    NTK_Label		*_NumericsBackground;
    NTK_Slider_Button	*_NLocX;
    NTK_Slider_Button	*_NLocY;
    NTK_Slider_Button	*_NLocZ;
    NTK_Slider_Button	*_NLocW;

    NTK_Slider_Button	*_NRotX;
    NTK_Slider_Button	*_NRotY;
    NTK_Slider_Button	*_NRotZ;
    NTK_Slider_Button	*_NSizX;
    NTK_Slider_Button	*_NSizY;
    NTK_Slider_Button	*_NSizZ;
    NTK_Button		*_NumericsOK;

    // Loft Menu
    NTK_Label		*_LoftBackground;
    NTK_Label		*_LoftTitle;
    NTK_Label		*_DegreeLabel;
    NTK_TextInput	*_Degree;
    NTK_Button		*_Loft;

    // Blend Menu
    NTK_Label		*_BlendBackground;
    NTK_Label		*_BlendTitle;
    NTK_Label		*_TwistLabel;
    NTK_Button		*_TwistButton;
    NTK_Button		*_Blend;

    // Rebuild Menu
    NTK_Label		*_RebuildBackground;
    NTK_Label		*_RebuildTitle;
    NTK_Label		*_DegreeULabel;
    NTK_Label		*_DegreeVLabel;
    NTK_Label		*_CPULabel;
    NTK_Label		*_CPVLabel;
    NTK_Slider_Horz	*_DegreeUSlider;
    NTK_Slider_Horz	*_DegreeVSlider;
    NTK_Slider_Horz	*_CPUSlider;
    NTK_Slider_Horz	*_CPVSlider;

    NTK_Group		*_RebuildGroup;
    NTK_Button		*_Centripetal;
    NTK_Button		*_Chord;
    NTK_Button		*_Universal;
    NTK_Button		*_Rebuild;

   // Render Menu
   NTK_Label		*_RenderBackground;
   NTK_Label		*_RenderTitle;
   NTK_Label		*_RenderArea;
   NTK_Button		*_RenderCancel;
   bool			_Complete;


    clock_t		_start,_stop;

  private:
    NWM_Menu		*_self;

    bool		_AutoDestroy;
    char		_MenuType;
    int			_Viewport[4];
    int			_MenuBox[4];

}; //eof class NWM_Menu

#endif
