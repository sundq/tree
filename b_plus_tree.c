#include "./b_tree.h"
#include "./util/memory_pool.h"

static int compare_int(void *a, void *b)
{
   int a1 = *(int *)a;
   int b1 = *(int *)b;
   return a1 - b1;
}

static int compare_str(void *a, void *b)
{
   return strcmp((char *)a, (char *)b);
}

static inline b_tree_node_t *create_b_tree_node(b_tree_t *root)
{
   b_tree_node_t *node = (b_tree_node_t *)allocate_memory(sizeof(b_tree_node_t));
   node->parent = NULL;
   node->data = (void **)allocate_memory(sizeof(*node->data) * (root->order + 1));
   node->key = allocate_memory(key_size * root->order);
   node->key_num = 0;
   node->leaf = 1;
   node->child_index = 0;
   return node;
}

static inline b_tree_node_t *release_b_tree_node(b_tree_node_t *node)
{
   free(node->data);
   free(node->key);
   free(node);
   return 0;
}

static inline int binary_search(key_t array, int len, void *key, int *index, compare_func_t compare)
{
   int low = 0;
   int high = len - 1;
   int mid = 0;
   int iret = -1;
   while (low <= high && iret != 0)
   {
      mid = (low + high) / 2;
      iret = compare(key, array[mid]);
      if (iret > 0)
      {
         low = mid + 1;
      }
      else if (iret < 0)
      {
         high = mid - 1;
      }
   }
   *index = iret > 0 ? mid + 1 : mid;
   return iret;
}

static inline int find_key(b_tree_t *btree, void *key, b_tree_node_t **found_node, int *key_index, compare_func_t compare)
{
   b_tree_node_t *node = btree->root;
   int index = 0;
   int iret = 1;
   *found_node = node;
   *key_index = 0;

   while (node != NULL && iret != 0)
   {
      index = 0;
      iret = binary_search(node->key, node->key_num, key, key_index, compare);
      *found_node = node;
      if (!is_leaf_node(node))
      {
         node = node->data[*key_index];
      }
      else
      {
         node = NULL;
      }
   }
   return iret;
}

//查找结点是第几个孩子
static inline int find_child_index(b_tree_node_t *node)
{
   return node->child_index;
}

//index是插入的位置
static inline int insert_key_to_tree_node(b_tree_node_t *node, void *key, void *data, int index)
{
   for (int m = node->key_num; m > index; m--) //移动关键字
   {
      assign_node_key(node->key[m], node->key[m - 1]);
   }

   for (int i = node->key_num; i > index; i--) //移动数据或者孩子
   {
      node->data[i + 1] = node->data[i];
      if (!is_leaf_node(node)) //非叶子结点
      {
         set_node_child_index(node->data[i + 1], i + 1);
      }
   }
   assign_node_key(node->key[index], key);
   node->key_num++;

   if (!is_leaf_node(node)) //非叶子结点
   {
      node->data[index + 1] = data; //set child
      set_node_parent(data, node);
      set_node_child_index(data, index + 1);
   }
   else
   {
      node->data[index] = data; //set data
   }
   return index;
}

static inline int del_key_from_tree_node(b_tree_t *btree, b_tree_node_t *node, int key_index)
{
   for (int i = key_index; i < node->key_num; i++)
   {
      assign_node_key(node->key[i], node->key[i + 1]);
   }
   clear_node_key(node->key[node->key_num]);

   node->key_num--;
   return 0;
}

void *b_plus_tree_find_int(b_tree_t *btree, int key)
{
   void *found_key = NULL;
   b_tree_node_t *found_node = NULL;
   int found_key_index = 0;
   int iret = find_key(btree, (void *)&key, &found_node, &found_key_index, compare_int);
   found_key = iret == 0 ? (void *)found_node->key[found_key_index] : NULL;
   return found_key;
}

