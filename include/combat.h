#ifndef combat_h
#define combat_h

typedef struct Combat {
    int turn; // a quien le toca?
    int hp1[4], hp2[4]; // hp de los pokemon
} Combat;

Combat *initCombat();
void startCombat();
void attackMenu();
void mainMenuCombat();
#endif
