#include "config.hpp"

Config::Config(const std::string &fileName) : _filename(fileName)
{
    std::cout << "Config file: " << _filename << " created" << std::endl;
}

Config::~Config()
{
    std::cout << "Config file: " << _filename << " destroyed" << std::endl;
}

void Config::configParser()
{
    std::cout << "Parsing config file: " << _filename << std::endl;
    std::ifstream file(_filename);
    std::string line;

    ConfigBlock *currentBlock;
    // ConfigBlock *previousBlock = nullptr;

    while (std::getline(file, line))
    {
        std::istringstream is_line(line);
        std::string key;
        std::string value;

        if (line.back() == '{')
        {
            std::cout << "Start of new block." << std::endl;
            currentBlock = new ConfigBlock();
            while (std::getline(is_line >> std::ws, key))
            {
                std::getline(is_line >> std::ws, key, ' ');
                std::getline(is_line >> std::ws, value);

                currentBlock->directives.insert(std::make_pair(key, value));
                std::cout << "Directive: " << key << " = " << value << std::endl;

                if (line.back() == '}')
                {
                    std::cout << "End of block." << std::endl;
                    break;
                }
            }
        }

    }
}

void Config::display()
{
    // std::cout << "Displaying config file: " << _filename << std::endl;
    // std::cout << _configBlock->directives.size() << " directives" << std::endl;
    // for (std::unordered_map<std::string, std::vector<ConfigBlock> >::iterator it = _configBlock->nestedBlocks.begin(); it!= _configBlock->nestedBlocks.end(); ++it)
    // {
    //     std::cout << it->first << std::endl;
    //     for (std::unordered_map<std::string, std::string>::iterator it2 = _configBlock->directives.begin(); it2!= _configBlock->directives.end(); ++it2)
    //     {
    //         std::cout << it->first << " = " << it2->second << std::endl;
    //     }
    // }
}