int b_plus_tree_add_node_int(b_tree_t *btree, int key, void *data)
{
   b_tree_node_t *insert_node = NULL;
   int found_key_index = 0;
   int iret = find_key(btree, (void *)&key, &insert_node, &found_key_index, compare_int);
   b_tree_node_t *next_node = insert_node->data[btree->order];

   if (iret != 0) //not found
   {
      insert_key_to_tree_node(insert_node, (void *)&key, data, found_key_index);
   }
   b_tree_node_t *cur_node = insert_node;
   while (cur_node->key_num == btree->order) //分裂
   {
      int child_index = find_child_index(cur_node);
      key_t middle_key = cur_node->key + cur_node->key_num / 2;
      b_tree_node_t *parent = cur_node->parent;
      b_tree_node_t *new = create_b_tree_node(btree);
      new->leaf = cur_node->leaf;
      int split_point = cur_node->key_num / 2 + !is_leaf_node(cur_node);
      new->key_num = cur_node->key_num - split_point;
      cur_node->key_num = cur_node->key_num / 2;
      memcpy(new->key, cur_node->key + split_point, key_size * new->key_num); //cp key
      memcpy(new->data, cur_node->data + split_point, sizeof(void *) * (new->key_num + 1));
      memset(cur_node->data + split_point, 0, sizeof(void *) * (new->key_num + 1));
      if (!is_leaf_node(new))
      {
         for (int i = 0; i <= new->key_num; i++)
         {
            {
               set_node_parent(new->data[i], new);
               set_node_child_index(new->data[i], i);
            }
         }
      }
      else
      {
         new->data[btree->order] = next_node;
         cur_node->data[btree->order] = new;
      }

      if (parent == NULL)
      {
         parent = create_b_tree_node(btree);
         parent->leaf = 0;
         btree->root = parent;
         parent->data[child_index] = cur_node;
         parent->data[child_index + 1] = new;
         set_node_parent(new, parent);
         set_node_parent(cur_node, parent);
         set_node_child_index(cur_node, child_index);
         set_node_child_index(new, child_index + 1);
         assign_node_key(parent->key[0], (void *)middle_key);
         parent->key_num++;
      }
      else
      {
         insert_key_to_tree_node(parent, (void *)middle_key, new, child_index);
      }
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

   assign_node_key(merge_node->key[merge_node->key_num++], parent->key[merge_child_index]); //移动父结点的关键字
   for (int i = merge_node->key_num, m = 0; m <= merged_node->key_num; i++, m++)            //合并孩子指针
   {
      merge_node->data[i] = merged_node->data[m];
      if (merge_node->data[i] != NULL)
      {
         set_node_parent(merge_node->data[i], merge_node);
         set_node_child_index(merge_node->data[i], i);
      }
   }
   memcpy(merge_node->key + merge_node->key_num, merged_node->key, sizeof(*(merged_node->key)) * merged_node->key_num); //合并关键字
   merge_node->key_num += merged_node->key_num;

   for (int i = merge_child_index + 1; i < parent->key_num; i++) //删除父结点一个孩子结点
   {
      parent->data[i] = parent->data[i + 1];
      set_node_child_index(parent->data[i], i);
   }
   parent->data[parent->key_num] = NULL;
   del_key_from_tree_node(btree, parent, merge_child_index);
   release_b_tree_node(merged_node);
   return merge_node;
}

int b_plus_tree_del_node_int(b_tree_t *btree, int key)
{
   int key_index = 0;
   b_tree_node_t *cur_node = NULL;
   int found_key_index = 0;
   int iret = find_key(btree, (void *)&key, &cur_node, &key_index, compare_int);
   if (iret == 0)
   {
      if (!is_leaf_node(cur_node)) //非叶子结点
      {
         b_tree_node_t *successor_node = cur_node->data[key_index + 1];
         while (successor_node->data[0] != NULL) //找后继结点
         {
            successor_node = successor_node->data[0];
         }
         assign_node_key(cur_node->key[key_index], successor_node->key[0]);
         cur_node = successor_node;
         key_index = 0;
      }
      del_key_from_tree_node(btree, cur_node, key_index);

      while ((cur_node != NULL &&
              cur_node->key_num < (ceil(btree->order, 2) - 1) &&
              cur_node->parent != NULL))
      {
         int child_index = find_child_index(cur_node);
         b_tree_node_t *l_sibling = child_index > 0 ? cur_node->parent->data[child_index - 1] : NULL;
         b_tree_node_t *r_sibling = child_index < cur_node->parent->key_num ? cur_node->parent->data[child_index + 1] : NULL;

         //借结点需要关键字和孩子结点一起借!!!
         if (l_sibling != NULL && l_sibling->key_num >= ceil(btree->order, 2)) //左兄弟丰满
         {
            for (int i = cur_node->key_num + 1; i > 0; i--) //先借孩子结点
            {
               cur_node->data[i] = cur_node->data[i - 1];
               set_node_child_index(cur_node->data[i], i);
            }
            cur_node->data[0] = l_sibling->data[l_sibling->key_num];
            if (cur_node->data[0] != NULL)
            {
               set_node_parent(cur_node->data[0], cur_node);
               set_node_child_index(cur_node->data[0], 0);
            }
            insert_key_to_tree_node(btree, cur_node, (void *)cur_node->parent->key[child_index - 1], 0);
            del_key_from_tree_node(btree, cur_node->parent, child_index - 1); //删除父结点的关键字
            insert_key_to_tree_node(btree, cur_node->parent, (void *)l_sibling->key[l_sibling->key_num - 1], child_index - 1);
            clear_node_key(l_sibling->key[l_sibling->key_num - 1]);
            l_sibling->key_num--;
         }
         else if (r_sibling != NULL && r_sibling->key_num >= ceil(btree->order, 2)) //右兄弟丰满
         {
            assign_node_key(cur_node->key[cur_node->key_num], cur_node->parent->key[child_index]);
            cur_node->key_num++;

            cur_node->data[cur_node->key_num] = r_sibling->data[0];
            if (cur_node->data[cur_node->key_num] != NULL)
            {
               set_node_parent(cur_node->data[cur_node->key_num], cur_node);
               set_node_child_index(cur_node->data[cur_node->key_num], cur_node->key_num);
            }

            for (int i = 0; i < r_sibling->key_num; i++)
            {
               r_sibling->data[i] = r_sibling->data[i + 1];
               set_node_child_index(r_sibling->data[i], i);
            }
            r_sibling->data[r_sibling->key_num] = NULL;

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
