#include <string>
#include <vector>
#include <map>

struct RouteConfig
{
    std::string path;
    std::vector<std::string> methods;
    std::string default_file;
    bool directory_listing;
    std::string root;
    std::pair<std::string, std::string> redirect;
};