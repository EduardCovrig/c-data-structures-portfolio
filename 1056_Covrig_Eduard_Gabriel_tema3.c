#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Casa Casa;
typedef struct Nod Nod;

struct Casa
{
    char* strada;
    int nr;
    char* propietar;
    int etaje;
    int nr_camere;
    int locuitori;
    char** lista_locuitori;
};

struct Nod
{
    Casa info;
    Nod* next;
};

Casa citireCasa(FILE* f)
{
        Casa c;
        char buffer[30];

        fgets(buffer, 30, f);
        strtok(buffer, "\n");
        c.strada = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
        strcpy_s(c.strada, strlen(buffer) + 1, buffer);

        fgets(buffer, 5, f);
        strtok(buffer, "\n");
        c.nr = atoi(buffer);

        fgets(buffer, 30, f);
        strtok(buffer, "\n");
        c.propietar = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
        strcpy_s(c.propietar, strlen(buffer) + 1, buffer);

        fgets(buffer, 5, f);
        strtok(buffer, "\n");
        c.etaje = atoi(buffer);

        fgets(buffer, 5, f);
        strtok(buffer, "\n");
        c.nr_camere = atoi(buffer);

        fgets(buffer, 5, f);
        strtok(buffer, "\n");
        c.locuitori = atoi(buffer);

        c.lista_locuitori = (char**)malloc(sizeof(char*) * c.locuitori);
        for (int i = 0; i < c.locuitori; i++)
        {
            fgets(buffer, 20, f);
            strtok(buffer, "\n");
            c.lista_locuitori[i] = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
            strcpy_s(c.lista_locuitori[i], strlen(buffer) + 1, buffer);
        }
        return c;
}

//2
void citesteStiva(FILE* f,Nod** varf)
{
    while (!feof(f))
    {
        Nod* nou = (Nod*)malloc(sizeof(Nod));
        nou->info = citireCasa(f);
        nou->next = *varf;
        *varf = nou;
    }
}

void afisareCasa(Casa c)
{
    printf("Strada: %s\n", c.strada);
    printf("Numar: %d\n", c.nr);
    printf("Proprietar: %s\n", c.propietar);
    printf("Numar etaje: %d\n", c.etaje);
    printf("Numar camere: %d\n", c.nr_camere);
    printf("Numar locuitori: %d\n", c.locuitori);
    printf("Lista locuitori:\n");
    for (int i = 0; i < c.locuitori; i++) {
        printf("Locuitor %d: %s\n", i + 1, c.lista_locuitori[i]);
    }
    printf("\n");
}

//7 (Afisarile) 
void afisareStiva(Nod* varf)
{
    while (varf != NULL)
    {
        afisareCasa(varf->info);
        varf = varf->next;
    }
}

void afisareLista(Nod* cap)
{
    while (cap != NULL)
    {
        afisareCasa(cap->info);
        cap = cap->next;
    }
}
//3
Casa selectareCasaLocuitori(Nod* varf, int nrLoc)
{
    while (varf != NULL)
    {
        if (varf->info.locuitori >= nrLoc)
            return varf->info;
        varf = varf->next;
    }
}

//4

typedef struct HashTable HashTable;

struct HashTable
{
    int dimensiune;
    Nod** vector;
};

HashTable initHashTable(int dimensiune)
{
    HashTable tabela;
    tabela.dimensiune = dimensiune;
    tabela.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune);
    for (int i = 0; i < dimensiune; i++)
    {
        tabela.vector[i] = NULL;
    }
    return tabela;
}

int hash(int dimensiune, int nrCamere,int nrLocuitori)
{
    return (nrCamere+nrLocuitori)%dimensiune;
}



void inserareHashTableStiva(HashTable tabela, Nod* stiva)
{
    if (tabela.dimensiune > 0)
    {
        while (stiva != NULL)
        {
            int pozitie = hash(tabela.dimensiune, stiva->info.nr_camere, stiva->info.locuitori);
            printf("Inserare casa la pozitia %d\n", pozitie);
            if (pozitie > 0 && pozitie < tabela.dimensiune)
            {
                Nod* nou = (Nod*)malloc(sizeof(Nod));
                nou->info = stiva->info;
                nou->next = NULL;

                if (tabela.vector[pozitie] == NULL)
                {
                    tabela.vector[pozitie] = nou;
                }
                else
                {
                    Nod* temp = tabela.vector[pozitie];
                    while (temp->next != NULL)
                    {
                        temp = temp->next;
                    }
                    temp->next = nou;
                }
            }
            stiva = stiva->next;
        }
    }
}


void afisareHashTable(HashTable tabela)
{
    for (int i = 0; i < tabela.dimensiune; i++)
    {
        printf("\nPozitia %d: \n", i);
        afisareLista(tabela.vector[i]);
    }
}


//5
void inserareHashTable(HashTable tabela, Casa c)
{
    if (tabela.dimensiune > 0)
    {
        int pozitie = hash(tabela.dimensiune, c.nr_camere, c.locuitori);
        printf("Inserare casa la pozitia %d\n", pozitie);
        if (pozitie > 0 && pozitie < tabela.dimensiune)
        {
            Nod* nou = (Nod*)malloc(sizeof(Nod));
            nou->info = c;
            nou->next = NULL;
            if (tabela.vector[pozitie] == NULL)
            {
                tabela.vector[pozitie] = nou;
            }
            else
            {
                Nod* temp = tabela.vector[pozitie];
                while (temp->next != NULL)
                {
                    temp = temp->next;
                }
                temp->next = nou;
            }
        }
    }
}

