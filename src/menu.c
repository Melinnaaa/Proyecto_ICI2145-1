#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "csvread.h"
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
    printf("0. Salir: \n");
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

    //Utilizado para guardar el nombre del pokemon.
    char tmp[30];
    fflush(stdin);
    if (player->pokemons->ptr != NULL)
    {
        printf("El jugador ya existe!\n");
        return;
    }
    printf("Ingresa tu nickName: ");
    scanf("%[^\n]*s", player->name);
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
            player->pokemons[i] = playerPokemon;
            //showPokemon(playerPokemon.ptr);
        } else {
            printf("El pokemon no existe. Por favor "
                    "intentalo nuevamente.\n");
            goto reask;  // Volver a preguntar por el pokemon 1
        }
    }
}

//Guardar los datos del jugador o sobreescribirlos.
void sign_in (Player* players)
{
    FILE* userData = NULL;//archivo
    char directory[51];//Ubicación del archivo.
    int j;//Numero del jugador.
    
    //Se lee el numero del jugador del que se desea guardar los datos.
    printf("Inserte numero del jugador a registrar (1/2).\n");
    do
    {
        scanf("%d", &j);
        getchar();
    } while (j < 1 || j > 2);

    //Se obtienen la ubicacion.
    snprintf(directory, sizeof(directory), "Player/%s%s", players[j-1].name,".csv");

    //Comprobar que tenga datos que guardar.
    if (players[j-1].pokemons->ptr == NULL)
    {
        printf("No tienes datos para guardar, por favor registrate.\n");
        return;
    }

    //Primero se abre en "r" para no borrar los datos.
    userData =(fopen(directory,"r"));

    //Si existe el jugador se pregunta si quiere sobreescribir los datos.
    if (userData != NULL)
    {
        char answer[3];
        printf("Ya existen datos de guaradado con este nombre.\n¿desea sobreescribirlos?\nsi/no\n");
        do
        {
            scanf("%s[^\n]", answer);
            stringToLower(answer);
            getchar();
        } while (strcmp(answer, "no") != 0 && strcmp(answer, "si") != 0);

        //Se finaliza la función, ya que el usuario no quiere sobreescribir los datos.
        if (strcmp(answer, "no") == 0) 
        {
            fclose(userData);
            return;
        }
    }

    //Se abre en modo "w", ya que se eligio sobreescribir o no existia una partida.
    userData =(fopen(directory,"w"));

    //Se escribe el nombre/victorias/derrotas en la primera linea.
    fprintf(userData, "%s,%d,%d\n", players[j - 1].name, players[j - 1].wins, players[j - 1].losses);
    
    for (int k = 0 ; k < 4 ; k++)
    {
        //Se guarda el nombre del pokemon.
        fprintf(userData, "%s,\"", players[j - 1].pokemons[k].ptr->name);

        //Se guardaa el nombre de los movimientos.
        for (int i = 0 ; i < 4 ; i++)
        {   
            if (i == 3) fprintf(userData, "%s\"\n", players[j - 1].pokemons[k].movements[i]->name);
            else fprintf(userData, "%s,", players[j - 1].pokemons[k].movements[i]->name);
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

//Carga los datos de un perfil creado anteriormente.
void login (Player* players, HashMap* pkm, HashMap* moveMap)
{
    FILE* userData = NULL;//archivo
    char directory[51];//Ubicación del archivo.
    char name[30];
    int j;//Numero del jugador.
    
    //Se lee el numero del jugador en donde se desea cargar los datos.
    printf("Igrese numero del jugador en donde se guardaran los datos (1/2).\n");
    do
    {
        scanf("%d", &j);
        getchar();
    } while (j < 1 || j > 2);

    //Se lee el nombre del perfil a cargar.
    printf("Ingrese el nombre del perfil.\n");
    scanf("%s[^\n]", name);
    getchar();

    //Se obtiene la ubicación del archivo.
    snprintf(directory, sizeof(directory), "Player/%s%s", name,".csv");
    userData =(fopen(directory,"r"));
    
    //Si el usuario ingresado no existe.
    if (userData == NULL)
    {
        printf("El usuario que ingreso no existe.\n");
        return;
    }

    char linea[1024];//Linea del archivo.
    Pokemon* tmpPkm;//Variable que guarda el pokemon.
    List* moves;//Lista que almacena los nombres de movimientos.
    char* moveName = calloc (30, sizeof(char));

    Movement* tmpMovement;
    //Se lee el archivo csv linea por linea.
    for (int i = 0; i < 5 ; i++)
    {
        fgets (linea, 1023, userData);
        //Pimero se guarda el nombre, luego las victorias y finalmente las derrotas.
        if (i == 0)
        {
           strcpy(players[j-1].name, (char*)get_csv_field(linea, 0));
           players[j-1].wins = atoi((char*)get_csv_field(linea, 1));
           players[j-1].losses = atoi((char*)get_csv_field(linea, 2));
        }
        //Luego se guardan los pokemosn y sus ataques.
        else
        {
            //Se cargan los pokemons.
            tmpPkm = searchMap(pkm, (char*)get_csv_field(linea, 0))->value;
            players[j-1].pokemons[i-1].ptr = tmpPkm;

            //Se cargan los ataques del archivo a una lista.
            moves = strToList((char*)get_csv_field(linea, 1), ",");
            strcpy(moveName, (char*)listFirst(moves));
            for (int k = 0 ; k < 4 ; k++)
            {
                tmpMovement = searchMap(moveMap, moveName)->value;
                players[j-1].pokemons[i-1].movements[k] = tmpMovement;
                moveName = calloc (30, sizeof(char));
                if (k < 3) strcpy(moveName, (char*)listNext(moves));
            }
        }
    }
}

