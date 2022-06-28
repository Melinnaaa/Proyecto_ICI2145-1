#include <string.h>
#include <stdio.h>
#include "combat.h"
#include "util.h"


void showCombatMenu(struct Combat *combat)
{
    printf("Turno de %s\n", combat->turn.current.ptr->name);

    //Se muestran los pokemons del jugador.
    for(int i = 0; i < 4; i++)
    {
        printf(
                (combat->turn.current.selection == combat->turn.current.ptr->pokemons + i) ?
                "\u00b7 %s: %d HP\n":"  %s: %d HP\n",
                combat->turn.current.ptr->pokemons[i].ptr->name,
                combat->turn.current.ptr->pokemons[i].hp);
    }

    printf("Qué quieres hacer?\n\n");
    printf("1. Atacar\t");
    printf("3. Sacar al frente\n");
    printf("2. Mochila\t");
    printf("0. Huír\n");
}

int shouldCloseCombat(struct Combat *combat)
{
    if (combat->shouldClose) return 1;

    if (combat->turn.current.consumed[0] && 
        combat->turn.current.consumed[1] && 
        combat->turn.current.consumed[2] && 
        combat->turn.current.consumed[2]) 

        return 1;
    return 0;
}

void tryToEscape(struct Combat *combat)
{
    printf("%s huyó!\n", combat->turn.current.ptr->name);
    combat->shouldClose = 1;
    combat->winner = combat->turn.enemy.ptr;
    combat->turn.enemy.ptr->wins += 1;
    combat->turn.current.ptr->losses += 1;
}

void showAttackable(struct Combat *combat)
{
    for (int i = 0; i < 4; i++)
    {
        printf("%d. %s: ", i + 1, combat->turn.enemy.ptr->pokemons[i].ptr->name);
        printf(
                combat->turn.enemy.ptr->pokemons[i].hp ? 
                "%d HP\n" : "muerto\n", 
                combat->turn.enemy.ptr->pokemons[i].hp);
    }
    printf("0. Volver\n");
}

void showAttacks(struct Combat *combat)
{
    for (int i = 0; i < 4; i++)
    {
        printf("%d. %s: %d PP\n", i + 1, combat->turn.current.selection->movements[i]->name, 
                combat->turn.current.selection->pps[i]);
    }
    printf("0. Volver\n");
}

PlayerPokemon *getEnemyPokemon(struct Combat *combat, int num)
{
    return combat->turn.enemy.ptr->pokemons + num;
}


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

int isEffective(struct Combat *combat, char *moveType, char *pkmType)
{
    return searchAbilities(combat->maps.effective, moveType, pkmType);
}

int isUneffective(struct Combat *combat, char *moveType, char *pkmType)
{
    return searchAbilities(combat->maps.uneffective, moveType, pkmType);
}

int checkEnemy(struct Combat* combat)
{
    int flag = 0;
    //Se verifica que queden pokemons vivos.
    for(int i = 0; i < 4; i++)
        if (combat->turn.enemy.ptr->pokemons[i].hp)
            flag++;
    //Sino quedan se finaliza el programa.
    if (flag == 0) {
        combat->winner = combat->turn.current.ptr;
        combat->shouldClose = 1;
        return 1;
    }
    return 0;
}

//Verifica si el jugador es apto para realizar un all-out attack.
int canAllOutAttack(struct Combat *combat)
{
    int tmp = 0;
    int alive = 0;
    for (int i = 0 ; i < 4 ; i++)
    {
        if (combat->turn.enemy.knocked[i] == 1 && combat->turn.enemy.ptr->pokemons[i].hp > 0) tmp++;
        if(combat->turn.enemy.ptr->pokemons[i].hp > 0) alive++;
    }    
    if (tmp == alive && alive != 0) return 1;
    return 0;
}

void doAllOutAttack(struct Combat *combat)
{
    int damage = randomNumber(10, 30);
    printf("Todos los pokemons enemigos recibieron %d puntos de daño!.\n\n", damage);
    for (int i = 0 ; i < 4 ; i++)
    {
        if(combat->turn.enemy.ptr->pokemons[i].hp > 0)
        {
            if (combat->turn.enemy.ptr->pokemons[i].hp - damage < 0) 
            {
                combat->turn.enemy.ptr->pokemons[i].hp = 0;
            }
            else
            {
                combat->turn.enemy.ptr->pokemons[i].hp -= damage;
                combat->turn.enemy.knocked[i] = 0;
            }
        }
    }
}

