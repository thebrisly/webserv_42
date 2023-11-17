#include "ConfigParser.hpp"
#include "ServerInitializer.hpp"
#include "Client.hpp"
#include <vector>
#include <sys/socket.h>
#include <sys/time.h>
#include <fstream>
#include <signal.h>

#define MAX_CLIENT 30
#define SIZE_WAITING_LIST 30
#define MAX_CLIENT 30
#define READ_M 1
#define WRITE_M 2

typedef std::map<int, std::string> MAP;
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
	std::cerr << "PROBLEME ECRITURE SOCKET" << sig <<std::endl;
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

void printRoute(const RouteConfig &route)
{
	std::cout << "Path: " << route.path << std::endl;
	std::cout << "Methods: ";
	for (std::vector<std::string>::const_iterator it = route.methods.begin(); it != route.methods.end(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	std::cout << "Default file: " << route.default_file << std::endl;
	std::cout << "Directory listing: " << route.directory_listing << std::endl;
	std::cout << "Root: " << route.root << std::endl;
	std::cout << "Redirect: " << route.redirect.first << " " << route.redirect.second << std::endl;
}



int main() 
{ 
	int c;
	int new_socket, activity, i , valread , sd;
	int port;
	std::string ip_address, server_name, root;
	int max_sd; 
	char buffer[1025];

	std::vector<ServerConfig> configs;


    ConfigParser parser;
    configs = parser.parseConfigs("config.config");

	if (configs.empty())
	{
		std::cout << "No server configs found" << std::endl;
		return 0;
	}
	ServerConfig config = configs.front();	


	port = config.getPort();
	ip_address = config.getIPAddress();
	server_name = config.getServerName();
	root = config.getRoot();

	fd_set readfds;
	fd_set writefds;
	std::ofstream outputFile("log.txt");

	struct timeval timeout;

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	//std::ofstream outputFile2("log2.txt");
	
	
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

	ServerInitializer server_init(config.getPort(), SIZE_WAITING_LIST);
	server_init.bind_listen_socket_serv();
	signal(SIGPIPE, sigpipeHandle);
	//ServerInitializer server_init2(PORT_2, SIZE_WAITING_LIST);
	//server_init2.bind_listen_socket_serv();

	int debug = 0;

	while(42) 
	{
		outputFile << "loop " << debug << std::endl;
		std::cout << "\033[1;31m" << "loop " << debug << "\033[0m" << std::endl;

		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
	
		FD_SET(server_init.get_sock_server(), &readfds);
		// FD_SET(server_init.get_sock_server(), &writefds);

		max_sd = server_init.get_sock_server(); 
			
		for ( i = 0 ; i < MAX_CLIENT ; i++) 
		{
			sd = clients_vector[i].get_socket();

			if(sd > 0) 
			{
				if (clients_vector[i].get_socket_mod() == WRITE_M)
				{
					outputFile << sd << " added in writefds." <<std::endl; 
					FD_SET( sd, &writefds);
				}
				else
				{
					outputFile << sd << " added in readfds." <<std::endl; 
					FD_SET( sd , &readfds);
				}
			}

			if(sd > max_sd) 
				max_sd = sd; 
		}

		std::cout << "readfds = "; 
		fd_set_checker(&readfds);
		std::cout << std::endl;

		std::cout << "writefds = "; 
		fd_set_checker(&writefds);
		std::cout << std::endl;

		int nb_connections2 = 0;
		for (int k = 0; k < MAX_CLIENT; k++)
		{
			if (clients_vector[k].get_socket() != 0)
			{
				std::cout << "[" << k << " : " << clients_vector[k].get_socket() << "] ";
				nb_connections2++;
			}
		}
		std::cout << "nb_connections = " << nb_connections2 << std::endl;


		std::cout << "SELECT" << " " << max_sd << " " << server_init.get_sock_server() << std::endl;
		outputFile << "SELECT" << " " << max_sd << " " << server_init.get_sock_server() << std::endl;
		activity = select(max_sd + 1, &readfds, &writefds, NULL, &timeout);
		outputFile << "activity = " << activity << std::endl;
		std::cout << "activity = " << activity << std::endl;

		if (activity < 0) 
		{ 
			perror("select");
			throw std::runtime_error("Issue with select");
		} 

		/*FD_ISSET mets le fd sur le premier 0 qu il trouve... */
		/*Si il reste une place avant le max_sd + 1 il est chéké direct*/
		/*Sinon il sera chéké tout de suite après...*/
		/*Je pense que c'est un probleme*/
		if (FD_ISSET(server_init.get_sock_server(), &readfds)) 
		{
			outputFile << "New connection" << std::endl;

			if ((new_socket = accept(server_init.get_sock_server(), (struct sockaddr *)&(server_init.get_ref_server_addr()), (socklen_t*)&server_init.get_ref_addrlen()))<0) 
			{ 
				perror("accept"); 
				throw std::runtime_error("Issue with accept");
			} 
			
			outputFile << "New client connected on socket with fd " << new_socket << " with ip " << inet_ntoa(server_init.get_server_addr().sin_addr) << " on port "<< ntohs (server_init.get_server_addr().sin_port) << std::endl;

			for (i = 0; i < MAX_CLIENT; i++) 
			{ 
				if( clients_vector[i].get_socket() == 0 ) 
				{ 
					clients_vector[i].set_socket(new_socket);
					clients_vector[i].set_socket_mod(READ_M);
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


				// LAURA


				if (valread == -1)
				{
					c = close (sd);
					if (c < 0)
						outputFile << "ERROR CLOSE" << std::endl;
					clients_vector[i].set_socket(0);
					std::cout << "Error reading from socket" << std::endl;
					
				}
				if (valread == 0) 
				{ 
					getpeername(sd , (struct sockaddr*)(&(server_init.get_ref_server_addr())) , (socklen_t*)&server_init.get_ref_addrlen()); 

					outputFile << "Client disconnected on socket with fd " << sd << " with ip " << inet_ntoa(server_init.get_server_addr().sin_addr) << " on port "<< ntohs (server_init.get_server_addr().sin_port) << std::endl;

					close( sd ); 
					if (c < 0)
						outputFile << "ERROR CLOSE" << std::endl;
					clients_vector[i].set_socket(0);
				}
				else if (valread > 0)
				{
					clients_vector[i].set_request(buffer);
					clients_vector[i].set_size_request(valread);
					clients_vector[i].set_socket_mod(WRITE_M);
				}


				outputFile << std::endl;
			}
			else if (FD_ISSET( sd, &writefds))
			{
				outputFile << "Activity in writefds :" << std::endl;
				outputFile << "i = " << i << " : " << clients_vector[i] <<std::endl;


				clients_vector[i].set_response(response);
				clients_vector[i].set_size_response(response.length());

				int size_send = send(sd , clients_vector[i].get_response().c_str(), clients_vector[i].get_size_response() , 0 ); 	
				outputFile << "Client " << i << " received " << size_send << " characters." <<std::endl;				
				//clients_vector[i].set_socket_mod(READ_M);
				c = close( sd );
				if (c < 0)
					outputFile << "ERROR CLOSE" << std::endl;
				clients_vector[i].set_socket(0);
			
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
