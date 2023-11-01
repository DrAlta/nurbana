#include "NTK_Slider_Vert.h"


NTK_Slider_Vert::NTK_Slider_Vert(NTK_Window *win, int PosX, int PosY, int SizX, int SizY) : NTK_Valuator(win, PosX, PosY, SizX, SizY, NTK_WT_Slider_Vert) {
} //eof NTK_Slider_Vert::NTK_Slider_Vert()

NTK_Slider_Vert::~NTK_Slider_Vert() {}





void _DrawVertSlider(int _pX, int _pY, int _sX, int _sY, char _Color[3], char _SliderColor[3], int _Precision, float _Value, float _Min, float _Max, char _Text[64], bool _State) {
  int		digitsV,digitsM,i,dispX;
  char		temp[32];
  float		min, max, valueB;
  /* 3--2
   * |  |
   * 4--1
   */

  min= 0;
  max= _Max - _Min;
  valueB= _Value - _Min;


    digitsV= ntku_Math::NumMag(valueB);
    digitsM= ntku_Math::NumMag(max);

    if (_Precision) {
      dispX= (digitsM+1+_Precision+1)*FontWidth;
    } else {
      dispX= (digitsM+_Precision+1)*FontWidth;
    } //fi

    if (_sX < dispX) {_sX= dispX;}

    ntku_Button::Rect(_pX, _pY, _sX, _sY, _Color, 2, _State);
      glBegin(GL_QUADS);
        /*** DRAW BLACK CREVICE ***/
        glColor3f(0.15, 0.15, 0.15);
         glVertex2f(_pX+_sX*.6,_pY+_sY*.95);
         glVertex2f(_pX+_sX*.6,_pY+FontHeight+6);
         glVertex2f(_pX+_sX*.4,_pY+FontHeight+6);
         glVertex2f(_pX+_sX*.4,_pY+_sY*.95);

        /*** TOP ***/
        glColor3f(0.3,0.3,0.3);
         glVertex2f(_pX+_sX*.6,_pY+FontHeight+6);
         glVertex2f(_pX+_sX*.6,_pY+FontHeight+6-BorderSize);
         glVertex2f(_pX+_sX*.4,_pY+FontHeight+6-BorderSize);
         glVertex2f(_pX+_sX*.4,_pY+FontHeight+6);

        /*** BOTTOM ***/
        glColor3f(0.6,0.6,0.6);
         glVertex2f(_pX+_sX*.6,_pY+_sY*.95+BorderSize);
         glVertex2f(_pX+_sX*.6,_pY+_sY*.95);
         glVertex2f(_pX+_sX*.4,_pY+_sY*.95);
         glVertex2f(_pX+_sX*.4,_pY+_sY*.95+BorderSize);

        /*** LEFT ***/
        glColor3f(0.3,0.3,0.3);
         glVertex2f(_pX+_sX*.4,_pY+_sY*.95+BorderSize);
         glVertex2f(_pX+_sX*.4,_pY+FontHeight+6-BorderSize);
         glVertex2f(_pX+_sX*.4-BorderSize,_pY+FontHeight+6-BorderSize);
         glVertex2f(_pX+_sX*.4-BorderSize,_pY+_sY*.95+BorderSize);

        /*** RIGHT ***/
        glColor3f(0.6,0.6,0.6);
         glVertex2f(_pX+_sX*.6+BorderSize,_pY+_sY*.95+BorderSize);
         glVertex2f(_pX+_sX*.6+BorderSize,_pY+FontHeight+6-BorderSize);
         glVertex2f(_pX+_sX*.6,_pY+FontHeight+6-BorderSize);
         glVertex2f(_pX+_sX*.6,_pY+_sY*.95+BorderSize);

        /*** SLIDER BUTTON ***/
        glColor3ub(_SliderColor[0], _SliderColor[1], _SliderColor[2]);
         glVertex2f(_pX+_sX*.6,FontHeight+6+_pY+(_sY*.95-FontHeight+6)*(valueB/max)+5);
         glVertex2f(_pX+_sX*.6,FontHeight+6+_pY+(_sY*.95-FontHeight+6)*(valueB/max)-5);
         glVertex2f(_pX+_sX*.4,FontHeight+6+_pY+(_sY*.95-FontHeight+6)*(valueB/max)-5);
         glVertex2f(_pX+_sX*.4,FontHeight+6+_pY+(_sY*.95-FontHeight+6)*(valueB/max)+5);
      glEnd();

     sprintf(temp, "%f", _Value);
     for (i= 0; i < int(strlen(temp)); i++) _Text[i]= 48;
     if (_Value < 1) {
       for (i= 0; i < int(strlen(temp)); i++) _Text[i+digitsM-digitsV-1]= temp[i];
     } else {
       for (i= 0; i < int(strlen(temp)); i++) _Text[i+digitsM-digitsV]= temp[i];
     } //fi

     //Now that Buttons[ID].Text has the string we must format it according
     //to its specified precision.  We already calculated # of digits up top
     if (_Precision) {
       _Text[digitsM+1+_Precision]= NULL;
     } else {
       _Text[digitsM]= NULL;
     } //fi
//     ntku_Text::Draw(_pX, _pY, _sX, FontHeight+3, _Text, NTK_JUSTIFY_LEFT, _Color, _State);
} //eof _DrawHorzSlider()


