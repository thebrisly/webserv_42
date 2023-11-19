#ifndef SERVER_INITIALIZER_HPP
#define SERVER_INITIALIZER_HPP


#include <unistd.h> //close 
#include <arpa/inet.h> // close aussi ...
#include <iostream>




class ServerInitializer
{
	private :
		int					_sock_server;
		int					_addrlen;
		struct sockaddr_in	_server_addr;
		int					_opt;

		const int			_PORT;
		const int			_size_waiting_list;
		// const std::string							_hostname;
		// const std::string							_error_page;
		// const int									_body_size;
		// const std::map<std::string, std::string>	_path_rules;

	public :
		ServerInitializer(int port, int size_waiting_list);
		~ServerInitializer();

		int						get_sock_server() const;
		struct sockaddr_in		get_server_addr();
		struct sockaddr_in &	get_ref_server_addr();

		int & 					get_ref_addrlen();

		void					bind_listen_socket_serv();
};


#endif