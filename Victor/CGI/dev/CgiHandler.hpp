#include <string>
#include <map>
#include <vector>
#include <unistd.h> // for pid_t

class CgiHandler
{
public:
        CgiHandler();
        CgiHandler(const std::string& path);
        CgiHandler(const CgiHandler& other);
        ~CgiHandler();


        CgiHandler& operator=(const CgiHandler& rhs);


        // Set functions
        void setCgiPid(pid_t cgi_pid);
        void setCgiPath(const std::string& cgi_path);

        // Get functions
        pid_t getCgiPid() const;
        std::string getCgiPath() const;
        const std::map<std::string, std::string>& getEnv() const;

        // Initialize CGI Environment
        //void initEnvCgi(HttpRequest& req, const std::vector<Location>::iterator it_loc);

    private:
        pid_t _cgi_pid;
        int _exit_status;
        std::string _cgi_path;
        char** _ch_env;
        char** _argv;
        std::map<std::string, std::string> _env;
};