/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbian <lfabbian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 11:25:13 by lfabbian          #+#    #+#             */
/*   Updated: 2023/11/28 09:24:24 by lfabbian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

int main()
{
    // Request request_test;

    // std::vector<std::string> httpMethods = request_test.initMethods();

	std::string requestString = "GET / HTTP/1.1\nHost: localhost:8888\nAccept: /\nAccept-Encoding: gzip, deflate\nUser-Agent: Mozilla/5.0 (apple-x86_64-darwin19.6.0) Siege/4.1.6\nConnection: close";
	std::string requestWebString = "GET /teststyle.css HTTP/1.1\nHost: localhost:8888\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:120.0) Gecko/20100101 Firefox/120.0\nAccept: text/css,/;q=0.1\nAccept-Language: fr,fr-FR;q=0.8,en-US;q=0.5,en;q=0.3\nAccept-Encoding: gzip, deflate, br\nConnection: keep-alive\nReferer: http://localhost:8888/\nSec-Fetch-Dest: style\nSec-Fetch-Mode: no-cors\nSec-Fetch-Site: same-origin";
    Request request_test2(requestWebString);

	request_test2.parseRequest(request_test2.getCurrentRequest());

	std::cout << "Method: " << request_test2.getMethod() << std::endl;
    std::cout << "Path: " << request_test2.getPath() << std::endl;
    std::cout << "Version: " << request_test2.getVersion() << std::endl;


    /*std::cout << "\nJust printing all the methods : \n";
    std::vector<std::string> httpMethods = request_test2.initMethods();
    for (std::vector<std::string>::const_iterator it = httpMethods.begin(); it != httpMethods.end(); ++it) {
        std::cout << "HTTP Method: " << *it << std::endl;
    }*/

    std::cout << "\nJust printing all the headers : \n";
    const std::map<std::string, std::string>& headers = request_test2.getHeaders();
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
    {
        std::cout << it->first << ": " << it->second << std::endl;
    }

}
