#include "CgiHandler.hpp"

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <string>

void executeCgiScript(const std::string& scriptPath) {
    pid_t pid = fork();
    int pipefd[2];
    if (pid == 0) { // Child process

        // close(pipefd[0]); // Close unused read end
        // dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to write end of the pipe
        // close(pipefd[1]);
        setenv("REQUEST_METHOD", "GET", 1);
        setenv("QUERY_STRING", "param1=value1&param2=value2", 1);

        execl(scriptPath.c_str(), NULL);
        //execl("/usr/bin/python3", "python3", scriptPath.c_str(), NULL);

        perror("execl");
        exit(EXIT_FAILURE);
    } else if (pid > 0) { // Parent process
        // close(pipefd[1]);
        int status;
        waitpid(pid, &status, 0);
        //         char buffer[1024];
        // std::vector<char> output;
        // ssize_t count;
        // while ((count = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        //     output.insert(output.end(), buffer, buffer + count);
        // }
        // close(pipefd[0]);

        // Output contains the CGI script's output
        // std::string cgiOutput(output.begin(), output.end());
        // std::cout << "CGI Output: " << cgiOutput << std::endl;

        if (WIFEXITED(status)) {
            std::cout << "CGI script exited with status " << WEXITSTATUS(status) << std::endl;
        } else {
            std::cout << "CGI script did not exit properly" << std::endl;
        }
    } else {
        perror("fork Error");
    }
}

int main() {
    std::string scriptPath = "../cgi-bin/cgi.py";

    std::cout << "Executing CGI script: " << scriptPath << std::endl;
    executeCgiScript(scriptPath);

    return 0;
}
