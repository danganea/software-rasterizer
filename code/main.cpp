#include "defs.h"
#include "surface.h"
#include "vector2d.h"

#include <SDL.h>
#include <iostream>

using namespace std;

constexpr uint32_t WINDOW_WIDTH = 640;
constexpr uint32_t WINDOW_HEIGHT = 640;

int main(int argc, char* argv[]) {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window* window =
      SDL_CreateWindow("SDL2 Pixel Drawing", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

  if (!window) {
    fprintf(stderr, "Window could not be created: %s!\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  int index = -1;
  uint32_t SDL_Renderer_Flags = 0;
  SDL_Renderer_Flags |= SDL_RENDERER_ACCELERATED;

  SDL_Renderer* renderer =
      SDL_CreateRenderer(window, index, SDL_Renderer_Flags);

  if (!renderer) {
    fprintf(stderr, "Renderer could not be created: %s!\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  SDL_Texture* frameBuffer = SDL_CreateTexture(
      renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
      WINDOW_WIDTH, WINDOW_HEIGHT);

  if (!frameBuffer) {
    fprintf(stderr, "Framebuffer could not be created: %s !\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  rast::Surface surface(WINDOW_WIDTH, WINDOW_HEIGHT);

  surface.drawLine(rast::ivec2(0, 0), rast::ivec2(0, 0));

  bool quit = false;
  SDL_Event event;

  while (!quit) {
    SDL_WaitEvent(&event);

    switch (event.type) {
      case SDL_QUIT:
        quit = true;
        break;
    }

    // Get mapped GPU texture & pitch of texture
    void* mappedTexture = nullptr;
    int pitch = 0;
    SDL_LockTexture(frameBuffer, NULL, &mappedTexture, &pitch);

    // Some architectures don't allow GPU data to have arbitrary 2D width.
    // Instead, data must be alligned to a "pitch".
    if (pitch == WINDOW_WIDTH * 4) {
      // Safe to just memcpy all the surface at once.
      memcpy(mappedTexture, surface.data, WINDOW_WIDTH * WINDOW_HEIGHT * 4);
    } else {
      // Need to loop over surface to handle pitch.
      unsigned char* t = reinterpret_cast<unsigned char*>(mappedTexture);
      for (size_t i = 0; i < WINDOW_HEIGHT; ++i) {
        memcpy(t + i * pitch, surface.data + i * WINDOW_WIDTH, WINDOW_WIDTH);
      }
    }

    SDL_UnlockTexture(frameBuffer);
    SDL_RenderCopy(renderer, frameBuffer, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}