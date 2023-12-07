#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include "includes/Color.hpp"
#include <unistd.h>
#include <vector>


class CgiHandler
{
	private :
		const char* _scriptPath;
		bool executePythonScript();
		std::string _py_body_response;
		const std::vector<char *> _vec_args;

		CgiHandler();

	public :
		~CgiHandler();
		CgiHandler(const char* scriptPath, const std::vector<char *> vec_args);
		CgiHandler &operator=(CgiHandler &src);

		const char* get_scriptPath() const;
		std::string get_py_body_response() const;
		std::vector<char *> get_vec_args() const;

};

CgiHandler::~CgiHandler(){}

CgiHandler::CgiHandler(const char* scriptPath, const std::vector<char *> vec_args) : _scriptPath(scriptPath), _vec_args(vec_args)
{
}

const char* CgiHandler::get_scriptPath() const
{
	return this->_scriptPath;
}

std::string CgiHandler::get_py_body_response() const
{
	return this->_py_body_response;
}

std::string CgiHandler::get_vec_args() const
{
	return this->_vec_args;
}


bool CgiHandler::executePythonScript()
{
	return true;
}



 /*
 pipefd[0] => READ
 pipefd[1] => WRITE
 */

int main()
{

	int pid;
	const char *scriptPath = "script.py";
    char* args[] = {(char*)"python", (char*)scriptPath, (char*)"Hello from Python Script", NULL};
	int pipefd[2];
	char buf;
	std::string result;


	if (pipe(pipefd) == -1) 
	{
		std::cerr << "CgiHandler.cpp :" << " executePythonScript" << std::endl;
		perror("pipe");
	}

	//std::cout << pipefd[0] << " " << pipefd[1] << std::endl;

	pid = fork();
	if (pid == -1) {

		std::cerr << "CgiHandler.cpp :" << " executePythonScript" << std::endl;
		perror("fork");

	}
	if (pid == 0) 
	{
		std::cerr << "CgiHandler.cpp :" << GREEN << " executePythonScript " << RESET <<  "Child process" << std::endl;

		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);


    	execve("/usr/local/bin/python3", args, NULL);
		std::cerr << "CgiHandler.cpp :" << " executePythonScript" << std::endl;
		perror("execve");		
	
	}
	else 
	{
		close(pipefd[1]);
		waitpid(pid, NULL, 0);
		while(read(pipefd[0], &buf, 1) > 0)
		{
			result += buf;
		}
		std::cerr << GREEN << "CgiHandler.cpp :" << RESET << result << std::endl;
		close(pipefd[0]);
	}


}






