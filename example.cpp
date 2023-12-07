#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>

std::string executePythonScript(const char* scriptPath) {
    int pipefd[2];
    pid_t pid;
    char buf;
    std::string result;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Child process
        close(pipefd[0]);  // Close unused read end
        dup2(pipefd[1], STDOUT_FILENO);  // Redirect stdout to pipe
        close(pipefd[1]);  // Close write end after dup

        // Execute Python script
        char* argv[] = {(char*)"python", (char*)scriptPath, NULL};
        execve("/usr/bin/python", argv, NULL);

        // execve only returns on error
        perror("execve");
        exit(EXIT_FAILURE);
    } else {  // Parent process
        close(pipefd[1]);  // Close unused write end

        // Read from pipe
        while (read(pipefd[0], &buf, 1) > 0) {
            result += buf;
        }

        close(pipefd[0]);  // Close read end
        waitpid(pid, NULL, 0);  // Wait for child
    }

    return result;
}

void sendHttpResponse(int clientSocket, const std::string& content) {
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/html\r\n";
    response += "Content-Length: " + std::to_string(content.length()) + "\r\n";
    response += "\r\n";
    response += content;

    send(clientSocket, response.c_str(), response.size(), 0);
}