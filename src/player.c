#include <string.h>
int putchar(int __c);

int printf (const char * , ...);
int scanf (const char *restrict , ...);
int getchar();


#include "player.h"

//Mostrar datos del jugador.
void showPlayer(Player *p)
{
    printf("Jugador: %s\n\n", p->name);
    printf("Victorias: %d\n", p->wins);
    printf("Derrotas: %d\n", p->losses);
    printf("Ratio victorias / derrotas: %f\n", (p->losses && p->wins)  ?
            ((float)p->wins / p->losses) : 0 );
    printf("Dinero: %d\n\n", p->money);
    printf("Items: \n");
    //Se muestra el inventario si tiene objetos.
    for(int i = 0; i < 5; i++) {
        if(p->inventory[i].item != NULL) {
            showItem(p->inventory[i].item);
        }
    }
    putchar('\n');

    //Se muestran los pokemons del jugador.
    for(int i = 0; i < 4; i++) 
    {
        showPlayerPokemon(&p->pokemons[i]);
        putchar('\n');
    }
}

//Obtiene la posición del jugador
int getPlayerPos()
{
    int j;
    printf("Qué jugador eres? (1/2)\n");
    printf("0 para volver al menú\n");
    do
    {
        scanf("%d", &j);
        getchar();
    } while ( ((j < 1 ) && ( j != 0 )) || (j > 2 ) );
    return j;
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

//Muestra el item del jugador.
void showItem(Item *item)
{
    printf("%s\n", item->name);
    return;
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

    for( int i = 0; i < 4; i++)
    {
        p->pokemons[i].ptr = NULL;
        for (int j = 0; j < 4; j++)
        {
            p->pokemons[i].movements[j] = NULL;
            p->pokemons[i].pps[j] = 0;
        }
    }
    p->wins = 0;
    p->losses = 0;
    p->money = 0;
    strcpy(p->name, "");
    p->canFight = 0;
}
