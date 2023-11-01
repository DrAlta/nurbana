/****************************************************
 * NWM Handle
 * Start Date: Aug 5, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Draws Handles that connect Windows
 ****************************************************/


#ifndef NWM_Handle_H
#define NWM_Handle_H

#include "NWM_Base.h"
#include "NWM.h"
#include "nwmu_Handle.h"
#include "../NTK/NTK_Window.h"
#include "../NGUtils.h"
#include "../Cursors/Cursors.h"

#define	NWM_Handle_Thickness	4


class NWM;
class NWM_Handle : NWM_Base {
  public:
    NWM_Handle(int Pos, int A, int B, int W, int H, NWM_List *Head, NWM *NWMptr, bool VH, bool Anchored);
    ~NWM_Handle();

    void		Update();
    void		Refresh();
    void		Reshape(int w, int h);
    char		Key(int x, int y);
    char		Mouse(int button, int state, int x, int y);
    char		Motion(int x, int y);
    char		Passive(int x, int y);
    void		Active(bool A);
    void		Misc(int V) {}

    bool		VH() { return( _VH ); }

  private:
    int			GetAttribute(int ID, int H);
    int			GetNLH(bool AB, bool PS);	// Get ID of Nearest Larger Handle (Must overlap Lengthwise)

    int			_Viewport[4];
    NWM			*_NWM;
    NWM_List		*_Head;
    bool		_VH;		// Vertical or Horizontal
    bool		_Anchored;	// Handle Anchored

    // USE _H3 and _H4 for storage of WIDTH and HEIGHT

}; //eof class NWM_Handle

#endif
