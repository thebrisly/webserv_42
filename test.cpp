#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>


int main()
{

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);								// creating socket
	struct sockaddr_in servaddr;												// creating address structure
	const int PORT = 8888;														// port number

	if (sockfd < 0)
	{
		std::cerr << "socket creation failed" << std::endl;
		return -1;
	}

	std::cout << "socket created" << std::endl;

	bzero(&servaddr, sizeof(servaddr));											// clearing address structure

	servaddr.sin_family = AF_INET;												// setting address family, always AF_INET for TCP

	std::cout << "address family set = " << AF_INET <<std::endl;				// printing address family	

	servaddr.sin_port = htons(PORT);											// setting port number

	if (bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0) 		// binding socket to address structure
	{ 
		std::cerr << "bind failed" << std::endl;
		return 0; 
	}

	if (listen(sockfd, 3) < 0) 
	{ 
		std::cerr << "listen failed" << std::endl;
		exit(EXIT_FAILURE); 
	}

	// char buffer[1024] = {0};int valread = read( new_socket , buffer, 1024); 
	// printf(“%s\n”,buffer );
	// if(valread < 0)
	// { 
	// 	printf("No bytes are there to read");
	// }char *hello = "Hello from the server";



	return 0;
}