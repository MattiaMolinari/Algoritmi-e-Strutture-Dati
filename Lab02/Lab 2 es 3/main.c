#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXLEN 30
#define MAXORD 4
#define INFILE "../corse.txt"
#define OUTFILE "../log.txt"

typedef enum {c_stampa, c_ordina, c_ricerca, c_fine} comando_e;
typedef enum {o_data, o_tratta, o_partenza, o_arrivo, o_fine} ordina_e;
typedef struct {
    char codice[MAXLEN+1], partenza[MAXLEN+1], destinazione[MAXLEN+1], data[MAXLEN+1], oraP[MAXLEN+1], oraA[MAXLEN+1];
    int date, hourP, hourA, ritardo;
} tratta;

comando_e leggiComando();   // legge l'azione voluta
ordina_e leggiOrdine();     // legge il tipo di ordinamento voluto
int convertiData(char date[MAXLEN+1]);   // converte la stringa della data in un int
int convertiOra(char hour[MAXLEN+1]);    // converte la stringa dell'ora in un int
tratta *lettura(int *n_righe, FILE *infile);
void stampa(tratta *elenco, int n_righe); // utilizzata per stampare a video o su file
int ordina(ordina_e sel, tratta **v, tratta *x, int j); // utilizzata per verificare certe condizioni in base al tipo di ordinamento
void InsertionSort(tratta **v, int n_righe, ordina_e sel);
void multiOrdinamento(tratta *elenco, int n_righe);
void menu(int n_righe, tratta *elenco);    // menu principale
int ricercaLin(int n_righe, tratta *elenco);  // ricerca lineare
int ricercaDic(int n_righe, tratta *elenco);  // ricerca dicotomica

int main() {
    tratta *tratte;
    char *nomeFile;
    int righe = 0;

    // la prima volta si legge il file originale
    tratte = lettura(&righe, fopen(INFILE, "r"));

    while (righe != -1) {
        // si mostra all'utente un menù a video
        menu(righe, tratte);

        printf("\nTerminare esecuzione [t], cambiare dati [n] o continuare con file corrente [c] (default) >");
        fflush(stdin);
        switch (getchar()) {
            case 'n':
                free(tratte);
                printf("Inserire nome del nuovo file >");
                nomeFile = (char *) malloc((MAXLEN+1)*sizeof(char));
                nomeFile[0] = '.'; nomeFile[1] = '.'; nomeFile[2] = '/';
                scanf( "%s", &nomeFile[3]);
                tratte = lettura(&righe, fopen(nomeFile, "r"));
                free(nomeFile);
                break;
            case 't':
                printf("Programma terminato\n");
                righe = -1;
                break;
            default:
                printf("Si continua con gli stessi dati\n");
        }
    }

    free(tratte);
    return 0;
}

