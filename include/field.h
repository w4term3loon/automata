#ifndef FIELD_H
#define FIELD_H

typedef struct Field{
    uint16_t rows;
    uint16_t cols;
    uint8_t **map;
    float **noise;
} Field;

uint8_t **alloc_map(const uint16_t rows, const uint16_t cols){
    uint8_t **map;
    map = malloc(rows * sizeof(uint8_t *));
    if (map == NULL){ return NULL; }
    for (unsigned int i = 0; i < rows; ++i){
        map[i] = malloc(cols * sizeof(uint8_t));
        if (map[i] == NULL) {
            for (unsigned int j = 0; j < i; ++j) {
                free(map[j]);
            }
            free(map);
            return NULL;
        }
    }
    return map;
}

float **alloc_noise(const uint16_t rows, const uint16_t cols){
    float **noise;
    noise = malloc(rows * sizeof(float *));
    if (noise == NULL){ return NULL; }
    for (unsigned int i = 0; i < rows; ++i){
        noise[i] = malloc(cols * sizeof(float));
        if (noise[i] == NULL) {
            for (unsigned int j = 0; j < i; ++j) {
                free(noise[j]);
            }
            free(noise);
            return NULL;
        }
    }
    return noise;
}

void free_map(uint8_t **map, uint16_t rows){
    for (unsigned int i = 0; i < rows; ++i){
        free(map[i]);
    }
    free(map);
    map = NULL;
}

void free_noise(float **noise, uint16_t rows){
    for (unsigned int i = 0; i < rows; ++i){
        free(noise[i]);
    }
    free(noise);
    noise = NULL;
}

void noise(Field *field, const float frequency, const uint8_t octaves){
    // Create and configure noise state
    fnl_state noise = fnlCreateState();
    noise.frequency = 4.0f / (float)field -> rows + frequency;
    noise.octaves = 2 + octaves;
    noise.noise_type = FNL_NOISE_OPENSIMPLEX2;

    // Gather noise data
    for (unsigned int i = 0; i < field -> rows; ++i){
        for (unsigned int j = 0; j < field -> cols; ++j){
            field -> noise[i][j] = fnlGetNoise2D(&noise, i, j);
        }
    }
}

//Field *field = construct_field(SIZE, SIZE);
Field *field_constructor(const uint16_t rows, const uint16_t cols){
    Field *field = malloc(sizeof(Field));
    if (field == NULL){
        printf("ERROR: failed to create field\n");
        return NULL;
    } else {
        field -> rows = rows;
        field -> cols = cols;
        field -> noise = alloc_noise(rows, cols);
        field -> map = alloc_map(rows, cols);
    }
    return field;
}

void field_destructor(Field *field){
    const uint16_t rows = field -> rows;
    free_map(field -> map, rows);
    free_noise(field -> noise, rows);
    field = NULL;
}

#endif
