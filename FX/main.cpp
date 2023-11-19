#include "ConfigParser.hpp"
#include "RunServer.hpp"

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
	
    	configs = parser.parseConfigs("config.config");

		std::cout << "Number of server : " << configs.size() << std::endl;


		ServerInitializer server_init(PORT, SIZE_WAITING_LIST);
		server_init.bind_listen_socket_serv();

		//std::cout << server_init.get_sock_server() << std::endl;

		RunServer run_server(server_init);

		std::cout << MAGENTA << "Listening on socket " << server_init.get_sock_server() << " bind with port " << PORT << "\033[0m" << std::endl;
		
		while(42) 
		{
			run_server.process();
		}
	}
	catch(std::exception& e)
	{
		std::cout << "Exception : " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return 0; 
}
