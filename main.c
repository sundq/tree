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


static int compare(void *a, void *b)
{
	int a1 = *(int *)a;
	int b1 = *(int *)b;
	printf("src=%d  dst=%d\n", a1, b1);
	return a1 - b1;
}

int main(int argc, char** argv)
{
	srand((unsigned) time(NULL));
	binary_tree_t *t = binary_tree_init(compare);
	
	for (int i=0; i < 1000; i++)
	{
		int *tmp = malloc(sizeof(int));
		*tmp = rand() % 1000;
		printf("xxL:%d\n", *tmp);
	    binary_tree_add(t, tmp);		
	}
    printf("xxxxxxxxxxtree size:%lu", t->size);
	return (EXIT_SUCCESS);
}

