#include "Client.hpp"

Client::Client(const int socket, const ServerConfig server_config) : _socket(socket), _server_config(server_config)
{

	this->_socket_mod = 0;
	this->_size_response = 0;
	this->_size_request = 0;

} 

Client::Client(): _socket(0), _socket_mod(0), _size_response(0), _size_request(0){};

Client::~Client() {};

int Client::get_socket() const
{
	return this->_socket;
};

int Client::get_size_response () const
{
	return this->_size_response;
};

void Client::set_size_response (int size_response)
{
	this->_size_response = size_response;
}

int Client::get_size_request () const
{
	return this->_size_request;
};

void Client::set_size_request (int size_request)
{
	this->_size_request = size_request;
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

void Client::set_response (std::string response)
{
	this->_response = response;
}

std::string Client::get_response() const
{
	return this->_response;
}

ServerConfig Client::get_server_config() const
{

	return this->_server_config;

}

std::ostream& operator<<(std::ostream& os, const Client &cl)
{
	os << "       _socket = " << cl.get_socket() << std::endl;
	os << "  _socket_mode = " << cl.get_socket_mod() << std::endl;
	os << "      _request = " << cl.get_request() << std::endl;
	os << " _size_request = " << cl.get_size_request() << std::endl;
	os << "     _response = " << cl.get_response() << std::endl;
	os << "_size_response = " << cl.get_size_response() << std::endl;
	os << "connected to server : " << cl.get_server_config().getPort() << std::endl;

	return os;
}