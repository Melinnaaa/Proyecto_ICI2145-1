#ifndef combat_h
#define combat_h
#include "player.h"

typedef struct Combat {
    int turn; // a quien le toca?
    int hp1[4], hp2[4]; // hp de los pokemon
    Player *players;
} Combat;

Combat *initCombat();
void startCombat(Player *players, Combat *combat);
void attackMenu(Combat *combat);
void mainMenuCombat(Combat *combat);
#endif
