#include "binary_tree.h"

//add a node for AVL tree

static binary_tree_node_t *ll_rotate(binary_tree_node_t *node)
{
	binary_tree_node_t *l_child = node->lchild;
	l_child->rchild = node;
	l_child->parent = NULL;
    l_child->parent = node->parent;
	node->parent = l_child;
	node->rchild = NULL;
	node->lchild = NULL;
	
	return l_child;
}

static binary_tree_node_t *rr_rotate(binary_tree_node_t *node)
{
	binary_tree_node_t *r_child = node->rchild;
	r_child->lchild = node;
	r_child->parent = NULL;
    r_child->parent = node->parent;
	node->parent = r_child;
	node->rchild = NULL;
	node->lchild = NULL;
	
	return r_child;
}

static binary_tree_node_t *lr_rotate(binary_tree_node_t *node)
{
	binary_tree_node_t *lchild = node->lchild;
	binary_tree_node_t *lr_child = lchild->rchild;
	node->lchild = lr_child;
	lr_child->lchild = lchild;
	lr_child->parent = lchild->parent;
	lchild->parent = lr_child;
	return ll_rotate(node);
}

static binary_tree_node_t *rl_rotate(binary_tree_node_t *node)
{
	binary_tree_node_t *rchild = node->rchild;
	binary_tree_node_t *rl_child = rchild->lchild;
	node->rchild = rl_child;
	rl_child->rchild = rchild;
	rl_child->parent = rchild->parent;
	rchild->parent = rl_child;
	return rr_rotate(node);
}

binary_tree_node_t *binary_tree_avl_add(binary_tree_t *btree, void *data)
{
	binary_tree_node_t *node = binary_tree_add(btree, data);
	binary_tree_node_t *rotate_sub_tree = btree->root; //sub tree after rotate
#if TREE_DEBUG
	printf("\nBEFORE ROTATE THE TREE IS:------------------\n");
	print_ascii_tree(btree->root);
#endif

	binary_tree_node_t *unbalance_node = node->parent;
	int ldeepth = 0;
	int rdeepth = 0;
	while (unbalance_node != NULL)
	{
		ldeepth = binary_tree_deepth_sub_tree(unbalance_node->lchild);
		rdeepth = binary_tree_deepth_sub_tree(unbalance_node->rchild);

		if (btree->compare_func(data, unbalance_node->data) > 0) // rchild child
		{
			if (rdeepth - ldeepth == 2) //RR rotate
			{
#if TREE_DEBUG
				printf("unbalance_node is: %d\n", *((int *) unbalance_node->data));
#endif				
				if (btree->compare_func(data, unbalance_node->rchild->data) > 0)
				{
					rotate_sub_tree = rr_rotate(unbalance_node);
				}
				else
				{
					rotate_sub_tree = rl_rotate(unbalance_node);
				}
				break;
			}
		}
		else if (btree->compare_func(data, unbalance_node->data) < 0) // lchild child
		{
			if (ldeepth - rdeepth == 2)
			{
				if (btree->compare_func(data, unbalance_node->lchild->data) > 0)
				{
					rotate_sub_tree = lr_rotate(unbalance_node);
				}
				else
				{
					rotate_sub_tree = ll_rotate(unbalance_node);
				}
				break;
			}
		}
		unbalance_node = unbalance_node->parent;
	}

	if (rotate_sub_tree->parent == NULL)
	{
		btree->root = rotate_sub_tree;
	}
#if TREE_DEBUG
	printf("\nAFTER ROTATE THE TREE IS:------------------\n");
	print_ascii_tree(btree->root);
#endif
	return node;
}
