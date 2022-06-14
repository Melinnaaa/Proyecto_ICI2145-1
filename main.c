#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "hashmap.h"
#include "menu.h"
#include "pokemon.h"
#include "item.h"
#include "player.h"
#include "util.h"

void showPlayers();

int main() {
    srand(time(NULL));
    printf("Bienvenido a pokemon.exe\n");
    HashMap* movementsStr = createMap(218);
    HashMap* pokemonsStr = createMap(917);
    HashMap* movements = createMap(218);
    Item* items = calloc(5, sizeof(Item*));
    loadPokemons(pokemonsStr);
    loadMovements(movementsStr, movements);
    loadItems(items);
    int j;//Almacena el numero del jugador.
    // Jugadores
    Player players[2];
    for (int i = 0; i < 2; i++) {
        createPlayer(&players[i]);
    }
    int in = -1;
    while (in != 0) {
        showMenu();
        in = checkNum(0, 5);
        
        switch(in)  
        {
            case 1: // Crear perfil
            {
                //Se recibe el numero en donde se almacenaran los datos del jugador
                j = getPlayerPos();
                createProfile(&players[j-1], pokemonsStr, movementsStr);
                break;
            }
            case 2: // Cargar perfiles
            {
                login(players, pokemonsStr, movements, items);
                break;
            }
            case 3: // Mostrar jugadores
            {
                showPlayers(players);
                break;
            }
            case 4:
            {
                sign_in(players);
                break;
            }
            case 5:
            {
                j = getPlayerPos();
                buyItems(items, &players[j-1]);
                break;
            }
        }
    }
    printf("lo vimoh\n");
    
    return 0;
}

/* Se crea el inventario vacio.
 * se carga si es que existe (TODO) */
void createPlayer(Player *p)
{
    for( int i = 0; i < 5; i++)
    {
        p->inventory[i].qty = 0;
        p->inventory[i].item = NULL;
    }
    p->wins = 0;
    p->losses = 0;
    p->money = 0;
    p->pokemons->ptr = NULL;
 
}

//menu
void showPlayers(Player *players)
{   
    for (int i = 0; i < 2; i++)
    {
        if(players[i].pokemons->ptr != NULL)
            showPlayer(&players[i]);
        else
            printf("El jugador %d no se ha registrado.\n\n", i + 1);
    }
}

void showPlayer(Player *p)
{
    printf("Jugador %s\n", p->name);
    printf("Victorias %d\n", p->wins);
    printf("Derrotas %d\n", p->losses);
    printf("Ratio victorias / derrotas %f\n", (p->losses && p->wins)  ?
            ((float)p->wins / p->losses) : 0 );
    // Inventario
    for(int i = 0; i < 5; i++) {
        if(p->inventory[i].item != NULL) {
            printf("a\n");
            showItem(p->inventory[i].item);
        }
    }

    for(int i = 0; i < 4; i++) 
    {
        showPlayerPokemon(&p->pokemons[i]);
    }
}

//TODO
void loadProfiles()
{
    return;
}

//TODO
void showItem(Item *item)
{
    return;
}

