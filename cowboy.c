#include <stdbool.h>
#include "cowboy.h"
#include "graphics.h"
#include "images.h"
#include "utility.h"
#include "address_map_arm.h"

struct obstacles activeObstacles[8];
struct CowboyBullet cowboyBullets[MAX_NUM_BULLET];
struct Cowboy blueCowboy;
struct Cowboy redCowboy;
int obstacleIdx;
int numBulletsLeft = 20;

void cowboyMain(){
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    *(pixel_ctrl_ptr + 1) = 0xC8000000;
    pixel_buffer_start = *pixel_ctrl_ptr;
    wait_for_vsync();
    bool gameOver = false; 
        // draw background
    draw_image(0, 0, cowboyBackground_320x240, 320, 240);
    initCowboy();
    initObstacles();
	displayScore(blueCowboy.score, redCowboy.score);
    while(!gameOver){
        deleteCowboy();
        deleteCBullets();
        readKeys();
        moveCowboy(&redCowboy);
        moveCowboy(&blueCowboy);
        moveCBullets();
        drawCowboy();
        drawCBullets();
        wait_for_vsync();
        pixel_buffer_start = *(pixel_ctrl_ptr + 1);
		displayScore(blueCowboy.score, redCowboy.score);
        if(redCowboy.score == 5 || blueCowboy.score == 5){
            gameOver = true;
            break;
        }
		if(redCowboy.fired && blueCowboy.fired){
			redCowboy.fired = false;
			blueCowboy.fired = false;
		}
        if(*(int*)SW_BASE & 0x1) break;
    }
    return 0;
}

void resetCowboy(){
    blueCowboy.x = COWBOY_INITIAL_X;
    redCowboy.x = RESOLUTION_X - COWBOY_INITIAL_X - COWBOY_WIDTH - 1;
    redCowboy.y = blueCowboy.y = COWBOY_INITIAL_Y;
    redCowboy.fired = blueCowboy.fired = false;
    redCowboy.dY = blueCowboy.dY = COWBOY_SPEED;
    redCowboy.lastY = blueCowboy.lastY = -1;
    int i;
	deleteCBullets();
    for(i = 0; i < 20; ++i){
        cowboyBullets[i].drawn = false;
    }
}

void initCowboy(){
    redCowboy.isRed = true;
    blueCowboy.isRed = false;
    redCowboy.score = blueCowboy.score = 0;
    resetCowboy();
}

void initObstacles(){
    int i;
    int randomObstacle;
	printf("initi");
    // 8 obstacles allowed in total 
    for(i=0; i<8; i++){
		randomObstacle = getRandom(0, 1);
		randFactor += 10;
        // get random position in the bounds of {85, 10}, {235, 10}, {85, 200}, {235, 200}
        activeObstacles[i].x = getRandom(85, 235);
        activeObstacles[i].y = getRandom(MIN_Y, RESOLUTION_Y - OBSTACLES_HEIGHT - 1);
        activeObstacles[i].type = (enum obstacleItems) randomObstacle;
        // draw image for rock at random position
        if(randomObstacle)draw_image(activeObstacles[i].x, activeObstacles[i].y, rock_14x14, OBSTACLES_WIDTH, OBSTACLES_HEIGHT);
        // draw image for cactus at random position
        else if (!randomObstacle) draw_image(activeObstacles[i].x, activeObstacles[i].y, cactus_14x14, OBSTACLES_WIDTH, OBSTACLES_HEIGHT);
    }
}

void drawCowboy(){
    draw_image(redCowboy.x, redCowboy.y, redCowboy_20x20, COWBOY_WIDTH, COWBOY_HEIGHT);
    draw_image(blueCowboy.x, blueCowboy.y, blueCowboy_20x20, COWBOY_WIDTH, COWBOY_HEIGHT);    
}

void drawCBullets(){
    int i = 0;
    for(i = 0; i < MAX_NUM_BULLET; ++i){
        if(cowboyBullets[i].drawn)
            draw_image(cowboyBullets[i].x, cowboyBullets[i].y, bullet_05x05, BULLET_WIDTH, BULLET_HEIGHT);  
    }
}

void deleteCowboy(){
    draw_rectangle(redCowboy.lastX, redCowboy.lastY, COWBOY_WIDTH, COWBOY_HEIGHT, TAN);
    draw_rectangle(blueCowboy.lastX, blueCowboy.lastY, COWBOY_WIDTH, COWBOY_HEIGHT, TAN);
}

