#define _crt_secure_no_warnings
#include <stdio.h>
#include <stdlib.h>

typedef struct Bautura Bautura;
struct Bautura
{
	int id;
	char* nume;
	float pret;
	int ml;
	int nivelZahar; 
	char* dataExpirare;
	char* producator;
};

typedef struct NodPrincipal NodPrincipal;
typedef struct NodSecundar NodSecundar;
struct NodPrincipal
{
	Bautura info;
	NodPrincipal* next;
	NodSecundar* vecini;
};

struct NodSecundar
{
	NodPrincipal* info;
	NodSecundar* next;
};

Bautura citesteBautura(FILE* f)
{
	Bautura b;
	char buffer[51];;

	fgets(buffer, sizeof(buffer), f);
	strtok(buffer, "\n");
	b.id = atoi(buffer);

	fgets(buffer, sizeof(buffer), f);
	strtok(buffer, "\n");
	b.nume = (char*)malloc(strlen(buffer) + 1);
	strcpy(b.nume, buffer);

	fgets(buffer, sizeof(buffer), f);
	strtok(buffer, "\n");
	b.pret = atof(buffer);

	fgets(buffer, sizeof(buffer), f);
	strtok(buffer, "\n");
	b.ml = atoi(buffer);

	fgets(buffer, sizeof(buffer), f);
	strtok(buffer, "\n");
	b.nivelZahar = atoi(buffer);

	fgets(buffer, sizeof(buffer), f);
	strtok(buffer, "\n");
	b.dataExpirare = (char*)malloc(strlen(buffer) + 1);
	strcpy(b.dataExpirare, buffer);

	fgets(buffer, sizeof(buffer), f);
	strtok(buffer, "\n");
	b.producator = (char*)malloc(strlen(buffer) + 1);
	strcpy(b.producator, buffer);

	return b;
}

	void afisareBautura(Bautura b) 
	{
		printf("%d %s %5.2f %d %d %s %s\n",b.id,b.nume,b.pret,b.ml,b.nivelZahar,b.dataExpirare,b.producator);
	}



void inserareListaPrincipala(NodPrincipal** cap, Bautura b)
{
	NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));
	nou->info = b;
	nou->next = NULL;
	nou->vecini = NULL;
	if (*cap == NULL)
		*cap = nou;
	else {
		NodPrincipal* aux = *cap;
		while (aux->next != NULL)
		{
			aux = aux->next;
		}
		aux->next = nou;
	}
}


void citireBauturi(NodPrincipal** graf, FILE* f)
{
	if (f == NULL) return;
	else
	{
		while (!feof(f))
		{
			inserareListaPrincipala(&(*graf), citesteBautura(f));
		}
	}
}

NodPrincipal* cautareNodDupaId(NodPrincipal* graf, int id) {
	while (graf != NULL && id != graf->info.id) 
	{
		graf = graf->next;
	}
	return graf;
}


void inserareListaSecundara(NodSecundar** cap, NodPrincipal* nodPr)
{
	NodSecundar* nou = (NodSecundar*)malloc(sizeof(NodSecundar));
	nou->info = nodPr;
	nou->next = NULL;
	if (*cap == NULL)
	{
		*cap = nou;
	}
	else
	{
		NodSecundar* aux = *cap;
		while (aux->next != NULL)
		{
			aux = aux->next;
		}
		aux->next = nou;
	}
}

void adaugaMuchie(NodPrincipal* graf, int id1, int id2)
{
	NodPrincipal* nod1 = cautareNodDupaId(graf, id1);
	NodPrincipal* nod2 = cautareNodDupaId(graf, id2);
	if (nod1 != NULL && nod2 != NULL)
	{
		inserareListaSecundara(&nod1->vecini, nod2);
		inserareListaSecundara(&nod2->vecini, nod1);
	}
}

