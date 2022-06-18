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
#include "combat.h"

void showPlayers();

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
        createPlayer(&players[0]);
        createPlayer(&players[1]);

    //Bucle principal del programa.
    while (in != 0) {
#ifdef DEBUG
        printf("DEBUG: players[0].canFight = %d\n", players[0].canFight);
        printf("DEBUG: players[1].canFight = %d\n", players[1].canFight);
#endif
        showMenu();
        in = checkNum(0, 6);
        
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
            case 6: // Iniciar combate
            {
                // la instancia de combat iniciada se elimina al terminar el combate
                startCombat(&players[0], initCombat(players));
                break;
            }
            case 0:
            {
                return 0;
            }
        }
    }
    printf("lo vimoh\n");
    
    return 0;
}





