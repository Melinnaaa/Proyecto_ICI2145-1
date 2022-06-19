#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#include "combat.h"
#include "player.h"
#include "util.h"

int searchAbilities (HashMap* map, char* attackType, char* typeEnemy)
{
    List* tmp;
    char *tmpType;
    if (searchMap(map, attackType) != NULL)
    {
#ifdef DEBUG
        printf("tipo ataque: %s\n", attackType);
        printf("tipo Enemigo: %s\n", typeEnemy);
#endif
        tmp = searchMap(map, attackType)->value;
        tmpType = listFirst(tmp);
        while (1)
        {
#ifdef DEBUG
            printf("TIPO: %s\n", tmpType);
#endif
            if (strcmp(tmpType, typeEnemy) == 0) return 1;//Es efectivo.
                                                
            tmpType = listNext(tmp);

            if (tmpType == NULL) { 
                break;
            } 
        }
    }
#ifdef DEBUG
    printf("no encontró nada\n");
#endif
    return 0; //No es efectivo.
}

// Crea una instancia de Combat
Combat *initCombat(Player *players, HashMap *effective, HashMap *uneffective)
{
    Combat *this = NULL;//Se inicializa en null
#ifdef DEBUG
    printf("DEBUG: p1p: %d\n p2p: %d\n", players[0].canFight, players[1].canFight);
#endif

    //Se comprueba que ambos jugadores tengan un perfil creado.
    if (!(players[0].canFight && 
            players[1].canFight ))
    {
        return this;
    }


    this = malloc(sizeof(Combat));
    this->turnAttack = 0;//Cuenta los ataques realizados, maximo 4.
    this->effective = effective, this->uneffective = uneffective;//guarda grafos de efectividad y inefectividad.
    this->turn = randomNumber(0, 1);//Comienza el turno un jugador random.
    this->players = players;//Puntero a los jugadores.
    printf("Empieza el jugador %d. (%s)\n", this->turn + 1, players[this->turn].name);
    //Ciclo que controla el combate, si la suma de todos los pokemons es 0 se finaliza.
    while (((players[0].pokemons[0].hp + 
            players[0].pokemons[1].hp + 
            players[0].pokemons[2].hp + 
            players[0].pokemons[3].hp) != 0 ) && 
            ((players[0].pokemons[0].hp + 
            players[0].pokemons[1].hp + 
            players[0].pokemons[2].hp + 
            players[0].pokemons[3].hp) != 0 )
        ){
        mainMenuCombat(this);
    }

    return this;
}

void escapeCombat()
{

}

void openInventory()
{

}

void doAttack(Combat *combat, int attackMov, int pokemon)
{
    Player *current = combat->players + combat->turn;
    Player *enemy = combat->players + !(combat->turn);
    //Se les resta uno para poder acceder a la posición correcta.
    attackMov --; pokemon --;

    //Se muestra el ataque que utilizo
    printf("%s usó %s\n", current->selection->ptr->name, current->selection->movements[attackMov]->name);

    //Se verifica que el ataque del pokemon sea efectivo.
    float effective = searchAbilities(
                combat->effective, 
                current->selection->movements[attackMov]->type,
                enemy->pokemons[pokemon].ptr->type->head->data
                ) * 1.5;
    float diff = current->selection->movements[attackMov]->damage * 0.25 ;

    //Si el ataque es efectivo se muestra un mensaje por pantalla.
    printf( effective ?
            "Es super efectivo\n" : "\n"  );

    //Si es efectivo se le añade un multiplicador de 1.5
    if (effective) diff *= effective;

    //Si no lo es se verifica que no sea efectivo.
    else {
        effective = searchAbilities(
                    combat->uneffective, 
                    current->selection->movements[attackMov]->type,
                    enemy->pokemons[pokemon].ptr->type->head->data
                    );
        diff *= !effective;
        if (effective){
            printf("No hizo nada!\n");
        } 

    }

    //Se le resta la vida al pokemon, a partir del daño realizado.
    if (enemy->pokemons[pokemon].hp -= diff < 0) 
    {
        printf("%s se ha debilitado!\n", enemy->pokemons[pokemon].ptr->name);
        enemy->pokemons[pokemon].hp = 0;
    }
    //Si no muere, se muestra el daño realizado por el pokemon
    else
    {
        printf("La vida del %s enemigo bajó %.1f HP\n", 
        enemy->pokemons[pokemon].ptr->name,
        diff);
    }


    //El pokemon ya no es apto para atacar.
    current->selection->enabled = 0;

    //Si el turnAttack es mayor, hay qye cambiar el turno.
    if (combat->turnAttack >= 3)
    {
        combat->turn = 1 - combat->turn;
        return;
    }
    //Sino se aumenta. 
    else {
        current->selection += 1;
        (combat->turnAttack) ++;
    }
#ifdef DEBUG               
    printf("turnAttack: %d\n", combat->turnAttack);
    printf("turn: %d\n", combat->turn);
#endif

}

