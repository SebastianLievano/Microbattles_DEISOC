#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

volatile int pixel_buffer_start;
char bitCodes[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x67};


//address map arm.h
/* This files provides address values that exist in the system */

#define BOARD                 "DE1-SoC"

/* Memory */
#define DDR_BASE              0x00000000
#define DDR_END               0x3FFFFFFF
#define A9_ONCHIP_BASE        0xFFFF0000
#define A9_ONCHIP_END         0xFFFFFFFF
#define SDRAM_BASE            0xC0000000
#define SDRAM_END             0xC3FFFFFF
#define FPGA_ONCHIP_BASE      0xC8000000
#define FPGA_ONCHIP_END       0xC803FFFF
#define FPGA_CHAR_BASE        0xC9000000
#define FPGA_CHAR_END         0xC9001FFF

/* Cyclone V FPGA devices */
#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define JP1_BASE              0xFF200060
#define JP2_BASE              0xFF200070
#define PS2_BASE              0xFF200100
#define PS2_DUAL_BASE         0xFF200108
#define JTAG_UART_BASE        0xFF201000
#define JTAG_UART_2_BASE      0xFF201008
#define IrDA_BASE             0xFF201020
#define TIMER_BASE            0xFF202000
#define AV_CONFIG_BASE        0xFF203000
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030
#define AUDIO_BASE            0xFF203040
#define VIDEO_IN_BASE         0xFF203060
#define ADC_BASE              0xFF204000

/* Cyclone V HPS devices */
#define HPS_GPIO1_BASE        0xFF709000
#define HPS_TIMER0_BASE       0xFFC08000
#define HPS_TIMER1_BASE       0xFFC09000
#define HPS_TIMER2_BASE       0xFFD00000
#define HPS_TIMER3_BASE       0xFFD01000
#define FPGA_BRIDGE           0xFFD0501C

/* ARM A9 MPCORE devices */
#define   PERIPH_BASE         0xFFFEC000    // base address of peripheral devices
#define   MPCORE_PRIV_TIMER   0xFFFEC600    // PERIPH_BASE + 0x0600

/* Interrupt controller (GIC) CPU interface(s) */
#define MPCORE_GIC_CPUIF      0xFFFEC100    // PERIPH_BASE + 0x100
#define ICCICR                0x00          // offset to CPU interface control reg
#define ICCPMR                0x04          // offset to interrupt priority mask reg
#define ICCIAR                0x0C          // offset to interrupt acknowledge reg
#define ICCEOIR               0x10          // offset to end of interrupt reg
/* Interrupt controller (GIC) distributor interface(s) */
#define MPCORE_GIC_DIST       0xFFFED000    // PERIPH_BASE + 0x1000
#define ICDDCR                0x00          // offset to distributor control reg
#define ICDISER               0x100         // offset to interrupt set-enable regs
#define ICDICER               0x180         // offset to interrupt clear-enable regs
#define ICDIPTR               0x800         // offset to interrupt processor targets regs
#define ICDICFR               0xC00         // offset to interrupt configuration regs

//graphics.h

/* VGA colors */
#define WHITE 0xFFFF
#define YELLOW 0xFFE0
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define GREY 0xC618
#define PINK 0xFC18
#define ORANGE 0xFC00
#define BLACK 0x0000
#define SKY_BLUE 0x773F
#define FLOOR_GREEN 0x1E07


/* Screen size. */
#define RESOLUTION_X 320
#define RESOLUTION_Y 240

extern volatile int pixel_buffer_start;

void draw_line(int x1, int y1, int x2, int y2, short int color);

void clear_screen();

void wait_for_vsync();

void swap(int* a, int* b);

void plot_pixel(int x, int y, short int line_color);

void draw_rectangle(int x1, int y1, int width, int length, short int color);

void draw_image(int start_x, int start_y, int image[], int width, int height);

//images.h

