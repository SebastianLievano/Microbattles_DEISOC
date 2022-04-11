#include "address_map_arm.h"
#include "utility.h"
#include "graphics.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//Returns  true if in bounds
bool inBounds(int min, int max, int val){
    if(val < min || val > max) return false;
    else return true;
}

int maxInt(int a, int b){
    return (a > b) ? a:b;
}

int minInt(int a, int b){
    return (a < b) ? a:b;
}

/*
When two objects are colliding, there must be some overlap in both x and y coords. 
This means the larger width
*/
bool objectsCollide(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2){
    return  (maxInt(x1, x2) < minInt(x1 + width1, x2 + width2)
                &&
            maxInt(y1, y2) < minInt(y1 + height1, y2 + height2));
}

int getRandom(int min, int max){
    time_t t;
    srand((unsigned) time(&t));
    int num = (rand() % (max - min +  1)) + min;
    return num;
}

void displayScore(int leftScore, int rightScore){
    *((int*)HEX3_HEX0_BASE) = bitCodes[rightScore];
    *((int*)HEX5_HEX4_BASE) = bitCodes[leftScore];
}