#include "myLib.h"
#include "game.h"

//Checks where to move the fish (up, down, left, right)
void checkMovement(FISH *myFish) {
  if (KEY_DOWN_NOW(BUTTON_LEFT)) {
    myFish->col = myFish->col - 2;
  } else if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
    myFish->col = myFish->col + 2;
  } else if (KEY_DOWN_NOW(BUTTON_UP)) {
    myFish->row = myFish->row - 2;
  } else if (KEY_DOWN_NOW(BUTTON_DOWN)) {
    myFish->row = myFish->row + 2;
  }
}

//O(1) check for collision between fish and food
int detectCollision(FISH *curFish, FOOD *curFood) {
  int FISHTop = curFish->row;
  int FISHBottom = curFish->row + curFish->size;
  int FISHLeft = curFish->col;
  int FISHRight = curFish->col + curFish->size;
  int curFoodTop = curFood->row;
  int curFoodBottom = curFood->row + curFood->size;
  int curFoodLeft = curFood->col;
  int curFoodRight = curFood->col + curFood->size;
  int checkBottomInfraction = FISHBottom < curFoodTop;
  int checkTopInfraction = FISHTop > curFoodBottom;
  int checkLeftInfraction = FISHLeft > curFoodRight;
  int checkRightInfraction = FISHRight < curFoodLeft;
  return !(checkBottomInfraction || checkTopInfraction || checkLeftInfraction || checkRightInfraction);
}