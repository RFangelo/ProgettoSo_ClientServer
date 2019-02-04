#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

char * salloc(char* string){
	char* a = (char *)malloc((sizeof(char)*(strlen(string) + 1)));
	if(a == NULL){
	printf("Fatal error in malloc...\n");
	exit(0);
	}
	strcpy(a, string);
	return a;
}/*alloco dinamicamente una stringa identica a quella puntata da strng e ne restituisco il puntatore*/

char * strcat2(char* string1, char* string2){
	int i = strlen(string1);
	int j = strlen(string1);
	char *a = (char *)malloc(sizeof(char)*(i + j + 2));
	
	for( int ii = 0 ; ii<i ; ii++ )
		a[ii]=string1[ii];
	a[i]='/';
	for( int ii = 0 ; ii<j ; ii++ )
		a[ii+1+i]=string2[ii];
	a[i+j+1]='\0';
	return a;
}/*alloco dinamicamente una stringa composta da "string1"+'/'+"string2"+'\0'e ne restituisco il puntatore*/

char * get_env(char * name_variable,char ** envp){
		int k = strlen(name_variable);
		int u;
		for(int i = 0;envp[i]!=NULL;i++)
			for(int j = 0;envp[i][j] == name_variable[j];j++)
				if(j == k-1)
					u = i;
		char* a = (char *)malloc((sizeof(char)*(strlen(envp[u])- k + 1)));
		for(int h = 0;h<(strlen(envp[u])- k);h++){
			a[h] = envp[u][k+1+h];
		}
				
		return a;
	}/*ritorna il una stringa contenente gli stessi caratteri di una variabie d'ambiente di nome name_variable*/

void printIP(){
	struct ifaddrs * addrs, *tmp;
	getifaddrs(&addrs);
	tmp = addrs;
	printf("Lista Indirizzi delle interfacce:\n");
	while(tmp){
		if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET){
        		struct sockaddr_in *pAddr = (struct sockaddr_in *)tmp->ifa_addr;
       			printf("%s: %s\n", tmp->ifa_name, inet_ntoa(pAddr->sin_addr));
    		}

    		tmp = tmp->ifa_next;
	}
	
	freeifaddrs(addrs);
}/*stampa a video le varie interfacce di rete con i relativi indirizzi*/
