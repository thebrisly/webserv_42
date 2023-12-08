#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include <iostream>
#include "Color.hpp"
#include <unistd.h>
//#include <vector>


class CgiHandler
{
	private :
		const char* _scriptPath;
		std::string _py_body_response;
		//const std::vector<char *> _vec_args;

		CgiHandler();

	public :
		~CgiHandler();
		CgiHandler(const char* scriptPath);
		CgiHandler &operator=(CgiHandler &src);

		const char* get_scriptPath() const;
		std::string get_py_body_response() const;
		// std::vector<char *> get_vec_args() const;
		bool executePythonScript();

};



#endif