void deleteCBullets(){
    int i;
    for(i = 0; i < 20; ++i){
        if(cowboyBullets[i].drawn == true){
            draw_rectangle(cowboyBullets[i].lastX, cowboyBullets[i].lastY, BULLET_WIDTH, BULLET_HEIGHT, TAN);
        }
    }
}

void moveCowboy(struct Cowboy* cowboy){
	cowboy -> lastX = cowboy -> x;
	cowboy -> lastY = cowboy -> y;
    if(cowboy->y <= MIN_Y || cowboy->y >= RESOLUTION_Y - COWBOY_HEIGHT) cowboy->dY = (cowboy->dY > 0) ? -COWBOY_SPEED : COWBOY_SPEED;
    cowboy->y += cowboy->dY;
}

void shootCBullets(int x, int y, int dX, int dY){
    int i;
    for(i = 0; i < 20; ++i){
        if(cowboyBullets[i].drawn == false){
			printf("found bullet\n");
            cowboyBullets[i].x = x;
            cowboyBullets[i].y = y;
            cowboyBullets[i].dX = dX*BULLET_SPEED;
			cowboyBullets[i].drawn = true;
            cowboyBullets[i].dY = dY;
            break;
        }
    }
}

void deleteObstacles(){
    draw_rectangle(activeObstacles[obstacleIdx].x, activeObstacles[obstacleIdx].y, OBSTACLES_WIDTH, OBSTACLES_HEIGHT, TAN);
}

void replaceObstacles(){
    int i;
    int count;
    bool valid = false;
    deleteObstacles();
    int randomObstacle = getRandom(CACTUS, ROCK);
	int newObstacleX = getRandom(85, 235);
    int newObstacleY = getRandom(MIN_Y, RESOLUTION_Y - OBSTACLES_HEIGHT - 1);
    activeObstacles[obstacleIdx].type = (enum obstacleItems) randomObstacle;

    for(i = 0; i < 8; ++i){
        if(newObstacleX == activeObstacles[i].x && newObstacleY == activeObstacles[i].y){
            newObstacleX = getRandom(85, 235);
            newObstacleY = getRandom(MIN_Y, RESOLUTION_Y - OBSTACLES_HEIGHT - 1);
            i = 0;
        }
    }
    activeObstacles[obstacleIdx].x = newObstacleX;
    activeObstacles[obstacleIdx].y = newObstacleY;

    if(activeObstacles[obstacleIdx].type == CACTUS){
        draw_image(activeObstacles[obstacleIdx].x, activeObstacles[obstacleIdx].y, cactus_14x14, OBSTACLES_WIDTH, OBSTACLES_HEIGHT);   
    }
    else if(activeObstacles[obstacleIdx].type == ROCK){
        draw_image(activeObstacles[obstacleIdx].x, activeObstacles[obstacleIdx].y, rock_14x14, OBSTACLES_WIDTH, OBSTACLES_HEIGHT);   
    }
}

void collisionHandler(struct CowboyBullet* bullets){
    int topYBound = activeObstacles[obstacleIdx].y + activeObstacles[obstacleIdx].y * 0.3;
    int middleYBound = activeObstacles[obstacleIdx].y + activeObstacles[obstacleIdx].y * 0.3 + activeObstacles[obstacleIdx].y * 0.4;
    int bottomYBound = activeObstacles[obstacleIdx].y + OBSTACLES_HEIGHT;
    int middleXBound = activeObstacles[obstacleIdx].x + (OBSTACLES_WIDTH)/2;
    int obstacleType = activeObstacles[obstacleIdx].type;

	
    replaceObstacles();
	
	if(obstacleType == CACTUS){
		// bullet hit the left side, dX = -1, hit the right side, dX = 1
		bullets->dX = (inBounds(activeObstacles[obstacleIdx].x, middleXBound, bullets->x)) ? -BULLET_SPEED : BULLET_SPEED;
		if(inBounds(activeObstacles[obstacleIdx].y, topYBound, bullets->y)) bullets->dY = -BULLET_SPEED;     // hit top 30% of obstacle -> reflect 45 degrees up
		else if(inBounds(topYBound, middleYBound, bullets->y)) bullets->dY = 0;                   // hit middle 40% of obstacle -> reflect straight back
		else if(inBounds(middleYBound, bottomYBound, bullets->y)) bullets->dY = BULLET_SPEED;
		else bullets -> dY = 0;
		// hit bottom 30% of obstacle -> reflect 45 degrees down
	}
    if(obstacleType == ROCK){
        int i;
        for(i = 0; i < 20; ++i){
            if(cowboyBullets[i].drawn == false && cowboyBullets[i+1].drawn == false){
				
                // create 2 new bullets if available
                cowboyBullets[i].x = cowboyBullets[i+1].x = bullets -> x;
                cowboyBullets[i].y = cowboyBullets[i+1].y = bullets->y;
                cowboyBullets[i].dX = cowboyBullets[i+1].dX = (bullets->dX > 0) ? BULLET_SPEED : -BULLET_SPEED;;
                cowboyBullets[i].dY = BULLET_SPEED;
				cowboyBullets[i+1].dY = -BULLET_SPEED;
				cowboyBullets[i].drawn = cowboyBullets[i+1].drawn = true;
				break;
            }
        }
    }
}

