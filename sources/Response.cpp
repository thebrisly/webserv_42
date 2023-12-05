#include "../includes/Request.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Color.hpp"


// No constructor as the response is just a variable of the object "Request"

// VERIFICATIONS BEFORE SENDING THE RESPONSE :

// Créer une fonction qui renvoie la response si toutes (5) les verifications suivantes sont correctes :


void Request::checkRequest()
{
	if (!checkHttpVersion())
	{
		this->_status_code = 505;
		std::cout << MAGENTA << "Response error 505 : bad version." << RESET << std::endl;
	}

	else if (!check_host_port()) {
		this->_status_code = 505; //trouver le bon code erreur
		std::cout << MAGENTA << "Response error 500 : host - port not resolved." << RESET << std::endl;
	}

	// else if (!checkPathType()) {
	// 	this->_status_code = 505; // trouver le bon code erreur
	// }

	// else if (!issetFile())
	// {
	// 	this->_status_code = 404; //page not found error
	// 	this->_status_string + "Not Found";
	// 	std::cout << MAGENTA << "Response error 404 : page not found." << RESET << std::endl;
	// }
	// else if (!checkMethods()) {
	// 	this->_status_code = 403; //wrong authorizations
	// 	this->_status_string = "Forbidden";
	// 	std::cout << MAGENTA << "Response error 403 : forbidden method." << RESET << std::endl;
	// }
	else if (checkRedirection())
	{
		this->checkRequest();
		return ;
	}
	else
	{
		this->_status_code = 200; // if all good then return Success
		this->_status_string = " OK";
		std::cout << MAGENTA << "Response OK" << RESET << std::endl;
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
            errorPagePath = it->second;
        }
        else
		{
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

            response = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/plain\r\nConnection: close\r\n\r\n500 - Internal Server Error";
		}
		// response = this->._version + " " + this->_status_code + this->status_string + "\nContent-Type: ../web/error_pages" + this->_status_code + ".html";
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
			response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nConnection: keep-alive\r\n\r\n404 - Not Found";
		}
	}

	this->_response = response;
}


// 1. Check version HTTP —> si marche pas : envoyer response error 505

bool Request::checkHttpVersion() const
{
	if (this->_version == "HTTP/1.1\r")
		return 1;
	else
		return 0;
}

bool Request::check_host_port() const
{
	if (this->_hostname != this->_server_config.getServerName() && this->_hostname != this->_server_config.getIPAddress())
	{
		std::cerr << RED << "NO RESOLVE FOR " << this->_hostname << RESET << std::endl;
		return false;
	}

	unsigned long ul_port = std::stoul(this->_port);

	if (ul_port != this->_server_config.getPort())
	{
		std::cerr << RED << "NO RESOLVE FOR " << this->_port << RESET << std::endl;
		return false;
	}

	return true;
}

bool Request::issetFile() const
{
	std::string root = this->_server_config.getRoot();
	std::string root_path_file = root + this->_path;
	std::string file_location;
	std::vector<RouteConfig> RoutesConfig = this->_server_config.getRoutes();
	std::ifstream file;

	// std::cout << "Ressource : " << root_path_file << std::endl;

	size_t i = root_path_file.rfind('/', root_path_file.length());
	if (i != std::string::npos)
	{
		file_location = root_path_file.substr(0, i + 1);
	}
	else
	{
		std::cerr << RED << "ERROR A DEBUG !!!" << std::endl;
		return false;
	}

	for (std::vector<RouteConfig>::iterator it = RoutesConfig.begin(); it!= RoutesConfig.end(); ++it)
	{
	//std::cout << file_location << "||" << root+it->path << std::endl;
		if (file_location == root+it->path)
		{
			file.open(root_path_file);
			if (file)
			{
				//std::cout << "OUI LA RESSOURCE EST DISPO" <<std::endl;
				return true;
			}
			else
			{
				//std::cout << "NON RESSOURCE PAS DISPO" <<std::endl;
				return false;
			}
		}
	}
	return false;
}


