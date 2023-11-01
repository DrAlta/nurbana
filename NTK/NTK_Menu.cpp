#include "NTK_Menu.h"

#define BUTTON_BORDER   2



NTK_Menu::NTK_Menu(NTK_Window *win, int PosX, int PosY, int SizX, int SizY) : NTK_Base(win, PosX, PosY, SizX, SizY, NTK_WT_Menu) {
  _Head= NULL;
  _Total= _Value= 0;
  _Title[0]= NULL;
  _ScrollBar= 0;
  _MenuCallback= NULL;
} //eof NTK_List::NTK_List()


NTK_Menu::~NTK_Menu() {
  PurgeList();
} //eof NTK_Menu::~NTK_Menu()


void NTK_Menu::Draw() {
  MenuNode	*Node;
  char		Color[3]= {0x80, 0x90, 0xA0};
  char		ColorB[3]= {0xA0, 0xA0, 0xA0};
  int		i, Fit, LowerBound, UpperBound;
  float		WIDTH= 1;

  if (_ScrollBar) WIDTH= .9;

  //Draw Title
  ntku_Button::Rect(_pX,_pY,_sX*WIDTH,25,Color,2,0);
  _Font -> Draw(_pX,_pY,_sX*WIDTH,25,_Title,NTK_JUSTIFY_CENTER,Color,0);


  LowerBound= UpperBound= i= 0;
  //Check if Bounds should be set
  if (_Total*20 > _sY-30) {
    Fit= (_sY-30)/20;
    LowerBound= int(_Value*(_Total-Fit));
    UpperBound= int(_Value*(_Total-Fit) + (_sY-30)/20);
  } //fi

  if (_ScrollBar) {
    ntku_Button::Rect(_pX+_sX*WIDTH, _pY, _sX*(1-WIDTH), _sY, Color, BUTTON_BORDER, 1);
    if (_Total*20 > _sY-30)
      ntku_Button::Rect(_pX+_sX*WIDTH+BUTTON_BORDER, _pY+(_sY-_sX*(1-WIDTH)-BUTTON_BORDER*2)*_Value+BUTTON_BORDER, _sX*(1-WIDTH)-BUTTON_BORDER*2, _sX*(1-WIDTH)-BUTTON_BORDER*2, Color, BUTTON_BORDER, _State);
  } //fi


  for (Node= _Head; Node != NULL; Node= Node -> Next) {
    if (UpperBound) {
      if ((i >= LowerBound) && (i < UpperBound)) {
        ntku_Button::Rect(_pX,_pY+(i-LowerBound)*20+30,_sX*WIDTH,20,ColorB,2,0);
        _Font -> Draw(_pX,_pY+1+(i-LowerBound)*20+30,_sX*WIDTH,20,Node -> _Text,NTK_JUSTIFY_CENTER,ColorB,0);
      } //fi
    } else {
      ntku_Button::Rect(_pX,_pY+i*20+30,_sX*WIDTH,20,ColorB,2,0);
      _Font -> Draw(_pX,_pY+1+i*20+30,_sX*WIDTH,20,Node -> _Text,NTK_JUSTIFY_CENTER,ColorB,0);
    } //fi
    i++;
  } //eof
} //eof NTK_List::Draw()



