#include "configBlock.hpp"

class Config
{
    public:
        Config(const std::string &fileName);
        ~Config();

        void configParser();
        void display();

        const std::string &_filename;
        std::vector<ConfigBlock *> _configBlock;
};
