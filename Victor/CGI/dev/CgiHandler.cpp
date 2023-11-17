
#include "CgiHandler.hpp"
#include <iostream>

CgiHandler::CgiHandler() : _cgi_pid(-1), _exit_status(0), _cgi_path(""), _ch_env(NULL), _argv(NULL) {}

CgiHandler::CgiHandler(const std::string& path) : _cgi_pid(-1), _exit_status(0), _cgi_path(path), _ch_env(NULL), _argv(NULL) {}

CgiHandler::CgiHandler(const CgiHandler& other) : _env(other._env), _ch_env(other._ch_env), _argv(other._argv), _cgi_path(other._cgi_path), _cgi_pid(other._cgi_pid), _exit_status(other._exit_status) {}

CgiHandler& CgiHandler::operator=(const CgiHandler& rhs) {
    if (this != &rhs) {
        _env = rhs._env;
        _ch_env = rhs._ch_env;
        _argv = rhs._argv;
        _cgi_path = rhs._cgi_path;
        _cgi_pid = rhs._cgi_pid;
        _exit_status = rhs._exit_status;
    }
    return *this;
}

CgiHandler::~CgiHandler() {
    
}

void CgiHandler::setCgiPid(pid_t cgi_pid) {
    _cgi_pid = cgi_pid;
}

void CgiHandler::setCgiPath(const std::string& cgi_path) {
    _cgi_path = cgi_path;
}

pid_t CgiHandler::getCgiPid() const {
    return _cgi_pid;
}

std::string CgiHandler::getCgiPath() const {
    return _cgi_path;
}

const std::map<std::string, std::string>& CgiHandler::getEnv() const {
    return _env;
}

// Initialize CGI Environment
// void CgiHandler::initEnvCgi(HttpRequest& req, const std::vector<Location>::iterator it_loc) {
//     // Environment initialization logic
// }

