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