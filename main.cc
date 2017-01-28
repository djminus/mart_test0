// SDL demo

#include <cstdio>
#include <cstdarg>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

using std::string;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void logf(const char* fmt, ...) {
  FILE *f = stderr;
  va_list arglist;
  va_start(arglist, fmt);
  vfprintf(f, fmt, arglist);
  va_end(arglist);
  fprintf(f, "\n");
}

void logErrorSDL() {
  logf("SDL error: %s", SDL_GetError());
}

int main(int argc, char *argv[]) {
  logf("Starting ...");
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    logErrorSDL();
    return 1;
  }
  if (TTF_Init() != 0) {
    logErrorSDL();
    return 1;
  }
  TTF_Font *font = TTF_OpenFont("assets/FreeMono.ttf", 16);
  if (font == nullptr) {
    logErrorSDL();
    return 1;
  }
  int mixFlags = MIX_INIT_OGG;
  if (Mix_Init(mixFlags) != mixFlags) {
    logErrorSDL();
    return 1;
  }
  if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024) < 0) {
    logErrorSDL();
    return 1;
  }
  Mix_Music *mus = Mix_LoadMUS("assets/BAKGM14a.ogg");
  if (mus == nullptr) {
    logErrorSDL();
    return 1;
  }
  SDL_Window *win =
      SDL_CreateWindow(
          "SDL Demo",
          SDL_WINDOWPOS_UNDEFINED,
          SDL_WINDOWPOS_UNDEFINED,
          SCREEN_WIDTH,
          SCREEN_HEIGHT,
          //SDL_WINDOW_SHOWN
          SDL_WINDOW_RESIZABLE
          //SDL_WINDOW_FULLSCREEN_DESKTOP
          );
  if (win == nullptr) {
    logErrorSDL();
    return 1;
  }
  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  if (ren == nullptr) {
    logErrorSDL();
    return 1;
  }
  SDL_Surface *imgSurf = IMG_Load("assets/haddock_grumpy.jpg");
  if (imgSurf == nullptr) {
    logErrorSDL();
    return 1;
  }
  SDL_Texture *imgTex = SDL_CreateTextureFromSurface(ren, imgSurf);
  if (imgTex == nullptr) {
    logErrorSDL();
    return 1;
  }
  SDL_SetRenderDrawColor(ren, 0x8c, 0xb7, 0x4c, 0xff);
  char status[1000];
  for (bool quit = false; !quit;) {
    snprintf(status, sizeof(status), "%08d", SDL_GetTicks());
    SDL_Surface *ticksSurf =
        TTF_RenderText_Solid(font, status, SDL_Color {0xff, 0xff, 0xff});
    if (ticksSurf == nullptr) {
      logErrorSDL();
      return 1;
    }
    SDL_Texture *ticksTex = SDL_CreateTextureFromSurface(ren, ticksSurf);
    if (ticksTex == nullptr) {
      logErrorSDL();
      return 1;
    }
    SDL_Rect ticksRect {0, 0, ticksSurf->w, ticksSurf->h};
    int w, h;
    SDL_GetRendererOutputSize(ren, &w, &h);
    SDL_Rect imgRect { w - imgSurf->w, h - imgSurf->h, imgSurf->w, imgSurf->h };
    SDL_RenderClear(ren);
    SDL_RenderCopy(ren, imgTex, nullptr, &imgRect);
    SDL_RenderCopy(ren, ticksTex, nullptr, &ticksRect);
    SDL_RenderPresent(ren);
    SDL_DestroyTexture(ticksTex);
    SDL_FreeSurface(ticksSurf);
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          quit = true;
          continue;
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
              quit = true;
              break;
            case SDLK_SPACE:
              if (Mix_PlayingMusic()) {
                if (Mix_PausedMusic()) {
                  Mix_ResumeMusic();
                } else {
                  Mix_PauseMusic();
                }
              } else {
                Mix_PlayMusic(mus, 1 /* once */);
              }
              break;
            default:
              break;
          }
          break;
        default:
          break;
      }
    }
  }
  logf("Exiting ...");
  TTF_Quit();
  Mix_CloseAudio();
  Mix_FreeMusic(mus);
  Mix_Quit();
  SDL_FreeSurface(imgSurf);
  SDL_DestroyTexture(imgTex);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
