/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbian <lfabbian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 07:43:38 by lfabbian          #+#    #+#             */
/*   Updated: 2023/11/17 15:54:23 by lfabbian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

/////////////////////////////////
// CONSTRUCTORS & DESTRUCTORS //
/////////////////////////////////

Request::Request() {}

Request::Request(const std::string& str) : _current_request(str) {}

Request::~Request() {}

Request& Request::operator=(const Request& other) {
	if (this != &other)
	{
        this->_current_request = other._current_request;
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
