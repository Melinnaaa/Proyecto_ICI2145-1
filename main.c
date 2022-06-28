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
#include "cpu.h"
#include "serialization.h"
#include "league.h"

void showPlayers();

#ifdef DEBUG

/*
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
    } poke[4];
};
*/


void serializationImport(Player *dest, unsigned char *name, HashMap *pokemon, HashMap *moves) {
    char hash_str[10];
    char dir[50] = "cache/";


    struct PlayerExport playerImport; // = malloc(sizeof(struct PlayerExport));

    // Obtener hash del nombre del jugador.
    unsigned long namehash = djb2hash(name);

    sprintf(hash_str, "%lx", namehash & 0xFFFFFFUL);
#ifdef DEBUG
    printf("hash: %lx\n", namehash & 0xFFFFFFUL);
#endif

    strcat(dir, hash_str);
    strcat(dir, ".pkdb");

    FILE *file = fopen(dir, "rb");
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
#endif

int main() {
    // inicializar random
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
        in = checkNum(0, 9);
#else 
        in = checkNum(0, 8);
#endif
        
        switch(in)  
        {
            case 1: // Crear perfil.
            {

                //Se recibe el numero en donde se almacenaran los datos del jugador
                j = getPlayerPos();

                if (j != 0){
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
            case 7:
            {
                Player cpu = createCPU(pokemonsStr, movements);
                int win = 0;
#ifdef DEBUG
                showPlayer(&cpu);
#endif  
                initCpuCombat(&players[0], &cpu, effective, uneffective, &win);
                break;
            }
            case 8:
            {
                //Se verifica que el jugador pueda ingresar a la liga.
                if (players[0].canPlay == 0)
                {
                    printf("\nPara ingresar a la liga es necesario crear un perfil...\n");
                    getchar();
                    continue;
                }
                printf("\nEsta es la liga pokemon!\n");
                initComputerLeague(&players[0], effective, uneffective, pokemonsStr, movements);
                break;
            }
#ifdef DEBUG
            case 9:
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
                        serializationImport(players + inplayer - 1, playerName, pokemonsStr, movements);
                        break;
                    case 2:
                        printf("Qué jugador eres? (1/2): \n");
                        inplayer = checkNum(1,2);
                        if (!inplayer) continue;
                        exportPlayer(players + inplayer - 1);

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