int bluePlaneLeft_20x10[200] = {
    30526,30526,30526,16,16,16,16,16,16,16,30526,30526,30526,30526,30526,30526,30526,30526,30526,0,
    30526,30526,30526,30526,0,30526,30526,30526,0,30526,30526,30526,14528,14528,30526,30526,30526,30526,0,0,
    30526,30526,30526,30526,0,30526,30526,30526,0,30526,33808,30526,65303,14528,30526,30526,30526,0,34079,0,
    0,30526,30526,16,0,16,16,16,0,16,16,16,16,16,16,16,16,34079,34079,0,
    0,30526,16,34079,0,34079,34079,34079,0,34079,34079,65535,34079,34079,34079,34079,34079,0,0,0,
    65535,16,34079,34079,0,34079,34079,34079,0,34079,65535,34079,34079,34079,34079,16,16,30526,30526,30526,
    0,30526,16,34079,0,34079,34079,34079,0,34079,34079,65535,16,16,16,30526,30526,30526,30526,30526,
    0,30526,30526,16,0,16,16,16,0,16,16,16,30526,30526,30526,30526,30526,30526,30526,30526,
    30526,30526,30526,30526,0,30526,30526,30526,0,30526,30526,30526,30526,30526,30526,30526,30526,30526,30526,30526,
    30526,30526,30526,16,16,16,16,16,16,16,30526,30526,30526,30526,30526,30526,30526,30526,30526,30526
};

int bluePlaneRight_20x10[200] = {
    0,30526,30526,30526,30526,30526,30526,30526,30526,30526,16,16,16,16,16,16,16,30526,30526,30526,
    0,0,30526,30526,30526,30526,14561,14561,30526,30526,30526,0,30526,30526,30526,0,30526,30526,30526,30526,
    0,34079,0,30526,30526,30526,14561,65271,30526,33808,30526,0,30526,30526,30526,0,30526,30526,30526,30526,
    0,34079,34079,16,16,16,16,16,16,16,16,0,16,16,16,0,16,30526,30526,0,
    0,0,0,34079,34079,34079,34079,34079,65535,34079,34079,0,34079,34079,34079,0,34079,16,30526,0,
    30526,30526,30526,16,16,34079,34079,34079,34079,65535,34079,0,34079,34079,34079,0,34079,34079,16,65535,
    30526,30526,30526,30526,30526,16,16,16,65535,34079,34079,0,34079,34079,34079,0,34079,16,30526,0,
    30526,30526,30526,30526,30526,30526,30526,30526,16,16,16,0,16,16,16,0,16,30526,30526,0,
    30526,30526,30526,30526,30526,30526,30526,30526,30526,30526,30526,0,30526,30526,30526,0,30526,30526,30526,30526,
    30526,30526,30526,30526,30526,30526,30526,30526,30526,30526,16,16,16,16,16,16,16,30526,30526,30526
};

int redPlaneLeft_20x10[200] = {
    30526,30526,30526,34816,34816,34816,34816,34816,34816,34816,30526,30526,30526,30526,30526,30526,30526,30526,30526,0,
    30526,30526,30526,30526,0,30526,30526,30526,0,30526,30526,30526,14528,14528,30526,30526,30526,30526,0,0,
    30526,30526,30526,30526,0,30526,30526,30526,0,30526,33808,30526,65303,14528,30526,30526,30526,0,63488,0,
    0,30526,30526,34816,0,34816,34816,34816,0,34816,34816,34816,34816,34816,34816,34816,34816,63488,63488,0,
    0,30526,34816,63488,0,63488,63488,63488,0,63488,63488,65535,63488,63488,63488,63488,63488,0,0,0,
    65535,34816,63488,63488,0,63488,63488,63488,0,63488,65535,63488,63488,63488,63488,34816,34816,30526,30526,30526,
    0,30526,34816,63488,0,63488,63488,63488,0,63488,63488,65535,34816,34816,34816,30526,30526,30526,30526,30526,
    0,30526,30526,34816,0,34816,34816,34816,0,34816,34816,34816,30526,30526,30526,30526,30526,30526,30526,30526,
    30526,30526,30526,30526,0,30526,30526,30526,0,30526,30526,30526,30526,30526,30526,30526,30526,30526,30526,30526,
    30526,30526,30526,34816,34816,34816,34816,34816,34816,34816,30526,30526,30526,30526,30526,30526,30526,30526,30526,30526
};

