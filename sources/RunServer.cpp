#include "../includes/RunServer.hpp"
#include "../includes/Request.hpp"

void display_clients(std::ofstream & out, std::map<int, Client> & map_clients)
{
	for (std::map<int, Client>::iterator it = map_clients.begin(); it!= map_clients.end(); ++it)
	{
		out << "Client : " << it->first << std::endl;
		out << (*it).second << std::endl;
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

/* RunServer is an object that runs the servers */

RunServer::RunServer(ServersManager & servers_manager, std::string log_filename) : _servers_manager(servers_manager), _out(log_filename, std::ofstream::out), _time_start(time(NULL))
{

	FD_ZERO(&this->_writefds);
	FD_ZERO(&this->_readfds);

	FD_ZERO(&this->_cpy_readfds);
	FD_ZERO(&this->_cpy_writefds);

	std::cout << "[" << time(0) - this->_time_start << "] " << "number of servers : " << this->_servers_manager.get_servers().size() << std::endl;

	for (unsigned long i = 0; i < this->_servers_manager.get_servers().size(); i++)
	{
		std::cout << GREEN << "[" << time(0) - this->_time_start << "] " << this->_servers_manager[i].get_config().getPort() << RESET << std::endl;

		FD_SET(this->_servers_manager[i].get_sock_server(), &this->_cpy_readfds);
	}

	this->_out << "Initial setup of fd_set" << std::endl;
	display_fd_set(this->_out, this->_cpy_readfds, this->_cpy_writefds);
	this->_out << std::endl;


	this->_timeout.tv_sec = 0;
	this->_timeout.tv_usec = 100000;
}

RunServer::~RunServer(){}

void RunServer::accept_new_connection(int i)
{
	int new_socket;

	ServerInitializer & server_init = this->_servers_manager.get_server_by_sock(i);

	if (i != server_init.get_sock_server())
	{
		std::cerr << RED << "ERROR : accept_new_connection " << i << " " << server_init.get_sock_server() << RESET << std::endl;
		sleep(10);
	}

	if ((new_socket = accept(i, (struct sockaddr *)&(server_init.get_ref_server_addr()), (socklen_t*)&server_init.get_ref_addrlen()))<0)
	{
		std::cerr << RED << "ERROR : " << RESET;
		perror("accept");
	}

	std::cout << BLUE << "[" << time(0) - this->_time_start << "]" << " New client connected on socket " << new_socket << " with ip " << inet_ntoa(server_init.get_server_addr().sin_addr) << " on port "<< server_init.get_config().getPort() << RESET << std::endl;
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
	else if (size_read == 0 && clock() - this->_map_clients[i].get_session_time() > CLOCKS_PER_SEC)
	{
		//getpeername(i , (struct sockaddr*)(&(server_init.get_ref_server_addr())) , (socklen_t*)&server_init.get_ref_addrlen());

		std::cout << CLOCKS_PER_SEC <<std::endl;

		std::cout << "Since creation : " << clock() - this->_map_clients[i].get_session_time() << std::endl;


		if (close(i) < 0)
		{
			std::cerr << RED << "ERROR : " << RESET;
			perror("close");
		}
		FD_CLR(i, &this->_cpy_readfds);
		FD_CLR(i, &this->_cpy_writefds);
		this->_map_clients.erase(i);
		std::cout << YELLOW << "[" << time(0) - this->_time_start << "] " << "Client " << i << " disconected due to empty socket." << RESET << std::endl;
	}
	else if (size_read > 0)
	{
		std::cout << GREEN << "[" << time(0) - this->_time_start << "] " << "Received request of " << size_read << " characters from client " << i << RESET << std::endl;
		FD_CLR(i, &this->_cpy_readfds);
		FD_SET(i, &this->_cpy_writefds);
		this->_map_clients[i].set_request(buffer);
		this->_map_clients[i].set_size_request(size_read);
		this->_map_clients[i].set_socket_mod(WRITE_M);
		//std::cout << "ICI : " << this->_map_clients[i].get_request() << std::endl;

		this->_map_clients[i].set_session_time(clock());


		Request request_test2(this->_map_clients[i].get_request(), this->_map_clients[i].get_server_config());
		request_test2.parseRequest(request_test2.getCurrentRequest());

		this->_map_clients[i].set_request_object(request_test2);


		// request_test2.check_host_port();

		std::cout << request_test2 << std::endl;

		request_test2.parseRequest(request_test2.getCurrentRequest());

		request_test2.checkRequest();


		request_test2.prepareResponse();

		// request_test2.issetFile();



		// std::string body = "";
		// std::string header = "";

		// if (request_test2.check_host_port() == false)
		// {

		// 	body = "Host and port are not valid";

		// 	header = "HTTP/1.1 200 OK\nContent-Type: text/html\nConnection: keep-alive\nContent-Length: " + std::to_string(body.length()) + "\r\n\r\n";


		// }
		// else if (request_test2.getPath() == "/")
		// {
		// 	std::ifstream file("web/index.html");
		// 	std::string current_line;
		// 	while (std::getline (file, current_line))
		// 	{
		// 		body += current_line;
		// 		body += "\n";
		// 	}
		// 	header = "HTTP/1.1 200 OK\nContent-Type: text/html\nConnection: keep-alive\nContent-Length: " + std::to_string(body.length()) + "\r\n\r\n";
		// }
		// else if (request_test2.getType() == "css")
		// {
		// 	std::ifstream file("web/style0.css");
		// 	std::string current_line;
		// 	while (std::getline (file, current_line))
		// 	{
		// 		body += current_line;
		// 		body += "\n";
		// 	}
		// 	header = "HTTP/1.1 200 OK\nContent-Type: text/css\nConnection: keep-alive\nContent-Length: " + std::to_string(body.length()) + "\r\n\r\n";
		// }
		// else if (request_test2.getType() == "jpg")
		// {
		// 	std::ifstream file("web/" + request_test2.getPath());
		// 	std::string current_line;
		// 	while (std::getline (file, current_line))
		// 	{
		// 		body += current_line;
		// 		body += "\n";
		// 	}
		// 	header = "HTTP/1.1 200 OK\nContent-Type: image/jpeg\nConnection: keep-alive\nContent-Length: " + std::to_string(body.length()) + "\r\n\r\n";
		// }
		// else
		// {
		// 	body = "The requested resource is not available.";
		// 	header = "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nConnection: keep-alive\n";
		// 	header += "Content-Length: " + std::to_string(body.length()) + "\r\n\r\n";
		// }

		// std::string response = header + body;

		std::cout << "Response = " << request_test2.getResponse() << std::endl;

		this->_map_clients[i].set_response(request_test2.getResponse());
		this->_map_clients[i].set_size_response(request_test2.getResponse().length());

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
		//exit(0);
	}

	std::cout << GREEN << "[" << time(0) - this->_time_start << "] " << "Sent response of " << response.length() << " characters to client "<< i << RESET <<std::endl;

	if (this->_map_clients[i].get_request_object().getConnection() == "close")
	{
		if (close (i) < 0)
		{
			std::cerr << RED << "ERROR : " << RESET;
			perror("close");
		}
		FD_CLR(i, &this->_cpy_readfds);
		FD_CLR(i, &this->_cpy_writefds);
		this->_map_clients.erase(i);

		std::cout << YELLOW << "[" << time(0) - this->_time_start << "] " << "Client " << i << " disconected." << RESET <<std::endl;
		//exit(0);
	}
	else
	{
		FD_CLR(i, &this->_cpy_writefds);
		FD_SET(i, &this->_cpy_readfds);
		this->_map_clients[i].set_socket_mod(READ_M);
		std::cout << "Connection status [" << this->_map_clients[i].get_request_object().getConnection()<< "]" <<std::endl;
		//exit(0);
	}

}

void RunServer::process (int loop_count)
{
	/*
	Donc la il faut garder alive la socket tant que toutes les requetes sont pas menees à bien et ensuite on ferme la socket
	Le client se deconnecte car il a pas le temps d'envoyer sa 2eme requete. Les tours de boucle se font trop rapidement.
	Le referer peut aider...
	*/

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
		//std::cout << "Considering client " << i << " : " <<std::endl;
		if (FD_ISSET(i, &this->_readfds) && this->_servers_manager.is_server_active(i))
		{
			this->_out << "Considering client " << i << " on loop " << loop_count << " : " <<std::endl;
			this->accept_new_connection(i);
			this->_out<< "------------------- accept new connection -------------------" << std::endl;
			display_fd_set(this->_out, this->_readfds, this->_writefds);
			this->_out<< "copy = ";
			display_fd_set(this->_out, this->_cpy_readfds, this->_cpy_writefds);
			display_clients(this->_out, this->_map_clients);
		}
		else if (FD_ISSET(i, &this->_readfds))
		{
			this->_out << "Considering client " << i << " on loop " << loop_count << " : " <<std::endl;
			this->recvs_request (i);
			this->_out<< "------------------- recvs request -------------------" << std::endl;
			display_fd_set(this->_out, this->_readfds, this->_writefds);
			this->_out<< "copy = ";
			display_fd_set(this->_out, this->_cpy_readfds, this->_cpy_writefds);
			display_clients(this->_out, this->_map_clients);
		}
		else if (FD_ISSET(i, &this->_writefds))
		{
			this->_out << "Considering client " << i << " on loop " << loop_count << " : " <<std::endl;
			this->send_response(i);
			this->_out<< "------------------- send response -------------------" << std::endl;
			display_fd_set(this->_out, this->_readfds, this->_writefds);
			this->_out<< "copy = ";
			display_fd_set(this->_out, this->_cpy_readfds, this->_cpy_writefds);
			display_clients(this->_out, this->_map_clients);
		}
	}
}
