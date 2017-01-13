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

static int printf_node_info(b_tree_node_t *node)
{
   if (node != NULL)
   {
      printf("address %x ", (int)node);
      printf("key_num:%lu, child_index:%d, leaf:%d ", node->key_num, node->child_index, node->leaf);
      printf("key:");
      for (int i = 0; i < node->key_num; i++)
      {
         printf("%d ", *(int *)node->key[i]);
      }
   }
   else
   {
      printf("node NULL\n");
   }

   return 0;
}

static int traverse(void *a)
{
   if (a == NULL)
   {
      printf("NULL ");
      return 0;
   }
   printf("%d ", *(int *)a);
   return 0;
}

static int check_child_index(b_tree_t *btree, b_tree_node_t *node)
{
   if (node != NULL && !is_leaf_node(node))
   {
      for (int i = 0; i <= node->key_num; i++)
      {
         b_tree_node_t *child_node = (b_tree_node_t *)node->data[i];
         if (child_node->child_index != i)
         {
            printf("hahahah...\n");
            printf_node_info(node);
            printf("\n");
            printf_node_info(child_node);
            printf("\n");
            b_tree_bfs_traversal_subtree(btree->root, traverse);
         }
         assert(child_node->child_index == i);
      }
   }
   return 0;
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

static inline b_tree_node_t *create_b_tree_root(b_tree_t *btree, void *key, b_tree_node_t *child1, b_tree_node_t *child2)
{
   b_tree_node_t *parent = create_b_tree_node(btree);
   parent->leaf = 0;
   btree->root = parent;
   parent->data[0] = child1;
   parent->data[1] = child2;
   set_node_parent(child1, parent);
   set_node_parent(child2, parent);
   set_node_child_index(child1, 0);
   set_node_child_index(child2, 1);
   assign_node_key(parent->key[0], key);
   parent->key_num++;

   return parent;
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
   *index = iret <= 0 ? mid : mid + 1;
   return iret;
}

static inline int find_key(b_tree_t *btree, void *key, b_tree_node_t **found_node, int *key_index, compare_func_t compare)
{
   b_tree_node_t *node = btree->root;
   int index = 0;
   int iret = 1;
   *found_node = node;
   *key_index = 0;

   while (node != NULL)
   {
      index = 0;
      iret = binary_search(node->key, node->key_num, key, key_index, compare);
      *found_node = node;
      if (!is_leaf_node(node))
      {
         if (iret != 0)
         {
            node = node->data[*key_index];
         }
         else
         {
            node = node->data[*key_index + 1];
         }
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
static inline int insert_key_data_to_leaf_node(b_tree_node_t *node, void *key, void *data, int index)
{
   for (int m = node->key_num; m > index; m--) //移动关键字
   {
      assign_node_key(node->key[m], node->key[m - 1]);
      node->data[m] = node->data[m - 1];
   }

   assign_node_key(node->key[index], key);
   node->key_num++;
   node->data[index] = data; //set data
   return index;
}

//index是插入的位置
static inline int insert_key_to_tree_node(b_tree_node_t *node, void *key, void *data, int index)
{
   for (int m = node->key_num; m > index; m--) //移动关键字
   {
      assign_node_key(node->key[m], node->key[m - 1]);
      node->data[m + 1] = node->data[m];
      set_node_child_index(node->data[m + 1], m + 1);
   }
   node->data[index + 1] = node->data[index];
   set_node_child_index(node->data[index + 1], index + 1);

   assign_node_key(node->key[index], key);
   node->key_num++;
   set_node_parent(data, node);
   set_node_child_index(data, index);
   node->data[index] = data; //set child
   return index;
}

static inline int del_key_from_tree_node(b_tree_node_t *node, int key_index)
{
   for (int i = key_index; i < node->key_num; i++)
   {
      assign_node_key(node->key[i], node->key[i + 1]);
      node->data[i] = node->data[i + 1];
      if (!is_leaf_node(node))
      {
         set_node_child_index(node->data[i], i);
      }
   }
   node->data[node->key_num] = NULL;
   node->key_num--;
   return 0;
}

static inline b_tree_node_t *split_node_key_data(b_tree_t *btree, b_tree_node_t *node, int split_point)
{
   b_tree_node_t *new = create_b_tree_node(btree);
   new->leaf = node->leaf;
   new->key_num = node->key_num - split_point;
   node->key_num = node->key_num / 2;
   memcpy(new->key, node->key + split_point, key_size * new->key_num); //cp key
   memcpy(new->data, node->data + split_point, sizeof(void *) * (new->key_num + 1));
   memset(node->data + split_point, 0, sizeof(void *) * (new->key_num + 1));

   return new;
}

static inline b_tree_node_t *split_non_leaf_node(b_tree_t *btree, b_tree_node_t *node, int split_point)
{
   b_tree_node_t *new = split_node_key_data(btree, node, split_point);
   for (int i = 0; i <= new->key_num; i++)
   {
      set_node_parent(new->data[i], new);
      set_node_child_index(new->data[i], i);
   }
   return new;
}

static inline b_tree_node_t *split_leaf_node(b_tree_t *btree, b_tree_node_t *node, int split_point)
{
   b_tree_node_t *next_node = node->data[btree->order];
   b_tree_node_t *new = split_node_key_data(btree, node, split_point);
   new->data[btree->order] = next_node;
   node->data[btree->order] = new;
   return new;
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

int b_plus_tree_add_key_int(b_tree_t *btree, int key, void *data)
{
   b_tree_node_t *insert_node = NULL;
   int found_key_index = 0;
   int iret = find_key(btree, (void *)&key, &insert_node, &found_key_index, compare_int);
   b_tree_node_t *next_node = insert_node->data[btree->order];

   if (iret != 0) //not found
   {
      insert_key_data_to_leaf_node(insert_node, (void *)&key, data, found_key_index);
   }
   b_tree_node_t *cur_node = insert_node;
   while (cur_node->key_num == btree->order) //split node
   {
      int child_index = find_child_index(cur_node);
      key_t middle_key = cur_node->key + cur_node->key_num / 2;
      b_tree_node_t *parent = cur_node->parent;

      int split_point = 0;
      b_tree_node_t *new = NULL;
      if (!is_leaf_node(cur_node))
      {
         split_point = cur_node->key_num / 2 + 1;
         new = split_non_leaf_node(btree, cur_node, split_point);
      }
      else
      {
         split_point = cur_node->key_num / 2;
         new = split_leaf_node(btree, cur_node, split_point);
      }

      if (parent == NULL)
      { //root node is full, create new root node, the height of the tree +1
         parent = create_b_tree_root(btree, (void *)middle_key, cur_node, new);
      }
      else
      {
         parent->data[child_index] = new;
         set_node_parent(new, parent);
         insert_key_to_tree_node(parent, (void *)middle_key, cur_node, child_index);
      }
#if DEBUG
      printf("\nnew info:");
      printf_node_info(new);
      printf("\nur_node info:");
      printf_node_info(cur_node);
      printf("\nparent info:");
      printf_node_info(cur_node->parent);
      printf("\n");
#endif
      cur_node = parent;
   }
   return 0;
}

static inline b_tree_node_t *delete_key_from_tree(b_tree_t *btree, void *key, compare_func_t compare)
{
   int key_index = 0;
   b_tree_node_t *node = NULL;
   int found_key_index = 0;
   int iret = find_key(btree, key, &node, &key_index, compare);
   if (iret == 0)
   {
      del_key_from_tree_node(node, key_index);
   }

   return node;
}

static inline b_tree_node_t *merge_node(b_tree_t *btree, b_tree_node_t *cur_node, int child_index, b_tree_node_t *l_sibling, b_tree_node_t *r_sibling)
{
   b_tree_node_t *parent = cur_node->parent;
   b_tree_node_t *merge_node = NULL;
   b_tree_node_t *merged_node = NULL;
   if (l_sibling != NULL)
   {
      merge_node = l_sibling;
      merged_node = cur_node;
   }
   else
   {
      merge_node = cur_node;
      merged_node = r_sibling;
   }

#if DEBUG
   printf("\nmerge_node info:");
   printf_node_info(merge_node);
   printf("\nmerged_node info:");
   printf_node_info(merged_node);
   printf("\nparent info:");
   printf_node_info(merged_node->parent);
   printf("\n");
#endif

   int merge_child_index = merged_node->child_index - 1;

   if (!is_leaf_node(merged_node))
   {
      assign_node_key(merge_node->key[merge_node->key_num++], parent->key[merge_child_index]);
      memcpy(merge_node->key + merge_node->key_num, merged_node->key, key_size * merged_node->key_num);
      memcpy(merge_node->data + merge_node->key_num, merged_node->data, sizeof(void *) * (merged_node->key_num + 1));
      for (int i = 0; i <= merged_node->key_num; i++)
      {
         set_node_parent(merged_node->data[i], merge_node);
         set_node_child_index(merged_node->data[i], i + merge_node->key_num);
      }
   }
   else
   {
      memcpy(merge_node->key + merge_node->key_num, merged_node->key, key_size * merged_node->key_num);
      memcpy(merge_node->data + merge_node->key_num, merged_node->data, sizeof(void *) * merged_node->key_num);
      merge_node->data[btree->order] = merged_node->data[btree->order];
   }

   merge_node->key_num += merged_node->key_num;
   merge_node->parent->data[merge_child_index + 1] = merge_node;
   del_key_from_tree_node(merge_node->parent, merge_child_index);
   release_b_tree_node(merged_node);
   return merge_node;
}

int b_plus_tree_del_key_int(b_tree_t *btree, int key)
{
   int key_index = 0;
   b_tree_node_t *cur_node = NULL;
   int found_key_index = 0;
   cur_node = delete_key_from_tree(btree, &key, compare_int);
   while ((cur_node != NULL &&
           cur_node->key_num < (ceil(btree->order, 2) - 1) &&
           cur_node->parent != NULL))
   {
      int child_index = find_child_index(cur_node);
      b_tree_node_t *l_sibling = child_index > 0 ? cur_node->parent->data[child_index - 1] : NULL;
      b_tree_node_t *r_sibling = child_index < cur_node->parent->key_num ? cur_node->parent->data[child_index + 1] : NULL;
#if DEBUG
      printf("\nbefore...");
      printf("\ncur_node info:");
      printf_node_info(cur_node);
      printf("\nl_sibling info:");
      printf_node_info(l_sibling);
      printf("\nr_sibling info:");
      printf_node_info(r_sibling);
      printf("\nparent info:");
      printf_node_info(cur_node->parent);
      printf("\n");
#endif
      if (l_sibling != NULL && l_sibling->key_num >= ceil(btree->order, 2)) //borrow key from left sibling
      {
         if (!is_leaf_node(cur_node))
         {
            insert_key_to_tree_node(cur_node, (void *)cur_node->parent->key[child_index - 1], l_sibling->data[l_sibling->key_num], 0);
            assign_node_key(cur_node->parent->key[child_index - 1], (void *)l_sibling->key[l_sibling->key_num - 1]);
         }
         else
         {
            assign_node_key(cur_node->parent->key[child_index - 1], (void *)l_sibling->key[l_sibling->key_num - 1]);
            insert_key_data_to_leaf_node(cur_node, (void *)cur_node->parent->key[child_index - 1], l_sibling->data[l_sibling->key_num], 0);
         }
         clear_node_key(l_sibling->key + l_sibling->key_num - 1);
         l_sibling->data[l_sibling->key_num] = NULL;
         l_sibling->key_num--;

#if DEBUG
         check_child_index(btree, cur_node);
         check_child_index(btree, cur_node->parent);
#endif
      }
      else if (r_sibling != NULL && r_sibling->key_num >= ceil(btree->order, 2)) //borrow key from right sibling
      {
         if (!is_leaf_node(cur_node))
         {
            assign_node_key(cur_node->key[cur_node->key_num++], cur_node->parent->key[child_index]);
            assign_node_key(cur_node->parent->key[child_index], r_sibling->key[0]);
            cur_node->data[cur_node->key_num] = r_sibling->data[0];
            set_node_parent(cur_node->data[cur_node->key_num], cur_node);
            set_node_child_index(cur_node->data[cur_node->key_num], cur_node->key_num);
         }
         else
         {
            // assign_node_key(cur_node->key[cur_node->key_num], cur_node->parent->key[child_index]);
            assign_node_key(cur_node->key[cur_node->key_num], r_sibling->key[0]);
            assign_node_key(cur_node->parent->key[child_index], r_sibling->key[1]);
            cur_node->data[cur_node->key_num++] = r_sibling->data[0];
         }
         del_key_from_tree_node(r_sibling, 0);
#if DEBUG
         check_child_index(btree, cur_node);
         check_child_index(btree, cur_node->parent);
#endif
      }
      else //合并
      {
         printf("merge..\n");
         b_tree_node_t *merged_node = merge_node(btree, cur_node, child_index, l_sibling, r_sibling);
#if DEBUG
         printf("merge step..\n");
         b_tree_bfs_traversal_subtree(btree->root, traverse);
         printf("\n");
         check_child_index(btree, merged_node);
         check_child_index(btree, merged_node->parent);
#endif
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
#if 0//DEBUG
      printf("\nafter...");
      printf("\ncur_node info:");
      printf_node_info(cur_node);
      printf("\nl_sibling info:");
      printf_node_info(l_sibling);
      printf("\nr_sibling info:");
      printf_node_info(r_sibling);
      printf("\nparent info:");
      printf_node_info(cur_node->parent);
      printf("\n");
#endif
   }
   return 0;
}
