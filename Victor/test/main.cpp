#include "ConfigParser.hpp"

typedef std::map<int, std::string> MAP;

int main() {
    ConfigParser parser;
    ServerConfig config = parser.parseConfig("config.config");

	std::cout << "Server name [" << config.getServerName() << "]" << std::endl;
	std::cout << "Port [" << config.getPort() << "]" << std::endl;
	std::cout << "IP address [" << config.getIPAddress() << "]" << std::endl;
	std::cout << "Max body size [" << config.getMaxBodySize() << "]" << std::endl;
	std::cout << "Root [" << config.getRoot() << "]" << std::endl;



	const MAP& m = config.getErrorPages();

	std::cout << "Numbers of error page(s): " << m.size() << std::endl;

	for (MAP::const_iterator it = m.begin(); it!= m.end(); ++it)
	{
		std::cout << "Error pages: " << it->second << std::endl;
	}

	// MAP::const_iterator pos = m.find(404);
	// std::cout << "Error pages: " << pos->second << std::endl;
	// pos = m.find(500);
	// std::cout << "Error pages: " << pos->second << std::endl;
	// pos = m.find(541);
	// std::cout << "Error pages: " << pos->second << std::endl;
	// pos = m.find(521);
	// std::cout << "Error pages: " << pos->second << std::endl;
}
