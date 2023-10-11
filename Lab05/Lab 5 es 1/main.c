#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int s;
    int f;
    int d;
} att;

void mergeSort(att *v, int n);
void bottomUpMergeSort(att *v, att *aus, int n, int s);
void merge(att *v, att *aus, int l, int q, int r, int s);
void attSel(int n, att *v);
int powerset(int pos, int n, att *v, int *sol, int *solFin, int max, int *maxFin, int f);
void updateSolFin(int n, int *sol, int *solFin, int max, int *maxFin);

int main() {
    FILE *infile;
    att *v;
    int n;

    infile = fopen("../att1.txt", "r");
    fscanf(infile, "%d", &n);
    v = (att *) malloc(n*sizeof(att));
    for (int i = 0; i < n; i++) {
        fscanf(infile, "%d %d", &v[i].s, &v[i].f);
        v[i].d = v[i].f - v[i].s;
    }
    fclose(infile);

    /** si ordinano gli intervalli per tempo di inizio crescente e in caso di uguaglianze per tempo di fine crescente:
     * si utilizza un algoritmo linearitmico che ordina prima per tempi di fine crescenti, poi per tempi di inizio crescenti
     * richiamando il modello controintuitivo del radix sort */
    mergeSort(v, n);
    attSel(n, v);
    free(v);
    return 0;
}

void attSel(int n, att *v) {
    int pos = 0, max = 0, maxFin = 0, *sol, *solFin, f = 0;
    sol = (int *) calloc(n,sizeof(int)); solFin = (int *) calloc(n,sizeof(int));
    max = powerset(pos, n, v, sol, solFin, max, &maxFin, f);
    for (int i = 0; i < n; i++) {
        if (solFin[i] != 0)
            printf("(%d %d) \t", v[i].s, v[i].f);
    }
    printf("\nDurata massima = %d", maxFin);
    free(sol); free(solFin);
}

int powerset(int pos, int n, att *v, int *sol, int *solFin, int max, int *maxFin, int f) {
    int i;
    if (pos >= n) {
        max = 0;
        for (i = 0; i < n; i++) {
            if (sol[i] != 0)
                max += v[i].d;
        }
        return max;
    }
    if (v[pos].s >= f) {
        sol[pos] = 1;
        max = powerset(pos+1, n, v, sol, solFin, max, maxFin, v[pos].f);
        updateSolFin(n, sol, solFin, max, maxFin);
    }
    sol[pos] = 0;
    max = powerset(pos+1, n, v, sol, solFin, max, maxFin, f);
    updateSolFin(n, sol, solFin, max, maxFin);

    return max;

}

void updateSolFin(int n, int *sol, int *solFin, int max, int *maxFin) {
    int i;
    if (max > *maxFin) {
        *maxFin = max;
        for (i = 0; i < n; i++)
            solFin[i] = sol[i];
    }
}

void mergeSort(att *v, int n) {
    att *aus = malloc(n* sizeof(att));
    bottomUpMergeSort(v, aus, n, 0);
    bottomUpMergeSort(v, aus, n, 1);
    free(aus);
}
void bottomUpMergeSort(att *v, att *aus, int n, int s) {
    int l=0, r=n-1, i, q, m, min;
    for (m=1; m<=r-l;m=m+m)
        for (i=l;i<=r-m;i+=m+m) {
            q=i+m-1;
            if (i+m+m-1 <= r) min = i+m+m-1;
            else min = r;
            merge(v, aus, i, q, min, s);
        }
}
void merge(att *v, att *aus, int l, int q, int r, int s) {
    int i=l, j=q+1, k;
    for (k=l; k <=r; k++) {
        if (i > q)
            aus[k]=v[j++];
        else if (j > r)
            aus[k]=v[i++];
        else if ((!s && v[i].f <= v[j].f) || (s && v[i].s <= v[j].s))
            aus[k]=v[i++];
        else
            aus[k]=v[j++];
    }
    for (k=l; k<=r; k++)
        v[k]=aus[k];
}