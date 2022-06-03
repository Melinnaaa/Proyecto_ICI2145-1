#include <stdio.h>
#include <stdlib.h>

//Función que lee el archivo 
char* get_csv_field (char * tmp, int k) 
{
    int open_mark = 0;
    char* ret = (char*) malloc (100*sizeof(char));
    int ini_i = 0, i = 0;
    int j = 0;

    // tmp: string de llegada
    // k: subcampo a obtener
    // mientras el siguiente caracter no sea nulo
    while(tmp[i+1] !='\0')
    {
        // si el caracter es comilla
        if(tmp[i]== '\"')
        {
            if (k==j) ret[i-ini_i] = 0;
            // da vuelta openMark
            open_mark = 1-open_mark;
            // si es la primer comilla 
            // guardamos en ini_i el inicio despues
            // de la comilla
            if(open_mark) ini_i = i+1;
            // avanzamos
            i++;
            continue;
        }
        // si estamos dentro de una comilla ó
        // la casilla actual NO es una coma 
        if(open_mark || tmp[i]!= ',')
        {
            // si el subcampo a obtener es el que 
            // buscamos
            //      guardamos la letra en la posicion actual menos
            //      la posicion de la comilla 
            if(k==j) ret[i-ini_i] = tmp[i];
            // avanzamos
            i++;
            continue;
        }

        // si estamos en una coma
        if(tmp[i]== ',')
        {
            // si estamos en el subcampo que queremos
            if(k==j) 
            {
               // terminamos el string y lo retornamos 

               ret[i-ini_i] = '\0';
               return ret;
            }

            // avanzamos el subcampo actual (j)
            // avanzamos ini_i una posicion despues de la coma
            

            j++; ini_i = i+1;
        }

        i++;
    }
    //  si estamos en el subcampo que queremos
    if(k==j) 
    {
        // devolvemos el string
       ret[i-ini_i] = 0;
       return ret;
    }

    return NULL;
}
