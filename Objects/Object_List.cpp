#include "Object_List.h"


Object_List::Object_List() {
  _ID_Counter= _New_Object_ID= 0;
  _Head= NULL;
} //eof Constructor


Object_List::~Object_List() {
  //Delete all Objects
} //eof Deconstructor




void Object_List::Push(Object_Base *obj) {
  //Add the Object to the list
  Object_List::ObjectNode *cur;

  if (_Head == NULL) {
    _Head= new Object_List::ObjectNode;
    _Head -> Next= NULL;
  } else {
    //Structured to create a LIFO stack
    cur= _Head;
    _Head= NULL;
    _Head= new Object_List::ObjectNode;

    //Connect the list
    _Head -> Next= cur;
  } //fi

  _Head -> Object= obj;
  _ID_Counter++;
  _Head -> Object -> _ID= _ID_Counter;
  _New_Object_ID= _ID_Counter;
} //eof Object_List::Push()



Object_List::ObjectNode* Object_List::GetHead() {
  if (_Head) {
    return(_Head);
  } //fi
  return(NULL);
} //eof Object_List::GetHead()



Object_List::ObjectNode* Object_List::GetObjectPtr(int ID) {
  Object_List::ObjectNode  *cur= NULL;

  if (_Head) {
    cur= _Head;
    while (cur != NULL) {
      if (cur -> Object -> _ID == ID) return(cur);
      cur= cur -> Next;
    } //eow
  } //fi
  return(NULL);
} //eof Object_List::GetObject()



int Object_List::GetNext(int ID) {
  Object_List::ObjectNode  *cur= NULL;

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
} //eof Object_List::GetNext()


int Object_List::GetNewObjectID() {
  return(_New_Object_ID);
} //eof Object_List::GetNewObjectID



void Object_List::PurgeList() {
  Object_List::ObjectNode  *cur= NULL;

  while (_Head != NULL) {
    cur= _Head -> Next;
    delete _Head -> Object;
    delete _Head;
    _Head= cur;
  } //eow
} //eof Object_List::PurgeList()


void Object_List::RemoveNode(int ID) {
  Object_List::ObjectNode  *cur= NULL;
  Object_List::ObjectNode  *pre= NULL;

  if (_Head) {
    if (_Head -> Object -> _ID == ID) {
      //If the head is the ID
      cur= _Head;
      _Head= _Head -> Next;
      delete cur -> Object;
      delete cur;
    } else {
      cur= _Head;
      pre= _Head;
      cur= cur -> Next;
      while (cur) {
        if (cur -> Object -> _ID == ID) {
          if (cur -> Next == NULL) {
            delete cur -> Object;
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
} //eof Object_List::RemoveNode()
