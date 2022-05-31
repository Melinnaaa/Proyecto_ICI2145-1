#ifndef LIST_H
#define LIST_H
#include <stdlib.h>


struct ListNode {
    void * data;
    struct ListNode * next;
    struct ListNode * prev;
};
/**
 * @brief Nodo de la lista enlazada.
 *
 * Nodo que contiene punteros al dato,
 * así como punteros al anterior y 
 * posterior elemento de la lista.
 */
typedef struct ListNode ListNode;

/**
 * @brief Lista enlazada 
 *
 * Lista doblemente enlazada 
 */
typedef struct List List;

struct List {
    ListNode * head;
    ListNode * tail;
    ListNode * current;
    size_t length;
};


ListNode * nodeCreate(void * data);
List * listCreate();

void * listFirst(List * list);
void * listNext(List * list);
void * listLast(List * list);
void * listPrev(List * list);

void listPushBack(List * list, void * data);
void listPushFront(List * list, void * data);
void listPushCurrent(List * list, void * data);

void * listPopFront(List * list);
void * listPopBack(List * list);
void * listPopCurrent(List * list);

void listClean(List * list);
#endif
