#include "../includes/CgiHandler.hpp"

CgiHandler::~CgiHandler(){}

CgiHandler::CgiHandler(const char* scriptPath) : _scriptPath(scriptPath)
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

// std::string CgiHandler::get_vec_args() const
// {
// 	return this->_vec_args;
// }

bool CgiHandler::executePythonScript()
{

	int pid;
    char* args[] = {(char*)"python", (char*)this->_scriptPath, (char*)"Hello from Python Script", NULL};
	int pipefd[2];
	char buf;

	if (pipe(pipefd) == -1) 
	{
		std::cerr << "CgiHandler.cpp :" << " executePythonScript" << std::endl;
		perror("pipe");
		return false;
	}

	//std::cout << pipefd[0] << " " << pipefd[1] << std::endl;

	pid = fork();
	if (pid == -1) 
	{
		std::cerr << "CgiHandler.cpp :" << " executePythonScript" << std::endl;
		perror("fork");
		return false;
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
		return false;
	}
	else 
	{
		close(pipefd[1]);
		waitpid(pid, NULL, 0);
		while(read(pipefd[0], &buf, 1) > 0)
		{
			this->_py_body_response += buf;
		}
		std::cerr << GREEN << "CgiHandler.cpp :" << RESET << this->_py_body_response << std::endl;
		close(pipefd[0]);
		return true;
	}
}

 /*
 pipefd[0] => READ
 pipefd[1] => WRITE
 */








