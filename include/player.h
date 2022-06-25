#ifndef PLAYER_H
#define PLAYER_H
#include "item.h"
#include "pokemon.h"

typedef struct {
    char name[30];//Nombre.
    int wins;//Victorias.
    int losses;//Derrotas.
    int money;//Dinero disponible.
    int canPlay; // Puedo pelear?              
    // Inventario
    PlayerItem inventory[5];
    PlayerPokemon pokemons[4];//Pokemons del jugador.
} Player;

struct PlayerExport {
    char name[30];
    int wins;
    int losses;
    int money;
    struct poke {
        char name[30];
        struct move {
            char name[15];
            int pp;
        } move[4];
    } poke[4];
};

void createPlayer(Player *player);
void showPlayer(Player *player);

int getPlayerPos();

#endif
