#ifndef UTIL_H
#define UTIL_H
#include "list.h"

#ifndef _WIN32
char *_strdup(const char *str);
#endif

/** @defgroup misc Misceláneo */

/** @addtogroup misc
 *  @brief Funciones varias de la tarea 3
 *
 *  Funciones utilizadas que no es posible
 *  catalogar dentro de un tipo de dato
 *  pues se utilizan a lo largo de varios
 *  Tipos de datos distintos.
 *
 *  @{
 */

/** @brief Separa un string por un caracter delimitador
 *
 *  @param string String a cortar
 *  @param delim Delimitador para separar el string
 *  @return Lista enlazada con los strings separados
 */
List *strToList(const char *string, const char* delim);

/** @brief Función de comparación de strings para Mapas.
 *
 * @param key1 Puntero al primer string a comparar
 *
 * @param key2 Puntero al segundo string a comparar
 *
 * @return key1 < key2
 *
 */
int lower_than_string(void* key1, void* key2);

/** @brief Convierte un string a minusculas
 *
 * @param words string a convertir
 *
 * @return puntero a words.
 */
char* stringToLower(char* words);

/** @brief Verifica que dos claves sean iguales
 */
int is_equal(void* key1, void *key2);

/** @} */

void showList (List* list);


int checkNum(int min, int max);

int randomNumber(int lowerbound, int upperbound);


unsigned long djb2hash(unsigned char *str);

#endif
