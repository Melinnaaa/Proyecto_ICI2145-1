#ifndef POKEMON_H
#define POKEMON_H
 #include "list.h"
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

void loadMovements(HashMap* movementMap);
void loadPokemons();

#endif
