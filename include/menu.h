#ifndef MENU_H
#define MENU_H
#include "hashmap.h"
#include "player.h"
void showMenu();
void createProfile(Player *player, HashMap *pokeMap, HashMap *movements);
void sign_in (Player* players);
void login (Player* players, HashMap* pkm, HashMap* moveMap, Item* items);
void showShop(Item* items);
void buyItems(Item* items, Player* players);
void getEffectiveNodes(HashMap* effective);
void getUneffectiveNodes(HashMap* uneffectiveMap);
int searchAbilities (HashMap* map, char* attackType, char* typeEnemy);
#endif
