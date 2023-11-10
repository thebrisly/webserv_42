#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>


class server
{
	private:
		unsigned long port;
		unsigned long ip_address;
		std::string name;
	public:
		server();
		~server();

		void setPort(unsigned long port);
		unsigned long getPort();
};

server::server()
{

}

server::~server()
{

}
