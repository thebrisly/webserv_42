/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcoindre <fcoindre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 16:00:57 by lfabbian          #+#    #+#             */
/*   Updated: 2023/12/01 13:44:42 by fcoindre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"
#include "../includes/Color.hpp"

// No constructor as the response is just a variable of the object "Request"

// VERIFICATIONS BEFORE SENDING THE RESPONSE :

// Créer une fonction qui renvoie la response si toutes (5) les verifications suivantes sont correctes :


// void Request::checkRequest() const
// {
// 	if (!checkHttpVersion()) {
// 		this->_status_code = 505;
// 	}

// 	if (!ckeck_host_port()) {
// 		this->_status_code = 505; //trouver le bon code erreur
// 	}

// 	if (!checkPathType()) {
// 		this->_status_code = 505; // trouver le bon code erreur
// 	}

// 	if (!checkFileOrDirectory()) {
// 		this->_status_code = 404; //page not found error
// 	}

// 	if (!checkActionAuthorization()) {
// 		this->_status_code = 403; //worng authorizations
// 	}

// 	return 200; // if all good then return Success
// }

// std::string	prepareResponse() const {

// }


// // 1. Check version HTTP —> si marche pas : envoyer response error 505

// bool Request::checkHttpVersion()
// {
// 	if (this->_version == "HTTP/1.1")
// 		return 1;
// 	else
// 		return 0;
// }

bool Request::ckeck_host_port() const
{
	if (this->_hostname != this->_server_config.getServerName() && this->_hostname != this->_server_config.getIPAddress())
	{
		std::cerr << RED << "NO RESOLVE FOR " << this->_hostname << RESET << std::endl;
		return false;
	}

	unsigned long ul_port = std::stoul(this->_port);

	if (ul_port != this->_server_config.getPort())
	{
		std::cerr << RED << "NO RESOLVE FOR " << this->_port << RESET << std::endl;
		return false;
	}

	return true;
}

bool Request::issetFile() const
{
	std::string path_complete = this->_server_config.getRoot() + this->_path;

	


	// this->_server_config.getRoutes();


	// if (!std::filesystem::exists(path_complete))
	// {
	// 	std::cerr << RED << "NO FILE OR DIRECTORY " << path_complete << RESET << std::endl;
	// 	return false;
	// }


	std::cout << "path complete : " << path_complete << std::endl;
	
	return true;
}


// 2a. Vérifier que le nom de domaine correspondent au port (check que host_name et port de  l’objet request = server_name et port du fichier config)
// 2b. Vérifier que l’ip corresponde au port (check que ip et port de  l’objet request)


// 3. Vérifier si le path = fichier ou dossier


// 4. Vérifier que le fichier ou dossier demandé (root + path dans request) existe —> si fichier : renvoyer fichier, si dossier renvoyer default file—> si n’existe pas, check redirection —> renvoyer le bon truc—> si marche pas :  error 404



// 5. Vérifier que l’action demandée est autorisée pour la location —> si marche pas : envoyer response error 403