comando_e leggiComando() {
    comando_e comando;
    char parola[MAXLEN+1], parole[3][10] = {"stampa", "ordina", "ricerca"};

    scanf("%s", parola);

    for (comando = c_stampa; comando < c_fine && strcmp(parole[comando], parola) != 0; comando++);

    return comando;
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

tratta *lettura(int *n_righe, FILE *infile) {
    tratta *tratte;

    fscanf(infile, "%d", n_righe);
    tratte = (tratta *)malloc((*n_righe)*sizeof(tratta));

    for (int riga = 0; riga < *n_righe && fscanf(infile, "%s %s %s %s %s %s %d",
                                              tratte[riga].codice, tratte[riga].partenza, tratte[riga].destinazione,
                                              tratte[riga].data, tratte[riga].oraP, tratte[riga].oraA, &tratte[riga].ritardo);
         riga++) {
        // si convertono le stringhe data e ora in int più facilmente utilizzabili tramite le funzioni appositamente create
        tratte[riga].date = convertiData(tratte[riga].data);
        tratte[riga].hourP = convertiOra(tratte[riga].oraP);
        tratte[riga].hourA = convertiOra(tratte[riga].oraA);
    }

    fclose(infile);

    return tratte;
}

void menu(int n_righe, tratta *elenco) {
    int c;

    printf("Selezionare comando: stampa, ordina, ricerca >");

    switch(leggiComando()){
        case c_stampa:
            stampa(elenco, n_righe);
            break;

        case c_ordina:
            multiOrdinamento(elenco, n_righe);
            break;

        case c_ricerca:
            printf("Selezionare opzioni di ricerca: dicotomica [d] o lineare [l] (default) >");
            fflush(stdin);
            switch (getchar()) {
                case 'd':
                    c = ricercaDic(n_righe, elenco);
                    break;
                default: // case 'l':
                    c = ricercaLin(n_righe, elenco);
            }
            if (c == 0) {
                printf("Nessun dato trovato\n");
            }
            break;

        default:
            printf("Comando non valido\n");
    }
}

void stampa(tratta *elenco, int n_righe) {
    FILE *outfile;
    printf("Selezionare opzione di stampa: file [f] o video [v] (default) >");
    fflush(stdin);
    switch (getchar()) {
        case 'f':
            outfile = fopen(OUTFILE, "w");
            break;
        default: // case 'v': (si evitano problemi)
            printf("\n");
            outfile = stdout;
    }
    for (int x = 0; x < n_righe; x++){
        fprintf(outfile, "%s %s %s %s %s %s %d\n",
                elenco[x].codice, elenco[x].partenza, elenco[x].destinazione,
                elenco[x].data, elenco[x].oraP,elenco[x].oraA, elenco[x].ritardo);
    }
    if (outfile != stdout){
        printf("Stampa su file \"%s\" eseguita\n", OUTFILE);
        fclose(outfile);
    }
}

int ordina(ordina_e sel, tratta **v, tratta *x, int j) {
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

void multiOrdinamento(tratta *elenco, int n_righe) {
    tratta ***matrice;
    ordina_e ordinamenti[MAXORD];
    int ord = -1;
    printf("Selezionare tipo di ordinamento (max 4): data, codice, partenza, arrivo\n"
           "(qualsiasi altro input viene letto come terminatore)\n");
    do {
        ord++;
        printf("%d su 4 >", ord+1);
        ordinamenti[ord] = leggiOrdine();
    }
    while(ordinamenti[ord] != o_fine && ord < MAXORD-1);

    matrice = (tratta ***) malloc((ord+1)*sizeof(tratta **));

    for (int n = 0; n <= ord; n++) {
        matrice[n] = (tratta **) malloc(n_righe*sizeof (tratta*));
    }

    for (int m = 0; m < n_righe; m++) {   // si legge il vettore originale una volta
        for (int n = 0; ordinamenti[n] != o_fine && n < MAXORD; n++) {
            matrice[n][m] = &elenco[m];
        }
    }

    for (int n = 0; ordinamenti[n] != o_fine && n < MAXORD; n++) {
        InsertionSort(matrice[n], n_righe, ordinamenti[n]);
        printf("\n\nORDINAMENTO %d\n", n+1);
        for (int m = 0; m < n_righe; m++) {
            printf("%s %s %s %s %s %s %d\n",
                   matrice[n][m]->codice, matrice[n][m]->partenza, matrice[n][m]->destinazione,
                   matrice[n][m]->data, matrice[n][m]->oraP,matrice[n][m]->oraA, matrice[n][m]->ritardo);
        }
    }
    if (ord == 0) {
        printf("Nessun ordinamento valido inserito\n");
    }

    for (int n = 0; n <= ord; n++) {
        free(matrice[n]);
    }
    free(matrice);
}

void InsertionSort(tratta **v, int n_righe, ordina_e sel) {
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

int ricercaLin(int n_righe, tratta *elenco) {
    char stazione[MAXLEN+1];
    int c = 0;
    printf("[lineare] Inserire stazione di partenza (attenzione alle maiuscole) >");
    scanf("%s", stazione);

    for (int x = 0; x < n_righe; x++){
        // si selezionano solo stazioni coincidenti con il prefisso (a partire da prima lettera)
        if (strstr(elenco[x].partenza, stazione) == elenco[x].partenza){
            c++;
            printf( "%s %s %s %s %s %s %d\n",
                    elenco[x].codice, elenco[x].partenza, elenco[x].destinazione,
                    elenco[x].data, elenco[x].oraP, elenco[x].oraA, elenco[x].ritardo);
        }
    }
    return c;
}

int ricercaDic(int n_righe, tratta *elenco) {
    char stazione[MAXLEN+1];
    tratta **ordinare;
    int len, c = 0;

    ordinare = (tratta **) malloc(n_righe*sizeof(tratta*));
    for (int x = 0; x < n_righe; x++) {
        ordinare[x] = &elenco[x];
    }

    /**
    * la ricerca dicotomica necessita un vettore di dati ordinato
    * non essendo mai ordinato direttamente in base alla richiesta dell'esercizio 4 del laboratorio 1,
    * il vettore sarà sempre non ordinato
    * si nota che si perde la complessità logaritmica della ricerca dicotomica in quanto è presente una funzione
    * quadratica al suo interno
    **/

    InsertionSort(ordinare, n_righe, o_partenza);

    printf("[dicotomica] Inserire stazione di partenza (attenzione alle maiuscole) >");
    scanf("%s", stazione);
    len = strlen(stazione);

    for (int x = n_righe / 2, fine = 0, y; !fine; x = x) {
        if (strstr(ordinare[x]->partenza, stazione) == ordinare[x]->partenza) {
            y = x;
            while (y>=1 && strstr(ordinare[y-1]->partenza, stazione) == ordinare[y-1]->partenza) {
                y--;
                printf("%s %s %s %s %s %s %d\n",
                       ordinare[y]->codice, ordinare[y]->partenza, ordinare[y]->destinazione,
                       ordinare[y]->data, ordinare[y]->oraP, ordinare[y]->oraA, ordinare[y]->ritardo);
            }
            while (x < n_righe && strstr(ordinare[x]->partenza, stazione) == ordinare[x]->partenza) {
                c++;
                printf("%s %s %s %s %s %s %d\n",
                       ordinare[x]->codice, ordinare[x]->partenza, ordinare[x]->destinazione,
                       ordinare[x]->data, ordinare[x]->oraP, ordinare[x]->oraA, ordinare[x]->ritardo);
                x++;
            }
            fine = 1;
        }
        // si confrontano alfabeticamente le stringhe tenendo conto che il prefisso è la meno lunga
        else if (strncmp(stazione, ordinare[x]->partenza, len) < 0) {
            x = x / 2;
            if (x == 0) {
                fine = 1;
            }

        }
        else {
            x = 3 * x / 2;
            if (x == n_righe) {
                fine = 1;
            }

        }
    }
    free(ordinare);
    return c;
}

