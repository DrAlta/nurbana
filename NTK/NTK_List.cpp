#include "NTK_List.h"

#define	BoxHeight	18

NTK_List::NTK_List(NTK_Window *win, int PosX, int PosY, int SizX, int SizY) : NTK_Base(win, PosX, PosY, SizX, SizY, NTK_WT_List) {
  _Head= _Tail= NULL;
  _Total= _Value= 0;
  _Selection[0]= NULL;
} //eof NTK_List::NTK_List()


NTK_List::~NTK_List() {
  ListNode	*cur;

  while (_Head) {
    cur= _Head -> Next;
    delete _Head;
    _Head= cur;
  } //eof
  delete (_Tail);
} //eof NTK::~NTK_List()


void NTK_List::Draw() {
  ListNode	*Node;
  char 		ColorB[3]= {0x33, 0x55, 0x88};
  int		i, Fit, LowerBound, UpperBound;


  ntku_Button::Rounded(_pX, _pY, _sX, _sY, _Color, 2, 1);
  LowerBound= UpperBound= i= 0;

  //Check if Bounds should be set
  if (_Total*BoxHeight >= _sY) {
    Fit= (_sY/BoxHeight);
    LowerBound= _Value*(_Total-Fit);
    UpperBound= _Value*(_Total-Fit) + (_sY/BoxHeight) - 2;
  } //fi

  for (Node= _Head; Node; Node= Node -> Next) {
    if (UpperBound) {
      if ((i >= LowerBound) && (i <= UpperBound)) {
        if (!strcmp(_Selection,Node -> _Text)) {
          ntku_Button::Rect(_pX+4, _pY+(i-LowerBound)*BoxHeight+5, _sX-8, BoxHeight+1, ColorB, 0, 1);
          _Font -> Draw(_pX+5, _pY+(i-LowerBound)*BoxHeight+5, _sX-5, BoxHeight, Node -> _Text, NTK_JUSTIFY_LEFT, ColorB, 1);
        } else {
          _Font -> Draw(_pX+5, _pY+(i-LowerBound)*BoxHeight+5, _sX-5, BoxHeight, Node -> _Text, NTK_JUSTIFY_LEFT, _Color, 1);
        } //fi
      } //fi
    } else { 
      if (!strcmp(_Selection,Node -> _Text)) {
        ntku_Button::Rect(_pX+4, _pY+i*BoxHeight+5, _sX-8, BoxHeight+1, ColorB, 0, 1);
        _Font -> Draw(_pX+5, _pY+i*BoxHeight+5, _sX-5, BoxHeight, Node -> _Text, NTK_JUSTIFY_LEFT, ColorB, 1);
      } else {
        _Font -> Draw(_pX+5, _pY+i*BoxHeight+5, _sX-5, BoxHeight, Node -> _Text, NTK_JUSTIFY_LEFT, _Color, 1);
      } //fi
    } //fi
    i++;
  } //eof
} //eof NTK_List::Draw()



bool NTK_List::Handle(unsigned char Event, int A, int B, int C, int D) {
  ListNode	*Node;
  int	i, Fit, LowerBound, UpperBound;

  LowerBound= UpperBound= i= 0;

  //Check if Bounds should be set
  if (_Total*BoxHeight >= _sY) {
    Fit= (_sY/BoxHeight);
    LowerBound= _Value*(_Total-Fit);
    UpperBound= _Value*(_Total-Fit) + (_sY/BoxHeight) - 1;
  } //fi


  switch( Event ) {
    case NTK_EVENT_MOUSE:
      if (B) {
        for (Node= _Head; Node != NULL; Node= Node -> Next) {
          if (UpperBound) {
            if ((i >= LowerBound) && (i <= UpperBound)) {
              if ((C > _pX+5) && (C < _pX+_sX-5) && (D > _pY+(i-LowerBound)*BoxHeight+5) && (D < _pY+(i-LowerBound)*BoxHeight+5+BoxHeight)) {
                _State= 1;
                strcpy(_Selection, Node -> _Text);
              } //fi
            } //fi
          } else { 
            if ((C > _pX+5) && (C < _pX+_sX-5) && (D > _pY+i*BoxHeight+5) && (D < _pY+i*BoxHeight+5+BoxHeight)) {
              _State= 1;
              strcpy(_Selection, Node -> _Text);
            } //fi
          } //fi
          i++;
        } //eof

        //Check to see if Selection will take place
        _Update= 2;
      } //fi
      break;

    default:
      break;
  } //eos
  return(0);
} //eof NTK_List::Handle()


bool NTK_List::Push(char *T) {
  ListNode	*Node= NULL;
  ListNode	*Temp= NULL;
  bool		Complete= 0;

  if (T) {
    Node= new ListNode;
    strcpy(Node -> _Text,T);
    if (!_Head) {
      // First Element
      _Head= Node;
      _Head -> Prev= NULL;
      _Head -> Next= NULL;
    } else {
      int	Length, i;
      bool	Complete= 0;
      bool	Less;

      for (Temp= _Head; Temp  && !Complete; Temp= Temp -> Next) {
        // Find shortest string
        Length= strlen(Temp -> _Text);
        if (int(strlen(Node -> _Text)) < Length)
          Length= strlen(Node -> _Text);

        Less= 0;
        for (i= 0; i < Length; i++) {
          if (Node -> _Text[i] < Temp -> _Text[i]) {
            Less= 1;
            break;
          } else if (Node -> _Text[i] > Temp -> _Text[i]) {
            Less= 0;
            break;
          } //fi
        } //eof

        if (Less) {
          // Insert Node Here
          if (Temp == _Head) {
            _Head= Node;
            _Head -> Prev= NULL;
            _Head -> Next= Temp;
            Temp -> Prev= _Head;
          } else {
            if (Temp -> Prev)
              Temp -> Prev -> Next= Node;

            Node -> Prev= Temp -> Prev;
            Node -> Next= Temp;
            Temp -> Prev= Node;
          } //fi
          Complete= 1;
        } else {
          // We're at the End so add it
          if (!Temp -> Next) {
            Temp -> Next= Node;
            Node -> Prev= Temp;
            Node -> Next= NULL;
            Complete= 1;
          } //fi
        } //fi
      } //eof

    } //fi
    _Total++;
  } //fi

  _Update= 2;
  if (T) return(1);
  return(0);
} //eof NTK_List::Push()


void NTK_List::PurgeList() {
  ListNode	*Node= NULL;

  while (_Head) {
    Node= _Head -> Next;
    delete _Head;
    _Head= Node;
  } //eow

  _Total= 0;
  _Selection[0]= NULL;
  _Update= 2;
} //eof NTK_List::PurgeList()
