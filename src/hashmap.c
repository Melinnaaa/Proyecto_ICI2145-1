#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"
#include "util.h"


int enlarge_called=0;


HashMapPair * createHashMapPair( char * key,  void * value) 
{
    HashMapPair * new = (HashMapPair *)malloc(sizeof(HashMapPair));
    new->key = key;
    new->value = value;
    return new;
}

HashMapSusPair * createHashMapSusPair( char * key,  int value) 
{
    HashMapSusPair * new = (HashMapSusPair *)malloc(sizeof(HashMapPair));
    new->key = key;
    new->value = value;
    return new;
}

long hash(char * key, long capacity) 
{
    unsigned long hash = 0;
    char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) 
    {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}


void insertMapSus(HashMapSus * map, char * key, int value) 
{
  long clave = hash(key, map->capacity);
  float porcentaje;
  map->size = map->size + 1;
  porcentaje = ((float)map->size / map->capacity);

  if (porcentaje > 0.70)
  {
    enlargeSus(map);
  }

  while (map->buckets[clave] != NULL && map->buckets[clave]->key != NULL)
  {
    if (is_equal(key, map->buckets[clave]->key) == 1)
    {
        break;
    }
    clave = (clave + 1) % map->capacity;//Se recorre el arrgelo circular
  }
  if (map->buckets[clave] != NULL)
  {
    map->buckets[clave]->key = key;
    map->buckets[clave]->value = value;
  }
  else
  {
    map->buckets[clave] = createHashMapSusPair(key, value);
  }
}

void insertMap(HashMap * map, char * key, void * value) 
{
  long clave = hash(key, map->capacity);
  float porcentaje;
  map->size = map->size + 1;
  porcentaje = ((float)map->size / map->capacity);

  if (porcentaje > 0.70)
  {
    enlarge(map);
  }

  while (map->buckets[clave] != NULL && map->buckets[clave]->key != NULL)
  {
    if (is_equal(key, map->buckets[clave]->key) == 1)
    {
        break;
    }
    clave = (clave + 1) % map->capacity;//Se recorre el arrgelo circular
  }
  if (map->buckets[clave] != NULL)
  {
    map->buckets[clave]->key = key;
    map->buckets[clave]->value = value;
  }
  else
  {
    map->buckets[clave] = createHashMapPair(key, value);
  }
}
/*
void insertMap(HashMap * map, char * key, void * value) {
    long pos = hash(key, map->capacity);
    while(map->buckets[pos] && map->buckets[pos]->key) {
        pos++;
        if(pos == map->capacity) pos = 0;
    }
    map->buckets[pos] = createPair(key, value);
    map->size++;
}

*/
void enlarge(HashMap * map) 
{
  enlarge_called = 1; //no borrar (testing purposes)
  HashMapPair** oldBucket = (HashMapPair**) calloc (map->capacity, sizeof(HashMapPair*));
  for (int i = 0 ; i < map->capacity ; i = i + 1)
  {
    if (map->buckets[i] != NULL && map->buckets[i]->key != NULL)
    {
      oldBucket[i] = createHashMapPair(map->buckets[i]->key, map->buckets[i]->value); 
    }
  }
  map->capacity = map->capacity * 2;
  HashMapPair** newBucket = (HashMapPair**) calloc (map->capacity, sizeof(HashMapPair*));
  map->buckets = newBucket;
  map->size = 0;
  for (int i = 0 ; i < map->capacity/2 ; i = i + 1)
  {
    if (oldBucket[i] != NULL && oldBucket[i]->key != NULL)
    {
      insertMap(map, oldBucket[i]->key, oldBucket[i]->value); 
    }
  }

}

void enlargeSus(HashMapSus * map) 
{
  enlarge_called = 1; //no borrar (testing purposes)
  HashMapSusPair** oldBucket = (HashMapSusPair**) calloc (map->capacity, sizeof(HashMapSusPair*));
  for (int i = 0 ; i < map->capacity ; i = i + 1)
  {
    if (map->buckets[i] != NULL && map->buckets[i]->key != NULL)
    {
      oldBucket[i] = createHashMapSusPair(map->buckets[i]->key, map->buckets[i]->value); 
    }
  }
  map->capacity = map->capacity * 2;
  HashMapSusPair** newBucket = (HashMapSusPair**) calloc (map->capacity, sizeof(HashMapSusPair*));
  map->buckets = newBucket;
  map->size = 0;
  for (int i = 0 ; i < map->capacity/2 ; i = i + 1)
  {
    if (oldBucket[i] != NULL && oldBucket[i]->key != NULL)
    {
      insertMapSus(map, oldBucket[i]->key, oldBucket[i]->value); 
    }
  }

}


HashMap * createMap(long capacity) 
{
    HashMap* tmp = (HashMap*) calloc (capacity, sizeof(HashMap));
    tmp->buckets = (HashMapPair**) calloc (capacity, sizeof(HashMapPair*));
    tmp->capacity = capacity;
    tmp->size = 0;
    tmp->current = -1;
    return tmp;
}

HashMapSus *createMapSus(long capacity) 
{
    HashMapSus* tmp = (HashMapSus*) calloc (capacity, sizeof(HashMap));
    tmp->buckets = (HashMapSusPair**) calloc (capacity, sizeof(HashMapPair*));
    tmp->capacity = capacity;
    tmp->size = 0;
    tmp->current = -1;
    return tmp;
}

void eraseMap(HashMap * map,  char * key) 
{    
    long clave = hash(key, map->capacity);
    while( map->buckets[clave] != NULL && map->buckets[clave]->key != NULL)
    {
      if (is_equal(map->buckets[clave]->key, key) == 1)
      {
        map->buckets[clave]->key = NULL;
        map->current = clave;
        map->size = map->size - 1;
      }
      clave = (clave + 1) % map->capacity;
    }
}

HashMapPair * searchMap(HashMap * map,  char * key) 
{   
    long clave = hash(key, map->capacity);
    while (map->buckets[clave] != NULL && map->buckets[clave]->key != NULL)
    {
      if (is_equal(key, map->buckets[clave]->key) == 1)
      {
        map->current = clave;
        return (map->buckets[clave]);
      }
      clave = (clave + 1) % map->capacity;
    }
    
    return NULL;
}

HashMapSusPair * searchMapSus(HashMapSus * map,  char * key) 
{   
    return (HashMapSusPair*)searchMap((HashMap*)map, key);
}

HashMapPair * firstMap(HashMap * map) 
{
    for (int i = 0 ; i < map->capacity ; i = i + 1)
    {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL)
        {
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}

HashMapPair * nextMap(HashMap * map) 
{
  for (long clave = map->current ; clave < map->capacity ; clave = clave + 1)
  {
    if (map->buckets[clave] != NULL && map->buckets[clave]->key != NULL && clave != map->current)
    {
      map->current = clave;
      return map->buckets[clave];
    }
  }
  return NULL;
}
