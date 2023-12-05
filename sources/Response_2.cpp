#include "../includes/Request.hpp"
#include "../includes/Color.hpp"
#include <unistd.h>

int Request::getLocation() const
{

	std::string path_to_test = this->_path;


	if (path_to_test.length() > 1 && path_to_test.back() == '/')
	{
		path_to_test = path_to_test.substr(0, path_to_test.size()-1);
	}

	std::cout << "path_to_test : " << path_to_test << std::endl;
	
	std::vector<RouteConfig> routes = this->_server_config.getRoutes();

	unsigned long i;
	for (i = 0; i < routes.size(); i++)
	{
		std::cout << "route " << i << " : " << routes[i].path << std::endl;

		if (routes[i].path.compare(0, routes[i].path.length(), path_to_test.substr(0, routes[i].path.length())) == 0)
		{
			std::cout << RED << "J ai trouve une location valide !" << RESET <<std::endl;
//			sleep(1);
			return i;
		}

	}


	return -1;
}
