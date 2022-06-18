#ifndef PLAYER_H
#define PLAYER_H
#include "item.h"
#include "pokemon.h"

typedef struct {
    char name[30];//Nombre.
    int wins;//Victorias.
    int losses;//Derrotas.
    int money;//Dinero disponible.
    // Inventario
    PlayerItem inventory[5];
    PlayerPokemon pokemons[4];//Pokemons del jugador.
    int canFight;                              
    PlayerPokemon *selection;
} Player;

void createPlayer(Player *player);
void showPlayer(Player *player);

int getPlayerPos();

#endif
