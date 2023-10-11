#ifndef LAB_8_ES_2_GRAPH_H
#define LAB_8_ES_2_GRAPH_H

#include <stdio.h>

#include "st.h"

typedef struct edge {
    int v, w, wt;
} Edge;
typedef struct graph *Graph;
typedef struct list *List;

Graph GRAPHinit(int V);
void GRAPHfree(Graph G);
Graph GRAPHload(FILE *fin);
void GRAPHstore(Graph G, FILE *fout);
int GRAPHgetIndex(Graph G, Item label);
void GRAPHedges(Graph G, Edge *a);
List LISTinit(Graph G);
void LISTfree(List L);
void LISTstore(Graph G, List L);
int GRAPHsubComplete(Graph G, List L);



#endif //LAB_8_ES_2_GRAPH_H
