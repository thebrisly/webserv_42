#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <random>
#include <time.h>

#define READ 1
#define WRIT 2

#define PORT 8888

int main()
{
	int		sock_client;
	int		status = WRIT;
	struct	sockaddr_in server_addr;	
	const	std::string message = "Hello from client";
	char	tempBuffer[1024] = {0};

	sock_client = socket(AF_INET, SOCK_STREAM, 0);

	std::cout << "sock_client = " << sock_client <<std::endl;

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int connect_status = connect(sock_client, (struct sockaddr *)&server_addr, sizeof(server_addr));

	std::cout << "Connection status = " << connect_status << std::endl;

	while (42)
	{

		if (status == WRIT)
		{
			srand(time(NULL));
			int random_number = std::rand() % 10;
			int send_status;

			if (random_number == 5)
			{
				send_status = send(sock_client, "", 0, 0);
				std::cout << "Send status " << send_status << std::endl;
				status = 0;
				return 0;
			}
			else
			{
				send_status = send(sock_client, message.c_str(), message.length(), 0);
				std::cout << "Send status " << send_status << std::endl;
				status = READ;
			}


			//sleep(1);
		}

		if (status == READ)
		{
			int recv_status = recv(sock_client, tempBuffer, 1024, 0);
			std::cout << "Recv status " << recv_status << std::endl;

			std::cout << "Received message : " << std::endl;
			std::cout << tempBuffer << std::endl;
			status = WRIT;
		}

	}

	//sleep(5);



	// int read_status = read(sock_client, tempBuffer, 1024); //bloc

	// std::cout << "read status " << read_status << std::endl;

	// std::cout << "Received message : " << std::endl;
	
	// std::cout << tempBuffer << std::endl;

	return 0;
}