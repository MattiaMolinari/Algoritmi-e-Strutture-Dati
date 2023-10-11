#include <stdio.h>
#include <stdlib.h>

#include "quotazione.h"

typedef struct BSTnode *BSTlink;
struct BSTnode {
    quotGiornaliera_t quotazione;
    BSTlink right, left, p;
    int n;
};

struct quotBST_s {
    BSTlink head;
};

int DATAconvert(int y, int m, int d) {
    return 10000*y+100*m+d;
}
BSTlink BSTNODEnew(int y, int m, int d) {
    BSTlink x = malloc(sizeof *x);
    x->quotazione.data.year = y;
    x->quotazione.data.month = m;
    x->quotazione.data.day = d;
    x->quotazione.vn = 0;
    x->quotazione.n = 0;
    x->right = x->left = x->p = NULL;
    x->n = 0;
    return x;
}

quotBST_t BSTinit() {
    quotBST_t BST = malloc(sizeof *BST);
    BST->head = NULL;
    return BST;
}
void BSTfreeR(BSTlink head) {
    if (head == NULL) return;
    BSTfreeR(head->left);
    BSTfreeR(head->right);
    free(head);
}
void BSTfree(quotBST_t BST) {
    BSTfreeR(BST->head);
    free(BST);
}

void BSTupdate(quotBST_t BST, FILE *fin, int transizioni) {
    BSTlink b = BST->head, tmp = NULL;
    char orario[MAXORARIO];
    int n, v, y, m, d, data1, data2;
    for (int t = 0; t < transizioni; t++) {
        fscanf(fin, "%d/%d/%d %s %d %d\n", &y, &m, &d, orario, &v, &n);
        data1 = DATAconvert(y, m, d);
        while (b != NULL) {
            tmp = b;
            data2 = DATAconvert(b->quotazione.data.year, b->quotazione.data.month, b->quotazione.data.day);
            if (data2 < data1)
                b = b->right;
            else if (data1 < data2)
                b = b->left;
            else break;
        }
        if (b == NULL) {
            b = BSTNODEnew(y, m, d);
            if (tmp != NULL) {
                b->p = tmp;
                if (data2 < data1) tmp->right = b;
                else { tmp->left = b; }
                while (tmp != NULL) {
                    tmp->n++;
                    tmp = tmp->p;
                }
            }
            else { BST->head = b; }

        }
        b->quotazione.vn += (v*n);
        b->quotazione.n += n;
        sscanf(orario, "%d:%d", &b->quotazione.data.hour, &b->quotazione.data.hour);
    }
}

void BSTstoreR(FILE *fout, BSTlink head) {
    if (head == NULL) return;
    BSTstoreR(fout, head->left);
    fprintf(fout, "\t%d/%d/%d - %d : %d\n",
            head->quotazione.data.year, head->quotazione.data.month, head->quotazione.data.day,
            head->quotazione.vn/head->quotazione.n, head->n);
    BSTstoreR(fout, head->right);
}

void BSTstore(FILE *fout, quotBST_t BST) {
    BSTstoreR(fout, BST->head);
}

int BSTsearchQ(quotBST_t BST, int data) {
    BSTlink b = BST->head;
    int q = -1, d;
    while (b != NULL) {
        d = DATAconvert(b->quotazione.data.year, b->quotazione.data.month, b->quotazione.data.day);
       if (data == d) {
           q = b->quotazione.vn/b->quotazione.n;
           break;
       }
       else if (data < d)
           b = b->left;
       else
           b = b->right;
    }
    return q;
}

void BSTsearchR(BSTlink head, int data1, int data2, int *max, int *min) {
    int d;
    if (head == NULL) return;
    d = DATAconvert(head->quotazione.data.year, head->quotazione.data.month, head->quotazione.data.day);
    if (d > data2) BSTsearchR(head->left, data1, data2, max, min);
    if (data1 <= d && d <= data2) {
        BSTsearchR(head->left, data1, data2, max, min);
        if (head->quotazione.vn/head->quotazione.n > *max) *max = head->quotazione.vn/head->quotazione.n;
        if (head->quotazione.vn/head->quotazione.n < *min) *min = head->quotazione.vn/head->quotazione.n;
        BSTsearchR(head->right, data1, data2, max, min);
    }
    if (d < data1) BSTsearchR(head->right, data1, data2, max, min);

}

void BSTsearchMaxMin(quotBST_t BST, int data1, int data2, int *max, int *min) {
    BSTsearchR(BST->head, data1, data2, max, min);
}

BSTlink rotR(BSTlink h) {
    BSTlink x = h->left;
    if (x == NULL) return x;
    if (h->left != NULL)
        h->left = x->right;
    if (x->right != NULL)
        x->right->p = h;
    x->right = h;
    x->p = h->p;
    h->p = x;
    x->n = h->n;
    h->n = 1;
    if (h->left != NULL)
        h->n += (h->left) ? h->left->n : 0;
    if (h->right != NULL)
        h->n += (h->right) ? h->right->n : 0;
    return x;
}
BSTlink rotL(BSTlink h) {
    BSTlink x = h->right;
    if (x == NULL) return x;
    if (h->right != NULL)
        h->right = x->left;
    if (x->left != NULL)
        x->left->p = h;
    x->left = h;
    x->p = h->p;
    h->p = x;
    x->n = h->n;
    h->n = 1;
    if (h->left != NULL)
        h->n += (h->left) ? h->left->n : 0;
    if (h->right != NULL)
        h->n += (h->right) ? h->right->n : 0;
    return x;
}
BSTlink partR(BSTlink h, int r) {
    int t = 0;
    if (h == NULL) return h;
    if (h->left != NULL)
        t = h->left->n;
    if (t > r) {
        h->left = partR(h->left, r);
        h = rotR(h);
    }
    if (t < r) {
        h->right = partR(h->right, r-t-1);
        h = rotL(h);
    }
    return h;
}
static BSTlink balanceR(BSTlink h) {
    int r;
    if (h == NULL) return NULL;
    r = (h->n+1)/2-1;
    h = partR(h, r);
    if (h != NULL)
        h->left = balanceR(h->left);
    if (h != NULL)
        h->right = balanceR(h->right);
    return h;
}
void BSTbalance(quotBST_t bst) {
    bst->head = balanceR(bst->head);
}

int longPathR(BSTlink head, int l) {
    if (head->right != NULL) {
        l++;
        longPathR(head->right, l);
    }
    if (head->left != NULL) {
        l++;
        longPathR(head->left, l);
    }
    return l;
}
int shortPathR(BSTlink head, int s) {
    if (head->left == NULL || head->right == NULL)
        return s;
    s++;
    shortPathR(head->left, s);
    shortPathR(head->right, s);
    return s;
}
int BSTrapport(quotBST_t BST) {
    BSTlink x = BST->head;
    int l = 1, s = 1;
    l = longPathR(x, l);
    s = shortPathR(x, s);
    return l/s;
}