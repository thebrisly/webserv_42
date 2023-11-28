#include "../includes/ConfigParser.hpp"
#include "../includes/RunServer.hpp"
#include <fstream>

#include <signal.h>

void sigpipeHandle (int sig)
{
	std::cerr << "sigpipeHandle : PROBLEME ECRITURE SOCKET : " << sig <<std::endl;
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
