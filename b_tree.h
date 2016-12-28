/* 
 * File:   b_tree.h
 * Author: sundq
 *
 * Created on 2016年12月17日, 下午6:25
 */

#ifndef B_TREE_H
#define	B_TREE_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h> 
#include "./binary_tree.h"
#include "./util/queue.h"


#define ceil(m, n) (m/n + m%n)    
    
typedef struct b_tree_node b_tree_node_t;
typedef struct b_tree b_tree_t;
//typedef int (* compare_func_t)(void *src, void *dst);

struct b_tree_node
{
    size_t key_num; //实际关键字个数
    b_tree_node_t *parent; //
    b_tree_node_t **child;
    intptr_t *key; //关键字数组
    int child_index;//如果是有父节点，表示是第几个孩子
    int leaf;
};

struct b_tree
{
    b_tree_node_t *root;
    size_t order;//b tree的阶
    size_t node_num;
    compare_func_t compare;
};
b_tree_t *b_tree_init(size_t m, compare_func_t func);
void *b_tree_find(b_tree_t *btree, void *key);
int b_tree_add_node(b_tree_t *btree, void *key);
int b_tree_del_node(b_tree_t *btree, void *key);
int b_tree_bfs_traversal_subtree(b_tree_node_t *subtree, traversal_callback cb);


#ifdef	__cplusplus
}
#endif

#endif	/* B_TREE_H */

