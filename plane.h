#ifndef PLANE.H
#define PLANE.H

#define PLANE_WIDTH 20
#define PLANE_HEIGHT 15
#define PLANE_X_SPEED 1
#define PLANE_INITIAL_X 20
#define PLANE_MIN_Y 180
#define BULLET_SPEED 2
#define BULLET_WIDTH 5
#define BULLET_LENGTH 5
#define PLANE_MAX_Y_SPEED



extern volatile int pixel_buffer_start;

struct Plane{
    bool isRed;   //True if plane is red, false is plane is blue
    bool isFiring;    //True if plane has activated shooting powerUp
    bool isFlying;      //True if plane has "lifted off"

    short int score;

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
    int lastY;
};

struct Powerup{
    int x;
    int y;
};

void planeMain();

void initPlanes();

void drawPlanes();

bool planeTurn(volatile int* pixel_ctrl_ptr);

void deletePlanes();

void deleteBullets();

void movePlane(struct Plane* plane);

int checkCollisions();

bool actOnCollisions();

void drawBullets();

#endif