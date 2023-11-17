#include "ServerInitializer.hpp"
#include "Client.hpp"
#define PORT 8888
#define PORT_2 8008
#define SIZE_WAITING_LIST 30
#define MAX_CLIENT 30
#define READ_M 0
#define WRITE_M 1

#define MAGENTA "\033[1;95m"
#define RED "\033[1;91m"
#define GREEN "\033[1;92m"
#define BLUE "\033[1;94m"
#define YELLOW "\033[1;93m"
#define RESET "\033[0m"

#include <vector>
#include <sys/socket.h>
#include <sys/time.h>
#include <fstream>
#include <signal.h>

class PredicateValueSd
{
	public :
		bool operator()(const Client& client) const
		{
			if (client.get_socket() == 0)
			{
				return false;
			}
			return true;
		}
};

void sigpipeHandle (int sig)
{
	std::cerr << "sigpipeHandle : PROBLEME ECRITURE SOCKET : " << sig <<std::endl;
}

void fd_set_checker (fd_set *set)
{
	for (int i = 0; i < 1024; i++)
	{
		if (FD_ISSET(i, set))
		{
			std::cout << i << " | "; 
		}
		// else
		// {
		// 	std::cout << i << " : 0 ";
		// }
		//std::cout << "|";
	}
}



int main() 
{ 
	int new_socket, i , size_read; 
	int max_sd; 
	char buffer[1025];

	fd_set readfds;
	fd_set writefds;

	fd_set cpy_readfds;
	fd_set cpy_writefds;

	FD_ZERO(&writefds);
	FD_ZERO(&readfds);

	FD_ZERO(&cpy_readfds);
	FD_ZERO(&cpy_writefds);

	std::ofstream outputFile("log.txt");

	struct timeval timeout;

	timeout.tv_sec = 0;
	timeout.tv_usec = 100000;
	
	std::string body = "<!DOCTYPE html><html><body><h1>My First Heading</h1><h2>My first paragraph.</h2><h2>My web server</h2></body></html>"; 
	std::string header = "HTTP/1.1 200 OK\nContent-Type: text/html\nConnection: close\nContent-Length: " + std::to_string(body.length()) + "\r\n\r\n";
	std::string response = header + body;
	

	std::map<int, Client> map_clients;

	//clients_vector.push_back(Client(0));

	// for (i = 0; i < MAX_CLIENT; i++)
	// {
	// 	clients_vector.push_back(Client(0));
	// }

	try
	{

		ServerInitializer server_init(PORT, SIZE_WAITING_LIST);
		server_init.bind_listen_socket_serv();

		signal(SIGPIPE, sigpipeHandle);

		FD_SET(server_init.get_sock_server(), &cpy_readfds);

		std::cout << MAGENTA << "Listening on socket " << server_init.get_sock_server() << " bind with port " << PORT << "\033[0m" << std::endl;
		while(42) 
		{
			readfds = cpy_readfds;
			writefds = cpy_writefds;

			std::cout << "LOOP " <<std::endl;
			for (std::map<int, Client>::iterator it = map_clients.begin(); it != map_clients.end(); ++it)
			{
				std::cout << it->second << std::endl;
			}


			max_sd = server_init.get_sock_server();

			for ( i = 0 ; i < MAX_CLIENT ; i++)
			{
				if (FD_ISSET(i, &readfds) && i > max_sd)
				{
					max_sd = i;
				}
				else if (FD_ISSET(i, &writefds) && i > max_sd)
				{
					max_sd = i;
				}
			}

			/* select() Delete from readfds and writefds all the sockets not "ready" for a I/O operation. */
			if (select(max_sd +1 , &readfds, &writefds, NULL, &timeout) < 0) 
			{ 
				std::cerr << RED << "ERROR : " << RESET;
				perror("select");
			} 

			for (i = 0; i < max_sd + 1; i++) 
			{
				if (FD_ISSET(i, &readfds) && i == server_init.get_sock_server()) 
				{
					if ((new_socket = accept(server_init.get_sock_server(), (struct sockaddr *)&(server_init.get_ref_server_addr()), (socklen_t*)&server_init.get_ref_addrlen()))<0) 
					{ 
						std::cerr << RED << "ERROR : " << RESET;
						perror("accept");
					}
					std::cout << BLUE << "New client connected on socket " << new_socket << " with ip " << inet_ntoa(server_init.get_server_addr().sin_addr) << " on port "<< ntohs (server_init.get_server_addr().sin_port) << RESET << std::endl;
					FD_SET(new_socket, &cpy_readfds);
					map_clients.insert(std::pair<int, Client>(new_socket, Client(new_socket)));
					
				}
				else if (FD_ISSET(i, &readfds)) 
				{
					memset (buffer, 0, 1024);
					size_read = recv(i, buffer, 1024, 0);

					if (size_read == -1)
					{
						if (close(i) < 0)
						{
							std::cerr << RED << "ERROR : " << RESET;
							perror("close");
						}
						FD_CLR(i, &cpy_readfds);
						map_clients.erase(i);

						std::cout << RED << "Error reading from socket" << RESET <<std::endl;
					}
					if (size_read == 0) 
					{ 
						getpeername(i , (struct sockaddr*)(&(server_init.get_ref_server_addr())) , (socklen_t*)&server_init.get_ref_addrlen()); 

						if (close(i) < 0)
						{
							std::cerr << RED << "ERROR : " << RESET;
							perror("close");
						}
						std::cout << YELLOW << "Client " << i << " disconected | IP = " << inet_ntoa(server_init.get_server_addr().sin_addr) << " | PORT = "<< ntohs (server_init.get_server_addr().sin_port) << RESET <<std::endl;
						FD_CLR(i, &cpy_readfds);
						map_clients.erase(i);
					}
					else if (size_read > 0)
					{
						std::cout << GREEN << "Received request of " << size_read << " characters from client " << i << RESET << std::endl;
						FD_CLR(i, &cpy_readfds);
						FD_SET(i, &cpy_writefds);
						map_clients[i].set_request(buffer);
						map_clients[i].set_size_request(size_read);
						map_clients[i].set_socket_mod(READ_M);
						//std::cout << map_clients[i] << std::endl;
					}
				}
				else if (FD_ISSET(i, &writefds))
				{
					int size_send = send(i , response.c_str(), response.length() , 0 );
					if (size_send == -1)
					{
						std::cerr << RED << "ERROR : " << RESET;
						perror("send");
					}
					
					std::cout << GREEN << "Sent response of " << size_send << " characters to client "<< i << RESET <<std::endl;
					
					if (close (i) < 0)
					{
						std::cerr << RED << "ERROR : " << RESET;
						perror("close");
					}
					std::cout << YELLOW << "Client " << i << " disconected." << RESET <<std::endl;				
					FD_CLR(i, &cpy_writefds);
					map_clients.erase(i);
				}
			}
		}
	}
	catch(std::exception& e)
	{
		std::cout << "Exception : " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return 0; 
}
