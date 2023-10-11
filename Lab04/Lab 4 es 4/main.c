#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N_PIETRE 4

/** struct che memorizza per ogni pietra il valore, la quantità e le ripetizioni consecutive già inserite */
typedef struct {
    int valore, quantita, ripetizione;
} pietra;

char *newValue(char c);
int newCount(char c);
void collana(int pos, char *val, char *sol, char *solFin, pietra *pietre, int n, int k, int tot_v,
             int *pmax, int *pmaxFin, int *fine, int j, int max_rip, int z, int s);

int main() {
    FILE *infile;
    int prove, max, maxFin, fine, max_rip, tot_q, tot_v, z, s;
    char *sol, *solFin, *val;

    infile = fopen("../test_set.txt", "r");
    fscanf(infile, "%d", &prove);
    for (int p = 0; p < prove; p++) {
        max = 0, maxFin = 0, fine = 0;

        pietra *pietre = (pietra *) malloc(N_PIETRE*sizeof(pietra));

        for (int q = 0; q < N_PIETRE; q++) {
            fscanf(infile, "%d", &pietre[q].quantita);
            pietre[q].ripetizione = 0;
        }
        for (int v = 0; v < N_PIETRE; v++) {
            fscanf(infile, "%d", &pietre[v].valore);
        }
        fscanf(infile, "%d", &max_rip);

        /**
        * come per l'es 3, si nota che il numero di topazi e rubini inseribili nella collana dipendono l'uno dall'altro:
        * infatti, al fine di massimizzare il loro numero, vanno inseriti alternati
        * di conseguenza, partendo da r rubini e t topazi, se il t < r possono inserirsi solamente t topazi e t+1 rubini,
        * se invece r < t possono inserirsi r rubini e r+1 topazi, inoltre converrà inserire come prima pietra un topazio
        *
        * è inoltre richiesto dall'esercizio che i zaffiri siano al più tanti quanti gli smeraldi,
        * perciò conviene diminuire il loro numero se necessario
         *
         * all'interno della funzione
        */

        val = "rtsz";
        if (pietre[3].quantita < pietre[0].quantita)
            pietre[0].quantita = pietre[3].quantita;
        z = pietre[0].quantita, s = pietre[3].quantita;
        if (pietre[1].quantita < pietre[2].quantita) {
            val = "tszr";
            pietre[2].quantita = pietre[1].quantita+1;
        }
        else if (pietre[2].quantita < pietre[1].quantita)
            pietre[1].quantita = pietre[2].quantita+1;

        tot_q = pietre[0].quantita + pietre[1].quantita+ pietre[2].quantita + pietre[3].quantita;
        tot_v = pietre[0].valore*pietre[0].quantita+pietre[1].valore*pietre[1].quantita+
                pietre[2].valore*pietre[2].quantita+pietre[3].valore*pietre[3].quantita;

        sol = malloc((tot_q+1)*sizeof(char)), solFin = (malloc(((tot_q + 1)) * sizeof(char)));


        sol[tot_q] = '\0', solFin[tot_q] = '\0';

        collana(0, val, sol, solFin, pietre, N_PIETRE, tot_q, tot_v, &max, &maxFin, &fine, 0, max_rip, z, s);
        printf("\n%3d    . ", maxFin);
        for (int i = 0; i < strlen(solFin); i++) {
            printf("%c ", solFin[i]);
        }

        free(sol), free(solFin); free(pietre);
    }
    fclose(infile);

    return 0;
}

char *newValue(char c) {
    switch (c) {
        case 'z':
            return "zr";
        case 'r':
            return "ts";
        case 't':
            return "zr";
        case 's':
            return "ts";
    }
}

int newCount(char c) {
    switch (c) {
        case 'z':
            return 0;
        case 'r':
            return 1;
        case 't':
            return 2;
        case 's':
            return 3;
    }
}

void collana(int pos, char *val, char *sol, char *solFin, pietra *pietre, int n, int k, int tot_v,
             int *pmax, int *pmaxFin, int *fine, int j, int max_rip, int z, int s) {
    if (pos >= k) {
        if (strcmp(solFin, sol) != 0) {
            *pmaxFin = *pmax;
            strcpy(solFin, sol);
        }
        return ;
    }

    for (int i = 0, c, rip; i < n && !(*fine); i++) {
        /** modello che richiama le disposizioni con ripetizione
         * aggiunta di funzione che varia i possibili val successivi in base alla pietra precedentemente inserita
         * si tiene conto di quante pietre di un certo tipo siano ancora disponibili
         */
        c = newCount(val[i]);
        if (pietre[c].ripetizione < max_rip) {
            if (pietre[c].quantita > 0) {

                rip = pietre[j].ripetizione;
                if (j != c)
                    pietre[j].ripetizione = 0;

                sol[pos] = val[i];
                pietre[c].quantita--; pietre[c].ripetizione++;
                *pmax += pietre[c].valore;
                collana(pos+1, newValue(val[i]), sol, solFin, pietre, 2, k, tot_v, pmax, pmaxFin, fine, c, max_rip, z, s);

                if (*pmax == tot_v) {
                    *fine = 1;
                }
                else {
                    *pmax -= pietre[c].valore;
                }

                pietre[c].quantita++; pietre[c].ripetizione--;
                if (j != c)
                    pietre[j].ripetizione = rip;
            }
            else {
                /** si considerano anche i casi che non raggiungono valore massimo
                 * (notare la ricorsione con k=pos per entrare direttamente nell'if)*/
                if (*pmax > *pmaxFin && s-pietre[3].quantita >= z-pietre[0].quantita) {
                    sol[pos] = '\0';
                    collana(pos, val, sol, solFin, pietre, n, pos, tot_v, pmax, pmaxFin, fine, c, max_rip, z, s);
                }
            }
        }
    }
}
