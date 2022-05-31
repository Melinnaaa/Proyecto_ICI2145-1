#include <stdio.h>
#include "menu.h"
#include "player.h"

void showMenu()
{
    printf("1. Crear Perfil: \n");
    printf("2. Cargar Perfiles: \n");
    printf("3. Mostrar Perfiles: \n");
}

void createProfile()
{
    Player player;
    printf("Ingrese el nombre del jugador: ");
    fflush(stdin);
    scanf("%[^\n]*s", player.name);
    getchar();

}

