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
//	printf("%d||", *(int *) a);
	return 0;
}

int main(int argc, char** argv)
{
	int *del_tmp = malloc(sizeof (int));
	srand((unsigned) time(NULL));
	binary_tree_t *t = binary_tree_init(compare);
//    int test[10] = {413, 267, 564, 536, 965, 692};
//	int test[10] = {885, 659, 912, 212, 759, 934, 11, 344, 168};
//	int test[10] = {836, 248, 985, 225, 455, 960, 172, 291, 505, 643};
//	int test[10] = {391, 225, 679, 123, 444, 995, 198};
	int test[10] = {371, 187, 913, 109, 349, 748, 987, 58, 166, 667};

	for (int i = 0; i < 10; i++)
	{
		int *tmp = malloc(sizeof (int));
		*tmp = test[i];//rand() % 1000;
		if (i == 3)
		{
			*del_tmp = 187;
		}
		binary_tree_avl_add(t, tmp);
	}
//	printf("xxxxxxxxxxtree size:%lu\n", t->size);
//	print_ascii_tree(t->root);
	printf("\n");
//	binary_tree_bfs_traversal(t, traverse_bfs);
	printf("\n");
	printf("del :%d\n", *del_tmp);
	binary_tree_avl_del(t, del_tmp);
	print_ascii_tree(t->root);
	binary_tree_inorder_traversal(t, traverse);
    
	printf("\n");
	return (EXIT_SUCCESS);
}

