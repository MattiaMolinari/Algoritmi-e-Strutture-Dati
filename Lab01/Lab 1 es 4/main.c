#include <stdio.h>
#include <string.h>
#define MAXRIGHE 1000
#define MAXLEN 30
#define MAXORD 4
#define INFILE "../corse.txt"

typedef enum {o_data, o_tratta, o_partenza, o_arrivo, o_fine} ordina_e;
typedef struct {
    char codice[MAXLEN+1], partenza[MAXLEN+1], destinazione[MAXLEN+1], data[MAXLEN+1], oraP[MAXLEN+1], oraA[MAXLEN+1];
    int date, hourP, hourA, ritardo;
} tratta;

ordina_e leggiOrdine();
int convertiData(char date[MAXLEN+1]);
int convertiOra(char hour[MAXLEN+1]);
int ordina(ordina_e sel, tratta *v[MAXRIGHE], tratta *x, int j);
/**
 * utilizzando solo un vettore di puntatori senza modificare l'originale per ogni ordinamento,
 * non è più possibile utilizzare il merge sort ma è necessario un algoritmo di ordinamento in loco
 **/
void InsertionSort(tratta *v[MAXRIGHE], int n_righe, ordina_e sel);

/**
 * SI RIPORTANO SOLAMENTE LE PARTI EFFETTIVAMENTE CAMBIATE DELL'ESERCIZIO 3
 **/

int main() {
    FILE *infile;
    tratta *matrice[MAXORD][MAXRIGHE];
    tratta tratte[MAXRIGHE];
    ordina_e ordinamenti[MAXORD];
    int ord = -1, righe = 0;

    infile = fopen(INFILE, "r");
    fscanf(infile, "%d", &righe);

    for (int riga = 0; riga < righe && fscanf(infile, "%s %s %s %s %s %s %d",
                                              tratte[riga].codice, tratte[riga].partenza, tratte[riga].destinazione,
                                              tratte[riga].data, tratte[riga].oraP, tratte[riga].oraA, &tratte[riga].ritardo);
         riga++) {
        tratte[riga].date = convertiData(tratte[riga].data);
        tratte[riga].hourP = convertiOra(tratte[riga].oraP);
        tratte[riga].hourA = convertiOra(tratte[riga].oraA);
    }

    fclose(infile);

    printf("Selezionare tipo di ordinamento (max 4): data, codice, partenza, arrivo\n(qualsiasi altro input viene letto come terminatore)\n");
    do {
        ord++;
        printf("%d su 4 >", ord+1);
        ordinamenti[ord] = leggiOrdine();
    }
    while(ordinamenti[ord] != o_fine && ord < MAXORD-1);

    for (int m = 0; m < righe; m++) {   // si legge il vettore originale una volta
        for (int n = 0; ordinamenti[n] != o_fine && n < MAXORD; n++) {
            matrice[n][m] = &tratte[m];
        }
    }

    for (int n = 0; ordinamenti[n] != o_fine && n < MAXORD; n++) {
        InsertionSort(matrice[n], righe, ordinamenti[n]);
        printf("\n\nORDINAMENTO %d\n", n+1);
        for (int m = 0; m < righe; m++) {
            printf("%s %s %s %s %s %s %d\n",
                   matrice[n][m]->codice, matrice[n][m]->partenza, matrice[n][m]->destinazione,
                   matrice[n][m]->data, matrice[n][m]->oraP,matrice[n][m]->oraA, matrice[n][m]->ritardo);
        }
    }
    if (ord == 0) {
        printf("Nessun ordinamento valido inserito\n");
    }

    return 0;
}

ordina_e leggiOrdine() {
    ordina_e comando;
    char parola[MAXLEN+1], parole[4][10] = {"data", "codice", "partenza", "arrivo"};

    scanf("%s", parola);

    for (comando = o_data; comando < o_fine && strcmp(parole[comando], parola) != 0; comando++);

    return comando;
}

int convertiData(char date[MAXLEN+1]) {
    int y, m, d;
    sscanf(date, "%d/%d/%d", &y, &m, &d);
    return y*10000 + m*100 + d;
}

int convertiOra(char hour[MAXLEN+1]) {
    int h, m, s;
    sscanf(hour, "%d:%d:%d", &h, &m, &s);
    return h*60*60 + m*60 + s;
}

int ordina(ordina_e sel, tratta *v[MAXRIGHE], tratta *x, int j) {
    int condizione;
    switch (sel) {
        case o_data:
            condizione = (x->date < v[j]->date ||
                          (x->date == v[j]->date && x->hourP < v[j]->hourP));
            break;

        case o_arrivo:
            condizione = (strcmp(x->destinazione, v[j]->destinazione) < 0);
            break;

        case o_partenza:
            condizione = (strcmp(x->partenza, v[j]->partenza) < 0);
            break;

        case o_tratta:
            condizione = (strcmp(x->codice, v[j]->codice) < 0);
            break;
    }
    return condizione;
}

void InsertionSort(tratta *v[MAXRIGHE], int n_righe, ordina_e sel) {
    tratta *x;
    int i, j, l=0, r=n_righe-1;

    for (i = l+1; i <= r; i++) {
        x = v[i];
        j = i - 1;
        while (j >= l && ordina(sel, v, x, j)){
            v[j+1] = v[j];
            j--;
        }
        v[j+1] = x;
    }
}
