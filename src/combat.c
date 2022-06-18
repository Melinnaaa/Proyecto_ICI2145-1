#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#include "combat.h"
#include "player.h"
#include "util.h"

// Crea una instancia de Combat
Combat *initCombat(Player *players)
{
    Combat *this = NULL;
#ifdef DEBUG
    printf("DEBUG: p1p: %d\n p2p: %d\n", players[0].canFight, players[1].canFight);
#endif

    if (!(players[0].canFight && 
            players[1].canFight ))
    {
        return this;
    }

    this = malloc(sizeof(Combat));
    this->turn = randomNumber(0, 1);
    printf("Empieza el jugador %d.\n", this->turn + 1);

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

void mainMenuCombat(Player *players) 
{
    char in = -1;

    while (in != '0') {

        printf("%s vs %s\n", players[0].name, players[1].name);

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

void startCombat(Player * players, Combat *combat)
{
    if (!combat)
    {
        printf("DEBUG: p1p: %d\n p2p: %d\n", players[0].canFight, players[1].canFight);
        printf("Para iniciar un combate es necesario cargar los perfiles.\n");
        printf("Pulsa una tecla para continuar..");
        getchar();
        return;
    }
    mainMenuCombat(&players[0]);
    
}
