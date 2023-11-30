/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcoindre <fcoindre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 13:27:08 by lfabbian          #+#    #+#             */
/*   Updated: 2023/11/30 18:11:22 by fcoindre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"

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
		if (line.empty()) { break; } // Check if the line is empty (end of headers)

		if (this->_method.empty())
		{
			this->_method = readMethod(line);
			this->_version = readVersion(line);
			this->_path = readFirstLine(line);
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

	// TODO: Parse body if needed
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


bool Request::isMethodAllowed() const
{

	if (this->_server_config.issetRoute(this->_path))
	{

		//std::cout << "Path existe "<< std::endl;
		std::vector<std::string> authorized_method = this->_server_config.getRoute(this->_path).methods;

		//std::cout << "test 0 " << authorized_method[0] << std:: endl;


		for (std::vector<std::string>::const_iterator it = authorized_method.begin(); it!= authorized_method.end(); ++it)
		{
			//std::cout << *it << " " << this->_method << std::endl;
			if (*it == this->_method)
			{
				return true;
			}
		}
	}
	else
	{
		//std::cout << "Path existe PAS"<< std::endl;

		std::vector<std::string> authorized_method = this->_server_config.getRoute("/").methods;

		for (std::vector<std::string>::const_iterator it = authorized_method.begin(); it!= authorized_method.end(); ++it)
		{
			if (*it == this->_method)
			{
				return true;
			}
		}

	}
	return false;
}


std::string Request::calculateResponse()
{

	//std::cout << this->_method << this->getMethod() << std::endl;

	std::cout << this->_path << this->getPath() << std::endl;

	//std::cout << "Path existe ? : " <<this->_server_config.issetRoute(this->_path) << std::endl;
	


	return "";
}
