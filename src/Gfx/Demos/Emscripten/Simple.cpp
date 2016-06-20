#include <Lp3/LE.h>
#define PLATFORM_NAME "Emscripten"
#include <Lp3/Engine/NoOptMain.h>
#ifdef LP3_COMPILE_TARGET_EMSCRIPTEN
#include <stdio.h>
#include <SDL/SDL.h>
//#include <SDL/SDL_ttf.h>
#include <emscripten.h>
#include <Lp3/Log.h>


int result = 1;

int keys[1000];

void input();
void render();

void iteration() {
    input();
    render();
}

void input() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_KEYDOWN:
        if (!keys[event.key.keysym.sym]) {
          keys[event.key.keysym.sym] = 1;
          printf("key down: sym %d scancode %d\n", event.key.keysym.sym, event.key.keysym.scancode);
        }
        break;
      case SDL_KEYUP:
        if (keys[event.key.keysym.sym]) {
          keys[event.key.keysym.sym] = 0;
          printf("key up: sym %d scancode %d\n", event.key.keysym.sym, event.key.keysym.scancode);
        }
        break;
    }
  }
}

void render() {

}

int main() {
    LP3_LOG_DEBUG("Hi from Javascript! FYI this is actually working.");

    for (size_t i = 0; i < 1000; i ++) {
        keys[i] = 0;
    }
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface *screen = SDL_SetVideoMode(600, 450, 32, SDL_HWSURFACE);

#ifdef EMSCRIPTEN
  emscripten_set_main_loop(iteration, 0, 0);
#else
  while (1) {
    iteration();
    SDL_Delay(time_to_next_frame());
  }
#endif

  return 0;
}
#endif
