#include <stdio.h>

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
    Pokemon *pk;

    char tmp[20];

    fflush(stdin);

    createPlayer(&player);
    printf("Ingrese el nombre del jugador: ");

    scanf("%[^\n]*s", player.name);
    getchar();

    printf("Ingresa el nombre de tu primer pokémon\n");
    scanf("%[^\n]*s", tmp);
    getchar();

    pair = searchMap(pokeMap, tmp);

    if (pair != NULL)
    {
        pk = pair->value;

    }


}

