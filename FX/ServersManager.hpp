#ifndef SERVERS_MANAGER_HPP
#define SERVERS_MANAGER_HPP

#include <vector>
#include "ServerInitializer.hpp"

class ServersManager
{
	private:
		const std::vector<ServerConfig>		_configs;
		std::vector<ServerInitializer>		_servers;

	public:
		ServersManager(const std::vector<ServerConfig> configs);
		~ServersManager();

		ServerInitializer & get_server_init(int i);

		std::vector<ServerInitializer> & get_servers();

		ServerInitializer & operator[](int i);


};


#endif