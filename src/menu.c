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
    printf("2. Cargar Perfil: \n");
    printf("3. Mostrar Perfiles: \n");
    printf("4. Guardar Perfil: \n");
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

void createProfile(Player *player, HashMap *pokeMap, HashMap *movements)
{
    /* Par y pokémon utilizados para la búsqueda */
    HashMapPair *pair;
    PlayerPokemon playerPokemon;

    char tmp[20];

    fflush(stdin);

    createPlayer(player);



    for (int j = 0; j < 2; j++)
    {

        printf("Ingrese el nombre del jugador %d: ", j + 1);
        scanf("%[^\n]*s", player[j].name);
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
            stringToLower(tmp);
            tmp[0] = toupper(tmp[0]);
            getchar();

            pair = searchMap(pokeMap, tmp);

            if (pair != NULL)
            {
                playerPokemon.ptr = pair->value;
                randomizeMovements(&playerPokemon, movements);
                player[j].pokemons[i] = playerPokemon;
                showPokemon(playerPokemon.ptr);
            } else {
                printf("El pokemon no existe. Por favor "
                        "intentalo nuevamente.\n");
                goto reask;  // Volver a preguntar por el pokemon 1
            }

        }

    }

}

//Guardar los datos o sobreescribirlos
void sign_in (Player* players)
{
    FILE* userData = NULL;//archivo
    char directory[51];//Ubicación del archivo.
    int j;//Numero del jugador.
    
    //Se lee el numero del jugador.
    printf("Inserte numero del jugador a registrar\n");
    scanf("%d", &j);
    getchar();
    //Se obtienen los datos del usuario.
    snprintf(directory, sizeof(directory), "Player/%s%s", players[j-1].name,".csv");
    userData =(fopen(directory,"w"));
    //Si existe el jugador se pregunta si quiere sobreescribir los datos.
    if (userData != NULL)
    {
        char answer[3];
        printf("Los datos ya existen, ¿desea sobreescribirlos?\nsi/no\n");
        scanf("%s[^\n]", answer);
        stringToLower(answer);
        getchar();
        //Se finaliza la función, ya que el usuario no quiere sobreescribir los datos.
        if (strcmp(answer, "no") == 0) 
        {
            fclose(userData);
            return;
        }
    }
    //Se escribe el nombre/victorias/derrotas en la primera linea
    fprintf(userData, "%s, %d, %d\n", players[j - 1].name, players[j - 1].wins, players[j - 1].losses);
    
    for (int k = 0 ; k < 4 ; k++)
    {
         //Se guarda el nombre del pokemon
        fprintf(userData, "%s, \"", players[j - 1].pokemons[k].ptr->name);
        //Se guardan los movimientos
        for (int i = 0 ; i < 4 ; i++)
        {   
            if (i == 3) fprintf(userData, "%s\"\n", players[j - 1].pokemons[k].movements[i]->name);
            else fprintf(userData, "%s, ", players[j - 1].pokemons[k].movements[i]->name);
        }
    }
    /*
    //Se guardan los objetos.
    fprintf(userData, "\"");
    for (int i = 0 ; i < 10 ; i++)
    {
        fprintf(userData, "%s, ", players[j - 1].inventory[i].item->name);
        if (i == 9) fprintf(userData, "%s\"\n", players[j - 1].inventory[i].item->name);
    }*/

    fclose(userData);
}

void login (Player* players)
{
    FILE* userData = NULL;//archivo
    char directory[51];//Ubicación del archivo.
    int j;//Numero del jugador.
    
    //Se lee el numero del jugador.
    printf("Inserte numero del jugador en donde se guardaran los datos.\n");
    scanf("%d", &j);
    getchar();
    //Se obtienen los datos del usuario.
    snprintf(directory, sizeof(directory), "Player/%s%s", players[j-1].name,".csv");
    userData =(fopen(directory,"r"));
    if (userData == NULL)
    {
        printf("El usuario que ingreso no existe.\n");
        return;
    }
}

