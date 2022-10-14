#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXNUM 20
#define MAXLINE 256

int binary_search(int *a, int K, int m, int n, int *c) {
    int i, j, p;
    *c = 0;

    i = m;
    j = n;
    while (i <= j) {
        *c = *c + 1;
        p = (i + j) / 2;
        if (K < a[p]) j = p - 1;
        else if (K > a[p]) i = p + 1;
        else if (K == a[p]) return p;
    }

    return -1;
}

int main() {
    int a[MAXNUM], i, k, ret, r, c;
    char input[MAXLINE];

    for (i = 0; i < MAXNUM; i++) {
        a[i] = i + 1;
        printf("%d ", a[i]);
    }
    printf("\n");

    printf("Key? ");
    fgets(input, MAXLINE, stdin);
    ret = sscanf(input, "%d", &k);
    if (ret != 1) {exit(1);}

    r = binary_search(a, k, 0, MAXNUM, &c);
    if (r == -1) {
        printf("Not found.\n");
    } else {
        printf("Found (Step = %d)\n", c);
    }
}