#ifndef __BIRD_H__
#define __BIRD_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct bird {
    float x;
    float y;

    // original position
    float ox, oy;
    float vy;

    int w;
    int h;


    SDL_Texture* frames[4];
    int frame;

    int dead;
} bird;

bird* createBird(float x,
                 float y,
                 SDL_Texture* frames[4]) {

    bird* b = malloc(sizeof(bird));
    if (b == NULL) {
        return NULL;
    }

    b->dead = 0;
    b->frame = 0;

    b->w = 50;
    b->h = 43;


    b->x = x;
    b->y = y;

    b->ox = x;
    b->oy = y;

    b->vy = 0.2;

    int i;
    for (i = 0; i < 4; i++) {
        SDL_Texture* t = frames[i];
        if (t == NULL) {
            continue;
        }
        b->frames[i] = t;
    }

    return b;
}

void updateBird(bird* b) {

    b->y += b->vy;
    if (b->y > 600) {
        b->dead = 1;
    }

    b->frame = ((b->frame + 1) % 4);
}

void drawBird(bird* b, SDL_Renderer* renderer) {
   SDL_Rect rect = {b->x, b->y, b->w, b->h};
   SDL_Texture* t = b->frames[b->frame];
   if (t == NULL) {
       return;
   }
   SDL_RenderCopy(renderer, t, NULL, &rect);
}

void tiltBird(bird* b, float v) {
    b->y -= v;
}

void resetBird(bird* b, float vy) {
    b->x = b->ox;
    b->y = b->oy;
    b->vy = vy;
}

void destroyBird(bird* b) {
    if (b != NULL) {
        free(b);
    }
}

#endif