void nextSelection(struct Combat *combat)
{
#ifdef DEBUG
    printf("DEBUG: NextSelection()\n");
#endif
    //Se verifica que el enemigo no haya muerto.
    if (checkEnemy(combat) == 1) return;
    int i = 0;
    int index = combat->turn.current.selectionIndex;
    int found = 0;//comprueba si existen pokemons vivos.
    // Iterar la selección cuatro veces.
    for(i = index; i < 4; i++) {
        if (i > 3) break;

        // si está consumido o no está vivo
        if (combat->turn.current.consumed[i] || 
                combat->turn.current.ptr->pokemons[i].hp == 0 ) continue;
        else {
            found = 1;
            break;
        }
    }
    if (!found)
    {
        //printf("Algo salió mal. No quedan pokémons.\n");
        return;
    } else {
        combat->turn.current.selection = combat->turn.current.ptr->pokemons + i;
        combat->turn.current.selectionIndex = i;
    }
}


// Se ejecuta en caso de un ataque normal o 
// en un ataque inefectivo.
void updateTurn(struct Combat *combat)
{

    if (checkEnemy(combat) == 1) return;
#ifdef DEBUG
    printf("DEBUG: updateTurn()\n");
#endif
    //Auxiliar para hacer el cambio de turnos.
    Player *auxPlayer = combat->turn.current.ptr;
    //El que ataca ahora es el otro jugador.
    combat->turn.current.ptr = combat->turn.enemy.ptr;
    //El que atacaba ahora es el enemigo.
    combat->turn.enemy.ptr = auxPlayer;

    //Se reinician las variables a 0.
    for (int i = 0; i < 4; i++){
        combat->turn.current.consumed[i] = 0;
        combat->turn.current.ptr->pokemons[i].consumed = 0;
        combat->turn.enemy.knocked[i] = 0;
    } 

    //Se inicia en el primer pokemon.
    combat->turn.current.selectionIndex = 0;
    //Se comienza con un pokemon que este vivo.
    while (combat->turn.current.ptr->pokemons[combat->turn.current.selectionIndex].hp == 0) 
    {
        combat->turn.current.selectionIndex++;
    }

    combat->turn.current.selection = combat->turn.current.ptr->pokemons + combat->turn.current.selectionIndex;

    nextSelection(combat);

}

int showBag (struct Combat* combat)
{
    int j = 0;
    for (int i = 0 ; i < 5 ; i++)
    {
        //Si el item no es nulo se muestra.
        if (combat->turn.current.ptr->inventory[i].item != NULL && combat->turn.current.ptr->inventory[i].qty > 0)
        {
            if (j == 0) printf("\nQue item quieres utilizar?\n\n");
            printf("%d. %s \t x %d\n", i+1, combat->turn.current.ptr->inventory[i].item->name, combat->turn.current.ptr->inventory[i].qty);
            j++;
        }
    }
    //No se encontraron items.
    if (j == 0)
    {
        printf("\nNo tienes items disponibles!\n\n");
        return j;
    }
    printf("0. Salir\n");
    return j;
}

int getPkmPos(struct Combat* combat)
{
    int pkm;
    //Se obtiene la posición del pokemon a atacar.
    pkm = checkNum(0, 4) - 1;
    if (pkm+1 == 0) return - 1;

    //Si el pokemon esta muerto se muestra un mensaje por pantalla.
    while (combat->turn.current.ptr->pokemons[pkm].hp == 0)
    {
        printf("%s esta muerto!, seleciona otro pokemon\n", combat->turn.current.ptr->pokemons[pkm].ptr->name);
        pkm = checkNum(0, 4) - 1;
        if (pkm+1 == 0) return - 1;
    }
    return (pkm);
}

