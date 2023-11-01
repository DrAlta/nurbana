#include "MaterialList.h"

MaterialList::MaterialList() {
  _Head= NULL;
  _default_diffuse[0]= _default_specularity[0]= 192;
  _default_diffuse[1]= _default_specularity[1]= 192;
  _default_diffuse[2]= _default_specularity[2]= 192;
  _ActiveMaterial= 0;
} //eof MaterialList::MaterialList()


MaterialList::~MaterialList() {
} //eof MaterialList::MaterialList()


int MaterialList::Create(char *Name) {
  int	ID;
  ID= GetFreeID();
  Create(ID,Name);
  return(ID);
} //eof MaterialList::Create()


void MaterialList::Create(int ID, char *Name) {
  MaterialNode	*cur;

  if (_Head == NULL) {
    _Head= new MaterialNode;
    _Head -> Next= NULL;
  } else {
    //Structured to create a LIFO stack
    cur= _Head;
    _Head= NULL;
    _Head= new MaterialNode;
    //Connect the list
    _Head -> Next= cur;
  } //fi

  _Head -> ID= ID;

  strcpy(_Head -> Name,Name);
} //eof MaterialList::Create()


void MaterialList::Remove(int RID) {
  MaterialNode  *cur= NULL;
  MaterialNode  *pre= NULL;

  if (_Head) {
    if (_Head -> ID == RID) {
      //If the head is the ID
      cur= _Head;
      _Head= _Head -> Next;
      delete cur;
    } else {
      cur= _Head;
      pre= _Head;
      cur= cur -> Next;
      while (cur) {
        if (cur -> ID == RID) {
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

  if (_Head)
    _ActiveMaterial= _Head -> ID;
} //eof MaterialList::Remove()


int MaterialList::GetFreeID() {
  MaterialNode  *cur= NULL;
  int		ID= 1;

  cur= _Head;
  while (cur) {
    if (cur -> ID >= ID) ID= cur -> ID+1;
    cur= cur -> Next;
  } //eow

  return(ID);
} //eof MaterialList::GetFreeID()


MaterialList::MaterialNode* GetNode(int ID, MaterialList::MaterialNode* _Head) {
  MaterialList::MaterialNode *Node;

  Node= _Head;
  while (Node) {
    if (Node -> ID == ID) return(Node);
    Node= Node -> Next;
  } //eow
  
  return(NULL);
} //eof GetNode()


unsigned char* MaterialList::Diff(int ID) {
  MaterialNode	*cur;

  if ((cur= GetNode(ID,_Head)))
    return(cur -> diff);

  return(_default_diffuse);
} //eof MaterialList::Diffuse()


unsigned char* MaterialList::Spec(int ID) {
  MaterialNode	*cur;

  if ((cur= GetNode(ID,_Head)))
    return(cur -> spec);

  return(_default_specularity);
} //eof MaterialList::Spec()


char* MaterialList::Name(int ID) {
  MaterialNode	*cur;

  if ((cur= GetNode(ID,_Head))) {
    return( cur -> Name );
  } else {
    return( "None" );
  } //fi
} //eof MaterialList::GetName()


void MaterialList::GetName(int ID, char *Text) {
  MaterialNode	*cur;

  if ((cur= GetNode(ID,_Head))) {
    strcpy(Text, cur -> Name);
  } else {
    strcpy(Text, "None");
  } //fi
} //eof MaterialList::GetName()


void MaterialList::SetName(int ID, char *Text) {
  MaterialNode	*cur;

  if ((cur= GetNode(ID,_Head))) {
    strcpy(cur -> Name, Text);
  } //fi
} //eof MaterialList::SetName()


bool MaterialList::GetHead(unsigned int &ID) {
  if (_Head) {
    ID= _Head -> ID;
    return(1);
  } //fi
  return(0);
} //eof MaterialList::GetHead();


bool MaterialList::GetNext(unsigned int &ID) {
  MaterialNode	*cur;

  if ((cur= GetNode(ID,_Head))) {
    if (cur -> Next) {
      ID= cur -> Next -> ID;
      return(1);
    } //fi
  } //fi
  return(0);
} //eof MaterialList::GetNext()
