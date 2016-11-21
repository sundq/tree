#include "binary_tree.h"

//add a node for AVL tree

static binary_tree_node_t *ll_rotate(binary_tree_node_t *node)
{
	binary_tree_node_t *top = node->lchild;
	node->lchild = top->rchild;
	top->rchild = node;
	return top;
}

static binary_tree_node_t *rr_rotate(binary_tree_node_t *node)
{
	binary_tree_node_t *top = node->rchild;
	node->rchild = top->lchild;
	top->lchild = node;
	return top;
}

static binary_tree_node_t *lr_rotate(binary_tree_node_t *node)
{
	node->lchild = rr_rotate(node->lchild);
	return ll_rotate(node);
}

static binary_tree_node_t *rl_rotate(binary_tree_node_t *node)
{
	node->rchild = ll_rotate(node->lchild);
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
	binary_tree_node_t *parent_node = NULL;
	int ldeepth = 0;
	int rdeepth = 0;
	while (unbalance_node != NULL)
	{
		parent_node = unbalance_node->parent;
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
				if (parent_node != NULL)
				{
					parent_node->rchild = rotate_sub_tree;
				}
				else
				{
					btree->root = rotate_sub_tree;
				}
				break;
			}
		}
		else if (btree->compare_func(data, unbalance_node->data) < 0) // lchild child
		{
			if (ldeepth - rdeepth == 2)
			{
#if TREE_DEBUG
				printf("unbalance_node is: %d\n", *((int *) unbalance_node->data));
#endif				
				
				if (btree->compare_func(data, unbalance_node->lchild->data) > 0)
				{
					rotate_sub_tree = lr_rotate(unbalance_node);
				}
				else
				{
					rotate_sub_tree = ll_rotate(unbalance_node);
				}
				if (parent_node != NULL)
				{
					parent_node->lchild = rotate_sub_tree;
				}
				else
				{
					btree->root = rotate_sub_tree;
				}				
				
				break;
			}
		}
		unbalance_node = unbalance_node->parent;
	}

#if TREE_DEBUG
	printf("\nAFTER ROTATE THE TREE IS:------------------\n");
	print_ascii_tree(btree->root);
#endif
	return node;
}
