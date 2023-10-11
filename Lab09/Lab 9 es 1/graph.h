#ifndef GRAPH_H
#define GRAPH_H

#define MAXC 30

typedef struct graph_s* graph_t;

/* inizializzazione durante lettura da file e deallocazione */
graph_t GRAPHload(FILE* fin);
void GRAPHfree(graph_t G);
/* stampa della lista delle adiacenze del grafo */
void GRAPHadjStore(graph_t G, FILE *fout);
/* controllo che il grafo sia un DAG [1] o meno [0] */
int GRAPHcheckDAG(graph_t G, int *mark, int *pre, int *post);
/* somma dei pesi degli archi segnati nel vettore mark */
int GRAPHsumWeight(graph_t G, int *mark);
/* aggiunta o rimozione degli archi segnati da mark */
void GRAPHaddEdges(graph_t G, int *mark);
void GRAPHremoveEdges(graph_t G, int *mark);
/* calcolo del percorso massimo */
void GRAPHmaximumPath(graph_t G, FILE *fout);
/* stampa dell'arco */
void EDGEstore(graph_t G, int index, FILE *fout);
/* funzioni che restituiscono rispettivamente il numero di archi e di vertici */
int GRAPHgetEdges(graph_t G);
int GRAPHgetVertex(graph_t G);

#endif