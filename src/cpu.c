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
    printf("%s\n", directory);
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
            if (playerImport.items[i].qty != 0)
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
            }
        }

        /*
         *
        // para cada item
        for (int i = 0; i < 4; i++)
        {
            
        }
        */
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
    #ifdef DEBUG
                printf("DEBUG: Encontrado pokemon %s\n", tmp->name);
    #endif
                // para cada movimiento
                for (int j = 0; j < 4; j++)
                {
                    printf("MOVIMIENTO: .%s.\n", playerImport.poke[i].move[j].name);
                    pair = searchMap(moves, playerImport.poke[i].move[j].name);
                    if (!pair) 
                    {
                        printf("Error: movimiento %s no encontrado\n", playerImport.poke[i].move[j].name);
                    } else {
                        dest->pokemons[i].movements[j] = pair->value;
                    }
                    dest->pokemons[i].pps[j] = playerImport.poke[i].move[j].pp;
                }

            } else {
                printf("Error cargando perfil:\n");
                printf("%s no fue encontrado en la base de datos\n", playerImport.poke[i].name);
            }
        }
        dest->canPlay = 1;
}


Cpu createCPU (HashMap* pokemon, HashMap* moves)
{
    DIR* dirp;
    Cpu tmp;
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

    printf("%s\n", entry->d_name);
    fileToPlayer(&tmp.player,entry->d_name, pokemon, moves);
    closedir(dirp);

    return tmp;
}
