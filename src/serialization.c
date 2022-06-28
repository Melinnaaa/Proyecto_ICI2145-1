#include <stdio.h>
#include <string.h>

#include "player.h"
#include "serialization.h"
#include "util.h"

//Exporta un perfil a un archivo binario, es utilizado para las cpus.
void exportPlayer(Player *player)
{
    //Se verifica que el usuario tenga datos para guardar.
    if (player->pokemons->ptr == NULL || player == NULL || strcmp(player->name, "") == 0)
    {
        printf("No tienes datos para guardar, por favor registrate.\n");
        return;
    }

    char dir[30] = "cache/";

    // Obtener hash del nombre del jugador.
    unsigned long namehash = djb2hash((unsigned char *)player->name);

    char hash_str[10];
    sprintf(hash_str, "%lx", namehash & 0xFFFFFFUL);

    strcat(dir, hash_str);
    strcat(dir, ".pkdb");

    FILE *file = fopen(dir, "wb");

    struct PlayerExport playerExport;
    //Se guarda el nombre.
    strcpy(playerExport.name, player->name);
    //Se guardan las victorias, derrotas y el dinero.
    playerExport.wins = player->wins;
    playerExport.losses = player->losses;
    playerExport.money = player->money;

    //Se recorre el inventario.
    for (int i = 0; i < 5; i++)
    {
        if (player->inventory[i].item != NULL && player->inventory[i].qty > 0)
        {
            strcpy(playerExport.items[i].name, player->inventory[i].item->name);
            playerExport.items[i].qty = player->inventory[i].qty;
        
        }
        else
        {
            playerExport.items[i].qty = 0;
        }
    }
    // para cada pokemon
    for (int i = 0; i < 4; i++)
    {
        strcpy(playerExport.poke[i].name, player->pokemons[i].ptr->name);
        // para cada movimiento
        for (int j = 0; j < 4; j++)
        {
            // copiamos el nombre
            strcpy(playerExport.poke[i].move[j].name, player->pokemons[i].movements[j]->name);
            // copiamos los pp
            playerExport.poke[i].move[j].pp = player->pokemons[i].movements[j]->pp;
        }
    }

    //Si el archivo se creo bien.
    if (file != NULL)
    {
        fwrite(&playerExport, sizeof(struct PlayerExport), 1, file);
        fclose (file);
        printf("Archivo escrito exitosamente\n");
    }
    //El archivo no se pudo abrir o se creo mal. 
    else {
        printf("Archivo no se puede abrir\n");
        exit(1);
    }

}
