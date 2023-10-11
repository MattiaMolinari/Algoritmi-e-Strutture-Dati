#include "inv.h"

void stat_read(FILE *fp, stat_t *statp) {
    fscanf(fp, "%d %d %d %d %d %d",
           &statp->hp, &statp->mp, &statp->atk, &statp->def, &statp->mag, &statp->spr);
}

void stat_print(FILE *fp, stat_t *statp, int soglia) {
    fprintf(fp, "%d %d %d %d %d %d\n",
            stat_single(statp->hp, soglia),
            stat_single(statp->mp, soglia),
            stat_single(statp->atk, soglia),
            stat_single(statp->def, soglia),
            stat_single(statp->mag, soglia),
            stat_single(statp->spr, soglia)
            );

}

int stat_single(int singlestat, int soglia) {
    if (singlestat >= soglia)
        return singlestat;
    return soglia-1;
}

void stat_reset(stat_t *statp) {
    statp->hp = statp->mp = statp->atk = statp->def = statp->mag = statp->spr = 0;
}

void stat_update(stat_t *statBasep, stat_t statModifier) {
    statBasep->hp += statModifier.hp;
    statBasep->mp += statModifier.mp;
    statBasep->atk+= statModifier.atk;
    statBasep->def += statModifier.def;
    statBasep->mag += statModifier.mag;
    statBasep->spr += statModifier.spr;
}

void inv_read(FILE *fp, inv_t *invp) {
    fscanf(fp, "%s %s ", invp->nome, invp->tipo);
    stat_read(fp, &invp->stat);
}

void inv_print(FILE *fp, inv_t *invp) {
    fprintf(fp, "%s %s ", invp->nome, invp->tipo);
    stat_print(fp, &invp->stat, -10000);
}

stat_t inv_getStat(inv_t *invp) {
    return invp->stat;
}

char *inv_getName(inv_t *invp) {
    return invp->nome;
}
