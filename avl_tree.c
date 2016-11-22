#include "binary_tree.h"

//add a node for AVL tree

static binary_tree_node_t *ll_rotate(binary_tree_node_t *node)
{
	binary_tree_node_t *top = node->lchild;

	node->lchild = top->rchild;
	if (top->rchild != NULL)
	{
		top->rchild->parent = node;
	}

	top->rchild = node;
	node->parent = top;
	return top;
}

static binary_tree_node_t *rr_rotate(binary_tree_node_t *node)
{
	binary_tree_node_t *top = node->rchild;

	node->rchild = top->lchild;
	if (top->lchild != NULL)
	{
		top->lchild->parent = node;
	}

	top->lchild = node;
	node->parent = top;
	return top;
}

static binary_tree_node_t *lr_rotate(binary_tree_node_t *node)
{
	node->lchild = rr_rotate(node->lchild);
	return ll_rotate(node);
}

static binary_tree_node_t *rl_rotate(binary_tree_node_t *node)
{
	node->rchild = ll_rotate(node->rchild);
	return rr_rotate(node);
}

static binary_tree_node_t *make_balance_4_add(binary_tree_t *btree, binary_tree_node_t *node)
{
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

		if (btree->compare_func(node->data, unbalance_node->data) > 0) // rchild child
		{
			if (rdeepth - ldeepth == 2)
			{
#if TREE_DEBUG
				printf("unbalance_node is: %d\n", *((int *) unbalance_node->data));
#endif				
				if (btree->compare_func(node->data, unbalance_node->rchild->data) > 0)
				{
#if TREE_DEBUG
					printf("need RR rotate");
#endif					
					rotate_sub_tree = rr_rotate(unbalance_node);
				}
				else
				{
#if TREE_DEBUG
					printf("need RL rotate");
#endif
					rotate_sub_tree = rl_rotate(unbalance_node);
				}
				break;
			}
		}
		else if (btree->compare_func(node->data, unbalance_node->data) < 0) // lchild child
		{
			if (ldeepth - rdeepth == 2)
			{
#if TREE_DEBUG
				printf("unbalance_node is: %d\n", *((int *) unbalance_node->data));
#endif				

				if (btree->compare_func(node->data, unbalance_node->lchild->data) > 0)
				{
#if TREE_DEBUG
					printf("need LR rotate");
#endif					
					rotate_sub_tree = lr_rotate(unbalance_node);
				}
				else
				{
#if TREE_DEBUG
					printf("need LL rotate");
#endif					
					rotate_sub_tree = ll_rotate(unbalance_node);
				}
				break;
			}
		}
		unbalance_node = unbalance_node->parent;
	}

	if (parent_node != NULL)
	{
		if (btree->compare_func(rotate_sub_tree->data, parent_node->data) > 0)
		{
			parent_node->rchild = rotate_sub_tree;
		}
		else
		{
			parent_node->lchild = rotate_sub_tree;
		}

	}
	else
	{
		btree->root = rotate_sub_tree;
	}
	rotate_sub_tree->parent = parent_node;
#if TREE_DEBUG
	printf("\nAFTER ROTATE THE TREE IS:------------------\n");
	print_ascii_tree(btree->root);
#endif	
	return rotate_sub_tree;
}


static binary_tree_node_t *make_balance_4_del(binary_tree_t *btree, binary_tree_node_t *sub_tree, void *del_data)
{
	binary_tree_node_t *rotate_sub_tree = btree->root; //sub tree after rotate
#if TREE_DEBUG
	printf("\nBEFORE ROTATE THE TREE IS:------------------\n");
	print_ascii_tree(btree->root);
#endif

	binary_tree_node_t *unbalance_node = sub_tree;
	binary_tree_node_t *parent_node = NULL;
	int ldeepth = 0;
	int rdeepth = 0;
	while (unbalance_node != NULL)
	{
		parent_node = unbalance_node->parent;
		ldeepth = binary_tree_deepth_sub_tree(unbalance_node->lchild);
		rdeepth = binary_tree_deepth_sub_tree(unbalance_node->rchild);
		
		if (rdeepth - ldeepth == 2)
		{
#if TREE_DEBUG
			printf("unbalance_node is: %d\n", *((int *) unbalance_node->data));
#endif				
			if (btree->compare_func(del_data, unbalance_node->rchild->data) > 0)
			{
#if TREE_DEBUG
				printf("need RR rotate");
#endif					
				rotate_sub_tree = rr_rotate(unbalance_node);
			}
			else
			{
#if TREE_DEBUG
				printf("need RL rotate");
#endif
				rotate_sub_tree = rl_rotate(unbalance_node);
			}
			break;
		}

		if (ldeepth - rdeepth == 2)
		{
#if TREE_DEBUG
			printf("unbalance_node is: %d\n", *((int *) unbalance_node->data));
#endif				

			if (btree->compare_func(del_data, unbalance_node->lchild->data) > 0)
			{
#if TREE_DEBUG
				printf("need LR rotate");
#endif					
				rotate_sub_tree = lr_rotate(unbalance_node);
			}
			else
			{
#if TREE_DEBUG
				printf("need LL rotate");
#endif					
				rotate_sub_tree = ll_rotate(unbalance_node);
			}
			break;
		}
		unbalance_node = unbalance_node->parent;
	}

	if (parent_node != NULL)
	{
		if (btree->compare_func(rotate_sub_tree->data, parent_node->data) > 0)
		{
			parent_node->rchild = rotate_sub_tree;
		}
		else
		{
			parent_node->lchild = rotate_sub_tree;
		}

	}
	else
	{
		btree->root = rotate_sub_tree;
	}
	rotate_sub_tree->parent = parent_node;
#if TREE_DEBUG
	printf("\nAFTER ROTATE THE TREE IS:------------------\n");
	print_ascii_tree(btree->root);
#endif	
	return rotate_sub_tree;
}

binary_tree_node_t *binary_tree_avl_add(binary_tree_t *btree, void *data)
{
	binary_tree_node_t *node = binary_tree_add(btree, data);
	make_balance_4_add(btree, node);
	return node;
}

binary_tree_node_t *binary_tree_avl_del(binary_tree_t *btree, void *data)
{
	binary_tree_node_t *sub_tree = binary_tree_del(btree, data);
	return make_balance_4_del(btree, sub_tree, data); 
}
