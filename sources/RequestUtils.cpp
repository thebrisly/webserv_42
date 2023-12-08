/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD
/*   By: lfabbian <lfabbian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 13:27:08 by lfabbian          #+#    #+#             */
/*   Updated: 2023/12/08 13:02:31 by lfabbian         ###   ########.fr       */
=======
/*   By: vgiordan <vgiordan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 13:27:08 by lfabbian          #+#    #+#             */
/*   Updated: 2023/12/08 10:49:25 by vgiordan         ###   ########.fr       */
>>>>>>> b143a04bfc31e562736cf52cab63e79dcb35adbd
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"
#include "../includes/Color.hpp"
#include "../includes/RunServer.hpp"

// https://en.wikipedia.org/wiki/HTTP#Request_methods
// For the webserv project we only need GET, POST and DELETE
std::vector<std::string>		Request::initMethods()
{
	std::vector<std::string>	methods;

	methods.push_back("GET");
	methods.push_back("HEAD");
	methods.push_back("POST");
	methods.push_back("PUT");
	methods.push_back("DELETE");
	methods.push_back("CONNECT");
	methods.push_back("OPTIONS");
	methods.push_back("TRACE" );

	return methods;
}


// Request parsing
// Méthode pour parser la requête
void 	Request::parseRequest(const std::string& request)
{
	// Clear existing data
	clearRequest();

	// Split the request into lines
	std::istringstream requestStream(request);
	std::string line;
	while (std::getline(requestStream, line))
	{
		if (line == "\r") //start of body
		{
			while (std::getline(requestStream, line))
				this->_body += line;
			parseUserData();
		} // Check if the line is empty (end of headers)

		if (this->_method.empty())
		{
			this->_method = readMethod(line);
			this->_version = readVersion(line);
			this->_path = readFirstLine(line);
			if (this->_path.back() == '/')
			{
				this->_path = this->_path.substr(0, this->_path.size() - 1);
			}
		}
		else
		{
			size_t colonPos = line.find(':');
			if (colonPos != std::string::npos)
            {
                std::string key = line.substr(0, colonPos);
                std::string value = line.substr(colonPos + 2); // Skip ': '
                this->_headers[key] = value;

				if (key == "Host")
                {
					//std::cout << "ici 1\n";
                    parseHostHeader(value, this->_hostname, this->_port);
                }
                else if (key == "Connection")
                {
                    this->_connection = getConnectionHeader();
                }
                else if (key == "Sec-Fetch-Dest")
                {
                    this->_secfetchdest = getSecFetchDestHeader();
                }
            }
		}
	}
}

std::string Request::readFirstLine(const std::string& line)
{
	size_t space1 = line.find(' ');
	size_t space2 = line.rfind(' ');

	if (space1 != std::string::npos && space2 != std::string::npos && space1 < space2)
	{
		return line.substr(space1 + 1, space2 - space1 - 1);
	}

	return "";
}

void Request::clearRequest()
{
	this->_headers.clear();
	this->_method.clear();
	this->_version.clear();
	this->_path.clear();
}

std::string Request::readVersion(const std::string& line)
{
	size_t space = line.rfind(' ');

	if (space != std::string::npos)
	{
		return line.substr(space + 1);
	}

	return "";
}


std::string Request::readMethod(const std::string& line)
{
	// Extract the method from the first line
	size_t space = line.find(' ');

	if (space != std::string::npos) {
		return line.substr(0, space);
	}

	return "";
}

std::string Request::getConnectionHeader() const
{
    std::map<std::string, std::string>::const_iterator it = _headers.find("Connection");
    if (it != _headers.end())
	{
        return it->second.substr(0, it->second.size() - 1);
    }
	else
	{
        return "";
    }
}

std::string Request::getHostHeader() const
{
    std::map<std::string, std::string>::const_iterator it = _headers.find("Host");
    if (it != _headers.end())
	{
        return it->second;
    }
	else
	{
        return "";
    }
}

std::string Request::getSecFetchDestHeader() const
{
    std::map<std::string, std::string>::const_iterator it = _headers.find("Sec-Fetch-Dest");
    if (it != _headers.end())
	{
        return it->second;
    }
	else
	{
        return "";
    }
}

