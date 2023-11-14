/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbian <lfabbian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 07:42:14 by lfabbian          #+#    #+#             */
/*   Updated: 2023/11/14 11:21:19 by lfabbian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.cpp"

int main()
{
    Request request_test;

    std::vector<std::string> httpMethods = request_test.initMethods();

    for (const auto& method : httpMethods) {
        std::cout << "HTTP Method: " << method << std::endl;
    }

}
