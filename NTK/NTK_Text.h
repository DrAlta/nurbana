/****************************************************
 * NTK - Text Renderer
 * Start Date: Nov 6, 2000
 * Last Updated: Apr 12, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Generates a Graphical String of Text
 *****************************************************/


#ifndef NTK_Text_H
#define NTK_Text_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ntku_Bresenham.h"
#include "ntku_include.h"
#include "../Objects/Bezier.h"
#include "../NurbanaMath.h"
#include "../Endian.h"

#ifndef NGLOBAL_H
#include "../NDefs/NGlobal.h"
#endif

#define FontWidth		8
#define FontHeight		15


#define	NTK_JUSTIFY_LEFT	0
#define	NTK_JUSTIFY_CENTER	1
#define	NTK_JUSTIFY_RIGHT	2


class NTK_Text {
  public:
    struct Descriptor {
      unsigned	char	Symbol;
      int		Pieces;
      int		P1X;
      int		P1Y;
      int		P2X;
      int		P2Y;
    }; //eos Descriptor

    //Font Descriptor Node
    struct FontDNode {
      int		Pts[3][2];
      FontDNode		*Next;
    }; //eos FontDNode

    struct FontCacheNode {
      short		FontSize;
      unsigned	int	TexIDS[128];
      FontCacheNode	*Next;
    }; //eos FontCacheNode

    NTK_Text::NTK_Text(char *Path);
    NTK_Text::~NTK_Text();
    int		Draw(int PosX, int PosY, int SizX, int SizY, char Text[], int Align, char Color[3], bool State);
    int		CharWidth(int SizX, int SizY);


  private:
    void		GenerateFont(int W, int H);

    FontDNode		*_Text[128];
    FontCacheNode	*_FontCacheHead;
}; //eof class NTK_Text
#endif
