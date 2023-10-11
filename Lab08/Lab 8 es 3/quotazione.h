#ifndef LAB_8_ES_3_QUOTAZIONE_H
#define LAB_8_ES_3_QUOTAZIONE_H

#include <stdio.h>

#define MAXDATA 10
#define MAXORARIO 5

/* Quasi ADT */
typedef struct data_s {
    int year, month, day;
    int hour, min;
} data_t;

/* Quasi ADT */
typedef struct quotGiornaliera_s {
    data_t data;
    int vn, n;
} quotGiornaliera_t;

/* ADT di I classe */
typedef struct quotBST_s *quotBST_t;

/* funzione che converte i valori di una data in un unico intero */
int DATAconvert(int y, int m, int d);
/* inizializzazione e deallocazione BST */
quotBST_t BSTinit();
void BSTfree(quotBST_t BST);
/* inserimento dati in BST */
void BSTupdate(quotBST_t BST, FILE *fin, int transizioni);
/* stampa BST */
void BSTstore(FILE *fout, quotBST_t BST);
/* ricerca quotazione in singola data */
int BSTsearchQ(quotBST_t BST, int data);
/* ricerca quotazione max e min su intervallo date */
void BSTsearchMaxMin(quotBST_t BST, int data1, int data2, int *max, int *min);
/* bilanciamento BST */
void BSTbalance(quotBST_t bst);
/* calcolo del rapporto tra cammino max e min */
int BSTrapport(quotBST_t BST);


#endif //LAB_8_ES_3_TITOLO_H
