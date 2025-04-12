#include "utils.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>

void render(SDL_Renderer* renderer, TTF_Font* font, Point player) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            SDL_Rect cell = {j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE};

            switch (maze[i][j]) {
                case WALL:  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); break;
                case PATH:  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); break;
                case START: SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); break;
                case GOAL:  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); break;
                case TRAIL: SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); break; // green
            }

            SDL_RenderFillRect(renderer, &cell);

            if (maze[i][j] != WALL && weights[i][j] != -1) {
                char text[3];
                snprintf(text, sizeof(text), "%d", weights[i][j]);
                SDL_Color textColor = {0, 0, 0, 255};
                SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

                SDL_Rect textRect;
                textRect.x = j * CELL_SIZE + CELL_SIZE / 2 - textSurface->w / 2;
                textRect.y = i * CELL_SIZE + CELL_SIZE / 2 - textSurface->h / 2;
                textRect.w = textSurface->w;
                textRect.h = textSurface->h;

                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(textTexture);
            }

            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            SDL_RenderDrawRect(renderer, &cell);
        }
    }

    // Draw semi-transparent green overlay on the player
    SDL_Rect p = {player.y * CELL_SIZE, player.x * CELL_SIZE, CELL_SIZE, CELL_SIZE};
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);  // Transparent green
    SDL_RenderFillRect(renderer, &p);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    SDL_RenderPresent(renderer);
}
