#include <assert.h>
#include "player.h"

int main() {
    Player player[2];
    createPlayer(&player[0]);
    createPlayer(&player[1]);
    assert(player[0].canFight = 0);
    for (int i = 0; i < 5; i++)
    {
        assert (player[0].inventory[i].item == NULL);
        assert (player[0].inventory[i].qty == 0);
    }

    assert (player[0].losses == 0);
    assert (player[0].wins == 0);
    assert (player[0].money == 2000);

    for (int i = 0; i < 4; i++) 
    {
        assert (player[0].pokemons[i].ptr == NULL);
    }
    
}
