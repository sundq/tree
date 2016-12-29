#include "./b_tree.h"
#include "./util/memory_pool.h"

static inline b_tree_node_t *create_b_tree_node(b_tree_t *root)
{
   b_tree_node_t *node = (b_tree_node_t *)allocate_memory(sizeof(b_tree_node_t));
   node->parent = NULL;
   node->child = (b_tree_node_t **)allocate_memory(sizeof(b_tree_node_t) * (root->order + 1));
   node->key = allocate_memory(sizeof(*(node->key)) * root->order);
   node->key_num = 0;
   node->leaf = 1;
   node->child_index = -1;
   return node;
}

static inline b_tree_node_t *release_b_tree_node(b_tree_node_t *node)
{
   free(node->child);
   free(node->key);
   free(node);
   return 0;
}

static inline int find_key(b_tree_t *btree, void *key, b_tree_node_t **found_node, int *key_index)
{
   b_tree_node_t *node = btree->root;
   *found_node = node;
   *key_index = 0;
   int index = 0;
   int iret = 1;
   while (node != NULL && iret != 0)
   {
      index = 0;
      iret = 1;
      while (iret > 0 && index < node->key_num) //break if iret <= 0, means find the key or the insert node
      {
         if ((void *)node->key[index] == NULL)
         {
            printf("%d %lu\n", index, node->key_num); 
            exit(1);    
         }
         iret = btree->compare(key, (void *)node->key[index]);
         index += (iret > 0 ? 1 : 0);
      }
      *key_index = index;
      *found_node = node;
      node = node->child[index];
   }
   return iret;
}

//查找结点是第几个孩子
static inline int find_child_index(b_tree_node_t *node)
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

//index是插入的位置
static inline int insert_key_to_tree_node(b_tree_t *btree, b_tree_node_t *node, void *key, int index)
{
   for (int m = node->key_num; m > index; m--)
   {
      node->key[m] = node->key[m - 1];
   }
   node->key[index] = (intptr_t)key;
   node->key_num++;
   return index;
}

static inline int del_key_from_tree_node(b_tree_t *btree, b_tree_node_t *node, int key_index)
{
   for (int i = key_index; i < node->key_num; i++)
   {
      node->key[i] = node->key[i + 1];
   }
   node->key[node->key_num] = 0;
   node->key_num--;
   return 0;
}

//m为b tree的阶
b_tree_t *b_tree_init(size_t m, compare_func_t func)
{
   b_tree_t *btree = (b_tree_t *)allocate_memory(sizeof(b_tree_t));
   btree->compare = func;
   btree->order = m;
   btree->root = create_b_tree_node(btree);
   return btree;
}

