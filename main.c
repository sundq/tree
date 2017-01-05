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
  int a1 = *(int *)a;
  int b1 = *(int *)b;
  //printf("+++++++%d %d\n",a1, b1);
  return a1 - b1;
}

static int traverse(void *a)
{
  if (a == NULL)
  {
    printf("NULL ");
    return 0;
  }
  printf("%d ", *(int *)a);
  return 0;
}

static int traverse_bfs(void *a)
{
  //	static binary_tree_node_t *p_node =
  printf("%d||", *(int *)a);
  return 0;
}

void rb_tree_test()
{
  int *del_tmp = malloc(sizeof(int));
  srand((unsigned)time(NULL));
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
    int *tmp = malloc(sizeof(int));
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
  #define size_buff 1024 * 1024
  int cache[size_buff] = {0};
  
  // int test[] = {1002, 651, 936, 697, 851, 475, 228, 279, 697, 584, 799, 308, 498, 930, 436, 175, 633,
  //               457, 913, 936, 431, 381};
  //int test[] = {22, 95, 60, 88, 1, 93, 85, 73, 63, 50, 47, 40, 44, 44};
  //int test[] = {271, 565, 206, 483, 144, 46, 259, 255, 303};

  int test[] = {
    27,
8, 17,35,
2, 6,12 ,14,20 ,23,29, 33,38,
0,3,7,11,13,16,18, 19,21 ,22,24, 25,28,31,34,37,39
  };
  //int test[] = {583, 384, 150, 240, 180, 491, 453, 35, 557};
  int num = sizeof(cache) / sizeof(int); //sizeof(test) / sizeof(int);
  srand((unsigned)time(NULL));
  b_tree_t *t = b_tree_init(4, compare);
  char xx[100] = {};
  for (int i = 0; i < num; i++)
  {
    int tmp = rand() % (num * 20);
    printf("add %d\n", tmp);
    cache[i] = tmp;
    //sprintf(xx, "echo '%d,' >> ./xx", *tmp);
    //system(xx);
    b_tree_add_node_int(t, tmp);
    // printf("*****************************1\n");
    // b_tree_bfs_traversal_subtree(t->root, traverse);
    // printf("*****************************2\n\n\n");
  }
  //system("echo '\n' >> ./xx");

  //for (int i = num - 1; i >= 0; i--)
  //int cache[] = {21, 18, 9, 12, 22, 20, 16, 29, 24, 25, 14, 18, 27, 13, 26, 15, 9, 2, 13, 3, 1};
  for (int i = 0; i < num; i++)
  {
    int tmp = cache[rand() % num];
    //*tmp = cache[i];
    printf("DEL %d\n", tmp);
    b_tree_del_node_int(t, tmp);
    // printf("==============================1\n");
    // b_tree_bfs_traversal_subtree(t->root, traverse);
    // printf("==============================2\n\n\n");
  }
  b_tree_bfs_traversal_subtree(t->root, traverse);
}

int main(int argc, char **argv)
{
  //	rb_tree_test();
  b_tree_test();
  return (EXIT_SUCCESS);
}
