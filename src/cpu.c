#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include "util.h"
#include "player.h"
#include "csvread.h"

void fileToPlayer(Player* dest, char* name, HashMap* pokemon, HashMap* moves)
{
    char directory[60] = "cache/";
    strcat(directory, name);
//  printf("%s\n", directory);
    struct PlayerExport playerImport;
    FILE *file = fopen(directory, "rb");
        size_t size = 0;
        if (file != NULL){
            fseek(file, 0, SEEK_END);
            size = ftell(file);
        }
    #ifdef DEBUG
            printf("Encontrado archivo %s\n", directory);
    #endif

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


        //printf("Nombre del perfil importado: %s\n", playerImport.name);
        strcpy(dest->name , playerImport.name);

        dest->wins = playerImport.wins;
        dest->losses = playerImport.losses;
        dest->money = playerImport.money;

        List *items = listCreate();

        FILE *items_fd = fopen("items.csv", "rb");


        char linea[1024];
        //Se lee el archivo de items y se pasa a una lista.
        while( (fgets(linea, 1023, items_fd)) != NULL) {
            Item *item = malloc(sizeof(Item));
            strcpy(item->name, get_csv_field(linea, 0));
            //printf("Añadiendo archivo %s\n", item->name );
            item->price = atoi(get_csv_field(linea, 1));
            item->effect = atoi(get_csv_field(linea, 2));
            listPushBack(items, item);
        }

        //Se cargan los items.
        for (int i = 0; i < 5; i++)
        {
            if (playerImport.items[i].qty > 0)
            {
                //printf("NOMBRE IMPORTAR %s\n", playerImport.items[i].name);
                for (Item *item = listFirst(items); item; item = listNext(items))
                {
                    if (strcmp(playerImport.items[i].name, item->name) == 0)
                    {
                        //printf("Encontrado item %s\n", item->name);
                        dest->inventory[i].item = item;
                        dest->inventory[i].qty = playerImport.items[i].qty;
                    }

                }
            }
            //Si no hay se inicializan en null y con cantidad 0.
            else 
            {
                dest->inventory[i].item = NULL;
                playerImport.items[i].qty = 0;
            }
        }

        //Para cada pokemon
        for (int i = 0; i < 4; i++)
        {
            HashMapPair *pair = searchMap(pokemon, playerImport.poke[i].name);
            //Si se encontó el pokemon.
            if (pair)
            {
                //Se guardan todos sus datos.
                Pokemon *tmp = pair->value;
                dest->pokemons[i].ptr = tmp;
                dest->pokemons[i].consumed = 0;
                dest->pokemons[i].hp = tmp->HP;
                //printf("POKEMON: .%s.\n", dest->pokemons[i].ptr->name);
    #ifdef DEBUG
                printf("DEBUG: Encontrado pokemon %s\n", tmp->name);
    #endif
                // para cada movimiento
                for (int j = 0; j < 4; j++)
                {
                    pair = searchMap(moves, playerImport.poke[i].move[j].name);
                    //Si no se encontró el movimiento.
                    if (!pair) 
                    {
                        printf("Error: movimiento %s no encontrado\n", playerImport.poke[i].move[j].name);
                    }
                    //Si se encuentran se asignan al player. 
                    else {
                        dest->pokemons[i].movements[j] = pair->value;
                        dest->pokemons[i].pps[j] = playerImport.poke[i].move[j].pp;
                       // printf("MOVIMIENTO: .%s.\n", dest->pokemons[i].movements[j]->name);
                    }
                }

            } else {
                //printf("Error cargando perfil:\n");
                //printf("%s no fue encontrado en la base de datos\n", playerImport.poke[i].name);
            }
        }
        //Al cargar los datos la cpu ya puede luchar.
        dest->canPlay = 1;
}

//Crea una Cpu.
Player createCPU (HashMap* pokemon, HashMap* moves)
{
    DIR* dirp;
    //Guarda el perfil cargado desde un archivo binario.
    Player cpu;
    //Cantidad de archivos.
    int numArchives = countArchives();
    //Se obtiene un archivo random.
    int random = randomNumber(0, numArchives-1);
    struct dirent * entry;
    //Cuenta los archivos leidos.
    int i = 0;
    //Se abre el directorio.
    dirp = opendir("cache"); 
    //Mientras existan archivos en el directorio.
    while ((entry = readdir(dirp)) != NULL) {
        //No se leen los archivos basura.
        if (strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0 ) continue;
        //Cuando se llega al archivo que buscamos se cierra el ciclo.
        if (i == random)
        {
            break;
        }

#ifdef DEBUG
        printf("%s\n", entry->d_name);
#endif
        i++;
    }

    //Se Carga el archivo en la variable cpu.
    fileToPlayer(&cpu,entry->d_name, pokemon, moves);
    //Se cierra el directorio.
    closedir(dirp);
    return cpu;
}

