#include "server_initializer.hpp"

server_initializer::server_initializer(int port) : _PORT(port)
{

	this->_sock_server = socket(AF_INET, SOCK_STREAM, 0);

	if (this->_sock_server <= 0)
	{
		std::cerr << "Could not create server socket" << std::endl;
		// throw std::runtime_error("Could not create server socket");
	}

	opt = 1;
	if( setsockopt(this->_sock_server, SOL_SOCKET, SO_REUSEADDR, (char *)&(this->opt), sizeof(this->opt)) < 0 ) 
	{ 
		std::cerr << "Could not set socket option" << std::endl;
		
		// throw std::runtime_error("Could not set socket option");
		//exit(EXIT_FAILURE); 
	} 


	this->_server_addr.sin_family = AF_INET;
	this->_server_addr.sin_addr.s_addr = INADDR_ANY;
	this->_server_addr.sin_port = htons(this->_PORT);

	this->_addrlen = sizeof(this->_server_addr);

}

server_initializer::~server_initializer(){};

int & server_initializer::get_ref_addrlen()
{
	return this->_addrlen;
}

int server_initializer::get_sock_server()
{
	return this->_sock_server;
}

struct sockaddr_in server_initializer::get_server_addr()
{
	return this->_server_addr;
}

int server_initializer::bind_socket_port()
{
	if (bind(this->_sock_server, (struct sockaddr *)&(this->_server_addr), sizeof(this->_server_addr))<0) 
	{ 
		std::cerr << "Could not bind socket to port" << std::endl;
		//throw std::runtime_error("Could not bind socket to port");
		return EXIT_FAILURE; 
	}

	return EXIT_SUCCESS;

}

struct sockaddr_in & server_initializer::get_ref_server_addr()
{
	return this->_server_addr;
}