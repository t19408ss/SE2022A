#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUCKET_SIZE 50
#define DELTA 3

#define INSERT_Q 20
#define MAXNUM 1000

#define MAXLINE 256

int get_rand(int min, int max) {
    return min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}

int hash_insert(int *a, int b, int d, int n) {
    int h = n % b;
    int o;

    if (a[h] == 0) {
        a[h] = n;
        return 0;
    } else {
        o = h;
        do {
            o = (o + d) % b;
            if (a[o] == 0) {
                a[o] = n;
                return 0;
            }
        } while (o != h);
        return -1;
    }
}

int hash_search(int *a, int b, int d, int K, int *c) {
    int h = K % b;
    int o;
    *c = 1;
    
    if (a[h] == K) {
        return h;
    } else {
        o = h;
        do {
            *c = *c + 1;
            o = (o + d) % b;
            if (a[o] == K) {
                return o;
            }
        } while (o != h);
        return -1;
    }
}

int main() {
    int a[BUCKET_SIZE], i, n, k, ret, r, c;
    char input[MAXLINE];

    for (i = 0; i < BUCKET_SIZE; i++) {
        a[i] = 0;
    }

    for (i = 0; i < INSERT_Q; i++) {
        n = get_rand(1, MAXNUM);
        printf("%d ", n);
        ret = hash_insert(a, BUCKET_SIZE, DELTA, n);
        if (ret == -1) {exit(1);}
    }
    printf("\n");

    printf("Key? ");
    fgets(input, MAXLINE, stdin);
    ret = sscanf(input, "%d", &k);
    if (ret != 1) {exit(1);}

    r = hash_search(a, BUCKET_SIZE, DELTA, k, &c);
    if (r == -1) {
        printf("Not found.\n");
    } else {
        printf("Found (Step = %d)\n", c);
    }
}