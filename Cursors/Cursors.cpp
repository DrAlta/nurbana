#include "Cursors.h"
#include "arrow.xpm"
#include "cross.xpm"
#include "updown.xpm"
#include "leftright.xpm"
#include "text.xpm"

SDL_Cursor	*Cursors::Cursor= NULL;

void Cursors::SetCursor(unsigned char C) {
  char	**image= arrow_xpm;

  switch( C ) {
    case CURSOR_ARROW:
      image= arrow_xpm;
      break;

    case CURSOR_CROSSHAIR:
      image= cross_xpm;
      break;

    case CURSOR_UPDOWN:
      image= updown_xpm;
      break;

    case CURSOR_LEFTRIGHT:
      image= leftright_xpm;
      break;

    case CURSOR_TEXT:
      image= text_xpm;
      break;

    default:
      break;
  } //eos


  int i, row, col;
  Uint8 data[4*32];
  Uint8 mask[4*32];
  int hot_x, hot_y;

  i= -1;
  for (row= 0; row < 32; ++row) {
    for (col= 0; col < 32; ++col) {
      if ( col % 8 ) {
        data[i] <<= 1;
        mask[i] <<= 1;
      } else {
        ++i;
        data[i]= mask[i] = 0;
      } //fi
      switch (image[4+row][col]) {
        case '.':
          data[i] |= 0x01;
          mask[i] |= 0x01;
          break;
        case '+':
          data[i] |= 0x00;
          mask[i] |= 0x01;
          break;
        case ' ':
          break;
      } //eos
    } //eof
  } //eof
  sscanf(image[4+row], "%d,%d", &hot_x, &hot_y);

  SDL_FreeCursor(Cursor);
  Cursor= SDL_CreateCursor(data, mask, 32, 32, hot_x, hot_y);
  SDL_SetCursor(Cursor);
} //eof SetCursor()
