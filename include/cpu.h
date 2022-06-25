#ifndef cpu_h
#define cpu_h
#include "player.h"

typedef struct
{
    Player player;
    HashMap* effective;
    HashMap* uneffective;
}Cpu;

void fileToPlayer(Player* dest, char* name, HashMap* pokemon, HashMap* moves);
Cpu createCPU (HashMap* pokemon, HashMap* moves);

#endif