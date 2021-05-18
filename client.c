#include "header.h"
#include "utils.h"
#include <string.h>
#include <pthread.h>


/**
* To compile: gcc client.c -lpthread -o cli
**/


int sfd;

void *clientListen() {
	unsigned char answer[1024];

	while (1){
		read(sfd, &answer, sizeof(answer));
		printf("-> %s\n", answer);
	}
	
}

int main(int argc, char* argv[]) {
    int j,i;
    unsigned char message[1024];
	int port, guess;
	struct sockaddr_in server_info;
    
	if (argc != 3) {
	    printf("usage: %s ip port\n", argv[0]);
	    return -2;
	}
	
	port = atoi(argv[2]);
	if (port < 5000) {
		printf("%s: The port must be greater than 5000.\n", argv[0]);
		return -3;
	}
	
	if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(argv[1]);
	server_info.sin_port = htons(port);
	if ( connect(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
    printf("Chat inicializado\n");
    
	pthread_t pthread_id;

	pthread_create(&pthread_id,NULL, clientListen,NULL);

    do
	{

		fgets(message, 1024, stdin);
		if ((strlen (message) > 0) && (message[strlen(message)-1] == '\n'))
    		message[strlen(message)-1] = '\0';
		write(sfd, &message, sizeof(message));
		//printf("-> %s\n", message);
		//read(sfd, &answer, sizeof(answer));
		//printf("> %s\n", answer);
	} while (!strcmp(message,"Exit") == 0);
	
	
	
	printf("Sesión de chat finalizada\n");
	close(sfd);
	fflush(NULL);
	return 0;
}
