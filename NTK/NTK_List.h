/****************************************************
 * NTK - List
 * Start Date: Mar 12, 2001
 * Author: Justin Shumaker
 *
 * Description:
 * Creates a List for lables
 ****************************************************/


#ifndef NTK_List_H
#define NTK_List_H


#include <string.h>
#include "NTK_Base.h"
#include "NTK_Text.h"
#include "ntku_Button.h"
#include "../KeyTable.h"

class NTK_List : public NTK_Base {
  public:

    struct ListNode {
      char	_Text[256];
      ListNode	*Next;
      ListNode	*Prev;
    };

    NTK_List::NTK_List(NTK_Window *win, int PosX, int PosY, int SizX, int SizY);
    NTK_List::~NTK_List();

    void	Draw();
    bool	Handle(unsigned char Event, int A, int B, int C, int D);
    void	Resize() {}

    int		Total() { return( _Total ); }
    void	PurgeList();
    bool	Push(char *T);
    void	Value(float v) { _Value= v; _Update= 2; }
    void	Selection(char *T) { strcpy(T,_Selection); }

  private:
    int		_Total;
    char	_Selection[256];
    float	_Value;
    ListNode	*_Head,*_Tail;
}; //eof class NTK_List
#endif

