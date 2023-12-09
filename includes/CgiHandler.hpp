#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include <iostream>
#include "Color.hpp"
#include <unistd.h>
//#include <vector>
#include <map>

#define MAX_ARGS 100


class CgiHandler
{
	private :
		const char* _scriptPath;
		const std::map<std::string, std::string> _mmap_args;
		std::string _py_body_response;
		char *args[MAX_ARGS];

		bool transform_map_to_strArray();

		CgiHandler();

	public :
		~CgiHandler();
		CgiHandler(const char* scriptPath, const std::map<std::string, std::string> mmap_args);
		CgiHandler &operator=(CgiHandler &src);

		const char* get_arg(int i) const;
		const char* get_scriptPath() const;
		std::string get_py_body_response() const;


		std::map<std::string, std::string> get_mmap_args() const;
		bool executePythonScript();

};

std::ostream& operator<<(std::ostream& os, const CgiHandler &cgi_hl);


#endif

/*
#include <string>
#include <cstring>
#include <iostream>
#include <vector>

int main() {
    std::vector<std::string> strings;
    strings.push_back("Hello");
    strings.push_back("World");
    strings.push_back("Example");
    strings.push_back("C++");

    // The array of char pointers
    char** args = new char*[strings.size()];

    // Dynamically allocate and copy each string
    for (size_t i = 0; i < strings.size(); ++i) {
        args[i] = new char[strings[i].size() + 1];
        std::strcpy(args[i], strings[i].c_str());
    }

    // Use the args array
    for (size_t i = 0; i < strings.size(); ++i) {
        std::cout << args[i] << std::endl;
    }

    // Deallocate memory to prevent memory leaks
    for (size_t i = 0; i < strings.size(); ++i) {
        delete[] args[i];
    }
    delete[] args;

    return 0;
}




*/