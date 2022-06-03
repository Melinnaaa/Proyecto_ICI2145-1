#include <ctype.h>
#include <stdio.h>
#include <string.h>


#include "hashmap.h"

#include "menu.h"
#include "player.h"
#include "pokemon.h"
#include "util.h"

void showMenu()
{
    printf("1. Crear Perfil: \n");
    printf("2. Cargar Perfiles: \n");
    printf("3. Mostrar Perfiles: \n");
}


typedef struct PokemonExport {
    int id;
    int moves[4];
} PokemonExport;

typedef struct PlayerExport {
    char name[30];
    PokemonExport pokemons[4];
    int inventory[10];
    int wins;
    int losses;
} PlayerExport;

void createProfile(HashMap *pokeMap, HashMap *movements)
{
    /* Par y pokémon utilizados para la búsqueda */
    HashMapPair *pair;
    PlayerPokemon playerPokemon;

    char tmp[30];

    fflush(stdin);

    PlayerExport player;


    printf("Ingrese el nombre del jugador :");
    fflush(stdout);
    scanf("%[^\n]*s", player.name);
    getchar();

    for (int i = 0; i < 4; i++)
    {
reask:
        printf("Ingresa el nombre de tu ");
        switch (i) 
        {
            case 0:
              printf("primer ");
              break;
            case 1:
              printf("segundo ");
              break;
            case 2:
              printf("tercer ");
              break;
            case 3:
              printf("cuarto ");
              break;
                
        }
        printf("pokemon: ");
        fflush(stdout);

        scanf("%[^\n]*s", tmp);
        stringToLower(tmp);
        tmp[0] = toupper(tmp[0]);
        char * guion = strchr(tmp, '-');
        if (guion) guion[1] = toupper(guion[1]);
        getchar();

        pair = searchMap(pokeMap, tmp);

        if (pair != NULL)
        {
            playerPokemon.ptr = pair->value;
            randomizeMovements(&playerPokemon, movements);
            for (int i = 0; i < 4; i++)
            {
                player.pokemons[i].id = playerPokemon.ptr->ID;
                for (int j = 0; j < 4; j++)
                {
                    player.pokemons[i].moves[j] = playerPokemon.movements[j].ID;

                }

            }
            showPokemon(playerPokemon.ptr);
        } else {
            printf("El pokemon no existe. Por favor "
                    "intentalo nuevamente.\n");
            goto reask;  // Volver a preguntar por el pokemon 1
        }
    }
}

