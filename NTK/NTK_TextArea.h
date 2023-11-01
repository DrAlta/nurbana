/****************************************************
 * NTK - Text Input Boxes
 * Start Date: Nov 6, 2000
 * Author: Justin Shumaker
 *
 * Description:
 * Text Input Boxes
 *****************************************************/


#ifndef NTK_TextArea_H
#define NTK_TextArea_H

#include <string.h>
#include "NTK_Base.h"
#include "NTK_Text.h"
#include "NTK_ScrollBar.h"
#include "ntku_Button.h"
#include "../KeyTable.h"


#define	NTK_TextArea_ScrollBarWidth	20


class NTK_TextArea : public NTK_Base {
  public:
    NTK_TextArea(NTK_Window *win, int PosX, int PosY, int SizX, int SizY);
    ~NTK_TextArea();

    struct TextLine {
      char	*Text;
      bool	LF;
      TextLine	*Prev;
      TextLine	*Next;
    };


    void		Draw();
    bool		Handle(unsigned char Event, int A, int B, int C, int D);
    void		Resize();

    void		Text(char *Text);
    char*		Text();
    void		FontSize(unsigned short S) { _FontSize= S; }
    void		WordWrap(bool W) { _WordWrap= W; }

  private:
    bool		_Load;
    bool		_WordWrap;
    unsigned	short	_FontSize;
    int			_Index;
    int			_Line;

    TextLine		*_CurLine;
    char		*_Buffer;
    NTK_ScrollBar	*_ScrollBar;

}; //eof class NTK_Area
#endif
