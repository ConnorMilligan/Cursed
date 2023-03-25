#define SDL_MAIN_HANDLED

#include <stdio.h>
#include "SDLwindow.h"

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 12
#define CHARS_PER_ROW 16
#define FONT_SPACING 0

int main(int argc, char* argv[]) {
    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_init(640, 480, "SDL window test", window, renderer, SDL_WINDOW_SHOWN);

    // Load the font image from file
    SDL_Surface* fontSurface = IMG_Load("curses_640x300.bmp");
    if (!fontSurface) {
        printf("Failed to load font image: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Convert the font surface to the correct pixel format
    SDL_Surface* fontSurfaceConverted = SDL_ConvertSurfaceFormat(fontSurface, SDL_PIXELFORMAT_ARGB8888, 0);
    if (!fontSurfaceConverted) {
        printf("Failed to convert font surface to ARGB8888 format: %s\n", SDL_GetError());
        SDL_FreeSurface(fontSurface);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_FreeSurface(fontSurface);

    // Create textures for each character in the font
    SDL_Texture* charTextures[256];
    for (int i = 0; i < 256; i++) {
        // Create a surface for the character
        SDL_Surface* charSurface = SDL_CreateRGBSurface(0, CHAR_WIDTH, CHAR_HEIGHT, 32, 0, 0, 0, 0);

        // Copy the pixels for the character from the font surface
        SDL_Rect srcRect = { (i % CHARS_PER_ROW) * CHAR_WIDTH, (i / CHARS_PER_ROW) * CHAR_HEIGHT, CHAR_WIDTH, CHAR_HEIGHT };
        SDL_BlitSurface(fontSurfaceConverted, &srcRect, charSurface, NULL);

        // Create a texture from the character surface
        charTextures[i] = SDL_CreateTextureFromSurface(renderer, charSurface);

        // Free the character surface
        SDL_FreeSurface(charSurface);
    }
    SDL_FreeSurface(fontSurfaceConverted);

    // Render some text using the font
    const char* text = "Hello, world!";
    int x = 0;
    int y = 0;
    for (const char* c = text; *c != '\0'; c++) {
        int charIndex = (int)*c;
        SDL_Texture* charTexture = charTextures[charIndex];
        if (charTexture) {
            SDL_Rect dstRect = { x, y, CHAR_WIDTH, CHAR_HEIGHT };
            SDL_RenderCopy(renderer, charTexture, NULL, &dstRect);
        }
        x += CHAR_WIDTH + FONT_SPACING;
    }

    // Wait for the user to close the window
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            break;
        }
    }

    // Cleanup resources
    for (int i = 0; i < 256; i++) {
        SDL_DestroyTexture(charTextures[i]);
    }
    SDL_FreeSurface(fontSurface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
