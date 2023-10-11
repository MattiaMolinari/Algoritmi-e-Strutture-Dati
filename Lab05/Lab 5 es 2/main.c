#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char col_o, col_v;
    int val_o, val_v;
}tail;

void solve(tail ***board, tail *tails, int n, int r, int c, int *mark);
int disp(tail ***board, tail *tails, int pos, int n, int k, int r, int c, int *pmax, int *mark, tail **sol, int cnt);
int value(tail ***board, int r, int c);
void rotate(tail *tails, int t);


int main() {
    FILE *infile;
    tail *tails, ***board;
    int n, r, c, t, rot, *mark;


    infile = fopen("../tiles.txt", "r");
    fscanf(infile, "%d\n", &n);
    tails = (tail *) malloc(n*sizeof(tail));
    mark = (int *) calloc(n, sizeof(int));
    for (int i = 0; i < n; i++) {
        fscanf(infile, "%c %d %c %d\n", &tails[i].col_o, &tails[i].val_o, &tails[i].col_v, &tails[i].val_v);
    }
    fclose(infile);
    infile = fopen("../board.txt", "r");
    fscanf(infile, "%d %d", &r, &c);
    board = (tail ***) malloc(r*sizeof(tail **));
    for (int i = 0; i < r; i++) {
        board[i] = (tail **) malloc(c * sizeof(tail *));
        for (int j = 0; j < c; j++) {
            fscanf(infile, "%d/%d", &t, &rot);
            if (t != -1) {
                mark[t] = 1;
                board[i][j] = &tails[t];
            }
            else
                board[i][j] = NULL;
            if (rot == 1)
                rotate(tails, t);
        }
    }
    fclose(infile);

    solve(board, tails, n, r, c, mark);

    for (int i = 0; i < r; i++) {
        free(board[i]);
    }
    free(board), free(tails), free(mark);


    return 0;
}
/** funzione che inizializza gli elementi necessari e chiama la funzione che risolve il problema, per poi stampare la soluzione */
void solve(tail ***board, tail *tails, int n, int r, int c, int *mark) {
    int pos = 0, k = r*c, max = 0, cnt = 0;
    tail **sol = (tail **) malloc(r*sizeof(tail *));
    for (int i = 0; i < c; i++)
        sol[i] = (tail *) malloc(c*sizeof(tail));

    printf("Totale disposizioni testate: %d\n", disp(board, tails, pos, n, k, r, c, &max, mark, sol, cnt));

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++)
            printf(" %c\t|\t", sol[i][j].col_v);
        printf("\n");
        for (int j = 0; j < c; j++)
            printf("%c %d\t|\t", sol[i][j].col_o, sol[i][j].val_o);
        printf("\n");
        for (int j = 0; j < c; j++)
            printf(" %d\t|\t", sol[i][j].val_v);
        printf("\n");
        for (int j = 0; j < c; j++)
            printf("-------");
        printf("\n");
        free(sol[i]);
    }
    free(sol);
    printf("Valore totale %d\n", max);
}

/** si usa il modello delle disposizioni semplici
 * @param board la tabella da completare
 * @param tails l'elenco di tessere totali
 * @param pos la posizione nella tabella (calcolata poi come indice di riga pos/c e indice di colonna pos%c)
 * @param n il numero totale di tessere
 * @param k il numero di schede massimo nella tabella
 * @param r il numero di righe
 * @param c il numero di colonne
 * @param pmax il valore massimo di una configurazione
 * @param mark il vettore che controlla se una tessera è già stata utilizzata
 * @param sol la soluzione a valore maggiore
 * il numero di schede effettivamente inserite dalla funzione è k <= n,
 * quindi le disposizioni trovate saranno al più 2^k * n!/(n-k)!
 * 2^k è dovuto dal fatto che ognuna delle k schede può essere inserita in due modi distinti
 */
int disp(tail ***board, tail *tails, int pos, int n, int k, int r, int c, int *pmax, int *mark, tail **sol, int cnt) {
    int max;
    if (pos >= k) {
        max = value(board, r, c);
        if (max > *pmax) {
            *pmax = max;
            for (int i = 0; i < r; i++)
                for (int j = 0; j < c; j++)
                    sol[i][j] = *board[i][j];
        }
        return cnt+1;
    }

    // si evita di modificare una tessere già inserita da problema
    if (board[pos/c][pos%c] == NULL) {
        for (int i = 0; i < n; i++)
            if (mark[i] == 0) {
                mark[i] = 1;
                board[pos/c][pos%c] = &tails[i];
                cnt = disp(board, tails, pos + 1, n, k, r, c, pmax, mark, sol,cnt);
                rotate(tails, i);
                cnt = disp(board, tails, pos + 1, n, k, r, c, pmax, mark, sol, cnt);
                board[pos/c][pos%c] = NULL;
                mark[i] = 0;
            }
    }
    else {
        cnt = disp(board, tails, pos + 1, n, k, r, c, pmax, mark, sol, cnt);
    }
    return cnt;
}

/** funzione che calcola il valore della tabella */
int value(tail ***board, int r, int c) {
    int value = 0, value_v, value_o;
    for (int i = 0, err_o, err_v; i < r; i++) {
        value_o = 0, value_v = 0, err_o = 0, err_v = 0;
        for (int j = 0; j < c; j++) {
            if (board[i][j]->col_o != board[i][0]->col_o)
                err_o = 1;
            if (board[j][i]->col_v != board[0][i]->col_v)
                err_v = 1;
            value_o += board[i][j]->val_o; value_v += board[j][i]->val_v;
        }
        if (!err_o)
            value += value_o;
        if (!err_v)
            value += value_v;
    }
    return value;
}

/** funzione che ruota una tessera */
void rotate(tail *tails, int t) {
    int val; char col;
    col = tails[t].col_o, val = tails[t].val_o;
    tails[t].col_o = tails[t].col_v, tails[t].val_o = tails[t].val_v;
    tails[t].col_v = col, tails[t].val_v = val;
}
