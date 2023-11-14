#include <fstream>
#include <sstream>
#include <iostream>
#include "ServerConfig.hpp"

class ConfigParser
{
	public:
		ServerConfig parseConfig(const std::string& filename);
	private:
		void processLine(const std::string& key, const std::string& value, ServerConfig& config, std::string& currentContext);
};
