#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#ifndef N
#define N 5
#endif
#ifndef FS
#define FS 38
#endif

struct node {
    int data;
    int fibdata;
    struct node *next;
};

int fib(int n) {
    int x, y;
    if (n < 2) {
        return (n);
    } else {
        x = fib(n - 1);
        y = fib(n - 2);
        return (x + y);
    }
}

void processwork(struct node *p) {
    int n;
    n = p->data;
    p->fibdata = fib(n);
}

struct node *init_list(struct node *p) {
    int i;
    struct node *head = NULL;
    struct node *temp = NULL;

    head = (struct node *) malloc(sizeof(struct node));
    p = head;
    p->data = FS;
    p->fibdata = 0;
    for (i = 0; i < N; i++) {
        temp = (struct node *) malloc(sizeof(struct node));
        p->next = temp;
        p = temp;
        p->data = FS + i + 1;
        p->fibdata = i + 1;
    }
    p->next = NULL;
    return head;
}

void array_impl(struct node *head) {
    struct node *p = head;
    int len = 0;
    while (p != NULL) {
        p = p->next;
        len++;
    }
    int ids[len];
    p = head;
    for (int i = 0; i < len; i++) {
        ids[i] = p->data;
        p = p->next;
    }
    int fibdata[len];
#pragma omp parallel for
    for (int i = 0; i < len; i++) {
        int n = fib(ids[i]);
#pragma omp critical
        fibdata[i] = n;
    } {
        int i = 0;
        p = head;
        while (p != NULL) {
            p->fibdata = fibdata[i];
            p = p->next;
            i++;
        }
    }
}

int main(int argc, char *argv[]) {
    double start, end;
    struct node *p = NULL;
    struct node *temp = NULL;
    struct node *head = NULL;

    printf("Process linked list\n");
    printf("  Each linked list node will be processed by function 'processwork()'\n");
    printf("  Each ll node will compute %d fibonacci numbers beginning with %d\n",N,FS);
    //38 : 39088169
    // 39 : 63245986
    // 40 : 102334155
    // 41 : 165580141
    // 42 : 267914296
    // 43 : 433494437
    p = init_list(p);
    head = p;
    start = omp_get_wtime();
    array_impl(head);

    // {
    //    while (p != NULL) {
    // processwork(p);
    // p = p->next;
    //    }
    // }

    end = omp_get_wtime();
    p = head;
    while (p != NULL) {
        printf("%d : %d\n", p->data, p->fibdata);
        temp = p->next;
        free(p);
        p = temp;
    }
    free(p);

    printf("Compute Time: %f seconds\n", end - start);

    return 0;
}
