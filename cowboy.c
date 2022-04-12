#include <stdbool.h>
#include "cowboy.h"
#include "graphics.h"
#include "images.h"
#include "utility.h"
#include "address_map_arm.h"

struct obstacles activeObstacles[8];
struct CowboyBullet cowboyBullets[MAX_NUM_BULLET];
struct Cowboy* cowboy;
int obstacleIdx;


void cowboyMain(){
    // check edge register; 1st time -> call shootBullets(), 2nd time -> call moveCowboy()
    int* keyPtr =  (int*)KEY_BASE;
    int* edgeRegister = (int*)(KEY_BASE + 0xC);
    bool gameOver = false;
    bool play;

    while(play){
        initCowboy();
        while(!gameOver){
            deleteCowboy();
            deleteBullets();
            readKeys();
            moveCowboy();
            moveBullet();
            drawCowboy();
            drawBullet();
            checkCollision();
            manageCollision();
        }
        // hex display winner on DE1-SOC board
        displayWinner();
        // ask if play again, to exit to main menu 
    }
}

void initCowboy(){
    red.isRed = true;
    blue.isRed = false;
    red.fired = blue.fired = false;
    red.score = blue.score = 0;
    resetCowboy();
}

void resetCowboy(){
    blue.x = COWBOY_INITIAL_X;
    red.x = RESOLUTION_X - COWBOY_INITIAL_X - 1;
    red.y = blue.y = COWBOY_INITIAL_Y;
    red.fired = blue.fired = false;
    red.dY = blue.dY = 1;
    red.lastY = blue.lastY = -1;
    int i;
    for(i = 0; i < 20; ++i){
        bullet[i]->drawn = false;
    }
}

void drawCowboy(){
    draw_image(red.x, red.y, redCowboy_20x20, COWBOY_WIDTH, COWBOY_HEIGHT);
    draw_image(blue.x, blue.y, blueCowboy_20x20, COWBOY_WIDTH, COWBOY_HEIGHT);    
}

void deleteCowboy(){
    draw_rectange(red.lastX, red.lastY, COWBOY_WIDTH, COWBOY_HEIGHT, TAN);
    draw_rectange(blue.lastX, blue.lastY, COWBOY_WIDTH, COWBOY_HEIGHT, TAN);
}

void deleteBullets(){
    draw_rectange(bullet.lastX, bullet.lastY, BULLET_WIDTH, BULLET_HEIGHT, TAN);
}

void deleteObstacles(){
    draw_rectange(activeObstacles[obstacleIdx].x, activeObstacles[obstacleIdx].y, OBSTACLES_WIDTH, OBSTACLES_HEIGHT, TAN);
}

void drawBullets(){
    int i = 0;
    for(i = 0; i < MAX_NUM_BULLET; ++i){
        if(cowboyBullets[i]->drawn)
    }
}

void shootBullets(){
    while(inBounds(0, RESOLUTION_X, bullet.x) && inBounds(0, RESOLUTION_Y, bullet.y)){
        if(!objectsCollide){
            draw_image(bullet.x, bullet.y, bullet_05x05, BULLET_WIDTH, BULLET_HEIGHT);
            bullet.lastX = bullet.x;
            bullet.x = bullet.x + bullet.dX;
            deleteBullets();
        } 
        else if(objectsCollide)
            manageCollision();
    }
}

void moveCowboy(){
    int* keyPtr = (int*)KEY_BASE;
    int keyChecker = (cowboy->isRed) ? 0x1 : 0x8;     //0x1 -> red, 0x8 -> blue
    if(*keyPtr & keyChecker){
        while(inBounds(0, RESOLUTION_Y, cowboy->y)){
            if(cowboy->y == 0 || cowboy->y == RESOLUTION_Y)
                cowboy->dY = (cowboy->dY == 1) ? -1 : 1;
            cowboy->x = cowboy->y + cowboy->dY;
        }
    }
}

void initObstacles(){
    int i;
    // only 8 obstacles allowed in total 
    for(i=0; i<9; i++){
        int randomObstacle = getRandom(CACTUS, ROCK);
        // get random position in the bounds of {85, 10}, {235, 10}, {85, 200}, {235, 200}
        int activeObstacles[i].x = getRandom(85, 235);
        int activeObstacles[i].y = getRandom(10, 200);
        activeObstacles.type = randomObstacle;
        // draw image for rock at random position
        if(randomObstacle)draw_image(activeObstacles[i].x, activeObstacles[i].y, rock_14x14, OBSTACLES_WIDTH, OBSTACLES_HEIGHT);
        // draw image for cactus at random position
        else if (!randomObstacle) draw_image(activeObstacles[i].x, activeObstacles[i].y, cactus_14x14, OBSTACLES_WIDTH, OBSTACLES_HEIGHT);
    }
}



void drawBullet45Up(){
    bullet->dY = 1;
    bullet->dX = (cowboy->isRed) ? 1 : -1;
    while(inBounds(0, RESOLUTION_X, bullet->x && 0, RESOLUTION_Y, bullet->y)){
        deleteBullets();
        draw_image(bullet->x, bullet->y, bullet_05x05, BULLET_WIDTH, BULLET_HEIGHT);
        bullet->lastX = bullet->x;
        bullet->lastY = bullet->y;
        bullet->x += dX;
        bullet->y -= dY;
    }
}

