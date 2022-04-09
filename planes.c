#include "stdbool.h"
#include "plane.h"
#include "graphics.h"
#include "images.h"

struct Bullet bullets[10];

int numBullets = 0;

void planeMain(){
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    *(pixel_ctrl_ptr + 1) = 0xC8000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1);

    draw_image(0,0, planeBackground_320x240, RESOLUTION_X, RESOLUTION_Y);
    initPlanes();
    drawPlanes();
    wait_for_vsync();
    
    bool isOver = false;
    while(!isOver){
        isOver = planeTurn(pixel_ctrl_ptr);
    }

}

void initPlanes(){
    red.isRed = true;
    blue.isRed = false;
    red.dX = -(PLANE_X_SPEED); 
    blue.dX = PLANE_X_SPEED;
    red.dY = blue.dY = red.accel = blue.accel = 0;
    red.isFlying = blue.isFlying = 0;
    blue.x = PLANE_INITIAL_X;
    red.x = RESOLUTION_X - 1 - PLANE_INITIAL_X;
    red.y = blue.y = PLANE_MIN_Y;
    red.isFiring = blue.isFiring = false;
    red.score = blue.score = false;
    red.lastX = red.lastY = blue.lastX = blue.lastY = -1;
    int i;
    for(i = 0; i < 10; ++i){
        bullets[i].x = -1;
    }
}

void drawPlanes(){
    //If dX positive, plane going right
    if(red.dX > 0) draw_image(red.x, red.y, redPlaneRight_20x10,
                                PLANE_WIDTH, PLANE_HEIGHT);
    else draw_image(red.x, red.y, redPlaneLeft_20x10,
                                PLANE_WIDTH, PLANE_HEIGHT);
    if(blue.dX > 0) draw_image(blue.x, blue.y, bluePlaneRight_20x10,
                                PLANE_WIDTH, PLANE_HEIGHT);
    else draw_image(blue.x, blue.y, bluePlaneLeft_20x10,
                                PLANE_WIDTH, PLANE_HEIGHT);
}


bool planeTurn(volatile int* pixel_ctrl_ptr){
    bool notDone = true;
    int collisionCode;
    while(notDone){
        deletePlanes();
        deleteBullets();
        movePlane(&red);
        movePlane(&blue);
        collisionCode = checkCollisions();
        notDone = actOnCollisions(collisionCode);
        drawPlanes();
        drawBullets();
        wait_for_vsync();
        pixel_buffer_start = *(pixel_ctrl_ptr + 1);
    }
}


void deletePlanes(){
    if(red.lastX == -1) return;
    draw_rectange(red.lastX, red.lastY, PLANE_WIDTH, PLANE_HEIGHT, BLUE);
    draw_rectange(blue.lastX, blue.lastY, PLANE_WIDTH, PLANE_HEIGHT, BLUE);
}

void deleteBullets(){
    int i;
    for(i = 0; i < numBullets; ++i){
        if(bullets[i].x == -1) continue;
        draw_rectange(bullets[i].lastX, bullets[i].lastY, BULLET_WIDTH, BULLET_LENGTH, BLUE);
    }
}

//Returns  true if in bounds
bool inBounds(int min, int max, int val){
    if(val < min || val > max) return false;
    else return true;
}

void movePlane(struct Plane* plane){
    //Manage acceleration
    if(plane -> isRed){

    } else {

    }
    //Accelerating plane if accelerating
    if(ABS(plane -> dY) < PLANE_MAX_Y_SPEED)
        plane -> dY += plane -> accel;
    
    plane -> lastX = plane -> x;
    plane -> lastY = plane -> y;
    //Plane bounces off x sides
    if(!inBounds(0, RESOLUTION_X - 1, plane -> x + plane -> dX))
        plane -> dX = -(plane -> dX);
    plane -> x += plane -> dX;
    //Y collisions are checked in checkCollisions as they are game ending
    plane -> y += plane -> dY;
}

int checkCollisions(){
    
}

bool actOnCollisions();

void drawBullets();