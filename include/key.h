#ifndef KEY_H
#define KEY_H

typedef struct Key{
    uint8_t n_states;
    uint8_t **law;
} Key;

uint8_t **alloc_law(const uint8_t n_states){
    uint8_t **law;
    law = malloc(n_states * sizeof(uint8_t *));
    if (law == NULL){ return NULL; }
    for (unsigned int i = 0; i < n_states; ++i) {
        law[i] = malloc(9 * sizeof(uint8_t));
        if (law[i] == NULL) {
            for (unsigned int j = 0; j < i; ++j) {
                free(law[j]);
            }
            free(law);
            return NULL;
        }
    }
    return law;
}

void free_law(uint8_t **law, const uint8_t n_states){
    for (unsigned int i = 0; i < n_states; ++i){
        free(law[i]);
    }
    free(law);
    law = NULL;
}

enum Key_type{
    conway,
    seeds,
    brian,
    high,
    maze,
    daynight,
    move
};

Key *key_constructor(const enum Key_type type){
    Key *key = malloc(sizeof(Key));
    if(key == NULL){
        printf("ERROR: failed to construct key\n");
        return NULL;
    } else {
        switch (type) {
            case conway:
                key -> n_states = 2;
                key -> law = alloc_law(key -> n_states);
                key -> law[0][3] = 1;
                key -> law[1][2] = 1;
                key -> law[1][3] = 1;
                break;

            case seeds:
                key -> n_states = 2;
                key -> law = alloc_law(key -> n_states);
                key -> law[0][2] = 1;
                break;

            case brian:
                key -> n_states = 3;
                key -> law = alloc_law(key -> n_states);
                key -> law[0][2] = 1;
                key -> law[1][0] = 2;
                key -> law[1][1] = 2;
                key -> law[1][2] = 2;
                key -> law[1][3] = 2;
                key -> law[1][4] = 2;
                key -> law[1][5] = 2;
                key -> law[1][6] = 2;
                key -> law[1][7] = 2;
                key -> law[1][8] = 2;
                break;

            case high:
                key -> n_states = 2;
                key -> law = alloc_law(key -> n_states);
                key -> law[0][3] = 1;
                key -> law[0][6] = 1;
                key -> law[1][2] = 1;
                key -> law[1][3] = 1;
                break;

            case maze:
                key -> n_states = 2;
                key -> law = alloc_law(key -> n_states);
                key -> law[0][3] = 1;
                key -> law[1][1] = 1;
                key -> law[1][2] = 1;
                key -> law[1][3] = 1;
                key -> law[1][4] = 1;
                key -> law[1][5] = 1;
                break;

            case daynight:
                key -> n_states = 2;
                key -> law = alloc_law(key -> n_states);
                key -> law[0][3] = 1;
                key -> law[0][6] = 1;
                key -> law[0][7] = 1;
                key -> law[0][8] = 1;
                key -> law[1][3] = 1;
                key -> law[1][4] = 1;
                key -> law[1][6] = 1;
                key -> law[1][7] = 1;
                key -> law[1][8] = 1;
                break;

            case move:
                key -> n_states = 2;
                key -> law = alloc_law(key -> n_states);
                key -> law[0][3] = 1;
                key -> law[0][6] = 1;
                key -> law[0][8] = 1;
                key -> law[1][2] = 1;
                key -> law[1][4] = 1;
                key -> law[1][5] = 1;
                break;

            default:
                printf("ERROR: invalid key type\n");
                return NULL;
        }
    }
    return key;
}

void key_destructor(Key *key){
    free_law(key -> law, key -> n_states);
    key = NULL;
}

#endif
