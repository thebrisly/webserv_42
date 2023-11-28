/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbian <lfabbian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 07:37:25 by lfabbian          #+#    #+#             */
/*   Updated: 2023/11/28 10:29:04 by lfabbian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <vector>
# include <map>
# include <string>
# include <iostream>
# include <sstream>

class Request {

    public :
        // Constructors, Destructors & Overloading operations
        Request();
        Request(const std::string& str);
		~Request();
		Request& operator=(const Request&);


        // function to initialize http methods
        std::vector<std::string>		            initMethods();
        void                                        clearRequest(); // pour vider le contenu et laisser de la place pour une nouvelle requete

        // parsing the request
        void                                        parseRequest(const std::string& request);
        std::string                                 readFirstLine(const std::string& line);
        std::string                                 readVersion(const std::string& line);
        std::string                                 readMethod(const std::string& line);


        // getters and setters
        std::string                                 getCurrentRequest() const;
        std::string                                 getVersion() const;
        std::string                                 getMethod() const;
        std::string                                 getPath() const;
        std::string                                 getHost() const;
        std::string                                 getConnection() const;
        std::string                                 getSecFetchDest() const;
        std::string                                 getPort() const;
        std::string                                 getHostname() const;

        const std::map<std::string, std::string>&   getHeaders() const;
        void                                        parseHostHeader(const std::string& hostHeader, std::string& hostname, std::string& port) const;

        std::string                                 getSecFetchDestHeader() const;
        std::string                                 getHostHeader() const;
        std::string                                 getConnectionHeader() const;

        // variable to store http methods
        static	std::vector<std::string>	        http_methods;

    private :

        // variables linked to the parsed request
        std::string                                 _current_request;
        std::string                                 _path;
        std::string                                 _method;
        std::string                                 _version;
        std::string                                 _host;
        std::string                                 _connection;
        std::string                                 _secfetchdest;
        std::string                                 _port;
        std::string                                 _hostname;

        std::map<std::string, std::string>	        _headers; //dictionnary of keys, values

        std::string                                 _body;

};

#endif
