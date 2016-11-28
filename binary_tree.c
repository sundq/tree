#include "binary_tree.h"
#include "./util/queue.h"

static int release_node(void *a)
{
	release_memory(a);
	return 0;
}

binary_tree_node_t *binary_tree_right_rotate(binary_tree_t *btree, binary_tree_node_t *node)
{
    binary_tree_node_t *top = left(node);
    binary_tree_node_t *p_node = p(node);

    left(node) = right(top);
    if (right(top) != NULL)
    {
        p(right(top)) = node;
    }

    right(top) = node;
    p(node) = top;
    if (p_node != NULL)
    {
        if (left(p_node) == node)
        {
            left(p_node) = top;
        }
        else
        {
            right(p_node) = top;
        }
    }
    else
    {
        btree->root = top;
    }
    p(top) = p_node;
    return top;
}

binary_tree_node_t *binary_tree_left_rotate(binary_tree_t *btree, binary_tree_node_t *node)
{
    binary_tree_node_t *top = node->rchild;
    binary_tree_node_t *p_node = node->parent;

    right(node) = left(top);
    if (left(top) != NULL)
    {
        p(left(top)) = node;
    }

    left(top) = node;
    p(node) = top;

    if (p_node != NULL)
    {
        if (left(p_node) == node)
        {
            left(p_node) = top;
        }
        else
        {
            right(p_node) = top;
        }
    }
    else
    {
        btree->root = top;
    }
    p(top) = p_node;
    return top;
}


binary_tree_t *binary_tree_init(compare_func_t func)
{
	binary_tree_t *btree = (binary_tree_t *) (allocate_memory(sizeof (binary_tree_t)));
	btree->root = NULL;
	btree->compare_func = func;
	btree->size = 0;
	btree->deep = 0;
	return btree;
}

binary_tree_node_t *binary_tree_add(binary_tree_t *btree, void *data)
{
	binary_tree_node_t **current_node = &btree->root;
	binary_tree_node_t *parent_node = btree->root;
	while (*current_node != NULL)
	{
		parent_node = *current_node;
		if (btree->compare_func(data, (*current_node)->data) > 0) //rchild child
		{
			current_node = &(*current_node)->rchild;
		}
		else if (btree->compare_func(data, (*current_node)->data) < 0) //lchild child
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
	return *current_node;
}

binary_tree_node_t *binary_tree_get(binary_tree_t *btree, void *data)
{
	binary_tree_node_t *current_node = btree->root;
	while (current_node != NULL)
	{
		if (btree->compare_func(data, current_node->data) > 0) //rchild child
		{
			current_node = current_node->rchild;
		}
		else if (btree->compare_func(data, current_node->data) < 0) //lchild child
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

//中序遍历

int binary_tree_inorder_traversal(binary_tree_t *btree, traversal_callback cb)
{
	binary_tree_node_t *current_node = btree->root;
	binary_tree_node_t *prev_node = NULL;
	while (current_node != NULL)
	{
		if (current_node->lchild == NULL) //the lchild child is NULL, process current node and set current_node to rchild child
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

int binary_tree_bfs_traversal_subtree(binary_tree_node_t *subtree, traversal_callback cb)
{
	int cur_num = 1; //num of nodes lchild in current level  
	int next_num = 0; //num of nodes in next level 
	int level = 0;


	if (subtree == NULL)
	{
		return level;
	}

	queue_t *q = queue_init(1024);
	queue_push(q, subtree);
	while (!queue_is_empty(q))
	{
		binary_tree_node_t *node = (binary_tree_node_t *) queue_pop(q);
		if (cb != NULL)
		{
			cb(node->data);
		}

		cur_num--;
		if (node->lchild != NULL)
		{
			queue_push(q, node->lchild);
			next_num++;
		}

		if (node->rchild != NULL)
		{
			queue_push(q, node->rchild);
			next_num++;
		}

		if (cur_num == 0)
		{
			cur_num = next_num;
			next_num = 0;
			level++;
		}
	}
	queue_destroy(q);
	return level;
}


//bfs traversal 广度优先， 中序遍历是一种深度优先搜索

int binary_tree_bfs_traversal(binary_tree_t *btree, traversal_callback cb)
{
	return binary_tree_bfs_traversal_subtree(btree->root, cb);
}

int binary_tree_deepth_sub_tree(binary_tree_node_t *subtree)
{
	return binary_tree_bfs_traversal_subtree(subtree, NULL);
}

binary_tree_node_t *binary_tree_del(binary_tree_t *btree, void *data)
{
	binary_tree_node_t *node = binary_tree_get(btree, data);
	binary_tree_node_t *parent_successor_node = NULL;
	binary_tree_node_t *successor_node = node;
	binary_tree_node_t *successor_child_node = NULL;
	if (node != NULL)
	{
		node->count--;
		if (node->count == 0)
		{
			if (node->rchild != NULL)
			{
				parent_successor_node = node;
				successor_node = node->rchild;
				while (successor_node->lchild != NULL)
				{
					parent_successor_node = successor_node;
					successor_node = successor_node->lchild;
				}
			}
			else if (node->lchild != NULL)
			{
				parent_successor_node = node;
				successor_node = node->lchild;
				while (successor_node->rchild != NULL)
				{
					parent_successor_node = successor_node;
					successor_node = successor_node->rchild;
				}
			}
			else
			{
				parent_successor_node = node->parent;
				successor_node = node;
			}

			successor_child_node = successor_node->lchild != NULL ? successor_node->lchild : successor_node->rchild;
			if (parent_successor_node == NULL)
			{
				btree->root = NULL;
			}
			else if (parent_successor_node->lchild == successor_node)
			{
				parent_successor_node->lchild = successor_child_node;
			}
			else
			{
				parent_successor_node->rchild = successor_child_node;
			}
			if (successor_child_node != NULL)
			{
				successor_child_node->parent = parent_successor_node;
			}


			node->data = successor_node->data;
			release_memory(successor_node);
		}
	}
	return successor_child_node != NULL ? successor_child_node : node;
}

int binary_tree_destory(binary_tree_t *btree)
{
	binary_tree_bfs_traversal_subtree(btree->root, release_node);
	return 0;
}
