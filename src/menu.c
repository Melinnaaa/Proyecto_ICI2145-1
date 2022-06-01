#include <stdio.h>
#include <ctype.h>

#include "hashmap.h"

#include "menu.h"
#include "player.h"
#include "pokemon.h"

void showMenu()
{
    printf("1. Crear Perfil: \n");
    printf("2. Cargar Perfiles: \n");
    printf("3. Mostrar Perfiles: \n");
}

void createProfile(HashMap *pokeMap)
{
    Player player;

    /* Par y pokémon utilizados para la búsqueda */
    HashMapPair *pair;
    PlayerPokemon playerPokemon;

    char tmp[20];

    fflush(stdin);

    createPlayer(&player);


    printf("Ingrese el nombre del jugador: ");
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

        scanf("%[^\n]*s", tmp);
        tmp[0] = toupper(tmp[0]);
        getchar();

        pair = searchMap(pokeMap, tmp);

        if (pair != NULL)
        {
            playerPokemon.ptr = pair->value;
            player.pokemons[0] = playerPokemon;
            //showPokemon(playerPokemon->ptr);
        } else {
            printf("El pokemon no existe. Por favor "
                    "intentalo nuevamente.\n");
            goto reask;  // Volver a preguntar por el pokemon 1
        }

    }

}

