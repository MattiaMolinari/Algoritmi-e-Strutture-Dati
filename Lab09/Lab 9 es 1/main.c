#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

#define INFILE "../grafo4.txt"

/* insieme delle parti (combinazioni semplici) */
void powerset(int pos, int *mark, graph_t G, int E, int k, int *found, int *maxWt, int *sol,
              int start, int *pre, int *post, FILE *fout);

int main() {

	int *mark,      /* vettore per marcare o smarcare l'arco i-esimo */
        *sol,       /* insieme soluzione ottima */
        found = 0,  /* flag per controllare se sia stato già trovato un insieme a cardinalità minima */
        maxWt = 0,  /* peso totale dell'insieme di peso massimo */
        i = 1,      /* variabile per le iterazioni */
        *pre, *post,/* vettori utilizzati per segnare i tempi di scoperta e completamente dei vertici */
        E, V;       /* numero di archi e di vertici */
	FILE *fin, *fout = stdout;
	graph_t G;

	if((fin = fopen(INFILE, "r")) == NULL) {
		fprintf(stdout,"ERRORE APERTURA FILE\n");
        return 1;
	}
    G = GRAPHload(fin);
	fclose(fin);
    E = GRAPHgetEdges(G);
    V = GRAPHgetVertex(G);

	mark = calloc(E, sizeof(int));
    sol = malloc(E * sizeof(int));
	pre	= malloc(V * sizeof(int));
	post = malloc(V * sizeof(int));

    /* ricordando che il modello scelto non genera l'insieme vuoto, si approfitta dell'allocazione del vettore mark
     * tramite funzione calloc per verificare se il grafo non sia già un DAG */
    if (GRAPHcheckDAG(G, mark, pre, post)) {
        printf("Il grafo risulta essere originariamente un DAG, proseguire comunque con cardinalità 1? sì [1] o no [0]>\n");
        fscanf(stdin, "%d", &i);
    }
    if (i) {
        for (i = 1; i < E && !found; i++) {
            powerset(0, mark, G, E, i, &found, &maxWt, sol, 0, pre, post, fout);
        }

        fprintf(fout,"\nSoluzione ottima:\n");
        for(i = 0; i < E; i++) {
            if(sol[i])
                EDGEstore(G, i, fout);
        }
        fprintf(fout, "\tpeso totale = %3d\n", maxWt);

        /* si crea definitivamente il DAG */
        GRAPHremoveEdges(G, sol);
        /* si tampano le distanze richieste */
        GRAPHmaximumPath(G, fout);
    }

    free(pre); free(post);
	GRAPHfree(G);
	free(mark); free(sol);
}



void powerset(int pos, int *mark, graph_t G, int E, int k, int *found, int *maxWt, int *sol,
              int start, int *pre, int *post, FILE *fout) {
	int i, sum;
	if (pos >= k) {
		if (GRAPHcheckDAG(G, mark, pre, post)) {
			if (!(*found)) {
				fprintf(fout, "Numero minimo di archi da rimuovere: %d\n",k);
				*found = 1;
                fprintf(fout,"Soluzioni possibili:\n");
			}

			for (i = 0; i < E; i++) {
				if (mark[i])
                    EDGEstore(G, i, fout);
			}

			sum = GRAPHsumWeight(G, mark);
			fprintf(fout,"\tpeso totale = %3d\n",sum);
			if(sum > *maxWt) {
				*maxWt = sum;
				for(int j = 0; j < E; j++)
                    sol[j] = mark[j];
			}
		}
		return;
	}

	for (i = start; i < E; i++) {
		mark[i] = 1;
        powerset(pos+1, mark, G, E, k, found, maxWt, sol, i+1, pre, post, fout);
		mark[i] = 0;
	}
}
