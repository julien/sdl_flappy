#ifndef __PIPES_H__
#define __PIPES_H__

#include <stdlib.h>
#include <time.h>
#include "bird.h"


const int maxpipes = 25;

typedef struct pipe {
    float x;
    int w;
    int h;
    int maxw;
    int maxh;
    int up;
} pipe;

pipe* createPipe(float maxw, float maxh) {
    pipe* p = malloc(sizeof(pipe));
    if (p == NULL) {
        return NULL;
    }

    p->x = 0;
    p->w = 52;
    p->h = 100;
    p->maxw = maxw;
    p->maxh = maxh;
    p->up = 0;

    return p;
}

int checkPipeCollission(pipe* p, bird* b) {
    if (b->x + b->w <= p->x || b->x >= p->x) {
        return 0;
    }
    if (p->up == 0) {
        return b->y <= p->h;
    }
    return b->y + b->h >= p->maxh - p->h;
    return 1;
}

void destroyPipe(pipe* p) {
    if (p != NULL) {
        free(p);
    }
}

// ----------------------

typedef struct pipeList {
    pipe* elements[25];
    SDL_Texture* src;
    // x velocity
    float vx;
} pipeList;

pipeList* createPipeList(SDL_Texture* src, float vx, float maxw, float maxh) {
    pipeList* p = malloc(sizeof(pipeList));
    if (p == NULL) {
        return NULL;
    }

    p->src = src;
    p->vx = vx;

    srand(time(NULL));

    int i;
    for (i = 0; i < maxpipes; i++) {
        pipe* e = createPipe(maxw, maxh);
        if (e == NULL) {
            continue;
        }
        e->x =  e->maxw + (rand() % e->maxw + 1);
        e->h =  rand() % (e->maxh / 4) + 1;
        e->up = rand() % 10 > 4 ? 1 : 0;

        p->elements[i] = e;
    }


    return p;
}

void updatePipeList(pipeList* p) {
    int i;
    for (i = 0; i < 10; i++) {
        pipe* e = p->elements[i];
        e->x -= p->vx;
        if (e->x < -e->w) {
            e->x =  e->maxw + (rand() % e->maxw + 1);
            e->h =  rand() % (e->maxh / 4) + 1;
            e->up = rand() % 10 > 4 ? 1 : 0;
        }
    }
}

void drawPipeList(pipeList* p, SDL_Renderer* r) {
    int i;
    for (i = 0; i < 10; i++) {
        pipe* e = p->elements[i];
        SDL_Rect rect = {e->x, e->maxh - e->h, e->w, e->h};
        SDL_RendererFlip flip = SDL_FLIP_NONE;

        if (!e->up) {
            rect.h = e->h;
            rect.y = 0;
            flip = SDL_FLIP_VERTICAL;
        }
        SDL_RenderCopyEx(r, p->src, NULL, &rect, 0, NULL, flip);
    }
}

int checkPipeListCollision(pipeList* p, bird* b) {
    int i;
    for (i = 0; i < maxpipes; i++) {
        pipe* e = p->elements[i];
        if (checkPipeCollission(e, b)) {
            return 1;
        }
    }
    return 0;
}

void destroyPipeList(pipeList* p) {
    if (p != NULL) {
        int i;
        for (i = 0; i < maxpipes; i++) {
            destroyPipe(p->elements[i]);
        }
        free(p);
    }
}

#endif
