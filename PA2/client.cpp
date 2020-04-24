#include <iostream>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <time.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT 8888

int main() {

	//Declare socket and connection
	int sockfd, connfd;
	char buffer[1024];
	char exitMsg[1024] = "Bye\n";
	
	struct sockaddr_in servaddr;

	//Create TCP socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	//Congifure server setup
	memset(&servaddr, 0, sizeof(servaddr)); //Initalize address to 0
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost
	servaddr.sin_port = htons(PORT); // port number

	//Establish connection to server
	int connection = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	
	bool valid = true;
	fd_set readfds;
	
	while(valid)
	{
		//Clear set
		FD_ZERO(&readfds);
		//Add a given file descriptor from a set.
		FD_SET(0, &readfds);   
        FD_SET(sockfd, &readfds);
        
        //Cleat buffer
        memset(buffer, 0, sizeof(buffer));
        
        //Monitor multiple file descriptors
		select(sockfd+1, &readfds, NULL, NULL, NULL);
        
        //tests to see if a file descriptor is part of the set
        if (FD_ISSET(0, &readfds))
        {
			//Input from keyboard
			fgets(buffer, 1024, stdin);
			//Send message to socket
			write(sockfd,buffer,strlen(buffer));
		}
		
		//Tests to see if a file descriptor is part of the set
        if (FD_ISSET(sockfd, &readfds))
        {
			//Receive message from socket
			read(sockfd, buffer, 1024);
			printf(buffer);
		}
		
		//Test if it is exit message
		if (strcmp(buffer, exitMsg) == 0)
		{
			//Close socket
			close(sockfd);
			valid = false;
		}
	}
	
	return 0;
}
