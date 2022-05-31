#ifndef ITEM_H 
#define ITEM_H 

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

#endif
