#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "util.c"

void push(list *i, char *name){
	list t = (list)(malloc(sizeof(node)));
	if(t == NULL){
		perror("Fatal error in malloc...\n");
		exit(0);
	}
	t->name = name;
	t->next = *i;
	*i = t;
}/*dato un puntatore a list e un puntatore a stringa, aggiunge come primo elemento a list un node che ha come membro name la stringa passata come parametro*/

char * pop(list *i){
	if(*i != NULL){
		char * t = salloc((*i)->name);
		list h = *i;
		*i = (*i)->next;
		free(h);
		return t;
	}
	else
		return NULL;
}/*dato un puntatore a list, rimuove il primo elemento, ne libera la memoria e restituisce un puntatore ad una stringa contenente il campo name del nodo prima liberato*/

void list_init(list *i){/*inizializza la lista*/
	*i = NULL;
}

int isEmpty(list *i){/*verifica se la lista è vuota*/
	if(*i == NULL)
		return 1;
	else
		return 0;
}

