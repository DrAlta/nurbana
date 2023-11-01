#include "NTK_Group.h"

NTK_Group::~NTK_Group() {
  NTK_ButtonGroup	*cur;

  while (Head) {
    delete Head -> Button;
    cur= Head -> Next;
    delete Head;
    Head= cur;
  } //eow
} //eof NTK_Group::~NTK_Group()

void NTK_Group::Push(NTK_Button *B) {
  NTK_ButtonGroup	*cur;

  if (Head == NULL) {
    Head= new NTK_ButtonGroup;
    Head -> Next= NULL;
  } else {
    //Structured to create a LIFO stack
    cur= Head;
    Head= NULL;
    Head= new NTK_ButtonGroup;
    //Connect the list
    Head -> Next= cur;
  } //fi
  Head -> Button= B;
} //eof NTK_Group::Push()


void NTK_Group::Off() {
  NTK_ButtonGroup	*cur;
  cur= Head;
  while(cur) {
    cur -> Button -> _State= NTK_OFF;
    cur -> Button -> _Update= 2;
    cur= cur -> Next;
  } //eow
} //eof NTK_GroupOff()
