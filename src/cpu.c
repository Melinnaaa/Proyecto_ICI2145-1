#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include "util.h"
#include "player.h"
#include "csvread.h"

void fileToPlayer(Player* dest, char* name, HashMap* pokemon, HashMap* moves)
{
    char directory[60] = "cache/";
    strcat(directory, name);
//  printf("%s\n", directory);
    struct PlayerExport playerImport;
    FILE *file = fopen(directory, "rb");
        size_t size = 0;
        if (file != NULL){
            fseek(file, 0, SEEK_END);
            size = ftell(file);
        }
    #ifdef DEBUG
            printf("Encontrado archivo %s\n", directory);
    #endif

        if (!size)
        {
            printf("Error: El perfil está vacío\n");
            return;
        }

        if (!file)
        {
            printf("Error: el archivo no existe\n");
            return;
        }

        rewind(file);
        fread(&playerImport, sizeof(struct PlayerExport), 1, file);


        printf("Nombre del perfil importado: %s\n", playerImport.name);
        strcpy(dest->name , playerImport.name);

        dest->wins = playerImport.wins;
        dest->losses = playerImport.losses;
        dest->money = playerImport.money;

        List *items = listCreate();

        FILE *items_fd = fopen("items.csv", "rb");


        char linea[1024];
        while( (fgets(linea, 1023, items_fd)) != NULL) {
            Item *item = malloc(sizeof(Item));
            strcpy(item->name, get_csv_field(linea, 0));
            printf("Añadiendo archivo %s\n", item->name );
            item->price = atoi(get_csv_field(linea, 1));
            item->effect = atoi(get_csv_field(linea, 2));
            listPushBack(items, item);
        }

        for (int i = 0; i < 5; i++)
        {
            if (playerImport.items[i].qty > 0)
            {
                printf("NOMBRE IMPORTAR %s\n", playerImport.items[i].name);
                for (Item *item = listFirst(items); item; item = listNext(items))
                {
                    if (strcmp(playerImport.items[i].name, item->name) == 0)
                    {
                        printf("Encontrado item %s\n", item->name);
                        dest->inventory[i].item = item;
                        dest->inventory[i].qty = playerImport.items[i].qty;
                    }

                }
            }
            else 
            {
                dest->inventory[i].item = NULL;
                playerImport.items[i].qty = 0;
            }
        }

        // para cada pokemon
        for (int i = 0; i < 4; i++)
        {
            HashMapPair *pair = searchMap(pokemon, playerImport.poke[i].name);
            if (pair)
            {
                Pokemon *tmp = pair->value;
                dest->pokemons[i].ptr = tmp;
                dest->pokemons[i].consumed = 0;
                dest->pokemons[i].hp = tmp->HP;
                printf("POKEMON: .%s.\n", dest->pokemons[i].ptr->name);
    #ifdef DEBUG
                printf("DEBUG: Encontrado pokemon %s\n", tmp->name);
    #endif
                // para cada movimiento
                for (int j = 0; j < 4; j++)
                {
                    pair = searchMap(moves, playerImport.poke[i].move[j].name);
                    if (!pair) 
                    {
                        printf("Error: movimiento %s no encontrado\n", playerImport.poke[i].move[j].name);
                    } else {
                        dest->pokemons[i].movements[j] = pair->value;
                        printf("MOVIMIENTO: .%s.\n", dest->pokemons[i].movements[j]->name);
                    }
                    dest->pokemons[i].pps[j] = playerImport.poke[i].move[j].pp;
                }

            } else {
                //printf("Error cargando perfil:\n");
                //printf("%s no fue encontrado en la base de datos\n", playerImport.poke[i].name);
            }
        }
        dest->canPlay = 1;
}


