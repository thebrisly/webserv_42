/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbian <lfabbian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 07:43:38 by lfabbian          #+#    #+#             */
/*   Updated: 2023/11/17 15:27:44 by lfabbian         ###   ########.fr       */
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
        _current_request = other._current_request;
	}
    return *this;
}


/////////////////////////////////
///////// SETTERS & GETTERS /////
/////////////////////////////////

std::string Request::getCurrentRequest() const
{
        return (_current_request);
}

std::string Request::getVersion() const
{
        return (_version);
}

std::string Request::getMethod() const
{
        return (_method);
}

std::string Request::getPath() const
{
        return (_path);
}

const std::map<std::string, std::string>& Request::getHeaders() const
{
    return _headers;
}
