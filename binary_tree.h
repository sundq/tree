/* 
 * File:   binary_tree.h
 * Author: sundq
 *
 * Created on 2016年11月11日, 下午12:50
 */

#ifndef BINARY_TREE_H
#define	BINARY_TREE_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h> 
#include "./util/memory_pool.h"

#define DEFAULT_BUFF_SIZE 1024 
#define MAX_HEIGHT 1000
#define INFINITY (1<<20)

typedef struct binary_tree_node binary_tree_node_t;
typedef struct binary_tree binary_tree_t;
typedef enum rb_color { RED = 1, BLACK} rb_color_t;
typedef int (* compare_func_t)(void *src, void *dst);
typedef int (* traversal_callback)(void *data);

struct binary_tree_node
{
    void *data;
    rb_color_t color; //for rb tree
    size_t count; //current node count   
    struct binary_tree_node *lchild;
    struct binary_tree_node *rchild;
    struct binary_tree_node *parent;
};

struct binary_tree
{
    binary_tree_node_t *root;
    size_t size;
    size_t deep;
    compare_func_t compare_func; //compare function 
};


binary_tree_t *binary_tree_init(compare_func_t func);
binary_tree_node_t *binary_tree_add(binary_tree_t *t, void *data);
binary_tree_node_t *binary_tree_avl_add(binary_tree_t *t, void *data);
binary_tree_node_t *binary_tree_rb_add(binary_tree_t *t, void *data);
binary_tree_node_t *binary_tree_get(binary_tree_t *t, void *data);
binary_tree_node_t *binary_tree_del(binary_tree_t *t, void *data);
binary_tree_node_t *binary_tree_avl_del(binary_tree_t *t, void *data);
binary_tree_node_t *binary_tree_rb_del(binary_tree_t *t, void *data);
int binary_tree_range(binary_tree_t *t, void *max_data, void *min_data);
int binary_tree_inorder_traversal(binary_tree_t *t, traversal_callback cb);
int binary_tree_bfs_traversal(binary_tree_t *t, traversal_callback cb);
int binary_tree_bfs_traversal_subtree(binary_tree_node_t *t, traversal_callback cb);
int binary_tree_deepth_sub_tree(binary_tree_node_t *t);
void print_ascii_tree(binary_tree_node_t * t);
#ifdef	__cplusplus
}
#endif

#endif	/* BINARY_TREE_H */

