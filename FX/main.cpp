#include "ServerInitializer.hpp"
#define PORT 8888 
#define SIZE_WAITING_LIST 1024
#define MAX_CLIENT 1024

#include <vector>


void display_client_list(int *client_socket, int max_clients)
{
	int i;
	for (i = 0; i < max_clients; i++)
	{
			std::cout << i << ":" << client_socket[i] << std::endl;
	}
	std::cout << std::endl;

}

int client_list_count(int *client_socket, int max_clients)
{
	int i;
	int count = 0;

	for (i = 0; i < max_clients; i++)
	{
		if (client_socket[i] != 0)
		{
			count++;
		}
	}
	return count;
}

class Client
{
	private :
		int	_socket;
		std::string _request;
		std::string _response;

	public :
		Client(int socket);
		~Client();

		int get_socket() const;
		void set_socket (int socket);

		std::string get_request() const;
		void set_request (std::string request);

		std::string get_response() const;
		void set_response (std::string response);

};

Client::Client(int socket) : _socket(socket) {};

Client::~Client() {};

int Client::get_socket() const
{
	return this->_socket;
};

void Client::set_socket (int socket)
{
	this->_socket = socket;
}

void Client::set_request (std::string request)
{
	this->_request = request;
}

std::string Client::get_request() const
{
	return this->_request;
}

void Client::set_response (std::string request)
{
	this->_request = request;
}

std::string Client::get_response() const
{
	return this->_request;
}

std::ostream& operator<<(std::ostream& os, const Client &cl)
{
	os << "Client on socket " << cl.get_socket() << std::endl;
	return os;
}

int main() 
{ 

	int new_socket, activity, i , valread , sd; 
	int max_sd; 
	char buffer[1025];
	fd_set readfds;
	fd_set writefds;
	
	std::string body = "<!DOCTYPE html><html><body><h1>My First Heading</h1><p>My first paragraph.</p></body></html>"; 
	std::string header = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(body.length()) + "\r\n\r\n";
	std::string response = header + body;
	

	std::vector<Client> clients_vector;

	for (i = 0; i < MAX_CLIENT; i++)
	{
		clients_vector.push_back(Client(0));
	}

	try
	{

	ServerInitializer server_init(PORT, SIZE_WAITING_LIST);
	server_init.bind_listen_socket_serv();

	puts("Waiting for connections ..."); 	
	while(true) 
	{

		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
	
		FD_SET(server_init.get_sock_server(), &readfds);

		max_sd = server_init.get_sock_server(); 
			
		for ( i = 0 ; i < MAX_CLIENT ; i++) 
		{ 
			sd = clients_vector[i].get_socket();

			if(sd > 0) 
			{
				FD_SET( sd , &readfds); 
				FD_SET( sd, &writefds);
			}

			if(sd > max_sd) 
				max_sd = sd; 
		} 
	
		activity = select(max_sd + 1, &readfds, &writefds, NULL, NULL); 

		if ((activity < 0) && (errno!=EINTR)) 
		{ 
			printf("select error"); 
		} 

		if (FD_ISSET(server_init.get_sock_server(), &readfds)) 
		{ 
			if ((new_socket = accept(server_init.get_sock_server(), (struct sockaddr *)&(server_init.get_ref_server_addr()), (socklen_t*)&server_init.get_ref_addrlen()))<0) 
			{ 
				perror("accept"); 
				exit(EXIT_FAILURE); 
			} 
			
			std::cout << "New client connected on socket with fd " << new_socket << " with ip " << inet_ntoa(server_init.get_server_addr().sin_addr) << " on port "<< ntohs (server_init.get_server_addr().sin_port) << std::endl;

			for (i = 0; i < MAX_CLIENT; i++) 
			{ 
				if( clients_vector[i].get_socket() == 0 ) 
				{ 
					clients_vector[i].set_socket(new_socket);	
					std::cout << "Client " << i << " connected on socket with fd " << new_socket << std::endl;	
					break; 
				} 
			}
		}

		for (i = 0; i < MAX_CLIENT; i++) 
		{ 

			sd = clients_vector[i].get_socket();

			if (FD_ISSET( sd , &readfds)) 
			{ 
				valread = read(sd , buffer, 1024);

				clients_vector[i].set_request(buffer);
				// LAURA

				std::cout << "Client " << i << " send on socket with fd " << sd << " : " << valread << " characters." <<std::endl;
				std::cout << "Request = " << buffer << std::endl;

				if (valread == 0) 
				{ 
					getpeername(sd , (struct sockaddr*)(&(server_init.get_ref_server_addr())) , (socklen_t*)&server_init.get_ref_addrlen()); 

					std::cout << "Client disconnected on socket with fd " << new_socket << " with ip " << inet_ntoa(server_init.get_server_addr().sin_addr) << " on port "<< ntohs (server_init.get_server_addr().sin_port) << std::endl;

					close( sd ); 

					clients_vector[i].set_socket(0);
				}
				else
				{
					// LAURA
					clients_vector[i].set_response(response);

					send(sd , response.c_str(), response.length() , 0 ); 
				} 
			}
			else if (FD_ISSET( sd, &writefds))
			{
				/*Sera utile pour les CGI*/
				//std::cout << "Client " << i << " send on socket with fd " << std::endl;
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
