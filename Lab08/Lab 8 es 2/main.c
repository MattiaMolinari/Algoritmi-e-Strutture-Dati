#include <stdio.h>

#include "graph.h"

int main() {
    FILE *infile = fopen("../grafo.txt", "r");
    Graph G; List L;
    int cmd = 1, freeList = 0;

    G = GRAPHload(infile);

    while (cmd != 0) {
        fprintf(stdout, "Seleziona comando:\n\t"
                        "0. Fine (default)\n\t"
                        "1. Stampa\n\t"
                        "2. Verifica adiacenza\n\t"
                        "3. Lista adiacenze\n");
        scanf("%d", &cmd);
        switch (cmd) {
            case 1:
                GRAPHstore(G, stdout);
                break;
            case 2:
                if (GRAPHsubComplete(G, L)) {
                    printf("Sottografo completo verificato\n");
                }
                else { printf("Sottografo non completo\n"); }
                break;
            case 3:
                L = LISTinit(G);
                freeList = 1;
                LISTstore(G, L);
                break;
            default:
                cmd = 0;

        }
    }

    if (freeList) LISTfree(L);
    GRAPHfree(G);

    return 0;
}
