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
#include "b_tree.h"

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

void rb_tree_test()
{
	int *del_tmp = malloc(sizeof (int));
	srand((unsigned) time(NULL));
	binary_tree_t *t = binary_tree_init(compare);
	//    int test[10] = {413, 267, 564, 536, 965, 692};
	//	int test[10] = {885, 659, 912, 212, 759, 934, 11, 344, 168};
	//	int test[10] = {836, 248, 985, 225, 455, 960, 172, 291, 505, 643};
	//	int test[10] = {391, 225, 6798, 123, 444, 995, 198};
	//	int test[10] = {371, 187, 913, 109, 349, 748, 987, 58, 166, 667};
	//	int test[13] = {100, 80, 200, 70, 85, 150, 250, 60, 125, 157, 230, 260, 120};
	//    int test[5] = {871, 60, 883, 435, 782};
	//	int test[] = {22, 531, 143, 841, 650, 372};
	//	int test[] = {768, 168, 792, 481, 698, 100};
	//	int test[] = {639, 371, 888, 66, 405, 389, 478, 622};
	int num = 1000 * 1000 * 10;
	int test[] = {557, 320, 854, 255, 527, 789, 982, 182};
/*	
	for(int i = 0; i < num; i++)
	{
		int *tmp = malloc(sizeof (int));
		*tmp = rand() % 100;//i;
		binary_tree_avl_add(t, tmp);
     	print_ascii_tree(t->root);
		
	}
	print_ascii_tree(t->root);
*/	


	for (int i = 0; i < num; i++)
	{
		int *tmp = malloc(sizeof (int));
		*tmp = i;
		if (i % (100000) == 0)
		{
			printf("%d\n", i / 100000);
		}
		binary_tree_rb_add(t, tmp);
	}
	printf("build a red-black tree ok.\n");
//	print_ascii_tree(t->root);
	for (int i = 0; i < num - 2; i++)
	{
		int tmp = i;
		binary_tree_rb_del(t, &tmp);
	}
		print_ascii_tree(t->root);

	printf("delete a red-black tree ok.\n");
	printf("\n");
	
}

void b_tree_test()
{
	int num = 26;
	srand((unsigned) time(NULL));
	b_tree_t *t = b_tree_init(5,compare);	
	for(int i = 0; i < num; i++)
	{
		int *tmp = malloc(sizeof (int));
		*tmp = rand() % 100;
		b_tree_add_node(t, tmp);
//     	print_ascii_tree(t->root);		
	}	
}

int main(int argc, char** argv)
{
//	rb_tree_test();
	b_tree_test();
	return (EXIT_SUCCESS);
}

