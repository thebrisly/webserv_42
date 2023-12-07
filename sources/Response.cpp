#include "../includes/Request.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Color.hpp"
#include <unistd.h>


// No constructor as the response is just a variable of the object "Request"

// VERIFICATIONS BEFORE SENDING THE RESPONSE :

// Créer une fonction qui renvoie la response si toutes (5) les verifications suivantes sont correctes :


void Request::checkRequest()
{
	int id_route;
	//bool is_file = this->isFile();

	if (!checkHttpVersion())
	{
		this->_status_code = 505;
		this->_status_string = "HTTP Version Not Supported";
		std::cout << "[Response.cpp] " << MAGENTA << "505 : HTTP Version Not Supported" << RESET << std::endl;
	}
	else if (!check_host_port()) 
	{
		this->_status_code = 503; //trouver le bon code erreur
		this->_status_string = "Service Unavailable";
		std::cout << "[Response.cpp] " << MAGENTA << "503 : Service Unavailable (host - port not resolved)" << RESET << std::endl;
	}
	else if ((id_route = this->getLocation()) == -1)
	{
		this->_status_code = 400; //page not found error
		this->_status_string = "Bad Request";
		std::cout << "[Response.cpp] " << MAGENTA << "400 : Bad Request" << RESET << std::endl;
	}
	else if (!checkMethods(id_route)) 
	{
		this->_status_code = 403; //wrong authorizations
		this->_status_string = "Forbidden";
		std::cout << "[Response.cpp] " << MAGENTA << "403 : Bad Request (method not allowed)" << RESET << std::endl;
	}
	else if (checkRedirection(id_route))
	{
		std::cout << "[Response.cpp] " << MAGENTA << "Redirection ..." << RESET << std::endl;
		this->checkRequest();
		return ;
	}
	else
	{
		if (this->isFile() == 1)
		{
			if (!this->issetFile(id_route))
			{
				this->_status_code = 404; //page not found error
				this->_status_string = "Not Found";
				std::cout << "[Response.cpp] "<< MAGENTA << "Response error 404 : page not found." << RESET << std::endl;
			}
			else
			{
				this->_status_code = 200;
				this->_status_string = "OK";
				std::cout << "[Response.cpp] "<< MAGENTA << "Response OK 200" << RESET << std::endl;
			}
		}
		else
		{
			/*Your playground Victor*/
		}
	}
}

void	Request::prepareResponse()
{
	const std::map<int, std::string>& errorPages = this->_server_config.getErrorPages();
	std::string 	errorPagePath;
	std::string		response;
	std::string		body;

	this->checkRequest(); //pour set le status_code et le status_string
	std::cout << "STATUS CODE :" << this->_status_code << std::endl;

	if (this->_status_code != 200) //s'il y a une erreur on envoie une page d'erreur
	{
		std::map<int, std::string>::const_iterator it = errorPages.find(this->_status_code);
        if (it != errorPages.end())
		{
            errorPagePath = this->_server_config.getRoot() + it->second;
        }
        else
		{
			std::cout << "ERREUR : page d'erreur non trouvée." << std::endl;
           errorPagePath = "web/default_error_pages/" + std::to_string(_status_code) + ".html";
        }

		std::cout << "FICHIER DE REFERENCE : " << errorPagePath << std::endl;
        std::ifstream file(errorPagePath);

		if (file.is_open())
		{
            std::ostringstream fileContent;
            fileContent << file.rdbuf();
            body = fileContent.str();

            response = this->_version + " " + std::to_string(this->_status_code) + " " + this->_status_string + "\r\n";
            response += "Content-Type: text/html\r\n";
            response += "Connection: keep-alive\r\n";
            response += "Content-Length: " + std::to_string(body.length()) + "\r\n\r\n";
            response += body;
        }
		else
		{
			std::ifstream file_rescue("web/default_error_pages/505.html");

            std::ostringstream fileContent;
            fileContent << file_rescue.rdbuf();
            body = fileContent.str();

            response = this->_version + " " + std::to_string(this->_status_code) + " " + this->_status_string + "\r\n";
            response += "Content-Type: text/html\r\n";
            response += "Connection: keep-alive\r\n";
            response += "Content-Length: " + std::to_string(body.length()) + "\r\n\r\n";
            response += body;

		}
	}

	else if (this->_status_code == 200)
	{

		std::cout << this->_server_config.getRoot() +  this->_path << std::endl;
		std::ifstream file(this->_server_config.getRoot() + _path);

		if (file.is_open()) {
			std::ostringstream fileContent;
			fileContent << file.rdbuf();
			std::string fileContentStr = fileContent.str();

			if (!fileContentStr.empty()) {
				response = _version + " 200 OK\r\n";
				response += "Content-Type: " + findMimeType(findFileType(this->_path)) + "\r\n";
				response += "Connection: keep-alive\r\n";
				response += "Content-Length: " + std::to_string(fileContentStr.length()) + "\r\n\r\n";
				response += fileContentStr;
			} else {
				response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nConnection: keep-alive\r\n\r\n404 - Not Found";
			}
		} else {
			std::ifstream file_rescue("web/default_error_pages/505.html");

            std::ostringstream fileContent;
            fileContent << file_rescue.rdbuf();
            body = fileContent.str();

            response = this->_version + " " + std::to_string(this->_status_code) + " " + this->_status_string + "\r\n";
            response += "Content-Type: text/html\r\n";
            response += "Connection: keep-alive\r\n";
            response += "Content-Length: " + std::to_string(body.length()) + "\r\n\r\n";
            response += body;
		}
	}

	this->_response = response;
}


// 1. Check version HTTP —> si marche pas : envoyer response error 505

