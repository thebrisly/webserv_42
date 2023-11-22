#include "RunServer.hpp"

RunServer::RunServer(ServersManager & servers_manager) : _servers_manager(servers_manager)
{
	FD_ZERO(&this->_writefds);
	FD_ZERO(&this->_readfds);

	FD_ZERO(&this->_cpy_readfds);
	FD_ZERO(&this->_cpy_writefds);

	std::cout << "number of servers : " << this->_servers_manager.get_servers().size() << std::endl;

	for (unsigned long i = 0; i < this->_servers_manager.get_servers().size(); i++)
	{
		std::cout << GREEN << this->_servers_manager[i].get_config().getPort() << RESET << std::endl;
		FD_SET(this->_servers_manager[i].get_sock_server(), &this->_cpy_readfds);
	}
	
	this->_timeout.tv_sec = 0;
	this->_timeout.tv_usec = 100000;
}

RunServer::~RunServer(){}

void RunServer::accept_new_connection(int i)
{
	int new_socket;

	std::cout << "i = " << i << std::endl;
	ServerInitializer & server_init = this->_servers_manager.get_server_by_sock(i);

	//std::cout << i << " " << this->_servers_manager[0].get_sock_server() << std::endl;
	//sleep(10);
	if (i != server_init.get_sock_server())
	{
		std::cerr << RED << "ERROR : accept_new_connection " << i << " " << server_init.get_sock_server() << RESET << std::endl;
		sleep(10);
	}
	//ServerInitializer & server_init = this->_servers_manager[0];

	if ((new_socket = accept(i, (struct sockaddr *)&(server_init.get_ref_server_addr()), (socklen_t*)&server_init.get_ref_addrlen()))<0) 
	{ 
		std::cerr << RED << "ERROR : " << RESET;
		perror("accept");
	}
	std::cout << BLUE << "New client connected on socket " << new_socket << " with ip " << inet_ntoa(this->_servers_manager[0].get_server_addr().sin_addr) << " on port "<< ntohs (this->_servers_manager[0].get_server_addr().sin_port) << RESET << std::endl;
	FD_SET(new_socket, &this->_cpy_readfds);
	this->_map_clients.insert(std::pair<int, Client>(new_socket, Client(new_socket)));				
}

void RunServer::recvs_request (int i)
{
	char buffer[1025];
	int size_read;
	//ServerInitializer & server_init = this->_servers_manager.get_server_by_sock(i);



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
		//getpeername(i , (struct sockaddr*)(&(server_init.get_ref_server_addr())) , (socklen_t*)&server_init.get_ref_addrlen()); 

		if (close(i) < 0)
		{
			std::cerr << RED << "ERROR : " << RESET;
			perror("close");
		}
		std::cout << YELLOW << "Client " << i << " disconected" << RESET << std::endl;
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
	int max_key;

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 100000;

	this->_readfds = this->_cpy_readfds;
	this->_writefds = this->_cpy_writefds;

	max_key = this->_servers_manager.get_socket_max();
	max_sd = this->_servers_manager.get_socket_max();

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
		std::cout << RED << "---------ERROR--------" << " max_key = " << max_key << " max_sd = " << max_sd << RESET <<std::endl;
	}


	/* select() Delete from readfds and writefds all the sockets not "ready" for an I/O operation. */
	if (select(max_sd +1 , &this->_readfds, &this->_writefds, NULL, &timeout) < 0) 
	{ 
		std::cerr << RED << "ERROR : " << RESET;
		perror("select");
	} 

	for (i = 0; i < max_sd + 1; i++) 
	{
		if (FD_ISSET(i, &this->_readfds) && this->_servers_manager.is_server_active(i)) 
		{
			std::cout << "Je veux accepter la connexion sur socket " << i << std::endl;
			this->accept_new_connection(i);
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