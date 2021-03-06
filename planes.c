//PLANE
#include "plane.h"
#include "utility.h"
#include "graphics.h"
#include "images.h"
#include "address_map_arm.h"

struct Bullet bullets[10];

int numBullets = 0;

struct Powerup powerups[2];

struct Plane red;
struct Plane blue;

int managePlaneCollision(){
    //Bouncing planes off each other
    if(ABS(red.y - blue.y) < 5){
        red.dX = -red.dX;
        blue.dX = -blue.dX;
        return -1;
    } else if(red.y < blue.y){
        red.score++;
        return 0;
    } else {
        blue.score++;
        return 1;
    }
}

void deletePlanes(){
    if(red.lastX != -1) draw_rectangle(red.lastX, red.lastY, PLANE_WIDTH, PLANE_HEIGHT, SKY_BLUE);
    if(blue.lastX != -1) draw_rectangle(blue.lastX, blue.lastY, PLANE_WIDTH, PLANE_HEIGHT, SKY_BLUE);
}

void deleteBullets(){
    int i;
    for(i = 0; i < 10; ++i){
        if(bullets[i].x == -1) continue;
        draw_rectangle(bullets[i].lastX, bullets[i].y, BULLET_WIDTH, BULLET_HEIGHT, SKY_BLUE);
    }
}

//Shooting bullets only every 30 frames
void shootBullets(){
    int i = 0;
    int direction;
    if(blue.isFiring){
        blue.shotsLeft--;
        if(!blue.shotsLeft) blue.isFiring = false;
        if(blue.shotsLeft % 10 == 0){
            direction = (blue.dX > 0) ? 1 : -1;
            numBullets++;
            for(i = 0; i < 10;  ++i){
                if(bullets[i].x == -1){
                    bullets[i].dX = 3*blue.dX;
                    bullets[i].y = blue.y;
                    bullets[i].x = (blue.dX > 0) ? blue.x + 5 + PLANE_WIDTH : blue.x - 5;
                }
            }
        }
    }
    if(red.isFiring){
        red.shotsLeft--;
        if(!red.shotsLeft) red.isFiring = false;
        if(red.shotsLeft % 10 == 0){
            direction = (red.dX > 0) ? 1 : -1;
            numBullets++;
            for(; i < 10;  ++i){
                if(bullets[i].x == -1){
                    bullets[i].dX = 3*red.dX;
                    bullets[i].y = red.y;
                    bullets[i].x = (red.dX > 0) ? red.x + 5 + PLANE_WIDTH : red.x - 5;
                }
            }
        }
    }
}


void movePlane(struct Plane* plane){
       int* keyPtr = (int*)KEY_BASE;
    //Manage acceleration
    short int keyChecker = (plane -> isRed) ? 0x1 : 0x8;
    if(!plane -> isFlying){
        if(*keyPtr & keyChecker){
            plane -> dY = -PLANE_Y_SPEED;
            plane -> isFlying = true;
        }
        return;
    }
    if(*keyPtr & keyChecker){
        plane -> dY = -PLANE_Y_SPEED;
    }
    else plane -> dY = PLANE_Y_SPEED;
    plane -> lastX = plane -> x;
    plane -> lastY = plane -> y ;
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
            draw_rectangle(bullets[i].x - 5, bullets[i].y - 1, BULLET_WIDTH + 10, BULLET_HEIGHT + 2, SKY_BLUE);
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
            return true;
        }
    }
    for(i = 0; i < 2; ++i){
        if(objectsCollide(plane -> x, plane -> y, PLANE_WIDTH, PLANE_HEIGHT, powerups[i].x, powerups[i].y, POWERUP_WIDTH, POWERUP_HEIGHT)){
            plane -> isFiring = true;
            plane -> shotsLeft = 120;
            draw_rectangle(powerups[i].x, powerups[i].y, POWERUP_WIDTH, POWERUP_HEIGHT, SKY_BLUE);
            addNewPowerup(i);
        }
    }
    if(plane -> isFlying && plane -> y >= FLOOR_Y - PLANE_HEIGHT){
        draw_rectangle(plane -> x, plane -> y, PLANE_WIDTH + 1, PLANE_HEIGHT, SKY_BLUE);
        return true;
    } else if(plane -> isFlying && plane -> y <= 0){
        draw_rectangle(plane -> x, plane -> y, PLANE_WIDTH + 1, PLANE_HEIGHT, SKY_BLUE);
        return true;
    }
    return false;
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
    blue.x = 5;
    red.x = 295;
    red.y = blue.y = FLOOR_Y - PLANE_HEIGHT - 1;
    red.isFiring = blue.isFiring = false;
    red.lastX = red.lastY = blue.lastX = blue.lastY = -1;
    red.shotsLeft = blue.shotsLeft = 0;
}

void drawPlanes(){
    //If dX positive, plane going right
    //printf("Drawing planes at: Blue: %d, %d, Red: %d, %d", blue.x, blue.y, red.x, red.y);
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
    addNewPowerup(0);
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
        if(*(int*)SW_BASE & 0x1) break;
    }
    draw_rectangle(blue.x, blue.y, PLANE_WIDTH, PLANE_HEIGHT, SKY_BLUE);
	draw_rectangle(red.x, red.y, PLANE_WIDTH, PLANE_HEIGHT, SKY_BLUE);
    return isRedLoser;
}


void planeMain(){    
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    *(pixel_ctrl_ptr + 1) = 0xC8000000;
    wait_for_vsync();
    pixel_buffer_start = *pixel_ctrl_ptr;
    bool isRedLoser;

    draw_rectangle(0,0, RESOLUTION_X, FLOOR_Y, SKY_BLUE);
    draw_rectangle(0,FLOOR_Y, RESOLUTION_X, RESOLUTION_Y - FLOOR_Y, FLOOR_GREEN);
    wait_for_vsync();
    initPlanes();
    displayScore(0,0);
    bool isOver = false;
    while(!isOver){
        isRedLoser = planeTurn(pixel_ctrl_ptr);
        displayScore(blue.score, red.score);
        resetPlanes();
        if(blue.score == 5 || red.score == 5) break;
        if(*(int*)SW_BASE & 0x1) break;
    }
	printf("done planes\n");
}
