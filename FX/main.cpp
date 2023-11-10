#include "ServerInitializer.hpp"
#define PORT 8888 


void display_client_list(int *client_socket, int max_clients)
{
	int i;
	for (i = 0; i < max_clients; i++)
	{
			std::cout << i << ":" << client_socket[i] << std::endl;
	}
	std::cout << std::endl;

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
	
	for (i = 0; i < max_clients; i++) 
	{ 
		client_socket[i] = 0; 
	}

	//display_client_list(client_socket, 30);	
	
	ServerInitializer server_init(PORT);
	server_init.bind_socket_port();
		
	//try to specify maximum of 3 pending connections for the master socket 
	if (listen(server_init.get_sock_server(), 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 

	//int debug = 0;

	puts("Waiting for connections ..."); 	
	while(true) 
	{ 
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
	
		FD_SET(server_init.get_sock_server(), &readfds);

		max_sd = server_init.get_sock_server(); 
			
		for ( i = 0 ; i < max_clients ; i++) 
		{ 
			sd = client_socket[i]; 

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
			
			printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket, inet_ntoa(server_init.get_server_addr().sin_addr) , ntohs (server_init.get_server_addr().sin_port)); 

			for (i = 0; i < max_clients; i++) 
			{ 
				if( client_socket[i] == 0 ) 
				{ 
					client_socket[i] = new_socket; 
					//printf("Adding to list of sockets as %d\n" , i);
					//display_client_list(client_socket, 30);	
					
					break; 
				} 
			}
		}

		for (i = 0; i < max_clients; i++) 
		{ 
			sd = client_socket[i]; 

			// if (sd > 0)
			// {
			// 	std::cout << "i = " << i << " sd = " << sd << std::endl;	
			// }

			if (FD_ISSET( sd , &readfds)) 
			{ 
				valread = read( sd , buffer, 1024);
				std::cout << "valread = " << valread << std::endl;
				if (valread == 0) 
				{ 
					getpeername(sd , (struct sockaddr*)(&(server_init.get_ref_server_addr())) , (socklen_t*)&server_init.get_ref_addrlen()); 
					printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(server_init.get_server_addr().sin_addr) , ntohs(server_init.get_server_addr().sin_port)); 		
					close( sd ); 
					client_socket[i] = 0;

					//display_client_list(client_socket, 30);	

				}
				else
				{ 
					std::cout << "Host " << i << " : " << buffer << std::endl;
					send(sd , response.c_str(), response.length() , 0 ); 
				} 
			}
			// else if (FD_ISSET( sd, &writefds))
			// {
			// 	//std::cout << debug << " Host " << i << " : " << "Writing" << std::endl;
			// 	debug = send(sd , response.c_str(), response.length() , 0 );
			// 	std::cout << debug << " Host " << i << " : " << "Writing done" << std::endl;
				
			// }
			// std::cout << std::endl;
		}

		//debug++;
	} 
	return 0; 
} 
