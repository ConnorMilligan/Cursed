#ifndef SDLWINDOW_H
#define SDLWINDOW_H

#include <SDL.h>
#include <SDL_image.h>

int SDL_init(int width, int height, char *title, SDL_Window *window, SDL_Renderer *renderer, int flags);


#endif // SDLWINDOW_H