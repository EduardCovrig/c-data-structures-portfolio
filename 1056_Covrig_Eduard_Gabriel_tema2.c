#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>

typedef struct Gara Gara;
typedef struct Nod Nod;



//1
struct Gara
{
	char* nume;
	char* oras;
	int nrTrenuriZi;
	int nrAngajati;
	bool deschisaNonStop;
	int anConstructie;
	char* manager;
};

struct Nod
{
	struct Gara info;
	Nod* next;
	Nod* prev;
};

//AFISARI

void afisareGara(Gara gara)
{
	printf("Nume Gara: %s\n", gara.nume);
	printf("Oras: %s\n", gara.oras);
	printf("Numar trenuri pe zi: %d\n", gara.nrTrenuriZi);
	printf("Numar angajati: %d\n", gara.nrAngajati);
	printf("Deschisa non-stop: %s\n", gara.deschisaNonStop ? "Da" : "Nu");
	printf("An constructie: %d\n", gara.anConstructie);
	printf("Manager: %s\n", gara.manager);
	printf("\n");
}

void afisareLista(Nod* cap)
{
	if (cap == NULL)
		return;
	while (cap != NULL)
	{
		afisareGara(cap->info);
		cap = cap->next;
	}
}

void afisareListaCirculara(Nod* cap)
{
	if (cap == NULL)
	{
		return;
	}
	Nod* copie = cap;
	afisareGara(cap->info);
	cap = cap->next;
	while (cap != copie)
	{
		afisareGara(cap->info);
		cap = cap->next;
	}
}

//2

Gara citireGara(FILE* f)
{
	Gara gara;
	char buffer[31];
	fgets(buffer, sizeof(buffer), f);
	strtok(buffer, "\n");
	gara.nume = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy_s(gara.nume, strlen(buffer) + 1, buffer);

	fgets(buffer, sizeof(buffer), f);
	strtok(buffer, "\n");
	gara.oras= (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy_s(gara.oras, strlen(buffer) + 1, buffer);

	fgets(buffer, sizeof(buffer), f);
	strtok(buffer, "\n");
	gara.nrTrenuriZi = atoi(buffer);

	fgets(buffer, sizeof(buffer), f);
	strtok(buffer, "\n");
	gara.nrAngajati = atoi(buffer);

	fgets(buffer, sizeof(buffer), f);
	strtok(buffer, "\n");
	gara.deschisaNonStop = atoi(buffer);

	fgets(buffer, sizeof(buffer), f);
	strtok(buffer, "\n");
	gara.anConstructie = atoi(buffer);

	fgets(buffer, sizeof(buffer), f);
	strtok(buffer, "\n");
	gara.manager = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy_s(gara.manager, strlen(buffer) + 1, buffer);

	return gara;
}


void InserareLaInceputLSI(Nod** cap, Gara gara)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod) * 1);
	nou->info = gara;
	nou->next = *cap;

	*cap = nou;
}
void citesteLista(FILE* f, Nod** cap)
{
	if (f == NULL)
		return;
	while (!feof(f))
	{
		InserareLaInceputLSI(&(*cap),citireGara(f));
	}
}

//3

Nod** convertireLSILDI(Nod* cap, Nod** capD, Nod** coadaD) //inserare sfarsit
{
	*capD = NULL;
	*coadaD = NULL;
	
	Nod* temp = cap;
	while (temp != NULL)
	{
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info.nume = (char*)malloc(strlen(temp->info.nume) + 1);
		strcpy(nou->info.nume, temp->info.nume);

		nou->info.oras = (char*)malloc(strlen(temp->info.oras) + 1);
		strcpy(nou->info.oras, temp->info.oras);

		nou->info.manager = (char*)malloc(strlen(temp->info.manager) + 1);
		strcpy(nou->info.manager, temp->info.manager);

		nou->info.nrTrenuriZi = temp->info.nrTrenuriZi;
		nou->info.nrAngajati = temp->info.nrAngajati;
		nou->info.deschisaNonStop = temp->info.deschisaNonStop;
		nou->info.anConstructie = temp->info.anConstructie;


		nou->next = NULL;
		nou->prev = *coadaD;

		if (*capD == NULL)
		{
			*capD = nou;
		}
		else
		{
			(*coadaD)->next = nou;
		}

		*coadaD = nou;

		temp = temp->next;
	}
	return *capD;
}


//4

void inserareSfarsitLDI(Nod** cap, Nod** coada, Gara gara)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = gara;
	nou->next = NULL;
	nou->prev = *coada;

	if ((*cap) == NULL)
	{
		*cap = *coada = nou;
	}
	else
	{
		(*coada)->next = nou;
		*coada = nou;
	}
}

//5

