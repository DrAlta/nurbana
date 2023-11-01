/*===========================================
 * Title: Invert Buffer Class
 * Date Started: 09-13-2000
 * Date Last Modified: Dec 17th 04:35
 * Version: 1.0
 * Author(s): Justin Shumaker
 * Description:
 * Usage:
 *
 *===========================================
 */

#include "InvertBuffer.h"


void InvertBuffer::DrawLine(int ApX, int ApY, int BpX, int BpY) {
  glEnable(GL_COLOR_LOGIC_OP);
  glLogicOp(GL_INVERT);

  glBegin(GL_LINES);
    glVertex2i(ApX,ApY);
    glVertex2i(BpX,BpY);
  glEnd();

  glDisable(GL_COLOR_LOGIC_OP);
} //eof InvertBuffer::DrawLine()


void InvertBuffer::DrawQuad(int ApX, int ApY, int BpX, int BpY, int CpX, int CpY, int DpX, int DpY) {
  //Ordering:
  //32
  //41

  glEnable(GL_COLOR_LOGIC_OP);
  glLogicOp(GL_INVERT);

  glBegin(GL_QUADS);
    glVertex2i(ApX,ApY);
    glVertex2i(BpX,BpY);
    glVertex2i(CpX,CpY);
    glVertex2i(DpX,DpY);
  glEnd();

  glDisable(GL_COLOR_LOGIC_OP);
} //eof InvertBuffer::DrawQuad()


void InvertBuffer::CursorHorzLine(int ApX, int ApY, int BpX, int BpY, int Width) {
  //Left to Right
  if (Width > 1) {
    InvertBuffer::DrawQuad(BpX,BpY+Width/2,BpX,BpY-Width/2,ApX,ApY-Width/2,ApX,ApY+Width/2);
  } else {
    InvertBuffer::DrawLine(ApX,ApY,BpX,BpY);
  } //fi
} //eof InvertBuffer::CursorHorzLine()


void InvertBuffer::CursorVertLine(int ApX, int ApY, int BpX, int BpY, int Width) {
  //Top to Bottom
  if (Width > 1) {
    InvertBuffer::DrawQuad(BpX+Width/2,BpY,ApX+Width/2,ApY,ApX-Width/2,ApY,BpX-Width/2,BpY);
  } else {
    InvertBuffer::DrawLine(ApX,ApY,BpX,BpY);
  } //fi
} //eof InvertBuffer::CursorVertLine()
