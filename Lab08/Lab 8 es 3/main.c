#include <stdio.h>

#include "titolo.h"

#define F1 "../F1.txt"
#define F2 "../F2.txt"
#define F3 "../F3.txt"

int main() {
    FILE *fin;
    ttlList_t L = LISTinit();
    char input = '1', codice[MAXC];
    int f, data1, data2, y, m, d;
    ttl_t titolo;

    while (input != '0') {
        fprintf(stdout, "SELEZIONARE COMANDO\n"
                "\t 0. termina programma (default) \n"
                "\t 1. acquisizione da file\n"
                "\t 2. ricerca titolo azionario\n"
                "\t 3. ricerca della quotazione giornaliera di un titolo in una data\n"
                "\t 4. ricerca della quotazione massima e minima di un titolo in un intervallo di date\n"
                "\t 5. ricerca della quotazione massima e minima di un titolo su tutte le date registrate\n"
                "\t 6. bilanciamento albero quotazioni se il rapporto tra il cammino più lungo e più corto supera una certa soglia S.\n");
        fflush(stdin); input = getc(stdin);
        switch (input) {
            case '1':   /* acquisizione da file */
                fprintf(stdout, "Selezionare file: F1 (default) , F2 o F3 (inserire numero) >");
                fscanf(stdin, "%d", &f);
                switch (f) {
                    case 2:
                        fin = fopen(F2, "r");
                        break;
                    case 3:
                        fin = fopen(F3, "r");
                        break;
                    default:
                        fin = fopen(F1, "r");
                }
                LISTload(fin, L);
                fclose(fin);
                LISTstore(stdout, L);
                break;

            case '2':   /* ricerca titolo */
                fprintf(stdout, "Inserire codice >");
                fscanf(stdin, "%s", codice);
                titolo = LISTsearch(L, codice);
                if (titolo!= NULL)
                    fprintf(stdout, "Trovato\n");
                else
                    fprintf(stdout, "NON trovato\n");
                break;

            case '3':   /* quotazione giornaliera in data */
                fprintf(stdout, "Inserire codice >");
                fscanf(stdin, "%s", codice);
                titolo = LISTsearch(L, codice);
                if (titolo == NULL) { fprintf(stdout, "Titolo NON trovato\n"); break; }
                fprintf(stdout, "Inserire data (formato yyyy/mm/dd) >");
                fscanf(stdin, "%d/%d/%d", &y, &m, &d); data1 = DATAconvert(y, m, d);
                TTLsearchQ(stdout, titolo, data1);
                break;

            case '4':   /* quotazione max e min in intervallo date */
                fprintf(stdout, "Inserire codice >");
                fscanf(stdin, "%s", codice);
                titolo = LISTsearch(L, codice);
                if (titolo == NULL) { fprintf(stdout, "Titolo NON trovato\n"); break; }
                fprintf(stdout, "Inserire prima data (formato yyyy/mm/dd) >");
                fscanf(stdin, "%d/%d/%d", &y, &m, &d); data1 = DATAconvert(y, m, d);
                fprintf(stdout, "Inserire seconda data (formato yyyy/mm/dd) >");
                fscanf(stdin, "%d/%d/%d", &y, &m, &d); data2 = DATAconvert(y, m, d);
                if (data1 > data2) {
                    f = data1;
                    data1 = data2;
                    data2 = f;
                }
                TTLsearchMaxMin(stdout, titolo, data1, data2);
                break;

            case '5':   /* qotazione max e min su tutto */
                fprintf(stdout, "Inserire codice >");
                fscanf(stdin, "%s", codice);
                titolo = LISTsearch(L, codice);
                if (titolo == NULL) { fprintf(stdout, "Titolo NON trovato\n"); break ;}
                data1 = 0; data2 = DATAconvert(9999, 12, 31);
                TTLsearchMaxMin(stdout, titolo, data1, data2);
                break;

            case '6':   /* bilanciamento */
                fprintf(stdout, "Inserire codice >");
                fscanf(stdin, "%s", codice);
                titolo = LISTsearch(L, codice);
                if (titolo == NULL) { fprintf(stdout, "Titolo NON trovato\n"); break ;}
                fprintf(stdout, "Inerire soglia >");
                fscanf(stdin, "%d", &f);
                if (f < TTLrapport(titolo))
                    TTLbalance(titolo);
                break;
            default: input = '0';
        }
    }

    LISTfree(L);

    return 0;
}
