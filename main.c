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
	int a1 = *(int *) a;
	int b1 = *(int *) b;
	return a1 - b1;
}

static int traverse(void *a)
{
	printf("%d ", *(int *) a);
	return 0;
}

static int traverse_bfs(void *a)
{
//	static binary_tree_node_t *p_node = 
	printf("%d||", *(int *) a);
	return 0;
}

int main(int argc, char** argv)
{
	srand((unsigned) time(NULL));
	binary_tree_t *t = binary_tree_init(compare);

	for (int i = 0; i < 10; i++)
	{
		int *tmp = malloc(sizeof (int));
		*tmp = rand() % 1000;
		binary_tree_add(t, tmp);
	}
	printf("xxxxxxxxxxtree size:%lu\n", t->size);
	int *tmp = malloc(sizeof (int));
	*tmp = 532;
	binary_tree_add(t, tmp);
	binary_tree_inorder_traversal(t, traverse);
	printf("\n");
	binary_tree_bfs_traversal(t, traverse_bfs);
	printf("\n");
	binary_tree_del(t, tmp);
	binary_tree_inorder_traversal(t, traverse);
    
	printf("\n");
	return (EXIT_SUCCESS);
}

