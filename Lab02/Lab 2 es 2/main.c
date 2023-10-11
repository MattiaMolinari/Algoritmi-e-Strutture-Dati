#include <stdio.h>
#include <stdlib.h>
#define INFILE "../mat.txt"

int **malloc2dR(FILE *infile, int nr, int nc);
void separa(int **mat, int nr, int nc, int **bianco, int **nero, int *w, int *b);

int main() {
    FILE *infile;
    int nr, nc, **mat, *bianco, *nero, w, b;

    infile = fopen(INFILE, "r");
    fscanf(infile, "%d %d", &nr, &nc);
    mat = malloc2dR(infile, nr, nc);
    fclose(infile);

    separa(mat, nr, nc, &bianco, &nero, &w, &b);
    printf("Bianco: ");
    for (int x = 0; x < w; x++) {
        printf("%d ", bianco[x]);
    }
    printf("\nNero: ");
    for (int x = 0; x < b; x++) {
        printf("%d ", nero[x]);
    }
    free(bianco); free(nero);

    for (int i = 0; i < nr; i++) {
        free(mat[i]);
    }
    free(mat);

    return 0;
}

int **malloc2dR(FILE *infile, int nr, int nc) {
    int **mat = (int **)malloc(sizeof(int *) * nr);
    for (int i = 0; i < nr; i++) {
        mat[i] = (int *)malloc(sizeof(int) * nc);
        for (int j = 0; j < nc; j++) {
            fscanf(infile, "%d", &mat[i][j]);
        }
    }

    return mat;
}

void separa(int **mat, int nr, int nc, int **bianco, int **nero, int *w, int *b) {
    int n = nr*nc/2 + 1;
    *bianco = (int *)malloc(sizeof(int) * n), *nero = (int *)malloc(sizeof(int) * n);
    *w = 0, *b = 0;

    for (int i = 0; i < nr; i++) {
        for (int j = 0; j < nc; j++) {
            if ((i+j) % 2 == 0) {
                (*bianco)[*w] = mat[i][j];
                (*w)++;
            }
            else {
                (*nero)[*b] = mat[i][j];
                (*b)++;
            }
        }
    }
}
