#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <map>
#include <fstream>
#include <sys/select.h>


#define MAX_CLIENT 1024

#include "Client.hpp"

std::string findFileType(std::string fileName);
std::string findMimeType (std::string extension);
std::string reducePath(std::string& path);

void display_clients(std::ofstream & out, std::map<int, Client> & map_clients);
void display_fd_set(std::ofstream & out, const fd_set & readfds, const fd_set & writefds);


#endif