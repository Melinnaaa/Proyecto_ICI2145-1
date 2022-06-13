#ifndef ITEM_H 
#define ITEM_H 

typedef struct {
    char name[30];
    int price;
    int effect;
} Item;

typedef struct {
    Item *item;
    int qty;
} PlayerItem;

void showItem(Item *item);

#endif
