#include "../includes/ConfigParser.hpp"
#include "../includes/RunServer.hpp"
#include "../includes/CgiHandler.hpp"

#include <signal.h>

void sigpipeHandle (int sig)
{
	std::cerr << "sigpipeHandle : PROBLEME ECRITURE SOCKET : " << sig <<std::endl;
}

bool fileExist(const std::string fileName)
{
	std::ifstream file;
	file.open(fileName);

	if (file)
	{
		file.close();
		return true;
	}
	return false;
}



int main(int argc, char *argv[])
{
	// {
	// 	/*Tests CGI FX tkt*/
	// 	const std::string scriptPath = "web/website0/script.py";
	// 	std::map<std::string, std::string> mmap_args;
	// 	mmap_args.insert(std::pair<std::string, std::string>("arg1", "value1"));
	// 	mmap_args.insert(std::pair<std::string, std::string>("arg2", "value2"));
	// 	mmap_args.insert(std::pair<std::string, std::string>("arg3", "value3"));

	// 	CgiHandler cgiHandler(scriptPath.c_str(), mmap_args);
		
	// 	cgiHandler.executePythonScript();

	// 	std::cout << cgiHandler <<std::endl;
	// }

	// exit(0);
	std::string configFile;
	if (argc == 1)
	{
		configFile = "config/config.config";

	}
	else if (argc == 2)
	{
		std::cout << "Configuration file : " << argv[1] << std::endl;
		configFile = argv[1];
		if (!fileExist(configFile))
		{
			std::cerr << RED << "Error : " << RESET << "Invalid config file" << std::endl;
			return 0;
		}
	}
	else
	{
		std::cerr << RED << "Error : " << RESET << "Invalid number of arguments" << std::endl;

	}

	try
	{
		signal(SIGPIPE, sigpipeHandle);

		std::vector<ServerConfig> configs;
	    ConfigParser parser;
    	configs = parser.parseConfigs(configFile);

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