bool NTK_Menu::Handle(unsigned char Event, int A, int B, int C, int D) {
  MenuNode	*Node;
  float		WIDTH= 1;
  int		i, Fit, LowerBound, UpperBound;

  if (_ScrollBar) WIDTH= .9;

  LowerBound= UpperBound= i= 0;
  //Check if Bounds should be set
  if (_Total*20 > _sY-30) {
    Fit= (_sY-30)/20;
    LowerBound= int(_Value*(_Total-Fit));
    UpperBound= int(_Value*(_Total-Fit) + (_sY-30)/20);
  } //fi

  switch( Event ) {
    case NTK_EVENT_MOUSE:
      if (B) {
        //Check if Item is clicked on
        for (Node= _Head; Node != NULL; Node= Node -> Next) {
          if (UpperBound) {
            if ((i >= LowerBound) && (i <= UpperBound)) {
              if ((C > _pX) && (C < _pX+_sX*WIDTH) && (D > _pY+(i-LowerBound)*20+30) && (D < _pY+(i-LowerBound)*20+30+20)) {
                _State= 1;
                _MenuCallback(Node -> _Value);
                _Visible= 0; //After callback, self-deactivate
              } //fi
            } //fi
          } else {
            if ((C > _pX) && (C < _pX+_sX*WIDTH) && (D > _pY+i*20+30) && (D < _pY+i*20+30+20)) {
              _State= 1;
              _MenuCallback(Node -> _Value);
              _Visible= 0; //After callback, self-deactivate
            } //fi
          } //fi
          i++;
        } //eof

        //Check if Scroll Bar Button is clicked on and there is need to draw it
        if ((_ScrollBar) && (_Total*20 > _sY-30)) {
          if ((C > _pX+_sX*WIDTH+BUTTON_BORDER) && (C < _pX+_sX*WIDTH+BUTTON_BORDER + _sX*(1-WIDTH)-BUTTON_BORDER*2) &&
              (D > _pY+(_sY-_sX*(1-WIDTH)-BUTTON_BORDER*2)*_Value+BUTTON_BORDER) && (D < _pY+(_sY-_sX*(1-WIDTH)-BUTTON_BORDER*2)*_Value+BUTTON_BORDER + _sX*(1-WIDTH)-BUTTON_BORDER*2)) {

             _State= 1;
             _CDY= D-(_pY+(_sY-_sX*(1-WIDTH)-BUTTON_BORDER*2)*_Value+BUTTON_BORDER);
//             _CDY= D-(_pY+_sY*_Value+BUTTON_BORDER);
          } //fi
        } //fi


        //Check to see if Selection will take place
        _Update= 2;
      } else {
        _State= 0;
        _Update= 2;
      } //fi
      break;


    case NTK_EVENT_MOTION:
      if (_State) {
        if (B <= _pY+BUTTON_BORDER+_CDY) {
          _Value= 0;
          _Update= 2;
        } else if (B >= _pY+_sY-(int(_sX*(1-WIDTH))-BUTTON_BORDER*2)+_CDY) {
          _Value= 1;
          _Update= 2;
        } else {
          _Value= float(B-_pY-_CDY)/float(_sY-(_sX*(1-WIDTH)-BUTTON_BORDER*2));
          _Update= 2;
        } //fi

      } //fi
      break;


    default:
      break;
  } //eos
  return(0);
} //eof NTK_Menu::Handle()


bool NTK_Menu::Push(char *T, int V) {
  MenuNode	*Node= NULL;

  if (T) {
    if (_Head) {
      Node= _Head;
      _Head= new MenuNode;
      _Head -> Next= Node;
    } else {
      _Head= new MenuNode;
      _Head -> Next= NULL;
    } //fi
    strcpy(_Head -> _Text,T);
    _Head -> _Value= V;
    _Total++;
    return(1);
  } //fi

  _Update= 2;
  return(0);
} //eof NTK_Menu::Push()


void NTK_Menu::Pop(int V) {
  MenuNode	*Node= NULL;
  MenuNode	*Last= NULL;

  while (Node) {
    if (Node -> _Value == V) {
      Last -> Next= Node -> Next;
      delete Node;
      break;
    } //fi
    Last= Node;
    Node= Node -> Next;
  } //eow
} //eof NTK_Menu::Pop()


void NTK_Menu::PurgeList() {
  MenuNode	*Node= NULL;

  while (_Head) {
    Node= _Head -> Next;
    delete _Head;
    _Head= Node;
  } //eow

  _Total= 0;
  _Update= 2;
} //eof NTK_Menu::PurgeList()


int NTK_Menu::GetFreeID() {
  MenuNode	*Node= NULL;
  int		i;

  i= 1;
  for (Node= _Head; Node != NULL; Node= Node -> Next) {
    if (i == Node -> _Value) {
      i++;
      Node= _Head;
    } //fi
  } //eof

  return(i);
} //eof NTK_Menu::GetFreeID()


void NTK_Menu::Text(int V, char Text[]) {
  MenuNode	*Node= NULL;

  for (Node= _Head; Node != NULL; Node= Node -> Next) {
    if (Node -> _Value == V) {
      strcpy(Node -> _Text,Text);
      return;
    } //fi
  } //eof
} //eof NTK_Menu::Text()

