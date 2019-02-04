

typedef struct struct_node{
	char *name;
	struct struct_node* next;
}node;/*struttura che ha come membri un puntatore a stringa ed un puntatore al nodo successivo*/

typedef struct struct_node *list;/*puntatore a node*/

int isEmpty(list *i);/*Verivica se la lista è vuota*/

void list_init(list *i);/*Inizializza una lista a null*/

void push(list *i, char *name);/*dato un puntatore a list e un puntatore a stringa, aggiunge come primo elemento a list un node che ha come membro name la stringa passata come parametro*/

char * pop(list *i);/*dato un puntatore a list, rimuove il primo elemento, ne libera la memoria e restituisce un puntatore ad una stringa contenente il campo name del nodo prima liberato*/

#include "list.c"






