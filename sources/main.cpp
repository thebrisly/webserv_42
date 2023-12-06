#include "../includes/ConfigParser.hpp"
#include "../includes/RunServer.hpp"

#include <signal.h>

void sigpipeHandle (int sig)
{
	std::cerr << "sigpipeHandle : PROBLEME ECRITURE SOCKET : " << sig <<std::endl;
}

int main(/*int argc, char *argv[]*/)
{

	// std::string configFile;
	// if (argc == 0)
	// {


	// }


	try
	{
		signal(SIGPIPE, sigpipeHandle);

		std::vector<ServerConfig> configs;
	    ConfigParser parser;
    	configs = parser.parseConfigs("config/config.config");

		std::cout << "Configs : " << configs << std::endl;

		ServersManager servers_manager(configs);

		RunServer run_server(servers_manager, "diary.txt");
		std::ofstream out_log("log.txt", std::ofstream::out);

		int loop_count = 0;
		while(42)
		{
			run_server.process(loop_count);
			loop_count++;
		}
	}
	catch(std::exception& e)
	{
		std::cout << "Exception : " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return 0;
}
