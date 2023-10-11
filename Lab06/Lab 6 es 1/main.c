#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int s;
    int f;
    int d;
} att;

/** applicabilità della programmazione dinamica partendo da un array di intervalli ordinati:
 * data una soluzione ottima, se non fosse ottima la soluzione al problema per ogni prefisso (sottoproblema),
 * se ne potrebbe trovare una migliore e di conseguenza la soluzione ottima non sarebbe tale (assurdo)
 * => sottostruttura ottima
 */

void mergeSort(att *v, int n);
void bottomUpMergeSort(att *v, att *aus, int n, int s);
void merge(att *v, att *aus, int l, int q, int r, int s);
int max(int a, int b);
int solveR(att *v, int i);
int solve(att *v, int n);
void solveDP(att *v, int n);
void stampaDP(att *v, int *p, int last);

int main() {
    FILE *infile;
    att *v;
    int n;

    infile = fopen("../att2.txt", "r");
    fscanf(infile, "%d", &n);
    v = (att *) malloc((n)*sizeof(att));
    for (int i = 0; i < n; i++) {
        fscanf(infile, "%d %d", &v[i].s, &v[i].f);
        v[i].d = v[i].f - v[i].s;
    }
    fclose(infile);

    mergeSort(v, n);

    printf("%d\n", solve(v, n));
    solveDP(v, n);
    free(v);
    return 0;
}

int solve(att *v, int n) {
    int ris = 0;
    for (int i = n-1; i >= 0; i--)
        ris = max(ris, solveR(v, i));
    return ris;
}

int solveR(att *v, int i) {
    int j, ris;
    if (i == 0)
        return v[0].d;
    ris = v[i].d;
    for (j=0; j < i; j++)
        if (v[j].f <= v[i].s)
            ris = max(ris, v[i].d + solveR(v, j));
    return ris;
}

void solveDP(att *v, int n) {
    int *l = (int *) malloc(n*sizeof(int)), *p = (int *) malloc(n*sizeof(int)), ris = 0, last = 1, i, j;
    l[0] = v[0].d, p[0]= -1;
    for (i=1; i<n; i++) {
        l[i] = v[i].d; p[i] = -1;
        for (j=0; j<i; j++)
            if ((v[j].f <= v[i].s) && (l[i] < v[i].d + l[j])) {
                l[i] = v[i].d + l[j]; p[i] = j;
            }
        if (ris < l[i])
            ris = l[i]; last = i;
    }
    printf("One of the Longest Increasing Sequences is ");
    stampaDP(v, p, last);
    printf("and its length is %d\n", ris);
    free(l), free(p);
}

void stampaDP(att *v, int *p, int i) {
    if (p[i]==-1) {
        printf("(%d %d) ", v[i].s, v[i].f);
        return;
    }
    stampaDP(v, p, p[i]);
    printf("(%d %d) ", v[i].s, v[i].f);
}

int max(int a, int b) {
    if (a > b)
        return a;
    return b;
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