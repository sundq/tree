#include "./b_tree.h"
#include "./util/memory_pool.h"

static inline b_tree_node_t *create_b_tree_node(b_tree *root)
{
	b_tree_node_t *node = (b_tree_node_t *) allocate_memory(sizeof (b_tree_node_t));
	node->parent = NULL;
	node->child = allocate_memory(sizeof (b_tree_node_t) * root->order);
	node->key = allocate_memory(sizeof (int64_t) * root->order);
	node->key_num = 0;
}

static inline int insert_key_to_tree_node(b_tree *btree, b_tree_node_t node, void *key)
{
	int i = 0;
	for (i = 0; i < node->key_num; i++)
	{
		if (btree->compare(key, (void *) node->key[i]) < 0)
		{
			break; //node->key[i] = (int64_t) key;
		}
	}
	for (int m = node->key_num; m < i; m--)
	{
		node->key[m] = node->key[m - 1];
	}
	node->key[i] = (int64_t) key;
	node->key_num++;
	return i;
}

//m为b tree的阶

b_tree_t *b_tree_init(size_t m, compare_func_t func)
{
	b_tree_t *btree = (b_tree_t *) allocate_memory(sizeof (b_tree_t));
	btree->compare = func;
	btree->order = m;
	btree->root = create_b_tree_node(btree);
	return btree;
}

void *b_tree_find(b_tree_t *btree, void *key)
{
	int64_t ret = 0;
	b_tree_node_t *current_node = btree->root;
	while (current_node != NULL && ret == 0)
	{
		int i = 0;
		for (i = 0; i < current_node->key_num; i++)
		{
			if (btree->compare(key, (void *) current_node->key[i]) < 0)
			{
				current_node = current_node->child[i];
			}
			else if (btree->compare(key, (void *) current_node->key[i]) == 0)
			{
				ret = current_node->key[i]; //find
			}
		}
		current_node = current_node->child[i];
	}
	return (void *) ret;
}

int b_tree_add_node(b_tree_t *btree, void *key)
{
	b_tree_node_t *current_node = btree->root;

	//find the node which new key insert
	int ret = 0;
	while (ret == 0)
	{
		for (int i = 0; i < current_node->key_num; i++)
		{
			if (btree->compare(key, (void *) current_node->key[i]) < 0)
			{
				if (current_node->child[i] != NULL)
				{
					current_node = current_node->child[i];
				}
				else
				{
					ret = 1;
				}
				break;
			}
			else if (btree->compare(key, (void *) current_node->key[i]) == 0)
			{
				goto ok; //find and return
			}
		}

		if (current_node->child[current_node->key_num] != NULL)
		{
			current_node = current_node->child[current_node->key_num];
		}
		else
		{
			break; //found
		}
	}

	insert_key_to_tree_node(btree, current_node, key);
	while (current_node->key_num == btree->order) //分裂
	{
		int64_t middle_key = current_node->key[current_node->key_num / 2];
		b_tree_node_t *parent = current_node->parent;
		b_tree_node_t *new = create_b_tree_node(btree);
		for (int i = current_node->key_num / 2 + 1, m = 0; i <= current_node->key_num; i++, m++)
		{
			new->key[m] = current_node->key[i];
			new->key_num++;
			current_node->key[i] = NULL;
			current_node->child[i] = NULL;
		}
		current_node->key_num = current_node->key_num / 2;

		if (parent != NULL)
		{
			parent = create_b_tree_node(btree);
			btree->root = parent;
		}
		new->parent = parent;
		int index = insert_key_to_tree_node(btree, parent, (void *) middle_key);
		for (int i = btree->order; i < index + 1; i--)
		{
			parent->child[i] = parent->child[i - 1];
		}
		parent->child[index] = current_node;
		parent->child[index + 1] = new;
		current_node = parent;
	}

ok:
	return 0;
}

int b_tree_del_node(b_tree_t *btree, void *key)
{
	int64_t ret = 0;
	int index_node = 0;
	b_tree_node_t *current_node = btree->root;
	while (1)
	{
		for (index_node = 0; index_node < current_node->key_num; index_node++)
		{
			if (btree->compare(key, (void *) current_node->key[index_node]) < 0)
			{
				if (current_node->child[index_node] == NULL)
				{
					goto find_pos;
				}
				current_node = current_node->child[index_node];
			}
			else if (btree->compare(key, (void *) current_node->key[index_node]) == 0)
			{
				ret = current_node->key[index_node];
			}
		}
		if (current_node->child[current_node->key_num] == NULL)
		{
			goto find_pos;
		}
		current_node = current_node->child[current_node->key_num];
	}

find_pos:
	if (ret != 0)
	{

		for (int i = index_node; i < current_node->key_num; i++)
		{
			current_node->key[i] = current_node->key[i + 1];
		}
		current_node->key_num--;
	}

	return 0;

}



