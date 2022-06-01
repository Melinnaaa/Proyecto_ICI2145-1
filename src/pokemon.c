#include <stdio.h>
#include <string.h>
#include "pokemon.h"
#include "list.h"
#include "hashmap.h"
#include "csvread.h"
#include "util.h"

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
                    tmpPkm->type = strToList(aux, ",");
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

void loadMovements(HashMap* movementMap)
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

void randomizeMovements(PlayerPokemon *ppk, HashMap* map)
{
    /* Recorremos la lista de los tipos */
    int j = 0;
    HashMap* tmp;
    /* i: string que recorre los tipos
     * j: int que cuenta las iteraciones */
    for(char *i = listFirst(ppk->ptr->type); 
              i != NULL; 
              i = listNext(ppk->ptr->type))
    {
        /* si el siguiente elemento es NULL Y
         * si estamos en el primer elemento */
        if (listNext(ppk->ptr->type) && j == 0)
        {
            for (int k = 0 ; k < 4 ; k++)
            {
                tmp = searchMap(map, i)->value;
                rand()
                searchMap(tmp, );
            }
            
        } else { // si no
            /* añadimos dos movimientos */
        }
        
        j++;
    }
}

void showPokemon(Pokemon* pokemon)
{
    printf("Nombre del pokemon: %s\n", pokemon->name);
    printf("Vida del pokemon: %d\n", pokemon->HP);
    printf("Tipo/s del pokemon: ");
    showList(pokemon->type);
}
