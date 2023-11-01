#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>


int main()
{

	int sock_server;															// creating socket
	int sock_client;
	struct sockaddr_in server_addr;										// creating address structure

	const int addr_len = sizeof(server_addr);

	const int PORT = 8888;														// port number

	sock_server = socket(AF_INET, SOCK_STREAM, 0);

	std::cout << "socket created" << std::endl;

	bzero(&server_addr, sizeof(server_addr));											// clearing address structure

	server_addr.sin_family = AF_INET;										// setting address family, always AF_INET for TCP
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);						// setting address to any interface
	server_addr.sin_port = htons(PORT);

	bind(sock_server,(struct sockaddr *)&server_addr,sizeof(server_addr));	// binding socket to address structure

	listen(sock_server, 30);

	while (42)
	{
		sock_client = accept(sock_server, (struct sockaddr *)&server_addr, (socklen_t*)&addr_len);
		std::cout << "Client connected : " << sock_client << std::endl;

		write(sock_client, "Hello from server", 18);

		char buffer[1024] = {0};

		read(sock_client, buffer, 1024);

		std::cout << "Received message : " << buffer << std::endl;
		
		close(sock_client);
	}
	
	return 0;
}