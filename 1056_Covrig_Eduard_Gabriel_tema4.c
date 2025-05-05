#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

typedef struct Telefon Telefon;
struct Telefon
{
	int id;
	char* brand;
	char* denumire;
	int memorie;
	int memorieRam;
	char* sistemOperare;
	float pret;
};

typedef struct Nod Nod;
struct Nod
{
	Telefon info;
	Nod* stanga;
	Nod* dreapta;
};

Telefon citireTelefon(FILE* f)
{	
	Telefon t;
	char buffer[31];

	fgets(buffer, sizeof(buffer), f);
	strtok(buffer, "\n");
	t.id = atoi(buffer);

	fgets(buffer, sizeof(buffer), f);
	strtok(buffer, "\n");
	t.brand = (char*)malloc(strlen(buffer) + 1);
	strcpy(t.brand, buffer);

	fgets(buffer, sizeof(buffer), f);
	strtok(buffer, "\n");
	t.denumire = (char*)malloc(strlen(buffer) + 1);
	strcpy(t.denumire, buffer);

	fgets(buffer, sizeof(buffer), f);
	strtok(buffer, "\n");
	t.memorie = atoi(buffer);

	fgets(buffer, sizeof(buffer), f);
	strtok(buffer, "\n");
	t.memorieRam = atoi(buffer);

	fgets(buffer, sizeof(buffer), f);
	strtok(buffer, "\n");
	t.sistemOperare = (char*)malloc(strlen(buffer) + 1);
	strcpy(t.sistemOperare, buffer);

	fgets(buffer, sizeof(buffer), f);
	strtok(buffer, "\n");
	t.pret = atof(buffer);

	return t;
}

void inserareInBST(Nod** radacina, Telefon t)
{
	if (*radacina == NULL)
	{
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = t;
		nou->stanga = nou->dreapta = NULL;
		*radacina = nou;
	}
	else if (t.id < (*radacina)->info.id) {
		inserareInBST(&(*radacina)->stanga, t);
	}
	else inserareInBST(&(*radacina)->dreapta, t);
}


//2
void citireTelefoaneBST(Nod** radacina, FILE* f)
{
	if (f == NULL) return;
	while (!feof(f))
	{
		inserareInBST(&(*radacina), citireTelefon(f));
	}
}

void afisareTelefon(Telefon t)
{
	printf("ID: %d | Brand: %s | Denumire: %s | Memorie: %d GB | RAM: %d GB | SO: %s | Pret: %.2f RON\n",
		t.id, t.brand, t.denumire, t.memorie, t.memorieRam, t.sistemOperare, t.pret);
}

void parcurgereInOrdine(Nod* radacina)
{
	if (radacina)
	{
		parcurgereInOrdine(radacina->stanga);
		afisareTelefon(radacina->info);
		parcurgereInOrdine(radacina->dreapta);
	}
}

//3

int frunzeCuIdCorespunzator(Nod* radacina, int id)
{
	if (radacina == NULL) return 0;
	else if (radacina->stanga == NULL && radacina->dreapta == NULL)
	{
		if (radacina->info.id > id)
		{
			return 1;
		}
		else return 0;
	}
	else return frunzeCuIdCorespunzator(radacina->stanga, id) + frunzeCuIdCorespunzator(radacina->dreapta, id);
}

//4

void adaugaInVector(Nod* radacina, Telefon* vector, int* index)
{
	if (radacina != NULL)
	{
		adaugaInVector(radacina->stanga, vector, &(*index));
		vector[*index] = radacina->info;
		(*index)++;
		adaugaInVector(radacina->dreapta, vector, &(*index));
	}
}

typedef struct MaxHeap MaxHeap;
struct MaxHeap
{
	Telefon* vector;
	int dimensiune;
};

//5

void filtrareMaxHeap(MaxHeap mHeap, int index) {
	if (mHeap.dimensiune > 0) {
		int pozMax = index;
		int pozSt = 2 * index + 1;
		int pozDr = 2 * index + 2;

		if (pozSt < mHeap.dimensiune && mHeap.vector[pozMax].id < mHeap.vector[pozSt].id) {
			pozMax = pozSt;
		}

		if (pozDr < mHeap.dimensiune && mHeap.vector[pozMax].id < mHeap.vector[pozDr].id) {
			pozMax = pozDr;
		}

		if (pozMax != index) {
			Telefon aux = mHeap.vector[index];
			mHeap.vector[index] = mHeap.vector[pozMax];
			mHeap.vector[pozMax] = aux;

			if (pozMax <= mHeap.dimensiune / 2 - 1) {
				filtrareMaxHeap(mHeap, pozMax);
			}
		}
	}
}

