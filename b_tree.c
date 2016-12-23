#include "./b_tree.h"
#include "./util/memory_pool.h"

static inline b_tree_node_t *create_b_tree_node(b_tree_t *root)
{
	b_tree_node_t *node = (b_tree_node_t *) allocate_memory(sizeof (b_tree_node_t));
	node->parent = NULL;
	node->child = (b_tree_node_t **) allocate_memory(sizeof (b_tree_node_t) * root->order);
	node->key = allocate_memory(sizeof (int64_t) * root->order);
	node->key_num = 0;
	node->leaf = 1;

	return node;
}

static inline int find_child_ptr(b_tree_node_t *node)
{
	b_tree_node_t *parent = node->parent;
	for (int i = 0; parent && i <= parent->key_num; i++)
	{
		if (parent->child[i] == node)
		{
			return i;
		}
	}
	return 0;
}

static inline int insert_key_to_tree_node(b_tree_t *btree, b_tree_node_t *node, void *key)
{
	int i = 0;
	for (i = 0; i < node->key_num; i++)
	{
		if (btree->compare(key, (void *) node->key[i]) < 0)
		{
			break; //node->key[i] = (int64_t) key;
		}
	}
	for (int m = node->key_num; m > i; m--)
	{
		node->key[m] = node->key[m - 1];
	}
	node->key[i] = (int64_t) key;
	node->key_num++;
	return i;
}

static inline int del_key_from_tree_node(b_tree_t *btree, b_tree_node_t *node, int key_index)
{
	for (int i = key_index; i < btree->order - 1; i++)
	{
		node->key[i] = node->key[i + 1];
	}
	node->key_num--;
	return 0;
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

int b_tree_bfs_traversal_subtree(b_tree_node_t *subtree, traversal_callback cb)
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
		b_tree_node_t *node = (b_tree_node_t *) queue_pop(q);
		if (node == NULL)
		{
			printf("|");
			continue;
		}

		if (cb != NULL)
		{
			for (int i = 0; i < node->key_num; i++)
			{
				cb((void *) node->key[i]);
			}
			printf("|");
		}

		cur_num--;
		for (int i = 0; i <= node->key_num; i++)
		{
			queue_push(q, node->child[i]);
			next_num++;
		}

		if (cur_num == 0)
		{
			printf("\n");
			cur_num = next_num;
			next_num = 0;
			level++;
		}
	}
	queue_destroy(q);
	return level;
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
	int loop_over = 0;
	while (loop_over == 0)
	{
		for (int i = 0; i < current_node->key_num; i++)
		{
			if (btree->compare(key, (void *) current_node->key[i]) < 0)
			{
				if (current_node->child[i] != NULL)
				{
					current_node = current_node->child[i];
					i = 0;
				}
				else
				{
					loop_over = 1;
				}
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
			break;
		}
	}

	insert_key_to_tree_node(btree, current_node, key);
	while (current_node->key_num == btree->order) //分裂
	{
		//		printf("split...\n");
		int64_t middle_key = current_node->key[current_node->key_num / 2];
		b_tree_node_t *parent = current_node->parent;
		b_tree_node_t *new = create_b_tree_node(btree);
		for (int i = current_node->key_num / 2 + 1, m = 0; i < current_node->key_num; i++, m++)
		{
			new->key[m] = current_node->key[i];
			new->key_num++;
			current_node->key[i] = 0;
			current_node->child[i] = NULL;
		}
		current_node->key_num = current_node->key_num / 2;

		if (parent == NULL)
		{
			parent = create_b_tree_node(btree);
			parent->leaf = 0;
			btree->root = parent;
		}
		new->parent = parent;
		current_node->parent = parent;
		int index = insert_key_to_tree_node(btree, parent, (void *) middle_key);
		for (int i = btree->order - 1; i > index + 1; i--)
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
	int index_node = 0;
	b_tree_node_t *current_node = btree->root;
	while (current_node != NULL)
	{
		while (index_node < current_node->key_num)
		{
			if (btree->compare(key, (void *) current_node->key[index_node]) < 0)
			{
				current_node = current_node->child[index_node];
				index_node = 0;
			}
			else if (btree->compare(key, (void *) current_node->key[index_node]) == 0)
			{
				goto node_del; //find
			}
			else
			{
				index_node++;
			}
		}
		index_node = 0;
		current_node = current_node->child[current_node->key_num];
	}

node_del:
	if (current_node != NULL)
	{
		if (current_node->leaf)
		{
			del_key_from_tree_node(btree, current_node, index_node);
		}

		while ((current_node != NULL &&
			current_node->key_num < ceil(btree->order, 2) - 1 && //至少有ceil(m/2)-1个关键字,除根结点 
			current_node->parent != NULL))
		{
			if (!current_node->leaf) //非叶子结点
			{
				b_tree_node_t *successor_node = current_node->child[index_node + 1];
				while (successor_node->child[0] != NULL)//找后继结点
				{
					successor_node = successor_node->child[0];
				}
				current_node->key[index_node] = successor_node->key[0];
				del_key_from_tree_node(btree, successor_node, 0);
				current_node = successor_node;
				index_node = 0;
				continue;
			}

			int index = find_child_ptr(current_node);
			b_tree_node_t *l_sibling = index > 0 ? current_node->parent->child[index - 1] : NULL;
			b_tree_node_t *r_sibling = index < btree->order - 1 ? current_node->parent->child[index + 1] : NULL;

			if (l_sibling != NULL && l_sibling->key_num >= ceil(btree->order, 2)) //左兄弟丰满
			{
				insert_key_to_tree_node(btree, current_node, (void *) current_node->parent->key[index - 1]); //父结点的关键字转移到当前结点
				del_key_from_tree_node(btree, current_node->parent, index - 1); //删除父结点的关键字
				insert_key_to_tree_node(btree, current_node->parent, (void *) l_sibling->key[l_sibling->key_num - 1]);
				del_key_from_tree_node(btree, l_sibling, l_sibling->key_num - 1);
			}
			else if (r_sibling != NULL && r_sibling->key_num >= ceil(btree->order, 2)) //右兄弟丰满
			{
				insert_key_to_tree_node(btree, current_node, (void *) current_node->parent->key[index]); //父结点的关键字转移到当前结点
				del_key_from_tree_node(btree, current_node->parent, index); //删除父结点的关键字
				insert_key_to_tree_node(btree, current_node->parent, (void *) r_sibling->key[0]);
				del_key_from_tree_node(btree, r_sibling, 0);
			}
			else //合并
			{
				if (l_sibling != NULL) //左兄弟存在 
				{
					insert_key_to_tree_node(btree, l_sibling, (void *) l_sibling->parent->key[index - 1]);
					del_key_from_tree_node(btree, current_node->parent, index - 1);
					for (int i = 0; i < current_node->key_num; i++)
					{
						l_sibling->key[l_sibling->key_num + i] = current_node->key[i]; //当前节点合并到左兄弟结点	
					}
				}
				else
				{
					insert_key_to_tree_node(btree, r_sibling, (void *) r_sibling->parent->key[index]); //当前结点合并到右兄弟结点
					del_key_from_tree_node(btree, r_sibling->parent, index);
					for (int i = 0; i < current_node->key_num; i++)
					{
						insert_key_to_tree_node(btree, r_sibling, (void *) current_node->key[i]);
					}
				}
				for (int i = index; i <= current_node->parent->key_num; i++)
				{
					current_node->parent->child[i] = current_node->parent->child[i + 1]; //移动孩子结点指针
				}
				release_memory(current_node);
				current_node = current_node->parent;
			}
		}
	}

	return 0;

}



