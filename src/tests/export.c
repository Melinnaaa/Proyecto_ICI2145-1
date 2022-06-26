#include <string.h>

#include "serialization.h"
#include "player.h"
#include "pokemon.h"
int main() 
{
    HashMap *movements = createMap(218);
    loadMovements(NULL, movements);
    Pokemon pk1 = {0};
    PlayerPokemon ppk1 = {0};
        ppk1.ptr = &pk1;
        randomizeMovements(&ppk1, movements);
    Movement movement = {0};
    initPokemon(&pk1, 1, "pikashu", "Tipo1,Tipo2", 666);
    initMovement(&movement, 1, "peo", "Tipo1", 3, 125, 1);
    /*
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
            {
                &pk1, // ptr
                {
                    &movement, // Move 1
                    NULL, // Move 2
                    NULL, // ...
                    NULL
                }
            }
        }
    };
    */
    showPlayerPokemon(&ppk1);
}