void attackMenu(Combat *combat)
{
    int in = -1, in2 = -1;//Numero de ataque y pokemon a atacar.
    Player *current = combat->players + combat->turn;//Posición del jugador a atacar.
    Player *enemy = combat->players + !(combat->turn);

    //Se muestra el pokemon seleccionado y su vida actual.
    printf("Tu pokémon seleccionado es %s:", current->selection->ptr->name);
    printf(" %d HP\n\n", current->selection->hp);
        

    printf("Elige tu ataque: \n");
    //Se muestran los ataques del pokemon actual.
    for(int i = 0; i < 4; i ++)
    {
        printf("%d. %s\n", i + 1, current->selection->movements[i]->name);
    }
reask:
    //Se obiene el numero del ataque.
    in = checkNum(0, 4);

    if (in == 0) return;

    printf("A quién quieres atacar?\n");

    //Se muestran los pokemons enemigos y su vida actual.
    for (int i = 0; i < 4; i++)
    {
        printf("%d. %s. HP: %d\n", i+1, enemy->pokemons[i].ptr->name, 
                enemy->pokemons[i].hp);
    }

    //Se obtiene la posición del pokemon a atacar.
    in2 = checkNum(0, 4);

    //Se verfica que el pokemon seleccionado no este debilitado.
    while (enemy->pokemons[in2-1].hp == 0)
    {
        printf("El pokemon esta debilitado, selecciona otro.\n");
        in2 = checkNum(0, 4);
    }
    if (in2 == 0) goto reask;// para atacar necesitamos: numero ataque.
    
    doAttack(combat, in, in2);
}

//Muestra las opciones principales del menu.
void showMainMenuCombat()
{
    printf("Qué quieres hacer?\n\n");
    printf(
        "1. Atacar\n"
        "2. Inventario\n"
        "3. Huir\n"
            );
}

void mainMenuCombat(Combat *combat)
{
    char in = -1;
    printf("Turn: %d\n", combat->turn);

    Player *current = combat->players + combat->turn;


    while (in != '0') {
        printf("Turn: %d\n", combat->turn);
        //Se obtiene el jugador al que le toca atacar y se muestra por pantalla.
        current = combat->players + combat->turn;
        printf("Turno de %s:\n\n", current->name);

        //Se muestran los pokemons del jugador.
        for(int i = 0; i < 4; i++)
        {
            printf(
                    current->selection == (current->pokemons + i) ? 
                    "\u00b7 %s: %d HP\n":"  %s: %d HP\n",
                    current->pokemons[i].ptr->name,
                    current->pokemons[i].hp);
        }
        
        //Se muestra el menu de combate.
        showMainMenuCombat();

        fflush(stdin);
        in = getchar() - '1' + 1;
        getchar(); // \n
        //Switch que maneja las opciones escogidas.     
        switch (in)
        {
            case 1: // Atacar
                attackMenu(combat);
                break;
            case 2: // Inventario
                openInventory();
                break;
            case 3: // Huir
                escapeCombat();
                break;
            case 0: // Salir
                return;
        }


    }


}

void startCombat(Player * players, Combat *combat)
{
    if (!combat)
    {
#ifdef DEBUG
        printf("DEBUG: p1p: %d\n p2p: %d\n", players[0].canFight, players[1].canFight);
#endif
        printf("Para iniciar un combate es necesario cargar los perfiles.\n");
        printf("Pulsa una tecla para continuar..");
        getchar();
        return;
    }
    mainMenuCombat(combat);
    
}