void NTK_Slider_Vert::Draw() {
  _DrawVertSlider(_pX,_pY,_sX,_sY,_Color,_SliderColor,_Precision,_Value,_Min,_Max,_Text,_State);
} //eof NTK_Slider_Vert::Draw()



bool NTK_Slider_Vert::Handle(char Event, int A, int B, int C, int D) {
  int digitsV, digitsM, dispX, dispY;
  float valueA;
  float min, max, valueB;

  min= 0;
  max= _Max - _Min;
  valueB= _Value - _Min;

  switch( Event) {
    case NTK_EVENT_MOUSE:
      if (B) {
        digitsV= ntku_Math::NumMag(valueB);
        digitsM= ntku_Math::NumMag(max);

        if (_Precision) {
          dispX= (digitsM+1+_Precision+1)*FontWidth;
        } else {
          dispX= (digitsM+1)*FontWidth;
        } //fi

        if (_sX < dispX) {_sX= dispX;}

        if ((C >= (_pX+_sX*.4)) &&
            (C <= (_pX+_sX*.6)) &&
            (D >= (FontHeight+6+_pY+(_sY*.95-FontHeight+6)*(valueB/max)-5)) &&
            (D <= (FontHeight+6+_pY+(_sY*.95-FontHeight+6)*(valueB/max)+5))) {
          _State= 1;
          _Update= 2;
          LM= D;

        } //fi
      } else {
        _State= 0;
        _Update= 2;
      } //fi
      break;

    case NTK_EVENT_MOTION:
      if (_State) { //IF BUTTON IS ON

        digitsV= ntku_Math::NumMag(valueB);
        digitsM= ntku_Math::NumMag(max);

        if (_Precision) {
          dispX= (digitsM+1+_Precision+1)*FontWidth;
        } else {
          dispX= (digitsM+1)*FontWidth;
        } //fi


        //Just testing X's cause mouse click verified slider is active
        if ((B >= _pY+FontHeight+6) && (B <= _pY+_sY*.95)) { //IF MOUSE IS WITHIN SLIDER LIMITS
          //Update Slider
          _Update= 2;

          //Add or Subtract the difference proportional to last mouse movement
          valueA= (B-LM)/(_sY*.95-FontHeight+6);
          valueA*= max;

          if ((valueA+valueB < max) && (valueA+valueB > min)) {
            valueB+= valueA;
          } else {
            if (valueA+valueB > max) {
              valueB= max;
            } else if (valueA+valueB < min) {
              valueB= min;
            } //fi
          } //fi
        } else if (B >= _pY+_sY*.95) {
          //If the mouse _IS_ out of slider boundaries, then assign its
          //value to the Max or Min respectively.  This is good if the
          //mouse _exits_ the slider region before reaching its respective
          //Max or Min value.
          valueB= max;
          _Update= 2;
        } else if (B <= _pY+FontHeight+6) {
          //Same as above
          valueB= min;
          _Update= 2;
        } //fi
        _Value= (valueB + _Min);
      } //fi

      LM= B;
      break;

    default:
      break;
  } //eos
  return(0);
} //eof NTK_Slider_Vert::Handle()
