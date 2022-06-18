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
    this->players = players;
    printf("Empieza el jugador %d. (%s)\n", this->turn + 1, players[this->turn].name);
    // hp1 hp2
    while (((players[0].pokemons[0].hp + 
            players[0].pokemons[1].hp + 
            players[0].pokemons[2].hp + 
            players[0].pokemons[3].hp) != 0 ) && 
            ((players[0].pokemons[0].hp + 
            players[0].pokemons[1].hp + 
            players[0].pokemons[2].hp + 
            players[0].pokemons[3].hp) != 0 )
        ){
        mainMenuCombat(this);
        

    }

    return this;
}

void escapeCombat()
{

}

void openInventory()
{

}

void doAttack(Combat *combat, int attackMov, int pokemon)
{
    Player *current = combat->players + combat->turn;
    Player *enemy = combat->players + !(combat->turn);
    attackMov --; pokemon --;

    printf("%s usó %s\n", current->selection->ptr->name, current->selection->movements[attackMov]->name);

    enemy->pokemons[pokemon].hp -= current->selection->movements[attackMov]->damage * 0.25;

    printf("La vida del %s enemigo bajó %f HP\n", 
            enemy->pokemons[pokemon].ptr->name,
            current->selection->movements[attackMov]->damage * 0.25);

    current->selection->enabled = 0;
}



void attackMenu(Combat *combat)
{
    int in = -1, in2 = -1;
    Player *current = combat->players + combat->turn;
    Player *enemy = combat->players + !(combat->turn);

    printf("Tu pokémon seleccionado es %s:", current->selection->ptr->name);
    printf(" %d HP\n\n", current->selection->hp);
        

    printf("Elige tu ataque: \n");

    for(int i = 0; i < 4; i ++)
    {
        printf("%d. %s\n", i + 1, current->selection->movements[i]->name);
    }
reask:
    in = checkNum(0, 4);

    if (in == 0) return;

    printf("A quién quieres atacar?\n");

    for (int i = 0; i < 4; i++)
    {
        printf("%d. %s. HP: %d\n", i+1, enemy->pokemons[i].ptr->name, 
                enemy->pokemons[i].hp);
    }

    

    in2 = checkNum(0, 4);
    if (in2 == 0) goto reask;
    // para atacar necesitamos: numero ataque.
 doAttack(combat, in, in2);
}

void showMainMenuCombat()
{
    printf("Qué quieres hacer?\n\n");
    printf(
        "1. Atacar\n"
        "2. Inventario\n"
        "3. Huir\n"
            );
}
void mainMenuCombat(Combat *combat)
{
    char in = -1;
    Player *current = &combat->players[combat->turn];


    while (in != '0') {
        printf("Turno de %s:\n\n", current->name);

        for(int i = 0; i < 4; i++)
        {
            printf(
                    current->selection == (current->pokemons + i) ? 
                    "\u00b7 %s: %d HP\n":"  %s: %d HP\n",
                    current->pokemons[i].ptr->name,
                    current->pokemons[i].hp);
        }

        showMainMenuCombat();

        fflush(stdin);
        in = getchar() - '1' + 1;
        getchar(); // \n
                   
        switch (in)
        {
            case 1: // Atacar
                attackMenu(combat);
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
#ifdef DEBUG
        printf("DEBUG: p1p: %d\n p2p: %d\n", players[0].canFight, players[1].canFight);
#endif
        printf("Para iniciar un combate es necesario cargar los perfiles.\n");
        printf("Pulsa una tecla para continuar..");
        getchar();
        return;
    }
    mainMenuCombat(combat);
    
}
