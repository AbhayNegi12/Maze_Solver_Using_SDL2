#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

#define CELL_SIZE 60
#define WALL 1
#define PATH 0
#define START 2
#define GOAL 3
#define TRAIL 4

typedef struct {
    int x, y;
} Point;

extern int ROWS, COLS, WALL_CHANCE;
extern int SCREEN_WIDTH, SCREEN_HEIGHT;
extern int maze[100][100];
extern int weights[100][100];
extern Point player;
extern Point user_path[10000];
extern int user_path_length;

bool is_valid(int x, int y);

#endif
