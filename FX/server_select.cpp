//Example code: A simple server side code, which echos back the received response. 
//Handle multiple socket connections with select and fd_set on Linux 
#include <stdio.h> 
#include <string.h> //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> //close 
#include <arpa/inet.h> //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 
#include <iostream>
#include <string>
	
#define TRUE 1 
#define FALSE 0 
#define PORT 8888 


class server_initializer
{

	private :
		int _sock_server;
		int _addrlen;
		struct sockaddr_in _server_addr;
		int opt;
		const int _PORT;

	public :
		server_initializer(int port);
		~server_initializer();

		void set_addrlen();
		void set_server_addr();
		void set_sock_server();


		int get_sock_server();
		struct sockaddr_in get_server_addr();
		struct sockaddr_in & get_ref_server_addr();

		int & get_ref_addrlen();


		int bind_socket_port();

};


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


	this->set_server_addr();

	this->set_addrlen();

}

server_initializer::~server_initializer(){};

void server_initializer::set_addrlen()
{
	this->_addrlen = sizeof(this->_server_addr);
}

void server_initializer::set_server_addr()
{
	this->_server_addr.sin_family = AF_INET;
	this->_server_addr.sin_addr.s_addr = INADDR_ANY;
	this->_server_addr.sin_port = htons(this->_PORT);
}

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

	
int main(int argc , char *argv[]) 
{ 
	//int opt = TRUE; 
	int addrlen , new_socket , client_socket[30] , max_clients = 30 , activity, i , valread , sd; 
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


	
	//type of socket created 
	//server_init.get_server_addr() = server_init.get_server_addr();



	//bind the socket to localhost port 8888 


	server_init.bind_socket_port();
		
	//try to specify maximum of 3 pending connections for the master socket 
	if (listen(server_init.get_sock_server(), 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
		
	//accept the incoming connection 
	addrlen = sizeof(server_init.get_server_addr()); 
	puts("Waiting for connections ..."); 
		
	while(TRUE) 
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
			if ((new_socket = accept(server_init.get_sock_server(), (struct sockaddr *)&(server_init.get_ref_server_addr()), (socklen_t*)&addrlen))<0) 
			{ 
				perror("accept"); 
				exit(EXIT_FAILURE); 
			} 
			
			//inform user of socket number - used in send and receive commands 
			printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(server_init.get_server_addr().sin_addr) , ntohs (server_init.get_server_addr().sin_port)); 
		
			//send new connection greeting response 
			if( send(new_socket, response.c_str(), response.length(), 0) != response.length() ) 
			{ 
				perror("send"); 
			} 
				
			puts("Welcome response sent successfully"); 
				
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
					getpeername(sd , (struct sockaddr*)(&(server_init.get_ref_server_addr())) , (socklen_t*)&addrlen); 
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
