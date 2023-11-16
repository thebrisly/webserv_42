#include "Client.hpp"

Client::Client(int socket) : _socket(socket), _socket_mod(0), _size_response(0), _size_request(0) {};

Client::~Client() {};

int Client::get_socket() const
{
	return this->_socket;
};

void Client::set_socket (int socket)
{
	this->_socket = socket;
}



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

void Client::set_response (std::string request)
{
	this->_request = request;
}

std::string Client::get_response() const
{
	return this->_request;
}

std::ostream& operator<<(std::ostream& os, const Client &cl)
{
	os << "Client on socket " << cl.get_socket() << " with mode " << cl.get_socket_mod();
	return os;
}