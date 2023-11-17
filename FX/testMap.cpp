#include <iostream>
#include <map>

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


Client::Client(int socket) : _socket(socket), _socket_mod(0), _size_response(0), _size_request(0) {};

Client::Client(): _socket(0), _socket_mod(0), _size_response(0), _size_request(0){};

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



int main ()
{
	std::map<int, Client> map_client;

	map_client.insert(std::pair<int, Client>(14, Client(14)));

	std::cout << map_client[14].get_socket() << std::endl;


	return 0;
}