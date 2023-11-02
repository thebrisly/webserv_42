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

    std::stack<ConfigBlock *> blockStack;
    ConfigBlock *currentBlock;
    // ConfigBlock *previousBlock = nullptr;
    blockStack.push(currentBlock);
    while (std::getline(file, line))
    {
        std::string key;
        std::string value;
        if (line.back() == '{')
        {
            std::cout << "Start of new block." << std::endl;
            currentBlock = new ConfigBlock();
            currentBlock->name = line.substr(0, line.size() - 1);
            blockStack.push(currentBlock);
            continue;
        }

        if (line.back() == '}')
        {
            if (blockStack.size() != 0)
                _configBlock.push_back(blockStack.top());
            else
            {
                std::cout << "Error: No block to end." << std::endl;
                exit(1);
            }
            blockStack.pop();
            std::cout << "End of block." << std::endl;
            continue;
        }
        std::istringstream is_line(line);
        std::getline(is_line >> std::ws, key, ' ');
        std::getline(is_line >> std::ws, value);

        if (key.size() == 0 || value.size() == 0)
            continue;
        if (key.front() == '#') // ligne is a comment
            continue;
        currentBlock->directives.insert(std::make_pair(key, value));
        std::cout << "Directive: " << key << " = " << value << std::endl;
            
    }
}

void Config::display()
{
    if (_configBlock.size() == 0)
    {
        std::cout << "No blocks found in config file." << std::endl;
        return ;
    }
    std::cout << "=========== " << "Displaying config file: " << _filename << " ===========" << std::endl;
    for (int i = _configBlock.size(); i > 0; i--)
    {
        _configBlock.back()->display();
        _configBlock.pop_back();
    }
}
