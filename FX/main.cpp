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

class Displayer
{
	static void console_out (const std::string msg, const std::string COLOR)
	{
		std::cout << COLOR << msg << RESET <<std::endl;
	}

	static void console_err (const std::string function)
	{
		std::cerr << RED << "ERROR : ";
		perror(function.c_str());
	}

};


class RunServer
{
	private :
		const int _socket_server;
		std::map<int, Client> _map_client;

		fd_set _readfds;
		fd_set _writefds;
		fd_set _cpy_readfds;
		fd_set _cpy_writefds;
		struct timeval _timeout;

	public :
		RunServer(const int server_init);
		~RunServer();

		void accept_new_connection();
		void recvs_request();
		void send_response();
		void process();
};

RunServer::RunServer(const int socket_server) : _socket_server(socket_server)
{
	FD_ZERO(&this->_writefds);
	FD_ZERO(&this->_readfds);

	FD_ZERO(&this->_cpy_readfds);
	FD_ZERO(&this->_cpy_writefds);


	FD_SET(this->_socket_server, &this->_cpy_readfds);
	

	this->_timeout.tv_sec = 0;
	this->_timeout.tv_usec = 100000;
}

void accept_new_connection(ServerInitializer & server_init, fd_set & cpy_readfds, int & new_socket, std::map<int, Client> & map_clients)
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

void recvs_request (ServerInitializer & server_init, fd_set & cpy_readfds, fd_set & cpy_writefds ,std::map<int, Client> & map_clients, int i)
{
	char buffer[1025];
	int size_read;

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
	else if (size_read == 0) 
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
	}

}

void send_response (int i, std::map<int, Client> & map_clients, fd_set & cpy_writefds)
{

	std::string body = "<!DOCTYPE html><html><body><h1>My First Heading</h1><h2>My first paragraph.</h2><h2>My web server</h2></body></html>"; 
	std::string header = "HTTP/1.1 200 OK\nContent-Type: text/html\nConnection: close\nContent-Length: " + std::to_string(body.length()) + "\r\n\r\n";
	std::string response = header + body;

	if (send(i , response.c_str(), response.length() , 0 ) == -1)
	{
		std::cerr << RED << "ERROR : " << RESET;
		perror("send");
	}
	
	std::cout << GREEN << "Sent response of " << response.length() << " characters to client "<< i << RESET <<std::endl;
	
	if (close (i) < 0)
	{
		std::cerr << RED << "ERROR : " << RESET;
		perror("close");
	}
	std::cout << YELLOW << "Client " << i << " disconected." << RESET <<std::endl;				
	FD_CLR(i, &cpy_writefds);
	map_clients.erase(i);

}


void process (ServerInitializer & server_init, fd_set & cpy_readfds, fd_set & cpy_writefds, fd_set & readfds, fd_set & writefds ,std::map<int, Client> & map_clients)
{
	int i;
	int max_sd;
	struct timeval timeout;
	int new_socket;

	timeout.tv_sec = 0;
	timeout.tv_usec = 100000;

	readfds = cpy_readfds;
	writefds = cpy_writefds;

	int max_key = server_init.get_sock_server();
	max_sd = server_init.get_sock_server();

	for (std::map<int, Client>::iterator it = map_clients.begin(); it != map_clients.end(); ++it)
	{
		if (it->first > max_key)
		{
			max_key = it->first;
		}
	}

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

	if (max_key != max_sd)
	{
		std::cout << RED << "---------ERROR--------" << RESET <<std::endl;
	}


	/* select() Delete from readfds and writefds all the sockets not "ready" for an I/O operation. */
	if (select(max_sd +1 , &readfds, &writefds, NULL, &timeout) < 0) 
	{ 
		std::cerr << RED << "ERROR : " << RESET;
		perror("select");
	} 

	for (i = 0; i < max_sd + 1; i++) 
	{
		if (FD_ISSET(i, &readfds) && i == server_init.get_sock_server()) 
		{
			accept_new_connection(server_init, cpy_readfds, new_socket, map_clients);
		}
		else if (FD_ISSET(i, &readfds)) 
		{
			recvs_request (server_init, cpy_readfds, cpy_writefds , map_clients, i);
		}
		else if (FD_ISSET(i, &writefds))
		{
			send_response(i, map_clients, cpy_writefds);
		}
	}	

}

int main() 
{ 

	fd_set readfds;
	fd_set writefds;

	fd_set cpy_readfds;
	fd_set cpy_writefds;

	FD_ZERO(&writefds);
	FD_ZERO(&readfds);

	FD_ZERO(&cpy_readfds);
	FD_ZERO(&cpy_writefds);

	//std::ofstream outputFile("log.txt");
	
	std::map<int, Client> map_clients;

	try
	{
		ServerInitializer server_init(PORT, SIZE_WAITING_LIST);
		server_init.bind_listen_socket_serv();

		signal(SIGPIPE, sigpipeHandle);

		FD_SET(server_init.get_sock_server(), &cpy_readfds);
		std::cout << MAGENTA << "Listening on socket " << server_init.get_sock_server() << " bind with port " << PORT << "\033[0m" << std::endl;
		
		while(42) 
		{
			process (server_init, cpy_readfds, cpy_writefds, readfds, writefds , map_clients);
		}
	}
	catch(std::exception& e)
	{
		std::cout << "Exception : " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return 0; 
}
