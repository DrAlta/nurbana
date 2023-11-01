/****************************************************
 * NTK - Window
 * Start Date: Nov 6, 2000
 * Author: Justin Shumaker
 *
 * Description:
 * Stores a list of pointers to each widget
 ****************************************************/


#ifndef NTK_Window_H
#define NTK_Window_H

#include "../NGUtils.h"
#include "NTK_Base.h"
#include "NTK_Text.h"
#include "ntku_include.h"
#include "ntku_Glyphic.h"
#include "../KeyTable.h"
#include "../NDefs/NTKDefs.h"

#define NTK_CCRed	0.70
#define NTK_CCGreen	0.70
#define NTK_CCBlue	0.70

class NTK_Base;
class NTK_Window {
  public:

    struct WidgetNode {
      NTK_Base		*Widget;
      WidgetNode	*Next;
    };


    NTK_Window(char *Path);
    ~NTK_Window();

    void		Push(NTK_Base *Widget);
    void		Handle(char Event, int A, int B, int C, int D);
    void		Draw();
    void		HideAll();
    void		UpdateAll();
    void		SetClear(char V) { _Clear= V; }
    void		ScreenSize(int W, int H);
    void		Widgets_Off();
    bool		TypeActive(char T);

    NTK_Glyphic		GlyphicXPM(char **G, bool Linear);
    NTK_Glyphic		GlyphicRAW(unsigned char *G, unsigned int W, unsigned int H, bool Linear);

    NTK_Text*		Font() { return( _Font ); }
    NTK_Window*		Window() { return( this ); }

  private:
    void		GenerateActWBB(int TLBR[4]);
    void		Scene(bool S) { _Scene= S; }

    WidgetNode		*Head;
    NTK_Text		*_Font;
    char		_Clear;
    int			_Width;
    int			_Height;
    int			_OfsetX;
    int			_OfsetY;
    int			_Mouse[2];
    bool		_Scene;

}; //eof class NTK_Window

#endif
