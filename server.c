#include "header.h"
#include <string.h>
#include <pthread.h>

/**
* To compile: gcc server.c -lpthread -o ser
**/

struct clients{
     int nsfd[1000];
     int i;
 };
struct clients clients;

int totalClients=0;

int filedescriptor=1000;
void *serves_client() {
	int i=clients.i;
    int sock = clients.nsfd[i];
	unsigned char message[1024];

	message[1024]='\0';

	printf("\na:Connection Established: %i\n", clients.i);
	int j = 0;
	while (!strcmp(message,"Exit") == 0)
	{
		
		read(sock, &message, sizeof(message));
		printf("id: %i> %s\n", i, message);
		for (j = 0; j < totalClients; j++)
		{
			printf("\n\n\n\n\n\nJ!=I: %i> \n\n\n\n\n", j!=i);
			if(j!=i){
				printf("Imprimiré algo:\n");
				write(clients.nsfd[j],&message, sizeof(message));
				}
		}
	}

    printf("Connection Lost: id: %i\n", i);
    
	close(sock);
}

void server(char* ip, int port, char* program) {
	int sfd, nsfd, pid,len;
	 
	struct sockaddr_in server_info, client_info;

	if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror(program);
		exit(-1);
	}
	
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(ip);
	server_info.sin_port = htons(port);
	if ( bind(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
		perror(program);
		exit(-1);
	}
	
	//Accept 10 total connections
	pthread_t pthread_id[1000];

	int i = 0;
	
	listen(sfd, 1000);
	while (i<1000) {
		
		
		len = sizeof(client_info);
		if ( (nsfd = accept(sfd, (struct sockaddr *) &client_info, &len)) < 0 ) {
			perror(program);
			exit(-1);
		}

		dup2(nsfd, filedescriptor);

		clients.nsfd[i] = filedescriptor;

		clients.i=i;
		printf("\nCliente de chat numero: %i\n", clients.i);
	
		pthread_create(&pthread_id[i],NULL, serves_client,NULL);

		if(i==999){
			pthread_join(pthread_id[i], NULL);
		}
		
		i++;
		totalClients++;
		filedescriptor--;
	}

	
 

}

int main(int argc, char* argv[]) {
	char ip[15];	
	int port;
	
	if (argc == 3) {
		strcpy(ip, argv[1]);
		port = atoi(argv[2]);
		if (port < 5000) {
        	printf("%s: The port must be greater than 5000.\n", argv[0]);
        	return -3;
        }
	} else{
		printf("usage: %s ip port\n", argv[0]);
		return -2;
	}
	
	server(ip, port, argv[0]);
	fflush(NULL);
	return 0;
}	
		
		
		
		
		

