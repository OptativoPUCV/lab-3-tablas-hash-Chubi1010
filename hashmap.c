#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    Pair* nuevoPar = createPair(key, value);
    long posicion = hash(key, map->capacity);
    Pair* aux = map->buckets[posicion];
    if (aux == NULL) 
    {
        map->buckets[posicion] = nuevoPar;
        map->size++;
    } 
    else 
    {
        while (map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL) 
        {
            posicion = (posicion + 1) % map->capacity;
        }
        if (map->buckets[posicion] == NULL) 
        {
            map->buckets[posicion] = nuevoPar;
            map->size++;
        } 
        else 
        {
            free(map->buckets[posicion]);
            map->buckets[posicion] = nuevoPar;
        }
    }
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) {
    HashMap * nuevo = (HashMap *)malloc(sizeof(HashMap));
    nuevo->buckets = (Pair **)calloc(sizeof(Pair *), capacity);
    if (nuevo->buckets == NULL) return NULL;
    nuevo->size = 0;
    nuevo->capacity = capacity;
    nuevo->current = -1;
    return nuevo;
}

void eraseMap(HashMap * map,  char * key) {    
    long posicion = hash(key, map->capacity);
    long original_pos = posicion;
    
    while (map->buckets[posicion] != NULL) 
    {
        if (map->buckets[posicion]->key != NULL && is_equal(map->buckets[posicion]->key, key)) 
        {
            map->buckets[posicion]->key = NULL;
            map->size--;
            return;
        }
        posicion = (posicion + 1) % map->capacity;
        if (posicion == original_pos) break;
    }
    return;
}

Pair * searchMap(HashMap * map,  char * key) { 
    long posicion = hash(key, map->capacity);
    while (map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL) 
    {
        if (is_equal(map->buckets[posicion]->key, key)) 
        {
            map->current = posicion;
            return map->buckets[posicion];
        }
        posicion = (posicion + 1) % map->capacity;
        if (map->buckets[posicion] == NULL) return NULL;
    }
    return NULL;
}

Pair * firstMap(HashMap * map) {
    for (int i = 0; i < map->capacity; i++) {
        if (map->buckets[i] != NULL) {
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}

Pair * nextMap(HashMap * map) {
    for (int i = map->current + 1; i < map->capacity; i++) {
        if (map->buckets[i] != NULL) {
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}
