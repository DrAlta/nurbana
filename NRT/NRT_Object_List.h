/****************************************************
 * NRT Object List
 * Start Date: Jul 14, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Contains list of NRT Objects
 ****************************************************/


#ifndef NRT_Object_List_H
#define NRT_Object_List_H

#include <stdio.h>
#include <stdlib.h>

#include "NRT_Object_Base.h"

class NRT_Object_Base;
class NRT_Object_List {
  public:

    struct ObjectNode {
      NRT_Object_Base	*Object;
      ObjectNode	*Next;
    };

    NRT_Object_List();
    ~NRT_Object_List();

    void	Push(NRT_Object_Base *obj, int ID);


    ObjectNode*	GetHead();
    ObjectNode*	GetObjectPtr(int ID);
    int		GetNext(int ID);

    int		GetNewObjectID();    
    void	PurgeList();
    void	RemoveNode(int ID);

  private:
    int			_New_Object_ID;
    ObjectNode		*_Head;

}; //eof class NRT_Object_List
#endif
