#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OUTFILE "../output.txt"
#define MAXLEN 50
#define DATALEN 10

typedef struct {
    char codice[MAXLEN+1], nome[MAXLEN+1], cognome[MAXLEN+1], data_s[DATALEN+1], via[MAXLEN+1], citta[MAXLEN+1];
    int data_i, cap;
} Item;

typedef struct node *link;

struct node {
    Item val;
    link next;
    link pred;
};

// le funzioni insertNode(_.) restituiscono la nuova head
link newNode(FILE *infile);
link newNode_v(link head);  // legge elemento da tastiera e lo inserisce
link newNode_f(link head);  // legge elementi da file e li inserisce
link insertNode(link head, link x); // inserisce nuovo elemento in maniera ordinata
// le funzioni di eliminazione e ricerca
link ricerca(link head);
link delete(link *phead, link x);
link delete_data(link *phead, int d1, int d2);
void stampa(FILE *outfile, link x);
int convertiData(char *ggmmaaaa);

int main() {
    FILE *outfile;
    int comando = 0, d1, d2, d;
    char *i, *f;

    link head = NULL, x, t;

    while (comando != -1) {
        printf("Possibili comandi:\n"
               "1. Inserimento nuovo elemento da tastiera\n"
               "2. Inserimento nuovi elementi da file\n"
               "3. Ricerca per codice di un elemento\n"
               "4. Cancellazione di un elemento dalla lista in base al codice\n"
               "5. Cancellazione degli elementi compresi tra due date\n"
               "6. Stampa della lista su file\n"
               "Qualsiasi altro comando termina il programma\n"
               "Selezionare azione desiderata tramite numero >");
        if (scanf("%d", &comando) <= 0) {
            comando = -1;
        }
        switch (comando) {
            case 1:
                head = newNode_v(head);
                break;
            case 2:
                head = newNode_f(head);
                break;
            case 3:
                x = ricerca(head);
                if (x == NULL) {
                    printf("La ricerca non ha prodotto risultato\n");
                }
                else {
                    stampa(stdout, x);
                }
                break;
            case 4:
                x = ricerca(head);
                if (x == NULL) {
                    printf("La ricerca non ha prodotto risultato\n");
                }
                else {
                    x = delete(&head, x);
                    stampa(stdout, x);
                    free(x);    // si cancella il dato
                }
                break;
            case 5:
                t = head;
                i = malloc((MAXLEN+1)*sizeof(char)), f = malloc((MAXLEN+1)*sizeof(char));
                printf("Inserire prima data >"); scanf("%s", i);
                printf("Inserire seconda data >"); scanf("%s", f);
                d1 = convertiData(i); d2 = convertiData(f);
                free(i), free(f);
                if (d1 > d2) {
                    d = d2; d2 = d1; d1 = d;
                } d = 0;
                while ((x = delete_data(&head, d1, d2)) != NULL) {
                    d++;
                    stampa(stdout, x);
                    free(x);
                }
                if (d == 0)
                    printf("La ricerca non ha prodotto risultato\n");

                break;
            case 6:
                x = head;
                outfile = fopen(OUTFILE, "w");
                while (x != NULL){
                    stampa(outfile, x);
                    x = x->next;
                }
                fclose(outfile);
                break;
            default:
                printf("Nessun comando valido inserito\n");
                comando = -1;
        }
        printf("\n");
    }

    printf("Programma terminato\n");

    return 0;
}

link newNode(FILE *infile) {
    link x = (link) malloc(sizeof(struct node));
    if (fscanf(infile, "%s %s %s %s %s %s %d", x->val.codice, x->val.nome, x->val.cognome,
           x->val.data_s, x->val.via, x->val.citta, &(x->val.cap)) == EOF) return NULL;
    x->val.data_i = convertiData(x->val.data_s);
    x->next = x->pred = NULL;
    return x;
}

link newNode_v(link head) {
    printf("Inserire nuovo elemento >");
    head = insertNode(head, newNode(stdin));
    return head;
}

link newNode_f(link head) {
    FILE *infile;
    char nomeFile[MAXLEN+4] = {'.','.','/'};
    link x;
    printf("Inserire nome file >");
    scanf("%s", &nomeFile[3]);
    infile = fopen(nomeFile, "r");
    while ((x = newNode(infile)) != NULL)
        head = insertNode(head, x);
    fclose(infile);
    return head;
}

link insertNode(link head, link x) {
    link h = head;

    if (head == NULL || head->val.data_i > x->val.data_i) {
        x->next = head;
        head = x;
    }
    else {
        while (h->next != NULL && h->next->val.data_i < x->val.data_i) {
            h = h->next;
        }
        x->next = h->next; x->pred = h;
        if (h->next != NULL) h->next->pred = x;
        h->next = x;
    }
    return head;
}

link ricerca(link head) {
    /** per far in modo che la ricerca possa essere comodamente usata anche per la cancellazione in base ad un codice dato,
        essa restituisce l'elemento precedente alla ricorrenza, altrimenti NULL (lista vuota o elemento non trovato)
        */
    link x = head;
    char cod[MAXLEN];
    if (x != NULL) {
        printf("Inserire codice >");
        scanf("%s", cod);
        while (x != NULL && strcmp(x->val.codice, cod) != 0) {
            x = x->next;
        }
    }
    return x;
}

link delete(link *phead, link x) {
    if (x == *phead)
        *phead = x->next;
    if (x->pred != NULL)
        x->pred->next = x->next;
    if (x->next != NULL)
        x->next->pred = x-> pred;
    return x;
}

link delete_data(link *phead, int d1, int d2) {
    link x = *phead;
    while (x != NULL && x->val.data_i < d1) {
        x = x->next;
    }
    if (x != NULL && x->val.data_i <= d2) {
        x = delete(phead, x);
        return x;
    }
    return NULL;
}

void stampa(FILE *outfile, link x) {
    fprintf(outfile, "%s %s %s %s %s %s %d\n", x->val.codice, x->val.nome, x->val.cognome,
            x->val.data_s, x->val.via, x->val.citta, x->val.cap);
}

int convertiData(char *ggmmaaaa) {
    int g, m, a;
    sscanf(ggmmaaaa, "%d/%d/%d", &g, &m, &a);
    return a*10000 + m*100 * g;
}