void afisareGraf(NodPrincipal* graf)
{
	while (graf != NULL)
	{
		printf("\nNod: ");
		afisareBautura(graf->info);
		NodSecundar* vecini = graf->vecini;
		printf("\nVecinii nodului: \n");
		while (vecini!=NULL)
		{
			afisareBautura(vecini->info->info);
			vecini = vecini->next;
		}
		printf("\n-------------------------------------------\n");
		graf = graf->next;
	}
}

//3.
float calculMedie(NodPrincipal* graf, int id)
{
	float suma = 0;
	int k = 0;
	while (graf != NULL)
	{
		if (graf->info.id < id ) {
			suma += graf->info.pret;
			k++;
		}
		graf = graf->next;
	}
	if (k == 0) return 0;
	else return suma / k;
}

//4 AVL

typedef struct Nod Nod;
struct Nod
{
	Bautura info;
	Nod* stanga;
	Nod* dreapta;

};

int inaltimeArbore(Nod* radacina)
{
	if (radacina)
	{
		int stanga = inaltimeArbore(radacina->stanga);
		int dreapta = inaltimeArbore(radacina->dreapta);
		if (stanga > dreapta)
		{
			return 1 + stanga;
		}
		else return 1 + dreapta;
	}
	else return 0;
}

int diferentaInaltimiSubarbori(Nod* radacina)
{
	if (radacina)
	{
		return inaltimeArbore(radacina->stanga) - inaltimeArbore(radacina->dreapta);
	}
	else return 0;
}

void rotireStanga(Nod** radacina)
{
	Nod* aux = (*radacina)->dreapta;
	(*radacina)->dreapta = aux->stanga;
	aux->stanga = *radacina;
	*radacina = aux;
}

void rotireDreapta(Nod** radacina)
{
	Nod* aux = (*radacina)->stanga; //Nodul din stanga va deveni noua radacina.
	(*radacina)->stanga = aux->dreapta;
	aux->dreapta = *radacina;
	*radacina = aux;
}
void inserareInAVL(Nod** radacina, Bautura b)
{
	if (*radacina == NULL)
	{
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = b;
		nou->stanga = NULL;
		nou->dreapta = NULL;
		*radacina = nou;
	}
	else if (b.id < (*radacina)->info.id)
		inserareInAVL(&(*radacina)->stanga, b);
	else inserareInAVL(&(*radacina)->dreapta, b);

	int diferentaInaltimi = diferentaInaltimiSubarbori(*radacina);
	if (diferentaInaltimi == 2)
	{
		if (diferentaInaltimiSubarbori((*radacina)->stanga) == -1)
		{
			rotireStanga(&(*radacina)->stanga);
		}
		rotireDreapta(&(*radacina));
	}
	if (diferentaInaltimi == -2)
	{
		if (diferentaInaltimiSubarbori((*radacina)->dreapta) == 1)
		{
			rotireDreapta(&(*radacina)->dreapta);
		}
		rotireStanga(&(*radacina));
	}
}

void inordine(Nod* radacina)
{
	if (radacina)
	{

		inordine(radacina->stanga);
		afisareBautura(radacina->info);
		inordine(radacina->dreapta);
	}
}

Nod* listaInAVL(NodPrincipal* graf) 
{
	Nod* radacina = NULL;
	while (graf != NULL) {
		inserareInAVL(&radacina, graf->info);
		graf = graf->next;
	}
	return radacina;
}

//5
NodPrincipal* copieNod(NodPrincipal* nodOriginal) {
	NodPrincipal* nodNou = (NodPrincipal*)malloc(sizeof(NodPrincipal));
	nodNou->info = nodOriginal->info;
	nodNou->next = NULL;
	nodNou->vecini = NULL;
	return nodNou;
}

void inserareListaPrincipalaSimpla(NodPrincipal** cap, NodPrincipal* nod) {
	if (*cap == NULL) {
		*cap = nod;
	}
	else {
		NodPrincipal* aux = *cap;
		while (aux->next != NULL) aux = aux->next;
		aux->next = nod;
	}
}

