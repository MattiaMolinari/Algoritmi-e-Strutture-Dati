#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {z_e, r_e, t_e, s_e} gems;

int fz(char *val, int *count, int p, char *sol);
int collana(char *val, int n);


int main() {
    int count[4] = {2,2,2,2}; char sol[50];
    sol[49] = '\0';
    printf("%d", fz("zrts\0", count, 0, sol));
    return 0;
}

char *newVal(char pietra) {
    switch(pietra) {
        case 'z': return "zr\0";
        case 'r': return "ts\0";
        case 't': return "zr\0";
        case 's': return "ts\0";
    }
}

gems newCount(char pietra) {
    switch(pietra) {
        case 'z': return z_e;
        case 'r': return r_e;
        case 't': return t_e;
        case 's': return s_e;
    }
}


int collana(char *val, int n) {
    return 0;
}

int fz(char *val, int *count, int p, char *sol) {
    if (p == 0) {
        if (count[z_e] == 0)
            return p;
        sol[p] = val[z_e];
        count[z_e]--;
        return p = fz(newVal(val[z_e]), count, p+1, sol);
    }
    for (int i = 0, c; i < strlen(val); i++) {
        c = newCount(val[i]);
        if (count[c] > 0) {
            sol[p] = val[i];
            count[c]--;
            p = fz(newVal(val[i]), count, p+1, sol); //todo
        }
    }
    return p-1;

}