void checkBag(struct Combat *combat)
{
    int in;//numero item.
    int pkm;//numero pokemon.
    int item;//habilidad a la que se le aplicará el item.
    pkm = showBag(combat);
    //Si no hay items se finaliza la función.
    if (pkm == 0) return;

    //Se verifica que el item ingresado exista.
    do
    {
        in = checkNum (0, 5) - 1;
        if (in+1 == 0) return;
    }while(combat->turn.current.ptr->inventory[in].item == NULL);

    //Se muestran los pokemons.
    printf("En que pokemon utilizarás el item?\n");
    for(int i = 0; i < 4; i++) printf("%d. %s\t", i+1, combat->turn.current.ptr->pokemons[i].ptr->name);
    printf("\n0. Salir\n");

    reask:

    pkm = getPkmPos(combat);
    if (pkm + 1 == 0) return;

    //Caso en donde el item sea una pocion.
    if (strstr(combat->turn.current.ptr->inventory[in].item->name, "Pocion") != NULL)
    {
        //Caso en que el pokemon este full vida.
        if(combat->turn.current.ptr->pokemons[pkm].hp == combat->turn.current.ptr->pokemons[pkm].ptr->HP)
        {
            printf("El pokemon no puede curarse, selecciona otro.\n");
            goto reask;
        }

        //Se le aplica el item al pokemon
        combat->turn.current.ptr->pokemons[pkm].hp += combat->turn.current.ptr->inventory[in].item->effect;

        //Si al aplicar el item la vida es mayor a la maxima, se deja al maximo solamente.
        if (combat->turn.current.ptr->pokemons[pkm].hp > combat->turn.current.ptr->pokemons[pkm].ptr->HP)
        {
            printf("%s recuperó %d puntos de vida!\n", combat->turn.current.ptr->pokemons[pkm].ptr->name, combat->turn.current.ptr->pokemons[pkm].hp - combat->turn.current.ptr->pokemons[pkm].ptr->HP);
            combat->turn.current.ptr->pokemons[pkm].hp = combat->turn.current.ptr->pokemons[pkm].ptr->HP;
        }
        else printf("%s recuperó %d puntos de vida!\n", combat->turn.current.ptr->pokemons[pkm].ptr->name, combat->turn.current.ptr->inventory[in].item->effect);
    }
    //Caso en que utilice un eter.
    else
    {
        //Se muestran las habilidades del pokemon seleccionado y sus pps.
        printf("En que habilidad quieres utilizar el Eter?\n");
        for(int i = 0; i < 4; i++) printf("%d. %s\t %dpp\n", i+1, combat->turn.current.ptr->pokemons[pkm].movements[i]->name, combat->turn.current.ptr->pokemons[pkm].pps[i]);
        printf("\n0. Salir\n");
        repeat:

        //Se verifica que la habildad ingresada este en el rango.
        item = checkNum (0, 4) - 1;
        if (in+1 == 0) return;

        //Caso en que la habilidad tenga los pps al maximo.
        if(combat->turn.current.ptr->pokemons[pkm].pps[item] == combat->turn.current.ptr->pokemons[pkm].movements[item]->pp) 
        {
            printf("El ataque tiene los pps al maximo, selecciona otro.\n");
            goto repeat;
        }
        //Se aplica el item a la habilidad.
        combat->turn.current.ptr->pokemons[pkm].pps[item] += combat->turn.current.ptr->inventory[in].item->effect;

        //Si al aplicar el item los pps son mayores que los maximos, se dejan al maximo solamente.
        if (combat->turn.current.ptr->pokemons[pkm].pps[item] > combat->turn.current.ptr->pokemons[pkm].movements[item]->pp)
        {
            printf("%s restauró %dpps\n", combat->turn.current.ptr->pokemons[pkm].movements[item]->name, (combat->turn.current.ptr->pokemons[pkm].pps[item] - combat->turn.current.ptr->pokemons[pkm].movements[item]->pp)-1);
            combat->turn.current.ptr->pokemons[pkm].pps[item] = combat->turn.current.ptr->pokemons[pkm].movements[item]->pp;
        }
        else printf("%s restauró %dpps\n", combat->turn.current.ptr->pokemons[pkm].movements[item]->name, combat->turn.current.ptr->inventory[in].item->effect);
    }

    //Se quita una unidad del item utilizado del inventario.
    combat->turn.current.ptr->inventory[in].qty--;
}


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

    combat.shouldClose = 0;

    // Determinar el primer turno.
    int who = randomNumber(0, 1);
    combat.turn.current.ptr = players + who;
    combat.turn.enemy.ptr = players + !(who);

    combat.turn.current.selection = players[who].pokemons;
    combat.turn.current.selectionIndex = 0;

    //Guardar los pps
    for (int i = 0 ; i < 4 ; i++)
    {
        for (int k = 0 ; k < 4 ; k++)
        {
            combat.turn.enemy.ptr->pokemons[i].pps[k] = combat.turn.enemy.ptr->pokemons[i].movements[k]->pp;
            combat.turn.current.ptr->pokemons[i].pps[k] = combat.turn.current.ptr->pokemons[i].movements[k]->pp;
        }
    }

    for (int i = 0; i < 4; i++)
        combat.turn.current.consumed[i] = 0;

    // Nadie ha ganado aún.
    combat.winner = NULL;
    int escape = -1; // Verifica que el jugador no haya huido.
    // mientras el combate no deba acabar.
    while (!(combat.shouldClose))
    {
        showCombatMenu(&combat);
        //Opción del menu.
        int j = checkNum(1, 3);
        //Escapó o no.
        escape = j;
        // Pokemon a atacar.
        int k;
        // Ataque a realizar.
        int l;
#ifdef DEBUG
        printf("DEBUG: menú combate j = %d\n", j);
#endif
        switch (j)
        {
            case 0: // Huir
                tryToEscape(&combat);
                break;
            case 1: // Atacar
            {
                printf("%s está al frente.\n", combat.turn.current.selection->ptr->name);
                printf("Elige tu ataque\n");
                showAttacks(&combat);
repeat:
                l = checkNum(1, 4);
                if (l == 0) continue;
                // Guarda el movimiento a realizar.
                Movement *movement = (combat.turn.current.selection->movements)[l - 1];
                //Verifica que al ataque le queden pps.
                if (combat.turn.current.selection->pps[l-1] == 0)
                {
                    printf("No le quedan pps al ataque. Selecciona otro ataque.\n");
                    goto repeat;
                }
                printf("A quién quieres atacar?\n");
                showAttackable(&combat);
reask:
                k = checkNum(1,4); // Preguntar el pokemon a atacar.
                if (k == 0) continue;// Si es 0 volvemos.

                //Guardamos el pokemon a atacar.
                PlayerPokemon *pkm = getEnemyPokemon(&combat, k - 1); 

                if ( ! (pkm->hp) )// Se verifica que el pokemon a atacar este vivo.
                {
                    printf("%s está muerto. Elige otro objetivo.\n", pkm->ptr->name);
                    goto reask;
                }
                printf("%s usó %s!", combat.turn.current.selection->ptr->name,
                        movement->name);

                getchar();
                putchar('\n');
                //Se restan los pps del ataque utilizado.
                combat.turn.current.selection->pps[l-1] -=1;
                //Si es 1 le toca al siguiente jugador, si es 0 sigue en el mismo turno.
                int flag = 1;
                List *types = pkm->ptr->type; // Lista de tipos de pokemons.
                float damage = movement->damage*0.25; // Se disminuye el daño por temas de balance.
                
                //Se recorre la lista de tipos.
                for (char *type = listFirst(types);
                        type;
                        type = listNext(types)) {
                    // Si es efectivo pasamos al siguiente pokémon,
                    if (isEffective(&combat, movement->type, type) && flag)
                    {
                        printf("Es super efectivo!");
                        damage *= 1.5; // Al ser efectivo se aumenta el daño.  
                        //Si ya estaba noqueado se acaba el turno.
                        if (combat.turn.enemy.knocked[k-1] == 1)
                        {
                            combat.turn.current.selection->consumed = 1;
                            combat.turn.current.consumed[combat.turn.current.selectionIndex] = 1;
                        }
                        //Si no estaba noqueado se muestra un mensaje y sigue el turno con el sgte pokemon.
                        else
                        {
                            flag = 0;
                            combat.turn.enemy.knocked[k-1] = 1;

                            // deshabilitamos el pokemon actual para atacar.
                            combat.turn.current.selection->consumed = 1;
                            combat.turn.current.consumed[combat.turn.current.selectionIndex] = 1;

                            getchar();
                            putchar('\n');
                            printf("%s está noqueado!", pkm->ptr->name);
                            getchar();
                            putchar('\n');
                        }
                        
                    // si no es efectivo, terminamos el turno.
                    } else if (isUneffective(&combat, movement->type, type) && flag){
                        printf("No hizo nada..");
                        damage = 0;
                        flag = 0;
                        updateTurn(&combat);
                        getchar();
                        putchar('\n');
                    }
                }

                //Si el daño fue distinto de 0 se muestra el daño realizado.
                if (damage)
                    printf("%s le quitó %d HP a %s!", combat.turn.current.selection->ptr->name, 
                            (int)damage,
                            pkm->ptr->name);

                getchar();
                putchar('\n');

                //Si el pokemon murió con el ataque.
                if ((pkm->hp - damage) <= 0)
                {
                    printf("%s murió.", pkm->ptr->name);
                    getchar();
                    putchar('\n');
                    pkm->hp = 0;
                }
                //Si el pokemon no murio, se muestra la vida actual. 
                else {
                    pkm->hp -= damage;
                    printf("%s ahora tiene %d HP", pkm->ptr->name, pkm->hp);
                    getchar();
                    putchar('\n');

                }

                //Se verifica que si puede realizar un all-out attcack.
                if (canAllOutAttack(&combat) == 1) 
                {
                    int allOut;
                    printf("Todos los pokemons estan noqueados, es tu oportunidad para realizar un All-out Attack!\n");
                    getchar();
                    printf("1.Si \t 2.No\n\n");
                    //Se verifica si quiere realizar el ataque.
                    allOut = checkNum(1, 2);
                    //Si lo realiza se aplica el daño correspondiente.
                    if (allOut == 1)
                    {     
                        printf("Beat 'em up!\n");
                        getchar();
                        doAllOutAttack(&combat);
                        if (checkEnemy(&combat) == 1);
                        else flag = 1;
                    }
                    //Sino se reinician los noqueos.
                    else 
                    {
                        //Si estamos en el ultimo pokemon no puede seguir atacando.
                        if (combat.turn.current.selectionIndex == 3) flag = 1;
                        for (int i = 0 ; i < 4 ; i++)
                            combat.turn.enemy.knocked[i] = 0;
                    }

                }
                // ataque normal. Terminamos el turno
                if (flag) {
                    updateTurn(&combat);
                    
#ifdef DEBUG
                    printf("DEBUG: Ataque normal\n");
#endif
                }
                //Sino el jugador sigue atacando. 
                else
                    nextSelection(&combat);
                break;
            }
            case 2:
            {
                checkBag(&combat);
            }
        }
    }

    //Si no existe ganador.
    if (!combat.winner)
    {
        printf("Algo salió mal. (El combate terminó y no hay ganador)\n");
    } 
    //Si existe ganador.
    else {
        //Se da dinero al jugador en un rango determinado.
        int money = randomNumber(500,1000);
        printf("El ganador es %s!\n", combat.winner->name);
        printf("Felicidades %s, ganaste $%d!\n", combat.winner->name, money);
        //Se aumenta el dinero y victorias del ganador.
        combat.winner->money += money;
        combat.winner->wins ++;
        if (escape == 0) combat.turn.current.ptr->losses ++; //El jugador huyó.
        else combat.turn.enemy.ptr->losses ++;//El jugador no huyó
        getchar();
    }
    //Se reinician las estadisticas de los pokemons.
    for (int j = 0; j < 2; j++)
        for (int i = 0; i < 4; i++)
        {
            players[j].pokemons[i].hp = players[j].pokemons[i].ptr->HP;
            players[j].pokemons[i].consumed = 0;
            for (int k = 0; k < 4; k++)
                players[j].pokemons[i].pps[k] = players[j].pokemons[i].movements[k]->pp;
        }
}
