#include <stdio.h>
#include <stdlib.h>
#define INFILE "../grafo.txt"

/** per la costruzione dell'insieme delle parti si ricorre al modello delle disposizioni ripetute:
 *  avendo infatti n vertici rappresentati da interi tra 0 e n-1, risulta comodo avere la soluzione di cardinalità n e k=2
 */

void powerset(int pos, int *sol ,int n, int e, int **E);

int main() {
    FILE *infile;
    int n, e, **E, *sol;

    infile = fopen(INFILE, "r");
    fscanf(infile, "%d %d", &n, &e);
    E = (int **) malloc(e*sizeof(int *));
    for (int i = 0; i < e; i++) {
        E[i] = (int *) malloc(2 * sizeof(int));
        fscanf(infile, "%d %d", &E[i][0], &E[i][1]);
    }
    fclose(infile);

    sol = (int *) calloc(n, sizeof(int));

    printf("I vertex cover sono:\n");
    powerset(0, sol, n, e, E);

    for (int i = 0; i < e; i++) {
        free(E[i]);
    }
    free(E); free(sol);
    return 0;
}

void powerset(int pos, int *sol, int n, int e, int **E) {
    int j;
    if (pos >= n) {
        /** si controlla che il sottoinsieme sia valido verificando che per ogni arco ci sia almeno un elemento come vertice */
        for (int arco = 0, err; arco < e; arco++) {
            err = 1;
            for (j=0; j<n; j++)
                if (sol[j] != 0 && (E[arco][0] == j || E[arco][1] == j))
                    err = 0;
            if (err)
                /** se il sottoinsieme non è valido non lo si stampa */
                return;
        }
        printf(" { \t" );
        for (j=0; j<n; j++)
            if (sol[j]!=0)
                printf(" %d \t" , j);
        printf(" } \n" );
        return ;
    }
    sol[pos] = 0;
    powerset(pos+1, sol , n, e, E);
    sol[pos] = 1;
    powerset(pos+1 , sol , n, e, E);
}
