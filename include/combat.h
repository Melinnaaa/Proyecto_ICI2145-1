#ifndef combat_h
#define combat_h
#include "hashmap.h"

#include "player.h"

void
initCombat(
        Player *players,
        HashMap *effective, 
        HashMap *uneffective
    );
#endif
