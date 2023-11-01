#include "NRT_Light_List.h"


NRT_Light_List::NRT_Light_List() {
  _New_Light_ID= 0;
  _Head= NULL;
} //eof NRT_Light_List::NRT_Light_List()


NRT_Light_List::~NRT_Light_List() {
  NRT_Light_List::LightNode	*cur;

  while (_Head) {
    cur= _Head -> Next;
    delete _Head;
    _Head= cur;
  } //eow
} //eof NRT_Light_List::~NRT_Light_List()




void NRT_Light_List::Push(NRT_Light_Base *obj, int ID) {
  //Add the Object to the list
  NRT_Light_List::LightNode *cur;

  if (_Head == NULL) {
    _Head= new NRT_Light_List::LightNode;
    _Head -> Next= NULL;
  } else {
    //Structured to create a LIFO stack
    cur= _Head;
    _Head= NULL;
    _Head= new NRT_Light_List::LightNode;

    //Connect the list
    _Head -> Next= cur;
  } //fi

  _Head -> Object= obj;
  _Head -> Object -> _ID= ID;
  _New_Light_ID= ID;
} //eof NRT_Light_List::Push()



NRT_Light_List::LightNode* NRT_Light_List::GetHead() {
  if (_Head) {
    return(_Head);
  } //fi
  return(NULL);
} //eof NRT_Light_List::GetHead()



NRT_Light_List::LightNode* NRT_Light_List::GetLightPtr(int ID) {
  NRT_Light_List::LightNode	*cur= NULL;

  if (_Head) {
    cur= _Head;
    while (cur != NULL) {
      if (cur -> Object -> _ID == ID) return(cur);
      cur= cur -> Next;
    } //eow
  } //fi
  return(NULL);
} //eof NRT_Light_List::GetLightPtr()



int NRT_Light_List::GetNext(int ID) {
  NRT_Light_List::LightNode  *cur= NULL;

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
} //eof NRT_Light_List::GetNext()


int NRT_Light_List::GetNewLightID() {
  return(_New_Light_ID);
} //eof NRT_Light_List::GetNewLightID()



void NRT_Light_List::PurgeList() {
  NRT_Light_List::LightNode  *cur= NULL;

  while (_Head != NULL) {
    cur= _Head -> Next;
    delete _Head;
    _Head= cur;
  } //eow
} //eof NRT_Light_List::PurgeList()


void NRT_Light_List::RemoveNode(int ID) {
  NRT_Light_List::LightNode  *cur= NULL;
  NRT_Light_List::LightNode  *pre= NULL;

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
} //eof NRT_Light_List::RemoveNode()
