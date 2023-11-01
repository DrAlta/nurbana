#include "NWM_List.h"


NWM_List::NWM_List() {
  _ID_Counter= _New_Window_ID= 0;
  _Head= NULL;
} //eof Constructor


NWM_List::~NWM_List() {
  //Delete all Objects
} //eof Deconstructor




void NWM_List::Push(NWM_Base *obj) {
  //Add the Object to the list
  NWM_List::WindowNode *cur;

  if (_Head == NULL) {
    _Head= new NWM_List::WindowNode;
    _Head -> Next= NULL;
  } else {
    //Structured to create a LIFO stack
    cur= _Head;
    _Head= NULL;
    _Head= new NWM_List::WindowNode;

    //Connect the list
    _Head -> Next= cur;
  } //fi

  _Head -> Window= obj;
  _ID_Counter++;
  _Head -> Window -> _ID= _ID_Counter;
  _New_Window_ID= _ID_Counter;
} //eof NWM_List::Push()



NWM_List::WindowNode* NWM_List::GetHead() {
  if (_Head)
    return(_Head);
  return(NULL);
} //eof NWM_List::GetHead()



NWM_List::WindowNode* NWM_List::GetWindowPtr(int ID) {
  NWM_List::WindowNode	*cur= NULL;

  if (_Head) {
    cur= _Head;
    while (cur != NULL) {
      if (cur -> Window -> _ID == ID) return(cur);
      cur= cur -> Next;
    } //eow
  } //fi
  return(NULL);
} //eof NWM_List::GetWindowPtr()



int NWM_List::GetNext(int ID) {
  NWM_List::WindowNode	*cur= NULL;

  if (_Head) {
    cur= _Head;
    while (cur != NULL) {
      if (cur -> Window -> _ID == ID) {
        if (cur -> Next) {
          return(cur -> Next -> Window -> _ID);
        } //fi
      } //fi
      cur= cur -> Next;
    } //eow
  } //fi
  return(NULL);
} //eof NWM_List::GetNext()


int NWM_List::GetNewWindowID() {
  return(_New_Window_ID);
} //eof NWM_List::GetNewWindowID()



void NWM_List::PurgeList() {
  NWM_List::WindowNode	*cur= NULL;

  while (_Head) {
    cur= _Head -> Next;
    delete _Head -> Window;
    delete _Head;
    _Head= cur;
  } //eow
} //eof NWM_List::PurgeList()


void NWM_List::RemoveNode(int ID) {
  NWM_List::WindowNode	*cur1= NULL;
  NWM_List::WindowNode	*cur2= NULL;

  // Pop Node
  if (_Head) {
    if (_Head -> Window -> _ID == ID) {
      cur1= _Head -> Next;
      delete _Head -> Window;
      delete _Head;
      _Head= cur1;
    } else {
      cur1= _Head;
      while (cur1 -> Next) {
        if (cur1 -> Next -> Window -> _ID == ID) {
          cur2= cur1 -> Next;
          cur1 -> Next= cur1 -> Next -> Next;
          delete cur2 -> Window;
          delete cur2;
          break;
        } //fi
        cur1= cur1 -> Next;
      } //eow
    } //fi
  } //fi
} //eof NWM_List::RemoveNode()
