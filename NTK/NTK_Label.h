/****************************************************
 * NTK - Label
 * Start Date: Nov 6, 2000
 * Author: Justin Shumaker
 *
 * Description:
 * Draws labels with or without a bevel
 ****************************************************/


#ifndef NTK_Label_H
#define NTK_Label_H


#include <string.h>
#include "NTK_Base.h"
#include "NTK_Text.h"
#include "ntku_Button.h"

#define	NTK_LABEL_TEXT		0
#define	NTK_LABEL_GLYPHIC	1


class NTK_Label : public NTK_Base {
  public:
    NTK_Label::NTK_Label(NTK_Window *win, int PosX, int PosY, int SizX, int SizY);
    NTK_Label::~NTK_Label();

    void	Draw();
    bool	Handle(unsigned char Event, int A, int B, int C, int D);
    void	Resize() {}

    void	Bevel(bool V) { _Bevel= V; }

    void	Text(char T[]) { strcpy(_Text,T); _Update= 2; }

    void	Glyphic(NTK_Glyphic G);
    void	StretchGlyphic(bool V) { _StretchGlyphic= V; }

    void	Type(char T) { _Type= T; }
    void	Align(unsigned char A) { _Align= A; }

  private:
    bool		_StretchGlyphic;
    bool		_Bevel;
    unsigned	char	_Type;
    unsigned	char	_Align;
    char		_Text[256];

    NTK_Glyphic		_Glyphic;
}; //eof class NTK_Label
#endif
