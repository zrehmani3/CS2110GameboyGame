#include "myLib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;
int __qran_seed = 42;

//Sets pixel at given row, col
void setPixel(int row, int col, u16 color) {
  videoBuffer[OFFSET(row, col, 240)] = color;
}

//Draws rectangle (using DMA) given row, col and height, width, color
void drawRect(int row, int col, int height, int width, u16 color) {
  for (int r = 0; r < height; r++) {
    DMA[3].src = &color;
    DMA[3].dst = videoBuffer + OFFSET(row + r, col, 240);
    DMA[3].cnt = DMA_ON | DMA_SOURCE_FIXED | width;
  }
}

//Checks if you are within the bounds of the screen
int boundsCheck(int *var, int bound, int size) {
  if (*var < 0) {
    return 1;
  }
  if (*var > bound - size + 1) {
    return 1;
  }
  return 0;
}

//checks if Food is within the bounds of the screen
int foodBoundsCheck(int *var) {
  if (*var < 0) {
    return 1;
  } else {
    return 0;
  }
}

void waitForVBlank() {
  while (SCANLINECOUNTER > 160);
  while (SCANLINECOUNTER < 160);
}

//Draws a line at a given row, with given length and color
void drawLine(int row, int length, u16 color) {
  for (int c = 0; c < length; c++) {
    setPixel(row, c, color);
  }
}

//Uses DMA to draw an image at row, col, given image and its height/width
void drawImage3(int r, int c, int width, int height, const u16 *image) {
  for (int i = 0; i < height; i++) {
    DMA[3].src = image + width * i;
    DMA[3].dst = videoBuffer + OFFSET(r + i, c, 240);
    DMA[3].cnt = DMA_ON | DMA_16 | DMA_NOW | width;
  }
}

//Sets the seed that will be use in the qran() function
int sqran(int seed) { 
  int old= __qran_seed;
  __qran_seed= seed; 
  return old; 
}

//Tonc's implementation of rand that is significantly faster
int qran() { 
  __qran_seed= 1664525 * __qran_seed + 1013904223;
  return (__qran_seed >> 16) & 0x7FFF;
}

//Gets a random number within a certain range
int qran_range(int min, int max) {
  return (qran() * (max - min) >> 15) + min;
}