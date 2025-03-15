#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>

typedef struct Ecran Ecran;


//1
struct Ecran
{
	float latime;
	float inaltime;
	char* tipPanou;
	char* rezolutieMaxima;
	int refreshRate;
	bool areHDR;
	char* brand;
	float pret;
};

Ecran citireEcran(FILE* f)
{
	Ecran ecran;
	fscanf(f, "%f %f", &ecran.latime, &ecran.inaltime);
	char buffer[20];
	fgets(buffer, 2, f); //citeste \n ramas dupa fscanf
	fgets(buffer, 20, f);
	strtok(buffer, "\n");
	ecran.tipPanou = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy_s(ecran.tipPanou, strlen(buffer) + 1, buffer);
	fgets(buffer, 20, f);
	strtok(buffer, "\n");
	ecran.rezolutieMaxima = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy_s(ecran.rezolutieMaxima, strlen(buffer) + 1, buffer);

	fscanf(f, "%d", &ecran.refreshRate);
	fscanf(f, "%d", &ecran.areHDR);
	fgets(buffer, 2, f); //citeste \n ramas dupa fscanf
	fgets(buffer, 20, f);
	strtok(buffer, "\n");
	ecran.brand = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy_s(ecran.brand, strlen(buffer) + 1, buffer);
	fscanf(f, "%f", &ecran.pret);
	return ecran;

}
void citireEcrane(FILE* f, Ecran** vectorEcran, int* nrEcrane)
{
	if (f == NULL)
	{
		return;
	}
	while (!feof(f))
	{
		*vectorEcran = (Ecran*)realloc(*vectorEcran, (*nrEcrane + 1) * sizeof(Ecran));
		(*vectorEcran)[*nrEcrane] = citireEcran(f);
		(*nrEcrane)++;
	}
}

void afisareEcran(Ecran ecran)
{
	printf("latime si inaltime: %5.2f %5.2f", ecran.latime, ecran.inaltime);
	printf("\ntip panou: %s", ecran.tipPanou);
	printf("\nrezolutie maxima: %s", ecran.rezolutieMaxima);
	printf("\nrefresh rate: %d", ecran.refreshRate);
	printf("\nHDR: %d", ecran.areHDR);
	printf("\nbrand: %s", ecran.brand);
	printf("\npret: %5.2f", ecran.pret);
	printf("\n \n");
}

//lista simplu inlantuita

typedef struct Nod Nod;
struct Nod
{
	Ecran info;
	Nod* next;
};

void inserareLaInceput(Nod** cap, Ecran ecran)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = ecran;
	nou->next = *cap;
	*cap = nou;
}

void stergeUltimulElement(Nod** cap)
{
	if (*cap == NULL)
		return;
	Nod* temp = *cap;
	if (temp->next == NULL)
	{
		free(temp->info.tipPanou);
		free(temp->info.rezolutieMaxima);
		free(temp->info.brand);
		free(temp);
		*cap = NULL;
		return;
	}
	while (temp->next->next != NULL)
	{
		temp = temp->next;
	}
	free(temp->next->info.tipPanou);
	free(temp->next->info.rezolutieMaxima);
	free(temp->next->info.brand);
	free(temp->next);

	temp->next = NULL;
}

void afisareLista(Nod* cap)
{
	while (cap != NULL)
	{
		afisareEcran(cap->info);
		cap = cap->next;
	}
}

int existaElement(Nod* cap, Ecran ecran)
{
	int pozitie = 0;
	while (cap != NULL)
	{
		if (cap->info.latime == ecran.latime && cap->info.inaltime == ecran.inaltime && strcmp(cap->info.tipPanou, ecran.tipPanou) == 0 && strcmp(cap->info.rezolutieMaxima, ecran.rezolutieMaxima) == 0
			&& cap->info.refreshRate == ecran.refreshRate && cap->info.areHDR == ecran.areHDR && strcmp(cap->info.brand, ecran.brand) == 0 && cap->info.pret == ecran.pret)
		{
			afisareEcran(cap->info);
			return pozitie;
		}
		pozitie++;
		cap = cap->next;
	}
	return -1;
}

void conversieListaVector(Nod** cap, Ecran** vector, int* nrEcrane)
{
	while (*cap != NULL)
	{
		*vector = (Ecran*)realloc(*vector, (*nrEcrane + 1) * sizeof(Ecran));
		(*vector)[*nrEcrane] = (*cap)->info;


		//8
		(*vector)[*nrEcrane].tipPanou = (char*)malloc(strlen((*cap)->info.tipPanou) + 1);
		strcpy((*vector)[*nrEcrane].tipPanou, (*cap)->info.tipPanou);

		(*vector)[*nrEcrane].rezolutieMaxima = (char*)malloc(strlen((*cap)->info.rezolutieMaxima) + 1);
		strcpy((*vector)[*nrEcrane].rezolutieMaxima, (*cap)->info.rezolutieMaxima);

		(*vector)[*nrEcrane].brand = (char*)malloc(strlen((*cap)->info.brand) + 1);
		strcpy((*vector)[*nrEcrane].brand, (*cap)->info.brand);


		(*nrEcrane)++;
		Nod* copie = *cap;
		(*cap) = (*cap)->next;

		free(copie->info.tipPanou);
		free(copie->info.rezolutieMaxima);
		free(copie->info.brand);
		free(copie);
	}
}




int main()
{
	//2
	FILE* f = fopen("ecrane.txt", "r");
	Ecran* vectorEcrane = NULL;
	int nrEcrane = 0;
	citireEcrane(f, &vectorEcrane, &nrEcrane);
	fclose(f);
	for (int i = 0; i < nrEcrane; i++)
		afisareEcran(vectorEcrane[i]);


	//3
	printf("\n-----------------------------------\n");
	Nod* cap = NULL;
	for (int i = 0; i < nrEcrane; i++)
	{
		inserareLaInceput(&cap, vectorEcrane[i]);
	}
	afisareLista(cap);

	//4
	stergeUltimulElement(&cap);
	printf("\n-----------------------------\n");
	printf("Dupa stergerea ultimului element: \n");
	afisareLista(cap);

	printf("\n-----------------------\n");

	//5
	printf("Verificare existenta element vectorEcrane[1] in lista \n");
	int pozitie = existaElement(cap, vectorEcrane[1]);
	if (pozitie != -1)
	{
		printf("Elemmentul a fost gasit pe pozitia %d\n", pozitie);
	}
	else printf("Elementul nu a fost gasit in lista!\n");

	//6
	printf("\n----------------------------------------------------- Afisare lista: \n");
	afisareLista(cap);

	//7
	Ecran* vectorNou = NULL;
	int nrEcraneNou = 0;
	printf("\n------------------------------------------ Conversie: ");
	conversieListaVector(&cap, &vectorNou, &nrEcraneNou);
	printf("\nafisare elemente lista: ");
	afisareLista(cap);
	printf("\nAfisare elemente vector: \n");
	for (int i = 0; i < nrEcraneNou; i++)
		afisareEcran(vectorNou[i]);


	//8
	free(vectorNou);
	free(vectorEcrane);

	//Autoevalure: 9

	return 0;
}