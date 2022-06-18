#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "combat.h"
#include "player.h"
#include "util.h"

// Crea una instancia de Combat
Combat *initCombat(Player *players)
{
    if (strcmp(players[0].name, "") == 0) {

    }
    {

    }

    Combat *this = malloc(sizeof(Combat));
    this->turn = randomNumber(0, 1);

    printf("Empieza el jugador %d", this->turn + 1);

    
    return this;
}

void escapeCombat()
{

}

void openInventory()
{

}

void attackMenu()
{

}

void mainMenuCombat() 
{
    char in = -1;

    while (in != '0') {
        in = getchar();
        getchar(); // \n
                   
        switch (in)
        {
            case 1: // Atacar
                attackMenu();
                break;
            case 2: // Inventario
                openInventory();
                break;
            case 3: // Huir
                escapeCombat();
                break;


        }
                   

    }


}

void startCombat()
{
    mainMenuCombat();
}
