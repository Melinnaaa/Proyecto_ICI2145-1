#include <string.h>
#include <stdio.h>

#include "combat.h"

#include "util.h"

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


void showCombatMenu(struct Combat *combat)
{
    printf("Turno de %s\n", combat->turn.current.ptr->name);

    //Se muestran los pokemons del jugador.
    for(int i = 0; i < 4; i++)
    {
        printf(
                (combat->turn.current.selection == combat->turn.current.ptr->pokemons + i) ?
                "\u00b7 %s: %d HP\n":"  %s: %d HP\n",
                combat->turn.current.ptr->pokemons[i].ptr->name,
                combat->turn.current.ptr->pokemons[i].hp);
    }

    printf("Qué quieres hacer?\n\n");
    printf("1. Atacar\t");
    printf("3. Sacar al frente\n");
    printf("2. Mochila\t");
    printf("0. Huír\n");
}

int shouldCloseCombat(struct Combat *combat)
{
    if (combat->shouldClose) return 1;

    if (combat->turn.current.consumed[0] && 
        combat->turn.current.consumed[1] && 
        combat->turn.current.consumed[2] && 
        combat->turn.current.consumed[2]) 

        return 1;

    return 0;
}

void tryToEscape(struct Combat *combat)
{
    printf("%s huyó!\n", combat->turn.current.ptr->name);
    combat->shouldClose = 1;
    combat->winner = combat->turn.enemy.ptr;
}

void showAttackable(struct Combat *combat)
{
    for (int i = 0; i < 4; i++)
    {
        printf("%d. %s: ", i + 1, combat->turn.enemy.ptr->pokemons[i].ptr->name);
        printf(
                combat->turn.enemy.ptr->pokemons[i].hp ? 
                "%d HP\n" : "muerto\n", 
                combat->turn.enemy.ptr->pokemons[i].hp);
    }
    printf("0. Volver\n");
}

void showAttacks(struct Combat *combat)
{
    for (int i = 0; i < 4; i++)
    {
        printf("%d. %s: %d PP\n", i + 1, combat->turn.current.selection->movements[i]->name, 
                combat->turn.current.selection->movements[i]->pp);
    }
    printf("0. Volver\n");
}

PlayerPokemon *getEnemyPokemon(struct Combat *combat, int num)
{
    return combat->turn.enemy.ptr->pokemons + num;
}


int searchAbilities (HashMap* map, char* attackType, char* typeEnemy)
{
    List* tmp;
    char *tmpType;
    if (searchMap(map, attackType) != NULL)
    {
#ifdef DEBUG
        printf("tipo ataque: %s\n", attackType);
        printf("tipo Enemigo: %s\n", typeEnemy);
#endif
        tmp = searchMap(map, attackType)->value;
        tmpType = listFirst(tmp);
        while (1)
        {
#ifdef DEBUG
            printf("TIPO: %s\n", tmpType);
#endif
            if (strcmp(tmpType, typeEnemy) == 0) return 1;//Es efectivo.
                                                
            tmpType = listNext(tmp);

            if (tmpType == NULL) { 
                break;
            } 
        }
    }
#ifdef DEBUG
    printf("no encontró nada\n");
#endif
    return 0; //No es efectivo.
}

int isEffective(struct Combat *combat, char *moveType, char *pkmType)
{
    return searchAbilities(combat->maps.effective, moveType, pkmType);
}

int isUneffective(struct Combat *combat, char *moveType, char *pkmType)
{
    return searchAbilities(combat->maps.uneffective, moveType, pkmType);
}

PlayerPokemon *nextSelection(struct Combat *combat)
{
    int i = 0;
    while (1){
        if (i <= 3) i++;
        if (!(combat->turn.current.consumed[i]) || i >= 4) break;
    }
    if (i >= 4){
        Player *auxPlayer = combat->turn.current.ptr;
        combat->turn.current.ptr = combat->turn.enemy.ptr;
        combat->turn.enemy.ptr = auxPlayer;
        combat->turn.current.selectionIndex = 0;
        for (int i = 0; i < 4; i++) combat->turn.current.consumed[i] = 0;
        return combat->turn.current.ptr->pokemons;
    }
    combat->turn.current.selectionIndex = i;
    return combat->turn.current.ptr->pokemons + i;
}

