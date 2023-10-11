#include <stdio.h>
#include <string.h>
#define MAXRIGHE 1000
#define MAXLEN 30
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
void stampa(tratta elenco[MAXRIGHE], int n_righe); // utilizzata per stampare a video o su file
int ordina(ordina_e sel, tratta elenco[MAXRIGHE], int i, int j); // utilizzata per verificare certe condizioni in base al tipo di ordinamento
void mergeSort(tratta elenco[MAXRIGHE], int n_righe, ordina_e sel);
void Merge(tratta elenco[MAXRIGHE], tratta aus[MAXRIGHE], int l, int q, int r, ordina_e sel);
int min(int a, int b);
void menu(int n_righe, tratta elenco[MAXRIGHE]);    // menu principale
int ricercaLin(int n_righe, tratta elenco[MAXRIGHE]);  // ricerca lineare
int ricercaDic(int n_righe, tratta elenco[MAXRIGHE]);  // ricerca dicotomica

int main() {
    FILE* infile;
    int righe;
    tratta tratte[MAXRIGHE];

    infile = fopen(INFILE, "r");
    fscanf(infile, "%d", &righe);

    for (int riga = 0; riga < righe && fscanf(infile, "%s %s %s %s %s %s %d",
                                      tratte[riga].codice, tratte[riga].partenza, tratte[riga].destinazione,
                                      tratte[riga].data, tratte[riga].oraP, tratte[riga].oraA, &tratte[riga].ritardo);
                                      riga++) {
        // si convertono le stringhe data e ora in int più facilmente utilizzabili tramite le funzioni appositamente create
        tratte[riga].date = convertiData(tratte[riga].data);
        tratte[riga].hourP = convertiOra(tratte[riga].oraP);
        tratte[riga].hourA = convertiOra(tratte[riga].oraA);
    }

    fclose(infile);

    // si mostra all'utente un menù a video
    menu(righe, tratte);

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

void menu(int n_righe, tratta elenco[MAXRIGHE]) {
    ordina_e sel;
    int c;

    printf("Selezionare comando: stampa, ordina, ricerca >");

    switch(leggiComando()){
        case c_stampa:
            stampa(elenco, n_righe);
            break;

        case c_ordina:
            printf("Selezionare tipo di ordinamento: data, codice, partenza, arrivo >");
            sel = leggiOrdine();
            if (sel != o_fine) {
                mergeSort(elenco, n_righe, sel);
                printf("Dati ordinati\n");
                printf("Stampare dati ordinati?: si [y] / no [n] (default) >");
                fflush(stdin);
                switch (getchar()) {
                    case 'y':
                        stampa(elenco, n_righe);
                        break;
                    default:;
                }
            }
            else {
                printf("Ordinamento non valido\n");
            }
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

    printf("Programma terminato\n");

}

void stampa(tratta elenco[MAXRIGHE], int n_righe) {
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

int ordina(ordina_e sel, tratta elenco[MAXRIGHE], int i, int j) {
    int condizione;
    switch (sel) {
        case o_data:
            condizione = (elenco[i].date < elenco[j].date ||
                    (elenco[i].date == elenco[j].date && elenco[i].hourP <= elenco[j].hourP));
            break;

        case o_arrivo:
            condizione = (strcmp(elenco[i].destinazione, elenco[j].destinazione) <= 0);
            break;

        case o_partenza:
            condizione = (strcmp(elenco[i].partenza, elenco[j].partenza) <= 0);
            break;

        case o_tratta:
            condizione = (strcmp(elenco[i].codice, elenco[j].codice) <= 0);
            break;
    }
    return condizione;
}

void mergeSort(tratta elenco[MAXRIGHE], int n_righe, ordina_e sel) {
    tratta aus[MAXRIGHE];
    int i, q, m, l = 0, r = n_righe - 1;
    for (m = 1; m <= r - l; m = m+m) {
        for (i = l; i <= r - m; i += m+m) {
            q = i+m-1;
            Merge(elenco, aus, i, q, min(i+m+m-1, r), sel);
        }
    }
}

void Merge(tratta elenco[MAXRIGHE], tratta aus[MAXRIGHE], int l, int q, int r, ordina_e sel) {
    int i = l, j = q+1, k;
    for (k = l; k <= r; k++) {
        if (i > q) {
            aus[k] = elenco[j++];
        }
        else if (j > r) {
            aus[k] = elenco[i++];
        }
        else if (ordina(sel, elenco, i, j)) {
            aus[k] = elenco[i++];
        }
        else {
            aus[k] = elenco[j++];
        }
    }
    for (k = l; k <= r; k++) {
        elenco[k] = aus[k];
    }
}

int min (int a, int b) {
    if (a > b) {
        return b;
    }
    return a;
}

int ricercaLin(int n_righe, tratta elenco[MAXRIGHE]) {
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

int ricercaDic(int n_righe, tratta elenco[MAXRIGHE]) {
    char stazione[MAXLEN+1];
    int len, c = 0;
    printf("[dicotomica] Inserire stazione di partenza (attenzione alle maiuscole) >");
    scanf("%s", stazione);
    len = strlen(stazione);

    /**
     * la ricerca dicotomica necessita un vettore di dati ordinato
     * si nota che si perde la complessità logaritmica della ricerca dicotomica in quanto è presente una funzione
     * linearitmica al suo interno
     **/
    mergeSort(elenco, n_righe, o_partenza);

    for (int x = n_righe / 2, fine = 0, y; !fine; x = x) {
        if (strstr(elenco[x].partenza, stazione) == elenco[x].partenza) {
            y = x;
            while (strstr(elenco[y-1].partenza, stazione) == elenco[y-1].partenza) {
                y--;
                printf("%s %s %s %s %s %s %d\n",
                       elenco[y].codice, elenco[y].partenza, elenco[y].destinazione,
                       elenco[y].data, elenco[y].oraP, elenco[y].oraA, elenco[y].ritardo);
            }
            while (strstr(elenco[x].partenza, stazione) == elenco[x].partenza) {
                c++;
                printf("%s %s %s %s %s %s %d\n",
                       elenco[x].codice, elenco[x].partenza, elenco[x].destinazione,
                       elenco[x].data, elenco[x].oraP, elenco[x].oraA, elenco[x].ritardo);
                x++;
            }
            fine = 1;
        }
        // si confrontano alfabeticamente le stringhe tenendo conto che il prefisso è la meno lunga
        else if (strncmp(stazione, elenco[x].partenza, len) < 0) {
            if (x == 0) {
                fine = 1;
            }
            x = x / 2;
        }
        else {
            if (x == n_righe) {
                fine = 1;
            }
            x = 3 * x / 2;
        }
    }
    return c;
}
