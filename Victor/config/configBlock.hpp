#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

class ConfigBlock
{
    public:
        std::string name;
        std::unordered_map<std::string, std::string> directives;
        std::vector<ConfigBlock> *nestedBlocks;
        ConfigBlock &operator=(const ConfigBlock& other);

        void display();

        ConfigBlock();
        ~ConfigBlock();
};

