#ifndef GRAPHICSMB
#define GRAPHICSMB

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

#define ABS(x) (((x) > 0) ? (x) : -(x))

/* Screen size. */
#define RESOLUTION_X 320
#define RESOLUTION_Y 240

char bitCodes[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x67};

extern volatile int pixel_buffer_start;

void draw_line(int x1, int y1, int x2, int y2, short int color);

void clear_screen();

void wait_for_vsync();

void swap(int* a, int* b);

void plot_pixel(int x, int y, short int line_color);

void draw_rectangle(int x1, int y1, int width, int length, short int color);

void draw_image(int start_x, int start_y, int image[], int width, int height);


#endif