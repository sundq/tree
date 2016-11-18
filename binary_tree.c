#include "binary_tree.h"
#include "./util/queue.h"

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
	binary_tree_node_t *parent_node = btree->root;
	while (*current_node != NULL)
	{
		parent_node = *current_node;
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
		(*current_node)->parent = parent_node;
	}
	return ret;
}

binary_tree_node_t *binary_tree_get(binary_tree_t *btree, void *data)
{
	binary_tree_node_t *current_node = btree->root;
	while (current_node != NULL)
	{
		if (btree->compare_func(data, current_node->data) > 0) //right child
		{
			current_node = current_node->rchild;
		}
		else if (btree->compare_func(data, current_node->data) < 0) //left child
		{
			current_node = current_node->lchild;
		}
		else
		{
			return current_node;
		}

	}
	return NULL;
}

int binary_tree_inorder_traversal(binary_tree_t *btree, traversal_callback cb)
{
	binary_tree_node_t *current_node = btree->root;
	binary_tree_node_t *prev_node = NULL;
	while (current_node != NULL)
	{
		if (current_node->lchild == NULL) //the left child is NULL, process current node and set current_node to right child
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

int binary_tree_bfs_traversal(binary_tree_t *btree, traversal_callback cb)
{
	queue_t *q = queue_init(1024);
	queue_push(q, btree->root);

	while (!queue_is_empty(q))
	{
		binary_tree_node_t *node = (binary_tree_node_t *) queue_pop(q);
		cb(node->data);
		if (node->lchild != NULL)
		{
			queue_push(q, node->lchild);
		}

		if (node->rchild != NULL)
		{
			queue_push(q, node->rchild);
		}
	}
	return 1;
}

int binary_tree_del(binary_tree_t *btree, void *data)
{
	binary_tree_node_t *node = binary_tree_get(btree, data);
	binary_tree_node_t *parent_node = NULL;
	binary_tree_node_t *back_node = NULL;
	if (node != NULL)
	{
		node->count--;
		if (node->count == 0)
		{
			parent_node = node->parent;
			if (node->lchild != NULL && node->rchild != NULL)
			{
				back_node = node->rchild;
				while (back_node->lchild != NULL)
				{
					back_node = back_node->lchild;
				}
			}
			else
			{
				back_node = node->lchild != NULL ? node->lchild : node->rchild;
			}

			if (back_node != NULL) //back_node is NULL when node is leaf node
			{
				back_node->parent = parent_node;
				back_node->lchild = node->lchild;
				back_node->rchild = node->rchild;
			}
			release_memory(node);

			if (parent_node != NULL)
			{
				if (parent_node->lchild == node) //del left child
				{
					parent_node->lchild = back_node;
				}
				else //del right child
				{
					parent_node->rchild = back_node;
				}

			}
			else
			{
				btree->root = back_node;
			}
		}
	}

	return 0;
}

