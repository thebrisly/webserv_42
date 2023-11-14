#include <fstream>
#include <sstream>
#include <iostream>
#include <stack>
#include "ServerConfig.hpp"

class ConfigParser
{
	public:
		std::vector<ServerConfig> parseConfigs(const std::string& filename);
	private:
		void processLine(const std::string& key, const std::string& value, ServerConfig& config, std::string& currentContext);
		void processServer(const std::string& key, const std::string& value, ServerConfig& config, std::string& currentContext);
		void processErrorPages(const std::string& key, const std::string& value, ServerConfig& config, std::string& currentContext);
		void processLocation(const std::string& key, const std::string& value, ServerConfig& config, std::string& currentContext);
};
