#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "st.h"

/* arco */
typedef struct edge_s {
	int v, w, wt;
} edge_t;

/* puntatore a nodo di una lista */
typedef struct node *link;
/* nodo di una lista */
struct node {
	int index, wt;
	link next;
};

/* lista */
typedef struct list_s{
	link head;
} list_t;

/* grafo */
struct graph_s {
	st_t st;
	edge_t *edges;
	int E, V;
	list_t *adj;
};

static void GRAPHadjLoad(graph_t G);
static int EDGEload(graph_t G, FILE *fin, edge_t *edge);
static int dfsR(graph_t G, edge_t edge, int *time, int *pre, int *post);
static void TSdfsR(graph_t G, int v, int *ts, int *pre, int *time);


graph_t GRAPHload(FILE* fin) {
	int i;
	char name[MAXC+1];
	edge_t edge;

	graph_t G = malloc(sizeof(struct graph_s));

	fscanf(fin, "%d", &(G->V));

    G->st = STinit(G->V);
    G->adj = malloc(G->V * sizeof(list_t));
    G->edges = malloc(2 * G->V * sizeof(edge_t));
    G->E = 0;

	for (i = 0; i < G->V; i++) {
		fscanf(fin, "%s", name);
		STinsert(G->st, name);
        G->adj[i].head = NULL;
	}

	while(EDGEload(G, fin, &edge)) {
        G->edges[G->E++] = edge;
	}

    GRAPHadjLoad(G);

	return G;
}
void GRAPHfree(graph_t G) {
    int i;
    link x, tmp;

    STfree(G->st);

    for(i = 0; i < G->V; i++) {
        for (x = G->adj[i].head; x != NULL; x = tmp) {
            tmp = x->next;
            free(x);
        }
    }
    free(G->adj);
    free(G->edges);
    free(G);
}

static int EDGEload(graph_t G, FILE *fin, edge_t *edge) {
	char name1[MAXC + 1], name2[MAXC + 1];
    int value;

	value = fscanf(fin, "%s %s %d", name1, name2, &(edge->wt));
	edge->v = STgetIndex(G->st, name1);
	edge->w = STgetIndex(G->st, name2);
	return (value == 3);
}

static void GRAPHadjLoad(graph_t G) {
	link x;
	for (int i = 0; i < G->E; i++) {
        x = malloc(sizeof(* x));
        x->index = G->edges[i].w;
        x->wt = G->edges[i].wt;
        /* inserimento in testa */
        x->next = G->adj[G->edges[i].v].head;
        G->adj[G->edges[i].v].head = x;
	}
}
void GRAPHadjStore(graph_t G, FILE *fout) {
    link x;
    for (int i = 0; i < G->V; i++) {
        fprintf(fout, "Nodo di partenza: %s\n", STgetName(G->st, i));
        for (x = G->adj[i].head; x != NULL; x = x->next)
            fprintf(fout,"\t- %s\n",STgetName(G->st, x->index));
    }
}

int GRAPHcheckDAG(graph_t G, int *mark, int *pre, int *post) {
	int v, time = 0, value = 1;
	edge_t edge;
	
	for (v = 0; v < G->V; v++) {
		pre[v] = -1; post[v] = -1;
	}
    /* si prova a rimuovere gli archi marcati */
	GRAPHremoveEdges(G,mark);

	for (v = 0; v < G->V && value; v++) {
		if (pre[v]==-1) {
			edge.v = v; edge.w = v;
            /* (guardando funzione successiva: value = 1 se il grafo è un DAG, altrimenti value = 0 */
			value = dfsR(G, edge, &time, pre, post);
		}
	}

    /* si riaggiungono gli archi rimossi precedentemente */
	GRAPHaddEdges(G,mark);

	return value;
}