bool Request::checkMethods() const
{
	std::string root = this->_server_config.getRoot();
	std::string root_path_file = root + this->_path;
	std::string file_location;
	std::vector<RouteConfig> RoutesConfig = this->_server_config.getRoutes();
	std::ifstream file;
	std::vector<std::string> authorized_methods;


	// std::cout << "Ressource : " << root_path_file << std::endl;

	size_t i = root_path_file.rfind('/', root_path_file.length());
	if (i != std::string::npos)
	{
		file_location = root_path_file.substr(0, i + 1);
	}
	else
	{
		std::cerr << RED << "ERROR A DEBUG !!!" << std::endl;
		return false;
	}

	for (std::vector<RouteConfig>::iterator it = RoutesConfig.begin(); it!= RoutesConfig.end(); ++it)
	{
	//std::cout << file_location << "||" << root+it->path << std::endl;
		if (file_location == root+it->path)
		{
			authorized_methods = it->methods;

			for (std::vector<std::string>::const_iterator it = authorized_methods.begin(); it!= authorized_methods.end(); ++it)
			{
				if (*it == this->_method)
				{
					// std::cout << "OUI LA METHODE EST DISPO" <<std::endl;
					return true;
				}

			}
		}
	}
	// std::cout << "NON METHODE PAS DISPO" <<std::endl;
	return false;

}


std::string getDirectoryFromFilePath(const std::string& filePath) {
    size_t lastSlashPos = filePath.find_last_of('/');

    // If the last slash is at the start or no slash is found, return the root ("/")
    if (lastSlashPos == 0 || lastSlashPos == std::string::npos) {
        return "/";
    }

    // Extract and return the directory part of the path
    return filePath.substr(0, lastSlashPos);
}

bool Request::checkRedirection()
{
    std::cout << "Checking for Redirection" << std::endl;
    std::cout << "Current Path: " << this->_path << std::endl;
	std::string usingPath = this->_path;


	if (isFile())
	{
		std::cout << RED << "File" << RESET << std::endl;
		usingPath = getDirectoryFromFilePath(this->_path);
		std::cout << "Using Path: " << usingPath << std::endl;
	}

    RouteConfig route;
    try
    {
        route = this->_server_config.getRoute(usingPath);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false; // Return false if an exception occurs
    }
    
    // Check if redirection is specified in the route configuration
	//std::cout <<  RED << "Redirect: " << route.redirect.first << " to: " << route.redirect.second << RESET << std::endl;
    if (route.redirect.first != "" && route.redirect.second != "")
    {
        // Check if the current path matches the path to redirect from
        if (usingPath == route.redirect.first)
        {
            std::cout << RED << "Redirecting to: " << RESET << route.redirect.second << std::endl;
            this->_path = route.redirect.second;
            return true;	
        }
    }

    return false;
}



// 1 if file, 0 if folder
bool Request::isFile() const {
    size_t lastSlashPos = this->_path.find_last_of('/');
    size_t lastDotPos = this->_path.find_last_of('.');

    // Case 1: No dot in the path, likely a directory
    if (lastDotPos == std::string::npos) {
        return false;
    }

    // Case 2: Dot present, but no slash - could be a file without directories
    if (lastSlashPos == std::string::npos) {
        return true;
    }

    // Case 3: Dot after the last slash - likely a file
    if (lastDotPos > lastSlashPos) {
        return true;
    }

    // Default case: Likely a directory
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

// 2a. Vérifier que le nom de domaine correspondent au port (check que host_name et port de  l’objet request = server_name et port du fichier config)
// 2b. Vérifier que l’ip corresponde au port (check que ip et port de  l’objet request)


// 3. Vérifier si le path = fichier ou dossier


// 4. Vérifier que le fichier ou dossier demandé (root + path dans request) existe —> si fichier : renvoyer fichier, si dossier renvoyer default file—> si n’existe pas, check redirection —> renvoyer le bon truc—> si marche pas :  error 404



// 5. Vérifier que l’action demandée est autorisée pour la location —> si marche pas : envoyer response error 403

