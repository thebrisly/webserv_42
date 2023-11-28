#include "../includes/Client.hpp"
#include <fstream>

Client::Client(const int socket, const ServerConfig server_config) : _socket(socket), _server_config(server_config)
{

	//const std::string body = "<!DOCTYPE html><html><body><h1>My First Heading</h1><h2>My first paragraph.</h2><h2>My web server</h2></body></html>";
	
	std::string body;
	//std::string css;
	std::string current_line;

	std::ifstream webTest;
	webTest.open("/Users/francois-xavier/Desktop/current/FX/web/index.html");

	while (std::getline (webTest, current_line))
	{
		
		body += current_line;
		body += "\n";

	}
	
	std::cout << "BODY = " << body << std::endl;

	const std::string header = "HTTP/1.1 200 OK\nContent-Type: text/html\nConnection: close\nContent-Length: " + std::to_string(body.length()) + "\r\n\r\n";

	std::string response = header + body;

	this->_response = response;
	this->_socket_mod = 0;
	this->_size_response = response.length();
	this->_size_request = 0;

}

Client::Client(): _socket(0), _socket_mod(0), _size_response(0), _size_request(0){};

Client::~Client() {};

int Client::get_socket() const
{
	return this->_socket;
};

unsigned long Client::get_size_response () const
{
	return this->_size_response;
};

void Client::set_size_response (unsigned long size_response)
{
	this->_size_response = size_response;
}

unsigned long Client::get_size_request () const
{
	return this->_size_request;
};

void Client::set_size_request (unsigned long size_request)
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
