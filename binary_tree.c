#include "binary_tree.h"

binary_tree_t *binary_tree_init(compare_func_t func)
{
	binary_tree_t *btree = (binary_tree_t *) (calloc(1, sizeof (binary_tree_t)));
	btree->header = calloc(1, DEAULT_BUFF_SIZE * sizeof (binary_tree_node_t));
	btree->compare_func = func;
	btree->size = DEAULT_BUFF_SIZE;
	return btree;
}

int binary_tree_add(binary_tree_t *btree, void *data)
{
	int ret = 0;
	size_t i = 0;
FIND_POSTION:
	i = 0;
	while (i < btree->size)
	{
		if (btree->header[i].data == NULL)
		{
			btree->header[i].data = data;
			btree->header[i].count++;
			break;
		}

		if (btree->compare_func(data, btree->header[i].data) > 0) //right child
		{
			i = i * 2 + 2;
		}
		else if (btree->compare_func(data, btree->header[i].data) < 0) //left child
		{
			i = i * 2 + 1;
		}
		else
		{
			btree->header[i].count++;
			break;
		}
	}

	if (i >= btree->size)
	{
		int new_size = (i % 2 ? (i - 1) * 2 : i * 2) * (sizeof (binary_tree_node_t));
		binary_tree_node_t *new_buffer = (binary_tree_node_t *) (calloc(1, new_size * sizeof (binary_tree_node_t)));
		if (new_buffer == NULL)
		{
			return -1;
		}
		memcpy(new_buffer, btree->header, btree->size);
		free(btree->header);
		btree->header = (binary_tree_node_t *) new_buffer;
		btree->size = new_size;
		goto FIND_POSTION;
	}
	return ret;
}