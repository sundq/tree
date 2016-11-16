#include "binary_tree.h"
#include "memory_pool.h"

binary_tree_t *binary_tree_init(compare_func_t func)
{
	binary_tree_t *btree = (binary_tree_t *) (allocate_memory(sizeof (binary_tree_t)));
	btree->root = NULL;
	btree->compare_func = func;
	btree->size = 0;
	btree->deep = 0;
	return btree;
}

int binary_tree_add(binary_tree_t *btree, void *data)
{
	int ret = 0;
	binary_tree_node_t **current_node = &btree->root;
	while (*current_node != NULL)
	{
		if (btree->compare_func(data, (*current_node)->data) > 0) //right child
		{
			current_node = &(*current_node)->rchild;
		}
		else if (btree->compare_func(data, (*current_node)->data) < 0) //left child
		{
			current_node = &(*current_node)->lchild;
		}
		else
		{
			(*current_node)->count++;
			break;
		}
	}

	if (*current_node == NULL)
	{
		*current_node = (binary_tree_node_t *) (allocate_memory(sizeof (binary_tree_node_t)));
		(*current_node)->data = data;
		(*current_node)->count++;
	}
	return ret;
}

int binary_tree_get(binary_tree_t *btree, void *data)
{
	int ret = 0;
	binary_tree_node_t *current_node = btree->root;
	while (current_node != NULL)
	{
		if (btree->compare_func(data, current_node->data) > 0) //right child
		{
			current_node = current_node->rchild;
		}
		else if (btree->compare_func(data, current_node->lchild) < 0) //left child
		{
			current_node = current_node->lchild;
		}
		else
		{
			return ret;
		}

	}
	ret = -1;
	return ret;
}

int binary_inorder_traversal(binary_tree_t *btree, traversal_callback cb)
{
	binary_tree_node_t *current_node = btree->root;
	binary_tree_node_t *prev_node = NULL;
	while (current_node != NULL)
	{
		if (current_node->lchild == NULL)
		{
			cb(current_node->data);
			current_node = current_node->rchild;
		}
		else
		{
			prev_node = current_node->lchild;
			while (prev_node->rchild != NULL && prev_node->rchild != current_node)
			{
				prev_node = prev_node->rchild;
			}
			
			if (prev_node->rchild == NULL)
			{
				prev_node->rchild = current_node;
				current_node = current_node->lchild;
			}
			else
			{
				prev_node->rchild = NULL;
				cb(current_node->data);
				current_node = current_node->rchild;
			}
		}
	}
	
	return 0;
}

//int binary_tree_del(binary_tree_t *btree, void *data)
//{
//	int ret = 0;
//	int index = binary_tree_get(btree, data);
//	if (index >= 0)
//	{
//		if (btree->buff[index].count > 1)
//		{
//			btree->buff[index].count--;
//			return ret;
//		}
//
//		int right_child_index = index << 1 + 2;
//		int left_child_index = index << 1 + 1;
//
//		if (right_child_index)
//		{
//
//		}
//
//
//
//
//	}
//	else
//	{
//		return -1;
//	}
//
//}

