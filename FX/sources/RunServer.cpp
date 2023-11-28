#include "../includes/RunServer.hpp"
#include "../includes/Request.hpp"
#include <fstream>

/* RunServer is an object that runs the servers */

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

	//std::cout << "i = " << i << std::endl;
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
	std::cout << BLUE << "New client connected on socket " << new_socket << " with ip " << inet_ntoa(server_init.get_server_addr().sin_addr) << " on port "<< server_init.get_config().getPort() << RESET << std::endl;
	FD_SET(new_socket, &this->_cpy_readfds);
	this->_map_clients.insert(std::pair<int, Client>(new_socket, Client(new_socket, server_init.get_config())));
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
		FD_CLR(i, &this->_cpy_writefds);
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
		FD_CLR(i, &this->_cpy_readfds);
		FD_CLR(i, &this->_cpy_writefds);
		this->_map_clients.erase(i);
		std::cout << YELLOW << "Client " << i << " disconected" << RESET << std::endl;
	}
	else if (size_read > 0)
	{
		std::cout << GREEN << "Received request of " << size_read << " characters from client " << i << RESET << std::endl;
		FD_CLR(i, &this->_cpy_readfds);
		FD_SET(i, &this->_cpy_writefds);
		this->_map_clients[i].set_request(buffer);
		this->_map_clients[i].set_size_request(size_read);
		this->_map_clients[i].set_socket_mod(WRITE_M);
		//std::cout << "ICI : " << this->_map_clients[i].get_request() << std::endl;

		Request request_object(this->_map_clients[i].get_request());
		request_object.parseRequest(request_object.getCurrentRequest());

		this->_map_clients[i].set_request_object(request_object);



		//this->_map_clients[i].get_request_object().parseRequest(this->_map_clients[i].get_request_object().getCurrentRequest());

		// std::cout << "WHAT1 " << request_object.getMethod() << std::endl;
		// std::cout << "WHAT2 " << this->_map_clients[i].get_request_object().getMethod() << std::endl;

		//exit(0);
        // std::string                                 _path;
        // std::string                                 _method;
        // std::string                                 _version;
        // std::string                                 _host;
        // std::string                                 _connection;
        // std::string                                 _secfetchdest;
        // std::string                                 _port;
        // std::string                                 _hostname;


		/* Calcul de la reponse */

		


	}
}

void RunServer::send_response (int i)
{
	unsigned long max_body_size = this->_map_clients[i].get_server_config().getMaxBodySize();
	std::string response = this->_map_clients[i].get_response();

	if (response.length() > max_body_size)
	{
		response = response.substr(0, max_body_size);
	}

	if (send(i , response.c_str(), response.length() , 0 ) != static_cast<long>(response.length()))
	{
		std::cerr << RED << "ERROR : " << RESET;
		perror("send");
	}

	std::cout << GREEN << "Sent response of " << response.length() << " characters to client "<< i << RESET <<std::endl;
	sleep(1);
	std::cout << "[" << this->_map_clients[i].get_request_object().getConnection() << "]"<< std::endl;
	sleep(1);
	bool debug = this->_map_clients[i].get_request_object().getConnection() == "close";

	std::cout << "LA : " << debug << std::endl;

	if (this->_map_clients[i].get_request_object().getConnection() == "close")
	{
		std::cout << "JE VEUX CLOSE !!!" << std::endl;
	}



	if (close (i) < 0)
	{
		std::cerr << RED << "ERROR : " << RESET;
		perror("close");
	}
	std::cout << YELLOW << "Client " << i << " disconected." << RESET <<std::endl;

	FD_CLR(i, &this->_cpy_readfds);
	FD_CLR(i, &this->_cpy_writefds);
	this->_map_clients.erase(i);

}

void display_clients(std::ofstream & out, std::map<int, Client> & map_clients)
{
	for (std::map<int, Client>::iterator it = map_clients.begin(); it!= map_clients.end(); ++it)
	{
		out << "key : " << it->first << (*it).second <<std::endl;
	}
};

void display_fd_set(std::ofstream & out, const fd_set & readfds, const fd_set & writefds)
{
	out << "readfds : ";
	for (int i = 0; i < MAX_CLIENT; i++)
	{
		if (FD_ISSET(i, &readfds))
		{
			out << i << " " ;
		}
	}
	out << std::endl;
	out << "writefds : ";
	for (int i = 0; i < MAX_CLIENT; i++)
	{
		if (FD_ISSET(i, &writefds))
		{
			out << i << " " ;
		}
	}
	out << std::endl;
}

void RunServer::process (std::ofstream & out)
{
	int i;
	int max_sd;
	int max_key;

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
		std::cout << RED << "ERROR : " << " max_key = " << max_key << " max_sd = " << max_sd << RESET <<std::endl;
		exit(0);
	}


	/* select() Delete from readfds and writefds all the sockets not "ready" for an I/O operation. */
	if (select(max_sd +1 , &this->_readfds, &this->_writefds, NULL, &this->_timeout) < 0)
	{
		std::cerr << RED << "ERROR : " << RESET;
		perror("select");
	}

	for (i = 0; i < max_sd + 1; i++)
	{
		if (FD_ISSET(i, &this->_readfds) && this->_servers_manager.is_server_active(i))
		{
			this->accept_new_connection(i);
			out << "accept new connection" << std::endl;
			display_fd_set(out, this->_readfds, this->_writefds);
			display_fd_set(out, this->_cpy_readfds, this->_cpy_writefds);
			display_clients(out, this->_map_clients);
		}
		else if (FD_ISSET(i, &this->_readfds))
		{
			this->recvs_request (i);
			out << "recvs request" << std::endl;
			display_fd_set(out, this->_readfds, this->_writefds);
			display_fd_set(out, this->_cpy_readfds, this->_cpy_writefds);
			display_clients(out, this->_map_clients);
		}
		else if (FD_ISSET(i, &this->_writefds))
		{
			this->send_response(i);
			out << "send response" << std::endl;
			display_fd_set(out, this->_readfds, this->_writefds);
			display_fd_set(out, this->_cpy_readfds, this->_cpy_writefds);
			display_clients(out, this->_map_clients);
		}
	}
}