NodPrincipal* listaNoduriCuPretMaiMareCa(NodPrincipal* graf, float prag) {
	NodPrincipal* listaRezultat = NULL;
	while (graf != NULL) {
		if (graf->info.pret > prag) {
			NodPrincipal* nodNou = copieNod(graf);
			inserareListaPrincipalaSimpla(&listaRezultat, nodNou);
		}
		graf = graf->next;
	}
	return listaRezultat;
}

void afisareListaPrincipalaSimpla(NodPrincipal* cap) {
	printf("\nLista filtrata:\n");
	while (cap != NULL) {
		afisareBautura(cap->info);
		cap = cap->next;
	}
	printf("\n");
}

//6. 
float sumaPreturiDupaCriterii(Nod* radacina, int pragZahar, int pragMl) {
	if (radacina == NULL)
		return 0;

	float sumaStanga = sumaPreturiDupaCriterii(radacina->stanga, pragZahar, pragMl);
	float sumaDreapta = sumaPreturiDupaCriterii(radacina->dreapta, pragZahar, pragMl);

	float sumaCurenta = 0;
	if (radacina->info.nivelZahar <= pragZahar && radacina->info.ml >= pragMl) {
		sumaCurenta = radacina->info.pret;
	}

	return sumaCurenta + sumaStanga + sumaDreapta;
}


//7. 
typedef struct Nod2 Nod2;
struct Nod2 {
	int ID;
	struct Nod2* next;
	struct Nod2* prev;
};
typedef struct ListaDubla ListaDubla;
struct ListaDubla {
	Nod2* first;
	Nod2* last;
};

ListaDubla initListaDubla() {
	ListaDubla ld;
	ld.first = ld.last = NULL;
	return ld;
}

void put(ListaDubla* ld, int ID) {
	Nod2* nou = (Nod2*)malloc(sizeof(Nod2));
	nou->ID = ID;
	nou->next = NULL;
	nou->prev = ld->last;

	if (ld->last == NULL) {
		ld->first = ld->last = nou;
	}
	else {
		ld->last->next = nou;
		ld->last = nou;
	}
}
int pop(ListaDubla* ld) {
	int ID = -1;
	if (ld->first) {
		ID = ld->first->ID;
		Nod* aux = ld->first;
		ld->first = ld->first->next;
		if (ld->first == NULL) {
			ld->last = NULL;
		}
		free(aux);
	}
	return ID;
}

void parcurgereLatime(NodPrincipal* graf, int idStart) {
	if (!graf) return;

	ListaDubla coada = initListaDubla();
	int vizitate[100] = { 0 }; 

	NodPrincipal* start = cautareNodDupaId(graf, idStart);
	if (!start) return;

	put(&coada, start->info.id); 
	vizitate[start->info.id] = 1;

	while (coada.first) {
		int idCurent = pop(&coada);
		NodPrincipal* nodCurent = cautareNodDupaId(graf, idCurent);
		if (nodCurent) {
			printf("Vizitat: ");
			afisareBautura(nodCurent->info);

			NodSecundar* vecin = nodCurent->vecini;
			while (vecin) {
				int idVecin = vecin->info->info.id;
				if (!vizitate[idVecin]) {
					put(&coada, idVecin);
					vizitate[idVecin] = 1;
				}
				vecin = vecin->next;
			}
		}
	}
}

//8.

void elibereazaBautura(Bautura* b) {
	if (b->nume) free(b->nume);
	if (b->dataExpirare) free(b->dataExpirare);
	if (b->producator) free(b->producator);
}

