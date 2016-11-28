#include "binary_tree.h"

//add a node for AVL tree

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
	binary_tree_node_t *cur_node = node;
	binary_tree_node_t *uncle_node = NULL;
	while (p(cur_node) && color(p(cur_node)) == RED)
	{
		if (p(p(cur_node)) == NULL)
		{
			break;
		}

		if (p(cur_node) == left(p(p(cur_node)))) //父节点是祖父结点的左孩子
		{
			uncle_node = right(p(p(cur_node))); //祖父结点的右孩子-叔叔结点
			if (uncle_node != NULL && color(uncle_node) == RED) //1 .叔叔是红色
			{
				color(p(cur_node)) = BLACK; //将“父节点”设为黑色。
				if (uncle_node != NULL)
				{
					color(uncle_node) = BLACK; //将“叔叔节点”设为黑色。	
				}
				color(p(p(cur_node))) = RED; //将“祖父节点”设为“红色”。
				cur_node = p(p(cur_node));
			}
			else //叔叔是黑色
			{
				if (cur_node == right(p(cur_node)))
				{
					cur_node = p(cur_node);
					binary_tree_left_rotate(btree, cur_node);
				}
				else
				{
					color(p(cur_node)) = BLACK;
					color(p(p(cur_node))) = RED;
					binary_tree_right_rotate(btree, p(p(cur_node)));
				}
			}
		}
		else
		{
			uncle_node = left(p(p(cur_node))); //祖父结点的右孩子-叔叔结点
			if (uncle_node != NULL && color(uncle_node) == RED) //1 .叔叔是红色
			{
				color(p(cur_node)) = BLACK; //将“父节点”设为黑色。
				color(uncle_node) = BLACK; //将“叔叔节点”设为黑色。	
				color(p(p(cur_node))) = RED; //将“祖父节点”设为“红色”。
				cur_node = p(p(cur_node));
			}
			else //叔叔是黑色
			{
				if (cur_node == left(p(cur_node)))
				{
					cur_node = p(cur_node);
					binary_tree_right_rotate(btree, cur_node);
				}
				else
				{
					color(p(cur_node)) = BLACK;
					color(p(p(cur_node))) = RED;
					binary_tree_left_rotate(btree, p(p(cur_node)));
				}
			}
		}
	}
	btree->root->color = BLACK;
	return btree->root;
}

/*
 RB-DELETE-FIXUP(T, x)
while x ≠ root[T] and color[x] = BLACK  
	do if x = left[p[x]]      
		  then w ← right[p[x]]                                             // 若 “x”是“它父节点的左孩子”，则设置 “w”为“x的叔叔”(即x为它父节点的右孩子)                                          
			   if color[w] = RED                                           // Case 1: x是“黑+黑”节点，x的兄弟节点是红色。(此时x的父节点和x的兄弟节点的子节点都是黑节点)。
				  then color[w] ← BLACK                        ▹  Case 1   //   (01) 将x的兄弟节点设为“黑色”。
					   color[p[x]] ← RED                       ▹  Case 1   //   (02) 将x的父节点设为“红色”。
					   LEFT-ROTATE(T, p[x])                    ▹  Case 1   //   (03) 对x的父节点进行左旋。
					   w ← right[p[x]]                         ▹  Case 1   //   (04) 左旋后，重新设置x的兄弟节点。
			   if color[left[w]] = BLACK and color[right[w]] = BLACK       // Case 2: x是“黑+黑”节点，x的兄弟节点是黑色，x的兄弟节点的两个孩子都是黑色。
				  then color[w] ← RED                          ▹  Case 2   //   (01) 将x的兄弟节点设为“红色”。
					   x ←  p[x]                               ▹  Case 2   //   (02) 设置“x的父节点”为“新的x节点”。
		else if color[right[w]] = BLACK                          // Case 3: x是“黑+黑”节点，x的兄弟节点是黑色；x的兄弟节点的左孩子是红色，右孩子是黑色的。
				then color[left[w]] ← BLACK          ▹  Case 3   //   (01) 将x兄弟节点的左孩子设为“黑色”。
					 color[w] ← RED                  ▹  Case 3   //   (02) 将x兄弟节点设为“红色”。
					 RIGHT-ROTATE(T, w)              ▹  Case 3   //   (03) 对x的兄弟节点进行右旋。
					 w ← right[p[x]]                 ▹  Case 3   //   (04) 右旋后，重新设置x的兄弟节点。
			  color[w] ← color[p[x]]                 ▹  Case 4   // Case 4: x是“黑+黑”节点，x的兄弟节点是黑色；x的兄弟节点的右孩子是红色的。(01) 将x父节点颜色 赋值给 x的兄弟节点。
			  color[p[x]] ← BLACK                    ▹  Case 4   //   (02) 将x父节点设为“黑色”。
			  color[right[w]] ← BLACK                ▹  Case 4   //   (03) 将x兄弟节点的右子节设为“黑色”。
			  LEFT-ROTATE(T, p[x])                   ▹  Case 4   //   (04) 对x的父节点进行左旋。
			  x ← root[T]                            ▹  Case 4   //   (05) 设置“x”为“根节点”。
else (same as then clause with "right" and "left" exchanged)        // 若 “x”是“它父节点的右孩子”，将上面的操作中“right”和“left”交换位置，然后依次执行。
color[x] ← BLACK
 
 */

static binary_tree_node_t *make_del_tree_balance(binary_tree_t *btree, binary_tree_node_t *node)
{
	binary_tree_node_t *sibling_node = NULL;
	binary_tree_node_t *current_node = node;

	while (current_node != btree->root && current_node->color == BLACK)
	{
		if (current_node == current_node->parent->lchild)
		{
			sibling_node = current_node->parent->rchild;
			if (sibling_node->color == RED)
			{
				sibling_node->color = BLACK;
				current_node->parent->color = RED;
				binary_tree_left_rotate(btree, current_node->parent->parent);
				sibling_node = current_node->parent->rchild;
			}

			if (sibling_node->lchild->color == BLACK && sibling_node->rchild->color == BLACK)
			{
				sibling_node->color = RED;
				current_node = current_node->parent;
			}
			else if (sibling_node->rchild->color == BLACK)
			{
				sibling_node->lchild->color = BLACK;
				sibling_node->color = RED;
				binary_tree_right_rotate(btree, sibling_node);
				sibling_node = current_node->parent->rchild;

				sibling_node->color = current_node->parent->color;
				current_node->parent->color = BLACK;
				binary_tree_left_rotate(btree, current_node->parent);
				current_node = btree->root;
			}
		}
		else
		{
			sibling_node = current_node->parent->lchild;
			if (sibling_node->color == RED)
			{
				sibling_node->color = BLACK;
				current_node->parent->color = RED;
				binary_tree_left_rotate(btree, current_node->parent);
				sibling_node = current_node->parent->lchild;
			}

			if (sibling_node->lchild->color == BLACK && sibling_node->rchild->color == BLACK)
			{
				sibling_node->color = RED;
				current_node = current_node->parent;
			}
			else if (sibling_node->lchild->color == BLACK)
			{
				sibling_node->rchild->color = BLACK;
				sibling_node->color = RED;
				binary_tree_right_rotate(btree, sibling_node);
				sibling_node = current_node->parent->lchild;

				sibling_node->color = current_node->parent->color;
				current_node->parent->color = BLACK;
				binary_tree_left_rotate(btree, current_node->parent);
				current_node = btree->root;
			}

		}
	}
	current_node->color = BLACK;
	return current_node;
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
	return make_del_tree_balance(btree, sub_tree);
}
