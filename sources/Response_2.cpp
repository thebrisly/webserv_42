#include "../includes/Request.hpp"


int Request::getLocation() const
{

	const std::string path_to_test = this->_path;

	std::vector<RouteConfig> routes = this->_server_config.getRoutes();

	for (unsigned long i = 0; i < routes.size(); i++)
	{
		std::cout << "route " << i << " : " << routes[i].path << std::endl;

		if (routes[i].path.length() <= this->getPath().length())
		{
			std::cout << routes[i].path.compare(0, routes[i].path.length(), path_to_test) << std::endl;
		}
	}

	/*
	
	int main() 
	{

		const std::string location_path = "/home/user/Desktop/";

		std::string str = "/home/user/Desktop/test.txt";

		if (str.length() >= location_path.length())
		{
			std::cout << location_path.compare(0, location_path.length(), str.substr(0,location_path.length())) << std::endl;

		}
		return 0;

	}
	
	*/





	exit(0);

	return 2147483647;
}

/*
bool compare_str_reference(const std::string& ref, const std::string& str) {
    if (ref.length() > str.length()) {
        return false;
    }
    return str.compare(0, ref.length(), ref) == 0;
}


*/