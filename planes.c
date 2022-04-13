#include <stdbool.h>
#include "plane.h"
#include "graphics.h"
#include "images.h"
#include "utility.h"
#include "address_map_arm.h"

struct Bullet bullets[10];

int numBullets = 0;

struct Powerup powerups[2];

struct Plane red;
struct Plane blue;

void planeMain(){
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    *(pixel_ctrl_ptr + 1) = 0xC8000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1);
    bool isRedLoser;
    draw_rectangle(0,0, RESOLUTION_X, FLOOR_Y, SKY_BLUE);
    draw_rectangle(0,FLOOR_Y, RESOLUTION_X, RESOLUTION_Y - FLOOR_Y, FLOOR_GREEN);
    initPlanes();
    drawPlanes();
    wait_for_vsync();
    displayScore(0,0);
    bool isOver = false;
    while(!isOver){
        isRedLoser = planeTurn(pixel_ctrl_ptr);
        //displayRoundWinner(isRedLoser);
        displayScore(blue.score, red.score);
        resetPlanes();
        if(blue.score == 5 || red.score == 5) break;
    }
    //displayFinalWinner(isRedLoser);
}

void initPlanes(){
    red.isRed = true;
    blue.isRed = false;
    red.score = blue.score = 0;
    resetPlanes();
    int i;
    for(i = 0; i < 10; ++i){
        bullets[i].x = -1;
    }
}

void resetPlanes(){
    blue.dX = PLANE_X_SPEED;
    red.dX = -PLANE_X_SPEED;
    red.dY = blue.dY = red.accel = blue.accel = 0;
    red.isFlying = blue.isFlying = 0;
    blue.x = PLANE_INITIAL_X;
    red.x = RESOLUTION_X - 1 - PLANE_INITIAL_X;
    red.y = blue.y = FLOOR_Y-PLANE_HEIGHT;
    red.isFiring = blue.isFiring = false;
    red.lastX = red.lastY = blue.lastX = blue.lastY = -1;
    red.shotsLeft = blue.shotsLeft = 0;
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
    int dummy;
    bool isRedLoser;
    addNewPowerup(0);
    addNewPowerup(1);
    while(notDone){
        deletePlanes();
        deleteBullets();
        movePlane(&red);
        movePlane(&blue);
        moveBullets();
        shootBullets();
        drawPlanes();
        drawBullets();
        wait_for_vsync();
        if(checkCollisions(&blue)){
            red.score++;
            isRedLoser = true;
            break;
        } else if (checkCollisions(&red)){
            blue.score++;
            isRedLoser = false;
            break;
        }
        if(objectsCollide(  red.x, red.y, PLANE_WIDTH, PLANE_HEIGHT, 
                            blue.x, blue.y, PLANE_WIDTH, PLANE_HEIGHT)){
                                dummy = managePlaneCollision();
                                if(dummy > -1){
                                    isRedLoser = dummy;
                                    break;
                                }
                            }

        pixel_buffer_start = *(pixel_ctrl_ptr + 1);
    }
    deletePlanes();
    return isRedLoser;
}

int managePlaneCollision(){
    //Bouncing planes off each other
    if(ABS(red.y - blue.y) < 5){
        red.dX = -red.dX;
        blue.dX = -blue.dX;
        return -1;
    } else if(red.y > blue.y){
        red.score++;
        return 0;
    } else {
        blue.score++;
        return 1;
    }
}

void deletePlanes(){
    if(red.lastX != -1)
        draw_rectangle(red.lastX, red.lastY, PLANE_WIDTH, PLANE_HEIGHT, BLUE);
    if(blue.lastX != -1)
        draw_rectangle(blue.lastX, blue.lastY, PLANE_WIDTH, PLANE_HEIGHT, BLUE);
}

