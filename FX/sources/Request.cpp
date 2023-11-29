/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgiordan <vgiordan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 07:43:38 by lfabbian          #+#    #+#             */
/*   Updated: 2023/11/28 16:39:08 by vgiordan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"

/////////////////////////////////
// CONSTRUCTORS & DESTRUCTORS //
/////////////////////////////////

Request::Request() {}

Request::Request(const std::string& str) : _current_request(str) {}

Request::~Request() {}

Request& Request::operator=(const Request& other) 
{
        if (this != &other)
	{
                this->_current_request = other.getCurrentRequest();
                this->_path = other.getPath();
                this->_method = other.getMethod();
                this->_version = other.getVersion();
                this->_host = other.getHost();
                this->_connection = other.getConnection();
                this->_secfetchdest = other.getSecFetchDest();
                this->_port = other.getPort();
                this->_hostname = other.getHostname();
                this->_headers = other.getHeaders();
                //this->_body = other.getBo;
	}
        return *this;
}


/////////////////////////////////
///////// SETTERS & GETTERS /////
/////////////////////////////////

std::string Request::getCurrentRequest() const
{
        return (this->_current_request);
}

std::string Request::getVersion() const
{
        return (this->_version);
}

std::string Request::getMethod() const
{
        return (this->_method);
}

std::string Request::getPath() const
{
        return (this->_path);
}

const std::map<std::string, std::string>& Request::getHeaders() const
{
    return (this->_headers);
}

std::string Request::getHost() const
{
        return (this->_host);
}

std::string Request::getConnection() const
{
        return (this->_connection);
}

std::string Request::getSecFetchDest() const
{
        return (this->_secfetchdest);
}

std::string Request::getPort() const
{
        return (this->_port);
}

std::string Request::getHostname() const
{
        return (this->_hostname);
}

std::string Request::getType() const
{
        return (this->_path.substr(_path.find_last_of(".") + 1));
}

std::string Request::getDefaultFile() const
{
        return (this->_default_file);
}


