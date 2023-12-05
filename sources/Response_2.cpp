#include "../includes/Request.hpp"
#include "../includes/Color.hpp"
#include <unistd.h>

int Request::getLocation() const
{
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	std::string path_to_test = this->_path;

	// std::cout << "path_to_test : " << path_to_test << std::endl;


	if (this->isFile() == 1)
	{
		path_to_test = path_to_test.substr(0, path_to_test.find_last_of('/') + 1);
	}

	// std::cout << "path_to_test : [" << path_to_test << "]" << std::endl;
	
	std::vector<RouteConfig> routes = this->_server_config.getRoutes();

	unsigned long i;

	while (path_to_test != "/")
	{

		for (i = 0; i < routes.size(); i++)
		{
			if (routes[i].path == path_to_test)
			{
				//std::cout << RED << "J ai trouve une location valide ! " << routes[i].path << RESET <<std::endl;

				return i;
			}
		}

		path_to_test = path_to_test.substr(0, path_to_test.find_last_of('/'));

		if (path_to_test.length() == 0)
		{
			path_to_test = "/";
		}

		// std::cout << "new path_to_test : [" << path_to_test << "]" << std::endl;
		// sleep(1);

	}

	for (i = 0; i < routes.size(); i++)
	{
		if (routes[i].path == path_to_test)
		{
			//std::cout << RED << "J ai trouve une location valide ! " << routes[i].path << RESET <<std::endl;

			return i;
		}
	}	




	return -1;
}
