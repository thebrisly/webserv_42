#include "ServerInitializer.hpp"
#define PORT 8888 
#define SIZE_WAITING_LIST 30
#define MAX_CLIENT 30
#define READ_M 1
#define WRITE_M 2

#include <vector>
#include <sys/socket.h>
#include <fstream>


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
		int _socket_mod;

	public :
		Client(int socket);
		~Client();

		int get_socket() const;
		void set_socket (int socket);

		int get_socket_mod() const;
		void set_socket_mod (int socket_mod);

		std::string get_request() const;
		void set_request (std::string request);

		std::string get_response() const;
		void set_response (std::string response);

};

Client::Client(int socket) : _socket(socket), _socket_mod(0) {};

Client::~Client() {};

int Client::get_socket() const
{
	return this->_socket;
};

void Client::set_socket (int socket)
{
	this->_socket = socket;
}

int Client::get_socket_mod() const
{
	return this->_socket_mod;
};

void Client::set_socket_mod (int socket_mod)
{
	this->_socket_mod = socket_mod;
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
	os << "Client on socket " << cl.get_socket() << " with mode " << cl.get_socket_mod() <<std::endl;
	return os;
}

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

int main() 
{ 

	int new_socket, activity, i , valread , sd; 
	int max_sd; 
	char buffer[1025];
	fd_set readfds;
	fd_set writefds;
	std::ofstream outputFile("log.txt");
	
	
	std::string body = "<!DOCTYPE html><html><body><h1>My First Heading</h1><p>My first paragraph.</p></body></html>"; 
	std::string header = "HTTP/1.1 200 OK\nContent-Type: text/html\nConnection: keep-alive\nContent-Length: " + std::to_string(body.length()) + "\r\n\r\n";
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

	int debug = 0;

	puts("Waiting for connections ..."); 	
	while(42) 
	{
		outputFile << "loop " << debug << std::endl;

		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
	
		FD_SET(server_init.get_sock_server(), &readfds);
		FD_SET(server_init.get_sock_server(), &writefds);

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
			exit(EXIT_FAILURE);
		} 

		if (FD_ISSET(server_init.get_sock_server(), &readfds)) 
		{
			outputFile << "New connection" << std::endl;
			//std::cout << "New client number of connections = " << count_if(clients_vector.begin(), clients_vector.end(), PredicateValueSd()) << std::endl;

			if ((new_socket = accept(server_init.get_sock_server(), (struct sockaddr *)&(server_init.get_ref_server_addr()), (socklen_t*)&server_init.get_ref_addrlen()))<0) 
			{ 
				perror("accept"); 
				exit(EXIT_FAILURE); 
			} 
			
			outputFile << "New client connected on socket with fd " << new_socket << " with ip " << inet_ntoa(server_init.get_server_addr().sin_addr) << " on port "<< ntohs (server_init.get_server_addr().sin_port) << std::endl;

			for (i = 0; i < MAX_CLIENT; i++) 
			{ 
				if( clients_vector[i].get_socket() == 0 ) 
				{ 
					clients_vector[i].set_socket(new_socket);
					clients_vector[i].set_socket_mod(READ_M);
					//std::cout << "Client " << i << " connected on socket with fd " << new_socket << std::endl;	
					break; 
				} 
			}
		}

		for (i = 0; i < max_sd; i++) 
		{ 

			sd = clients_vector[i].get_socket();

			if (FD_ISSET(sd, &readfds)) 
			{
				outputFile << "Activity in readfds :" << std::endl;
				outputFile << "i = " << i << " : " << clients_vector[i];

				memset (buffer, 0, 1024);
				valread = recv(sd,	buffer, 1024, 0);
				outputFile << "Client " << i << " send on socket with fd " << sd << " : " << valread << " characters." <<std::endl;
				outputFile << "Request = " << buffer << std::endl;

				clients_vector[i].set_request(buffer);
				// LAURA



				if (valread == -1)
				{
					std::cout << "Error reading from socket" << std::endl;
					return EXIT_FAILURE;
				}
				if (valread == 0) 
				{ 
					getpeername(sd , (struct sockaddr*)(&(server_init.get_ref_server_addr())) , (socklen_t*)&server_init.get_ref_addrlen()); 

					outputFile << "Client disconnected on socket with fd " << sd << " with ip " << inet_ntoa(server_init.get_server_addr().sin_addr) << " on port "<< ntohs (server_init.get_server_addr().sin_port) << std::endl;

					close( sd ); 

					clients_vector[i].set_socket(0);
					//outputFile << "Remaining connections = " << count_if(clients_vector.begin(), clients_vector.end(), PredicateValueSd()) << std::endl;
					// for (int k = 0; k < MAX_CLIENT; k++)
					// {
					// 	if (clients_vector[k].get_socket() != 0)
					// 	{
					// 		std::cout << "Client " << k << " connected on socket with fd " << clients_vector[k].get_socket() << std::endl;
					// 	}
					// }
				}
				else
				{

					clients_vector[i].set_socket_mod(WRITE_M);
					// clients_vector[i].set_response(response);

					// send(sd , response.c_str(), response.length() , 0 ); 
				} 
				outputFile << std::endl;
			}
			else if (FD_ISSET( sd, &writefds))
			{
				outputFile << "Activity in writefds :" << std::endl;
				outputFile << "i = " << i << " : " << clients_vector[i] <<std::endl;

				if (clients_vector[i].get_socket_mod() == WRITE_M)
				{
					clients_vector[i].set_response(response);

					int size_send = send(sd , response.c_str(), response.length() , 0 ); 	
					outputFile << "Client " << i << " received " << size_send << " characters." <<std::endl;				
					clients_vector[i].set_socket_mod(READ_M);
					close( sd );
					clients_vector[i].set_socket(0);
				}
				
			}

		}

		debug ++;
		int nb_connections = 0;
		for (int k = 0; k < MAX_CLIENT; k++)
		{
			if (clients_vector[k].get_socket() != 0)
			{
				outputFile << "[" << k << " : " << clients_vector[k].get_socket() << "] ";
				nb_connections++;
			}
		}
		outputFile << std::endl;
		outputFile << "nb_connections = " << nb_connections << std::endl;
		outputFile << std::endl;
		outputFile << std::endl;


	}
	}
	catch(std::exception& e)
	{
		std::cout << "Exception : " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return 0; 
}
