#ifndef PLANEMB
#define PLANEMB

#define PLANE_WIDTH 20
#define PLANE_HEIGHT 10
#define PLANE_X_SPEED 2
#define PLANE_INITIAL_X 20
#define FLOOR_Y 180
#define BULLET_SPEED 2
#define BULLET_WIDTH 5
#define BULLET_HEIGHT 5
#define POWERUP_WIDTH 7
#define POWERUP_HEIGHT 7
#define PLANE_Y_SPEED 2

#include <stdbool.h>



extern volatile int pixel_buffer_start;

struct Plane{
    bool isRed;   //True if plane is red, false is plane is blue
    bool isFiring;    //True if plane has activated shooting powerUp
    bool isFlying;      //True if plane has "lifted off"

    short int score;

    int shotsLeft;
    int dX;     //Horizontal velocity of plane
    int dY;     //Vertical velocity of plane
    int accel;  //Vertical acceleration of plane
    int x;      //Position of the plane
    int y;      

    int lastX;
    int lastY;
} red, blue;

struct Bullet{
    int x;
    int y;
    int dX;
    int lastX;
};

struct Powerup{
    int x;
    int y;
};

void planeMain();

void initPlanes();

void resetPlanes();

void addNewPowerup(int idx);

void drawPlanes();

bool planeTurn(volatile int* pixel_ctrl_ptr);

void deletePlanes();

void deleteBullets();

void moveBullets();

void shootBullets();

void movePlane(struct Plane* plane);

int managePlaneCollision();

bool checkCollisions(struct Plane* plane);

bool actOnCollisions();

void drawBullets();

#endif