/****************************************************
 * NWM Main Panel
 * Start Date: May 23, 2003
 * Author: Justin Shumaker
 *
 * Description:
 * Construction Panel
 ****************************************************/


#ifndef NWM_Panel_Construction_H
#define NWM_Panel_Construction_H

#include "NWM_Base.h"
#include "NWM.h"
#include "../NTK/NTK_Window.h"

//Widgets
#include "../NTK/NTK_Button.h"

class NWM;
class NWM_Panel_Construction : NWM_Base {
  public:
    NWM_Panel_Construction(int H1, int H2, int H3, int H4, NWM_List *obj, NWM *NWMptr, Object_Manager *ObjectList);
    ~NWM_Panel_Construction() {}


    static		NWM_Panel_Construction	*Access;	// Public Self


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
    NTK_Button		*_Loft;
    NTK_Button		*_Blend;
    NTK_Button		*_Rebuild;


  private:
    NWM_Panel_Construction	*_self;			// Local Self

}; //eof class NWM_Panel_Construction

#endif