/* funzione che restituisce 0 se si ha un ciclo, altrimenti 1 */
static int dfsR(graph_t G, edge_t edge, int *time, int *pre, int *post){
	link x;
	int v, w = edge.w;
	edge_t y;
	pre[w] = (*time)++;
	for (x = G->adj[w].head; x != NULL; x = x->next) {
		if (pre[x->index] == -1) {
            y.v = w;
            y.w = x->index;
			if(!dfsR(G, y, time, pre, post))
				return 0;
		}
		else {
			v = x->index;
			if (post[v] == -1) {
                /* si ritorna su un vertice non ancora completato => ha un ciclo */
				return 0;
			}
		}
	}
	post[w] = (*time)++;
	return 1;
}

int GRAPHsumWeight(graph_t G, int *mark) {
	int sum = 0;
	for(int i = 0; i < G->E; i++)
		if(mark[i]) sum += G->edges[i].wt;
	return sum;
}

void GRAPHaddEdges(graph_t G, int *mark) {
	link x;
	for (int i = 0; i < G->E; i++) {
		if (mark[i]) {
            x = malloc(sizeof(struct node));
            x->wt = G->edges[i].wt;
            x->index = G->edges[i].w;
            /* inserimento in testa */
            x->next = G->adj[G->edges[i].v].head;
            G->adj[G->edges[i].v].head = x;
		}
	}
}

void GRAPHremoveEdges(graph_t G, int *mark) {
	link x, tmp;
	for(int i = 0; i < G->E; i++) {
		if(mark[i]) {
			for (x = G->adj[G->edges[i].v].head, tmp = NULL; x != NULL; tmp = x, x = x->next) {
				if(G->edges[i].w == x->index) {
					if(x == G->adj[G->edges[i].v].head)
                        G->adj[G->edges[i].v].head = x->next;
					else
                        tmp->next = x->next;

					free(x);
					break;
				}
			}
		}
	}
}

void GRAPHmaximumPath(graph_t G, FILE *fout) {
	int v, w, time = 0, *pre, *ts, *paths;
	link t;

	pre = malloc(G->V * sizeof(int));
    ts = malloc(G->V * sizeof(int));
    paths = malloc(G->V * sizeof(int));

	for (v = 0; v < G->V; v++) {
        pre[v] = -1;
        ts[v] = -1;
        paths[v] = 0;
    }

	for (v = 0; v < G->V; v++)
		if (pre[v]== -1)
            TSdfsR(G, v, ts, pre, &time);

	for (v = G->V - 1; v >= 0; v--) {
		for (t = G->adj[ts[v]].head; t != NULL; t = t->next) {
			if (paths[ts[v]] + 1 > paths[t->index])
                paths[t->index] = paths[ts[v]] + 1;
		}
	}

	fprintf(fout, "\nCammini massimi a partire dalle sorgenti:\n");
	for (v = 0; v < G->V; v++) {
        if (paths[ts[v]] == 0) {
            fprintf(fout,"\t- sorgente: %s\n", STgetName(G->st, ts[v]));
            for (w = G->V - 1; w >= 0; w--) {
                if (paths[ts[w]] != 0)
                    fprintf(fout,"\t\t- %s, %3d\n", STgetName(G->st, ts[w]), paths[ts[w]]);
            }
        }
	}
	
	free(pre);
	free(ts);
	free(paths);
}

/* funzione tratta da lucidi */
static void TSdfsR(graph_t G, int v, int *ts, int *pre, int *time) {
	link t;
    pre[v] = 0;
	for (t = G->adj[v].head; t != NULL; t = t->next)
		if (pre[t->index] == -1)
			TSdfsR(G, t->index, ts, pre, time);
	ts[(*time)++] = v;
}

void EDGEstore(graph_t G, int index, FILE *fout) {
	fprintf(fout,"\t%s-%s : %2d\t",
           STgetName(G->st, G->edges[index].v),
           STgetName(G->st, G->edges[index].w),
           G->edges[index].wt);
}

int GRAPHgetEdges(graph_t G) {
	return G->E;
}
int GRAPHgetVertex(graph_t G) {
	return G->V;
}