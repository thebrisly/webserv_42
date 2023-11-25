#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include "ServerConfig.hpp"

class Client
{
	private :
		const int	_socket;
		std::string _request;
		std::string _response;
		int _socket_mod;
		int _size_response;
		int _size_request;

		const ServerConfig	_server_config;


	public :
		Client(int socket, const ServerConfig server_config);
		Client();
		~Client();

		int get_socket() const;

		ServerConfig get_server_config() const;

		int get_size_response() const;
		void set_size_response (int size_response);

		int get_size_request() const;
		void set_size_request (int size_request);

		int get_socket_mod() const;
		void set_socket_mod (int socket_mod);

		std::string get_request() const;
		void set_request (std::string request);

		std::string get_response() const;
		void set_response (std::string response);

};

std::ostream& operator<<(std::ostream& os, const Client &cl);


#endif