#include <stdio.h>
#include <string.h>

#include "menu.h"

typedef struct {

} Pokemon;

typedef struct {
    int ID;
    char name[30];
    int price;
} Item;

typedef struct {
    Item *item;
    int qty;
} PlayerItem;

void showItem(Item *item);



typedef struct {
    char name[30];
    int wins, losses;
    // Inventario
    PlayerItem inventory[10];
} Player;

void createPlayer(Player *player);
void showPlayer(Player *player);

typedef struct {

} HashMap;

struct Movements {
    FILE *csv;
    HashMap *data;
} movements;


void loadMovements();
void loadProfiles();
void showPlayers();


int main() {
    printf("Bienvenido a pokemon.exe\n");

    // Jugadores
    Player players[2];
    for (int i = 0; i < 2; i++) {
        createPlayer(&players[i]);
    }
    
    int in = -1;
    loadMovements();
    while (in != 0) {
        showMenu();
        scanf("%d", &in);
        switch(in)  
        {
            case 1: // Crear perfil
            {
                createProfile();
            }
            case 2: // Cargar perfiles
            {
                loadProfiles();
                break;
            }
            case 3: // Mostrar jugadores
            {
                showPlayers();
                break;
            }
        }
    }
    printf("Shao\n");
    
    return 0;
}

/* Se crea el inventario vacio.
 * se carga si es que existe (TODO) */
void createPlayer(Player *p)
{
    for( int i = 0; i < 10; i++)
    {
        p->inventory[i].qty = 0;
        p->inventory[i].item = NULL;
    }
}

//menu
void showPlayers(Player *players)
{   
    for (int i = 0; i < 2; i++)
    {
        showPlayer(&players[i]);
    }
}

void showPlayer(Player *p)
{
    printf("Jugador %s\n", p->name);
    printf("Victorias %d\n", p->wins);
    printf("Derrotas %d\n", p->losses);
    // Inventario
    for(int i = 0; i < 10; i++) {
        if(p->inventory[i].item != NULL) {
            showItem(p->inventory[i].item);
        }
    }
}