void deleteBullets(){
    int i;
    for(i = 0; i < 10; ++i){
        if(bullets[i].x == -1) continue;
        draw_rectangle(bullets[i].lastX, bullets[i].y, BULLET_WIDTH, BULLET_HEIGHT, BLUE);
    }
}
//Shooting bullets only every 30 frames
void shootBullets(){
    int i = 0;
    if(blue.isFiring){
        blue.shotsLeft--;
        if(!blue.shotsLeft) blue.isFiring = false;
        if(blue.shotsLeft % 30 == 0){
            numBullets++;
            for(i = 0; i < 10;  ++i){
                if(bullets[i].x == -1){
                    bullets[i].dX = 3*blue.dX;
                    bullets[i].y = blue.y;
                    bullets[i].x = (blue.dX > 0) ? blue.x - 2 : blue.x + 2 + PLANE_WIDTH;
                }
            }
        }
    }
    if(red.isFiring){
        red.shotsLeft--;
        if(!red.shotsLeft) red.isFiring = false;
        if(red.shotsLeft % 30 == 0){
            numBullets++;
            for(; i < 10;  ++i){
                if(bullets[i].x == -1){
                    bullets[i].dX = 3*red.dX;
                    bullets[i].y = red.y;
                    bullets[i].x = (red.dX > 0) ? red.x - 2 : red.x + 2 + PLANE_WIDTH;
                }
            }
        }
    }
}


void movePlane(struct Plane* plane){
    int* keyPtr = (int*)KEY_BASE;
    //Manage acceleration
    short int keyChecker = (plane -> isRed) ? 0x1 : 0x8;
    plane -> lastX = plane -> x;
    plane -> lastY = plane -> y;
    if(!plane -> isFlying){
        if(*keyPtr & keyChecker){
            plane -> accel = PLANE_ACCEL_INC;
            plane -> isFlying = true;
        }
        return;
    }
    if(*keyPtr & keyChecker){
        if(plane -> accel < PLANE_MAX_ACCEL) plane -> accel -= PLANE_ACCEL_INC;
    } else if (plane -> accel > -PLANE_MAX_ACCEL) plane -> accel += PLANE_ACCEL_INC;
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

void moveBullets(){
    int i;
    for(i = 0; i < 10; ++i){
        if(bullets[i].x == -1) continue;
        bullets[i].lastX = bullets[i].x;
        bullets[i].x += bullets[i].dX;

        if(!inBounds(0, RESOLUTION_X, bullets[i].x)){
            bullets[i].x = -1;
            numBullets --;
        }
    }
}

bool checkCollisions(struct Plane* plane){
    //Check for bullet collisions
    int i;
    for(i = 0; i < numBullets; ++i){
        if(bullets[i].x == -1) continue;
        if(objectsCollide(plane -> x, plane -> y, PLANE_WIDTH, PLANE_HEIGHT, bullets[i].x, bullets[i].y, BULLET_WIDTH, BULLET_HEIGHT)){
            return false;   
        }
    }
    for(i = 0; i < 2; ++i){
        if(objectsCollide(plane -> x, plane -> y, PLANE_WIDTH, PLANE_HEIGHT, powerups[i].x, powerups[i].y, POWERUP_WIDTH, POWERUP_HEIGHT)){
            plane -> isFiring = true;
            plane -> shotsLeft = 40;
            draw_rectangle(powerups[i].x, powerups[i].y, POWERUP_WIDTH, POWERUP_HEIGHT, SKY_BLUE);
            addNewPowerup(i);
        }
    }
    if(plane -> isFlying && !(inBounds(0,FLOOR_Y, plane -> y))) return false;
    return true;
}

void drawBullets(){
    int i;
    for(i = 0; i < 10; ++i){
        if(bullets[i].x == -1) continue;
        draw_image(bullets[i].x, bullets[i].y, bullet_5x5, BULLET_WIDTH, BULLET_HEIGHT);
    }
}

void addNewPowerup(int idx){
    powerups[idx].x = getRandom(20, RESOLUTION_X - 20);
    powerups[idx].y = getRandom(20, FLOOR_Y - 20);
    draw_image(powerups[idx].x, powerups[idx].y, planePowerup_7x7, POWERUP_WIDTH, POWERUP_WIDTH);
}