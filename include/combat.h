#ifndef combat_h
#define combat_h
#include "hashmap.h"

#include "player.h"

struct Combat {
    struct {
        struct {
            Player *ptr;
            int consumed[4]; // Atacó el pokémon?
            PlayerPokemon *selection;
            int selectionIndex;
        } current;

        struct {
            Player *ptr;
            int knocked[4]; // Esta aturdido el enemigo?
        } enemy;

    } turn;

    struct {
        HashMap *effective,
                *uneffective;
    } maps;

    Player *winner;
    int shouldClose;
};

void
initCombat(
        Player *players,
        HashMap *effective, 
        HashMap *uneffective
    );
#endif

void showCombatMenu(struct Combat *combat);
int shouldCloseCombat(struct Combat *combat);
void tryToEscape(struct Combat *combat);
void showAttackable(struct Combat *combat);
void showAttacks(struct Combat *combat);
PlayerPokemon *getEnemyPokemon(struct Combat *combat, int num);
int searchAbilities (HashMap* map, char* attackType, char* typeEnemy);
int isEffective(struct Combat *combat, char *moveType, char *pkmType);
int isUneffective(struct Combat *combat, char *moveType, char *pkmType);
int checkEnemy(struct Combat* combat);
int canAllOutAttack(struct Combat *combat);
void doAllOutAttack(struct Combat *combat);
void nextSelection(struct Combat *combat);
void updateTurn(struct Combat *combat);
void changePokemon(struct Combat* combat);
int showBag (struct Combat* combat);
int getPkmPos(struct Combat* combat);
void checkBag(struct Combat *combat);