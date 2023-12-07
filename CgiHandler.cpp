#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include "includes/Color.hpp"
#include <unistd.h>

// bool executePythonScript(const char* scriptPath) 
// {




// }

 /*
 pipefd[0] => READ
 pipefd[1] => WRITE
 */

int main()
{

	int pid;
	const char *scriptPath = "script.py";
    char* args[] = {(char*)"python", (char*)scriptPath, NULL};
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


    	execve("/usr/bin/python", args, NULL);
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






