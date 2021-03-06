#include "binary_tree.h"
#include "./util/queue.h"

static int release_node(void *a)
{
  release_memory(a);
  return 0;
}

static int reset_height_tree_node(binary_tree_node_t *node)
{
  binary_tree_node_t *cur_node = node;
  while (p(cur_node) != NULL)
  {
    h(p(cur_node)) = h(cur_node) + 1;
    cur_node = p(cur_node);
  }

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

  h(node) = max(binary_tree_deepth_sub_tree(node->lchild), binary_tree_deepth_sub_tree(node->rchild)) + 1;
  reset_height_tree_node(node);
  return top;
}

binary_tree_node_t *binary_tree_left_rotate(binary_tree_t *btree, binary_tree_node_t *node)
{
  binary_tree_node_t *top = right(node);
  binary_tree_node_t *p_node = p(node);

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

  h(node) = max(binary_tree_deepth_sub_tree(node->lchild), binary_tree_deepth_sub_tree(node->rchild)) + 1;
  reset_height_tree_node(node);
  return top;
}

binary_tree_t *binary_tree_init(compare_func_t func)
{
  binary_tree_t *btree = (binary_tree_t *)(allocate_memory(sizeof(binary_tree_t)));
  btree->root = NULL;
  btree->compare_func = func;
  btree->size = 0;
  btree->deep = 0;
  return btree;
}

binary_tree_node_t *binary_tree_add(binary_tree_t *btree, void *data)
{
  binary_tree_node_t **cur_node = &btree->root;
  binary_tree_node_t *parent_node = btree->root;
  while (*cur_node != NULL)
  {
    parent_node = *cur_node;
    if (btree->compare_func(data, (*cur_node)->data) > 0) //rchild child
    {
      cur_node = &(*cur_node)->rchild;
    }
    else if (btree->compare_func(data, (*cur_node)->data) < 0) //lchild child
    {
      cur_node = &(*cur_node)->lchild;
    }
    else
    {
      //the key have existed
      break;
    }
  }

  if (*cur_node == NULL)
  {
    *cur_node = (binary_tree_node_t *)(allocate_memory(sizeof(binary_tree_node_t)));
    (*cur_node)->data = data;
    (*cur_node)->height = 1;
    (*cur_node)->parent = parent_node;
  }
  reset_height_tree_node(*cur_node);
  return *cur_node;
}

binary_tree_node_t *binary_tree_get(binary_tree_t *btree, void *data)
{
  binary_tree_node_t *cur_node = btree->root;
  while (cur_node != NULL)
  {
    if (btree->compare_func(data, cur_node->data) > 0) //rchild child
    {
      cur_node = cur_node->rchild;
    }
    else if (btree->compare_func(data, cur_node->data) < 0) //lchild child
    {
      cur_node = cur_node->lchild;
    }
    else
    {
      return cur_node;
    }
  }
  return NULL;
}

//中序遍历

int binary_tree_inorder_traversal(binary_tree_t *btree, traversal_callback cb)
{
  binary_tree_node_t *cur_node = btree->root;
  binary_tree_node_t *prev_node = NULL;
  while (cur_node != NULL)
  {
    if (cur_node->lchild == NULL) //the lchild child is NULL, process current node and set current_node to rchild child
    {
      cb(cur_node->data);
      cur_node = cur_node->rchild;
    }
    else
    {
      prev_node = cur_node->lchild;
      while (prev_node->rchild != NULL && prev_node->rchild != cur_node)
      {
        prev_node = prev_node->rchild;
      }

      if (prev_node->rchild == NULL)
      {
        prev_node->rchild = cur_node;
        cur_node = cur_node->lchild;
      }
      else
      {
        prev_node->rchild = NULL;
        cb(cur_node->data);
        cur_node = cur_node->rchild;
      }
    }
  }

  return 0;
}

int binary_tree_bfs_traversal_subtree(binary_tree_node_t *subtree, traversal_callback cb)
{
  int cur_num = 1;  //num of nodes lchild in current level
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
    binary_tree_node_t *node = (binary_tree_node_t *)queue_pop(q);
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
  return subtree != NULL ? subtree->height : 0;
}

binary_tree_node_t *binary_tree_del(binary_tree_t *btree, void *data)
{
  binary_tree_node_t *node = binary_tree_get(btree, data);
  binary_tree_node_t *parent_successor_node = NULL;
  binary_tree_node_t *successor_node = node;
  binary_tree_node_t *successor_child_node = NULL;
  binary_node_color_t del_color = 0;
  if (node != NULL)
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
    successor_child_node->parent = parent_successor_node;

    node->data = successor_node->data;
    del_color = color(successor_node);
    release_memory(successor_node);
  }

  return parent_successor_node;
}

int binary_tree_destory(binary_tree_t *btree)
{
  binary_tree_bfs_traversal_subtree(btree->root, release_node);
  return 0;
}
