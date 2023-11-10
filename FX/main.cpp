#include "ServerInitializer.hpp"
#define PORT 8888 
#define SIZE_WAITING_LIST 30
#define MAX_CLIENT 1024


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

	public :
		Client(int socket);
		~Client();

		int get_socket() const;
		void set_socket (int socket);

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

std::ostream& operator<<(std::ostream& os, const Client &cl)
{
	os << "Client on socket " << cl.get_socket() << std::endl;
	return os;
}

int main() 
{ 

	int new_socket , client_socket[30] , max_clients = 30 , activity, i , valread , sd; 
	int max_sd; 
	char buffer[1025];
	fd_set readfds;
	fd_set writefds;
	
	std::string body = "<!DOCTYPE html><html><body><h1>My First Heading</h1><p>My first paragraph.</p></body></html>"; 
	std::string header = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(body.length()) + "\r\n\r\n";
	std::string response = header + body;
	
	std::map<int, Client> clients_map;

	for (i = 0; i < MAX_CLIENT; i++)
	{
		clients_map.insert(std::make_pair(i, Client(0)));
	}

    int socket = 10; 
    Client client(socket);
    std::cout << client;

	for (std::map<int, Client>::iterator it = clients_map.begin(); it != clients_map.end(); ++it )
	{
		std::cout << it->second;
	}

	for (i = 0; i < max_clients; i++) 
	{ 
		client_socket[i] = 0; 
	}

	try
	{

	ServerInitializer server_init(PORT, SIZE_WAITING_LIST);
	server_init.bind_listen_socket_serv();

	puts("Waiting for connections ..."); 	
	while(true) 
	{
		//debug = client_list_count(client_socket, 30);

		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
	
		FD_SET(server_init.get_sock_server(), &readfds);

		max_sd = server_init.get_sock_server(); 
			
		for ( i = 0 ; i < max_clients ; i++) 
		{ 
			sd = client_socket[i];

			//sd = clients_map[i].get_socket();

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

			for (i = 0; i < max_clients; i++) 
			{ 
				// if (clients_map[i].get_socket() == 0)
				// {
				// 	clients_map[i].set_socket(new_socket);
				// 	std::cout << "Client " << i << " connected on socket with fd " << new_socket << std::endl;	
					
				// 	break; 

				// }

				if( client_socket[i] == 0 ) 
				{ 
					client_socket[i] = new_socket; 

					std::cout << "Client " << i << " connected on socket with fd " << new_socket << std::endl;	
					
					break; 
				} 
			}
		}

		for (i = 0; i < max_clients; i++) 
		{ 
			sd = client_socket[i]; 

			if (FD_ISSET( sd , &readfds)) 
			{ 
				valread = read( sd , buffer, 1024);
				std::cout << "Client " << i << " send on socket with fd " << sd << " : " << valread << " characters." <<std::endl;
				std::cout << "Request = " << buffer << std::endl;

				if (valread == 0) 
				{ 
					getpeername(sd , (struct sockaddr*)(&(server_init.get_ref_server_addr())) , (socklen_t*)&server_init.get_ref_addrlen()); 

					std::cout << "Client disconnected on socket with fd " << new_socket << " with ip " << inet_ntoa(server_init.get_server_addr().sin_addr) << " on port "<< ntohs (server_init.get_server_addr().sin_port) << std::endl;

					close( sd ); 
					client_socket[i] = 0;
				}
				else
				{
					send(sd , response.c_str(), response.length() , 0 ); 
				} 
			}
			else if (FD_ISSET( sd, &writefds))
			{
				/*Sera utile pour les CGI*/
				
			}

		}

		// if (debug == 3)
		// {
		// 	display_client_list(client_socket, 30);
		// 	break;
		// }

	}
	}
	catch(std::exception& e)
	{
		std::cout << "Exception : " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return 0; 
} 
