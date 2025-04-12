#include "utils.h"
#include "maze.h"
#include "render.h"
#include "dijkstra.h"
#include "settings.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>

int ROWS = 10, COLS = 10, WALL_CHANCE = 70;
int SCREEN_WIDTH, SCREEN_HEIGHT;
int maze[100][100];
int weights[100][100];
Point player;
Point user_path[10000];
int user_path_length = 0;

bool is_valid(int x, int y) {
    return x >= 0 && x < ROWS && y >= 0 && y < COLS && maze[x][y] != WALL;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("SDL init error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("TTF init error: %s\n", TTF_GetError());
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }

    // Load sounds
    Mix_Chunk* startSound = Mix_LoadWAV("assets/start.wav");
    Mix_Chunk* stepSound = Mix_LoadWAV("assets/step.wav");
    Mix_Chunk* goalSound = Mix_LoadWAV("assets/goal.wav");

    if (startSound) Mix_PlayChannel(-1, startSound, 0);

    SDL_Window* tempWindow = SDL_CreateWindow("Maze Solver",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* tempRenderer = SDL_CreateRenderer(tempWindow, -1, SDL_RENDERER_ACCELERATED);

    wait_for_level(tempRenderer);
    SDL_DestroyRenderer(tempRenderer);
    SDL_DestroyWindow(tempWindow);

    SCREEN_WIDTH = COLS * CELL_SIZE;
    SCREEN_HEIGHT = ROWS * CELL_SIZE;

    SDL_Window* window = SDL_CreateWindow("Maze Solver",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font* font = TTF_OpenFont("ARIAL.TTF", 24);
    if (!font) {
        printf("Font load error: %s\n", TTF_GetError());
        exit(1);
    }

    generate_maze();

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
            else if (event.type == SDL_KEYDOWN) {
                int new_x = player.x;
                int new_y = player.y;

                switch (event.key.keysym.sym) {
                    case SDLK_UP:    new_x--; break;
                    case SDLK_DOWN:  new_x++; break;
                    case SDLK_LEFT:  new_y--; break;
                    case SDLK_RIGHT: new_y++; break;
                }

                if (user_path_length >= 2 &&
                    user_path[user_path_length - 2].x == new_x &&
                    user_path[user_path_length - 2].y == new_y &&
                    maze[new_x][new_y] != WALL) {

                    maze[player.x][player.y] = PATH;
                    user_path_length--;
                    player = user_path[user_path_length - 1];
                }
                else if (is_valid(new_x, new_y)) {
                    if (maze[player.x][player.y] != START &&
                        maze[player.x][player.y] != GOAL) {
                        maze[player.x][player.y] = TRAIL;
                    }

                    player.x = new_x;
                    player.y = new_y;
                    user_path[user_path_length++] = player;

                    if (stepSound) Mix_PlayChannel(-1, stepSound, 0);

                    if (maze[player.x][player.y] == GOAL) {
                        if (goalSound) Mix_PlayChannel(-1, goalSound, 0);

                        bool counted[100][100] = { false };
                        int total = 0;
                        for (int i = 0; i < user_path_length; i++) {
                            Point p = user_path[i];
                            if (!counted[p.x][p.y]) {
                                total += weights[p.x][p.y];
                                counted[p.x][p.y] = true;
                            }
                        }

                        printf("\nYour Path:");
                        for (int i = 0; i < user_path_length; i++) {
                            Point p = user_path[i];
                            printf("[%d] -> ", weights[p.x][p.y]);
                        }
                        printf(" = %d\n", total);
                        SDL_Delay(1500);
                        dijkstra(renderer, font);
                        running = false;
                    }
                }
            }
        }

        render(renderer, font, player);
        SDL_RenderPresent(renderer);
        SDL_Delay(5);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_FreeChunk(startSound);
    Mix_FreeChunk(stepSound);
    Mix_FreeChunk(goalSound);
    Mix_CloseAudio();
    Mix_Quit();

    TTF_Quit();
    SDL_Quit();
    return 0;
}
