#define FNL_IMPL

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#include "include/fastnoise.h"
#include "include/field.h"
#include "include/key.h"
#include "include/automata.h"

#include "SDL.h"

uint16_t SIZE = 512;
enum Key_type RULE = conway;
uint16_t ITER = 100;
bool SDL = 0;
bool PPM = 0;

bool save_field_to_ppm(const char *filename, const Automata *automata){
    FILE *file_life;
    FILE *file_noise;

    file_life = fopen(filename, "wb");
    if (file_life == NULL){
        printf("ERROR: failed to open file\n");
        return false;
    }

    file_noise = fopen("noise.ppm", "wb");
    if (file_noise == NULL){
        printf("ERROR: failed to open file\n");
        return false;
    }

    const uint16_t rows = automata -> field -> rows;
    const uint16_t cols = automata -> field -> cols;

    fprintf(file_life, "P6\n%d %d\n%d\n", rows, cols, 255);
    fprintf(file_noise, "P6\n%d %d\n%d\n", rows, cols, 255);

    for (unsigned int i = 0; i < rows; ++i){
        for (unsigned int j = 0; j < cols; ++j){
            unsigned char pixel = automata -> field -> map[i][j] * 255 / (automata -> key -> n_states - 1);
            fwrite(&pixel, 1, 1, file_life);
            fwrite(&pixel, 1, 1, file_life);
            fwrite(&pixel, 1, 1, file_life);

            pixel = (automata -> field -> noise[i][j] + 1.0f) / 2.0f * 255 ;
            fwrite(&pixel, 1, 1, file_noise);
            fwrite(&pixel, 1, 1, file_noise);
            fwrite(&pixel, 1, 1, file_noise);
        }
    }

    fclose(file_life);
    fclose(file_noise);
    return true;
}

void add_glider(Field *field, const uint16_t x, const uint16_t y){
    field -> map[x][y + 2] = 1;
    field -> map[x + 1][y] = 1;
    field -> map[x + 1][y + 2] = 1;
    field -> map[x + 2][y + 1] = 1;
    field -> map[x + 2][y + 2] = 1;
}

bool plant_seed(Field *field){
    if (field -> rows < 2 && field -> cols < 2){
        printf("ERROR: seed can not fit the field\n");
        return 0;
    }
    const uint16_t mid_row = floor(field -> rows / 2);
    const uint16_t mid_col = floor(field -> cols / 2);
    for (unsigned int i = mid_row - 1; i < mid_row + 1; ++i){
        for (unsigned int j = mid_col - 1; j < mid_col + 1; ++j){
            field -> map[i][j] = 1;
        }
    }
    return 1;
}

enum Key_type set_rule(const char *flag){
    switch (flag[0]) {
        case 'c':
            return conway;
            break;
        case 's':
            return seeds;
            break;
        case 'b':
            return brian;
            break;
        case 'h':
            return high;
            break;
        case 'd':
            return daynight;
            break;
        case 'm':
            if (strcmp(flag, "maze") == 0){
                return maze;
            } else {
                return move;
            }
            break;
        default:
            printf("ERROR: invalid rule type: %s\n", flag);
            exit(1);
    }
}

void updatePixel(SDL_Surface* surface, Automata* automata) {

    SDL_LockSurface(surface);
    uint8_t* pixels = (uint8_t*)surface -> pixels;

    for (size_t sr = 0; sr < automata -> field -> rows; ++sr) {
        for (size_t sc = 0; sc < automata -> field -> cols; ++sc) {
            const uint8_t color = automata -> field -> map[sr][sc] * 255 / (automata -> key -> n_states - 1);
            pixels[sr * surface -> pitch + sc * surface -> format -> BytesPerPixel + 0] = color;
            pixels[sr * surface -> pitch + sc * surface -> format -> BytesPerPixel + 1] = color;
            pixels[sr * surface -> pitch + sc * surface -> format -> BytesPerPixel + 2] = color;
        }
    }
    SDL_UnlockSurface(surface);
}

int main(int argc, char **argv){

#if 1
    assert(argv[0] != NULL);

    if (argc >= 9){
        printf("ERROR: not valid arguments, usage: -s <size> -r <rule> -i <iterations> [-a] [-s]\n -s\tsave last generation to .ppm image\n -a\tshow animation of life, hold Q to step\n");
        exit(1);
    }

    uint8_t flagcount = 0;
    const char size_flag[] = "-s";
    const char type_flag[] = "-r";
    const char iter_flag[] = "-i";
    const char anim_flag[] = "-a";
    const char ppm_flag[] = "-p";

    for (unsigned int i = 1; i < argc; ++i){
        if (strcmp(argv[i], size_flag) == 0){
            SIZE = atoi(argv[i + 1]);
            flagcount += 1;
        }
        else if (strcmp(argv[i], type_flag) == 0){
            RULE = set_rule(argv[i + 1]);
            flagcount += 1;
        }
        else if (strcmp(argv[i], iter_flag) == 0){
            ITER = atoi(argv[i + 1]);
            flagcount += 1;
        }
        else if (strcmp(argv[i], anim_flag) == 0){
            SDL = 1;
            flagcount += 1;
        }
        else if (strcmp(argv[i], ppm_flag) == 0){
            PPM = 1;
            flagcount += 1;
        }
    }

    // if (flagcount != 3){
    //    printf("ERROR: not valid arguments, usage: -s <size> -r <rule> -i <iterations> [-a] [-s]\n -s\tsave last generation to .ppm image\n -a\tshow animation of life, hold Q to step\n");
    //    exit(1);
    //}

#endif
    SDL_Window* window;
    SDL_Surface* surface;
     
    if (SDL) {
        SDL_Init(SDL_INIT_VIDEO);
 
        window = SDL_CreateWindow("Automata", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SIZE, SIZE, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            fprintf(stderr, "ERROR: failed to create SDL_WINDOW: %s\n", SDL_GetError());
            return 1;
        }

        surface = SDL_GetWindowSurface(window);
        if (surface == NULL) {
            fprintf(stderr, "ERROR: failed to get SDL_SURFACE: %s\n", SDL_GetError());
            return 1;
        }

        printf("SDL initialized with no errors...\n");
    }
    

    const enum Key_type type = RULE;
    Automata *automata = automata_constructor(SIZE, SIZE, 1, 0.005f, 0, type);
    assert(automata != NULL);

    if (SDL) {
        bool quit = false;
        while (!quit) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quit = true;
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_Q) {
                    // quit = true;
                    generation_step(automata);
                }
            }
            updatePixel(surface, automata);
            SDL_UpdateWindowSurface(window);
        }

        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    
    if (PPM) { save_field_to_ppm("life.ppm", automata); }
    automata_destructor(automata);
    free(automata);
    
    return 0;
}

// TODO: implement random into the noise generation
// TODO: ppm compression so that it takes up less space
// TODO: all errorhandling
// TODO: usage message correction
