#include "binary_tree.h"

//add a node for AVL tree
static inline binary_tree_node_t *lr_rotate(binary_tree_t *btree, binary_tree_node_t *node)
{
  left(node) = binary_tree_left_rotate(btree, node->lchild);
  return binary_tree_right_rotate(btree, node);
}

static inline binary_tree_node_t *rl_rotate(binary_tree_t *btree, binary_tree_node_t *node)
{
  right(node) = binary_tree_right_rotate(btree, node->rchild);
  return binary_tree_left_rotate(btree, node);
}

static binary_tree_node_t *make_tree_balance(binary_tree_t *btree, binary_tree_node_t *node)
{
  int ldeepth = 0;
  int rdeepth = 0;
  binary_tree_node_t *unbalance_node = node;
  binary_tree_node_t *taller_child_node = NULL;
  binary_tree_node_t *taller_grandson_child = NULL;
  binary_tree_node_t *top = NULL;
  binary_tree_node_t *p_node = NULL;

  while (unbalance_node != NULL)
  {
    ldeepth = binary_tree_deepth_sub_tree(left(unbalance_node));
    rdeepth = binary_tree_deepth_sub_tree(right(unbalance_node));
    p_node = p(unbalance_node);
    if (ldeepth - rdeepth != 2 && ldeepth - rdeepth != -2)
    {
      unbalance_node = p_node;
      continue;
    }

    taller_child_node = ldeepth > rdeepth ? left(unbalance_node) : right(unbalance_node);
    ldeepth = binary_tree_deepth_sub_tree(left(taller_child_node));
    rdeepth = binary_tree_deepth_sub_tree(right(taller_child_node));
    taller_grandson_child = ldeepth > rdeepth ? left(taller_child_node) : right(taller_child_node);
#if TREE_DEBUG
    printf("unbalance node is...%d\n", *(int *)(unbalance_node->data));
#endif

    if (left(unbalance_node) == taller_child_node && left(taller_child_node) == taller_grandson_child)
    {
      top = binary_tree_right_rotate(btree, unbalance_node); //R rotate
    }
    else if (right(unbalance_node) == taller_child_node && right(taller_child_node) == taller_grandson_child)
    {
      top = binary_tree_left_rotate(btree, unbalance_node); //L rotate
    }
    else if (left(unbalance_node) == taller_child_node && right(taller_child_node) == taller_grandson_child)
    {
      top = lr_rotate(btree, unbalance_node); //LR rotate
    }
    else
    {
      top = rl_rotate(btree, unbalance_node); //RL rotate
    }
    unbalance_node = p_node;
#if TREE_DEBUG
    printf("after rotate...\n");
    print_ascii_tree(btree->root);
#endif
  }

  return btree->root;
}

binary_tree_node_t *binary_tree_avl_add(binary_tree_t *btree, void *data)
{
  binary_tree_node_t *node = binary_tree_add(btree, data);
  make_tree_balance(btree, node);
  return node;
}

binary_tree_node_t *binary_tree_avl_del(binary_tree_t *btree, void *data)
{
  binary_tree_node_t *sub_tree = binary_tree_del(btree, data);
  return make_tree_balance(btree, sub_tree);
}