//6
void listaHash(Nod** cap, HashTable tabela, int cheie)
{
    if(tabela.dimensiune>0)
        if (cheie > 0 && cheie < tabela.dimensiune)
        {
            if (tabela.vector[cheie] != NULL)
            {
                Nod* capH = tabela.vector[cheie];
                while (capH != NULL)
                {
                    if (*cap == NULL)
                        *cap = capH;
                    else
                    {
                        Nod* copie = *cap;
                        while (copie->next != NULL)
                        {
                            copie = copie->next;
                        }
                        copie->next = capH;
                    }
                    Nod* next = capH->next;
                    capH->next = NULL;
                    capH = next;
                }
            }
        }
}
//8
void stergereDinHashTable(HashTable tabela, int nrEtaje)
{
    if (tabela.dimensiune > 0)
    {
        for (int i = 0; i < tabela.dimensiune; i++) 
        {
            Nod* curent = tabela.vector[i]; 
            Nod* anterior = NULL; 

            while (curent != NULL) 
            {
                if (curent->info.etaje == nrEtaje) 
                {
                    Nod* deSters = curent; 
                    if (anterior == NULL) 
                    {
                        tabela.vector[i] = curent->next; 
                        curent = curent->next; 
                    }
                    else 
                    {
                        anterior->next = curent->next; 
                        curent = curent->next; 
                    }
                    free(deSters->info.strada);
                    free(deSters->info.propietar);
                    if (deSters->info.lista_locuitori != NULL) 
                    {
                        for (int i = 0; i < deSters->info.locuitori; i++)
                        {
                            free(deSters->info.lista_locuitori[i]);
                        }
                        
                    }
                    free(deSters); 
                }
                else
                {
                    anterior = curent; 
                    curent = curent->next; 
                }
            }
        }
    }
}

//9
void dezalocareHash(HashTable* tabela)
{
    for (int i = 0; i < (*tabela).dimensiune; i++)
    {
        Nod* current = tabela->vector[i];
        while (current != NULL)
        {

            free(current->info.strada);
            free(current->info.propietar);


            if (current->info.lista_locuitori != NULL)
            {
                for (int j = 0; j < current->info.locuitori; j++)
                {
                    free(current->info.lista_locuitori[j]);
                }
                free(current->info.lista_locuitori);
            }
            Nod* copie = current;
            current = current->next;
            free(copie);
        }
    }
    free(tabela->vector);
    tabela->vector = NULL;
    tabela->dimensiune = 0;
}

int main()
{

    FILE* f = fopen("case.txt", "r");
    //Casa c = citireCasa(f);
    //afisareCasa(c);

    //2
    Nod* stiva = NULL;
    citesteStiva(f, &stiva);
    afisareStiva(stiva);

    //3
    Casa casaCautata = selectareCasaLocuitori(stiva, 2);
    printf("---------------------------\n");
    printf("Casa cautata este: \n");
    afisareCasa(casaCautata);

    //4
    printf("---------------------------\n");
    HashTable tabela = initHashTable(5);
    inserareHashTableStiva(tabela, stiva);
    afisareHashTable(tabela, 5);
    
    fclose(f);
    //5

    printf("\n------------------5---------------------\n");
    Casa c;

    c.strada = (char*)malloc(sizeof(char) * strlen("Strada Alexandria") + 1);
    strcpy_s(c.strada, strlen("Strada Alexandria") + 1, "Strada Alexandria");
    c.nr = 3;
    c.propietar = (char*)malloc(sizeof(char) * strlen("Marinica Georgica") + 1);
    strcpy_s(c.propietar, strlen("Marinica Georgica") + 1, "Marinica Georgica");
    c.etaje = 1;
    c.nr_camere = 6;
    c.locuitori = 2;
    c.lista_locuitori = (char**)malloc(sizeof(char*) * c.locuitori);
    c.lista_locuitori[0] = (char*)malloc(sizeof(char) * strlen("Marinica Georgica") + 1);
    strcpy_s(c.lista_locuitori[0], strlen("Marinica Georgica") + 1, "Marinica Georgica");
    c.lista_locuitori[1] = (char*)malloc(sizeof(char) * strlen("Ioana Dante") + 1);
    strcpy_s(c.lista_locuitori[1], strlen("Ioana Dante") + 1, "Ioana Dante");
    c.lista_locuitori[2] = (char*)malloc(sizeof(char) * strlen("Ion Cicito") + 1);
    strcpy_s(c.lista_locuitori[2], strlen("Ion Cicito") + 1, "Ion Cicito");

    inserareHashTable(tabela, c);
    afisareHashTable(tabela);


    //6
    printf("\n------------6-------------\n");
    Nod* cap = NULL;
    listaHash(&cap, tabela, 3);
    afisareLista(cap);

    //7
    printf("\n-------------7--------------\n");
    printf("Afisare stiva: \n");
    afisareStiva(stiva);
    printf("\nAfisare tabel: \n");
    afisareHashTable(tabela);

    //8
    printf("\n------------8-----------------\n");
    stergereDinHashTable(tabela, 1);
    afisareHashTable(tabela);


    //9
    dezalocareHash(&tabela);
    afisareHashTable(tabela);

    return 0;
    
}