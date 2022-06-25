#ifndef cpu_h
#define cpu_h
#include "player.h"
#include "combat.h"
#include "hashmap.h"

struct CpuCombat {
    struct Combat;
    Player* player;
};

void fileToPlayer(Player* dest, char* name, HashMap* pokemon, HashMap* moves);
Player createCPU (HashMap* pokemon, HashMap* moves);
void initCpuCombat (Player* players, Player* cpu, HashMap* effective, HashMap* uneffective);


#endif