#include <stdio.h>
#include <stdlib.h>

//Función que lee el archivo 
char* get_csv_field (char * tmp, int k) 
{
    int open_mark = 0;
    char* ret = (char*) malloc (100*sizeof(char));
    int ini_i = 0, i = 0;
    int j = 0;

    while(tmp[i+1] !='\0')
    {
        if(tmp[i]== '\"')
        {
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1;
            i++;
            continue;
        }
        if(open_mark || tmp[i]!= ',')
        {
            if(k==j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }
        if(tmp[i]== ',')
        {
            if(k==j) 
            {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i+1;
        }

        i++;
    }
    if(k==j) 
    {
       ret[i-ini_i] = 0;
       return ret;
    }

    return NULL;
}