#include <stdio.h>

typedef struct {

} Pokemon;

typedef struct {
    int ID;
     
} Item;

typedef struct {
    Item *inventory;
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