Player createCPU (HashMap* pokemon, HashMap* moves)
{
    DIR* dirp;
    Player cpu;
    int numArchives = countArchives();
    int random = randomNumber(0, numArchives-1);
    struct dirent * entry;
    int i = 0;
    dirp = opendir("cache"); 
    while ((entry = readdir(dirp)) != NULL) {
        if (strcmp(entry->d_name,".")==0 ||
                strcmp(entry->d_name,"..")==0 ) continue;
        if (i == random)
        {
            break;
        }

#ifdef DEBUG
        printf("%s\n", entry->d_name);
#endif
        i++;
    }

    //printf("%s\n", entry->d_name);
    fileToPlayer(&cpu,entry->d_name, pokemon, moves);
    closedir(dirp);
    return cpu;
}


void initCpuCombat (Player* players, Player* cpu, HashMap* effective, HashMap* uneffective)
{
    if (!(players->canPlay) || !(cpu->canPlay))
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
    combat.turn.current.ptr = cpu;
    combat.turn.enemy.ptr = players;

    combat.turn.current.selection = cpu->pokemons;
    combat.turn.current.selectionIndex = 0;

    //Guardar los pps
    for (int i = 0 ; i < 4 ; i++)
    {
        for (int k = 0 ; k < 4 ; k++)
        {
            combat.turn.enemy.ptr->pokemons[i].pps[k] = combat.turn.enemy.ptr->pokemons[i].movements[k]->pp;
            combat.turn.current.ptr->pokemons[i].pps[k] = combat.turn.current.ptr->pokemons[i].movements[k]->pp;
        }
    }

    for (int i = 0; i < 4; i++)
        combat.turn.current.consumed[i] = 0;

    // Nadie ha ganado aún.
    combat.winner = NULL;

    // mientras el combate no deba acabar.
    while (!(combat.shouldClose))
    {
        int j;
        if(combat.turn.current.ptr != cpu) 
        {
            showCombatMenu(&combat);
        }
        if(combat.turn.current.ptr == cpu)
        {
            j = 1;
        }
        else j = checkNum(1, 3);
        int k;//Pokemon a atacar.
        int l;//Ataque a realizar.
#ifdef DEBUG
        printf("DEBUG: menú combate j = %d\n", j);
#endif
        switch (j)
        {
            case 0: // Huir
                tryToEscape(&combat);
                break;
            case 1: // Atacar
            {
                Movement *movement;
                PlayerPokemon *pkm;
                if (combat.turn.current.ptr != cpu)
                {
                    printf("%s está al frente.\n", combat.turn.current.selection->ptr->name);
                    printf("Elige tu ataque\n");
                    showAttacks(&combat);
repeat:
                    l = checkNum(1, 4);
                    if (l == 0) continue;
                    movement = (combat.turn.current.selection->movements)[l - 1];
                    if (combat.turn.current.selection->pps[l-1] == 0)
                    {
                        printf("No le quedan pps al ataque. Selecciona otro ataque.\n");
                        goto repeat;
                    }
                    printf("A quién quieres atacar?\n");
                    showAttackable(&combat);
reask:
                    k = checkNum(1,4);
                    if (k == 0) continue;

                    pkm = getEnemyPokemon(&combat, k - 1);

                    if ( ! (pkm->hp) )
                    {
                        printf("%s está muerto. Elige otro objetivo.\n", pkm->ptr->name);
                        goto reask;
                    }
                }
                else
                {
                    printf("Turno de %s:\n\n", combat.turn.current.ptr->name);
                    do
                    {
                        l = randomNumber(0,3);
                        printf("ataque: %d\n", l);
                        movement = combat.turn.current.selection->movements[l];
                    }while (combat.turn.current.selection->pps[l] == 0);

                    do
                    {
                        k = randomNumber(0,3);
                        printf("pokemon: %d\n", k);
                        pkm = combat.turn.enemy.ptr->pokemons + k;
                    } while (pkm->hp == 0);
                }

                printf("Actual: %d", combat.turn.current.selectionIndex);
                printf("%s usó %s!", combat.turn.current.selection->ptr->name,
                        combat.turn.current.selection->movements[l]->name);

                getchar();
                putchar('\n');
                //Se restan los pps del ataque utilizado.
                combat.turn.current.selection->pps[l-1] -=1;
                int flag = 1;
                List *types = pkm->ptr->type;
                float damage = movement->damage*0.25;
                for (char *type = listFirst(types);
                        type;
                        type = listNext(types)) {
                    // Si es efectivo pasamos al siguiente pokémon,
                    if (isEffective(&combat, movement->type, type) && flag)
                    {
                        printf("Es super efectivo!");
                        damage *= 1.5;
                        if (combat.turn.enemy.knocked[k-1] == 1)
                        {
                            combat.turn.current.selection->consumed = 1;
                            combat.turn.current.consumed[combat.turn.current.selectionIndex] = 1;
                        }
                        else
                        {
                            flag = 0;
                            combat.turn.enemy.knocked[k-1] = 1;

                            // deshabilitamos el pokemon actual para atacar.
                            combat.turn.current.selection->consumed = 1;
                            combat.turn.current.consumed[combat.turn.current.selectionIndex] = 1;

                            getchar();
                            putchar('\n');
                            printf("%s está noqueado!", pkm->ptr->name);
                            getchar();
                            putchar('\n');
                        }
                        
                    // si no es efectivo, terminamos el turno.
                    } else if (isUneffective(&combat, movement->type, type) && flag){
                        printf("No hizo nada..");
                        damage = 0;
                        flag = 0;
                        updateTurn(&combat);
                        getchar();
                        putchar('\n');
                    }
                }

                if (damage)
                {
                    printf("%s le quitó %d HP a %s!", combat.turn.current.selection->ptr->name, 
                            (int)damage,
                            pkm->ptr->name);
                    getchar();
                    putchar('\n');
                }



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

                if (canAllOutAttack(&combat) == 1) 
                {
                    int allOut;
                    if(combat.turn.current.ptr != cpu)
                    {
                        printf("Todos los pokemons estan noqueados, es tu oportunidad para realizar un All-out Attack!\n");
                        getchar();
                        printf("1.Si \t 2.No\n\n");
                        allOut = checkNum(1, 2);
                        if (allOut == 1)
                        {     
                            printf("Beat 'em up!\n");
                            getchar();
                            doAllOutAttack(&combat);
                            if (checkEnemy(&combat) == 1);
                            else flag = 1;
                        }
                        else 
                        {
                            if (combat.turn.current.selectionIndex == 3) flag = 1;
                            for (int i = 0 ; i < 4 ; i++)
                                combat.turn.enemy.knocked[i] = 0;
                        }
                    }
                    //La cpu siempre realizará all-out attack
                    else
                    {
                        doAllOutAttack(&combat);
                        if (checkEnemy(&combat) == 1);
                        else flag = 1;
                        for (int i = 0 ; i < 4 ; i++)
                                combat.turn.enemy.knocked[i] = 0;
                    }
                }
                // ataque normal. Terminamos el turno
                if (flag) {
                    updateTurn(&combat);
                    
#ifdef DEBUG
                    printf("DEBUG: Ataque normal\n");
#endif
                } else
                    nextSelection(&combat);
                break;
            }
            case 2:
            {
                checkBag(&combat);
            }
        }
    }

    if (!combat.winner)
    {
        printf("Algo salió mal. (El combate terminó y no hay ganador)\n");
    } else {
        int money = randomNumber(500,1000);
        printf("El ganador es %s!\n", combat.winner->name);
        printf("Felicidades, ganaste $%d!\n", money);
        combat.winner->money += money;
        combat.winner->wins ++;
        combat.turn.enemy.ptr->losses ++;
        getchar();
    }
    for (int j = 0; j < 2; j++)
        for (int i = 0; i < 4; i++)
        {
            players->pokemons[i].hp = players->pokemons[i].ptr->HP;
            players->pokemons[i].consumed = 0;
            for (int k = 0; k < 4; k++)
                players->pokemons[i].pps[k] = players->pokemons[i].movements[k]->pp;
        }
}
