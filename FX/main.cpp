//Example code: A simple server side code, which echos back the received response. 
//Handle multiple socket connections with select and fd_set on Linux 
#include "server_initializer.hpp"
	

#define PORT 8888 

	
int main() 
{ 
	//int opt = TRUE; 
	int new_socket , client_socket[30] , max_clients = 30 , activity, i , valread , sd; 
	int max_sd; 
	//struct sockaddr_in server_init.get_server_addr(); 
		
	char buffer[1025]; //data buffer of 1K 
		
	//set of socket descriptors 
	fd_set readfds; 
	
	//a response 

	std::string body = "<!DOCTYPE html><html><body><h1>My First Heading</h1><p>My first paragraph.</p></body></html>"; 
	std::string header = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(body.length()) + "\r\n\r\n";

	std::string response = header + body;
	
	//initialise all client_socket[] to 0 so not checked 
	for (i = 0; i < max_clients; i++) 
	{ 
		client_socket[i] = 0; 
	} 
	

	server_initializer server_init(PORT);
	server_init.bind_socket_port();
		
	//try to specify maximum of 3 pending connections for the master socket 
	if (listen(server_init.get_sock_server(), 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
		
	//accept the incoming connection 

	puts("Waiting for connections ..."); 
		
	while(true) 
	{ 
		//clear the socket set 
		FD_ZERO(&readfds); 
	
		//add master socket to set 
		FD_SET(server_init.get_sock_server(), &readfds); 
		max_sd = server_init.get_sock_server(); 
			
		//add child sockets to set 
		for ( i = 0 ; i < max_clients ; i++) 
		{ 
			//socket descriptor 
			sd = client_socket[i]; 
				
			//if valid socket descriptor then add to read list 
			if(sd > 0) 
				FD_SET( sd , &readfds); 
				
			//highest file descriptor number, need it for the select function 
			if(sd > max_sd) 
				max_sd = sd; 
		} 
	
		//wait for an activity on one of the sockets , timeout is NULL , 
		//so wait indefinitely 
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL); 
	
		if ((activity < 0) && (errno!=EINTR)) 
		{ 
			printf("select error"); 
		} 
			
		//If something happened on the master socket , 
		//then its an incoming connection 
		if (FD_ISSET(server_init.get_sock_server(), &readfds)) 
		{ 
			if ((new_socket = accept(server_init.get_sock_server(), (struct sockaddr *)&(server_init.get_ref_server_addr()), (socklen_t*)&server_init.get_ref_addrlen()))<0) 
			{ 
				perror("accept"); 
				exit(EXIT_FAILURE); 
			} 
			
			//inform user of socket number - used in send and receive commands 
			printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(server_init.get_server_addr().sin_addr) , ntohs (server_init.get_server_addr().sin_port)); 
		
			//send new connection greeting response 
			// if( send(new_socket, response.c_str(), response.length(), 0) != response.length() ) 
			// { 
			// 	perror("send"); 
			// } 
				
			// puts("Welcome response sent successfully"); 
				
			//add new socket to array of sockets 
			for (i = 0; i < max_clients; i++) 
			{ 
				//if position is empty 
				if( client_socket[i] == 0 ) 
				{ 
					client_socket[i] = new_socket; 
					printf("Adding to list of sockets as %d\n" , i);	
					break; 
				} 
			} 
		} 
			
		//else its some IO operation on some other socket 
		for (i = 0; i < max_clients; i++) 
		{ 
			sd = client_socket[i]; 
				
			if (FD_ISSET( sd , &readfds)) 
			{ 
				//Check if it was for closing , and also read the 
				//incoming response 
				valread = read( sd , buffer, 1024);
				std::cout << "valread = " << valread << std::endl;
				if (valread == 0) 
				{ 
					//Somebody disconnected , get his details and print 
					getpeername(sd , (struct sockaddr*)(&(server_init.get_ref_server_addr())) , (socklen_t*)&server_init.get_ref_addrlen()); 
					printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(server_init.get_server_addr().sin_addr) , ntohs(server_init.get_server_addr().sin_port)); 
						
					//Close the socket and mark as 0 in list for reuse 
					close( sd ); 
					client_socket[i] = 0; 
				} 
					
				//Echo back the response that came in 
				else
				{ 
					//set the string terminating NULL byte on the end 
					//of the data read 
					buffer[valread] = '\0'; 

					std::cout << "Host " << i << " : " << buffer << std::endl;

					send(sd , response.c_str(), response.length() , 0 ); 

				} 
			} 
		} 
	} 
		
	return 0; 
} 
