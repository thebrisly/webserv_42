#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8888

int main()
{

	int sock_client;									// creating socket
	struct sockaddr_in server_addr;						// creating address structure

	const std::string message = "sdfg";	// message to send

	sock_client = socket(AF_INET, SOCK_STREAM, 0);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);

	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	connect(sock_client, (struct sockaddr *)&server_addr, sizeof(server_addr));

	std::cout << "Connected to server" << std::endl;
	
	send(sock_client, message.c_str(), message.length(), 0);


	char tempBuffer[1024] = {0};

	read(sock_client, tempBuffer, 1024);

	std::cout << "Received message : " << tempBuffer << std::endl;






	return 0;
}