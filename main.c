#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "hashmap.h"

#include "combat.h"
#include "item.h"
#include "menu.h"
#include "player.h"
#include "pokemon.h"
#include "util.h"

void showPlayers();

#ifdef DEBUG

struct PlayerExport {
    char name[30];
    int wins;
    int losses;
    int money;
    struct poke {
        char name[30];
        struct move {
            char name[15];
            int pp;
        } move[4];
    } pokemonExport[4];
};

void serializationExport(Player *player)
{
    if (player->pokemons->ptr == NULL)
    {
        printf("No tienes datos para guardar, por favor registrate.\n");
        return;
    }

    char dir[30] = "cache/";

    // Obtener hash del nombre del jugador.
    unsigned long namehash = djb2hash(player->name);

    char hash_str[10];
    sprintf(hash_str, "%lx", namehash & 0xFFFFFFUL);

    strcat(dir, hash_str);
    strcat(dir, ".pkdb");

    FILE *file = fopen(dir, "wb");

    struct PlayerExport playerExport;
    strcpy(playerExport.name, player->name);
    playerExport.wins = player->wins;
    playerExport.losses = player->losses;
    playerExport.money = player->money;
    // para cada pokemon
    for (int i = 0; i < 4; i++)
    {
        strcpy(playerExport.pokemonExport[i].name, player->pokemons[i].ptr->name);
        // para cada movimiento
        for (int j = 0; j < 4; j++)
        {
            // copiamos el nombre
            strcpy(playerExport.pokemonExport[i].move[j].name, player->pokemons[i].movements[j]->name);
            // copiamos los pp
            playerExport.pokemonExport[i].move[j].pp = player->pokemons[i].movements[j]->pp;
        }
    }

    if (file != NULL)
    {
        fwrite(&playerExport, sizeof(struct PlayerExport), 1, file);
    }

}

void serializationImport(Player *dest, unsigned char *name, HashMap *pokemon, HashMap *moves) {
    char hash_str[10];
    char dir[50] = "cache/";


    struct PlayerExport *playerImport = malloc(sizeof(struct PlayerExport));

    // Obtener hash del nombre del jugador.
    unsigned long namehash = djb2hash(name);

    sprintf(hash_str, "%lx", namehash & 0xFFFFFFUL);
    printf("hash: %lx\n", namehash & 0xFFFFFFUL);

    strcat(dir, hash_str);
    strcat(dir, ".pkdb");

    FILE *file = fopen(dir, "rb");

    if (file != NULL)
        fread(&playerImport, sizeof(struct PlayerExport), 1, file);
    if (playerImport->name[0] == 0) {
        printf("El archivo no existe.\n");
        return;
    }

    strcpy(dest->name , playerImport->name);

    dest->wins = playerImport->wins;
    dest->losses = playerImport->losses;
    dest->money = playerImport->money;

    // para cada pokemon
    for (int i = 0; i < 4; i++)
    {
        HashMapPair *pair = searchMap(pokemon, playerImport->pokemonExport[i].name);
        if (pair)
        {
            Pokemon *tmp = pair->value;
            dest->pokemons[i].ptr = tmp;
            dest->pokemons[i].consumed = 0;
            // para cada movimiento
            for (int j = 0; j < 4; j++)
                dest->pokemons[i].pps[j] = playerImport->pokemonExport[i].move[j].pp;

        } else {
            printf("Error cargando perfil:\n");
            printf("%s no fue encontrado en la base de datos\n", playerImport->pokemonExport[i].name);
        }

    }


    
}
#endif