void VerificareOras(Nod* cap, Nod* coada)
{
	char temp[31];
	printf("Orasul cautat: (Bucuresti sau Constanta)\n");
	scanf("%30s", temp);
	if (strcmp(temp, "Constanta")==0 || strcmp(temp, "Bucuresti")==0)
	{
		while (cap != NULL)
		{
			if (strcmp(cap->info.oras, temp) == 0)
				afisareGara(cap->info);
			cap = cap->next;
		}
	}
	else printf("Oras invalid!");
}

//8

void StergereGara(Nod** cap, Nod** coada)
{
	struct Nod* curent = *cap;
	do {
		if (strcmp(curent->info.nume, "Gara Basarab") == 0) {
			
			if (curent == *cap) {
				*cap = curent->next;
			}
			if (curent == *coada) {
				*coada = curent->prev;
			}


		
			if (curent->prev != NULL) {
				curent->prev->next = curent->next;
			}
			if (curent->next != NULL) {
				curent->next->prev = curent->prev;
			}

			
			if (curent->next == curent) {
				*cap = *coada = NULL;
			}

		
			free(curent->info.nume);
			free(curent->info.oras);
			free(curent->info.manager);

			
			free(curent);
			printf("Gara Basarab a fost stearsa din lista!\n");
			return;
		}
		curent = curent->next;

	} while (curent != *cap);  

	printf("Nu s-a gasit gara cu numele 'Gara Basarab'.\n");

}


//9
void dezalocareDubla(Nod** cap, Nod** coada)
{

	Nod* capInitial = *cap;
	do 
	{
		free((*cap)->info.oras);
		free((*cap)->info.nume);
		free((*cap)->info.manager);

		Nod* copie = *cap;
		*cap = (*cap)->next;
		if (*cap!= capInitial)
		{
			(*cap)->prev = NULL;
		}
		free(copie);
	} while (*cap != capInitial);

	*cap = NULL;
	*coada = NULL;
}

void dezalocareSimpla(Nod** cap)
{
	while (*cap != NULL)
	{
		free((*cap)->info.oras);
		free((*cap)->info.nume);
		free((*cap)->info.manager);

		Nod* copie = *cap;
		*cap = (*cap)->next;
		free(copie);
	}
	*cap = NULL;
}

int main()
{

	//2
	FILE* fisier = fopen("gara.txt", "r");
	if (fisier == NULL)
		return 1000;
	Nod* cap = NULL;

	citesteLista(fisier, &cap);
	afisareLista(cap);

	//3
	fclose(fisier);

	Nod* capDublu = NULL;
	Nod* coadaDubla = NULL;
	
	capDublu=convertireLSILDI(cap,&capDublu,&coadaDubla);
	printf("\n-------------------------------\n");
	afisareLista(capDublu);


	//4
	Gara g1;
	g1.nume = (char*)malloc(sizeof(char) * strlen("Gara Constanta") + 1);
	strcpy(g1.nume,"Gara Constanta");
	g1.oras = (char*)malloc(sizeof(char) * strlen("Constanta") + 1);
	strcpy(g1.oras,"Constanta");
	g1.nrTrenuriZi = 314;
	g1.nrAngajati = 55;
	g1.deschisaNonStop = 1;
	g1.anConstructie = 1921;
	g1.manager = (char*)malloc(sizeof(char) * (strlen("Cristian Popescu") + 1));
	strcpy(g1.manager,"Cristian Popescu");
	inserareSfarsitLDI(&capDublu, &coadaDubla, g1);
	printf("\n-------------------------------\n");
	afisareLista(capDublu);

	//5
	printf("\n-------------------------------\n");
	VerificareOras(capDublu, coadaDubla);
	printf("\n-------------------------------\n");
	//6+7
	//Afisare lista dublu inlantuita
	printf("Afisare lista dublu inlantuita inainte de transformare: \n");
	afisareLista(capDublu);
	printf("\n-------------------------------\n");
	//Transformare in circulara
	capDublu->prev = coadaDubla;
	coadaDubla->next = capDublu;
	printf("Afisare lista circulara: \n");
	afisareListaCirculara(capDublu);
	printf("\n-------------------------------\n");
	//8
	StergereGara(&capDublu, &coadaDubla);
	printf("\n-------------------------------\n");
	afisareListaCirculara(capDublu);

	//9
	dezalocareDubla(&capDublu, &coadaDubla);
	printf("Verificare afisare lista dubla circulara dupa dezalocare: \n");
	afisareListaCirculara(capDublu);
	dezalocareSimpla(&cap);
	printf("Verificare afisare lista simplu inlantuita duap dezalocare: \n");
	afisareLista(cap);
	

	//Nota 7
	return 0;
}