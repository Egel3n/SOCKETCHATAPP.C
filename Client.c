#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <stdbool.h>

void error(const char* msg);
void listenAndPrint(int socketFD);
void startListeningAndPrintMessagesOnNewThread(int socketFD);
void SendMessages(int socketFD);
void ReadMessage(int socketFD);
void ReadMessagesWithThread(int socketFD);

int main(int argc , char** argv)
{


	////////////////////////////SOCKET SETTINGS////////////////////////////////////
	int sockfd,portno,n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	

	if(argc < 4){
		fprintf(stderr,"Wrong Usage. %s <IpAddress> <PORT> <Username>",argv[0]);
		exit(1);
	}

	portno = atoi(argv[2]); //string to int

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
		error("Error on Initializing socket");


	server = gethostbyname(argv[1]);
	if(server == NULL)
	{
		fprintf(stderr,"Invalid IP");
	}

	bzero(&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char * )server->h_addr, (char *) &serv_addr.sin_addr.s_addr,server->h_length);
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port =htons(portno);

	if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
		perror("Error on Connecting");

	////////////////////////////SOCKET SETTINGS END////////////////////////////////////

		//for fetching username
		char username[255];
		bzero(username, 255);
		strcpy(username,argv[3]);
		n = write(sockfd,username,strlen(username));
		if(n < 0 )
			perror("Error on Username");


	//LISTENING AND SENDING MESSAGES
	startListeningAndPrintMessagesOnNewThread(sockfd);
	SendMessages(sockfd);

	return 0;
}






void SendMessages(int socketFD)
{
	char buffer[255];
	while(1){
	
	int n;
		bzero(buffer, 255);
		fgets(buffer, 255,stdin);
		n = write(socketFD,buffer,strlen(buffer));
		if(n < 0 )
			perror("Error on writing");

				int i = strncmp("Bye", buffer,3);

		if(i == 0)
			break;

	}
}

void error(const char* msg)
{

	perror(msg);
	exit(1);

}

void startListeningAndPrintMessagesOnNewThread(int socketFD) {

    pthread_t id ;
    pthread_create(&id,NULL,listenAndPrint,socketFD);
}

void listenAndPrint(int socketFD) {
    char buffer[1024];

    while (true)
    {
        ssize_t  amountReceived = recv(socketFD,buffer,1024,0);

        if(amountReceived>0)
        {
            buffer[amountReceived] = 0;
            printf("%s",buffer);
        }

        if(amountReceived==0)
            break;
    }

    close(socketFD);
}
