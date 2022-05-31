#ifndef POKEMON_H
#define POKEMON_H

typedef struct Movement {
    char name[30];
    char type[30];
    int pp;
    int damage;
    float accuracy;
} Movement;

typedef struct Pokemon{
char type[30];
char name[30];
Movement* moves[4];
} Pokemon;

void loadMovements();
void loadPokemons();

#endif
