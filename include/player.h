#ifndef PLAYER_H
#define PLAYER_H
#include "item.h"
#include "pokemon.h"

typedef struct {
    char name[30];
    int wins;
    int losses;
    // Inventario
    PlayerItem inventory[10];
    PlayerPokemon pokemons[4];
} Player;

void createPlayer(Player *player);
void showPlayer(Player *player);

#endif
