#include "utils.h"
#include <stdio.h>
#include <limits.h>

typedef struct {
    int dist;
    Point prev;
    bool visited;
} Node;

void dijkstra() {
  Node nodes[100][100];

  for (int i = 0; i < ROWS; i++)
      for (int j = 0; j < COLS; j++) {
          nodes[i][j].dist = INT_MAX;
          nodes[i][j].visited = false;
          nodes[i][j].prev = (Point){-1, -1};
      }

  nodes[0][0].dist = 0;

  while (true) {
      int minDist = INT_MAX;
      Point u = {-1, -1};

      for (int i = 0; i < ROWS; i++) {
          for (int j = 0; j < COLS; j++) {
              if (!nodes[i][j].visited && maze[i][j] != WALL && nodes[i][j].dist < minDist) {
                  minDist = nodes[i][j].dist;
                  u = (Point){i, j};
              }
          }
      }

      if (u.x == -1) break;
      nodes[u.x][u.y].visited = true;
      if (maze[u.x][u.y] == GOAL) break;

      Point dirs[] = {{-1,0},{1,0},{0,-1},{0,1}};
      for (int d = 0; d < 4; d++) {
          int nx = u.x + dirs[d].x;
          int ny = u.y + dirs[d].y;

          if (is_valid(nx, ny) && !nodes[nx][ny].visited) {
              int alt = nodes[u.x][u.y].dist + weights[nx][ny];
              if (alt < nodes[nx][ny].dist) {
                  nodes[nx][ny].dist = alt;
                  nodes[nx][ny].prev = u;
              }
          }
      }
  }

  Point path[10000];
  int length = 0;
  Point current = (Point){ROWS - 1, COLS - 1};
  int dijkstra_total = 0;

  while (current.x != -1 && current.y != -1) {
      path[length++] = current;
      current = nodes[current.x][current.y].prev;
  }

  printf("\nDijkstra Path:");
  for (int i = length - 1; i >= 0; i--) {
      Point p = path[i];
      dijkstra_total += weights[p.x][p.y];
      printf("[%d] -> ",weights[p.x][p.y]);
  }
  printf(" = %d \n\n", dijkstra_total);

}