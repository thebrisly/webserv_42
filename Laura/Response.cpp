/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbian <lfabbian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 16:00:57 by lfabbian          #+#    #+#             */
/*   Updated: 2023/11/17 16:03:29 by lfabbian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

/////////////////////////////////
// CONSTRUCTORS & DESTRUCTORS //
/////////////////////////////////

Response::Response() {}

Response::Response(const std::string& str) {}

Response::~Response() {}

Response& Response::operator=(const Response& other) {
	// if (this != &other)
	// {
    //     this->_ = other._;
	// }
	(void) other;
    return *this;
}
