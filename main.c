#include "graphics.h"
#include "cowboy.h"
#include "address_map_arm.h"
#include "plane.h"
#include "images.h"


volatile int pixel_buffer_start;

int main(void){
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    *(pixel_ctrl_ptr + 1) = 0xC8000000;
    pixel_buffer_start = *pixel_ctrl_ptr;
    wait_for_vsync();

    draw_image(0,0, mainBackground_320x240, RESOLUTION_X, RESOLUTION_Y);
    int* keyPtr = (int*)KEY_BASE;
    //Manage acceleration
    while (1){
        if(*keyPtr & 0x1) planeMain();
        else if(*keyPtr & 0x2) cowboyMain();
    }   
}