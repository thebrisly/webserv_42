#include <string>
#include <vector>
#include <map>

class RouteConfig
{
    std::string path;
    std::vector<std::string> methods;
    std::string default_file;
    bool directory_listing;
};