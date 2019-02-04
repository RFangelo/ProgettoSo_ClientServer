# ProgettoSo_ClientServer
Send and receive files between server and client
Progetto di Aglieco Angelo, Università degli studi di Catania, Corso di laurea in Ingegneria Informatica.
Progetto di Sistemi Operativi-Lato Server
----------util.c------------------
Libreria con varie funzioni utili


char * salloc(char* string)	/*alloco dinamicamente una stringa identica a quella puntata da strng e ne restituisco il puntatore*/
char * strcat2(char* string1, char* string2)	/*alloco dinamicamente una stringa composta da "string1"+'/'+"string2"+'\0'e ne restituisco il puntatore*/
char * get_env(char * name_variable,char ** envp)	/*ritorna il una stringa contenente gli stessi caratteri di una variabie d'ambiente di nome name_variable*/
void printIP();/*stampa a video le varie interfacce di rete con i relativi indirizzi*/
----------list.c-------------------
Implementazione di una pila di strighe, che andremo ad utilizzare per la scansione di una qualsiasi directory


typedef struct struct_node{...}node;/*struttura che ha come membri un puntatore a stringa ed un puntatore al nodo successivo*/

typedef struct struct_node *list;/*puntatore a node*/

int isEmpty(list *i);/*Verivica se la lista è vuota*/

void list_init(list *i);/*Inizializza una lista a null*/

void push(list *i, char *name);/*dato un puntatore a list e un puntatore a stringa, aggiunge come primo elemento a list un node che ha come membro name la stringa passata come parametro*/

char * pop(list *i);/*dato un puntatore a list, rimuove il primo elemento, ne libera la memoria e restituisce un puntatore ad una stringa contenente il campo name del nodo prima liberato*/
----------fork1.c------------------
Come variabili globali abbiamo due pile, variabili per il controllo sui thread e vari mutex per la mutua esclusione di tali variabili.

int main(...);/*Inizialmente componiamo una stringa del tipo "/home/user/Scrivania" ed inizializziamo liste e mutex.
Dopo di che il main thread apre "/home/user/Scrivania" e la scansiona (escludendo "." e "..") caricando le varie stringhe nella lista f se è un file, d se directory. Quindi chiudo la directory "/home/user/Scrivania" ed creo due thread che eseguono la funzione "thread_function(...);". A questo punto il main thread aspetta che i thread finiscano di scansionare tutte le sottodirectory, ciò avviene quando il contatore dei thread terminati è == NCORE e la lista di directory è vuota.
Fatto ciò, viene richiesto al kernel una zona di shared memory, ed un semaforo per passare al processo padre la lista di file scansionati. */

void *thread_function(...);/*Incomincia a leggere directory dalla lista d (inizializzata e parzialmente riempita dal main thread), e la scansiona caricando file e directory nelle rispettive liste, tutto questo ottenendo mutua esclusione con mutex.Tutto continua finche pop non riceve null, questo perchè la lista directory è vuota. Infatti è vuota se nell'ultimo ciclo i vari thread non hanno aggiunto directory, ciò implica che abbiamo raggiunto le estremità dell'albero...*/
-----------main.c-------------------
Codice del processo server:

int main(...);/*Dichiaro tutte le variabili principali che andro ad utilizzare, eseguo una fork, facendo eseguire al figlio il codice di "fork1.c", fatto ciò il padre dorme per 1 s affinche il figlio esegua la scansione a partire dalla Directory "/home/user/Scrivania" ed inizializza shared memory e semaforo, così da passare il risultato della sua scansione, che il padre salvera in una pila. Il figlio termina dopo questo scambio di dati, lasciando il padre che configura socket TCP con le relative funzioni. Vengono stampati a video vari indirizzi delle interfacce di rete e la relativa porta sulla quale il processo si mette in ascolto.
Viene richiesto di impostare un username ed una password, che verrà richista al client per potere richiedere i vari servizi che elencheremo in seguito.
Nel caso in cui venga effettuato un login con successo, il server potrà :
-Disconnettere il client ed accettare eventuali nuove richiste
-Inviare la lista dei file su "/home/user/Scrivania", non avendo implementato un iteratore per la pila procedo svuotandone una così da leggere il contenuto che invio al client e riempendo una seconda pila, alternando continuamente f[0],f[1];
-Inviare un file, ricevendo inizialmente il path ed inviando la dim del relativo file che subito dopo invio
-Ricevere file che salverà col nome che verrà passato dal client  
Dopo 5 connessioni il server termina*/
----------------------------------------------------------------------------------------------------------------------------------------------
Progetto di Sistemi Operativi-Lato Client
----------main.c------------------
int main(...);/*Leggo da tastiera indirizzo ip e porta sulla quale il server è in ascolto, se il server è occupato nella risoluzione di altre richieste rimango in attesa, altrimenti viene richiesto username e password impostati dal server in precedenza.
A questo punto viene letto da tastiera un intero tra 9-0-1-2 che corrispondono a:
9-Uscire
0-Ricevi/Stampa lista FILE server
1-Ricevi FILE
2-Invia FILE
Dopo la lettura dell' intero, quest'ultimo vien inviato al server, richiedendo così il servizio*/
--------Considerazioni finali------------------------------------------------------------------------------------------------------------------
Lo sviluppo del progetto ha portato ad un arricchimento delle mie capacità, infatti è il primo software da me progettato che possa essere definito un programma completo con utilità concreta. Infatti solitamente scrivevo programmi dove veniva simulata una qualche situazione, e questi programmi servivano solo allo scopo di esercitazione. Questa volta è stato diverso, mettere in pratica gran parte dei servizi che il SO ha da offrire. Le maggiori difficolta riscontrate sono state:
-La scansione delle directory, risolto dedicando una pila per le directory ed una per i file.
-Capire in che modo inviare ed in che dimensione scrivere e leggere dal socket i segmenti di file. Ho risolto leggendo campi di 1024 byte finchè n di byte rimanenti da leggere sono < di 1024, a quel  punto leggo il num esatto di byte rimanenti. 
Eventuali implementazioni future saranno una versione (lato client) per android, ed una GUI.
