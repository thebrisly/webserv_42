#include "configBlock.hpp"


ConfigBlock::ConfigBlock()
{
    nestedBlocks = new std::vector<ConfigBlock>();
}


ConfigBlock::~ConfigBlock()
{
    delete nestedBlocks;
}

ConfigBlock& ConfigBlock::operator=(const ConfigBlock& other)
{
    if (this != &other)
    {
        this->directives = other.directives;
        this->nestedBlocks = new std::vector<ConfigBlock>();
        for (std::vector<ConfigBlock>::iterator it = other.nestedBlocks->begin(); it!= other.nestedBlocks->end(); it++)
        {
            this->nestedBlocks->push_back(*it);
        }
        nestedBlocks = other.nestedBlocks;
    }
    return *this;
}

void ConfigBlock::display(int tree_level)
{

    std::string indent(tree_level, '\t');
    std::cout << indent << "Blocks: " << name << std::endl;
    std::cout << indent << "  Directives: " << std::endl;
    for (std::unordered_map<std::string, std::string>::iterator it = directives.begin(); it!= directives.end(); it++)
    {
        std::cout << indent <<  "    " << it->first << " = " << it->second << std::endl;
    }
    
    for (std::vector<ConfigBlock>::iterator it = nestedBlocks->begin(); it!= nestedBlocks->end(); it++)
    {
        std::cout << indent << "  Nested Blocks: " << std::endl;
        it->display(10);
    }
}