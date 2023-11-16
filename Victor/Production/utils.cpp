#include "utils.hpp"

int countTabs(const std::string &str)
{
	int tabCount = 0;
	for (int i = 0; i < (int) str.size(); i++)
	{
		if (str[i] == '\t')
			tabCount++;
		else
			break;

	}
	return tabCount;
}

const std::string& get_error_page(ServerConfig& server, int error_code)
{
    const std::map<int, std::string>& errorPages = server.getErrorPages();
    std::map<int, std::string>::const_iterator it = errorPages.find(error_code);
    if (it == errorPages.end()) {
        std::map<int, std::string>::const_iterator default_it = errorPages.find(500);
        if (default_it != errorPages.end()) {
            return default_it->second;
        } else {
			throw std::runtime_error("Error page not found");
            // static const std::string defaultErrorPage = "Page Not Found";
            // return defaultErrorPage;
        }
    }
    return it->second;
}

const std::vector<std::string> get_allowed_methods(ServerConfig &server, std::string &route)
{
	std::vector<RouteConfig> routes = server.getRoutes();
	for (std::vector<RouteConfig>::iterator it = routes.begin(); it != routes.end(); ++it)
	{
		if (it->path == route)
		{
			return it->methods;
		}
	}
	throw std::runtime_error("Route not found");
}