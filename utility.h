#ifndef UTILITY_H
#define UTILITY_H
#include <stdbool.h>

#define FLOOR_Y 200
#define ABS(x) (((x) > 0) ? (x) : -(x))
static int randFactor = 0;

int maxInt(int a, int b);

int minInt(int a, int b);

bool objectsCollide(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);

bool inBounds(int min, int max, int val);

int getRandom(int min, int max);

void displayScore(int scoreLeft, int scoreRight);
#endif