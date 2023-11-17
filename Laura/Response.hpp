/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbian <lfabbian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 16:00:41 by lfabbian          #+#    #+#             */
/*   Updated: 2023/11/17 16:18:02 by lfabbian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <vector>
# include <map>
# include <string>
# include <iostream>
# include <sstream>

class Response
{

    public :

        // Constructors, Destructors & Overloading operations
        Response();
        Response(const std::string& str);
		~Response();
		Response& operator=(const Response&);

	private :

		int											_status_code;
		std::map<std::string, std::string> 			_headers;
    	std::string 								_body;
};

#endif
