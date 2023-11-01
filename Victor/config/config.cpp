#include "config.hpp"

Config::Config(const std::string &fileName) : _filename(fileName), _configBlock(new ConfigBlock())
{
    configParser();
}

void Config::configParser()
{
    
}