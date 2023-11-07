#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <string>
#include <poll.h>

#define MAX_CLIENTS 10


void display_tabfds(struct pollfd *tabfd, int nfds)
{
	std::cout << "nfds = " << nfds << std::endl;

	for (int i = 0; i < nfds; i++)
	{
		std::cout << "fd = " << tabfd[i].fd << " events = " << tabfd[i].events << std::endl;
	}
}


int main()
{

	int sock_server;
	int sock_client;
	struct sockaddr_in server_addr;

	const int addr_len = sizeof(server_addr);

	const int PORT = 8888;

	std::string index_html;

    std::ifstream file("index.html");
    std::ostringstream fileStream;
	fileStream << file.rdbuf();
	index_html = fileStream.str();
	file.close();

	std::string msg = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 315\r\n\r\n";

	msg = msg + index_html;

	sock_server = socket(AF_INET, SOCK_STREAM, 0);

	std::cout << "server socket = " << sock_server << std::endl;

	bzero(&server_addr, sizeof(server_addr));						

	server_addr.sin_family = AF_INET;			
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	bind(sock_server,(struct sockaddr *)&server_addr,sizeof(server_addr));

	listen(sock_server, MAX_CLIENTS);

	struct pollfd fds[MAX_CLIENTS + 1]; // TABLEAU DE structures pollfd {}

	int nfds = 1;

	bzero(&fds, sizeof(fds));


	fds[0].fd = sock_server;
	fds[0].events = POLLIN;

	//display_tabfds(fds, nfds);


	while (42)
	{
		std::cout << "Waiting for clients" << std::endl;

		//display_tabfds(fds, nfds);

		int ret = poll(fds, nfds, -1); // -1 = infini 1 because we have only one fd seted in fds[0].

		//std::cout << "poll ret = " << ret << std::endl;
		if (fds[0].revents & POLLIN)
		{

			sock_client = accept(sock_server, (struct sockaddr *)&server_addr, (socklen_t*)&addr_len);

			std::cout << "Client connected on socket : " << sock_client << std::endl;

			// Ajouter le nouveau client à la liste des clients connectés.
			for (int i = 1; i < MAX_CLIENTS + 1; i++)
			{
				if (fds[i].fd == 0)
				{
					fds[i].fd = sock_client;
					fds[i].events = POLLIN;
					nfds++;

					std::cout << "New client added to list " << nfds <<std::endl;
					
					
					break;
				}
			}
		}
			// Envoyer le message à tous les clients connectés.
		for (int i = 1; i < nfds; i++)
		{
			if (fds[i].revents & POLLIN)
			{
				char buffer[1024] = {0};
				int byte_reads = read(fds[i].fd, buffer, 1024);

				if (byte_reads > 0)
				{
					std::cout << "Client " << fds[i].fd << " : " << buffer << std::endl;
					write(fds[i].fd, msg.c_str(), msg.length());
				}
				else if (byte_reads == 0)
				{
					std::cout << "Client " << fds[i].fd << " has disconnected" << std::endl;
					close(fds[i].fd);
					fds[i].fd = 0;
					nfds--;

				}
			}
		}
	}
	
	close(sock_server);

	return 0;
}
