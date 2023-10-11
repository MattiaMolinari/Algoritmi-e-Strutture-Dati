#include <stdio.h>

int gcd(int a, int b);

int main() {
    int m = -5,n = 0; //esempio

    printf("%d", gcd(m,n));
    return 0;
}

int gcd(int a, int b){
    int tmp, mcd = 1;

    if (a < b){
        tmp = a;
        a = b;
        b = tmp;
    }

    if (b <= 0) {
        printf("Massimo comune divisore non eseguito (necessari numeri maggiori di 0)\n");
        return -1;
    }

    if (a % b == 0) {
        return b;
    }

    if (a % 2 == 0 && b % 2 == 0){
        mcd = 2*gcd(a/2, b/2);
    }
    else if (a % 2 != 0 && b % 2 == 0){
        mcd = gcd(a, b/2);
    }
    else if (a % 2 != 0 && b % 2 != 0){
        mcd = gcd((a-b)/2, b);
    }
    else {
        mcd = gcd((a/2), b);
    }
    return mcd;
}
