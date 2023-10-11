#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 255
#define INFILE "../brani.txt"


typedef struct {
    char **scelte;
    int num_scelte;
} friend;

/** applicazione del principio di moltiplicazione */
int princ_molt(int pos, friend *val, char **sol, int k, int count);

int main() {
    FILE *infile;
    friend *canzoni;
    char brano[MAXLEN], **playlist;
    int amici, amico;

    infile = fopen(INFILE, "r");

    fscanf(infile, "%d", &amici);
    canzoni = (friend *)malloc(amici*sizeof(friend));
    playlist = (char **)malloc(amici*sizeof(char *));

    for (amico = 0; amico < amici && fscanf(infile, "%d", &canzoni[amico].num_scelte); amico++)  {
        canzoni[amico].scelte = (char **)malloc(canzoni[amico].num_scelte*sizeof(char *));
        for (int canzone = 0; canzone < canzoni[amico].num_scelte; canzone++){
            fscanf(infile, "%s", brano);
            canzoni[amico].scelte[canzone] = (char *)malloc((strlen(brano)+1)*sizeof(char));
            strcpy(canzoni[amico].scelte[canzone], brano);
        }
    }
    fclose(infile);

    princ_molt(0, canzoni, playlist, amici, 0);

    for (amico = 0; amico < amici; amico++) {
        for (int canzone = 0; canzone < canzoni[amico].num_scelte; canzone++) {
            free(canzoni[amico].scelte[canzone]);
        }
        free(canzoni[amico].scelte);
    }
    free(canzoni); free(playlist);

    return 0;
}

int princ_molt(int pos, friend *val, char **sol, int k, int count) {
    int i;
    if (pos >= k) {
        for (i = 0; i < k; i++)
            printf("%s ", sol[i]);
        printf("\n");
        return count+1;
    }
    for (i = 0; i < val[pos].num_scelte; i++) {
        sol[pos] = val[pos].scelte[i];
        count = princ_molt(pos+1, val, sol, k, count);
    }
    return count;
}

