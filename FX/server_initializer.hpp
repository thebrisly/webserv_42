#ifndef SERVER_INITIALIZER_HPP
#define SERVER_INITIALIZER_HPP

#include <stdio.h> 
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


		int get_sock_server();
		struct sockaddr_in get_server_addr();
		struct sockaddr_in & get_ref_server_addr();

		int & get_ref_addrlen();


		int bind_socket_port();

};


#endif