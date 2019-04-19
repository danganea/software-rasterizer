#include <SDL.h>
#include <iostream>
using namespace std;

constexpr uint32_t WINDOW_WIDTH = 640;
constexpr uint32_t WINDOW_HEIGHT = 640;

int main(int argc, char *argv[]) {
  bool quit = false;
  SDL_Event event;

  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window =
      SDL_CreateWindow("SDL2 Pixel Drawing", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_Texture *frameBuffer = SDL_CreateTexture(
      renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
      WINDOW_WIDTH, WINDOW_HEIGHT);

  uint32_t *prerendered_Frame = new uint32_t[WINDOW_WIDTH * WINDOW_HEIGHT];
  memset(prerendered_Frame, 0, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(uint32_t));

  for (int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT / 2; ++i) {
    uint8_t colors[4] = {};
    colors[0] = 0x00; // BLUE
    colors[1] = 0x00; // GREEN
    colors[2] = 0xFF; // RED
    colors[3] = 0xFF;

		memcpy(&prerendered_Frame[i], colors, sizeof(colors));
  }

  while (!quit) {
    SDL_WaitEvent(&event);

    switch (event.type) {
      case SDL_QUIT:
        quit = true;
        break;
    }

    void *target = 0;
    int pitch;
    SDL_LockTexture(frameBuffer, NULL, &target, &pitch);
    memcpy(target, prerendered_Frame, WINDOW_WIDTH * WINDOW_HEIGHT * 4);

    SDL_UnlockTexture(frameBuffer);
    SDL_RenderCopy(renderer, frameBuffer, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}