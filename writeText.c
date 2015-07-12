#include "myLib.h"
#include "writeText.h"

void drawChar(int row, int col, char ch, u16 color) {
  for (int r = 0; r < 8; r++) {
    for (int c = 0; c < 6; c++) {
      if (fontdata_6x8[OFFSET(r, c, 6) + ch * 48]) {
        setPixel(row + r, col + c, color);
      }
    }
  }
}

void drawString(int row, int col, char *str, u16 color) {
  int chars_drawn = 0;
  while (*str) {
    drawChar(row, col + 6 * chars_drawn++, *str++, color);
  }
}
