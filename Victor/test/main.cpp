
#include "utils.hpp"


typedef std::map<int, std::string> MAP;


void printRoute(const RouteConfig &route)
{
	std::cout << "Path: " << route.path << std::endl;
	std::cout << "Methods: ";
	for (std::vector<std::string>::const_iterator it = route.methods.begin(); it != route.methods.end(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	std::cout << "Default file: " << route.default_file << std::endl;
	std::cout << "Directory listing: " << route.directory_listing << std::endl;
	std::cout << "Root: " << route.root << std::endl;
	std::cout << "Redirect: " << route.redirect.first << " " << route.redirect.second << std::endl;
}

int main() {

	std::vector<ServerConfig> configs;

    ConfigParser parser;
    configs = parser.parseConfigs("config.config");

	if (configs.empty())
	{
		std::cout << "No server configs found" << std::endl;
		return 0;
	}
	ServerConfig config = configs.front();			
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

	int i = 0;
	for (std::vector<RouteConfig>::const_iterator it = config.getRoutes().begin(); it != config.getRoutes().end(); ++it)
	{
		std::cout << "<----- Route " << i++ << " ------>" << std::endl;
		printRoute(*it);
	}

	const std::string &test = get_error_page(config, 403);
	std::cout << "Error page: " << test << std::endl;

	std::string route = "/";
	std::vector<std::string> v = get_allowed_methods(config, route);

	std::cout << "Allowed methods for route " << v.front() << ": ";

	// MAP::const_iterator pos = m.find(404);
	// std::cout << "Error pages: " << pos->second << std::endl;
	// pos = m.find(500);
	// std::cout << "Error pages: " << pos->second << std::endl;
	// pos = m.find(541);
	// std::cout << "Error pages: " << pos->second << std::endl;
	// pos = m.find(521);
	// std::cout << "Error pages: " << pos->second << std::endl;
}