int b_tree_bfs_traversal_subtree(b_tree_node_t *subtree, traversal_callback cb)
{
   int cur_num = 1;  //num of nodes lchild in current level
   int next_num = 0; //num of nodes in next level
   int level = 0;
   if (subtree == NULL)
   {
      return level;
   }

   queue_t *q = queue_init(1024);
   b_tree_node_t *p = subtree;
   queue_push(q, subtree);
   while (!queue_is_empty(q))
   {
      b_tree_node_t *node = (b_tree_node_t *)queue_pop(q);

      if (node == NULL)
      {
         continue;
      }
      if (p != node->parent)
      {
         printf("*");
      }

      if (cb != NULL)
      {
         for (int i = 0; i < node->key_num; i++)
         {
            //printf("(%lu)", ((size_t)node->parent) % 100);
            cb((void *)node->key[i]);
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
      p = node->parent;
   }
   queue_destroy(q);
   return level;
}

void *b_tree_find(b_tree_t *btree, void *key)
{
   void *found_key = NULL;
   b_tree_node_t *found_node = NULL;
   int found_key_index = 0;
   int iret = find_key(btree, key, &found_node, &found_key_index);
   found_key = iret == 0 ? (void *)found_node->key[found_key_index] : NULL;
   return found_key;
}

int b_tree_add_node(b_tree_t *btree, void *key)
{
   b_tree_node_t *insert_node = NULL;
   int found_key_index = 0;
   int iret = find_key(btree, key, &insert_node, &found_key_index);
   if (iret != 0) //not found
   {
      insert_key_to_tree_node(btree, insert_node, key, found_key_index);
   }
   b_tree_node_t *cur_node = insert_node;
   while (cur_node->key_num == btree->order) //分裂
   {
      int child_index = find_child_index(cur_node);
      intptr_t middle_key = cur_node->key[cur_node->key_num / 2];
      b_tree_node_t *parent = cur_node->parent;
      b_tree_node_t *new = create_b_tree_node(btree);
      int i = cur_node->key_num / 2 + 1;
      int m = 0;
      for (i = cur_node->key_num / 2 + 1, m = 0; i <= cur_node->key_num; i++, m++)
      {
         if (i < cur_node->key_num)
         {
            new->key[m] = cur_node->key[i];
            new->key_num++;
            cur_node->key[i] = 0;
         }
         new->child[m] = cur_node->child[i];
         if (new->child[m] != NULL)
         {
            new->child[m]->parent = new;
            new->leaf = 0;
         }
         cur_node->child[i] = NULL;
      }
      cur_node->key_num = cur_node->key_num / 2;
      if (parent == NULL)
      {
         parent = create_b_tree_node(btree);
         parent->leaf = 0;
         btree->root = parent;
      }
      new->parent = parent;
      cur_node->parent = parent;
      insert_key_to_tree_node(btree, parent, (void *)middle_key, child_index);
      for (int i = parent->key_num + 1; i > child_index + 1; i--)
      {
         parent->child[i] = parent->child[i - 1];
      }
      parent->child[child_index] = cur_node;
      parent->child[child_index + 1] = new;
      cur_node = parent;
   }
   return 0;
}

static inline b_tree_node_t *merge_node(b_tree_t *btree, b_tree_node_t *cur_node, int child_index, b_tree_node_t *l_sibling, b_tree_node_t *r_sibling)
{
   b_tree_node_t *parent = cur_node->parent;
   b_tree_node_t *merge_node = NULL;
   b_tree_node_t *merged_node = NULL; //被合并的结点
   int merge_child_index = child_index;
   if (l_sibling != NULL) //左兄弟存在
   {
      merge_node = l_sibling;
      merged_node = cur_node;
      merge_child_index -= 1;
   }
   else
   {
      merge_node = cur_node;
      merged_node = r_sibling;
      merge_child_index = child_index;
   }

   merge_node->key[merge_node->key_num++] = parent->key[merge_child_index];         //移动父结点的关键字
   for (int i = merge_node->key_num, m = 0; m <= merged_node->key_num; i++, m++) //合并孩子指针
   {
      merge_node->child[i] = merged_node->child[m];
      if (merge_node->child[i] != NULL)
      {
         merge_node->child[i]->parent = merge_node;
      }
   }
   memcpy(merge_node->key + merge_node->key_num, merged_node->key, sizeof(*(merged_node->key)) * merged_node->key_num); //合并关键字
   merge_node->key_num += merged_node->key_num;

   for (int i = merge_child_index + 1; i < parent->key_num; i++) //删除父结点一个孩子结点
   {
      parent->child[i] = parent->child[i + 1];
   }
   parent->child[parent->key_num] = NULL;
   del_key_from_tree_node(btree, parent, merge_child_index);
   release_b_tree_node(merged_node);
   return merge_node;
}

int b_tree_del_node(b_tree_t *btree, void *key)
{
   int key_index = 0;
   b_tree_node_t *cur_node = NULL;
   int found_key_index = 0;
   int iret = find_key(btree, key, &cur_node, &key_index);
   if (iret == 0)
   {
      if (!is_leaf_node(cur_node)) //非叶子结点
      {
         b_tree_node_t *successor_node = cur_node->child[key_index + 1];
         while (successor_node->child[0] != NULL) //找后继结点
         {
            successor_node = successor_node->child[0];
         }
         cur_node->key[key_index] = successor_node->key[0];
         cur_node = successor_node;
         key_index = 0;
      }
      del_key_from_tree_node(btree, cur_node, key_index);

      while ((cur_node != NULL &&
              cur_node->key_num < (ceil(btree->order, 2) - 1) &&
              cur_node->parent != NULL))
      {
         int child_index = find_child_index(cur_node);
         b_tree_node_t *l_sibling = child_index > 0 ? cur_node->parent->child[child_index - 1] : NULL;
         b_tree_node_t *r_sibling = child_index < cur_node->parent->key_num ? cur_node->parent->child[child_index + 1] : NULL;

         //借结点需要关键字和孩子结点一起借!!!
         if (l_sibling != NULL && l_sibling->key_num >= ceil(btree->order, 2)) //左兄弟丰满
         {
            for (int i = cur_node->key_num + 1; i > 0; i--) //先借孩子结点
            {
               cur_node->child[i] = cur_node->child[i - 1];
            }
            cur_node->child[0] = l_sibling->child[l_sibling->key_num];
            if (cur_node->child[0] != NULL)
            {
               cur_node->child[0]->parent = cur_node;
            }
            insert_key_to_tree_node(btree, cur_node, (void *)cur_node->parent->key[child_index - 1], 0);
            del_key_from_tree_node(btree, cur_node->parent, child_index - 1); //删除父结点的关键字
            insert_key_to_tree_node(btree, cur_node->parent, (void *)l_sibling->key[l_sibling->key_num - 1], child_index - 1);
            l_sibling->key[l_sibling->key_num - 1] = 0;
            l_sibling->key_num--;
         }
         else if (r_sibling != NULL && r_sibling->key_num >= ceil(btree->order, 2)) //右兄弟丰满
         {
            cur_node->key[cur_node->key_num] = cur_node->parent->key[child_index];
            cur_node->key_num++;

            cur_node->child[cur_node->key_num] = r_sibling->child[0];
            if (cur_node->child[cur_node->key_num] != NULL)
            {
               cur_node->child[cur_node->key_num]->parent = cur_node;
            }

            for (int i = 0; i < r_sibling->key_num; i++)
            {
               r_sibling->child[i] = r_sibling->child[i + 1];
            }
            r_sibling->child[r_sibling->key_num] = NULL;

            del_key_from_tree_node(btree, cur_node->parent, child_index); //删除父结点的关键字
            insert_key_to_tree_node(btree, cur_node->parent, (void *)r_sibling->key[0], child_index);
            del_key_from_tree_node(btree, r_sibling, 0);
         }
         else //合并
         {
            b_tree_node_t *merged_node = merge_node(btree, cur_node, child_index, l_sibling, r_sibling);
            if (merged_node->parent != NULL && merged_node->parent->parent == NULL && merged_node->parent->key_num == 0)
            {
               btree->root = merged_node;
               release_b_tree_node(merged_node->parent); //树的高度降低
               merged_node->parent = NULL;
               cur_node = NULL;
            }
            else
            {
               cur_node = merged_node->parent;
            }
         }
      }
   }

   return 0;
}