void initCombat(Player *players, HashMap *effective, HashMap *uneffective)
{
    if (!(players[0].canPlay) || !(players[1].canPlay))
    {
        printf("Para iniciar un combate es necesario cargar los perfiles.\n");
        printf("Pulsa una tecla para continuar..");
        getchar();
        return;
    }


    struct Combat combat;

    // Guardar los punteros de los mapas.
    combat.maps.effective = effective;
    combat.maps.uneffective = uneffective;

    combat.shouldClose = 0;

    // Determinar el primer turno.
    int who = randomNumber(0, 1);
    combat.turn.current.ptr = players + who;
    combat.turn.enemy.ptr = players + !(who);

    combat.turn.current.selection = players[who].pokemons;
    combat.turn.current.selectionIndex = 0;


    for (int i = 0; i < 4; i++)
        combat.turn.current.consumed[i] = 0;


    // Nadie ha ganado aún.
    combat.winner = NULL;

    // mientras el combate no deba acabar.
    while (!(combat.shouldClose))
    {
        showCombatMenu(&combat);
        int j = checkNum(1, 3);
        int k, l;
#ifdef DEBUG
        printf("DEBUG: menú combate j = %d\n", j);
#endif
        switch (j)
        {
            case 0: // Huir
                tryToEscape(&combat);
                break;
            case 1: // Atacar
                printf("%s está al frente.\n", combat.turn.current.selection->ptr->name);
                printf("Elige tu ataque\n");
                showAttacks(&combat);
                l = checkNum(1, 4);
                if (l == 0) continue;
                Movement *movement = (combat.turn.current.selection->movements)[l - 1];
                printf("A quién quieres atacar?\n");
                showAttackable(&combat);
reask:
                k = checkNum(1,4);
                if (k == 0) continue;

                PlayerPokemon *pkm = getEnemyPokemon(&combat, k - 1);

                if ( ! (pkm->hp) )
                {
                    printf("%s está muerto. Elige otro objetivo.\n", pkm->ptr->name);
                    goto reask;
                }
                printf("%s usó %s!", combat.turn.current.selection->ptr->name,
                        movement->name);

                getchar();
                putchar('\n');

                int flag = 1;
                List *types = pkm->ptr->type;
                float damage = movement->damage*0.25;
                for (char *type = listFirst(types);
                        type;
                        type = listNext(types)) {
                    if (isEffective(&combat, movement->type, type) && flag)
                    {
                        printf("Es super efectivo!");
                        damage *= 1.5;
                        flag = 0;
                        getchar();
                        putchar('\n');
                    } else if (isUneffective(&combat, movement->type, type) && flag){
                        printf("No hizo nada..");
                        damage = 0;
                        flag = 0;
                        getchar();
                        putchar('\n');
                    }
                }
#ifdef DEBUG
                if (flag)
                    printf("DEBUG: Ataque normal\n");
#endif
                printf("%s le quitó %d HP a %s!", combat.turn.current.selection->ptr->name, 
                        (int)damage,
                        pkm->ptr->name);
                getchar();
                putchar('\n');

                if ((pkm->hp - damage) <= 0)
                {
                    printf("%s murió.", pkm->ptr->name);
                    getchar();
                    putchar('\n');
                    pkm->hp = 0;
                } else {
                    pkm->hp -= damage;
                    printf("%s ahora tiene %d HP", pkm->ptr->name, pkm->hp);
                    getchar();
                    putchar('\n');

                }

                combat.turn.current.selection->consumed = 1;
                combat.turn.current.consumed[combat.turn.current.selectionIndex] = 1;
                combat.turn.current.selection = nextSelection(&combat);
                break;
        }
    }

    if (!combat.winner)
    {
        printf("Algo salió mal. (El combate terminó y no hay ganador)\n");
    } else {
        printf("El ganador es %s!\n", combat.winner->name);
    }

}