void Request::parseHostHeader(const std::string& hostHeader, std::string& hostname, std::string& port) const
{
    std::istringstream stream(hostHeader);
    std::getline(stream, hostname, ':'); // Read until the colon
    std::getline(stream, port);          // Read the rest

    // If the port is not present, set a default value
    if (port.empty()) {
        port = "80"; // You can choose a different default port if needed
    }
}

void Request::parseUserData()
{
    // Clear existing user data
    this->_userData.clear();

    // Create a stringstream from _body
    std::istringstream bodyStream(this->_body);

    // Tokenize the body using '&' as the delimiter
    std::string token;
    while (std::getline(bodyStream, token, '&'))
    {
        // Split the token into key and value using '=' as the delimiter
        size_t equalPos = token.find('=');
        if (equalPos != std::string::npos)
        {
            std::string key = token.substr(0, equalPos);
            std::string value = token.substr(equalPos + 1);

            // Add or update the key-value pair in the map
            this->_userData[key] = value;
        }
    }
}


std::ostream& operator<<(std::ostream& os, const Request &request)
{

	//os << CYAN <<"current request : " << RESET<< request.getCurrentRequest() << std::endl;

	os << BLUE <<" -------------------- VARIABLES REQUEST -------------------- " << RESET << std::endl;
	os << CYAN <<"           path : " << RESET<< request.getPath() << std::endl;
	os << CYAN <<"         method : " << RESET<< request.getMethod() << std::endl;

	if (request.getMethod() == "POST" || request.getMethod() == "DELETE")
	{
		os << CYAN <<"current request : " << RESET<< request.getCurrentRequest() << std::endl;
	}

	os << CYAN <<"        version : " << RESET<< request.getVersion() << std::endl;
	os << CYAN <<"           host : " << RESET<< request.getHost() << std::endl;
	os << CYAN <<"     connection : " << RESET<< request.getConnection() <<std::endl;
	os << CYAN <<"   secfetchdest : " << RESET<< request.getSecFetchDest() << std::endl;
	os << CYAN <<"           port : " << RESET<< request.getPort() << std::endl;
	os << CYAN <<"       hostname : " << RESET<< request.getHostname() << std::endl;
	os << CYAN <<"           body : " << RESET<< request.getBody() << std::endl;
	os << CYAN <<"        headers : " << RESET << std::endl;

	for (std::map<std::string, std::string>::const_iterator it = request.getHeaders().begin(); it!= request.getHeaders().end(); ++it)
	{
		os << "           * " << MAGENTA << it->first << RESET << " : " << it->second << std::endl;
	}

	// Afficher les éléments avec des clés groupées
	os << CYAN <<"     user data : " << RESET << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = request.getUserData().begin(); it!= request.getUserData().end(); ++it)
	{
		os << "           * " << MAGENTA << it->first << RESET << " : " << it->second << std::endl;
	}

	os << CYAN <<"   default file : " << RESET<< request.getDefaultFile() << std::endl;

	os << CYAN <<"  server_config : " << RESET << request.getServerConfig().getServerName() << MAGENTA <<  " on port " << RESET << request.getServerConfig().getPort() << std::endl;

	os << BLUE <<" -------------------- VARIABLES RESPONSE -------------------- " << RESET << std::endl;

	os << CYAN <<"    status_code : " << RESET << request.getStatusCode() <<std::endl;
	os << CYAN <<"  status_string : " << RESET << request.getStatusString() <<std::endl;
	os << CYAN <<"response_header : " << RESET << request.getResponseHeader() <<std::endl;
	os << CYAN <<"  response_body : " << RESET << request.getResponseBody().substr(0, 100) << " ... " <<std::endl;
	os << CYAN <<"       response : " << RESET << std::endl;
	os << request.getResponse().substr(0, 200) << " ... " <<std::endl;


	return os;
}

void sendRedirectionResponse(int clientSocket, const std::string& newUrl)
{
    std::string response = "HTTP/1.1 301 Moved Permanently\r\n";
    response += "Location: " + newUrl + "\r\n";
    response += "Content-Length: 0\r\n";
    response += "Connection: close\r\n\r\n";

    send(clientSocket, response.c_str(), response.length(), 0);
}
