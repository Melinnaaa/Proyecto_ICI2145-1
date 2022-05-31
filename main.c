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
    // Inventario
    PlayerItem inventory[10];
} Player;

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
