/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbian <lfabbian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 07:37:25 by lfabbian          #+#    #+#             */
/*   Updated: 2023/11/14 11:18:23 by lfabbian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <vector>
# include <map>
# include <string>
# include <iostream>

class Request {

    public :
        // Constructors, Destructors & Overloading operations
        Request();
        Request(const std::string& str);
		~Request();
		Request&	operator=(const Request&);


        // function to initialize http methods
        std::vector<std::string>		    initMethods();
        
        // variable to store http methods
        static	std::vector<std::string>	http_methods;
        std::map<std::string, std::string>	header; //dictionnary of keys, values
        
    private :

};

#endif