/****************************************************
 * NTK - Button
 * Start Date: Nov 6, 2000
 * Author: Justin Shumaker
 *
 * Description:
 * Draws a Button with a Border
 ****************************************************/


#ifndef NTK_Button_H
#define NTK_Button_H

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include "NTK_Base.h"
#include "NTK_Group.h"
#include "NTK_Text.h"
#include "ntku_Button.h"


#define NTK_BUTTON_TEXT		0
#define NTK_BUTTON_GLYPHIC	1


class NTK_Group;
class NTK_Button : public NTK_Base {
  public:
    NTK_Button::NTK_Button(NTK_Window *win, int PosX, int PosY, int SizX, int SizY);
    NTK_Button::~NTK_Button();

    void	Draw();
    bool	Handle(unsigned char Event, int A, int B, int C, int D);
    void	Resize() {}

    void	Type(int T)
		{ _Type= T; }

    void	Text(char T[])
		{ strcpy(_Text,T); }

    void	Glyphic(NTK_Glyphic G);

    void	StretchGlyphic(bool V)
		{ _StretchGlyphic= V; }

    void	Sticky(bool S) { _Sticky= S; }
    void	Group(NTK_Group *G);
    void	GroupOff(void);

  private:
    bool		_StretchGlyphic;
    bool		_Sticky;
    unsigned	char	_Type;
    unsigned	char	_BorderSize;
    char		_Text[256];
    NTK_Group		*_Group;

    NTK_Glyphic		_Glyphic;

}; //eof class NTK_Button
#endif
