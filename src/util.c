#include "util.h"
#include <string.h>
#include <ctype.h>

#ifndef _WIN32
    char * _strdup(const char * str) {
        char * aux = (char *)malloc(strlen(str) + 1);
        strcpy(aux, str);
        return aux;
    }
#endif

//Crea la lista de generos y guarda los mismos al detectarlos
List *strToList(const char *string, const char* delim)
{
    List *list = listCreate();

    char *aux = _strdup(string);
    char *token = strtok(aux, delim);
    while (token != NULL)
    {
        listPushBack(list, token);
        token = strtok(NULL, delim);
    }  
     
    return list;
}

//Función utilizada para los mapas
int lower_than_string(void* key1, void* key2){
    char* k1=(char*) key1;
    char* k2=(char*) key2;
    if(strcmp(k1,k2)<0) return 1;
    return 0;
}

//Convierte todos los caracteres a minusculas.
char* stringToLower(char* words)
{
    int i = 0;
    while (words[i] != '\0')
    {
        words[i] = tolower(words[i]);
        i++;
    }
    return words;
}

//Función que comprueba si hay que eliminar un caracter.
int hayQueEliminar(char c, char* string_chars)
{
    for(int i=0 ; i<strlen(string_chars) ; i++)
    {
        if(string_chars[i]==c) return 1;
    }
    return 0;
}

#include <stdio.h>

//Función que quita los caracteres excluidos.
char* quitar_caracteres(char* string, char* c)
{
    int j;
    int i;
    for(i = 0; i < strlen(string); i++ ) 
    {

        if (hayQueEliminar(string[i], c)) 
        {
            for(j = i; j < strlen(string); j++ ) 
            {
                string[j] = string[j+1];
            }
            i--;
        }

    }
    string[i] = '\0';
    return string;
}

//Función utilizada para el HashMap.
int is_equal(void* key1, void* key2)
{
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}

//Muestra el contenido de una lista.
void showList (List* list)
{
    char* id = listFirst(list);

    while (id != NULL)
    {
        printf("%s\n", id);
        id = listNext(list);
    }     
}

//Obtiene la posición del jugador
int getPlayerPos()
{
    int j;
    printf("Igrese numero del jugador(1/2).\n");
    do
    {
        scanf("%d", &j);
        getchar();
    } while (j < 1 || j > 2);
    return j;
}

//Comprueba que el numero este en un determinado rango
int checkNum(int min, int max)
{
    int tmp;
    do
    {
        scanf("%d", &tmp);
        getchar();
    } while (tmp < min || tmp > max);
    return tmp;
}