#include <stdio.h>

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

int main() {
    loadMovements();
    
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
