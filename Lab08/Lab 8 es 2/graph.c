#include <stdio.h>
#include <stdlib.h>

#include "st.h"
#include "graph.h"

struct graph {
    int V;
    int E;
    int **madj ;
    ST tab ;
} ;

typedef struct vertex *link;
struct vertex {
    int adj, wt;
    link next;
};

struct list {
    link *heads;
    int V;
};

// creazione matrice adiacenze
static int **MATRIXint(int r, int c, int val) {
    int i, j;
    int **t = malloc(r * sizeof(int *));
    for (i=0; i < r; i++) t[i] = malloc(c * sizeof(int));
    for (i=0; i < r; i++)
        for (j=0; j < c; j++)
            t[i][j] = val;
    return t;
}

// inserimento e cancellazione in/da matrice adiacenze
static void insertE(Graph G, int v, int w, int wt) {
    if (G->madj[v][w] == 0)	G->E++;
    G->madj[v][w] = wt;
    G->madj[w][v] = wt;
}
static void removeE(Graph G, int v, int w) {
    if (G->madj[v][w] != 0) G->E--;
    G->madj[v][w] = 0;
    G->madj[w][v] = 0;
}

// creazione e distruzione grafo
Graph GRAPHinit(int V) {
    Graph G = (Graph) malloc(sizeof *G);
    G->V = V; G->E = 0;
    G->madj = MATRIXint(V, V, 0);
    G->tab = STinit(V);
    return G;
}
void GRAPHfree(Graph G) {
    for (int i=0; i<G->V; i++)
        free(G->madj[i]);
    free(G->madj);
    STfree(G->tab);
    free(G);
}


// restituisce l'indice di un vertice nella tabella di simboli
int GRAPHgetIndex(Graph G, Item label) {
    int id = STsearch(G->tab, label.nome);
    if (id == -1) {
        id = STcount(G->tab);
        STinsert(G->tab, label);
    }
    return id;
}

// lettura e scrittura grafo da/su file
Graph GRAPHload(FILE *fin) {
    int V, wt, cnt = 0;
    Item label1, label2;
    Graph G;

    for (V = 0; fscanf(fin, "%*s %*s %*s %*s %*d")!= EOF; V++);
    rewind(fin);

    G = GRAPHinit(2*V);

    while(fscanf(fin,"%s %s %s %s %d", label1.nome, label1.rete,  label2.nome, label2.rete, &wt) == 5) {
        if (STsearch(G->tab, label1.nome) == -1) {
            STinsert(G->tab, label1); cnt++;
        }
        if (STsearch(G->tab, label2.nome) == -1) {
            STinsert(G->tab, label2); cnt++;
        }
    }
    rewind(fin);

    while(fscanf(fin,"%s %s %s %s %d", label1.nome, label1.rete,  label2.nome, label2.rete, &wt) == 5)
        insertE(G, STsearch(G->tab, label1.nome), STsearch(G->tab, label2.nome), wt);

    G->V = cnt;
    return G;
}
void GRAPHstore(Graph G, FILE *fout) {
    int i;

    for (i = 0; i < G->V; i++) {
        if (STsearchByIndex(G->tab, i).nome[0] != '\0')
            fprintf(fout, "%s\n", STsearchByIndex(G->tab, i).nome);
        for (int v = 0; v < G->V; v++)
            if (G->madj[i][v] != 0)
                printf("\t- %s : %d\n", STsearchByIndex(G->tab, v).nome, G->madj[i][v]);
    }
}

static link NEW(int j, int wt) {
    link x = malloc(sizeof(*x));
    x->next = NULL; x->adj = j; x->wt = wt;
    return x;
}

List LISTinit(Graph G) {
    int i, j; link *h;
    List L = malloc(sizeof *L);
    L->heads = malloc(G->V * sizeof(link));
    L->V = G->V;

    for (i = 0; i < G->V; i++) {
        h = &L->heads[i];
        *h = NULL;
        for (j = 0; j < G->V; j++) {
            if (G->madj[i][j] != 0) {
                (*h) = NEW(j, G->madj[i][j]);
                h = &(*h)->next;
            }
        }
    }
    return L;
}

void LISTfree(List L) {
    int i; link t;
    for (i = 0; i < L->V; i++) {
        while (L->heads[i] != NULL && L->heads[i]->next != NULL) {
            t = L->heads[i]->next;
            L->heads[i]->next = t->next;
            free(t);
        }
        free(L->heads);
    }
}

void LISTstore(Graph G, List L) {
    link h;
    for (int i = 0; i < L->V; i++) {
        printf("%s\n", STsearchByIndex(G->tab, i).nome);
        h = L->heads[i];
        while (h!=NULL) {
            printf("\t%s : %d\n", STsearchByIndex(G->tab, h->adj).nome , h->wt);
            h=h->next;
        }
    }
}

int GRAPHsubComplete(Graph G, List L) {
    link h; int s = 1, id1, id2, id3; char v1[MAXC+1], v2[MAXC+1], v3[MAXC+1];
    printf("Inserire nomi vertici >");
    scanf("%s %s %s", v1, v2, v3);
    id1 = STsearch(G->tab, v1); id2 = STsearch(G->tab, v2); id3 = STsearch(G->tab, v3);
    printf("Matrice adiacenze [1] (default) o lista adiacenze [0]");
    scanf("%d", &s);

    if (!s) {
        h = L->heads[id1];
        while (h != NULL) {
            if (h->adj == id2) s++;
            if (h->adj == id3) s++;
            h = h->next;
        }
        h = L->heads[id2];
        while (h != NULL) {
            if (h->adj == id3) s++;
            h = h->next;
        }
        if (s != 3) s = 0;
    }
    else {
        s = (G->madj[id1][id2] != 0 && G->madj[id1][id3] != 0 && G->madj[id3][id2] != 0);
    }

    return s;

}






