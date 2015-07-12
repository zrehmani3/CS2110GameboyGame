#include "myLib.h"
#include "writeText.h"
#include <stdlib.h>
#include <stdio.h>
#include "startLevel.h"
#include "gameover.h"
#include "youWin.h"
#include "scorePic.h"
#include "game.h"

#define FOODOBJ 5
#define BGCOLOR CYAN

static int level = 1;
static int lives = 3;
int setInitSeed = 1;


// PROTOTYPES
void gameOver();
void waitForSelect();
void randomizeFood(FOOD *fishFood, FOOD *oldFishFood, int curSize);
void startLevelScreen();
void waitForStart();
void youWinScreen();
void restart();
void drawInitialBackground();

int main() {
  REG_DISPCTL = MODE3 | BG2_ENABLE;
  startLevelScreen();
  drawInitialBackground();
  char buffer[2];
  FISH curFish;
  curFish.row = 30;
  curFish.col = 0;
  curFish.size = 10;
  curFish.color = BLUE;
  int oldRow = curFish.row;
  int oldCol = curFish.col;
  int oldSize = curFish.size;
  FISH *myFish = &curFish;
  u16 colors[] = {RED, GREEN, MAGENTA, YELLOW, WHITE};
  int numColors = sizeof(colors)/(sizeof(colors[0]));
  FOOD fishFood[FOODOBJ];
  FOOD oldFishFood[FOODOBJ];
  for (int i = 0; i < FOODOBJ; i++) {
    fishFood[i].size = qran_range(myFish->size - 7, myFish->size + 7);
    fishFood[i].col = qran_range(160, 240 - fishFood[i].size);
    fishFood[i].row = qran_range(0, 150 - fishFood[i].size);
    fishFood[i].colDelta = -level;
    fishFood[i].color = colors[i % numColors];
    oldFishFood[i] = fishFood[i];
  }
  int score = 0;
  while(1) {
    if (KEY_DOWN_NOW(BUTTON_SELECT)) {
      restart();
    }
    checkMovement(myFish);
    if (boundsCheck(&myFish->row, 148, myFish->size)) {
      lives--;
      if (lives > 0) {
        main();
      } else {
        gameOver();
      }
    }
    if (boundsCheck(&myFish->col, 239, myFish->size)) {
      lives--;
      if (lives > 0) {
        main();
      } else {
        gameOver();
      }
    }
    //Update Food position
    for (int i = 0; i < FOODOBJ; i++) {
      fishFood[i].col = fishFood[i].col + fishFood[i].colDelta;
    }
    waitForVBlank();
    for (int i = 0; i < FOODOBJ; i++) {
      drawRect(oldFishFood[i].row, oldFishFood[i].col,
        oldFishFood[i].size, oldFishFood[i].size, BGCOLOR);
    }
    //Redraw food at new position, checking if its in the screen or collifion with the fish
    for (int i = 0; i < FOODOBJ; i++) {
      drawRect(fishFood[i].row, fishFood[i].col, fishFood[i].size, fishFood[i].size, fishFood[i].color);
      oldFishFood[i] = fishFood[i];
      if (foodBoundsCheck(&fishFood[i].col)) {
        drawRect(oldFishFood[i].row, oldFishFood[i].col,
        oldFishFood[i].size, oldFishFood[i].size, BGCOLOR);
        randomizeFood(&fishFood[i], &oldFishFood[i], myFish->size);
      }
      if (detectCollision(myFish, &fishFood[i])) {
        if (fishFood[i].size < myFish->size) {
          drawRect(oldFishFood[i].row, oldFishFood[i].col,
          oldFishFood[i].size, oldFishFood[i].size, BGCOLOR);
          randomizeFood(&fishFood[i], &oldFishFood[i], myFish->size);
          myFish->size++;
          score++;
          drawRect(151, 130, 10, 36, BGCOLOR);
          sprintf(buffer, "%d", score);
          drawString(151, 130, buffer, WHITE);
          if (score == 7) {
            if (level < 3) {
              level++;
              main();
            } else if (level == 3) {
              youWinScreen();
            }
          }
        }
        else {
          lives--;
          if (lives > 0) {
            main();
          } else {
            gameOver();
          }
        }
      }
    }
    drawRect(oldRow, oldCol, oldSize, oldSize, BGCOLOR);
    drawRect(myFish->row, myFish->col, myFish->size, myFish->size, myFish->color);
    oldRow = myFish->row;
    oldCol = myFish->col;
    oldSize = myFish->size;
  }
}

//Prints game over, lives == 0
void gameOver() {
  drawImage3(0, 0, 240, 160, gameover);
  drawString(100, 15, "GAME OVER (Press SELECT to restart!)", WHITE);
  level = 1;
  lives = 3;
  waitForSelect();
}

//Waits for restart
void waitForSelect() {
  while (!KEY_DOWN_NOW(BUTTON_SELECT));
  restart();
}

//Restarts game
void restart() {
  level = 1;
  lives = 3;  
  main();
}

//Creates a new food (random size within a certain range) on the right-ish part of the screen
void randomizeFood(FOOD *fishFood, FOOD *oldFishFood, int curSize) {
  fishFood->size = qran_range(curSize - 7, curSize + 7);
  fishFood->col = qran_range(160, 240 - fishFood->size);
  fishFood->row = qran_range(0, 150 - fishFood->size);
  fishFood->colDelta = -level;
  *oldFishFood = *fishFood;
}

//Prints the Start screen
void startLevelScreen() {
  char levelString[10];
  sprintf(levelString, "Level: %d", level);
  char livesString[10];
  sprintf(livesString, "Lives: %d", lives);
  drawImage3(0, 0, 240, 160, startLevel);
  drawString(100, 80, levelString, WHITE);
  drawString(120, 80, livesString, WHITE);
  waitForStart();
}

//Waits to start game, and if this is the first time, sets the seed based on time it takes to press start
void waitForStart() {
  if (setInitSeed) {
    int x = 0;
    while (!KEY_DOWN_NOW(BUTTON_START)) {
      x++;
      if (KEY_DOWN_NOW(BUTTON_SELECT)) {
        restart();
      }
    }
    sqran(x);
    setInitSeed = 0;
  } else {
    while (!KEY_DOWN_NOW(BUTTON_START)) {
      if (KEY_DOWN_NOW(BUTTON_SELECT)) {
        restart();
      }
    }
  }
}

//You won! Prints out the screen
void youWinScreen() {
  drawImage3(0, 0, 240, 160, youWin);
  waitForSelect();
}

//Draws the initial background that looks like an ocean (fishy game)
void drawInitialBackground() {
  u16 bg = BGCOLOR;
  DMA[3].src = &bg;
  DMA[3].dst = videoBuffer;
  DMA[3].cnt = 38400 | DMA_ON | DMA_SOURCE_FIXED;
  drawImage3(150, 5, SCOREPIC_WIDTH, SCOREPIC_HEIGHT, scorePic);
  drawChar(151, 130, '0', WHITE);
  drawLine(149, 240, BLACK);
}