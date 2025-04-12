#include "utils.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

void wait_for_level(SDL_Renderer* renderer) {
  SDL_Event event;
  bool selected = false;

  TTF_Font* font = TTF_OpenFont("ARIAL.TTF", 36);
  if (!font) {
      printf("Font load error: %s\n", TTF_GetError());
      exit(1);
  }

  SDL_Color color = {255, 255, 255, 255};
  SDL_Surface* s1 = TTF_RenderText_Solid(font, "Press E for Easy ", color);
  SDL_Surface* s2 = TTF_RenderText_Solid(font, "Press H for Hard ", color);

  SDL_Texture* t1 = SDL_CreateTextureFromSurface(renderer, s1);
  SDL_Texture* t2 = SDL_CreateTextureFromSurface(renderer, s2);

  SDL_Rect r1 = {(600 - s1->w) / 2, 200, s1->w, s1->h};
  SDL_Rect r2 = {(600 - s2->w) / 2, 280, s2->w, s2->h};

  SDL_FreeSurface(s1);
  SDL_FreeSurface(s2);

  while (!selected) {
      while (SDL_PollEvent(&event)) {
          if (event.type == SDL_QUIT) exit(0);
          if (event.type == SDL_KEYDOWN) {
              if (event.key.keysym.sym == SDLK_e) {
                  ROWS = 5; COLS = 5; WALL_CHANCE = 40; selected = true;
              }
              if (event.key.keysym.sym == SDLK_h) {
                  ROWS = 10; COLS = 10; WALL_CHANCE = 70; selected = true;
              }
          }
      }

      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);
      SDL_RenderCopy(renderer, t1, NULL, &r1);
      SDL_RenderCopy(renderer, t2, NULL, &r2);
      SDL_RenderPresent(renderer);
      SDL_Delay(1);
  }

  SDL_DestroyTexture(t1);
  SDL_DestroyTexture(t2);
  TTF_CloseFont(font);
}