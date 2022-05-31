#ifndef PLAYER_H
#define PLAYER_H
#include "item.h"

typedef struct {
    char name[30];
    int wins, losses;
    // Inventario
    PlayerItem inventory[10];
} Player;

void createPlayer(Player *player);
void showPlayer(Player *player);

#endif
