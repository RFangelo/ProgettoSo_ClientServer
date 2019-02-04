#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/stat.h>
#define MAXSIZE 1024


int main(){
	char IP[16];
	int PORT;
	printf("Inserire indirizzo ip server...");
	scanf("%15[0123456789.]c",IP);
	printf("Inserire PORTA server...");
	scanf("%d",&PORT);
	int sockid = socket(AF_INET,SOCK_STREAM,0);
	if (sockid<0) {
		printf("error open stream socket\n");
		exit(1);
	}
	struct sockaddr_in local, server;
	local.sin_family = AF_INET;
	local.sin_port = htons(0);
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sockid,(struct sockaddr *)&local,sizeof local);
	memset(&server,0,sizeof server);
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr(IP);
	printf("In attesa della disponibilita' del server...\n");
	if(connect (sockid,(struct sockaddr *) &server ,sizeof server ) < 0){
		printf("error connect stream socket\n");
		exit(2);
	}
	char username[31];
	char password[31];
	char buf[MAXSIZE];
	char nl = '\n';//New line
	char newPath[MAXSIZE];
	int fd, listaRicevuta = 0, n, v;
	long int nread, dim;
	struct stat statFile;
	while(1){
		printf("username:  ");
		scanf("%s",username);	
		printf("password:  ");
		scanf("%s",password);
		write(sockid,username,sizeof(char)*30);
		write(sockid,password,sizeof(char)*30);
		read(sockid,buf,sizeof(char));
		if(buf[0] == 'y')
			break;
		printf("Username o password errati\n");
		printf("Client login fault\n");
	}
	printf("Login effettuato con successo\n");
	for(int i = 0; i < 100 ; i++)
		printf("\n");
	int op = 0;
	while(op != 9){
		printf("9-Uscire\n0-Ricevi/Stampa lista FILE server\n1-Ricevi FILE\n2-Invia FILE\nInserire valore... ");
		scanf("%d",&op);
		switch(op){
		case 9://exit
			write(sockid,&op,sizeof(int));
			break;
		case 0://recv list
			write(sockid,&op,sizeof(int));
			if(listaRicevuta == 0){
				listaRicevuta++;
				fd = open("list.txt", O_CREAT | O_RDWR, 0644);
				for(read(sockid,&dim,sizeof(long int));dim != 0;read(sockid,&dim,sizeof(long int))){
					read(sockid,buf,sizeof(char)*dim);
					write(fd,buf,sizeof(char)*dim);
					write(fd,&nl,sizeof(char));
				}
				printf("Lista ricevuta...\n");
				lseek(fd,0L,SEEK_SET);
			}
			else
				fd = open("list.txt", O_RDWR, 0);
			for(;read(fd,buf,sizeof(char)) != 0;write(1,buf,sizeof(char))){}
			printf("\n\n\n");
			close(fd);
			break;
		case 1://RCV file
			printf("Inserisci il percorso del file nel server...");
			scanf("%s",buf);
			printf("Inserisci il percorso del nuovo file...");
			scanf("%s", newPath);
			if (write(sockid,&op,sizeof(int))<0) {
				printf("error write socket\n");
				exit(3);
			}
			if(send(sockid,buf,MAXSIZE,0)<0)//invia path
				printf("error in write\n");
			if(recv(sockid,buf,MAXSIZE,0)<0){//ricevo dimfile
				printf("error in recv\n");
				exit(3);
			}
			fd = open(newPath,O_CREAT | O_RDWR,0644);
			n = 0;
			nread = 0;
			dim = atol(buf);
			v = MAXSIZE;
			printf("dim file recived,is %ld Byte\n",dim);
			if(dim < v)
				v = dim;
			while(n = read(sockid,buf,v)>0){
				
				nread += n*v;
				write(fd,buf,n*v);
				if(dim-nread <v)
					v = dim-nread;
				if(n == 0)
					break; 
			}
			printf("File OK....Completed\n");
			break;
		case 2://SEND file
			write(sockid,&op,sizeof(int));
			printf("Invio file...\nInserire path del file da inviare...");
			scanf("%s", newPath);
			printf("Inserire il nome del file da dare sul server...");
			scanf("%s", buf);
			n = strlen(buf);
			write(sockid,&n,sizeof(int));
			write(sockid,buf,n*sizeof(char));
			fd = open(newPath,O_RDWR,0);
			if (fd < 0){
				printf("error in open\n");
				exit(4);
			}
			printf("file opened\n");
			n = 0;
			nread = 0;
			if(fstat(fd,&statFile)<0)
			{
				printf("error in fstat\n");
				exit(4);
			}
			nread =statFile.st_size;
			printf("dim file readed, it's %ld\n",nread);
			sprintf(buf,"%ld",nread);
			write(sockid,&nread,sizeof(long int));
			printf("dim file sended\n");
			close(fd);
			fd = open(newPath,O_RDWR,0);
			while(n = read(fd,buf,MAXSIZE)>0){
				if(n == 0)
					break;
				write(sockid,buf,n*MAXSIZE);
			}
			printf("file SENDED\n");
			break;
		default:
			printf("Input non valido\n");
			break;
		}
	}
	close(sockid);
	printf("Terminazione\n");
}
















