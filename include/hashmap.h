//
//  HashMap.h
//  TablaHashLab
//
//  Created by Matias Barrientos on 11-08-18.
//  Copyright © 2018 Matias Barrientos. All rights reserved.
//

#ifndef HashMap_h
#define HashMap_h


typedef struct HashMap HashMap;
typedef struct HashMapSus HashMapSus;

typedef struct HashMapPair {
     char * key;
     void * value;
} HashMapPair;

typedef struct HashMapSusPair {
     char * key;
     int value;
} HashMapSusPair;

struct HashMapSus
{
    HashMapSusPair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

struct HashMap 
{
    HashMapPair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

HashMap * createMap(long capacity);
HashMapSus * createMapSus(long capacity);

void insertMap(HashMap * table, char * key, void * value);
void insertMapSus(HashMapSus * table, char * key, int value);

void eraseMap(HashMap * table, char * key);

HashMapPair * searchMap(HashMap * table, char * key);
HashMapSusPair * searchMapSus(HashMapSus * table, char * key);

HashMapPair * firstMap(HashMap * table);

HashMapPair * nextMap(HashMap * table);

void enlarge(HashMap * map);
void enlargeSus(HashMapSus * map);

#endif /* HashMap_h */