int redPlaneRight_20x10[200] = {
    0,30526,30526,30526,30526,30526,30526,30526,30526,30526,34816,34816,34816,34816,34816,34816,34816,30526,30526,30526,
    0,0,30526,30526,30526,30526,14528,14528,30526,30526,30526,0,30526,30526,30526,0,30526,30526,30526,30526,
    0,63488,0,30526,30526,30526,14528,65303,30526,33808,30526,0,30526,30526,30526,0,30526,30526,30526,30526,
    0,63488,63488,34816,34816,34816,34816,34816,34816,34816,34816,0,34816,34816,34816,0,34816,30526,30526,0,
    0,0,0,63488,63488,63488,63488,63488,65535,63488,63488,0,63488,63488,63488,0,63488,34816,30526,0,
    30526,30526,30526,34816,34816,63488,63488,63488,63488,65535,63488,0,63488,63488,63488,0,63488,63488,34816,65535,
    30526,30526,30526,30526,30526,34816,34816,34816,65535,63488,63488,0,63488,63488,63488,0,63488,34816,30526,0,
    30526,30526,30526,30526,30526,30526,30526,30526,34816,34816,34816,0,34816,34816,34816,0,34816,30526,30526,0,
    30526,30526,30526,30526,30526,30526,30526,30526,30526,30526,30526,0,30526,30526,30526,0,30526,30526,30526,30526,
    30526,30526,30526,30526,30526,30526,30526,30526,30526,30526,34816,34816,34816,34816,34816,34816,34816,30526,30526,30526
};

int bullet_5x5[25] = {
    65535,52759,31370,44275,65535,
    54840,52499,37286,26721,59099,
    46323,39074,47689,37351,57051,
    63422,45998,43723,54710,65535,
    65535,65535,65535,65535,65535
};

int planePowerup_7x7[49] = {
    30526,65028,65028,65028,65028,65028,30526,
    65028,0,0,0,61991,65028,65028,
    65028,65028,65028,65028,65028,65028,65028,
    65028,65028,0,0,0,61991,65028,
    65028,65028,65028,65028,65028,65028,65028,
    65028,0,0,0,61991,65028,65028,
    30526,65028,65028,65028,65028,65028,30526
};

//plane.h

#define PLANE_WIDTH 20
#define PLANE_HEIGHT 10
#define PLANE_X_SPEED 1
#define PLANE_INITIAL_X 20
#define FLOOR_Y 180
#define BULLET_SPEED 1
#define BULLET_WIDTH 5
#define BULLET_HEIGHT 5
#define POWERUP_WIDTH 7
#define POWERUP_HEIGHT 7
#define PLANE_Y_SPEED 1
#define PLANE_ACCEL_INC 2
#define PLANE_MAX_ACCEL 4

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

//utility.h
#define ABS(x) (((x) > 0) ? (x) : -(x))

//UTILITY.C

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

//GRAPHICS.C


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
    int status;
    *pixel_ctrl_ptr = 1;
    status = *(pixel_ctrl_ptr + 3);
    while((status & 0x01) != 0){
        status = *(pixel_ctrl_ptr + 3);
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

void displayScore(int leftScore, int rightScore){
    *((int*)HEX3_HEX0_BASE) = bitCodes[rightScore];
    *((int*)HEX5_HEX4_BASE) = bitCodes[leftScore];
}

//PLANE.C

struct Bullet bullets[10];

int numBullets = 0;

struct Powerup powerups[2];


int main(void){
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
            draw_rectangle(bullets[i].x, bullets[i].y, BULLET_WIDTH + 2, BULLET_HEIGHT, SKY_BLUE);
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
