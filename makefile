all:
	gcc main.c render.c maze.c dijkstra.c settings.c -o main.exe -IE:\MAZE_SOLVER\src\include -LE:\MAZE_SOLVER\src\lib -lmingw32 -lSDL2main -lSDL2_mixer -lSDL2 -lSDL2_ttf -lwinmm
