#include "include/list.h"
#include "include/util.h"
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
#include <ifaddrs.h>

#define PORT 1234
#define MAXSIZE 1024
#define MAXSERVITI 5





int main(){
	list f[2];
	int len, sdc, fd, n, sd, id_shm, semid, il = 0, v;
	long int nl, dim;
	int op = 0;
	int n_serviti = 0;
	long int nread;
	char buf[MAXSIZE];
	struct stat statFile;
	char * ptrshm;
	char username[31];
	char password[31];
	char username_c[31];
	char password_c[31];
	char pathname[MAXSIZE];
	int pid = fork();
	switch(pid){
	case -1:
		printf("Error fork\n");
		exit(1);
		break;
	case 0:
		execl("fork1","scanHome",NULL);
		break;
	default:
		break;
	}
	list_init(f);
	list_init(&f[1]);
	sleep(1);/*do un po di vantaggio al figlio che chiede al kernel la creazione di shared memory e semaforo*/
	semid = semget((key_t)1902,0,0);
	struct sembuf operazione;
	operazione.sem_num = 0;
	operazione.sem_flg = SEM_UNDO;
	id_shm = shmget((key_t)1902,0,0);
	if(id_shm == -1){
		printf("Error shared memory\n");
		exit(2);
		}
	ptrshm = (char *) shmat(id_shm,0,0);
	for(int i = 0;i == 0;){
		operazione.sem_op = -1;
		semop(semid,&operazione,1);
		if(ptrshm[0] == '\0')
			i++;
		else
			push(f,salloc(ptrshm));
		operazione.sem_op = 1;
		semop(semid,&operazione,1);
	}
	semctl(semid, 0, IPC_RMID);
	shmdt(ptrshm);
	struct sockaddr_in server, client;
	 sd = socket(AF_INET,SOCK_STREAM,0);
	if (sd<0) {
		printf("error open stream socket\n");
		exit(1);
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = 0;//htons(PORT);
	if(bind(sd,(struct sockaddr *)&server,sizeof server)<0){
		printf("Error in bind\n");
		exit(2);
	}
	listen(sd,5);
	len = sizeof client;
	printf("Inserire username (Max 30 caratteri)\nusername:  ");
	scanf("%30s",username);	
	printf("Inserire password (Max 30 caratteri)\npassword:  ");
	scanf("%30s",password);

	struct sockaddr_in temp;
	int temp_len = sizeof(temp);
	getsockname(sd, (struct sockaddr *)&temp,&temp_len);

	for(int i = 0; i < 100 ; i++)
		printf("\n");
	
	while(n_serviti < MAXSERVITI){
		n_serviti++;
		printIP();
		printf("PORT:%d\n",ntohs(temp.sin_port));
		printf("In attesa...\n");
		sdc = accept(sd,(struct sockaddr *)&client,&len);
		if (sdc<0) {
			printf("error accept stream socket\n");
			exit(3);
		}
		printf("Connected with %s\nPort %d\nUsername:%s\tPassword:%s\n", inet_ntoa(client.sin_addr),PORT,username,password);
		
		while(1){
			read(sdc,username_c,sizeof(char)*30);
			read(sdc,password_c,sizeof(char)*30);
			if( strncmp(username,username_c,30) == 0 && strncmp(password,password_c,30) == 0)
				break;
			buf[0] = 'n';
			write(sdc,buf,sizeof(char));
		}
		buf[0] = 'y';
		write(sdc,buf,sizeof(char));
		printf("Client login\n");
		op = 0;
		while(op != 9){
			printf("In ascolto...\n");
			read(sdc,&op,sizeof(int));
			switch(op){
			case 9://exit
				printf("Client disconnesso\n");
				break;
			case 0://send list
				printf("Invio lista file HOME...\n");
				nl = 0;
				for(char *t = pop(&f[il]);t != NULL;t = pop(&f[il])){
					nl = strlen(t);
					write(sdc,&nl,sizeof(long int));
					write(sdc,t,sizeof(char)*nl);
					push(&f[!il],t);
				}
				nl = 0;
				write(sdc,&nl,sizeof(long int));
				il = !il;
				printf("Invio completato...\n");
				break;
			case 1://send file
				printf("Invio file...\n");
				if(recv(sdc,pathname,MAXSIZE,0)<0){//ricevo path
					printf("error in recv\n");
					exit(3);
				}	
				printf("%s\n",pathname);
				fd = open(pathname,O_RDWR,0);
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
				if(send(sdc,buf,MAXSIZE,0)<0){//invio dim file
					printf("error in send\n");
					exit(5);
				}
				printf("dim file sended\n");
				close(fd);
				fd = open(pathname,O_RDWR,0);
				while(n = read(fd,buf,MAXSIZE)>0){
					if(n == 0)
						break;
					write(sdc,buf,n*MAXSIZE);
				}
				printf("file SENDED\n");
				break;
			case 2://rcv file
				printf("Ricevo nome file...;");
				read(sdc,&n,sizeof(int));
				read(sdc,pathname,n*sizeof(char));
				read(sdc,&dim,sizeof(long int));
				fd = open(pathname,O_CREAT | O_RDWR,0644);
				n = 0;
				nread = 0;
				v = MAXSIZE;
				printf("dim file recived,is %ld Byte\n",dim);
				if(dim < v)
					v = dim;
				while(n = read(sdc,buf,v)>0){
					nread += n*v;
					write(fd,buf,n*v);
					if(dim-nread <v)
						v = dim-nread;
					if(n == 0)
						break; 
				}
				close(fd);
				printf("File OK....Completed\n");
				recv( sdc,buf,MAXSIZE,  MSG_DONTWAIT );
				break;
			default:
				printf("Input non valido\n");
				break;
			}
		}
		close(sdc);
	}
	printf("Client serviti %d\nDisconnessione e terminazione.\n",MAXSERVITI);
	close(sd);
}


