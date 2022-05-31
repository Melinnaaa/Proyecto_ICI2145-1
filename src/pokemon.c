#include "pokemon.h"
#include  <stdio.h>
#include "list.h"
#include "hashmap.h"

void loadPokemons()
{
    FILE* pokemon = fopen ("out.csv", "r");
    char linea[1024];
    int largo;

    //Se lee el archivo csv linea por linea.
    while (fgets (linea, 1023, pokemon) != NULL) 
    {
        Pokemon* tmpPkm = malloc(sizeof(Pokemon));
        for (int i = 0 ; i < 4 ; i = i + 1) //Itera 5 veces por linea, ya que una canción solo tiene 5 caracteristicas.
        {
            char* aux = get_csv_field(linea, i); // Se guardan los datos en la variable auxiliar

            switch (i)
            {
                case 0:
                {
                    tmpPkm->ID = atoi(aux);
                    break;
                }
                case 1:
                {
                    strcpy(tmpPkm->name, aux);
                    break;
                }
                case 2:
                {
                    tmpPkm->type = listCreate();
                    printf("%s\n", aux);
                    break;
                }
                case 3:
                {

                    break;
                }
            }  
        }
        
    }
}