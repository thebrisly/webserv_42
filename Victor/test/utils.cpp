#include "utils.hpp"

int countTabs(const std::string &str)
{
	int tabCount = 0;
	for (int i = 0; i < (int) str.size(); i++)
	{
		if (str[i] == '\t')
			tabCount++;
		else
			break;

	}
	return tabCount;
}