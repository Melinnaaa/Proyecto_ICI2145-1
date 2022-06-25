#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "util.h"
#include "player.h"

void fileToPlayer(Player* dest, char* name, HashMap* pokemon, HashMap* moves)
{
    char directory[60] = "cache/";
    strcat(directory, name);
    printf("%s\n", directory);
    struct PlayerExport playerImport;
    FILE *file = fopen(directory, "rb");
        size_t size;
        if (file != NULL){
            fseek(file, 0, SEEK_END);
            size = ftell(file);
    #ifdef DEBUG
            printf("Encontrado archivo %s\n", dir);
    #endif
        }

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
    int random = randomNumber(0, numArchives);
    struct dirent * entry;
    dirp = opendir("cache"); 
    for(int i = 0 ; i < random ; i++)
    {
        entry = readdir(dirp);
    }
    printf("%s\n", entry->d_name);
    fileToPlayer(&tmp.player,entry->d_name, pokemon, moves);
    closedir(dirp);

    return tmp;
}
