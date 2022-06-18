#include <assert.h>
#include "player.h"

int main() {
    // Crear player solo.
    Player player;
    createPlayer(&player);
    player.canFight = 0;
    assert(player.canFight == 0);
    for (int i = 0; i < 5; i++)
    {
        assert (player.inventory[i].item == NULL);
        assert (player.inventory[i].qty == 0);
    }

    assert (player.losses == 0);
    assert (player.wins == 0);
    assert (player.money == 0);

    for (int i = 0; i < 4; i++) 
    {
        assert (player.pokemons[i].ptr == NULL);
    }

    // Crear player solo.
    Player players[2];
    for (int i = 0; i < 2; i++)
    {
        createPlayer(&players[i]);
        players[i].canFight = 0;
        assert(players[i].canFight == 0);
        for (int j = 0; j < 5; j++)
        {
            assert (players[i].inventory[j].item == NULL);
            assert (players[i].inventory[j].qty == 0);
        }

        assert (players[i].losses == 0);
        assert (players[i].wins == 0);
        assert (players[i].money == 0);

        for (int j = 0; j < 4; j++) 
        {
            assert (players[i].pokemons[j].ptr == NULL);
        }

        }
    
}
