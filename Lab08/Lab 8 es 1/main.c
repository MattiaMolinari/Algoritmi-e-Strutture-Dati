#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 100
#define MAX_EL 5
#define MAX_DIAG 3

typedef struct {
    char nome[MAX_LEN+1];
    int tipologia, dirIngresso, dirUscita;
    /* elemento deve dare precedenza ad altri [1] o no [0] */
    int precedenza;
    /* elemento necessariamente ultimo [1] o no [0] */
    int finale;
    float valore;
    int diff;

} elemento;

float programma(elemento *elementi, int nElementi, int DP, int DD, int maxDiag, int maxEl);
int condizione(elemento *elementi, int e, int prec);
int controllo(elemento *elementi, int **prog, int maxDiag, int *diag);



int main() {
    FILE *infile;
    elemento *elementi;
    int nElementi, DD = 10, DP = 20;

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
            prec = -1, best, min;
    float valDiag, val = 0.0f, maxRapp = 0.0f, bonus, diff, acr, av = 0, in = 0;

    for (int e = 0, d = DD; e < nElementi; e++)
        if (elementi[e].tipologia != 0 && elementi[e].diff < d) {
            min = e; d = elementi[e].diff;
        }

    for (int d = 0; d < maxDiag; d++) {
        prog[d] = (int *) malloc(maxEl * sizeof(int));
        for (int e = 0; e < maxEl; e++)
            prog[d][e] = -1;
    }

    for (int d = maxDiag - 1; d >= 0; d--) {
        valDiag = 0.0f, bonus = 1.0f; best = 0; acr = 0;
        for (int e = 0; best != -1 && e < maxEl; e++) {
            best = -1;
            for (int n = 0; n < nElementi; n++) {
                diff = (float)elementi[n].diff;
                if (!acr) {
                    diff += (float)elementi[min].diff;
                    acr = 1;
                }
                if (condizione(elementi, n, prec) && valDiag + diff <= (float) DD && val + diff <= (float) DP &&
                    (float) elementi[n].valore / diff > maxRapp)
                    best = n;
            }
            if (best != -1) {
                if (d == maxDiag && elementi[best].diff >= 8)
                    bonus = 1.5f;
                prog[d][e] = best;
                prec = best;
                valDiag += (float) elementi[best].diff;
            }
        }
        val += valDiag * bonus;
    }

    printf("%.3f\n", val);
    for (int d = 0; d < maxDiag; d++) {
        for (int e = 0; prog[d][e] != -1; e++) {
            printf("%s\n", elementi[prog[d][e]].nome);
        }
        printf("\n");
    }

    for (int d = 0; d < maxDiag; d++) {
        free(prog[d]);
    }
    free(prog);
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