/****************************************************
 * NWM Window List
 * Start Date: Jul 27, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Stores a list of pointers to each Window
 ****************************************************/


#ifndef NWM_List_H
#define NWM_List_H

#include <stdlib.h>
#include "NWM_Base.h"


class NWM_Base;
class NWM_List {
  public:

    struct WindowNode {
      NWM_Base		*Window;
      WindowNode	*Next;
    };

    NWM_List();
    ~NWM_List();

    void	Push(NWM_Base *obj);


    WindowNode*	GetHead();
    WindowNode*	GetWindowPtr(int ID);
    int		GetNext(int ID);

    int		GetNewWindowID();
    void	PurgeList();
    void	RemoveNode(int ID);

  private:
    int			_ID_Counter;
    int			_New_Window_ID;
    WindowNode		*_Head;

}; //eof class NWM_List
#endif
