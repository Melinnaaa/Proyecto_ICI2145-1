#ifndef PLAYER_H
#define PLAYER_H
#include "item.h"
#include "pokemon.h"

typedef struct {
    char name[30];//Nombre.
    int wins;//Victorias.
    int losses;//Derrotas.
    // Inventario
    PlayerItem inventory[10];
    PlayerPokemon pokemons[4];//Pokemons del jugador.
} Player;

void createPlayer(Player *player);
void showPlayer(Player *player);

#endif
