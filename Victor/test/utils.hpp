#include "ConfigParser.hpp"


int countTabs(const std::string &str);
const std::string &get_error_page(ServerConfig &server, int error_code);
const std::vector<std::string> get_allowed_methods(ServerConfig &server, std::string &route);