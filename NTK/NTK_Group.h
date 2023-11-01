/****************************************************
 * NTK - Group
 * Start Date: Dec 8, 2000
 * Author: Justin Shumaker
 *
 * Description:
 * Draws a Group
 ****************************************************/

#ifndef NTK_Group_H
#define NTK_Group_H

#include "NTK_Button.h"

class NTK_Button;
class NTK_Group {
  public:
    NTK_Group::NTK_Group() { Head= NULL; }
    NTK_Group::~NTK_Group();

    void Push(NTK_Button *B);
    void Off();

  private:
    struct NTK_ButtonGroup {
      NTK_Button	*Button;
      NTK_ButtonGroup	*Next;
    };

    NTK_ButtonGroup	*Head;
}; //eof class NTK_Group

#endif
