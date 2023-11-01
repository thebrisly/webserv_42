#include "config.hpp"

int main()
{
    Config config("config.config");
    config.configParser();
    config.display();
    return 0;
}