//6

void inserareVectorBrandMaxheap(MaxHeap mHeap, Telefon** vector, int *index, const char* brand)
{
	for(int i=0;i<mHeap.dimensiune;i++)
		if (strcmp(mHeap.vector[i].brand, brand) == 0)
		{
			*vector = (Telefon*)realloc(*vector, (*index + 1) * sizeof(Telefon));
			(*vector)[*index] = mHeap.vector[i];
			(*index)++;
		}
}

//7 traversari

void traversareMaxHeap(MaxHeap mh)
{
	for (int i = 0; i < mh.dimensiune; i++)
	{
		afisareTelefon(mh.vector[i]);
	}
}

//8 stergere din coada de prioritate brand

void steregereDinMaxHeap(MaxHeap* mHeap,const char* brand)
{
	for (int i = 0; i < mHeap->dimensiune; i++)
	{
		if (strcmp(mHeap->vector[i].brand, brand) == 0)
		{
			mHeap->vector[i] = mHeap->vector[mHeap->dimensiune - 1];
			mHeap->dimensiune--;
			filtrareMaxHeap(*mHeap, i);
			i--;
		}
	}

}

//9

void dezalocareArbore(Nod** radacina)
{
	if (*radacina)
	{
		dezalocareArbore(&(*radacina)->stanga);
		dezalocareArbore(&(*radacina)->dreapta);
		free((*radacina)->info.brand);
		free((*radacina)->info.denumire);
		free((*radacina)->info.sistemOperare);
		free(*radacina);
	}
}

void dezalocareMaxHeap(MaxHeap* mHeap)
{
	for (int i = 0; i < mHeap->dimensiune; i++)
	{
		//free(mHeap->vector[i].brand);
		//free(mHeap->vector[i].denumire);
		//free(mHeap->vector[i].sistemOperare); (s-au dezalocat deja cand am apelat dezalocareArbore)
	}
	free(mHeap->vector);
}

int main()
{
	FILE* f = fopen("telefoane.txt", "r");
	Nod* radacina = NULL;
	citireTelefoaneBST(&radacina, f);
	parcurgereInOrdine(radacina);

	//3
	int k = frunzeCuIdCorespunzator(radacina, 1);
	printf("%d frunze au id-ul mai mare decat 1\n", k);

	printf("\n-----------------MaxHeap-------------\n");
	//4
	int nrElemente = 4;
	Telefon* vector = (Telefon*)malloc(nrElemente * sizeof(Telefon));
	int index = 0;
	adaugaInVector(radacina, vector, &index);
	MaxHeap mh;
	mh.dimensiune = 4;
	mh.vector = (Telefon*)malloc(sizeof(Telefon) * mh.dimensiune);
	for (int i = 0; i < mh.dimensiune; i++)
		mh.vector[i] = vector[i];
	free(vector);

	//5
	for (int i = mh.dimensiune / 2 - 1; i >= 0; i--)
		filtrareMaxHeap(mh, i);
	for (int i = 0; i < mh.dimensiune; i++)
		afisareTelefon(mh.vector[i]);

	//6
	vector = NULL;
	index = 0;
	inserareVectorBrandMaxheap(mh, &vector, &index, "Apple");
	printf("\n\n");
	for (int i = 0; i < index; i++)
	{
		afisareTelefon(vector[i]);
	}
	//7
	printf("\nTraversare maxheap:\n");
	traversareMaxHeap(mh);
	printf("\nTraversare arbore binar de cautare:\n");
	parcurgereInOrdine(radacina);

	//8
	steregereDinMaxHeap(&mh, "Apple");
	printf("\nDupa stergere Apple:\n");
	traversareMaxHeap(mh);

	//9
	dezalocareArbore(&radacina);
	dezalocareMaxHeap(&mh);


	return 0;
}