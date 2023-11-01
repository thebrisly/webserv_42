#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

class ConfigBlock
{
    public:
        std::unordered_map<std::string, std::string> directives;
        std::vector<ConfigBlock> *nestedBlocks;
};

class Config
{
    public:
        const std::string &_filename;
        ConfigBlock *_configBlock;
        Config(const std::string &fileName);
        void configParser();
        void display();
};
