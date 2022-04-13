/*
graphics.c
=====
File containing functions that involve drawing onto the
screen. 
*/

#include "graphics.h"
#include "address_map_arm.h"
#include <stdlib.h>
#include <stdbool.h>


void swap(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}


void draw_line(int x1, int y1, int x2, int y2, short int color){
    //Write algorithm here
    bool isSteep = ABS(y2 - y1) > ABS(x2-x1);
    if (isSteep){
        swap(&x1, &y1);
        swap(&x2, &y2);
    }
    
    if(x1 > x2){
        swap(&x1, &x2);
        swap(&y1, &y2);
    }

    int deltax = x2 - x1;
    int deltay = ABS(y2 - y1);
    int error = -(deltax/2);
    int y = y1;
    int yStep = (y1 < y2) ? 1 : -1;
    int x;
    for(x = x1; x != x2; x++){
        if(isSteep) plot_pixel(y, x, color);
        else plot_pixel(x, y, color);
        error = error + deltay;
        if(error > 0){
            y = y + yStep;
            error = error - deltax;
        }
    }
}

void clear_screen(){
    draw_rectangle(0,0, RESOLUTION_X, RESOLUTION_Y, BLACK);
}

void wait_for_vsync(){
    volatile int * pixel_ctrl_ptr = (int*)(PIXEL_BUF_CTRL_BASE);
    *pixel_ctrl_ptr = 1;
    while(1){
        if(*(pixel_ctrl_ptr + 3) & 0x01) continue;
        else break;
    }
}

bool checkBounds(int x, int y){
    if(x < 0 || y < 0) return false;
    if(x >= RESOLUTION_X || y >= RESOLUTION_Y) return false;
    return true;
}

void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

void draw_rectangle(int x1, int y1, int width, int height, short int color){
    unsigned itX, itY;
    int plotX, plotY;
    for(itY = 0; itY < height; itY++){
        for(itX = 0; itX < width; ++itX){
            plotX = itX + x1;
            plotY = itY + y1;
            if(checkBounds(plotX, plotY))
                plot_pixel(plotX, plotY, color);
        }
    }
}

void draw_image(int start_x, int start_y, int image[], int width, int height){
    unsigned itX, itY;
    int plotX, plotY, ctr = 0;
    for(itY = 0; itY < height; ++itY){
        for(itX = 0; itX < width; ++itX){
            plotX = itX + start_x;
            plotY = itY + start_y;
            if(checkBounds(plotX, plotY))
                plot_pixel(plotX, plotY, image[ctr]);
            ctr++;
        }
    }
}