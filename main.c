/* 
 * File:   main.c
 * Author: sundq
 *
 * Created on 2016年11月11日, 下午12:49
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "binary_tree.h"

/*
 * 
 */


static int xrandom()
{
	time_t t;
	srand((unsigned) time(&t));
	return rand();
}

static int compare(void *a, void *b)
{
	int a1 = *(int *)a;
	int b1 = *(int *)b;
	return a - b;
}

int main(int argc, char** argv)
{
	binary_tree_t *t = binary_tree_init(compare);
	
	for (int i=0; i < 10000; i++)
	{
		printf("xxL:%d", i);
		int *tmp = malloc(sizeof(int));
		*tmp = xrandom();
	    binary_tree_add(t, tmp);		
	}
    printf("xxxxxxxxxxtree size:%d", t->size);
	return (EXIT_SUCCESS);
}

