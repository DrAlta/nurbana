/****************************************************
 * NTK - Text Input Boxes
 * Start Date: Nov 6, 2000
 * Author: Justin Shumaker
 *
 * Description:
 * Text Input Boxes
 *****************************************************/


#ifndef NTK_TextInput_H
#define NTK_TextInput_H

#include <string.h>
#include "NTK_Base.h"
#include "NTK_Text.h"
#include "ntku_Button.h"
#include "../KeyTable.h"

class NTK_TextInput : public NTK_Base {
  public:
    NTK_TextInput(NTK_Window *win, int PosX, int PosY, int SizX, int SizY);
    ~NTK_TextInput();

    void	Draw();
    bool	Handle(unsigned char Event, int A, int B, int C, int D);
    void	Resize() {}

    void	Text(char T[]) { strcpy(_Text,T); _Index= strlen(_Text); _Update= 2; };
    void	GetText(char *S) { strcpy(S,_Text); }
    void	FullText(char *S);
    void	StaticText(char ST[]) { strcpy(_StaticText,ST); _Update= 2; };
    float	Value() { return( atof(_Text) ); }
    void	Value(float V);

  private:
    char	_Text[256];
    char	_StaticText[256];
    int		_Index;

}; //eof class NTK_TextInput
#endif
