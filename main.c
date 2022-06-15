#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "hashmap.h"
#include "menu.h"
#include "pokemon.h"
#include "item.h"
#include "player.h"
#include "util.h"

void showPlayers();

int main() {
    srand(time(NULL));
    printf("Bienvenido a pokemon.exe\n");
    
    //Mapa de movimientos con clave a partir del tipo.
    HashMap* movementsStr = createMap(218);

    //Mapa de pokemons con clave a partir de su nombre.
    HashMap* pokemonsStr = createMap(917);

    //Mapa de movimientos con clave a partir de su nombre.
    HashMap* movements = createMap(218);

    //Arreglo de items disponibles.
    Item* items = calloc(5, sizeof(Item*));

    //Grafo de afinidades.
    HashMap* effective = createMap(35);

    //Grafo de debilidades.
    //HashMap* uneffective = createMap(40);

    //Se cargan los pokemons, movimientos y items de archivos csv.
    loadPokemons(pokemonsStr);
    loadMovements(movementsStr, movements);
    loadItems(items);
    getEffectiveNodes(effective);
    //getUneffectiveNodes(uneffective);

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
        in = checkNum(0, 5);
        
        switch(in)  
        {
            case 1: // Crear perfil.
            {
                //Se recibe el numero en donde se almacenaran los datos del jugador
                j = getPlayerPos();
                createProfile(&players[j-1], pokemonsStr, movementsStr);
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
    p->pokemons->ptr = NULL;
 
}

//Iterar jugadores.
void showPlayers(Player *players)
{   
    for (int i = 0; i < 2; i++)
    {
        //Si el jugador existe se muestran sus datos.
        if(players[i].pokemons->ptr != NULL)
            showPlayer(&players[i]);
        else
            printf("El jugador %d no se ha registrado.\n\n", i + 1);
    }
}

//Mostrar datos del jugador.
void showPlayer(Player *p)
{
    printf("Jugador: %s\n", p->name);
    printf("Victorias: %d\n", p->wins);
    printf("Derrotas: %d\n", p->losses);
    printf("Ratio victorias / derrotas: %f\n", (p->losses && p->wins)  ?
            ((float)p->wins / p->losses) : 0 );
    printf("Dinero: %d", p->money);
    //Se muestra el inventario si tiene objetos.
    for(int i = 0; i < 5; i++) {
        if(p->inventory[i].item != NULL) {
            showItem(p->inventory[i].item);
        }
    }

    //Se muestran los pokemons del jugador.
    for(int i = 0; i < 4; i++) 
    {
        showPlayerPokemon(&p->pokemons[i]);
    }
}

//Muestra el item del jugador.
void showItem(Item *item)
{
    printf("%s\n", item->name);
    return;
}

