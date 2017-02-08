#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "bird.h"
#include "pipes.h"
#include "texture.h"

const int maxw = 800;
const int maxh = 600;

SDL_Window*   window;
SDL_Renderer* renderer;

texture* textures[6];
const char* images[6] = {
    "res/imgs/background.png",
    "res/imgs/bird_frame_1.png",
    "res/imgs/bird_frame_2.png",
    "res/imgs/bird_frame_3.png",
    "res/imgs/bird_frame_4.png",
    "res/imgs/pipe.png"
};

bird* flappy;
SDL_Texture* birdtextures[4];

const char* fontfile = "res/fonts/font.ttf";
const int fontsize = 50;
SDL_Color fontcolor = {255, 100, 0, 255};
TTF_Font* font;

texture* endtex;
SDL_Rect endrect;

pipeList* pipes;

// ---------------

int init();
int loadAssets();
int initAssets();
void stop();
void destroy();

// ----------------
int init() {

    int result = 1;

    srand(time(NULL));

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("%s\n", SDL_GetError());
        result = 0;
    }

    if (IMG_Init(IMG_INIT_PNG) < 0) {
        printf("%s\n", IMG_GetError());
        result = 0;
    }

    if (TTF_Init() == -1) {
        printf("%s\n", TTF_GetError());
        result = 0;
    }

    window = SDL_CreateWindow("Flappy",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              maxw,
                              maxh,
                              SDL_WINDOW_RESIZABLE);

    if (window == NULL) {
        printf("%s\n", SDL_GetError());
        result = 0;
    }

    // TODO: find out why
    // SDL_RENDERER_ACCELERATED is very slow on mac os.
    // SDL_RENDERER_SOFTWARE seems to work the same way on all 3 platforms
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        printf("%s\n", SDL_GetError());
        result = 0;
    }

    return result;
}

int loadAssets(SDL_Renderer* renderer) {

    int result = 1;

    int i;
    for (i = 0; i < 6; i++) {

        texture* tex = createTexture();
        if (tex == NULL) {
            result = 0;
            continue;
        }

        const char* file = images[i];
        if (loadTextureFromFile(renderer, tex, file) == 0) {
            printf("Could not create textures for: %s\n", file);
            result = 0;
            continue;
        }

        textures[i] = tex;
    }

    font =  TTF_OpenFont(fontfile, fontsize);
    if (font == NULL) {
        printf("%s\n", TTF_GetError());
        result = 0;
    }

    return result;
}

int initAssets() {
    int result = 1;

    int i, j;
    for (i = 1, j = 0; i < 5; i++, j++) {
        birdtextures[j] = textures[i]->src;
    }

    float x = (maxw - 50) / 2;
    float y = (maxh - 43) / 2;

    flappy = createBird(x, y, birdtextures);
    if (flappy == NULL) {
        printf("Could not create flappy\n");
        result = 0;
    }

    pipes = createPipeList(textures[5]->src, 0.6, maxw, maxh);
    if (pipes == NULL) {
        printf("Couldn't create pipes\n");
        result = 0;
    }

    endtex = createTextTexture("GAME OVER", renderer, font, fontcolor);
    if (endtex == NULL) {
        printf("Couldn't create game over text texture\n");
        result = 0;
    }

    endrect.x = (maxw - endtex->w) / 2;
    endrect.y = (maxh - endtex->h) / 2;
    endrect.w = endtex->w;
    endrect.h = endtex->h;

    return result;
}

void destroy() {

    destroyBird(flappy);
    destroyPipeList(pipes);

    destroyTexture(endtex);

    int i;
    for (i = 0; i < 6; i++) {
        destroyTexture(textures[i]);
    }

    IMG_Quit();
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char *argv[]) {

    if (!init()) {
        printf("Could not initialize game\n");
        return 2;
    } else {
       if (!loadAssets(renderer)) {
           printf("Could not load assets\n");
           return 2;
       }

       if (!initAssets()) {
           printf("Coulnd't initialize assets\n");
           return 2;
       }

       SDL_Event event;
       int quit = 0;

       while (!quit) {

           while (SDL_PollEvent(&event) != 0) {

               if (event.type == SDL_QUIT) {
                   quit = 1;
               }

               if (event.type == SDL_MOUSEBUTTONDOWN) {
                   if (flappy->dead == 1) {

                       SDL_Delay(16 * 5);
                       resetBird(flappy, 0.2);
                       flappy->dead = 0;

                       continue;
                   }
                   tiltBird(flappy, 35.0);
               }
           }

           // render
           SDL_RenderClear(renderer);

           if (flappy->dead == 1) {
               resetBird(flappy, 0.0);
               SDL_RenderCopy(renderer, endtex->src, NULL, &endrect);
           }


           updatePipeList(pipes);
           updateBird(flappy);

           if (checkPipeListCollision(pipes, flappy) == 1) {
               flappy->dead = 1;
               continue;
           }

           drawPipeList(pipes, renderer);
           drawBird(flappy, renderer);

           SDL_RenderPresent(renderer);
       }

       destroy();
    }

    return 0;
}
