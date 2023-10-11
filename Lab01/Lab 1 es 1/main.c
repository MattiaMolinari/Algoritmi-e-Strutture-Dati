#include <stdio.h>
#include <string.h>
#include <ctype.h>

char *cercaRegexp(char *src, char *regexp);

int main() {
    char fonte[30] = "KfjdafotomotorotocotoA", cerca[21] = "[dcf]oto.ot\\o[^c]";  //esempio
    printf("%s", cercaRegexp(fonte, cerca));
    return 0;
}

char *cercaRegexp(char *src, char *regexp) {
    char *occ = NULL;
    int lenRegexp = 0, lenSrc = strlen(src), lenReal = strlen(regexp);

    // si calcola effettiva lunghezza regexp
    for (int i = 0; regexp[i] != '\0'; i++){
        switch (regexp[i]) {
            case '\\':
                break;
            case '[':
                for (i = i; regexp[i] != ']'; i++);
                lenRegexp++;
                break;
            default:
                lenRegexp++;
        }
    }

    /**
     * pos è l'indice su src da cui dovrebbe partire l'occorrenza della regular expression
     * cnt incrementa tale indice per controllare anche le lettere successive
     * l è l'indice su regexp
     * err è 0 finché il confronto avviene con successo
     *
     * se in pos e in tutti i cnt successi fino ad esaurire la lunghezza effettiva della regular expression cercata non si trovano errori, si ha successo
     * => err = 0 alla fine dei cicli, l'occorrenza è stata trovata a partire da src[pos]
     */
    for (int pos = 0, err; pos < lenSrc - lenRegexp && occ == NULL; pos++){
        err = 0;
        for (int cnt = 0, l = 0, neg = 0; !err && cnt < lenRegexp && l < lenReal; cnt++){
            switch (regexp[l]) {
                case '.':
                    break;

                case '[':
                    if (regexp[l+1] == '^') {
                        l++;
                        neg = 1;
                    }
                    else {
                        err = 1;
                    }

                    for (l = l+1; regexp[l] != ']'; l++){
                        if (neg && regexp[l] == src[pos+cnt]){
                            err = 1;
                        }
                        else if (!neg && regexp[l] == src[pos+cnt]){
                            err = 0;
                        }
                    }
                    break;

                case '\\':
                    l++;
                    if (islower(regexp[l]) != islower(src[pos+cnt])){
                        err = 1;
                    }
                    break;

                default:
                    if (regexp[l] != src[pos+cnt]){
                        err = 1;
                    }
            }
            l++;
        }
        if (!err) {
            occ = &src[pos];
        }
    }

    return occ;
}

