#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct texture {
    int w;
    int h;
    SDL_Texture* src;

} texture;

texture* createTexture() {
    texture* t = malloc(sizeof(texture));
    if (t == NULL) {
        return NULL;
    }
    t->w = 0;
    t->h = 0;
    t->src = NULL;

    return t;
}

void deleteTexture(texture* t) {
   if (t->src != NULL) {
       SDL_DestroyTexture(t->src);
       t->w = 0;
       t->h = 0;
       t->src = NULL;
   }
}

int loadTextureFromFile(SDL_Renderer* renderer, texture* tex, const char* file) {

    deleteTexture(tex);

    SDL_Surface* s = IMG_Load(file);
    if (s == NULL) {
        printf("IMG Error: %s\n", IMG_GetError());
        return 0;
    }

    tex->src = SDL_CreateTextureFromSurface(renderer, s);
    if (tex->src == NULL) {
        printf("%s\n", SDL_GetError());
        return 0;
    }

    tex->w = s->w;
    tex->h = s->h;
    SDL_FreeSurface(s);
    return 1;
}

void destroyTexture(texture* t) {
    if (t != NULL) {
        deleteTexture(t);
        free(t);
    }
}

texture* createTextTexture(const char* str, SDL_Renderer* renderer, TTF_Font* font, SDL_Color color) {

    texture* t = createTexture();
    if (t == NULL) {
        return NULL;
    }

    SDL_Surface* s = TTF_RenderUTF8_Solid(font, str, color);
    if (s == NULL) {
        printf("%s\n", SDL_GetError());
        return NULL;
    }

    t->w = s->w;
    t->h = s->h;

    t->src = SDL_CreateTextureFromSurface(renderer, s);
    if (t->src == NULL) {
        return NULL;
    }
    SDL_FreeSurface(s);

    return t;
}


#endif
