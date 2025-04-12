#include "utils.h"
#include <stdlib.h>
#include <time.h>

void generate_maze() {
    srand(time(NULL));
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            maze[i][j] = WALL;

    int x = 0, y = 0;
    maze[x][y] = PATH;

    while (x != ROWS - 1 || y != COLS - 1) {
        if (x == ROWS - 1) y++;
        else if (y == COLS - 1) x++;
        else if (rand() % 2) x++;
        else y++;
        maze[x][y] = PATH;
    }

    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (maze[i][j] != PATH)
                maze[i][j] = (rand() % 100 < WALL_CHANCE) ? WALL : PATH;

    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            weights[i][j] = (maze[i][j] != WALL) ? rand() % 9 + 1 : -1;

    maze[0][0] = START;
    maze[ROWS - 1][COLS - 1] = GOAL;
    player.x = 0;
    player.y = 0;
    user_path_length = 0;
    user_path[user_path_length++] = player;
}