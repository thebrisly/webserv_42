#include <iostream>
#include <map>

class Objet
{
	private :
		int _nb;

	public :
		Objet(int nb);
		Objet() : _nb(0) {};

};

Objet::Objet(int nb) : _nb(nb)
{
	std::cout << "nb : " << nb << std::endl;
}

int main ()
{
	std::map<int, Objet> map_objet;

	Objet objet1(1);

	map_objet[1] = objet1;

	return 0;
}