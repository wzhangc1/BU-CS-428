#include <iostream>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <time.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#include <chrono>
#include <poll.h>

#define PORT	12000

int main()
{
	char buffer[1024];
	int sockfd, n;
	socklen_t len;
	struct sockaddr_in servaddr;
	
	int count;
	struct pollfd fd;
	time_t start, end;
	
	// Create a UDP socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	memset(&servaddr, 0, sizeof(servaddr));
	
	// Fill server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; // localhost
	servaddr.sin_port = htons(PORT); // port number
	
	buffer[0] = '\0';
	
	//Initialize fd
	fd.events = POLLIN; //Data other than high-priority data may be read without blocking.
	fd.fd = sockfd; //File descriptor
	
    for(int i = 0; i < 10; i++)
    {
		std::chrono::duration<double> duration;
		
		printf("%d. ", i);
		
		//Set time pointing to the start time
		auto start = std::chrono::system_clock::now();
		
		//Send the client packet along with the address it is coming from
		sendto(sockfd, (const char *)buffer, strlen(buffer), 
			MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
		
		//Wait for 1000ms on pollfd
		if (poll(&fd, 1, 1000))
		{
			//Receive the client packet along with the address it is coming from
			n = recvfrom(sockfd, (char *)buffer, sizeof(buffer), 
				MSG_WAITALL, ( struct sockaddr *) &servaddr, &len);
			
			//Set time pointing to the end time
			auto end = std::chrono::system_clock::now();
			
			buffer[n] = '\0';
			
			//Calculate round trip time
			duration = end - start;
			
			printf("RTT: %.6fs\n", duration);
		}
		else
		{
			printf("Packet Lost\n");
		}
	}
	
	return 0; 
}
