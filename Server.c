#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>

struct AcceptedSocket {
	int acceptedSocketFD;
	struct sockaddr_in address;
	int error;
};

//Prototypes
struct AcceptedSocket* acceptIncomingConnection(int serverSocketFD);
void startAcceptingIncomingConnections(int serverSocketFD);
void receiveAndPrintIncomingDataOnSeparateThread(struct AcceptedSocket* clientSocket);
void receiveAndPrintIncomingData(int socketFD);
void error(const char* msg);
void sendReceivedMessageToTheOtherClients(char *buffer,int socketFD);
char *get_ip_str(const struct sockaddr *sa, char *s, size_t maxlen);



struct AcceptedSocket acceptedSockets[10];
int acceptedSocketCounter = 0;



int main(int argc, char* argv[])
{


	////////////////////SOCKET SETTINGS//////////////////////////////////

	if(argc < 2) //checking if port number given
	{

		fprintf(stderr,"Port number not provided.");
		exit(1);
	}

	int sockfd,newsockfd,portno,n;
	struct sockaddr_in serv_addr, client_addr;
	socklen_t client_len;

	sockfd = socket(AF_INET,SOCK_STREAM,0);

	if(sockfd < 0) // -1 means error
		error("Error on initialize socket");

	bzero(&serv_addr,sizeof(serv_addr));

	portno = atoi(argv[1]); //string to int

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);


	if(bind(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr))<0)
		error("Error on binding");


	if(listen(sockfd,5)<0)
		error("Error on Listening");
		
	////////////////////SOCKET SETTINGS END//////////////////////////////////
		


	startAcceptingIncomingConnections(sockfd);

	


	return 0;

}





void startAcceptingIncomingConnections(int serverSocketFD){

	while(1)
	{
		struct AcceptedSocket *clientSocket = acceptIncomingConnection(serverSocketFD);
		acceptedSockets[acceptedSocketCounter++] = *clientSocket;

		receiveAndPrintIncomingDataOnSeparateThread(clientSocket);
	}


}

void sendReceivedMessageToTheOtherClients(char *buffer,int socketFD) {

    for(int i = 0 ; i<acceptedSocketCounter ; i++)
        if(acceptedSockets[i].acceptedSocketFD !=socketFD)
        {
            write(acceptedSockets[i].acceptedSocketFD,buffer, strlen(buffer));
        }

}


struct AcceptedSocket* acceptIncomingConnection(int serverSocketFD){

	struct sockaddr_in clientAddress;
	int clientAddressSize = sizeof (struct sockaddr_in);
	int clientSocketFD = accept(serverSocketFD,(struct sockaddr*)&clientAddress,&clientAddressSize);
	

	struct AcceptedSocket* acceptedSocket = malloc(sizeof (struct AcceptedSocket));


	acceptedSocket->acceptedSocketFD = clientSocketFD;
	acceptedSocket->address = clientAddress;

	if(clientSocketFD < 0)
		acceptedSocket->error = clientSocketFD;


	char ipAddr[20];
	printf("Connection Accepted From %s \n",get_ip_str(&clientAddress,ipAddr,20));
	return acceptedSocket;
}


void receiveAndPrintIncomingDataOnSeparateThread(struct AcceptedSocket* clientSocket)
{

	pthread_t t;

	pthread_create(&t,NULL,receiveAndPrintIncomingData,clientSocket->acceptedSocketFD);


}


void receiveAndPrintIncomingData(int socketFD)
{
	int n;
	char buffer[256];
	//for username 
	bzero(buffer, sizeof(buffer));
		n = read(socketFD, buffer, sizeof(buffer));
		if(n<0)
			error("Error on Username");

	char username[10];
	strncpy(username,buffer,strlen(buffer));


	while(true)
	{


		bzero(buffer, sizeof(buffer));
		n = read(socketFD, buffer, sizeof(buffer));
		if(n<0)
			error("Error on Reading");

		if(n==0)
			break;


		printf("%s: %s",username,buffer);

		char messageToSend[266];
		strcpy(messageToSend,username);
		strcat(messageToSend,": ");
		strncat(messageToSend,buffer,strlen(buffer));

		sendReceivedMessageToTheOtherClients(messageToSend,socketFD);
		bzero(buffer,sizeof(buffer));
	}

	close(socketFD);
}









char *get_ip_str(const struct sockaddr *sa, char *s, size_t maxlen)
{
    switch(sa->sa_family) {
        case AF_INET:
            inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr),
                    s, maxlen);
            break;

        case AF_INET6:
            inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr),
                    s, maxlen);
            break;

        default:
            strncpy(s, "Unknown AF", maxlen);
            return NULL;
    }

    return s;
}




void error(const char* msg)
{

	perror(msg);
	exit(1);

}








