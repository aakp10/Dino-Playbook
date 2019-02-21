#include <stdio.h>
#include "complex_no.h"
#include <pthread.h>
#include <error.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

typedef struct complex_pair complex_pair;
struct complex_pair{
    complex_no *a;
    complex_no *b;
};

void *
complex_multiply(void *arg)
{
    complex_pair *multiplicands = (complex_pair *)arg;
    complex_no a = *(multiplicands->a);
    complex_no b = *(multiplicands->b);
    complex_no *result = (complex_no *)malloc(sizeof(complex_no));
    result->real = a.real*b.real - a.img*b.img;
    result->img = a.real*b.img + a.img*b.real;
    pthread_exit((void *)result);
}

int main(int argc, char *argv[])
{
    int N;
    char *buf = NULL;
    size_t len = 80;
    complex_no **c_no;
    /**
     *Take the input
     */
    FILE *fin = fopen("input.txt", "r");
    if (fin == NULL) {
        perror("file failed to open");
        exit(0);
    }
    getline(&buf, &len, fin);
    sscanf(buf, "%d", &N);
    printf("%d", N);
    c_no = (complex_no**)malloc(sizeof(complex_no *) * N);
    int index = 0;
    while(getline(&buf, &len, fin) > 0)
    {
        int real, img;
        sscanf(buf, "%d + i%d", &real, &img);
        c_no[index] = (complex_no *)malloc(sizeof(complex_no));
        c_no[index]->real = real;
        c_no[index]->img = img;
        index++;
    }

    for(int i = 0 ; i < N; i++)
    {
        printf("%d + i%d\n", c_no[i]->real, c_no[i]->img);
    }

    int nodes_count = N;
    while(nodes_count > 1)
    {
        printf("%d\n", nodes_count);
        pthread_t tid[nodes_count];
        complex_no **result;
        int level_ele_count = (int)ceilf((float)nodes_count/2);
        result = (complex_no **)malloc(sizeof(complex_no *) * level_ele_count);
        int j = 0;
        void *void_result[level_ele_count];
        struct complex_pair multiplicand_list[level_ele_count];
        for(int i = 0; i < nodes_count/2; i ++)
        {
            multiplicand_list[i].a = c_no[j++];
            multiplicand_list[i].b = c_no[j++];
            pthread_create(tid + i, NULL, complex_multiply, (void *)(multiplicand_list + i));
        }
        for(int i = 0; i < nodes_count/2; i ++)
        {
            pthread_join(tid[i], void_result + i);
            result[i] = (complex_no *)void_result[i];
        }
        //if odd add the unprocessed complex no.
        if(nodes_count & 1 == 1) {
            result[level_ele_count-1] = c_no[nodes_count-1];
        }
        //free the memory in c_no.
        int ele_in_c_no = sizeof(c_no)/ sizeof(complex_no *);
        for(int i = 0; i < ele_in_c_no - 1; i++)
        {
            free(c_no[i]);
        }
        //then
        c_no = result;
        nodes_count = (int)ceil((float)nodes_count/2);
        printf("%d after", nodes_count);
    }
    printf("ANS is %d + i%d\n", c_no[0]->real, c_no[0]->img);

    return 0;
}