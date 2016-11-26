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


/*
 Case 1	当前节点的父节点是红色，且当前节点的祖父节点的另一个子节点（叔叔节点）也是红色。	
(01) 将“父节点”设为黑色。
(02) 将“叔叔节点”设为黑色。
(03) 将“祖父节点”设为“红色”。
(04) 将“祖父节点”设为“当前节点”(红色节点)；即，之后继续对“当前节点”进行操作。

Case 2	当前节点的父节点是红色，叔叔节点是黑色，且当前节点是其父节点的右孩子	
(01) 将“父节点”作为“新的当前节点”。
(02) 以“新的当前节点”为支点进行左旋。

Case 3	当前节点的父节点是红色，叔叔节点是黑色，且当前节点是其父节点的左孩子	
(01) 将“父节点”设为“黑色”。
(02) 将“祖父节点”设为“红色”。
(03) 以“祖父节点”为支点进行右旋。
 */
static binary_tree_node_t *make_tree_balance(binary_tree_t *btree, binary_tree_node_t *node)
{
	binary_tree_node_t *current_node = node;
	binary_tree_node_t *p_node = NULL;
	binary_tree_node_t *p_p_node = NULL;
	binary_tree_node_t *uncle_node = NULL;
	while (current_node->parent->color == RED)
	{
		p_node = current_node->parent;
		p_p_node = p_node->parent; //祖先结点肯定存在，不用判断NULL情况
		if (p_node == p_p_node->lchild) //父节点是祖父结点的左孩子
		{
			uncle_node = p_p_node->rchild; //祖父结点的右孩子-叔叔结点
		}
		else
		{
			uncle_node = p_p_node->lchild; //祖父结点的右孩子-叔叔结点			
		}

		if (uncle_node->color == RED) //1 .叔叔是红色
		{
			p_node->color = BLACK; //将“父节点”设为黑色。
			uncle_node->color = BLACK; //将“叔叔节点”设为黑色。
			p_p_node->color = RED; //将“祖父节点”设为“红色”。
			current_node = p_p_node;
		}
		else //叔叔是黑色
		{
			if (current_node == p_node->rchild)
			{
				current_node = p_node;
				if (p_p_node->lchild == p_node)
				{
					p_p_node->lchild = ll_rotate(current_node); //左旋
				}
				else
				{
					p_p_node->rchild = ll_rotate(current_node);
				}
			}
			else
			{
				p_node->color = BLACK;
				p_p_node->color = RED;
				if (p_p_node->parent->lchild == p_node)
				{
					p_p_node->parent->lchild = rr_rotate(p_p_node); 
				}
				else
				{
					p_p_node->parent->rchild = rr_rotate(p_p_node);
				}				
			}
		}
	}
	btree->root->color = BLACK;
	return btree->root;
}

binary_tree_node_t *binary_tree_rb_add(binary_tree_t *btree, void *data)
{
	binary_tree_node_t *node = binary_tree_add(btree, data);
	node->color = RED;

	if (node->parent != NULL && node->parent->color != BLACK)
	{
		make_tree_balance(btree, node);
	}
	return node;
}

binary_tree_node_t *binary_tree_rb_del(binary_tree_t *btree, void *data)
{
	binary_tree_node_t *sub_tree = binary_tree_del(btree, data);
	return make_tree_balance(btree, sub_tree);
}
