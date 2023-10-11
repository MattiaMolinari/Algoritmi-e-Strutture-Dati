#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N_PIETRE 4

char *newValue(char c);
int newCount(char c);
int collana(int pos, char *val, char *sol, char *solFin, int *count, int n, int k, int max, int maxFin, int *fine);

int main() {
    FILE *infile, *outfile;
    /** vettore count che memorizza il numero di pietre disponibili per ciascun tipo, massimo da calcolare (almeno uguale a uno),
     * dal momento che si vuole solo calcolare il massimo e mostrare una possibile composizione di tale lunghezza,
     * nel caso in cui si raggiunga come massimo il numero totale di pietre prima di aver calcolato tutte le possibili disposizioni
     * si può arrestare la funzione utilizzando fine */
    char *sol, *solFin, *val,
    *nomiFile[4] = {"../very_very_easy_test_set.txt", "../very_easy_test_set.txt","../easy_test_set.txt",
                    "../hard_test_set.txt"};
    int prove, *count, max, maxFin, fine, z, r, t, s, tot;

    outfile = fopen("../risultati.txt", "w");
     for (int f = 0; f < 4; f++) {
         infile = fopen(nomiFile[f], "r");

         fprintf(outfile, "FILE %s\n", &nomiFile[f][3]);

         fscanf(infile, "%d", &prove);

         for (int p = 0; p < prove; p++) {
             fprintf(outfile,"TEST #%d\n", p+1);
             max = 1, maxFin = 0, fine = 0;

             fscanf(infile, "%d %d %d %d", &z, &r, &t, &s);
             tot = z + r + s + t; // numero totale di pietre
             fprintf(outfile, " zaffiri: %2d, rubini: %2d, topazi: %2d, smeraldi: %2d, TOT: %2d\n", z, r, t, s, tot);

             /**
            * si nota che il numero di topazi e rubini inseribili nella collana dipendono l'uno dall'altro:
            * infatti, al fine di massimizzare il loro numero, vanno inseriti alternati
            * di conseguenza, partendo da r rubini e t topazi, se il t < r possono inserirsi solamente t topazi e t+1 rubini,
            * se invece r < t possono inserirsi r rubini e r+1 topazi, inoltre converrà inserire come prima pietra un topazio
            *
            * tutto ciò permette di conoscere anticipatamente il numero massimo di pietre inseribili, anche se minore di tot,
            * e trovare il prima possibile una soluzione ottimale
            */
             val = "zrst";
             if (r < t) {
                 t = r+1;
                val = "tzrs";
             }
             else if (r > t) {
                 r = t+1;
             }


             sol = (char *) malloc((z+r+s+t+1)*sizeof(char));
             solFin = (char *) malloc((z+r+s+t+1)*sizeof(char));
             sol[z+r+s+t] = '\0', solFin[z + r + t + s] = '\0', solFin[0] = '\0';
             count = (int *)malloc(N_PIETRE*sizeof(int));
             count[0] = z, count[1] = r, count[2] = t, count[3] = s;

             max = collana(0, val, sol, solFin, count, N_PIETRE, z + r + t + s, max, maxFin, &fine);

             fprintf(outfile, " Collana di massima lunghezza %2d\n  ", max);
             for (int i = 0; i < max; i++) {
                 fprintf(outfile, "%c ", solFin[i]);
             }
             fprintf(outfile,"\n");

             free(sol), free(solFin); free(count);
         }
         fprintf(outfile,"\n\n");

         fclose(infile);
     }
    fclose(outfile);

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
        default: return "zrts";
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

int collana(int pos, char *val, char *sol, char *solFin, int *count, int n, int k, int max, int maxFin, int *fine) {
    if (pos >= k) {
        if (strcmp(solFin, sol) != 0) {
            strcpy(solFin, sol);
        }
        return pos;
    }

    for (int i = 0, c; i < n && !(*fine); i++) {
        /** modello che richiama le disposizioni con ripetizione
         * aggiunta di funzione che varia i possibili val successivi in base alla pietra precedentemente inserita
         * si tiene conto di quante pietre di un certo tipo siano ancora disponibili
         */
        c = newCount(val[i]);
        if (count[c] > 0) {
            sol[pos] = val[i];
            count[c]--;
            max = collana(pos+1, newValue(val[i]), sol, solFin, count, 2, k, max, maxFin, fine);
            if (max == k) {
                *fine = 1;
            }
            count[c]++;
        }
        else {
            /** si considerano anche i casi che non raggiungono valore massimo
             * (notare la ricorsione con k=pos per entrare direttamente nell'if)*/
            if (pos > max) {
                sol[pos] = '\0';
                max = collana(pos, val, sol, solFin, count, n, pos, max, maxFin, fine);
            }
        }
    }
    return max;
}

