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
		void processLine(const std::string& key, const std::string& value, ServerConfig& config);
		void processServer(const std::string& key, const std::string& value, ServerConfig& config);
		void processErrorPages(const std::string& key, const std::string& value, ServerConfig& config);
		void processLocation(const std::string& key, const std::string& value, RouteConfig& route);
		void processRedirect(const std::string& key, const std::string& value, RouteConfig& route);
};
