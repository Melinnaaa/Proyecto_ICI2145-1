#include "combat.h"

#include "util.h"

int printf (const char * , ...);
int getchar();

struct Combat {
    struct {
        struct {
            Player *ptr;
            int consumed[4]; // Atacó el pokémon?
        } current;

        struct {
            Player *ptr;
            int knocked[4]; // Esta aturdido el enemigo?
        } enemy;

    } turn;

    struct {
        HashMap *effective,
                *uneffective;
    } maps;
};

void initCombat(Player *players, HashMap *effective, HashMap *uneffective)
{
    if (!(players[0].canPlay) || !(players[1].canPlay))
    {
        printf("Para iniciar un combate es necesario cargar los perfiles.\n");
        printf("Pulsa una tecla para continuar..");
        getchar();
        return;
    }


    struct Combat combat;

    // Guardar los punteros de los mapas.
    combat.maps.effective = effective;
    combat.maps.uneffective = uneffective;

    // Determinar el primer turno.
    int who = randomNumber(0, 1);
    combat.turn.current.ptr = players + who;
    combat.turn.enemy.ptr = players + !(who);

    printf("Parte %s\n", combat.turn.current.ptr->name);
}
