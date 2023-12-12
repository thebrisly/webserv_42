#include "../includes/Request.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Color.hpp"
#include "../includes/CgiHandler.hpp"
#include <unistd.h>
#include <dirent.h>


// No constructor as the response is just a variable of the object "Request"

// VERIFICATIONS BEFORE SENDING THE RESPONSE :

// Créer une fonction qui renvoie la response si toutes (5) les verifications suivantes sont correctes :


void Request::checkRequest()
{
	int id_route;

	//bool is_file = this->isFile();


	//std::cout << "[response.cpp]" GREEN << " checkRequest " << RESET << "is_file : " << is_file << std::endl;
	if (!checkHttpVersion())
	{
		this->_status_code = 505;
		this->_status_string = "HTTP Version Not Supported";
		std::cout << "[Response.cpp] " << MAGENTA << "505 : HTTP Version Not Supported" << RESET << std::endl;
	}
	if (!check_host_port()) 
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
		this->_status_code = 301; //redirection (error) code

		//this->checkRequest();
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
			// else if (findFileType(this->_path) == "py")
			// {
			// 	this->_status_code = 200;
			// 	this->_status_string = "OK";
			// 	this->_is_cgi = true;
			// 	std::cout << "[Response.cpp] "<< MAGENTA << "checkRequest " << RESET << "CGI asked" << std::endl;

				// const std::string scriptPath = "web/website0/script.py";
				// std::map<std::string, std::string> mmap_args;
				// mmap_args.insert(std::pair<std::string, std::string>("arg1", "value1"));
				// mmap_args.insert(std::pair<std::string, std::string>("arg2", "value2"));
				// mmap_args.insert(std::pair<std::string, std::string>("arg3", "value3"));

				// CgiHandler cgiHandler(scriptPath.c_str(), mmap_args);
				
				// cgiHandler.executePythonScript();

				// std::cout << cgiHandler <<std::endl;

				/*CgiHandler...*/
			// }
			else
			{
				this->_status_code = 200;
				this->_status_string = "OK";
				std::cout << "[Response.cpp] "<< MAGENTA << "Response OK 200" << RESET << std::endl;
			}
		}
		else
		{
			
			this->_status_code = 200;
			this->_status_string = "OK";
		}
	}
}

