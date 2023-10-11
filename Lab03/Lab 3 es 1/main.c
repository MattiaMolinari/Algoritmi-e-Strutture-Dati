#include <stdio.h>
/**
 * COMPLESSITA':
 * al peggio T(n) = 2T(n/2) + n/2 = 2*(2T(n/4)+n/4) + n/2 = ..., terminazione in T(1) = 1 => n/(2^i) = 1 => i = log_2(n)
 * T(n) = sommatoria[0<=i<=log_2(n)]{[2^(i-1)n]/(2^i)} = O(nlog(n))
 **/

int majority(int *a, int N);

int main() {
    int vet1[7] = {3, 3, 9, 4, 3, 5, 3}; // esempio
    int vet2[8] = {0,1,0,2,3,4,0,5}; // esempio
    printf("%d\n", majority(vet1, 7));
    printf("%d\n", majority(vet2, 8));

    return 0;
}

int majority(int *a, int N) {
    int resD, resS;

    if (N == 1) {
        return *a;
    }

    resS = majority(&a[0], N/2);
    resD = majority(&a[N/2], N/2+N%2);

    if (resS == resD) {
        return resS;
    }
    else {
        if (resD != -1 && N/2 < N/2+N%2) {
            return resD;

        }
        if (resS == -1) {
            for (int n = 0; n < N/2; n++) {
                if (resD == a[n]) {
                    return resD;
                }
            }
        }
        if (resD == -1) {
            for (int n = N/2; n < N; n++) {
                if (resS == a[n]) {
                    return resS;
                }
            }
        }
    }

    return -1;

}
