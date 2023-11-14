
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <poll.h>

#define MAX_CLIENTS 30 // Maximum number of clients


int main() {
    int sock_server;
	int sock_client;
    struct sockaddr_in server_addr;
    const int addr_len = sizeof(server_addr);
    const int PORT = 8888;

    std::ifstream file("index.html");
    std::ostringstream fileStream;
    fileStream << file.rdbuf();
    std::string index_html = fileStream.str();
    file.close();

    std::string msg = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " +
                      std::to_string(index_html.length()) + "\r\n\r\n" + index_html;

    sock_server = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "Server socket = " << sock_server << std::endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    bind(sock_server, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(sock_server, MAX_CLIENTS);

    struct pollfd fds[MAX_CLIENTS + 1]; 
    int nfds = 1; 

 	bzero(&fds, sizeof(fds));

    fds[0].fd = sock_server;
    fds[0].events = POLLIN;

    while (42) 
	{
        int ret = poll(fds, nfds, -1);

        if (fds[0].revents & POLLIN) 
		{
            sock_client = accept(sock_server, (struct sockaddr *)&server_addr, (socklen_t*)&addr_len);

            std::cout << "Client connected on socket: " << sock_client << std::endl;

            for (int i = 1; i < MAX_CLIENTS + 1; i++) 
			{
                if (fds[i].fd == 0) 
				{
                    fds[i].fd = sock_client;
                    fds[i].events = POLLIN;
                    nfds++;
                    break;
                }
            }
        }

        for (int i = 1; i < nfds; i++) 
		{
			//std::cout << "nfds = " << nfds <<std::endl;
            if (fds[i].revents & POLLIN) 
			{
                char buffer[1024] = {0};
                int bytes_read = read(fds[i].fd, buffer, 1024);

                if (bytes_read > 0) 
				{
                    std::cout << "Received message from client " << fds[i].fd << ": " << buffer << std::endl;
                    write(fds[i].fd, msg.c_str(), msg.length());
                } 
				else if (bytes_read == 0) 
				{
                    std::cout << "Client disconnected on socket: " << fds[i].fd << std::endl;
					std::cout << std::endl;
                    close(fds[i].fd);
                    fds[i].fd = 0;
                    nfds--;
                }
            }
        }
    }

    close(sock_server);
    return 0;
}
