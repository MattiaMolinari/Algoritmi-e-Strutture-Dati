#include "pg.h"

int pg_read(FILE *fp, pg_t *pgp) {
    int err;
    err = fscanf(fp, "%s %s %s ", pgp->cod, pgp->nome, pgp->classe);
    stat_read(fp, &pgp->b_stat);
    stat_reset(&pgp->eq_stat);
    stat_update(&pgp->eq_stat, pgp->b_stat);
    pgp->equip = equipArray_init();
    return err;
}

void pg_clean(pg_t *pgp) {
    equipArray_free(pgp->equip);
}

void pg_print(FILE *fp, pg_t *pgp, invArray_t invArray) {
    fprintf(fp, "%s %s %s ", pgp->cod, pgp->nome, pgp->classe);
    stat_print(fp, &pgp->eq_stat, MIN_STAT);
    equipArray_print(fp, pgp->equip, invArray);
}

void pg_updateEquip(pg_t *pgp, invArray_t invArray) {
    equipArray_update(pgp->equip, invArray);
    stat_reset(&pgp->eq_stat);
    stat_update(&pgp->eq_stat, pgp->b_stat);
    for (int i = 0; i < equipArray_inUse(pgp->equip); i++)
        stat_update(&pgp->eq_stat, inv_getStat(
                equipArray_getEquipByIndex(pgp->equip, i)));
}

char *pg_getCod(pg_t *pgp)  {
    return pgp->cod;
}