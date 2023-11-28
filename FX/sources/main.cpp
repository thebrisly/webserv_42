#include "../includes/ConfigParser.hpp"
#include "../includes/RunServer.hpp"
#include <fstream>

#include <signal.h>

void sigpipeHandle (int sig)
{
	std::cerr << "sigpipeHandle : PROBLEME ECRITURE SOCKET : " << sig <<std::endl;
}

ServersManager::ServersManager(const std::vector<ServerConfig> configs) : _configs(configs)
{
	for (std::vector<ServerConfig>::const_iterator it = configs.begin(); it!= configs.end(); ++it)
	{
		ServerInitializer server_init(*it, SIZE_WAITING_LIST);
		this->_servers.push_back(server_init);
	}
};

ServersManager::~ServersManager(){};

ServerInitializer & ServersManager::get_server_init(int i)
{
	return this->_servers[i];
}

ServerInitializer & ServersManager::operator[](int i)
{
	return this->_servers[i];
}

std::vector<ServerInitializer> & ServersManager::get_servers()
{
	return this->_servers;
};

int ServersManager::get_socket_max() const
{
	int max = 0;

	for (std::vector<ServerInitializer>::const_iterator it = this->_servers.begin(); it!= this->_servers.end(); ++it)
	{
		if (it->get_sock_server() > max)
		{
			max = it->get_sock_server();
		}
	}
	return max;
}

bool ServersManager::is_server_active(const int i) const
{
	for (std::vector<ServerInitializer>::const_iterator it = this->_servers.begin(); it!= this->_servers.end(); ++it)
	{
		if (it->get_sock_server() == i)
		{
			return true;
		}
	}
	return false;
}

ServerInitializer & ServersManager::get_server_by_sock(const int sock)
{
	for (std::vector<ServerInitializer>::iterator it = this->_servers.begin(); it!= this->_servers.end(); ++it)
	{
		if (it->get_sock_server() == sock)
		{
			return *it;
		}
	}
	throw std::runtime_error("Could not find server by socket");
}

int main()
{
	try
	{
		signal(SIGPIPE, sigpipeHandle);

		std::vector<ServerConfig> configs;
	    ConfigParser parser;
    	configs = parser.parseConfigs("/Users/fcoindre/Desktop/webserv/FX/config/config.config");

		ServersManager servers_manager(configs);

		RunServer run_server(servers_manager);
		std::ofstream out("log.txt", std::ofstream::out);

		while(42)
		{
			run_server.process(out);
		}
	}
	catch(std::exception& e)
	{
		std::cout << "Exception : " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return 0;
}
