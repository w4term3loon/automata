#ifndef AUTOMATA_H
#define AUTOMATA_H

typedef struct Automata{
    Key *key;
    Field *field;
} Automata;

void swap_map(Field *field, uint8_t **buffer){
    for (unsigned int i = 0; i < field -> rows; ++i){
        for (unsigned int j = 0; j < field -> cols; ++j){
            field -> map[i][j] = buffer[i][j];
        }
    }
}

uint8_t count_neighbours_by_state(const Field *field, const uint16_t x, const uint16_t y, const uint8_t state){
    uint8_t count = 0;
    for (unsigned int i = (x - 1); i <= (x + 1); ++i){
        for (unsigned int j = (y - 1); j <= (y + 1); ++j){
            if (i == x && j == y){
                continue;
            }
            const uint16_t wrapped_i = (i + field -> rows) % field -> rows;
            const uint16_t wrapped_j = (j + field -> cols) % field -> cols;

            if (field -> map[wrapped_i][wrapped_j] == state){
                ++count;
            }
        }
    }
    return count;
}

bool generation_step(Automata *automata){
    const uint16_t rows = automata -> field -> rows;
    const uint16_t cols = automata -> field -> cols;

    uint8_t **buffer_map = alloc_map(rows, cols);

    for (unsigned int i = 0; i < rows; ++i){
        for (unsigned int j = 0; j < cols; ++j){
            const uint8_t state = automata -> field -> map[i][j];
            const uint8_t neighbours = count_neighbours_by_state(automata -> field, i, j, 1);
            buffer_map[i][j] = automata -> key -> law[state][neighbours];
        }
    }
    swap_map(automata -> field, buffer_map);
    free_map(buffer_map, rows);

    if (automata -> field -> map == NULL){
        return false;
    }
    return true;
}

void scale_noise_to_states(Automata *automata){
    for (unsigned int i = 0; i < automata -> field -> rows; ++i){
        for (unsigned int j = 0; j < automata -> field -> cols; ++j){
            automata -> field -> map[i][j] = floor((automata -> field -> noise[i][j] + 1.0f) / 2.0f * automata -> key -> n_states);
        }
    }
}

Automata *automata_constructor(const uint16_t rows, const uint16_t cols, const bool is_noisy, const float frequency, const uint8_t octaves, const enum Key_type type){
    Automata *automata = malloc(sizeof(Automata));
    if (automata == NULL){
        printf("ERROR: failed to create automata\n");
        return NULL;
    } else {
        automata -> field = field_constructor(rows, cols);
        automata -> key = key_constructor(type);

        if (is_noisy){
            noise(automata -> field, frequency, octaves);
            scale_noise_to_states(automata);
        }
    }
    return automata;
}

void automata_destructor(Automata *automata){
    field_destructor(automata -> field);
    key_destructor(automata -> key);
    automata = NULL;
}

#endif
