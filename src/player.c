int printf (const char * , ...);
int scanf (const char *restrict , ...);
int getchar();


#include "player.h"

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
