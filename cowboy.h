#ifndef COWBOY
#define COWBOY

#include <stdbool.h>

#define COWBOY_WIDTH 20
#define COWBOY_HEIGHT 20
#define COWBOY_INITIAL_X 10
#define COWBOY_INITIAL_Y 90
#define BULLET_WIDTH 5
#define BULLET_HEIGHT 5
#define OBSTACLES_WIDTH 14
#define OBSTACLES_HEIGHT 14
#define MAX_NUM_BULLET 20

enum obstacleItems {CACTUS, ROCK};

struct Cowboy{
    bool isRed;     //true -> player is red, false -> player is blue
    bool isFiring;
    bool fired;
    
    short int score;

    int x;
    int y;      // position of the player 
    int dY;        // change in y direction 
    int lastX;
    int lastY;
} red, blue;

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
    obstacleItems type;
};


void cowboyMain();
void initCowboy();
void resetCowboy();
void drawCowboy();
void deleteCowboy();
void deleteBullets();
void deleteObstacles();
void shootBullets();
void moveCowboy();
void initObstacles();
void drawBullet45Up();
void drawBulletStraight();
void drawBullet45Down();
void CACTUSCollision();
void ROCKCollision();
int checkCollision();
void replaceObstacles();


#endif