#ifndef COWBOY
#define COWBOY

#include <stdbool.h>

#define MIN_Y 21
#define COWBOY_WIDTH 20
#define COWBOY_HEIGHT 20
#define COWBOY_INITIAL_X 10
#define COWBOY_INITIAL_Y 90
#define BULLET_WIDTH 5
#define BULLET_HEIGHT 5
#define OBSTACLES_WIDTH 14
#define OBSTACLES_HEIGHT 14
#define MAX_NUM_BULLET 20
#define COWBOY_SPEED 3
#define BULLET_SPEED 3

enum obstacleItems {CACTUS = 0, ROCK = 1};

struct Cowboy{
    bool isRed;     //true -> player is red, false -> player is blue
    bool fired;
    
    short int score;

    int x;
    int y;      // position of the player 
    int dX;
    int dY;        // change in y direction 
    int lastX;
    int lastY;
};

struct CowboyBullet{
    int x;
    int y;
    int dX;
    int dY;
    int lastX;
    int lastY;
    bool drawn;
};

struct obstacles{
    int x;
    int y;
    enum obstacleItems type;
};

void cowboyMain();
void initCowboy();
void initObstacles();
void resetCowboy();
void deleteCowboy();
void deleteCBullets();
void readKeys();
void moveCowboy();
void moveCBullets();
void drawCowboy();
void drawCBullets();
void checkCollision(struct CowboyBullet* bullets);
void collisionHandler(struct CowboyBullet* bullets);
void replaceObstacles();


#endif