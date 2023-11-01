#include "NRT_Object_List.h"


NRT_Object_List::NRT_Object_List() {
  _New_Object_ID= 0;
  _Head= NULL;
} //eof NRT_Object_List::NRT_Object_List()


NRT_Object_List::~NRT_Object_List() {
  NRT_Object_List::ObjectNode	*cur;

  while (_Head) {
    cur= _Head -> Next;
    delete _Head;
    _Head= cur;
  } //eow
} //eof NRT_Object_List::~NRT_Object_List()


void NRT_Object_List::Push(NRT_Object_Base *obj, int ID) {
  //Add the Object to the list
  NRT_Object_List::ObjectNode	*cur= NULL;

  if (_Head == NULL) {
    _Head= new NRT_Object_List::ObjectNode;
    _Head -> Next= NULL;
  } else {
    //Structured to create a LIFO stack
    cur= _Head;
    _Head= NULL;
    _Head= new NRT_Object_List::ObjectNode;

    //Connect the list
    _Head -> Next= cur;
  } //fi

  _Head -> Object= obj;
  _Head -> Object -> _ID= ID;
  _New_Object_ID= ID;
} //eof Object_List::Push()



NRT_Object_List::ObjectNode* NRT_Object_List::GetHead() {
  if (_Head) {
    return(_Head);
  } //fi
  return(NULL);
} //eof NRT_Object_List::GetHead()



NRT_Object_List::ObjectNode* NRT_Object_List::GetObjectPtr(int ID) {
  NRT_Object_List::ObjectNode  *cur= NULL;

  if (_Head) {
    cur= _Head;
    while (cur != NULL) {
      if (cur -> Object -> _ID == ID) return(cur);
      cur= cur -> Next;
    } //eow
  } //fi
  return(NULL);
} //eof NRT_Object_List::GetObject()



int NRT_Object_List::GetNext(int ID) {
  NRT_Object_List::ObjectNode	*cur= NULL;

  if (_Head) {
    cur= _Head;
    while (cur != NULL) {
      if (cur -> Object -> _ID == ID) {
        if (cur -> Next) {
          return(cur -> Next -> Object -> _ID);
        } //fi
      } //fi
      cur= cur -> Next;
    } //eow
  } //fi
  return(NULL);
} //eof NRT_Object_List::GetNext()


int NRT_Object_List::GetNewObjectID() {
  return(_New_Object_ID);
} //eof NRT_Object_List::GetNewObjectID



void NRT_Object_List::PurgeList() {
  NRT_Object_List::ObjectNode  *cur= NULL;

  while (_Head != NULL) {
    cur= _Head -> Next;
    delete _Head;
    _Head= cur;
  } //eow
} //eof NRT_Object_List::PurgeList()


void NRT_Object_List::RemoveNode(int ID) {
  NRT_Object_List::ObjectNode	*cur= NULL;
  NRT_Object_List::ObjectNode	*pre= NULL;

  if (_Head) {
    if (_Head -> Object -> _ID == ID) {
      //If the head is the ID
      cur= _Head;
      _Head= _Head -> Next;
      delete cur;
    } else {
      cur= _Head;
      pre= _Head;
      cur= cur -> Next;
      while (cur) {
        if (cur -> Object -> _ID == ID) {
          if (cur -> Next == NULL) {
            delete cur;
            cur= NULL;
            pre -> Next= NULL;
          } else {
            pre -> Next= cur -> Next;
          } //fi
        } //fi
        pre= cur;
        if (cur) cur= cur -> Next;
      } //eow
    } //fi
  } //fi
} //eof NRT_Object_List::RemoveNode()
