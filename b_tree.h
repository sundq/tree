/* 
 * File:   b_tree.h
 * Author: sundq
 *
 * Created on 2016年12月17日, 下午6:25
 */

#ifndef B_TREE_H
#define B_TREE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "./binary_tree.h"
#include "./util/queue.h"

#define ceil(m, n) (m / n + m % n)
#define key_size 32
#define is_leaf_node(node) (((b_tree_node_t *)(node))->leaf == 1)
#define set_node_parent(node, p)                         \
   do                                                    \
   {                                                     \
      b_tree_node_t *macro_node = (b_tree_node_t *)node; \
      b_tree_node_t *macro_p = (b_tree_node_t *)p;       \
      macro_node->parent = p;                            \
      macro_p->leaf = 0;                                 \
   } while (0)

#define set_node_child_index(node, i) ((b_tree_node_t *)node)->child_index = i

#define assign_node_key(dst, src) memcpy(dst, src, key_size)
#define clear_node_key(buff) memset(buff, 0, key_size)

typedef struct b_tree_node b_tree_node_t;
typedef struct b_tree b_tree_t;
typedef char (*key_t)[key_size];
// typedef int (*compare_func_t)(void *src, void *dst);

struct b_tree_node
{
   size_t key_num;
   b_tree_node_t *parent;
   int child_index;
   int leaf;
   void **data; //the child node when node is node leaf otherwise the data pointer
   key_t key;   //the max size of key is 24 bytes
};

struct b_tree
{
   b_tree_node_t *root;
   size_t order; //b tree的阶
   size_t node_num;
};
b_tree_t *b_tree_init(size_t m, compare_func_t func);
void *b_tree_find(b_tree_t *btree, void *key);
void *b_tree_find_int(b_tree_t *btree, int key);
int b_tree_add_node(b_tree_t *btree, void *key);
int b_tree_add_node_int(b_tree_t *btree, int key);
int b_plus_tree_add_node_int(b_tree_t *btree, int key, void *data);
int b_tree_del_node(b_tree_t *btree, void *key);
int b_tree_del_node_int(b_tree_t *btree, int key);
int b_plus_tree_del_node_int(b_tree_t *btree, int key);
int b_tree_bfs_traversal_subtree(b_tree_node_t *subtree, traversal_callback cb);

#ifdef __cplusplus
}
#endif

#endif /* B_TREE_H */
