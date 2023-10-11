#ifndef LAB_8_ES_3_TITOLO_H
#define LAB_8_ES_3_TITOLO_H

#include <stdio.h>

#include "quotazione.h"

#define MAXC 20

/* due ADT di I classe */
typedef struct ttl_s *ttl_t;
typedef struct ttlList_s *ttlList_t;

/* inizializzazione e deallocazione lista di titoli */
ttlList_t LISTinit();
void LISTfree(ttlList_t L);
/* lettura e stampa lista */
void LISTload(FILE *fin, ttlList_t L);
void LISTstore(FILE *fout, ttlList_t L);
/* ricerca titolo partendo da codice */
ttl_t LISTsearch(ttlList_t L, char *codice);
/* ricerca quotazione dati titolo e data */
void TTLsearchQ(FILE *fout, ttl_t titolo, int data);
/* ricerca quotazione max e min dati titolo e intervallo date */
void TTLsearchMaxMin(FILE *fout, ttl_t titolo, int data1, int data2);
/* bilanciamento */
void TTLbalance(ttl_t titolo);
/* calcolo rapporto tra cammino max e min */
int TTLrapport(ttl_t titolo);


#endif //LAB_8_ES_3_TITOLO_H