//Crea una instancia de combate para el modo Cpu v/s jugador.
void initCpuCombat (Player* players, Player* cpu, HashMap* effective, HashMap* uneffective, int* win)
{
    //Si no hay perfiles cargados.
    if (!(players->canPlay) || !(cpu->canPlay))
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

    //El combate no debe cerrarse al iniciar.
    combat.shouldClose = 0;

    // Determinar el primer turno.
    combat.turn.current.ptr = cpu;
    combat.turn.enemy.ptr = players;

    //Se comienza con el primer pokemon del equipo
    combat.turn.current.selection = cpu->pokemons;
    combat.turn.current.selectionIndex = 0;

    //Verifica que el jugador no haya huido.
    int escape;

    //Guardar los pps
    for (int i = 0 ; i < 4 ; i++)
    {
        for (int k = 0 ; k < 4 ; k++)
        {
            combat.turn.enemy.ptr   ->pokemons[i].pps[k] = combat.turn.enemy.ptr    ->pokemons[i].movements[k]->pp;
            combat.turn.current.ptr ->pokemons[i].pps[k] = combat.turn.current.ptr  ->pokemons[i].movements[k]->pp;
        }
    }

    //Se inician los pokemons en 0, ya que no han sido utilizados.
    for (int i = 0; i < 4; i++)
        combat.turn.current.consumed[i] = 0;

    // Nadie ha ganado aún.
    combat.winner = NULL;

    // mientras el combate no deba acabar.
    while (!(combat.shouldClose))
    {
#ifdef DEBUG
        printf("DEBUG: inicio loop combat cpu\n");
        printf("%s\n", cpu->pokemons[0].movements[3]->name);
#endif
        // Opcion del menu.
        int j;
        //Pokemon a atacar. 
        int k;
        //Ataque a realizar.
        int l;
        // Si el jugador actual no es la cpu.
        if(combat.turn.current.ptr != cpu) 
        {
            showCombatMenu(&combat);
            j = checkNum(1, 3);
        }
        // Si es la cpu j = 1 (atacar).
        if(combat.turn.current.ptr == cpu)
        {
            j = 1;
        }
        escape = j;
#ifdef DEBUG
        printf("DEBUG: menú combate j = %d\n", j);
#endif
        switch (j)
        {
            // Huir
            case 0:
                tryToEscape(&combat);
                break;
            // Atacar
            case 1:
            {
                Movement *movement; // guarda el movimiento a realizar
                PlayerPokemon *pkm; // guarda el pokemon atacando
                if (combat.turn.current.ptr != cpu) // si el jugador no es la cpu
                {
                    printf("%s está al frente.\n", combat.turn.current.selection->ptr->name);
                    printf("Elige tu ataque\n");
                    showAttacks(&combat);
repeat:
                    l = checkNum(1, 4); //  preguntar movimiento
                    if (l == 0) continue; // si se elige el 0 volvemos
                    movement = (combat.turn.current.selection->movements)[l - 1]; // se guarda el movimiento realizado
                    if (combat.turn.current.selection->pps[l-1] == 0) // se verifica que el movimiento es realizable
                    {
                        printf("No le quedan pps al ataque. Selecciona otro ataque.\n");
                        goto repeat; // volver a preguntar
                    }
                    printf("A quién quieres atacar?\n");
                    showAttackable(&combat);
reask:
                    k = checkNum(1,4); // preguntar el pokemon a atacar
                    if (k == 0) continue; // si es 0 volvemos

                    pkm = getEnemyPokemon(&combat, k - 1); // guardamos el pokemon enemigo a atacar

                    if ( ! (pkm->hp) ) // vemos que este vivo
                    {
                        printf("%s está muerto. Elige otro objetivo.\n", pkm->ptr->name);
                        goto reask;
                    }
                }
                else // si somos la cpu
                {
                    printf("Turno de %s:\n\n", combat.turn.current.ptr->name);
                    //Se busca un ataques con pps > 0.
                    do
                    {
                        l = randomNumber(1,4); // Elegimos un movimiento aleatorio.
                        movement = combat.turn.current.selection->movements[l-1];
                    }while (combat.turn.current.selection->pps[l-1] == 0);

                    //Se busca un pokemon con hp > 0.
                    do
                    {
                        k = randomNumber(1,4);// Elegimos un pokemon aleatorio.
                        pkm = getEnemyPokemon(&combat, k-1);// Se obtiene el pokemon a atacar.
                    } while (pkm->hp == 0);
                }
#ifdef DEBUG
                if (combat.turn.current.selection->ptr == NULL) {
                    printf("Error: no  hay selección\n");
                } else {
                    printf("Selección: %s\n", combat.turn.current.selection->ptr->name);
                }

                if (!movement) {
                    printf("Error: no hay movimiento\n");
                    printf("Aunque la cpu eligió el mov. %d\n", l - 1 );
                }
#endif
                printf("%s usó %s!", combat.turn.current.selection->ptr->name,
                        movement->name);

                getchar();
                putchar('\n');
                //Se restan los pps del ataque utilizado.
                combat.turn.current.selection->pps[l-1] -=1;
                //Si es 1 le toca al siguiente jugador, si es 0 sigue en el mismo turno.
                int flag = 1;
                List *types = pkm->ptr->type;//Lista de tipos de pokemon.
                float damage = movement->damage*0.25;// El daño se disminuye por temas de balance.

                //Se recorre la lista de tipos.
                for (char *type = listFirst(types);
                        type;
                        type = listNext(types)) {
                    // Si es efectivo pasamos al siguiente pokémon.
                    if (isEffective(&combat, movement->type, type) && flag)
                    {
                        printf("Es super efectivo!");
                        damage *= 1.5;//Si es efectivo se aumenta el daño.
                        //Si ya estaba noqueado se acaba el turno.
                        if (combat.turn.enemy.knocked[k-1] == 1)
                        {
                            combat.turn.current.selection->consumed = 1;
                            combat.turn.current.consumed[combat.turn.current.selectionIndex] = 1;
                        }
                        //Si no estaba noqueado se muestra un mensaje y sigue el turno con el sgte pokemon.
                        else
                        {
                            flag = 0;//puede seguir atacando.
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
                        
                    // Si no es efectivo, terminamos el turno.
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
                {
                    printf("%s le quitó %d HP a %s!", combat.turn.current.selection->ptr->name, 
                            (int)damage,
                            pkm->ptr->name);
                    getchar();
                    putchar('\n');
                }


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
                    //Si no es la cpu se muestran mensajes.
                    if(combat.turn.current.ptr != cpu)
                    {
                        printf("Todos los pokemons estan noqueados, es tu oportunidad para realizar un All-out Attack!\n");
                        getchar();
                        printf("1.Si \t 2.No\n\n");
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
                    //La cpu siempre realizará all-out attack
                    else
                    {
                        doAllOutAttack(&combat);
                        if (checkEnemy(&combat) == 1);
                        else flag = 1;
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
                } else//Sino el jugador sigue atacando.
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
        
        printf("El ganador es %s!\n", combat.winner->name);
        if (combat.winner != cpu)
        {
            //Se da dinero al jugador en un rango determinado.
            int money = randomNumber(500,1000);
            printf("Felicidades, ganaste $%d!\n", money);
            //Se aumenta el dinero, victorias del ganador
            combat.winner->money += money;
            combat.winner->wins ++;
            *win = *win + 1;
        }
        else *win = -1;
        if (escape == 0) combat.turn.current.ptr->losses ++; //El jugador huyó.
        else combat.turn.enemy.ptr->losses ++;//El jugador no huyó
        getchar();
    }
    //Se reinician las estadisticas de los pokemons del usuario.
    for (int j = 0; j < 2; j++)
        for (int i = 0; i < 4; i++)
        {
            players->pokemons[i].hp = players->pokemons[i].ptr->HP;
            players->pokemons[i].consumed = 0;
            for (int k = 0; k < 4; k++)
                players->pokemons[i].pps[k] = players->pokemons[i].movements[k]->pp;
        }
}
