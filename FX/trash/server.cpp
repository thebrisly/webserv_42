#include <iostream>
#include <netinet/in.h>
#include <libc.h>
#include <poll.h>
#include <sys/socket.h>

#define SERVER_PORT 8888
#define MAX_CLIENT 200
#define BUFFER_SIZE 1024


int main ()
{
	std::cout << "SERVER TEST" << std::endl;

	int listen_sd;
	int rc;
	int on = 1;
	struct sockaddr_in addr;
	struct pollfd fds[MAX_CLIENT];
	int nfds = 1;
	int current_size = 0;
	int i, j;
	bool end_server = false;
	int new_sd = -1;

	char buffer[BUFFER_SIZE];
	bool close_conn;

	int len;
	int len_response;

	std::string page = "Hello";
	len_response = page.length();	
	std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(len_response) + "\r\n\r\n" + page;

	bool compress_array;

	listen_sd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sd < 0)
	{
		std::cerr << "socket() failed" << std::endl;
		exit (EXIT_FAILURE);
	}

	rc = setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	if (rc < 0)
	{
		std::cerr << "setsockopt() failed" << std::endl;
		close(listen_sd);
		exit (EXIT_FAILURE);
	}

	
	rc = ioctl(listen_sd, FIONBIO, (char *)&on);
	if (rc < 0)
	{
		perror("ioctl() failed");
		close(listen_sd);
		exit(-1);
	}
	

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(SERVER_PORT);

	rc = bind (listen_sd, (struct sockaddr *)&addr, sizeof(addr));
	if (rc < 0)
	{
		std::cerr << "bind() failed" <<std::endl;
		close(listen_sd);
		exit(EXIT_FAILURE);
	}

	rc = listen(listen_sd, 32);
	if (rc < 0)
	{
		std::cerr << "listen() failed" <<std::endl;
		close(listen_sd);
		exit(EXIT_FAILURE);
	}

	bzero(fds, sizeof(fds));

	fds[0].fd = listen_sd;
	fds[0].events = POLLIN;

	do
	{
		std::cout << "Waiting on poll()..." << std::endl;

		rc = poll(fds, nfds, -1);
		if (rc < 0)
		{
			std::cerr << " poll() failed" <<std::endl;
			break;
		}

		current_size = nfds;
		for (i = 0; i < current_size; i++)
		{
			std::cout << "debug " << i << std::endl;
			if (fds[i].revents == 0)
				continue;

			if (fds[i].revents != POLLIN)
			{
				std::cout << " Error revents = " << fds[i].revents << std::endl;
				end_server = true;
				break;
			}

			if (fds[i].fd == listen_sd)
			{
				std::cout << " Listening socket is readable" << std::endl;
				do
				{
					new_sd = accept(listen_sd, NULL, NULL);
					if (new_sd < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							std::cerr << " accept() failed" << std::endl;
							end_server = true;
						}
						break;
					}

					std::cout << " New incoming connection - " << new_sd << std::endl;
					fds[nfds].fd = new_sd;
					fds[nfds].events = POLLIN;
					nfds++;
				}
				while (new_sd != -1);
			}
			else
			{
				std::cout << " Descriptor " << fds[i].fd << " is readable " << std::endl;
				close_conn = false;

				//do
				//{
				rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
				if (rc < 0)
				{
					if (errno != EWOULDBLOCK)
					{
						std::cerr << " recv() failed" << std::endl;
						close_conn = true;
					}
					break;
				}

				if (rc == 0)
				{
					std::cout << " Connection closed" << std::endl;
					close_conn = true;
					break;
				}

				len = rc;
				std::cout << len << " byte received" << std::endl;

				rc = send (fds[i].fd, page.c_str(), len_response, 0);
				std::cout << "send" << std::endl;
				if (rc < 0)
				{
					std::cerr << " send() failed" << std::endl;
					close_conn = true;
					break;
				}

				close_conn = true;

				//}
				//while (true);

				if (close_conn == true)
				{
					close(fds[i].fd);
					fds[i].fd = -1;
					compress_array = true;
				}
			}
		}

		if (compress_array == true)
		{
			compress_array = false;
			for (i = 0; i < nfds; i++)
			{
				if (fds[i].fd == -1)
				{
					for(j = i; j < nfds-1; j++)
					{
						fds[j].fd = fds[j+1].fd;
					}
					i--;
					nfds--;
				}
      		}
		}
	}
	while (end_server == false);


	for (i = 0; i < nfds; i++)
	{
	if(fds[i].fd >= 0)
		close(fds[i].fd);
	}

	return 0;
}