void	Request::prepareResponse()
{
	this->checkRequest();
	std::string		response;
	std::string		body;
	std::ostringstream fileContent;
	std::ifstream file;
	if (this->isFile())
	{
		std::cout << "[Response.cpp] " << MAGENTA << "prepare response " << RESET << "considered as a file." << std::endl;
		file.open(this->_server_config.getRoot() + this->_path);
	}
	std::string fileType;

	fileType = findMimeType(findFileType(this->_path));

	std::cout <<"[response.cpp]"<< GREEN << " createResponse : "<< RESET << this->_path << std::endl;
	std::cout << "Status code : " << this->_status_code << std::endl;
	if (_status_code == 200 || _status_code == 301)
	{
		std::cout << "[Response.cpp]" << MAGENTA << " prepare response " << RESET << " code is 200 " << std::endl;

		if (file.is_open())
		{
			std::cout << GREEN << "FILE FOUND" << RESET << std::endl;
			if (findFileType(this->_path) == "py")
			{
				file.close();
				std::cout << "[Response.cpp]" << MAGENTA << " prepare response " << RESET << "is a python file." << std::endl;
				fileType = "text/html";

				const std::string scriptPath = this->_server_config.getRoot() + this->_path;
				std::map<std::string, std::string> mmap_args;
				mmap_args.insert(std::pair<std::string, std::string>("arg1", "value1"));
				mmap_args.insert(std::pair<std::string, std::string>("arg1", "value2"));


				//mmap_args.insert(std::pair<std::string, std::string>("arg2", "value2"));
				//mmap_args.insert(std::pair<std::string, std::string>("arg3", "value3"));

				CgiHandler cgiHandler(scriptPath.c_str(), this->_userData, this->_method);
				
				cgiHandler.executePythonScript();

				// std::cout << cgiHandler <<std::endl;

				body = cgiHandler.get_py_body_response() ;
				//std::cout << "[Response.cpp]" << MAGENTA << " prepare response " << RESET << "MIME type = " << fileType << std::endl;
			}
			else
			{
				fileContent << file.rdbuf();
				body = fileContent.str();
				file.close();
			}



		}
		else //Not a file in a directory
		{
			bool directory_listing;
			std::cout << "[Response.cpp]" << MAGENTA << " prepare response " << RESET << "is a directory." << std::endl;
			try
			{
				directory_listing = this->_server_config.getRoute(this->_server_config.getRoot() + this->_path).directory_listing;
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
			fileType = "text/html";
			if (directory_listing == false)
			{
				std::ifstream file(this->_server_config.getRoot() + this->_server_config.getDefaultFile());
				std::ostringstream fileContent;
				if (file.is_open())
				{
					fileContent << file.rdbuf();
					body = fileContent.str();
					//fileType = findMimeType(this->_server_config.getDefaultFile());
				}
				else
				{
					std::cerr << "ERROR : Cannot read default file : " << this->_server_config.getRoot() + this->_server_config.getDefaultFile() << std::endl;
				}
			}
			else
			{
				DIR *dir;
				struct dirent *ent;
				std::string dirPath = this->_server_config.getRoot() + this->_path;
				dir = opendir(dirPath.c_str());
				fileType = "text/html";
				if (dir != NULL)
				{
					//fileType = "text/html";
					body = "<html><head><title>Directory Listing</title></head><body>";
					body += "<h1>Directory Listing of " + dirPath + "</h1>";
					body += "<ul>";

					while ((ent = readdir(dir)) != NULL)
					{
						body += "<li><a href='" + std::string(ent->d_name) + "'>" + std::string(ent->d_name) + "</a></li>";
					}

					body += "</ul></body></html>";
					closedir(dir);
					std::cout << "[Response.cpp]" << MAGENTA << " prepare response " << RESET << "MIME type = " << fileType << std::endl;
				}
			}			
		}
	}
	else
	{	
		file.close();
		std::cout << YELLOW << "FILE NOT FOUND" << RESET << std::endl;
		std::map<int, std::string> errorPages = this->_server_config.getErrorPages();
		std::map<int, std::string>::const_iterator it = errorPages.find(this->_status_code);
		if (it == errorPages.end())
		{
			fileType = "text/html";
			std::cout << YELLOW << "2" << RESET << std::endl;
			body = "<!DOCTYPE>\n<html>\n<header></header><body> " + std::to_string(this->_status_code) + " - Status Code </body> </html>";
		}
		else
		{
			std::ifstream errorfile;
			std::ostringstream errorFileContent;
			std::cout << this->_server_config.getRoot() + it->second << std::endl;
			body = "";
			std::cout << it->second << std::endl;
			errorfile.open(this->_server_config.getRoot() + it->second);
			errorFileContent << errorfile.rdbuf();
			body = errorFileContent.str();
			std::cout << RED << body << RESET << std::endl;
			fileType = "text/html";
			//this->_path = this->_server_config.getRoot() + it->second;
		}
	}


	

	response = "HTTP/1.1 " + std::to_string(this->_status_code) + " " + this->_status_string + "\r\n";
	response += "Content-Type: "+fileType+"\r\n";
	response += "Content-Length: " + std::to_string(body.size()) + "\r\n";
	response += "Connection: keep-alive\r\n";
	response += "Location: " + this->_path + "\r\n\r\n";
	response += body;

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
	(void) id_route;
	std::cout << "[Response.cpp] " << GREEN << "checkRedirection " <<RESET << " Checking for redirection for ["<< this->_path << "]" <<std::endl;
    //std::cout << "Current Path: " << this->_path << std::endl;
	std::string usingPath = this->_path;

	if (this->_path == "")
		usingPath = "/";
	RouteConfig route;// = this->_server_config.getRoutes()[id_route];
	try
	{
		route = this->_server_config.getRoute(usingPath);
		
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		std::cout << "[Response.cpp] " << RED << "checkRedirection " << RESET << " No Redirection found" << std::endl;
		return false;
	}
	
	std::cout << "[Response.cpp] " << GREEN << "checkRedirection " << RESET << RED << "Redirection to " << RESET << route.redirection << std::endl;
	if (route.redirection != "")
	{

		this->_path = route.redirection;
		std::cout << "[Response.cpp] " << GREEN << "checkRedirection " << RESET << RED << "Redirection to " << RESET << route.redirection << std::endl;
		return true;

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