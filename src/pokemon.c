#include <stdio.h>
#include <string.h>
#include "pokemon.h"
#include "list.h"
#include "hashmap.h"
#include "csvread.h"
#include "util.h"
#include "item.h"

void loadPokemons(HashMap *map)
{
    FILE* pokemon = fopen ("out.csv", "r");
    char linea[1024];
    //Se lee el archivo csv linea por linea.
    while (fgets (linea, 1023, pokemon) != NULL) 
    {
        Pokemon* tmpPkm = malloc(sizeof(Pokemon));
        for (int i = 0 ; i < 4 ; i = i + 1) //Itera 4 veces por linea
        {
            char* aux = get_csv_field(linea, i); // Se guardan los datos en la variable auxiliar
            switch (i)
            {
                case 0://Se guarda el ID
                {
                    tmpPkm->ID = atoi(aux);
                    break;
                }
                case 1://Se guarda el nombre
                {
                    strcpy(tmpPkm->name, aux);
                    break;
                }
                case 2://Se guarda los tipos
                {
                    tmpPkm->type = strToList(aux, ", ");
                    break;
                }
                case 3://Se guarda la vida
                {
                    tmpPkm->HP = atoi(aux);
                    break;
                }
            } // switch  
        } //for
        insertMap(map, tmpPkm->name, tmpPkm);
    } // while
    fclose(pokemon);
}

void loadMovements(HashMap* movementMap, HashMap* movements)
{
    FILE* moves = fopen ("move-data-clean.csv", "r");
    char linea[1024];

    //Se lee el archivo csv linea por linea.
    while (fgets (linea, 1023, moves) != NULL) 
    {
        Movement* tmpMove = malloc(sizeof(Movement));
        for (int i = 1 ; i < 6 ; i = i + 1) //Itera 4 veces por linea
        {
            char* aux = get_csv_field(linea, i); // Se guardan los datos en la variable auxiliar
            switch (i)
            {
                case 0: //Se guarda el ID
                {
                    tmpMove->id = atoi(aux);
                }
                case 1://Se guarda el nombre
                {
                    strcpy(tmpMove->name, aux);
                    break;
                }
                case 2://Se guarda el tipo
                {
                    strcpy(tmpMove->type, aux);
                    break;
                }
                case 3:// Se guardan los power points
                {
                    tmpMove->pp = atoi(aux);
                    break;
                }
                case 4:// Se guarda el daño
                {
                    tmpMove->damage = atoi(aux);
                    break;
                }
                case 5:// Se guarda el % de acierto
                {
                    tmpMove->accuracy = atoi(aux);
                    tmpMove->accuracy /= 100;
                }
            }  
        }
        /* Se busca si existe un mapa del tipo X */
        insertMap(movements, tmpMove->name, tmpMove);
        HashMapPair *pair = searchMap(movementMap, tmpMove->type);
        if(pair == NULL)
        {
            /* si no existe se crea y se añade el movimiento */
            HashMap* tmp = createMap(20);
            insertMap(tmp, tmpMove->name, tmpMove);
            insertMap(movementMap, tmpMove->type, tmp);
        } else {
            /* si existe se añade el movimiento */
            insertMap(pair->value, tmpMove->name, tmpMove);
        }
    }
    fclose(moves);
}

void loadItems(Item* items)
{
    FILE* moves = fopen ("items.csv", "r");
    char linea[1024];
    int i = 0;
    //Se lee el archivo csv linea por linea.
    while (fgets (linea, 1023, moves) != NULL) 
    {
    
        strcpy(items[i].name, get_csv_field(linea, 0));
        items[i].price = atoi(get_csv_field(linea, 1));
        items[i].effect = atoi(get_csv_field(linea, 2));
        i++;
    }
}

// pain
void randomizeMovements(PlayerPokemon *ppk, HashMap* map)
{
    /* Recorremos la lista de los tipos */
    /* j solo se usa para comprobar si estamos en la primera iteracion */
    int j = 0;
    /* l se utiliza cuando el pokemon tiene dos tipos distintos,
     * en este caso se utiliza para saber en que casilla 
     * guardamos el movimiento */
    int l = 0;

    /* mapa que almacena el mapa dentro del mapa de tipos. */
    HashMap* tmp;
    /* numero en el que se guardara el random. Varia entre 
     * 1 y tmp->capacity/2. */
    int num; 
    /* i: string que recorre los tipos
     * j: int que cuenta las iteraciones */
    for(char *i = listFirst(ppk->ptr->type); 
              i != NULL; 
              i = listNext(ppk->ptr->type))
    {
        /* si el siguiente elemento es NULL Y
         * si estamos en el primer elemento */
        tmp = searchMap(map, i)->value;
        if (ppk->ptr->type->current->next == NULL && j == 0)
        {
            for (int k = 0 ; k < 4 ; k++)
            {
            again:
                num = ((rand()  % ((tmp->capacity/2) + 1)));
                while (tmp->buckets[num] == NULL) {

                    if (num > tmp->capacity - 1)
                        num = 0;
                    num++;
                }
                int repetido = 0;
                for (int m  = 0; m < 4; m ++)
                {
                    if (tmp->buckets[num]->value == ppk->movements[m] && ppk->movements[m] != NULL)
                        repetido = 1;
                }

                if (repetido != 0)
                {
                    tmp = searchMap(map, "Normal")->value;
                    goto again;
                }
                ppk->movements[k] = tmp->buckets[num]->value;
            }
            
        } else { // si no
repeat:
            printf("J: %d\n", j);
            printf("L: %d\n", l);
            /* añadimos dos movimientos */
            for (int k = 0 ; k < 2 ; k++)
            {
                num = ((rand()  % ((tmp->capacity/2) + 1)));
                while (tmp->buckets[num] == NULL ) {
                    if (num > tmp->capacity - 1) 
                        num = 0;
                    num++;
                }
                int repetido = 0;
                for (int m  = 0; m < 4; m ++)
                {
                    if (tmp->buckets[num]->value == ppk->movements[m] && ppk->movements[m] != NULL)
                        repetido = 1;
                }

                if (repetido != 0)
                {
                    tmp = searchMap(map, "Normal")->value;
                    goto repeat;
                }
                    ppk->movements[l] = tmp->buckets[num]->value;
                    l++;
                }
        }
        
        j++;
    }
    showPlayerPokemon(ppk);
}

void showPokemon(Pokemon* pokemon)
{
    printf("Nombre : %s\n", pokemon->name);
    printf("Vida: %d\n", pokemon->HP);
    printf("Tipo/s: \n");
    showList(pokemon->type);
    putchar('\n');
}

void showPlayerPokemon(PlayerPokemon *pokemon)
{
    showPokemon(pokemon->ptr);
    printf("Movimientos: \n");
    for(int i = 0; i < 4; i++)
    {
        if ( i % 2)
            printf("  %s\n", pokemon->movements[i]->name);
        else 
            printf("  %s\t", pokemon->movements[i]->name);
    }
}
