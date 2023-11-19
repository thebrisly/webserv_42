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

void sigpipeHandle (int sig)
{
	std::cerr << "sigpipeHandle : PROBLEME ECRITURE SOCKET : " << sig <<std::endl;
}

class RunServer
{
	private :
		ServerInitializer& _server_init;
		std::map<int, Client> _map_clients;

		fd_set			_readfds;
		fd_set			_writefds;
		fd_set			_cpy_readfds;
		fd_set			_cpy_writefds;
		struct timeval	_timeout;

	public :
		RunServer(ServerInitializer& server_init);
		~RunServer();

		void accept_new_connection();
		void recvs_request(int i);
		void send_response(int i);
		void process();
};

RunServer::RunServer(ServerInitializer& server_init) : _server_init(server_init)
{
	FD_ZERO(&this->_writefds);
	FD_ZERO(&this->_readfds);

	FD_ZERO(&this->_cpy_readfds);
	FD_ZERO(&this->_cpy_writefds);

	FD_SET(this->_server_init.get_sock_server(), &this->_cpy_readfds);
	
	this->_timeout.tv_sec = 0;
	this->_timeout.tv_usec = 100000;
}

RunServer::~RunServer(){}

void RunServer::accept_new_connection()
{
	int new_socket;

	if ((new_socket = accept(this->_server_init.get_sock_server(), (struct sockaddr *)&(this->_server_init.get_ref_server_addr()), (socklen_t*)&this->_server_init.get_ref_addrlen()))<0) 
	{ 
		std::cerr << RED << "ERROR : " << RESET;
		perror("accept");
	}
	std::cout << BLUE << "New client connected on socket " << new_socket << " with ip " << inet_ntoa(this->_server_init.get_server_addr().sin_addr) << " on port "<< ntohs (this->_server_init.get_server_addr().sin_port) << RESET << std::endl;
	FD_SET(new_socket, &this->_cpy_readfds);
	this->_map_clients.insert(std::pair<int, Client>(new_socket, Client(new_socket)));				
}

void RunServer::recvs_request (int i)
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
		FD_CLR(i, &this->_cpy_readfds);
		this->_map_clients.erase(i);

		std::cout << RED << "Error reading from socket" << RESET <<std::endl;
	}
	else if (size_read == 0) 
	{ 
		getpeername(i , (struct sockaddr*)(&(this->_server_init.get_ref_server_addr())) , (socklen_t*)&this->_server_init.get_ref_addrlen()); 

		if (close(i) < 0)
		{
			std::cerr << RED << "ERROR : " << RESET;
			perror("close");
		}
		std::cout << YELLOW << "Client " << i << " disconected | IP = " << inet_ntoa(this->_server_init.get_server_addr().sin_addr) << " | PORT = "<< ntohs (this->_server_init.get_server_addr().sin_port) << RESET <<std::endl;
		FD_CLR(i, &this->_cpy_readfds);
		this->_map_clients.erase(i);
	}
	else if (size_read > 0)
	{
		std::cout << GREEN << "Received request of " << size_read << " characters from client " << i << RESET << std::endl;
		FD_CLR(i, &this->_cpy_readfds);
		FD_SET(i, &this->_cpy_writefds);
		this->_map_clients[i].set_request(buffer);
		this->_map_clients[i].set_size_request(size_read);
		this->_map_clients[i].set_socket_mod(READ_M);
	}

}

void RunServer::send_response (int i)
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
	FD_CLR(i, &this->_cpy_writefds);
	this->_map_clients.erase(i);

}

void RunServer::process ()
{
	int i;
	int max_sd;
	struct timeval timeout;


	timeout.tv_sec = 0;
	timeout.tv_usec = 100000;

	this->_readfds = this->_cpy_readfds;
	this->_writefds = this->_cpy_writefds;

	int max_key = this->_server_init.get_sock_server();
	max_sd = this->_server_init.get_sock_server();

	for (std::map<int, Client>::iterator it = this->_map_clients.begin(); it != this->_map_clients.end(); ++it)
	{
		if (it->first > max_key)
		{
			max_key = it->first;
		}
	}

	for ( i = 0 ; i < MAX_CLIENT ; i++)
	{
		if (FD_ISSET(i, &this->_readfds) && i > max_sd)
		{
			max_sd = i;
		}
		else if (FD_ISSET(i, &this->_writefds) && i > max_sd)
		{
			max_sd = i;
		}
	}

	if (max_key != max_sd)
	{
		std::cout << RED << "---------ERROR--------" << RESET <<std::endl;
	}


	/* select() Delete from readfds and writefds all the sockets not "ready" for an I/O operation. */
	if (select(max_sd +1 , &this->_readfds, &this->_writefds, NULL, &timeout) < 0) 
	{ 
		std::cerr << RED << "ERROR : " << RESET;
		perror("select");
	} 

	for (i = 0; i < max_sd + 1; i++) 
	{
		if (FD_ISSET(i, &this->_readfds) && i == _server_init.get_sock_server()) 
		{
			this->accept_new_connection();
		}
		else if (FD_ISSET(i, &this->_readfds)) 
		{
			recvs_request (i);
		}
		else if (FD_ISSET(i, &this->_writefds))
		{
			send_response(i);
		}
	}	

}

int main() 
{ 
	try
	{
		signal(SIGPIPE, sigpipeHandle);

		ServerInitializer server_init(PORT, SIZE_WAITING_LIST);
		server_init.bind_listen_socket_serv();

		//std::cout << server_init.get_sock_server() << std::endl;

		RunServer run_server(server_init);

		std::cout << MAGENTA << "Listening on socket " << server_init.get_sock_server() << " bind with port " << PORT << "\033[0m" << std::endl;
		
		while(42) 
		{
			run_server.process();
		}
	}
	catch(std::exception& e)
	{
		std::cout << "Exception : " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return 0; 
}
