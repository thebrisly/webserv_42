/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbian <lfabbian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 13:27:08 by lfabbian          #+#    #+#             */
/*   Updated: 2023/11/17 15:57:01 by lfabbian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

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