void drawBulletStraight(){
    bullet->dX = (cowboy->isRed) ? 1 : -1;
    while(inBounds(0, RESOLUTION_X, bullet->x && 0, RESOLUTION_Y, bullet->y)){
        deleteBullets();
        draw_image(bullet.x, bullet.y, bullet_05x05, BULLET_WIDTH, BULLET_HEIGHT);
        bullet->lastX = bullet->x;
        bullet->x -= dX;
    }
}

void drawBullet45Down(){
    bullet->dX = (cowboy->isRed) ? 1 : -1;
    while(inBounds(0, RESOLUTION_X, bullet->x && 0, RESOLUTION_Y, bullet->y)){
        deleteBullets();
        draw_image(bullet.x, bullet.y, bullet_05x05, BULLET_WIDTH, BULLET_HEIGHT);
        bullet->lastX = bullet->x;
        bullet->lastY = bullet->y;
        bullet->x += dX;
        bullet->y += 1;
    }
}

void CACTUSCollision(int obstacleIdx){
    int i;
    int hitY;
    int topBound = activeObstacles[obstacleIdx].y + activeObstacles[obstacleIdx].y * 0.3;
    int middleBound = activeObstacles[obstacleIdx].y + activeObstacles[obstacleIdx].y * 0.3 + activeObstacles[obstacleIdx].y * 0.4;
    int bottomBound = activeObstacles[obstacleIdx].y + 14;

    replaceObstacles();
    // check the y coord of where the obstacle got hit
    while(activeObstacles[obstacleIdx].y < activeObstacles[obstacleIdx].y + 15){
        if(bullet->y == activeObstacles[obstacleIdx].y){
            hitY = activeObstacles[obstacleIdx].y;
            break;
        }
        activeObstacles[obstacleIdx].y = activeObstacles[obstacleIdx].y + 1;
    }

    // hit top 30% of obstacle -> reflect 45 degrees up 
    if(inBounds(activeObstacles[obstacleIdx].y, topBound, hitY)){
        drawBullet45Up();
    }
        
    // hit middle 40% of obstacle -> reflect straight back
    else if(inBounds(topBound, middleBound, hitY)){
        drawBulletStraight();
    }

    // hit bottom 30% of obstacle -> reflect 45 degrees down
    else if(inBounds(middleBound, bottomBound, hitY)){
        drawBullet45Down();
    }
}

void ROCKCollision(){
    // delete original bullet
    deleteBullets();
    // draw two bullets going 45 degrees up and down
    drawBullet45Up();
    drawBullet45Down();
}

int checkCollision(){
    int i;
    while(inBounds(0,RESOLUTION_X, ))
    for(i=0; i < 9; ++i){
        int type = activeObstacles[i].type;
        // check if bullet collided with any obstacles 
        if(objectsCollide(bullet->x, bullet->y, BULLET_WIDTH, BULLET_HEIGHT, activeObstacles[i].x, activeObstacles[i].y, 14, 14)){
            obstacleIdx = i;
            if(type == CACTUS) CACTUSCollision();
            else if(type == ROCK) ROCKCollision();
            return 1;
        }

        // check if bullet collided with another bullet
        else if(objectsCollide(bullet->x, bullet->y, BULLET_WIDTH, BULLET_HEIGHT, bullet->x, bullet->y, BULLET_WIDTH, BULLET_HEIGHT)){
            deleteBullets();
            return 1;
        }
        
        //check if bullet hit player
        else if(objectsCollide(bullet->x, bullet->y, BULLET_WIDTH, BULLET_HEIGHT, cowboy->x, cowboy->y, COWBOY_WIDTH, COWBOY_HEIGHT)){
            if(cowboy->isRed) blue.score++;
            else red.score++;
            return 1;
        }
    }
    return 0;
}

void replaceObstacles(){
    int i;
    int count;
    bool valid = false;;
    deleteObstacles();
    int randomObstacle = getRandom(CACTUS, ROCK);
    // get random position in the bounds of {85, 10}, {235, 10}, {85, 200}, {235, 200}
    int newObstacleX = getRandom(85, 235);
    int newObstacleY = getRandom(10, 200);
    activeObstacles[obstacleIdx].x = getRandom(85, 235);
    activeObstacles[obstacleIdx].y = getRandom(10, 200);
    activeObstacles[obstacleIdx].type = randomObstacle;

    while(valid == false){
        for(i = 0; i < 8; ++i){
            if(newObstacleX == activeObstacles[i].x) && newObstacleY == activeObstacles[i].y){
                newObstacleX = getRandom(85, 235);
                newObstacleY = getRandom(10, 200);
                i = 0;
            }
        }
        valid = true;
        activeObstacles[obstacleIdx].x = newObstacleX;
        activeObstacles[obstacleIdx].y = newObstacleY;

        if(activeObstacles[obstacleIdx].type == CACTUS){
            draw_image(activeObstacles[obstacleIdx].x, activeObstacles[obstacleIdx].y, cactus_14x14, OBSTACLES_WIDTH, OBSTACLES_HEIGHT);   
        }
        else if(activeObstacles[obstacleIdx].type == ROCK){
            draw_image(activeObstacles[obstacleIdx].x, activeObstacles[obstacleIdx].y, rock_14x14, OBSTACLES_WIDTH, OBSTACLES_HEIGHT);   
        }
    }
}





