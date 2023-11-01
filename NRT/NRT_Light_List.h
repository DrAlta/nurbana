/****************************************************
 * Light List
 * Start Date: Jul 14, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Stores a list of pointers to each Object and
 * contains many functions related to those Objects.
 ****************************************************/


#ifndef NRT_Light_List_H
#define NRT_Light_List_H


#include "NRT_Light_Base.h"

class NRT_Light_Base;
class NRT_Light_List {
  public:

    struct LightNode {
      NRT_Light_Base	*Object;
      LightNode		*Next;
    };

    NRT_Light_List();
    ~NRT_Light_List();

    void	Push(NRT_Light_Base *obj, int ID);


    LightNode*	GetHead();
    LightNode*	GetLightPtr(int ID);
    int		GetNext(int ID);

    int		GetNewLightID();    
    void	PurgeList();
    void	RemoveNode(int ID);

  private:
    int			_New_Light_ID;
    LightNode		*_Head;

}; //eof class NRT_Light_List
#endif
