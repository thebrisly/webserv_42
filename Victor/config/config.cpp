#include "config.hpp"

Config::Config(const std::string &fileName) : _filename(fileName), _configBlock(new ConfigBlock())
{
    std::cout << "Config file: " << _filename << " created" << std::endl;
}

void Config::configParser()
{
    std::cout << "Parsing config file: " << _filename << std::endl;
    std::ifstream file(_filename);
    std::string line;

    ConfigBlock *currentBlock = _configBlock;
    ConfigBlock *previousBlock = nullptr;
    while (std::getline(file, line))
    {
        std::istringstream is_line(line);
        std::string key;

        if (line.back() == '}')
        {
            currentBlock = &(currentBlock->nestedBlocks->back());
            std::cout << "End of block: " << std::endl;
        }
        else if (line.back() == '{')
        {
            std::cout << "New block: " << std::endl;
            previousBlock = currentBlock;
            std::cout << "Previous block: " << previousBlock << std::endl;
            new currentBlock->nestedBlocks->back();
            currentBlock = &(currentBlock->nestedBlocks->back());
        }

        // Skip leading whitespaces for the key
        std::getline(is_line >> std::ws, key, ' ');

        std::string value;
        if (std::getline(is_line >> std::ws, value)) // Skip leading whitespaces for the value
        {
            currentBlock->directives.insert(std::make_pair(key, value));
            std::cout << "Directive: " << key << " = " << value << std::endl;
        }
    }
}

void Config::display()
{
    std::cout << "Displaying config file: " << _filename << std::endl;

    std::cout << _configBlock->directives.size() << " directives" << std::endl;
    // for (std::unordered_map<std::string, std::vector<ConfigBlock> >::iterator it = _configBlock->nestedBlocks.begin(); it!= _configBlock->nestedBlocks.end(); ++it)
    // {
    //     std::cout << it->first << std::endl;
    //     for (std::unordered_map<std::string, std::string>::iterator it2 = _configBlock->directives.begin(); it2!= _configBlock->directives.end(); ++it2)
    //     {
    //         std::cout << it->first << " = " << it2->second << std::endl;
    //     }
    // }
}