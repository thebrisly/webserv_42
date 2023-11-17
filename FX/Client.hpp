#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client
{
	private :
		int	_socket;
		std::string _request;
		std::string _response;
		int _socket_mod;
		int _size_response;
		int _size_request;

	public :
		Client(int socket);
		Client();
		~Client();

		int get_socket() const;
		void set_socket (int socket);

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