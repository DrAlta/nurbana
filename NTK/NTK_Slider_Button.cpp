#include "NTK_Slider_Button.h"


NTK_Slider_Button::NTK_Slider_Button(NTK_Window *win, int PosX, int PosY, int SizX, int SizY) : NTK_Valuator(win, PosX, PosY, SizX, SizY, NTK_WT_Slider_Button) {
  _Step= 0;

  _TextInput= new NTK_TextInput(win,PosX,PosY,SizX,SizY);
  _TextInput -> Visible(0);
  _TextInput -> Layer(1);
  _TextInput -> Color(0xA0,0xA0,0xA0);
  _TextInput -> CallbackTrigger(NTK_CT_MOUSE_DOWN | NTK_CT_KEY);
} //eof NTK_Slider_Button::NTK_Slider_Button()

NTK_Slider_Button::~NTK_Slider_Button() {}



void NTK_Slider_Button::Draw() {
  char	temp[32];
  int	i,Prec;

  if (!_TextInput -> Visible()) {
    //Draw button
    if (_Rounded) {
      ntku_Button::Rounded(_pX,_pY,_sX,_sY,_Color,2,_State);
    } else {
      ntku_Button::Rect(_pX,_pY,_sX,_sY,_Color,2,_State);
    } //fi

    strcpy(temp,_StaticText);
    strcat(temp,"%.");
    sprintf(_Text,"%d",_Precision);
    strcat(temp,_Text);
    strcat(temp,"f");

    Prec= 1;
    for (i= 0; i < _Precision; i++) Prec*= 10;

    sprintf(_Text, temp, float(int(_Value*Prec))/float(Prec));

    _Font -> Draw(_pX,_pY,_sX,_sY,_Text,NTK_JUSTIFY_CENTER,_Color,_State);
  } //fi
} //eof NTK_Slider_Button::Draw()



bool NTK_Slider_Button::Handle(unsigned char Event, int A, int B, int C, int D) {
  char		temp[32],temp2[32];
  int		i,Prec;

  switch( Event) {
    case NTK_EVENT_MOUSE:
      if (B) {
        if (_TextInput -> Visible()) {
          _TextInput -> Visible(0);
          _Value= _TextInput -> Value();
        } else {
          if (KeyTable::Key() == SDLK_LSHIFT || KeyTable::Key() == SDLK_RSHIFT) {
            _TextInput -> Visible(1);
            _TextInput -> Status(1);
            _State= 1;

            strcpy(temp,"%.");
            sprintf(_Text,"%d",_Precision);
            strcat(temp,_Text);
            strcat(temp,"f");

            Prec= 1;
            for (i= 0; i < _Precision; i++) Prec*= 10;
            sprintf(temp2, temp, float(int(_Value*Prec))/float(Prec));
            _TextInput -> Text(temp2);
          } else {
            if (C > _pX + _sX/2) {
              _Value+_Step > _Max ? _Value= _Max : _Value+= _Step;
            } else {
              _Value-_Step < _Min ? _Value= _Min : _Value-= _Step;
            } //fi

            _MouseX= C;
            _State= 1;
            _Update= 2;
          } //fi
        } //fi
      } else {
        if (!_TextInput -> Visible()) _State= 0;
        _Update= 2;
      } //fi
      break;

    case NTK_EVENT_MOTION:
      if (_State) {
        if (A > _MouseX+10) {
          _Value+_Step > _Max ? _Value= _Max : _Value+= _Step;
          _MouseX= A;
          _Update= 2;
        } else if (A < _MouseX-10) {
          _Value-_Step < _Min ? _Value= _Min : _Value-= _Step;
          _MouseX= A;
          _Update= 2;
        } //fi
      } //fi
      break;

    case NTK_EVENT_KEY:
      if (KeyTable::Key() == SDLK_RETURN) {
        _TextInput -> Visible(0);
        _Value= _TextInput -> Value();
        if (_Value > _Max) _Value= _Max;
        if (_Value < _Min) _Value= _Min;
        _State= 0;
        _Update= 2;
      } //fi
      break;

    default:
      break;
  } //eos
  return(0);
} //eof NTK_Slider_Button::Handle()


void NTK_Slider_Button::StaticText(char *T) {
  strcpy(_StaticText,T);
  _TextInput -> StaticText(T);
} //eof NTK_Slider_Button::StaticText()