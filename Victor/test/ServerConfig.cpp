#include "ServerConfig.hpp"

// Constructor
ServerConfig::ServerConfig() : port(0), ip_address(""), max_body_size(0), server_name(""), root("") {}

// Destructor
ServerConfig::~ServerConfig() {}

// Copy Constructor
ServerConfig::ServerConfig(const ServerConfig &other) : port(other.port),
	ip_address(other.ip_address),
	max_body_size(other.max_body_size),
	server_name(other.server_name),
	error_pages(other.error_pages),
	root(other.root),
	routes(other.routes) {}

// Copy Assignment Operator
ServerConfig &ServerConfig::operator=(const ServerConfig &other) {
    if (this != &other) {
        port = other.port;
        ip_address = other.ip_address;
        max_body_size = other.max_body_size;
        server_name = other.server_name;
        error_pages = other.error_pages;
        root = other.root;
        routes = other.routes;
    }
    return *this;
}

// Setters
void ServerConfig::setPort(unsigned long p) { port = p; }
void ServerConfig::setIPAddress(const std::string& ip) { ip_address = ip; }
void ServerConfig::setMaxBodySize(unsigned long size) { max_body_size = size; }
void ServerConfig::setServerName(const std::string& name) { server_name = name; }
void ServerConfig::setErrorPage(int errorCode, const std::string& pagePath) { error_pages[errorCode] = pagePath; }
void ServerConfig::setRoot(const std::string& r) { root = r; }
void ServerConfig::addRoute(const RouteConfig& route) { routes.push_back(route); }

// Getters
unsigned long ServerConfig::getPort() const { return port; }
std::string ServerConfig::getIPAddress() const { return ip_address; }
unsigned long ServerConfig::getMaxBodySize() const { return max_body_size; }
const std::string& ServerConfig::getServerName() const { return server_name; }
const std::map<int, std::string>& ServerConfig::getErrorPages() const { return error_pages; }
const std::string& ServerConfig::getRoot() const { return root; }
const std::vector<RouteConfig>& ServerConfig::getRoutes() const { return routes; }
