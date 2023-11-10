#include "server.hpp"


Server::Server()
{

}

Server::~Server()
{
    
}

Server::Server(const Serverg &other)
{
	if (this != &other)
    {
        this->name = other.name;
        // ...
	}
}


/* assinment operator */
Server &Server::operator=(const Server &other)
{
	if (this != &other)
	{
		this->name = other.name;
        // ...
	}
	return (*this);
}
