
char * get_env(char * name_variable, char ** envp);/*ritorna il una stringa contenente gli stessi caratteri di una variabie d'ambiente di nome name_variable*/

char * salloc(char* string);/*alloco dinamicamente una stringa identica a quella puntata da strng e ne restituisco il puntatore*/

char * strcat2(char* string1, char* string2);/*alloco dinamicamente una stringa composta da "string1"+'/'+"string2"+'\0'e ne restituisco il puntatore*/

void printIP();/*stampa a video le varie interfacce di rete con i relativi indirizzi*/
