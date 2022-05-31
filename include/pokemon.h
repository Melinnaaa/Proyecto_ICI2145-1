#ifndef POKEMON_H
#define POKEMON_H
#include "list.h"
#include "hashmap.h"
typedef struct Movement {
    char name[30];
    char type[30];
    int pp;
    int damage;
    float accuracy;
} Movement;

typedef struct Pokemon{
int ID;
List* type;
char name[30];
int HP;
//Movement* moves[4];
} Pokemon;

typedef struct PlayerPokemon {
    Pokemon *ptr;
    Movement *movements[4];
    int pps[4];
} PlayerPokemon;


void loadMovements(HashMap* movementMap);
void loadPokemons(HashMap *map);


#endif
