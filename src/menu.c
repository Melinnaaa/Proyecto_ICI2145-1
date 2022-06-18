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
    printf("1. Nueva partida\t\t");
    printf("3. Mostrar jugadores\n");

    printf("2. Cargar partida: \t\t");
    printf("4. Guardar perfiles \n\n");

    printf("5. Tienda\n");
    printf("6. Jugador vs Jugador\n\n");
//    printf("3. Mostrar Perfiles \n");
//    printf("4. Guardar Perfil: \n");
//    printf("5. Comprar objetos: \n");
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

    //Se verifica que no existan datos anteriores, de existir se finaliza la funcion.
    if (player->pokemons->ptr != NULL)
    {
        printf("El jugador ya existe!\n");
        return;
    }

    //Se solicita el nombre del usuario.
    printf("Cuál es tu nombre?:  ");
    scanf("%[^\n]*s", player->name);
    getchar();
    player->money = 2000;
    //Ciclo que se encarga de guardar 4 pokemons.
    printf("\nPara que empieces tu aventura te otorgaré 4 pokémons, así que eligelos sabiamente.\n\n ");
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

        //Se lee el nombre del pokemon solicitado.
        scanf("%[^\n]*s", tmp);
        getchar();
        stringToLower(tmp);
        tmp[0] = toupper(tmp[0]);
        //Caso en que el nombre tenga guion.
        char * guion = strchr(tmp, '-');
        if (guion) 
        {
            printf("%s\n", guion);
            guion[1] = toupper(guion[1]);
        }
        //Caso en que el nombre tenga espacio.
        char * espacio = strchr(tmp, ' ');
        if (espacio) espacio[1] = toupper(espacio[1]);

        //Y se busca en el mapa de pokemones.
        pair = searchMap(pokeMap, tmp);

        //Si existe se le generan movimientos randoms.
        if (pair != NULL)
        {
            playerPokemon.ptr = pair->value;
            randomizeMovements(&playerPokemon, movements);
            player->pokemons[i] = playerPokemon;
            //showPokemon(playerPokemon.ptr);
        } 
        //De no encontrarse se solicita que ingrese el nombre nuevamente.
        else 
        {
            printf("El pokemon no existe. Por favor "
                    "intentalo nuevamente.\n");
            goto reask;  // Volver a preguntar por el pokemon 1
        }
    }
    player->canFight = 1;
}

//Guardar los datos del jugador o sobreescribirlos.
void sign_in (Player* players)
{
    FILE* userData = NULL;//archivo
    char directory[51];//Ubicación del archivo.
    int j;//Numero del jugador.
    
    //Se lee el numero del jugador del que se desea guardar los datos.
    printf("Inserte numero del jugador a registrar (1/2).\n");
    j = checkNum(1, 2);

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

    //Si existe el jugador se pregunta si quiere sobrescribir los datos.
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
    fprintf(userData, "%s,%d,%d,%d\n", players[j - 1].name, players[j - 1].wins, players[j - 1].losses, players[j - 1].money);
    
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
    
    //Se guardan los objetos.
    for (int i = 0 ; i < 5 ; i++)
    {
        //Si la casilla i contiene objetos se guarda la posición, cantidad y nombre del objeto.
        if(players[j - 1].inventory[i].item != NULL && players[j-1].inventory[i].qty > 0)
            fprintf(userData, "%d,%d\n", i, players[j-1].inventory[i].qty);
    }

    fclose(userData);
} 

//Carga los datos de un perfil creado anteriormente.
void login (Player* players, HashMap* pkm, HashMap* moveMap, Item* items)
{
    FILE* userData = NULL;//archivo del usuario.
    char directory[51];//Ubicación del archivo.
    char name[30];
    int j;//Numero del jugador.
    int k;//Posición del objeto.
    
    //Se lee el numero del jugador en donde se desea cargar los datos.
    printf("Igrese numero del jugador en donde se guardaran los datos (1/2).\n");
    j = checkNum(1, 2);
#ifdef DEBUG
    printf("DEBUG: login() j value = %d\n", j)
#endif

    if (j == '0')
    {
        fflush(stdin);
        return;
    }

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
           players[j-1].money = atoi((char*)get_csv_field(linea, 3));
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
    while (fgets (linea, 1023, userData) != NULL)
    {
        k = atoi((char*)get_csv_field(linea, 0));
        players[j-1].inventory[k].item = &items[k];
        players[j-1].inventory[k].qty = atoi((char*)get_csv_field(linea, 1));
    }
    
    fclose(userData);
    // El jugador ahora si puede pelear.
    players[j-1].canFight = 1;
}

//Muestra los items disponibles de la tienda.
void showShop(Item* items)
{
    for (int i = 0 ; i < 5 ; i++)
    {
        printf("%d. %s  $%d\n", i+1, items[i].name, items[i].price);
        if (i < 3)
            printf("Medicina en spray que restaura %d ps de tu pokemon\n", items[i].effect);
        else
            printf("Restaura un total de %d pp de tu pokemon\n", items[i].effect);
        printf("\n");
    }
}

//Comprar items de la tienda
void buyItems(Item* items, Player* players)
{
repeat:
    //Se muestran los items de la tienda
    showShop(items);
    int i;//Opcion
    int qty;//Cantidad de items a comprar
    printf("Seleccione una opcion\n");
    i = checkNum(1, 5);

    printf("¿%s? Buena eleccion\n", items[i-1].name);
    printf("¿Cuantas quieres?, maximo 10 unidades.\n");
    qty = checkNum(0, 10);
    
    //Se comprueba que el usuario disponga de dinero.
    if (players->money >= (qty*items[i-1].price))
    {
        players->money -= (qty*items[i-1].price);
        players->inventory[i-1].item = &items[i-1];
        players->inventory[i-1].qty = qty;
    }
    else
    {
        printf("Lo sentimos, no tienes suficiente dinero.\n");
    }
    printf("Desea continuar?\n");
    printf("1. Si\n");
    printf("2. No\n");
    i = checkNum(1, 2);

    if (i == 1)
        //Se repite todo nuevamente si desea continuar.
        goto repeat;
    
}

void getEffectiveNodes(HashMap* effective)
{
    FILE* affinities = fopen ("Affinities.csv", "r");
    char linea[1024];
    //Se lee el archivo csv linea por linea.
    while (fgets (linea, 1023, affinities) != NULL)
    {
        insertMap(effective, get_csv_field(linea, 0), strToList(get_csv_field(linea, 1), ", "));
    }
    fclose(affinities);
}

void getUneffectiveNodes(HashMap* uneffectiveMap)
{
    // printf("ola");
    FILE* uneffective = fopen("rip.csv", "r");
    if (uneffective == NULL)
    {
//   printf("cagué");
    }
    char linea[1024];
    //Se lee el archivo csv linea por linea.
    while (fgets (linea, 1023, uneffective) != NULL)
    {
        insertMap(uneffectiveMap, get_csv_field(linea, 0), strToList(get_csv_field(linea, 1), ", "));
    }
  //   printf("salí");
    fclose(uneffective);
}

