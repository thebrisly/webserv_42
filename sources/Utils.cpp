#include "../includes/Utils.hpp"

std::string findFileType(std::string fileName)
{
	unsigned long pos = 0;
	while (pos != std::string::npos)
	{
		pos = fileName.find(".");
		fileName.erase(0, pos + 1);
	}
	return fileName;
}

std::string findMimeType (std::string extension)
{
	std::map<std::string, std::string> ext_to_MimeType;

	ext_to_MimeType["css"] = "text/css";
	ext_to_MimeType["jpe"] = "text/jpeg";
	ext_to_MimeType["jpg"] = "text/jpeg";
	ext_to_MimeType["jpeg"] = "text/jpeg";
	ext_to_MimeType["png"] = "text/png";
	ext_to_MimeType["html"] = "text/html";

	if (ext_to_MimeType.find(extension) == ext_to_MimeType.end())
	{
		return "application/octet-stream";
	}

	return ext_to_MimeType[extension];
}


std::string reducePath(std::string& path)
{
    if (path == "/") {
        return path;
    }

    size_t lastSlashPos = path.find_last_of('/');

    if (lastSlashPos == std::string::npos || lastSlashPos == 0) {
        path = "/";
    } else {
        path = path.substr(0, lastSlashPos);
    }

    return path;
}

void display_clients(std::ofstream & out, std::map<int, Client> & map_clients)
{
	for (std::map<int, Client>::iterator it = map_clients.begin(); it!= map_clients.end(); ++it)
	{
		out << "Client : " << it->first << std::endl;
		out << (*it).second << std::endl;
	}
}

void display_fd_set(std::ofstream & out, const fd_set & readfds, const fd_set & writefds)
{
	out << "readfds : ";
	for (int i = 0; i < MAX_CLIENT; i++)
	{
		if (FD_ISSET(i, &readfds))
		{
			out << i << " " ;
		}
	}
	out << std::endl;
	out << "writefds : ";
	for (int i = 0; i < MAX_CLIENT; i++)
	{
		if (FD_ISSET(i, &writefds))
		{
			out << i << " " ;
		}
	}
	out << std::endl;
}

std::string listDirectoriesAsHTML(const std::string& path)
{
    DIR *dir = opendir(path.c_str());
    if (dir == nullptr) {
        std::cerr << "Error opening directory: " << path << std::endl;
        return "<p>Error opening directory.</p>";
    }

    std::stringstream html;
    html << "<html><head><title>Directory Listing</title></head><body>";
    html << "<h1>Directory Listing of " << path << "</h1><ul>";

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_DIR) {
            std::string name = entry->d_name;
            if (name != "." && name != "..") {
                html << "<li>" << name << "</li>";
            }
        }
    }

    html << "</ul></body></html>";
    closedir(dir);

    return html.str();
}