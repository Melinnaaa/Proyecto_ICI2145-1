#include <string.h>

#include "serialization.h"
#include "player.h"
#include "pokemon.h"

void newRandomizeMovements(PlayerPokemon *ppk, HashMap *map);

int main() 
{
    HashMap *movements = createMap(218);
    loadMovements(NULL, movements);
    Pokemon pk1 = {0};
    PlayerPokemon ppk1 = {0};
        ppk1.ptr = &pk1;
        initPokemon(&pk1, 1, "pikashu", "Electrico,Hada", 666);
        ppk1.movements[0] = searchMap(movements, "Pound")->value;
        ppk1.movements[1] = searchMap(movements, "Karate Chop")->value;
        ppk1.movements[2] = searchMap(movements, "Comet Punch")->value;
        ppk1.movements[3] = searchMap(movements, "Pay Day")->value;
    Movement movement = {0};
    initMovement(&movement, 1, "peo", "Tipo1", 3, 125, 1);
    Player player = {
        "Pedro", // name
        3, // wins
        0, // losses
        1500, // money
        0, // canplay
        {
            {NULL, 0}, // item 1
            {NULL, 0}, // item 2
            {NULL, 0}, // ...
            {NULL, 0},
            {NULL, 0}, 
        },
        
        {
            ppk1, ppk1, ppk1, ppk1
        }
    };

    exportPlayer(&player);

    showPlayer(&player);
}
