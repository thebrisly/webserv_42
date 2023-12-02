#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <vector>
# include <map>
# include <string>
# include <iostream>
# include <sstream>
# include <fstream>

#include "ServerConfig.hpp"

class Request {

    public :
        // Constructors, Destructors & Overloading operations
        Request();
        Request(const std::string& str, const ServerConfig server_config);
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
        std::string                                 getType() const;
        std::string                                 getDefaultFile() const;
        std::string                                 getResponse() const;


        const std::map<std::string, std::string>&   getHeaders() const;
        void                                        parseHostHeader(const std::string& hostHeader, std::string& hostname, std::string& port) const;

        std::string                                 getSecFetchDestHeader() const;
        std::string                                 getHostHeader() const;
        std::string                                 getConnectionHeader() const;

        void                                        checkRequest();
        bool                                        checkHttpVersion() const;
        bool                                        check_host_port() const;
        bool                                        checkPathType() const;
        bool                                        checkFileOrDirectory() const;
        bool                                        checkActionAuthorization() const;
        bool                                        checkMethods() const;

        void    	                                prepareResponse();


        bool                                        fileIsAvaible() const;


        bool                                        issetFile() const;
        bool                                        isFile() const;

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

        std::string                                 _response;
        int                                         _status_code;
        std::string                                 _status_string;

        std::map<std::string, std::string>	        _headers; //dictionnary of keys, values

        std::string                                 _response_body;
        std::string                                 _response_header;

        std::string                                 _default_file;

        const ServerConfig                          _server_config;




};

std::ostream& operator<<(std::ostream& os, const Request &request);

#endif