int main() {
    srand(time(NULL));
    printf("Bienvenido a pokemon.exe\n");

    printf("Para empezar crea o carga tu perfil.\n\n");
    
    //Mapa de movimientos con clave a partir del tipo.
    HashMap* movementsStr = createMap(218);

    //Mapa de pokemons con clave a partir de su nombre.
    HashMap* pokemonsStr = createMap(917);

    //Mapa de movimientos con clave a partir de su nombre.
    HashMap* movements = createMap(218);

    //Arreglo de items disponibles.
    Item* items = calloc(5, sizeof(Item));
//    Item items[6];

    //Grafo de afinidades.
    HashMap* effective = createMap(35);

    //Grafo de debilidades.
    HashMap* uneffective = createMap(40);

    //Se cargan los pokemons, movimientos y items de archivos csv.
    loadPokemons(pokemonsStr);
    loadMovements(movementsStr, movements);
    loadItems(items);
    getEffectiveNodes(effective);
    getUneffectiveNodes(uneffective);

    int j;//Almacena el numero del jugador.
    int in = -1;//Opcion ingresada por el usuario.

    // Se inicializan las variables de los jugadores.
    Player players[2];
    for (int i = 0; i < 2; i++) {
        createPlayer(&players[i]);
    }

    //Bucle principal del programa.
    while (in != 0) {
        showMenu();

#ifdef DEBUG
        in = checkNum(0, 7);
#else 
        in = checkNum(0, 6);
#endif
        
        switch(in)  
        {
            case 1: // Crear perfil.
            {

                //Se recibe el numero en donde se almacenaran los datos del jugador
                j = getPlayerPos();

                if (j != 0){
                    printf("¡Hola a todos! ¡Bienvenidos al mundo de POKÉMON! ¡Me llamo Ignacio! ¡Este mundo está habitado por unas criaturas llamadas POKÉMON! Para algunos, los POKÉMON son mascotas. Pero otros los usan para pelear. En cuanto a mí... Estudio a los POKÉMON como profesión. \n");
                    createProfile(&players[j-1], pokemonsStr, movementsStr);
                }
                if (j == 0) {
                    printf("Regresando al menú.");
                    putchar('\n');
                    continue;
                }
                break;
            }
            case 2: // Cargar perfiles.
            {
                login(players, pokemonsStr, movements, items);
                break;
            }
            case 3: // Mostrar jugadores.
            {
                showPlayers(players);
                break;
            }
            case 4://Guardar los perfiles.
            {
                sign_in(players);
                break;
            }
            case 5://Comprar items en la tienda.
            {
                j = getPlayerPos();
                buyItems(items, &players[j-1]);
                break;
            }

            case 6:
            {
                initCombat(players, effective, uneffective);
                break;
            }
#ifdef DEBUG
            case 7:
            {
                printf("1. Importar\n2. Exportar\n0. Volver");
                int in2 = checkNum(1,2);
                char playerName[20];
                switch (in2) {
                    case 0:
                        continue;
                        break;
                    case 1:
                        printf("Qué jugador eres? (1/2): \n");
                        int inplayer = checkNum(1,2);
                        if (!inplayer) continue;
                        printf("Ingresa el nombre del jugador: \n");
                        scanf("%[^\n]20s", playerName);
                        printf(".%s.\n", playerName);
                        serializationImport(players + inplayer - 1, playerName, pokemonsStr, movementsStr);
                        break;
                    case 2:
                        printf("Qué jugador eres? (1/2): \n");
                        inplayer = checkNum(1,2);
                        if (!inplayer) continue;
                        serializationExport(players + inplayer - 1);

                }
                break;
            }
#endif
            case 0:
            {
                return 0;
            }
        }
    }
    printf("lo vimoh\n");
    
    return 0;
}

/* Se crea el inventario vacio.
 * se carga si es que existe (TODO) */
void createPlayer(Player *p)
{
    for( int i = 0; i < 5; i++)
    {
        p->inventory[i].qty = 0;
        p->inventory[i].item = NULL;
    }
    p->wins = 0;
    p->losses = 0;
    p->money = 0;
    p->canPlay = 0;
    p->pokemons->ptr = NULL;
 
}



//Muestra el item del jugador.
void showItem(Item *item)
{
    printf("%s\n", item->name);
    return;
}