void checkCollision(struct CowboyBullet* bullets){
    int i, j;
    for(i = 0; i < 8; ++i){
        obstacleIdx = i;
        // check if bullet hit any obstacles
        if(objectsCollide(bullets->x, bullets->y, BULLET_WIDTH, BULLET_HEIGHT, activeObstacles[i].x, activeObstacles[i].y, OBSTACLES_WIDTH, OBSTACLES_HEIGHT)){
			printf("Collisionwith obstacle of type %d\n", (int) activeObstacles[i].type);
            collisionHandler(bullets);
        }
	}

        // check if bullet hit red player
	if(objectsCollide(bullets->x, bullets->y, BULLET_WIDTH, BULLET_HEIGHT, redCowboy.x, redCowboy.y, COWBOY_WIDTH, COWBOY_HEIGHT)){
            draw_rectangle(bullets -> x, bullets -> y, BULLET_WIDTH + 2, BULLET_HEIGHT + 2, TAN);
			blueCowboy.score ++;
            resetCowboy();
       }

        // check if bullet hit blue player
		if(objectsCollide(bullets->x, bullets->y, BULLET_WIDTH, BULLET_HEIGHT, blueCowboy.x, blueCowboy.y, COWBOY_WIDTH, COWBOY_HEIGHT)){
			draw_rectangle(bullets -> x, bullets -> y, BULLET_WIDTH + 2, BULLET_HEIGHT + 2, TAN);
            redCowboy.score ++;
            resetCowboy();
        }
    
}

void moveCBullets(){
    int i;
    for(i = 0; i < 20; ++i){
        if(cowboyBullets[i].drawn == true){
            cowboyBullets[i].lastX = cowboyBullets[i].x;
            cowboyBullets[i].lastY = cowboyBullets[i].y;            
            cowboyBullets[i].x += cowboyBullets[i].dX;
            cowboyBullets[i].y += cowboyBullets[i].dY;
            checkCollision(&cowboyBullets[i]);
            if(!inBounds(0, RESOLUTION_X, cowboyBullets[i].x) || !inBounds(MIN_Y, RESOLUTION_Y, cowboyBullets[i].y)){
                draw_rectangle(cowboyBullets[i].x, cowboyBullets[i].y, BULLET_WIDTH + 5, BULLET_HEIGHT + 5, TAN);
                cowboyBullets[i].drawn = false;
            }
        }
    }
}

void readKeys(){
    int* keyPtr = (int*)(KEY_BASE + 0xC);
    if(*keyPtr & 0x1){
        if(redCowboy.fired == true){
            redCowboy.dY = -redCowboy.dY;
        }
        else{
			printf("Red shooting\n");
            redCowboy.fired = true;
            shootCBullets(redCowboy.x - 5, redCowboy.y, -1, 0);
        }
        *keyPtr = 0x1;
    }
    if(*keyPtr & 0x8){
        if(blueCowboy.fired == true){
            blueCowboy.dY = -blueCowboy.dY;
        }
        else{
			printf("Blue shooting\n");
            blueCowboy.fired = true;
			shootCBullets(blueCowboy.x + COWBOY_WIDTH + 5, blueCowboy.y, 1, 0);
            //shootBullets(COWBOY_INITIAL_X + COWBOY_WIDTH, COWBOY_INITIAL_Y + (COWBOY_HEIGHT/2), 2, 0);
        }
        *keyPtr = 0xf;
    }
}