void stergeNodDupaProducator(NodPrincipal** graf, const char* producator) {
	if (!graf || !(*graf)) return;

	NodPrincipal* curent = *graf;
	NodPrincipal* anterior = NULL;

	while (curent != NULL) {
		if (strcmp(curent->info.producator, producator) == 0) {

			NodPrincipal* temp = *graf;
			while (temp != NULL) {
				if (temp != curent) {
					NodSecundar** vecini = &temp->vecini;
					while (*vecini != NULL) {
						if ((*vecini)->info == curent) {
							NodSecundar* deSters = *vecini;
							*vecini = (*vecini)->next;
							free(deSters);
						}
						else {
							vecini = &((*vecini)->next);
						}
					}
				}
				temp = temp->next;
			}


			if (anterior == NULL) {
				*graf = curent->next;
			}
			else {
				anterior->next = curent->next;
			}

			elibereazaBautura(&curent->info);

			NodSecundar* vecin = curent->vecini;
			while (vecin != NULL) {
				NodSecundar* urm = vecin->next;
				free(vecin);
				vecin = urm;
			}

			free(curent);
			return;
		}

		anterior = curent;
		curent = curent->next;
	}
}

//9. 
void elibereazaListaSecundara(NodSecundar* cap) {
	while (cap) {
		NodSecundar* temp = cap;
		cap = cap->next;
		free(temp);
	}
}
void elibereazaListaPrincipala(NodPrincipal* cap) {
	while (cap) {
		NodPrincipal* temp = cap;
		cap = cap->next;

		elibereazaBautura(&temp->info);
		elibereazaListaSecundara(temp->vecini);
		free(temp);
	}
}
void elibereazaAVL(Nod* radacina) {
	if (radacina) {
		elibereazaAVL(radacina->stanga);
		elibereazaAVL(radacina->dreapta);
		elibereazaBautura(&radacina->info);
		free(radacina);
	}
}
void elibereazaListaDubla(ListaDubla* ld) {
	Nod2* current = ld->first;
	while (current) {
		Nod2* temp = current;
		current = current->next;
		free(temp);
	}
	ld->first = ld->last = NULL;
}




int main()
{
	FILE* f = fopen("bauturi.txt", "r");
	NodPrincipal* graf = NULL;
	citireBauturi(&graf, f);
	adaugaMuchie(graf, 1, 2);
	adaugaMuchie(graf, 1, 3);
	adaugaMuchie(graf, 2, 4);
	adaugaMuchie(graf, 2, 5);
	adaugaMuchie(graf, 3, 6);
	adaugaMuchie(graf, 4, 7);
	adaugaMuchie(graf, 5, 7);
	adaugaMuchie(graf, 6, 7);
	adaugaMuchie(graf, 3, 5);
	adaugaMuchie(graf, 1, 7);

	afisareGraf(graf);

	//3.
	printf("Suma preturile produselor cu id mai mic decat 5 este : %f", calculMedie(graf, 5));

	//4.
	Nod* radacinaAVL = listaInAVL(graf);
	printf("\nAfisare arbore: \n");
	inordine(radacinaAVL);

	//5.
	NodPrincipal* listaFiltrata = listaNoduriCuPretMaiMareCa(graf, 5.0);
	afisareListaPrincipalaSimpla(listaFiltrata);

	//6
	float suma = sumaPreturiDupaCriterii(radacinaAVL, 10, 100);
	printf("Suma preturilor pentru bauturile cu nivel zahar <= 10 si ml >= 100 este: %5.2f\n", suma);

	//7. 
	printf("----------------------------------------------\n");
	printf("Parcurgere in latime de la nodul 1\n");
	parcurgereLatime(graf, 1);
	
	//8. 
	printf("\n--------------------------------------------------\n");
	printf("\nDUPA STERGERE\n");
	stergeNodDupaProducator(&graf, "Carpathian Springs"); // va sterge nodul cu Aqua Carpatica.
	afisareGraf(graf);

	//9.
	elibereazaListaPrincipala(graf);
	//elibereazaAVL(radacinaAVL); //obiectele referentiaza aceeasi memorie, si campurile alocate dinamic din structura sunt deja eliberate in dezalocarea grafului.
	//elibereazaListaPrincipala(listaFiltrata);
	fclose(f);

	return 0;
}
