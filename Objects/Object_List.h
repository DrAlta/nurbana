/****************************************************
 * Object Manager
 * Start Date: Jan 27, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Stores a list of pointers to each Object and
 * contains many functions related to those Objects.
 ****************************************************/


#ifndef Object_List_H
#define Object_List_H

#include <stdio.h>
#include <stdlib.h>

#include "Object_Base.h"

class Object_Base;
class Object_List {
  public:

    struct ObjectNode {
      Object_Base	*Object;
      ObjectNode	*Next;
    };

    Object_List();
    ~Object_List();

    void	Push(Object_Base *obj);


    ObjectNode*	GetHead();
    ObjectNode*	GetObjectPtr(int ID);
    int		GetNext(int ID);

    int		GetNewObjectID();    
    void	PurgeList();
    void	RemoveNode(int ID);

  private:
    int			_ID_Counter;
    int			_New_Object_ID;
    ObjectNode		*_Head;

}; //eof class Object_List
#endif
