#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

class Route {
public:
    std::string path;
    std::string root;
    std::vector<std::string> methods;
    // ... autres propriétés ...

    // ... méthodes pour définir les propriétés ...
};

class Server {
public:
    std::string host;
    int port;
    std::vector<Route> routes;
    // ... autres propriétés ...

    // ... méthodes pour définir les propriétés ...
};

class ConfigParser {
private:
    std::vector<Server> servers;

    std::vector<std::string> split(const std::string& s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

public:
    void Parse(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;

        Server currentServer;
        Route currentRoute;

        while (std::getline(file, line)) {
            auto tokens = split(line, ':');
            if (tokens.size() < 2) continue; // Not a valid key-value pair

            std::string key = tokens[0];
            std::string value = tokens[1];

            if (key == "Server") {
				std::cout << "Server: " << value << std::endl;
                if (currentServer.host != "") { // Save the previous server before starting a new one
                    servers.push_back(currentServer);
                    currentServer = Server(); // Reset the currentServer object
                }
            } else if (key == "Host") {
                currentServer.host = value;
            } else if (key == "Port") {
                currentServer.port = std::stoi(value);
            } else if (key == "Route") {
                if (currentRoute.path != "") { // Save the previous route before starting a new one
                    currentServer.routes.push_back(currentRoute);
                    currentRoute = Route(); // Reset the currentRoute object
                }
            } else if (key == "Path") {
                currentRoute.path = value;
            } else if (key == "Root") {
                currentRoute.root = value;
            } else if (key == "Methods") {
                currentRoute.methods = split(value, ',');
            }
            // ... handle other keys ...
        }

        // Don't forget to save the last server and route read from the file
        if (currentRoute.path != "") {
            currentServer.routes.push_back(currentRoute);
        }
        if (currentServer.host != "") {
            servers.push_back(currentServer);
        }
    }

    const std::vector<Server>& GetServers() const {
        return servers;
    }
};

int main() {
    ConfigParser parser;
    parser.Parse("config.config");

	std::cout << "Servers:" << std::endl;
    const auto& servers = parser.GetServers();
    for (const auto& server : servers) {
        std::cout << "Host: " << server.host << std::endl;
        std::cout << "Port: " << server.port << std::endl;
        // ... Print other server details ...
        for (const auto& route : server.routes) {
            std::cout << "  Path: " << route.path << std::endl;
            // ... Print other route details ...
        }
    }

    return 0;
}
