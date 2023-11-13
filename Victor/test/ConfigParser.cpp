#include "ConfigParser.hpp"

#include <map>
#include <string>

// Assume processLine is part of the ConfigParser class
void ConfigParser::processLine(const std::string& key, const std::string& value, ServerConfig& config, std::string& currentContext)
{
    if (key == "server_name") {
        config.setServerName(value);
    } else if (key == "port") {
        config.setPort(std::stoi(value));
    } else if (key == "ip") {
        config.setIPAddress(value);
    } else if (key == "max_body_size") {
        config.setMaxBodySize(std::stoi(value));
    } else if (key == "root") {
        config.setRoot(value);
    } else if (key == "error_page") {
        currentContext = "error_page";
    }
	else if (!currentContext.empty() && currentContext == "error_page") {
        if (key.empty() || !std::all_of(key.begin(), key.end(), ::isdigit)) {
            currentContext.clear();
            return;
        }
        std::cout << "KEY " << key << " is a valid error code" << std::endl;

        try {
            int errorCode = std::stoi(key);
            config.setErrorPage(errorCode, value);
        } catch (const std::invalid_argument& e) {
            // Handle or log error
            std::cerr << "Invalid error code in configuration: " << key << std::endl;
        }
    }

}


ServerConfig ConfigParser::parseConfig(const std::string& filename) {
	std::cout << "Parsing config file: " << filename << std::endl;
    ServerConfig serverConfig;
    std::ifstream file(filename.c_str());
    std::string line, currentContext;

    if (!file.is_open()) {
        std::cerr << "Could not open config file: " << filename << std::endl;
        // Handle error appropriately, possibly throw an exception or exit
    }

    while (std::getline(file, line)) {
        // Trim leading whitespace from the line
        line.erase(0, line.find_first_not_of(" \t"));

        // Skip empty lines or comments
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, ':')) {
            key.erase(key.find_last_not_of(" \t") + 1); // Trim trailing whitespace
            std::getline(iss, value);
            value.erase(0, value.find_first_not_of(" \t")); // Trim leading whitespace
            processLine(key, value, serverConfig, currentContext);
        }
    }

    file.close();
    return serverConfig;
}

