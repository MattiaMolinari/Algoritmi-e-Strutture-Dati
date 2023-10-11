#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 100
#define MAX_EL 5
#define MAX_DIAG 3

typedef enum {transizione, indietro, avanti} e_tipologia;

typedef struct {
    char nome[MAX_LEN+1];
    e_tipologia tipologia;
    int dirIngresso, dirUscita;
    /* elemento deve dare precedenza ad altri [1] o no [0] */
    int precedenza;
    /* elemento è necessariamente ultimo [1] o no [0] */
    int finale;
    float valore;
    int diff;

} elemento;

float programma(elemento *elementi, int nElementi, int DP, int DD, int maxDiag, int maxEl);
void programmaR(elemento *elementi, int nElementi, int **prog, int **progFin, int *diag, int *diagFin, int r, int c,
                int maxDiag, int maxEl, int DD, int DP, int prec, int diffDiag, int diffProg, float *pValFin);
int condizione(elemento *elementi, int e, int prec);
int controllo(elemento *elementi, int **prog, int maxDiag, int *diag);
float valore(elemento *elementi, int **prog, int maxDiag, int *diag);


int main() {
    FILE *infile;
    elemento *elementi;
    int nElementi, DD = 12, DP = 28;

    infile = fopen("../elementi.txt", "r");
    fscanf(infile, "%d", &nElementi);
    elementi = (elemento *) malloc(nElementi * sizeof(elemento));
    for (int e = 0; e < nElementi; e++)
        fscanf(infile, "%s %d %d %d %d %d %f %d",
               elementi[e].nome,
               &elementi[e].tipologia,
               &elementi[e].dirIngresso, &elementi[e].dirUscita,
               &elementi[e].precedenza, &elementi[e].finale,
               &elementi[e].valore, &elementi[e].diff);
    fclose(infile);

    programma(elementi, nElementi, DP, DD, MAX_DIAG, MAX_EL);

    return 0;
}

float programma (elemento *elementi, int nElementi, int DP, int DD, int maxDiag, int maxEl) {
    int **prog = (int **) malloc(maxDiag * sizeof(int *)),
    **progFin = (int **) malloc(maxDiag * sizeof(int *)),
    *diag = (int *) calloc(maxDiag, sizeof(int)),
    *diagFin = (int *) calloc(maxDiag, sizeof(int));
    int d; float val = 0, valFin = -1;

    for (d = 0; d < maxDiag; d++) {
        prog[d] = (int *) malloc(maxEl * sizeof(int));
        progFin[d] = (int *) malloc(maxEl * sizeof(int));
    }

    programmaR(elementi, nElementi, prog, progFin, diag, diagFin, 0, 0, maxDiag, maxEl,
               DD, DP, -1, 0, 0, &valFin);


    printf("%.3f\n", valFin);
    for (d = 0; d < maxDiag; d++) {
        for (int e = 0; e < diagFin[d]; e++) {
            printf("%s\n", elementi[progFin[d][e]].nome);
        }
        printf("\n");
    }

    for (d = 0; d < maxDiag; d++) {
        free(prog[d]); free(progFin[d]);
    }
    free(prog); free(progFin); free(diag);

    return val;

}

void programmaR(elemento *elementi, int nElementi, int **prog, int **progFin, int *diag, int *diagFin, int r, int c,
                int maxDiag, int maxEl, int DD, int DP, int prec, int diffDiag, int diffProg, float *pValFin) {
    int e, err, diff; float val;
    if (c >= maxEl) {
        for (e = 0, err = 1; e < c; e++) {
            if (elementi[prog[r][e]].tipologia != transizione)
                err = 0;
        }
        if (err)
            return;

        diag[r] = c;

        if (r+1 >= maxDiag) {
           if ((val = valore(elementi, prog, maxDiag, diag)) >= *pValFin && controllo(elementi, prog, maxDiag, diag)) {
               for (int d = 0; d < r+1; d++) {
                   diagFin[d] = diag[d];
                   for (e = 0; e < diag[d]; e++)
                       progFin[d][e] = prog[d][e];
                   *pValFin = val;
               }
           }
        }
        else {
            programmaR(elementi, nElementi, prog, progFin, diag, diagFin, r+1, 0, maxDiag, maxEl,
                       DD, DP, -1, 0, diffProg, pValFin);
        }
        return;
    }

    prog[r][c] = -1;
    for (e = 0; e < nElementi; e++) {
        diff = elementi[e].diff;
        if (condizione(elementi, e, prec) && diffDiag + diff <= DD && diffProg + diff <= DP) {
            prog[r][c] = e;
            programmaR(elementi, nElementi, prog, progFin, diag, diagFin, r, c+1, maxDiag, maxEl, DD, DP,
                       e,diffDiag + diff, diffProg + diff, pValFin);
        }
    }

    if (prog[r][c] == -1)
        programmaR(elementi, nElementi, prog, progFin, diag, diagFin, r, c, maxDiag, c,
                   DD, DP, prec, diffDiag, diffProg, pValFin);
}


int condizione(elemento *elementi, int e, int prec) {
    int good = 1;
    if (prec == -1) {
        good *= !elementi[e].precedenza; // 1 => può essere primo
        good *= (elementi[e].dirIngresso == 1); // 1 => si inizia la diagonale frontalmente
    }
    else {
        good *= (elementi[e].dirIngresso == elementi[prec].dirUscita); // 1 => sono compatibili
        good *= !elementi[prec].finale; // 1 => quello precedente non deve essere l'ultimo
    }
    return good;
}

int controllo(elemento *elementi, int **prog, int maxDiag, int *diag) {
    int d, e, sqz, err_avanti = 0, err_indietro = 0, err_sequenza = 0;

    for (d = 0, sqz = 0; d < maxDiag; d++) {
        for (e = 0; e < diag[d]; e++) {
            if (elementi[prog[d][e]].tipologia == avanti)
                err_avanti = 1;
            if (elementi[prog[d][e]].tipologia == indietro)
                err_indietro = 1;
            if (elementi[prog[d][e]].tipologia != transizione)
                sqz++;
        }
        if (sqz > 1)
            err_sequenza = 1;
    }
    return err_sequenza*err_avanti*err_indietro; // se il valore di ritorno è 0 il programma non è valido
}

float valore(elemento *elementi, int **prog, int maxDiag, int *diag) {
    int d = maxDiag-1, e = diag[d]-1; float val = 0, bonus = 1;
    if (elementi[prog[d][e]].diff >= 8)
        bonus = 1.5f;

    for (d = 0; d < maxDiag; d++)
        for (e = 0; e < diag[d]; e++) {
            if (d == maxDiag-1)
                val += bonus*elementi[prog[d][e]].valore;
            else
                val += elementi[prog[d][e]].valore;
        }
    return val;
}