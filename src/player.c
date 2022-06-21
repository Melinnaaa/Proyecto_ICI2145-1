int printf (const char * , ...);
int scanf (const char *restrict , ...);
int getchar();
int putchar(int __c);


#include "player.h"

//Mostrar datos del jugador.
void showPlayer(Player *p)
{
    printf("Jugador: %s\n\n", p->name);
    printf("Victorias: %d\t", p->wins);
    printf("Derrotas: %d\n\n", p->losses);
    printf("Ratio victorias / derrotas: %.1f%%\n\n", (p->losses && p->wins)  ?
            ((float)p->wins / p->losses) : 0 );
    printf("Dinero: $%d\n\n", p->money);
    printf("Items: \n");

    int flagItems = 0;
    //Se muestra el inventario si tiene objetos.
    for(int i = 0; i < 5; i++) {
        if(p->inventory[i].item != NULL) {
            printf(" %s\t x %d", p->inventory[i].item->name,
                    p->inventory[i].qty);
            flagItems = 1;
        }
    }

    if (!flagItems) printf("no hay items\n");
    //Se muestran los pokemons del jugador.
    for(int i = 0; i < 4; i++) 
    {
        showPlayerPokemon(&p->pokemons[i]);
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
        {
            showPlayer(&players[i]);
            putchar('\n');
        }

        else
            printf("El jugador %d no se ha registrado.\n\n", i + 1);
    }
}
