#include "st.h"
#include <stdlib.h>
#include <string.h>

struct st_s {
	char **alfnums;
	int maxN, n;
};

st_t STinit(int maxN) {
	st_t st = malloc(sizeof(struct st_s));
	st->alfnums = malloc(maxN * sizeof(char *));
	st->n = 0;
	st->maxN = maxN;
	return st;
}
void STfree(st_t st) {
    for(int i = 0; i < st->n; i++)
        free(st->alfnums[i]);
    free(st->alfnums);
    free(st);
}

static int search(st_t st, char *name) {
    for (int i = 0; i < st->n; i++) {
        if(strcmp(name,st->alfnums[i]) == 0)
            return i;
    }
    return -1;
}
int STgetIndex(st_t st, char *name) {
	int index = search(st,name);
	if (index != -1)
		return index;
	return STinsert(st,name);
}
int STinsert(st_t st, char *name) {
	st->alfnums[st->n++] = strdup(name);
	return st->n-1;
}
char *STgetName(st_t st, int index) {
	return st->alfnums[index];
}



