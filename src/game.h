typedef struct {
  int row;
  int col;
  int size;
  u16 color;
} FISH;

typedef struct {
  int row;
  int col;
  int colDelta;
  int size;
  u16 color;
} FOOD;

//PROTOTYPES
void checkMovement(FISH *myFish);
int detectCollision(FISH *curFish, FOOD *curFood);