#ifndef POKEMON_H
#define POKEMON_H
#include "list.h"
#include "hashmap.h"
#include "item.h"
typedef struct Movement {
    int id;
    char name[30];
    char type[30];
    int pp;
    int damage;
    float accuracy;
} Movement;

void loadMovements(HashMap* movementMap, HashMap* movements);

typedef struct Pokemon{
int ID;
List* type;
char name[30];
int HP;
//Movement* moves[4];
} Pokemon;

void loadPokemons(HashMap *map);
void loadItems(Item* items);
void showPokemon(Pokemon *pokemon);

typedef struct PlayerPokemon {
    Pokemon *ptr;
    Movement *movements[4];
    int pps[4];
    int hp;
    int enabled;
} PlayerPokemon;

void showPlayerPokemon(PlayerPokemon *pokemon);


void randomizeMovements(PlayerPokemon *ppk, HashMap* map);




#endif
