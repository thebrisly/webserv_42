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