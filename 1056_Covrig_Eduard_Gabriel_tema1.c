#include <iostream>
#include <fstream>

struct Ecran
{
	int latime;
	int inaltime;
	char* rezolutieMaxima;
	char* tipPanou;
	float pret;
	int refreshRate;
	char* brand;
	bool suportHDR;
};

int main()
{
	FILE* f = fopen("ecrane.txt",r);


	return 0;
}