bool Request::checkHttpVersion() const
{
	if (this->_version == "HTTP/1.1\r")
	{
		std::cout << "[Response.cpp] " << GREEN << "checkHttpVersion : OK" << RESET << std::endl; 
		return 1;
	}
	else
	{
		std::cout << "[Response.cpp] " << RED << "checkHttpVersion : KO" << RESET << std::endl; 
		return 0;
	}
}

bool Request::check_host_port() const
{
	if (this->_hostname != this->_server_config.getServerName() && this->_hostname != this->_server_config.getIPAddress())
	{
		std::cout << "[Response.cpp] " << RED << "check_host_port : KO " << this->_hostname << RESET << std::endl;
		return false;
	}

	unsigned long ul_port = std::stoul(this->_port);

	if (ul_port != this->_server_config.getPort())
	{
		std::cout << "[Response.cpp] " << RED << "check_host_port : KO " << this->_port << RESET << std::endl;
		return false;
	}

	std::cout << "[Response.cpp] " << GREEN << "check_host_port : OK " << this->_port << RESET << std::endl;
	return true;
}

bool Request::issetFile(int id_route) const
{
	(void) id_route;
	//RouteConfig route = this->_server_config.getRoutes()[id_route];


	std::string root_path_file = this->_server_config.getRoot() + this->_path;
	std::ifstream file;

	file.open(root_path_file);
	if (file)
	{
		file.close();
		std::cout << "[Response.cpp] " << GREEN << "issetFile : OK " << RESET << root_path_file << std::endl;
		return true;
	}
	else
	{
		std::cout << "[Response.cpp] " << RED << "issetFile : KO " << RESET << root_path_file << std::endl;
		return false;
	}
}


bool Request::checkMethods(int id_route) const
{
	RouteConfig route = this->_server_config.getRoutes()[id_route];
	std::vector<std::string> authorized_methods = route.methods;

	for (std::vector<std::string>::const_iterator it = authorized_methods.begin(); it!= authorized_methods.end(); ++it)
	{
		if (*it == this->_method)
		{
			std::cout << "[Response.cpp] " << GREEN << "checkMethods : OK " << RESET << std::endl;
			return true;
		}
	}

	std::cout << "[Response.cpp] " << RED << "checkMethods : OK " << RESET << std::endl;
	return false;
}

bool Request::checkRedirection(int id_route)
{
    //std::cout << "Checking for Redirection" << std::endl;
    //std::cout << "Current Path: " << this->_path << std::endl;
	std::string usingPath = this->_path;

	RouteConfig route = this->_server_config.getRoutes()[id_route];

	//std::cout << RED << "Route " << route.path << RESET << std::endl;
	// if (isFile())
	// {
	// 	std::cout << RED << "File" << RESET << std::endl;
	// 	usingPath = getDirectoryFromFilePath(this->_path);
	// 	std::cout << "Using Path: " << usingPath << std::endl;
	// }

    // Check if redirection is specified in the route configuration

	//std::cout <<  RED << "Redirect: " << route.redirect.first << " to: " << route.redirect.second << RESET << std::endl;
	std::string checking_path = this->_path;
	while (42)
	{
		for (std::vector<std::pair<std::string, std::string> >::const_iterator it = route.redirections.begin(); it!= route.redirections.end(); ++it)
		{
			std::pair<std::string, std::string> redirection = *it;
			if (redirection.first != "" && redirection.second != "")
			{
				// Check if the current path matches the path to redirect from
				if (checking_path == redirection.first)
				{
					std::cout << "[Response.cpp] " << GREEN << "checkRedirection : " << RESET << "Redirecting to " << redirection.second << " from " << redirection.first <<std::endl;
					this->_path = redirection.second;
					return true;	
				}
			}
		}
		checking_path = reducePath(checking_path);
		if (checking_path == "/")
			break;
	}

	std::cout << "[Response.cpp] " << GREEN << "checkRedirection : " << RESET <<  " No Redirection found"<< std::endl;
    return false;
}



// 1 if file, 0 if folder
bool Request::isFile() const {
    size_t lastSlashPos = this->_path.find_last_of('/');
    size_t lastDotPos = this->_path.find_last_of('.');

    // Case 1: No dot in the path, likely a directory
    if (lastDotPos == std::string::npos) {
		std::cout << "[Response.cpp] " << GREEN << "isFile : " << RESET << "not a file" << std::endl;
        return false;
    }

    // Case 2: Dot present, but no slash - could be a file without directories
    if (lastSlashPos == std::string::npos) {
		std::cout << "[Response.cpp] " << GREEN << "isFile : " << RESET << "OK file" <<std::endl;
        return true;
    }

    // Case 3: Dot after the last slash - likely a file
    if (lastDotPos > lastSlashPos) {
		std::cout << "[Response.cpp] " << GREEN << "isFile : " << RESET << "OK file" <<std::endl;
        return true;
    }

    // Default case: Likely a directory
	std::cout << "[Response.cpp] " << GREEN << "isFile : " << RESET << "not a file" <<std::endl;
    return false;
}




/*
#include <string>

vector<string> getWords(string s){
    vector<string> res;
    int pos = 0;
    while(pos < s.size()){
        pos = s.find("%20");
        res.push_back(s.substr(0,pos));
        s.erase(0,pos+3); // 3 is the length of the delimiter, "%20"
    }
    return res;
}

*/
/*
std::string getDirectoryFromFilePath(const std::string& filePath) {
    size_t lastSlashPos = filePath.find_last_of('/');

    // If the last slash is at the start or no slash is found, return the root ("/")
    if (lastSlashPos == 0 || lastSlashPos == std::string::npos) {
        return "/";
    }

    // Extract and return the directory part of the path
    return filePath.substr(0, lastSlashPos);
}
*/