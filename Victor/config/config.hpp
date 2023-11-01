#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

class ConfigBlock
{
    private:
        std::unordered_map<std::string, std::string> directives;
        std::unordered_map<std::string, std::vector<ConfigBlock>> nestedBlocks;
};

class Config
{
    private:
        const std::string &_filename;
        ConfigBlock *_configBlock;
    public:
        Config(const std::string &fileName);
        void configParser();
};
